// £adowanie
LadujDoki()
{
	mysql_query("SELECT * FROM `Doki`");
	mysql_store_result();

	if(mysql_num_rows()==0)
		return print("[DOKI] Brak dokow do zaladowania.");

	new string[140];
	while(mysql_fetch_row(string, "|")) 
	{
    	sscanf(string, "e<p<|>ds[32]fffd>", LoadingPoints[ID_DOKOW]);

    	new str[80];
		format(str, sizeof str, "{F0F000}%s (%s) [bID: %d | sID: %d]", LoadingPoints[ID_DOKOW][PointName], KategorieTowarow[LoadingPoints[ID_DOKOW][pType]], LoadingPoints[ID_DOKOW][lID], ID_DOKOW);
		LoadingPointsText3D[ID_DOKOW]= CreateDynamic3DTextLabel(str, ZIELONY99, LoadingPoints[ID_DOKOW][PointPosX], LoadingPoints[ID_DOKOW][PointPosY], LoadingPoints[ID_DOKOW][PointPosZ]+0.5, 50.0);

		for(new playerid; playerid < MAX_PLAYERS; playerid++)
			LoadingPoinstMapIcon[ID_DOKOW][playerid] = CreateDynamicMapIcon(LoadingPoints[ID_DOKOW][PointPosX], LoadingPoints[ID_DOKOW][PointPosY], LoadingPoints[ID_DOKOW][PointPosZ], 51,  0, _, _, playerid, 1000.0);

		ID_DOKOW++;
	} 

	printf("[DOKI] Zaladowano %d dokow.", ID_DOKOW-1);
	mysql_free_result();

	return 1;
}

LadujTowary()
{
	mysql_query("SELECT `id`, `typ`, `stawka`, `minczas`, `poziom`, `adr`, `nazwa` FROM `Towary`");
	mysql_store_result();

	if(mysql_num_rows()==0)
		return print("[TOWARY] Brak towarow do zaladowania.");

	new id=1, string[140];
	while(mysql_fetch_row(string, "|")) 
	{
    	sscanf(string, "e<p<|>ddfddds[32]>", Towary[id]);
    	id++;
	} 

	printf("[TOWARY] Zaladowano %d towarow.", id);
	mysql_free_result();
	return 1;
}


// £adowanie
forward LoadVehicle(playerid);
public LoadVehicle(playerid)
{
	new string[124];

    if(GetPVarInt(playerid, "LadowanieCzas") == 0)
	{
		new towarid, waga, vehicleid;

		KillTimer(GetPVarInt(playerid, "LadowanieTimer"));
		DeletePVar(playerid, "LadowanieCzas");

		TogglePlayerControllable(playerid, true);

		ShowInfo(playerid, "Towar zosta³ za³adowany.");
		
		TextDrawShowForPlayer(playerid, CargoTextDraws[CargoTDBox]);
		PlayerTextDrawShow(playerid, CargoTextDraws[CargoTDInfo][playerid]);
		timer14[playerid] = SetTimerEx("redi", 1000, false, "d", playerid);
		
		SetPVarInt(playerid, "NieMozeRozladowac", 1);

		TextDrawShowForPlayer(playerid, Speedometer[CargoInfoTD]);
		PlayerTextDrawShow(playerid, Speedometer[VehicleCargo][playerid]);

		if(IsPlayerInTruck(playerid))
			vehicleid = GetVehicleTrailer(GetPlayerVehicleID(playerid));
		else
			vehicleid = GetPlayerVehicleID(playerid);

		towarid = vehOtherInfo[vehicleid][vCargoID];
		waga = vehOtherInfo[vehicleid][vCargoWeight]; 

		format(string,sizeof string,"%s~n~%dkg", Towary[towarid][NazwaTowaru], waga);
		PlayerTextDrawSetString(playerid, Speedometer[VehicleCargo][playerid], string);

		format(string, sizeof(string), "Za³adowano towar (vehicleUID: %d | vehicleid: %d | dokid: %d | towarid: %d | waga: %d)", Spawned[vehicleid] ? (-1) : vehInfo[DBVehID[vehicleid]][vID], vehicleid, GetPVarInt(playerid, "dokid"), towarid, waga);
		ToLog(PlayerInfo[playerid][pID], LOG_TYPE_PLAYER, string);
	}
	else
	{
		format(string, sizeof(string), "Ladowanie towaru %ds", GetPVarInt(playerid, "LadowanieCzas"));
		GameTextForPlayer(playerid, string, 1100, 3);

		printf(string);
		ShowInfo(playerid, string);

		SetPVarInt(playerid, "LadowanieCzas", GetPVarInt(playerid, "LadowanieCzas")-1);
	}

	return 1;
}

// Wy³adowanie
forward UnLoadVehicle(playerid, towarid, weight, vehicleid);
public UnLoadVehicle(playerid, towarid, weight, vehicleid)
{
	new string[256];

	new dokid = GetPVarInt(playerid, "dokid"), stacjaid = GetStacja(playerid);
    new Float:Money;
    new ile_da;

  	if(stacjaid != (-1))
  	{
  		new ile_potrzeba = 3000-Stacje[stacjaid][Zbiornik];
  		new ile_ma = floatround(weight/8);
		
  		if(ile_potrzeba < ile_ma)
  		{
  			format(string, sizeof(string), "%s~n~%dkg", Towary[vehOtherInfo[vehicleid][vCargoID]][NazwaTowaru], vehOtherInfo[vehicleid][vCargoWeight]);
  			vehOtherInfo[vehicleid][vCargoWeight] -= ile_potrzeba*8;

			PlayerTextDrawSetString(playerid, Speedometer[VehicleCargo][playerid], string);
			ile_da = 3000;
  		}
  		else
  		{
  			vehOtherInfo[vehicleid][vCargoID] = 0;
			vehOtherInfo[vehicleid][vCargoWeight] = 0;

			TextDrawHideForPlayer(playerid, Speedometer[CargoInfoTD]);
			PlayerTextDrawHide(playerid, Speedometer[VehicleCargo][playerid]);
			ile_da = ile_ma;
  		}

  		Stacje[stacjaid][Zbiornik] += ile_da;
		
		format(string, sizeof string, "{808040}STACJA BENZYNOWA {808040}[VID: {FFFFFF}%d{808040}]\n[ Zbiorniki: %d / 3000 ]\n{FFFFFF}/tankuj", stacjaid, Stacje[stacjaid][Zbiornik]);
		UpdateDynamic3DTextLabelText(StacjeText3D[stacjaid], 0xFFFFFFFF, string);

		strcat(string, "Roz³adowa³eœ na stacji.");
	}
  	else
  	{
  		vehOtherInfo[vehicleid][vCargoID] = 0;
		vehOtherInfo[vehicleid][vCargoWeight] = 0;

		TextDrawHideForPlayer(playerid, Speedometer[CargoInfoTD]);
		PlayerTextDrawHide(playerid, Speedometer[VehicleCargo][playerid]);

		strcat(string, "Roz³adowa³eœ w doku.");
  	}

	new Distance = GetDistanceFromMeToPoint(playerid, LoadingPoints[dokid][PointPosX], LoadingPoints[dokid][PointPosY], LoadingPoints[dokid][PointPosZ]);
	Money = Distance;
	Money *= Towary[towarid][Stawka];
	Money *= floatdiv(weight, 27000);

	if(Towary[towarid][Typ] == TYPE_ILLEGAL)
		GiveScore(playerid, 2);
	else
		GiveScore(playerid, 1);

	format(string, sizeof(string), "%s\n\nPrzjecha³eœ: %dj\nStan towaru: 100%\nNazwa towaru: %s\nWaga towaru: %dkg\nStawka: %0.2f\n\n", string, Distance, Towary[towarid][NazwaTowaru], weight, Towary[towarid][Stawka]);

	if(PlayerInfo[playerid][pFirma])
	{
   		format(string, sizeof(string), "%sNa konto firmowe wp³ynê³o: $%d", string, floatround(Money, floatround_ceil));
   		Firmy[PlayerInfo[playerid][pFirma]][tBank] += floatround(Money, floatround_ceil);
	}
	else
	{
		format(string, sizeof(string), "%sZa roz³adunek dosta³eœ: $%d", string, floatround(Money, floatround_ceil));
		GiveMoney(playerid, floatround(Money, floatround_ceil));
	}
	ShowInfo(playerid, string);

	format(string, sizeof(string), "Roz³adowano towar (vehicleUID: %d | vehicleid: %d | towarid: %d | roz³adowano: %d)", Spawned[vehicleid] ? (-1) : vehInfo[DBVehID[vehicleid]][vID], vehicleid, GetPVarInt(playerid, "dokid"), towarid, GetPVarInt(playerid, "w_stacji") ? weight : ile_da);
	ToLog(PlayerInfo[playerid][pID], LOG_TYPE_PLAYER, string);

   	DeletePVar(playerid, "dokid");
   	DeletePVar(playerid, "rozladowywuje");

	return 1;
}

// Komendy
CMD:zaladuj(playerid, params[])
{
	new dokid = GetPlayerLoadingPoint(playerid);
	new vehicleid;
	new string[1024];

	if(IsPlayerInTruck(playerid))
		vehicleid = GetVehicleTrailer(GetPlayerVehicleID(playerid));
	else
		vehicleid = GetPlayerVehicleID(playerid);


	if(!IsPlayerInAnyVehicle(playerid))
		return ShowInfo(playerid, "Nie jesteœ w pojeŸdzie.");

	if(dokid == (-1))
		return ShowInfo(playerid, "Nie jesteœ w doku za³adunkowym.");

	if(GetPVarInt(playerid, "Worked") > 0)
		return Msg(playerid, COLOR_ERROR, "Jesteœ aktualnie na s³u¿bie, nie mo¿esz ³adowaæ towarów.");

	if(IsPlayerInTruck(playerid))
	{
		if(!IsTrailerAttachedToVehicle(GetPlayerVehicleID(playerid)))
			return ShowInfo(playerid, "Nie posiadasz naczepy.");

		else if(!IsValidTrailerType(vehicleid, LoadingPoints[dokid][pType]))
		{
			format(string, sizeof(string), "Nie obs³ugujemy tej naczepy.\n\nKategoria towarów: %s\nModel naczepy: %d", KategorieTowarow[LoadingPoints[dokid][pType]], GetVehicleModel(vehicleid));
			return Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, " ", string, "Ok", "");
		}

		else if( vehOtherInfo[vehicleid][vCargoID] )
			return ShowInfo(playerid, "Jesteœ ju¿ za³adowany.");

		else if(GetPVarInt(playerid, "LadowanieCzas"))
			return ShowInfo(playerid, "£adujesz ju¿ towar.");
	}
	else if(IsPlayerInBus(playerid))
	{
		if( vehOtherInfo[vehicleid][vCargoID] )
			return ShowInfo(playerid, "Jesteœ ju¿ za³adowany.");
	}
	else
		return ShowInfo(playerid, "Nie obs³ugujemy tego pojazdu.");

	format(string, sizeof(string), "Towar\tStawka\tCzas\tPoziom\n");

	for(new i; i < MAX_TOWAROW; i++)
		if(Towary[i][tID] && Towary[i][Typ] == LoadingPoints[dokid][pType] && Towary[i][Poziom] <= GetPVarInt(playerid, "LEVEL"))
		{
			new h, m, s;
			ConvertSeconds(Towary[i][MinimalnyCzas], h, m, s);

			format(string, sizeof(string), "%s%s\t%0.2f\t%dm %ds\t%d\n", string, Towary[i][NazwaTowaru], Towary[i][Stawka], m, s, Towary[i][Poziom]);
		}

	SetPVarInt(playerid, "dokid", dokid);
	Dialog_Show(playerid, DIALOG_ID_LOAD_CARGO, DIALOG_STYLE_TABLIST_HEADERS, " ", string, "Za³aduj", "Anuluj");

	return 1;
}

CMD:rozladuj(playerid, params[])
{
	new LoadingPoint = GetPlayerLoadingPoint(playerid);
	new vehicleid = GetPlayerVehicleID(playerid);
	new towarid, waga;

	if(!IsPlayerInAnyVehicle(playerid))
		return ShowInfo(playerid, "Nie jesteœ w pojeŸdzie.");

	if(IsPlayerInTruck(playerid))
		vehicleid = GetVehicleTrailer(GetPlayerVehicleID(playerid));
	else
		vehicleid = GetPlayerVehicleID(playerid);

	if(IsPlayerInTruck(playerid))
	{
		if(!IsTrailerAttachedToVehicle(GetPlayerVehicleID(playerid)))
			return ShowInfo(playerid, "Nie posiadasz naczepy.");
	}
	else 
	{
		if(!IsPlayerInBus(playerid))
			return ShowInfo(playerid, "Nie obs³ugujemy tego pojazdu.");
	}

	towarid = vehOtherInfo[vehicleid][vCargoID];
	waga = vehOtherInfo[vehicleid][vCargoWeight];

	if(towarid == 0 || waga == 0)
		return SendClientMessage(playerid, LIGHTRED, "Nie jesteœ za³adowany.");

	if(GetPVarInt(playerid, "CzasDoRozladunku") > 0)
		return SendClientMessage(playerid, LIGHTRED, "Czas do roz³adunku jeszcze nie min¹³.");

	if(GetStacja(playerid) != (-1))
	{
		if(Towary[towarid][Typ] != TYPE_DANGEROUS)
			return ShowInfo(playerid, "Nie roz³adowaæ siê na stacji.");

		if(Stacje[GetPVarInt(playerid, "w_stacji")][Zbiornik] == 3000)
			return ShowInfo(playerid, "Ta stacja jest pe³na.");

		if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_PETROL)
			return ShowInfo(playerid, "Tylko Petrol Tank mo¿e dotankowywaæ stacje.");
	}
	else
	{
		if(LoadingPoint == (-1))
			return ShowInfo(playerid, "Nie jesteœ w doku roz³adunkowym.");
			
		if(GetPVarInt(playerid, "dokid") == LoadingPoint)
			return ShowInfo(playerid, "Nie mo¿esz siê roz³adowaæ w miejscu gdzie ³adowa³eœ towar");

		if(Towary[towarid][Typ] != LoadingPoints[LoadingPoint][pType])
		{
			new string[150];
			format(string, sizeof(string), "Nie mo¿esz tutaj siê roz³adowaæ.\nKategoria towaru: %s\nKategoria doku: %s", KategorieTowarow[Towary[towarid][Typ]], KategorieTowarow[LoadingPoints[LoadingPoint][pType]]);
			return ShowInfo(playerid, string);
		}
	}

	if(GetPVarInt(playerid, "rozladowywuje") == 1)
		return ShowInfo(playerid, "Ju¿ roz³adowywujesz towar.");

	SetPVarInt(playerid, "rozladowywuje", 1);
	SendClientMessage(playerid, LIGHTRED, "Trwa roz³adowywanie...");
	SetTimerEx("UnLoadVehicle", 10*1000, 0, "dddd", playerid, towarid, waga, vehicleid);
	return 1;
}
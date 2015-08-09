LadujStacje()
{
	mysql_query("SELECT * FROM `Stacje`");
	mysql_store_result();

	if(mysql_num_rows()==0)
		return print("[STACJE] Brak stacji do zaladowania.");

	new string[140];
	while(mysql_fetch_row(string, "|")) 
	{
    	sscanf(string, "e<p<|>dfffdddfff>", Stacje[ID_STACJA]);

    	format(string, sizeof string, "{808040}STACJA BENZYNOWA {808040}[VID: {FFFFFF}%d{808040}]\n{FFFFFF}/tankuj", ID_STACJA);
    	StacjeText3D[ID_STACJA] = CreateDynamic3DTextLabel(string, ZIELONY2, Stacje[ID_STACJA][sPosX], Stacje[ID_STACJA][sPosY], Stacje[ID_STACJA][sPosZ]+0.5, 30.0);
		StacjeMapIcon[ID_STACJA] = CreateDynamicMapIcon(Stacje[ID_STACJA][sPosX], Stacje[ID_STACJA][sPosY], Stacje[ID_STACJA][sPosZ], 55, 0, _, _, _, 300.0);
		
		StacjeMapIconForAll[ID_STACJA] = CreateDynamicMapIcon(Stacje[ID_STACJA][sPosX], Stacje[ID_STACJA][sPosY], Stacje[ID_STACJA][sPosZ], 55, 0, _, _, GetMaxPlayers()+1, 12000.0);
		Streamer_SetIntData(STREAMER_TYPE_MAP_ICON, StacjeMapIconForAll[ID_STACJA], E_STREAMER_STYLE, MAPICON_GLOBAL);

		ID_STACJA++;
	} 

	printf("[STACJE] Zaladowano %d stacji.", ID_STACJA);
	mysql_free_result();
	return 1;
}

CMD:tankuj(playerid, params[])
{
	if(!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie znajdujesz siê w pojeŸdzie.");

	new stacjaid = GetStacja(playerid);
	if(!stacjaid)
		return Msg(playerid, COLOR_ERROR, "Nie znajdujesz siê przy stacji.");

	new vehicleid=GetPlayerVehicleID(playerid), fueltype[8];

	switch( (Spawned[vehicleid]) ? (vehOtherInfo[vehicleid][vFuelType]) : (vehInfo[DBVehID[vehicleid]][vFuelType]) )
	{
		case FUEL_TYPE_GAS: strcat(fueltype, "Gaz");
		case FUEL_TYPE_BENZ: strcat(fueltype, "Benzyna");
		case FUEL_TYPE_TDI: strcat(fueltype, "Ropa");
	}
	
	Dialog_Show(playerid, DIALOG_TANKOWANIE_ILOSC, DIALOG_STYLE_TABLIST, "Stacja", "Zbiornik gazu:\t%dL\nZbiornik ropy:\t%dL\nZbiornik benzyny:\t%dL\n---\nCena gazu:\t$%.02f\nCena ropy:\t$%0.2f\nCena benzyny:\t$%0.2f\n---\nRodzaj paliwa pojazdu:\t%s", "Tankuj", "Anuluj", 
			Stacje[stacjaid][ZbiornikGaz], Stacje[stacjaid][ZbiornikRopa], Stacje[stacjaid][ZbiornikBenzyna], Stacje[stacjaid][CenaGaz], Stacje[stacjaid][CenaRopa], Stacje[stacjaid][CenaBenzyna], fueltype);

	return 1;
}

Dialog:DIALOG_TANKOWANIE_ILOSC(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new vehicleid=GetPlayerVehicleID(playerid), Float:mozeZatankowac=float(MaxFuel(GetVehicleModel(vehicleid)));

	if(Spawned[vehicleid])
		mozeZatankowac -= vehOtherInfo[vehicleid][vFuel];
	else
		mozeZatankowac -= vehInfo[DBVehID[vehicleid]][vFuel];

	if(mozeZatankowac == 0)
		return ShowInfo(playerid, "Nie mo¿esz zatankowaæ tego pojazdu - jest pe³en.");

	Dialog_Show(playerid, DIALOG_TANKOWANIE_TANKUJ, DIALOG_STYLE_INPUT, "Stacja", "Mo¿esz zatankowaæ: %0.2fL\n\nWpisz iloœæ paliwa w litrach (0, by do pe³na):", "Tankuj", "Anuluj", mozeZatankowac);

	return 1;
}

Dialog:DIALOG_TANKOWANIE_TANKUJ(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new Float:ileZatankowac = floatstr(inputtext), vehicleid = GetPlayerVehicleID(playerid), Float:mozeZatankowac=float(MaxFuel(GetVehicleModel(vehicleid))), stacjaid = GetStacja(playerid), koszt;

	if(Spawned[vehicleid])
		mozeZatankowac -= vehOtherInfo[vehicleid][vFuel];
	else
		mozeZatankowac -= vehInfo[DBVehID[vehicleid]][vFuel];

	if(ileZatankowac < 0)
		return Dialog_Show(playerid, DIALOG_TANKOWANIE_TANKUJ, DIALOG_STYLE_INPUT, "Stacja", "Nieprawid³owa iloœæ.\nMo¿esz zatankowaæ: %0.2fL\n\nWpisz iloœæ paliwa w litrach (0, by do pe³na):", "Tankuj", "Anuluj", mozeZatankowac);

	if(ileZatankowac == 0)
		ileZatankowac = mozeZatankowac;

	switch( (Spawned[vehicleid]) ? (vehOtherInfo[vehicleid][vFuelType]) : (vehInfo[DBVehID[vehicleid]][vFuelType]) )
	{
		case FUEL_TYPE_GAS: koszt = floatround(ileZatankowac * Stacje[stacjaid][CenaGaz]);
		case FUEL_TYPE_BENZ: koszt = floatround(ileZatankowac * Stacje[stacjaid][CenaBenzyna]);
		case FUEL_TYPE_TDI: koszt = floatround(ileZatankowac * Stacje[stacjaid][CenaRopa]);
	}

	if(GetMoney(playerid) < koszt) 
		return Dialog_Show(playerid, DIALOG_TANKOWANIE_TANKUJ, DIALOG_STYLE_INPUT, "Stacja", "Nie masz tylu pieniêdzy! Potrzebujesz: $%d\nMo¿esz zatankowaæ: %0.2fL\n\nWpisz iloœæ paliwa w litrach (0, by do pe³na):", "Tankuj", "Anuluj", ileZatankowac * Stacje[stacjaid][CenaGaz], mozeZatankowac);

	if(Spawned[vehicleid])
		vehOtherInfo[vehicleid][vFuel] += ileZatankowac;
	else
		vehInfo[DBVehID[vehicleid]][vFuel] += ileZatankowac;

	GiveMoney(playerid, -koszt);
	Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, "Stacja", "Zatankowa³eœ $%0.2fL za $%d.", "Ok", "", ileZatankowac, koszt);
	return 1;
}

CMD:stacjatp(playerid, params[])
{
	if(Stacje[strval(params)][sID] == 0)
		return 1;

	SetPlayerPos(playerid, Stacje[strval(params)][sPosX], Stacje[strval(params)][sPosY], Stacje[strval(params)][sPosZ]+3.0);
	return 1;
}

CMD:ustawzbiornik(playerid, params[])
{
	new vehicleid = GetPlayerVehicleID(playerid);

	if(!IsPlayerInAnyVehicle(playerid))
		return 1;

	if(Spawned[vehicleid])
		vehOtherInfo[vehicleid][vFuel] = strval(params);
	else
		vehInfo[DBVehID[vehicleid]][vFuel] = strval(params);

	return 1;
}
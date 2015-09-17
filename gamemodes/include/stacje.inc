LadujStacje()
{
	mysql_query("SELECT * FROM Stacje");
	mysql_store_result();

	if(mysql_num_rows()==0)
		return print("[STACJE] Brak stacji do zaladowania.");

	new string[140];
	while(mysql_fetch_row(string, "|")) 
	{
    	sscanf(string, "e<p<|>dfffdddfff>", stationInfo[ID_STACJA]);

    	format(string, sizeof string, "Stacja paliwowa w {b}%s{/b} (ID: {b}%d{/b})\nAby zatankowaæ wpisz {b}/tankuj{/b}", GetPointZone(stationInfo[ID_STACJA][sPosX], stationInfo[ID_STACJA][sPosY]), ID_STACJA);
    	StacjeText3D[ID_STACJA] = CreateDynamic3DTextLabel(clText(COLOR_INFO2, string), -1, stationInfo[ID_STACJA][sPosX], stationInfo[ID_STACJA][sPosY], stationInfo[ID_STACJA][sPosZ]+0.5, 30.0);
		StacjeMapIcon[ID_STACJA] = CreateDynamicMapIcon(stationInfo[ID_STACJA][sPosX], stationInfo[ID_STACJA][sPosY], stationInfo[ID_STACJA][sPosZ], 55, 0, _, _, _, 300.0);
		
		StacjeMapIconForAll[ID_STACJA] = CreateDynamicMapIcon(stationInfo[ID_STACJA][sPosX], stationInfo[ID_STACJA][sPosY], stationInfo[ID_STACJA][sPosZ], 55, 0, _, _, GetMaxPlayers()+1, 12000.0);
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

	switch( (Spawned[vehicleid]) ? (vehInfo_Temp[vehicleid][vFuelType]) : (vehInfo[DBVehID[vehicleid]][vFuelType]) )
	{
		case FUEL_TYPE_GAS: strcat(fueltype, "Gaz");
		case FUEL_TYPE_BENZ: strcat(fueltype, "Benzyna");
		case FUEL_TYPE_TDI: strcat(fueltype, "Ropa");
	}
	if(!Spawned[vehicleid] && vehInfo[DBVehID[vehicleid]][vGasStatus])
		format(fueltype, sizeof fueltype, "Gaz");
	
	Dialog_Show(playerid, DIALOG_TANKOWANIE_ILOSC, DIALOG_STYLE_TABLIST, "Stacja", "Zbiornik gazu:\t%dL\nZbiornik ropy:\t%dL\nZbiornik benzyny:\t%dL\n---\nCena gazu:\t$%.02f\nCena ropy:\t$%0.2f\nCena benzyny:\t$%0.2f\n---\nRodzaj paliwa pojazdu:\t%s", "Tankuj", "Anuluj", 
			stationInfo[stacjaid][sGasAmount], stationInfo[stacjaid][sPetroleumAmount], stationInfo[stacjaid][sPetrolAmount], stationInfo[stacjaid][sGasPrice], stationInfo[stacjaid][sPetroleumPrice], stationInfo[stacjaid][sPetrolPrice], fueltype);

	return 1;
}

Dialog:DIALOG_TANKOWANIE_ILOSC(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new vehicleid=GetPlayerVehicleID(playerid), Float:mozeZatankowac = float(MaxFuel(GetVehicleModel(vehicleid)));

	if(Spawned[vehicleid])
		mozeZatankowac -= vehInfo_Temp[vehicleid][vFuel];
	else
		mozeZatankowac -= (!vehInfo[DBVehID[vehicleid]][vGasStatus]) ? (vehInfo[DBVehID[vehicleid]][vFuel]) : (vehInfo[DBVehID[vehicleid]][vGasAmount]);

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
		mozeZatankowac -= vehInfo_Temp[vehicleid][vFuel];
	else
		mozeZatankowac -= (!vehInfo[DBVehID[vehicleid]][vGasStatus]) ? (vehInfo[DBVehID[vehicleid]][vFuel]) : (vehInfo[DBVehID[vehicleid]][vGasAmount]);

	if(ileZatankowac < 0)
		return Dialog_Show(playerid, DIALOG_TANKOWANIE_TANKUJ, DIALOG_STYLE_INPUT, "Stacja", "Nieprawid³owa iloœæ.\nMo¿esz zatankowaæ: %0.2fL\n\nWpisz iloœæ paliwa w litrach (0, by do pe³na):", "Tankuj", "Anuluj", mozeZatankowac);

	if(ileZatankowac == 0)
		ileZatankowac = mozeZatankowac;

	switch( (Spawned[vehicleid]) ? (vehInfo_Temp[vehicleid][vFuelType]) : (vehInfo[DBVehID[vehicleid]][vFuelType]) )
	{
		case FUEL_TYPE_GAS: koszt = floatround(ileZatankowac * stationInfo[stacjaid][sGasPrice]);
		case FUEL_TYPE_BENZ: koszt = floatround(ileZatankowac * stationInfo[stacjaid][sPetrolPrice]);
		case FUEL_TYPE_TDI: koszt = floatround(ileZatankowac * stationInfo[stacjaid][sPetroleumPrice]);
	}

	if(!Spawned[vehicleid] && vehInfo[DBVehID[vehicleid]][vGasStatus])
		koszt = floatround(ileZatankowac * stationInfo[stacjaid][sGasPrice]);

	new firmaid = playerInfo[playerid][pFirm];

	if(((!GetPVarInt(playerid, "Worked") || firmaid == 0 && GetMoney(playerid) < koszt)) || (firmInfo[firmaid][tID] && firmInfo[firmaid][tBank] < koszt))
		return Dialog_Show(playerid, DIALOG_TANKOWANIE_TANKUJ, DIALOG_STYLE_INPUT, "Stacja", "Nie masz tylu pieniêdzy! Potrzebujesz: $%d\nMo¿esz zatankowaæ: %0.2fL\n\nWpisz iloœæ paliwa w litrach (0, by do pe³na):", "Tankuj", "Anuluj", koszt, mozeZatankowac);

	if(Spawned[vehicleid])
		vehInfo_Temp[vehicleid][vFuel] += ileZatankowac;
	else
	{
		if(!vehInfo[DBVehID[vehicleid]][vGasStatus])
			vehInfo[DBVehID[vehicleid]][vFuel] += ileZatankowac;
		else
			vehInfo[DBVehID[vehicleid]][vGasAmount] += ileZatankowac;
	}

	if(firmInfo[firmaid][tID])
		firmInfo[firmaid][tBank] -= koszt;
	else
		GiveMoney(playerid, -koszt);

	if(Spawned[vehicleid])
	{
		switch( (Spawned[vehicleid]) ? (vehInfo_Temp[vehicleid][vFuelType]) : (vehInfo[DBVehID[vehicleid]][vFuelType]) )
		{
			case FUEL_TYPE_GAS: stationInfo[stacjaid][sGasAmount] -= floatround(ileZatankowac);
			case FUEL_TYPE_BENZ: stationInfo[stacjaid][sPetroleumAmount] -= floatround(ileZatankowac);
			case FUEL_TYPE_TDI: stationInfo[stacjaid][sPetrolAmount] -= floatround(ileZatankowac);
		}
	}
	else
	{
		if(!vehInfo[DBVehID[vehicleid]][vGasStatus])
		{
			switch(vehInfo[DBVehID[vehicleid]][vFuelType])
			{
				case FUEL_TYPE_GAS: stationInfo[stacjaid][sGasAmount] -= floatround(ileZatankowac);
				case FUEL_TYPE_BENZ: stationInfo[stacjaid][sPetroleumAmount] -= floatround(ileZatankowac);
				case FUEL_TYPE_TDI: stationInfo[stacjaid][sPetrolAmount] -= floatround(ileZatankowac);
			}
		}
		else
		{
			stationInfo[stacjaid][sGasAmount] -= floatround(ileZatankowac);
		}
	}
	
		
	Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, "Stacja", "Zatankowa³eœ $%0.2fL za $%d.", "Ok", "", ileZatankowac, koszt);
	return 1;
}

CMD:stacjatp(playerid, params[])
{
	if(stationInfo[strval(params)][sID] == 0)
		return 1;

	SetPlayerPos(playerid, stationInfo[strval(params)][sPosX], stationInfo[strval(params)][sPosY], stationInfo[strval(params)][sPosZ]+3.0);
	return 1;
}

CMD:ustawzbiornik(playerid, params[])
{
	new vehicleid = GetPlayerVehicleID(playerid);

	if(!IsPlayerInAnyVehicle(playerid))
		return 1;

	if(Spawned[vehicleid])
		vehInfo_Temp[vehicleid][vFuel] = strval(params);
	else
		vehInfo[DBVehID[vehicleid]][vFuel] = strval(params);

	return 1;
}
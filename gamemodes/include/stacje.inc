LadujStacje()
{
	mysql_query("SELECT * FROM stations");
	mysql_store_result();

	if(mysql_num_rows() == 0)
		return print("[STACJE] Brak stacji do zaladowania.");

	new string[180];
	while(mysql_fetch_row(string, "|")) 
	{
    	sscanf(string, "e<p<|>dfffdddfffddd>", stationInfo[ID_STACJA]);

    	format(string, sizeof string, "Stacja paliwowa {b}%s{/b} w {b}%s{/b} (ID: {b}%d{/b})\nAby u¿yæ wpisz {b}/stacja{/b}\nOtwarte w godzinach {b}%02d:00 - %02d:00{/b}", GetStationName(ID_STACJA), GetPointZone(stationInfo[ID_STACJA][sPosX], stationInfo[ID_STACJA][sPosY]), ID_STACJA, stationInfo[ID_STACJA][sOpenHour], stationInfo[ID_STACJA][sCloseHour]);
    	StacjeText3D[ID_STACJA] = CreateDynamic3DTextLabel(clText(COLOR_INFO2, string), -1, stationInfo[ID_STACJA][sPosX], stationInfo[ID_STACJA][sPosY], stationInfo[ID_STACJA][sPosZ]+0.5, 30.0);
		StacjeMapIcon[ID_STACJA] = CreateDynamicMapIcon(stationInfo[ID_STACJA][sPosX], stationInfo[ID_STACJA][sPosY], stationInfo[ID_STACJA][sPosZ], 55, 0, _, _, _, 300.0);
		stationInfo[ID_STACJA][sCreated] = true;

		StacjeMapIconForAll[ID_STACJA] = CreateDynamicMapIcon(stationInfo[ID_STACJA][sPosX], stationInfo[ID_STACJA][sPosY], stationInfo[ID_STACJA][sPosZ], 55, 0, _, _, GetMaxPlayers()+1, 12000.0);
		Streamer_SetIntData(STREAMER_TYPE_MAP_ICON, StacjeMapIconForAll[ID_STACJA], E_STREAMER_STYLE, MAPICON_GLOBAL);
		ID_STACJA++;
	} 

	printf("[STACJE] Zaladowano %d stacji.", ID_STACJA);
	mysql_free_result();
	return 1;
}

stock IsValidStation(stationid)
{
	if(stationid >= MAX_STATIONS) return false;
	if(stationInfo[stationid][sCreated])
		return true;
	return false;
}

stock GetStationName(stationid)
{
	new szName[32], type = stationInfo[stationid][sPetrolFirm];
	switch(type)
	{
		case 0: strcat(szName, "Express Tank");
		case 1: strcat(szName, "VPower Tank");
		case 2: strcat(szName, "Total Tank");
		case 3: strcat(szName, "Trucksuper Tank");
	}
	return szName;
}

CMD:stacja(playerid, params[])
{
	DeletePVar(playerid, "isAtStation");
	DeletePVar(playerid, "fuelPrice");
	DeletePVar(playerid, "tempFuel");
	new isAtStation = -1;
	for (new i = 0; i < MAX_STATIONS; i++)
	{
		if (!IsValidStation(i))
			continue;

		if (IsPlayerInRangeOfPoint(playerid, 5.0, stationInfo[i][sPosX], stationInfo[i][sPosY], stationInfo[i][sPosZ]))
		{
			isAtStation = i;
			break;
		}
	}
	if(isAtStation < 0)
		return Msg(playerid, COLOR_ERROR, "Nie jesteœ na ¿adnej stacji.");

	if((gmInfo[gmHour] < stationInfo[isAtStation][sOpenHour]) || (gmInfo[gmHour] > stationInfo[isAtStation][sCloseHour]))
		return Msg(playerid, COLOR_ERROR, "Stacja jest jeszcze zamkniêta.");

	SetPVarInt(playerid, "isAtStation", isAtStation);
	new szString[300];
	format(szString, sizeof szString, "Stacja {b}%s{/b} w {b}%s{/b}\n", GetStationName(isAtStation), GetPointZone(stationInfo[isAtStation][sPosX], stationInfo[isAtStation][sPosY]));
	format(szString, sizeof szString, "%s \n", szString);
	format(szString, sizeof szString, "%s{b}Cennik:{/b}\n", szString);
	format(szString, sizeof szString, "%s × Benzyna {b}$%.02f/l{/b}\n", szString, stationInfo[isAtStation][sPetrolPrice]);
	format(szString, sizeof szString, "%s × Olej napêdowy {b}$%.02f/l{/b}\n", szString, stationInfo[isAtStation][sPetroleumPrice]);
	format(szString, sizeof szString, "%s × Gaz {b}$%.02f/l{/b}\n", szString, stationInfo[isAtStation][sGasPrice]);
	format(szString, sizeof szString, "%s \n", szString);
	format(szString, sizeof szString, "%sTankuj pojazd\n", szString);
	format(szString, sizeof szString, "%sKup kanister 10l - {b}$100{/b}\n", szString);
	format(szString, sizeof szString, "%sKup zestaw naprawczy - {b}$150{/b}\n", szString);
	format(szString, sizeof szString, "%sKup hot-doga - {b}$50{/b}", szString);

	Dialog_Show(playerid, DIALOG_STATION_MAIN, DIALOG_STYLE_LIST, "Stacja", clText(COLOR_INFO2, szString), "Wybierz", "WyjdŸ");
	return 1;
}

Dialog:DIALOG_STATION_MAIN(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	switch(listitem)
	{
		case 7: 
		{
			if (!IsPlayerInAnyVehicle(playerid) || GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
			{
				Msg(playerid, COLOR_ERROR, "Nie jesteœ w ¿adnym pojeŸdzie.");
				return cmd_stacja(playerid, "");
			}
			new lights, alarm, doors, bonnet, boot, objective, engine;
			GetVehicleParamsEx(GetPlayerVehicleID(playerid), engine, lights, alarm, doors, bonnet, boot, objective);
			if (engine)
			{
				Msg(playerid, COLOR_ERROR, "Aby zatankowaæ paliwo musisz mieæ wy³¹czony silnik.");
				return cmd_stacja(playerid, "");
			}
			DeletePVar(playerid, "fuelPrice");
			DeletePVar(playerid, "tempFuel");
			new szString[128];
			format(szString, sizeof szString, "{FFFFFF}Tankowanie pojazdu {b}%s{/b}\n \nZatankowano: {b}0l{/b}\nDo zaplaty: {b}$0,00{/b}", GetVehicleModelName(GetVehicleModel(GetPlayerVehicleID(playerid))));
			Dialog_Show(playerid, DIALOG_STATION_MAIN_TANK, DIALOG_STYLE_MSGBOX, "Stacja > Tankowanie", clText(COLOR_INFO2, szString), "Start", "Wstecz");
		}
		case 8: 
		{
			if(GetMoney(playerid) < 100)
			{
				Msg(playerid, COLOR_ERROR, "Nie masz wystarczaj¹co pieniêdzy aby kupiæ kanister.");
				return cmd_stacja(playerid, "");
			}
			if (playerInfo[playerid][pCanister] >= 2)
			{
				Msg(playerid, COLOR_ERROR, "Posiadasz ju¿ 2 kanistry.");
				return cmd_stacja(playerid, "");
			}

			playerInfo[playerid][pCanister] ++;
			Msg(playerid, COLOR_INFO2, "Kupi³eœ kanister.");
			GiveMoney(playerid, -100);
			return cmd_stacja(playerid, "");
		}
		case 9:
		{
			if(GetMoney(playerid) < 150)
			{
				Msg(playerid, COLOR_ERROR, "Nie masz wystarczaj¹co pieniêdzy aby kupiæ zestaw naprawczy.");
				return cmd_stacja(playerid, "");
			}
			if (playerInfo[playerid][pRepairKit] >= 1)
			{
				Msg(playerid, COLOR_ERROR, "Posiadasz ju¿ zestaw naprawczy.");
				return cmd_stacja(playerid, "");
			}

			playerInfo[playerid][pRepairKit] ++;
			GiveMoney(playerid, -150);
			Msg(playerid, COLOR_INFO2, "Kupi³eœ zestaw naprawczy.");
			return cmd_stacja(playerid, "");
		}
		case 10: 
		{
			if(GetMoney(playerid) < 50)
			{
				Msg(playerid, COLOR_ERROR, "Nie masz wystarczaj¹co pieniêdzy aby kupiæ hot-doga.");
				return cmd_stacja(playerid, "");
			}
			new Float:health;
			GetPlayerHealth(playerid, health);
			health += float(5+random(15)); if(health > 100.0) health = 100.0;
			playerInfo[playerid][pHunger] += float(5+random(10)); if(playerInfo[playerid][pHunger] > 100.0) playerInfo[playerid][pHunger] = 100.0;
			SetPlayerHealth(playerid, health);
			Msg(playerid, COLOR_INFO2, "Kupi³eœ hot-doga.");
			if (!IsPlayerInAnyVehicle(playerid))
			{
				PreloadAnimLib(playerid, "FOOD");
				ApplyAnimation(playerid, "FOOD", "EAT_Burger", 4.1, 0, 1, 1, 0, 0);
			}
			SetPlayerProgressBarValue(playerid, hudInfo[tdHungerProgress][playerid], playerInfo[playerid][pHunger]);
			GiveMoney(playerid, -50);
			return cmd_stacja(playerid, "");
		}
		default: return cmd_stacja(playerid, "");
	}
	return 1;
}

forward OnPlayerVehicleTank(playerid, vehicleid);
public OnPlayerVehicleTank(playerid, vehicleid)
{
	new Float:fuel = GetPVarFloat(playerid, "tempFuel"), Float:totalFuel, Float:price, model = GetVehicleModel(vehicleid);
	switch(Spawned[vehicleid])
	{
		case 0: // const
		{
			vehicleid = DBVehID[vehicleid];
			if (vehInfo[vehicleid][vGasStatus])
				price = fuel * stationInfo[GetPVarInt(playerid, "isAtStation")][sGasPrice];
			else
			{
				switch(vehInfo[vehicleid][vFuelType])
				{
					case FUEL_TYPE_BENZ: price = fuel * stationInfo[GetPVarInt(playerid, "isAtStation")][sPetrolPrice];
					case FUEL_TYPE_TDI: price = fuel * stationInfo[GetPVarInt(playerid, "isAtStation")][sPetroleumPrice];
				}
			}
			totalFuel = fuel + vehInfo[vehicleid][vFuel];
		}
		case 1: // temp
		{
			switch(vehInfo_Temp[vehicleid][vFuelType])
			{
				case FUEL_TYPE_BENZ: price = fuel * stationInfo[GetPVarInt(playerid, "isAtStation")][sPetrolPrice];
				case FUEL_TYPE_TDI: price = fuel * stationInfo[GetPVarInt(playerid, "isAtStation")][sPetroleumPrice];
			}
			totalFuel = fuel + vehInfo_Temp[vehicleid][vFuel];
		}
	}

	new szString[128];
	format(szString, sizeof szString, "{FFFFFF}Tankowanie pojazdu {b}%s{/b}\n \nZatankowano: {b}%dl{/b}\nDo zaplaty: {b}$%.02f{/b}", GetVehicleModelName(model), floatround(fuel), price);
	if (price >= GetMoney(playerid))
	{
		KillTimer_(GetPVarInt(playerid, "StationTimer"));
		Dialog_Show(playerid, DIALOG_STATION_MAIN_TANK, DIALOG_STYLE_MSGBOX, "Stacja > Tankowanie", clText(COLOR_INFO2, szString), "Start", "Wstecz");
		Msg(playerid, COLOR_INFO, "Nie starczy Ci na wiêcej paliwa!");
		return 1;
	}
	if (totalFuel >= MaxFuel(model))
	{
		KillTimer_(GetPVarInt(playerid, "StationTimer"));
		Dialog_Show(playerid, DIALOG_STATION_MAIN_TANK, DIALOG_STYLE_MSGBOX, "Stacja > Tankowanie", clText(COLOR_INFO2, szString), "Start", "Wstecz");
		Msg(playerid, COLOR_INFO, "Zatankowano do pe³na!");
		return 1;
	}

	SetPVarFloat(playerid, "fuelPrice", price);
	SetPVarFloat(playerid, "tempFuel", fuel + 1.0);
	Dialog_Show(playerid, DIALOG_STATION_MAIN_TANKP, DIALOG_STYLE_MSGBOX, "Stacja > Tankowanie", clText(COLOR_INFO2, szString), "Zatankuj", "Stop");
	return 1;
}

Dialog:DIALOG_STATION_MAIN_TANK(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
		return cmd_stacja(playerid, "");
	}
	SetPVarInt(playerid, "StationTimer", SetTimerEx_("OnPlayerVehicleTank", 500, 0, -1, "ii", playerid, GetPlayerVehicleID(playerid)));
	
	new szString[128], model = GetVehicleModel(GetPlayerVehicleID(playerid)), Float:fuel = GetPVarFloat(playerid, "tempFuel"), Float:price = GetPVarFloat(playerid, "fuelPrice");
	format(szString, sizeof szString, "{FFFFFF}Tankowanie pojazdu {b}%s{/b}\n \nZatankowano: {b}%dl{/b}\nDo zaplaty: {b}$%.02f{/b}", GetVehicleModelName(model), floatround(fuel), price);
	Dialog_Show(playerid, DIALOG_STATION_MAIN_TANKP, DIALOG_STYLE_MSGBOX, "Stacja > Tankowanie", clText(COLOR_INFO2, szString), "Zatankuj", "Stop");
	return 1;
}

Dialog:DIALOG_STATION_MAIN_TANKP(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
		new szString[128], Float:fuel = GetPVarFloat(playerid, "tempFuel"), Float:price = GetPVarFloat(playerid, "fuelPrice");
		format(szString, sizeof szString, "{FFFFFF}Tankowanie pojazdu {b}%s{/b}\n \nZatankowano: {b}%dl{/b}\nDo zaplaty: {b}$%.02f{/b}", GetVehicleModelName(GetVehicleModel(GetPlayerVehicleID(playerid))), floatround(fuel), price);
		KillTimer_(GetPVarInt(playerid, "StationTimer"));
		Dialog_Show(playerid, DIALOG_STATION_MAIN_TANK, DIALOG_STYLE_MSGBOX, "Stacja > Tankowanie", clText(COLOR_INFO2, szString), "Start", "Wstecz");
	}
	return 1;
}

CMD:kanister(playerid)
{
	if (playerInfo[playerid][pCanister] < 1)
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz kanistra.");

	if (IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie mo¿esz siedzieæ w pojeŸdzie.");

	new v = INVALID_VEHICLE_ID, Float:pos[3], Float:fDistance = 50.0, Float:fTemp;
	for(new i = 0; i < GetVehiclePoolSize(); i++)
	{
		if (!IsValidVehicle(i))
			continue;

		GetVehiclePos(i, pos[0], pos[1], pos[2]);
		fTemp = GetDistanceFromMeToPoint(playerid, pos[0], pos[1], pos[2]);
		if(fTemp < fDistance)
		{
			v = i;
			fDistance = fTemp;
		}
	}
	if (v == INVALID_VEHICLE_ID)
		return Msg(playerid, COLOR_ERROR, "Nie jesteœ w pobli¿u ¿adnego pojazdu.");

	GetVehiclePos(v, pos[0], pos[1], pos[2]);
	if (!IsPlayerInRangeOfPoint(playerid, 6.0, pos[0], pos[1], pos[2]))
		return Msg(playerid, COLOR_ERROR, "Nie jesteœ w pobli¿u ¿adnego pojazdu.");

	playerInfo[playerid][pCanister] --;
	Msg(playerid, COLOR_INFO, "Trwa tankowanie...");
	SetTimerEx_("SetVehicleCanister", 0, 5 * 60 * 1000, 1, "ii", playerid, v);
	return 1;
}

forward SetVehicleCanister(playerid, vehicleid);
public SetVehicleCanister(playerid, vehicleid)
{
	if (!Spawned[vehicleid])
	{
		vehInfo_Temp[vehicleid][vFuel] += 10;
		if (vehInfo[vehicleid][vFuel] > MaxFuel(GetVehicleModel(vehicleid)))
			vehInfo[vehicleid][vFuel] = MaxFuel(GetVehicleModel(vehicleid));
	}
	else
	{
		vehInfo[DBVehID[vehicleid]][vFuel] += 10;
		if (vehInfo[DBVehID[vehicleid]][vFuel] > MaxFuel(GetVehicleModel(vehicleid)))
			vehInfo[DBVehID[vehicleid]][vFuel] = MaxFuel(GetVehicleModel(vehicleid));
	}

	Msg(playerid, COLOR_INFO, "Pomyœlnie wlano kanister.");
	return 1;
}

CMD:zestaw(playerid)
{
	if (playerInfo[playerid][pRepairKit] < 1)
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz zestawu naprawczego.");

	if (IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie mo¿esz siedzieæ w pojeŸdzie.");

	new v = INVALID_VEHICLE_ID, Float:pos[3], Float:fDistance = 50.0, Float:fTemp;
	for(new i = 0; i <  GetVehiclePoolSize(); i++)
	{
		if (!IsValidVehicle(i))
			continue;

		GetVehiclePos(i, pos[0], pos[1], pos[2]);
		fTemp = GetDistanceFromMeToPoint(playerid, pos[0], pos[1], pos[2]);
		if(fTemp < fDistance)
		{
			v = i;
			fDistance = fTemp;
		}
	}
	if (v == INVALID_VEHICLE_ID)
		return Msg(playerid, COLOR_ERROR, "Nie jesteœ w pobli¿u ¿adnego pojazdu.");

	GetVehiclePos(v, pos[0], pos[1], pos[2]);
	if (!IsPlayerInRangeOfPoint(playerid, 6.0, pos[0], pos[1], pos[2]))
		return Msg(playerid, COLOR_ERROR, "Nie jesteœ w pobli¿u ¿adnego pojazdu.");

	playerInfo[playerid][pRepairKit] --;
	Msg(playerid, COLOR_INFO, "Pomyœlnie u¿yto zestawu naprawczego.");

	new Float:h;
	GetVehicleHealth(v, h);
	RepairVehicle(v);
	h += 250;
	if (h > 1000.0)
		h = 1000.0;
	SetVehicleHealth(v, h);
	return 1;
}
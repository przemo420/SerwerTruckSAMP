CMD:przeglad(playerid, params[])
{
	new targetid, vehicleid, string[100];

	if(firmInfo[playerInfo[playerid][pFirm]][tType] != TEAM_TYPE_POMOC)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(sscanf(params, "d", targetid))
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/przeglad [id gracza]{/b}");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Gracza z tym ID nie ma online.");

	if(GetDistancePlayerToPlayer(playerid, targetid) > 20)
		return Msg(playerid, COLOR_ERROR, "Znajdujesz siê za daleko od gracza.");

	if(GetPlayerState(targetid) != PLAYER_STATE_DRIVER)
		return Msg(playerid, COLOR_ERROR, "Gracz z tym ID nie prowadzi pojazdu.");

	vehicleid = GetPlayerVehicleID(targetid);

	if(Spawned[vehicleid])
		return Msg(playerid, COLOR_ERROR, "Nie mo¿esz zrobiæ przegl¹du w tym pojeŸdzie.");

	format(string, sizeof(string), "UPDATE `Pojazdy` SET `Przeglad`=CURRENT_TIMESTAMP() DEFAULT WHERE `id`='%d'", DBVehID[vehicleid]);
	mysql_query(string);

	format(string, sizeof(string), "SELECT `Przeglad` FROM `Pojazdy` WHERE `id`='%d'", DBVehID[vehicleid]);
	mysql_query(string);
	mysql_store_result();
	mysql_fetch_field("Przeglad", vehInfo[DBVehID[vehicleid]][vPrzeglad]);
	mysql_free_result();

	Msg(playerid, COLOR_INFO, "Zrobi³eœ przegl¹d w pojeŸdzie.");
	Msg(targetid, COLOR_INFO, "W Twoim pojeŸdzie zrobiono przegl¹d.");
	return 1;
}

forward Pomoc_OnPlayerEnterJob(playerid);
public Pomoc_OnPlayerEnterJob(playerid)
{
	GivePlayerWeapon(playerid, 11, 99999);
	return GivePlayerWeapon(playerid, 41, 99999);
}

CMD:vehcolor(playerid, params[])
{
	if(playerInfo[playerid][pFirm] != TEAM_TYPE_POMOC)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie");

	new color[2];
	if(sscanf(params, "dd", color[0], color[1]))
		return Msg(playerid, COLOR_ERROR, "Wpisz /vehcolor [kolor 1] [kolor2]");

	if((color[0] > 255 || color[0] < 0) || (color[1] > 255 || color[1] < 0))
		return Msg(playerid, COLOR_ERROR, "Kolor musi znajdowaæ siê w przedziale {b}0 - 255{/b}.");

	SetPVarInt(playerid, "changeColor", 1);
	SetPVarInt(playerid, "changeColor_", color[0]);
	SetPVarInt(playerid, "changeColor__", color[1]);
	Msg(playerid, COLOR_INFO, "Wszystko w porz¹tku, teraz wejdŸ do pojazdu i wciœnij klawisz 'Y'.");
	return 1;
}

CMD:setplate(playerid, params[])
{
	if(firmInfo[playerInfo[playerid][pFirm]][tType] != TEAM_TYPE_POMOC)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");
	
	if(strlen(params) < 3 || strlen(params) > 32)
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/setplate [treœæ]{/b}");

	SetPVarString(playerid, "PLATE_Name", params);
	new szString[128], Float:fPos[3], count = 0;
	GetPlayerPos(playerid, fPos[0], fPos[1], fPos[2]);
	
	for(new i = 0; i < MAX_VEHICLES; i++)
	{
		if(IsValidVehicle(i))
		{
			new Float:fDistance;
			fDistance = GetVehicleDistanceFromPoint(i, fPos[0], fPos[1], fPos[2]);
			if(fDistance < 10.0)
			{
				new szTemp[32];
				format(szTemp, sizeof szTemp, "%d\t%s\n", i, GetVehicleModelName(GetVehicleModel(i)));
				strcat(szString, szTemp);
				count++;
			}
		}
	}
	if(count < 1)
		strcat(szString, "Nie odnaleziono pojazdów w promieniu 5m.");
	else
		strins(szString, "ID\tNazwa\n", 0);

	SetPVarInt(playerid, "PLATE_Count", count);
	Dialog_Show(playerid, DIALOG_SET_PLATE, count > 0 ? DIALOG_STYLE_TABLIST_HEADERS : DIALOG_STYLE_LIST, " ", szString, "Wybierz", "WyjdŸ");
	return 1;
}

forward SetPos(vehicleid, Float:x, Float:y, Float:z);
public SetPos(vehicleid, Float:x, Float:y, Float:z)
{
	SetVehiclePos(vehicleid, x, y, z);
	return 1;
}

CMD:zatankuj(playerid)
{
	if(firmInfo[playerInfo[playerid][pFirm]][tType] != TEAM_TYPE_POMOC)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie siedzisz w ¿adnym pojeŸdzie.");

	new vehicleid = GetPlayerVehicleID(playerid), string[80];

	if(Spawned[vehicleid])
		vehInfo_Temp[vehicleid][vFuel] = MaxFuel(GetVehicleModel(vehicleid));
	else
	{
		if(!vehInfo[DBVehID[vehicleid]][vGasStatus])
			vehInfo[DBVehID[vehicleid]][vFuel] = float(MaxFuel(GetVehicleModel(vehicleid)));
		else
			vehInfo[DBVehID[vehicleid]][vGasAmount] = 60.0;
	}

	format(string, sizeof(string), "Zatankowa³eœ pojazd {b}ID: %d{/b} do pe³na.", vehicleid);
	Msg(playerid, COLOR_INFO3, string);
	return 1;
}

CMD:rachunek(playerid, params[])
{
	if(firmInfo[playerInfo[playerid][pFirm]][tType] != TEAM_TYPE_POMOC)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	new cash, targetid;
	if(sscanf(params, "dd", targetid, cash))
		return Msg(playerid, COLOR_ERROR, "Wpisz /rachunek [id gracza] [wielkoœæ]");
	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Gracza z tym ID nie ma online.");
	if(cash < 0)
		return Msg(playerid, COLOR_ERROR, "Podana kwota jest zbyt niska.");
	new szString[158];
	format(szString, sizeof szString, "Pracownik {b}Pomocy Drogowej{/b} %s wystawi³ Ci rachunek na kwotê {b}$%d{/b}. Aby zap³aciæ wpisz /przelej.", PlayerName(playerid), cash);
	Msg(targetid, COLOR_INFO3, szString);
	Msg(playerid, COLOR_INFO3, "Rachunek wystawiony!");
	return 1;
}

Dialog:DIALOG_SET_PLATE(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;
	new playerName[32];
	GetPVarString(playerid, "PLATE_Name", playerName, sizeof playerName);
	if(GetPVarInt(playerid, "PLATE_Count") < 1)
		return cmd_setplate(playerid, playerName);

	new Float:fPos[3], vehicleid;
	GetPlayerPos(playerid, fPos[0], fPos[1], fPos[2]);
	sscanf(inputtext, "d{s[20]}", vehicleid);

	new szTemp[64];
	format(szTemp, sizeof szTemp, "Pomyœlnie zmieni³eœ rejestracjê pojazdu {b}%s{/b}.", playerName);
	Msg(playerid, COLOR_INFO3, szTemp);
	
	GetVehiclePos(vehicleid, fPos[0], fPos[1], fPos[2]);
	SetTimerEx("SetPos", 1500, false, "dfff", vehicleid, fPos[0], fPos[1], fPos[2]);
	SetVehicleNumberPlate(vehicleid, playerName);
	SetVehicleToRespawn(vehicleid);

	DeletePVar(playerid, "PLATE_Name");
	DeletePVar(playerid, "PLATE_Count");

	if(!Spawned[vehicleid])
	{
		strdel(vehInfo[DBVehID[vehicleid]][vPlate], 0, 31);
		strins(vehInfo[DBVehID[vehicleid]][vPlate], playerName, 0);

		SaveVehicle(vehicleid);
	}

	return 1;
}
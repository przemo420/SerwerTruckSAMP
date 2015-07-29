// KOMENDY POLICJI
/*
	- /mandat
	- /aresztuj
	- /wywal /wyrzuc
	- /praca
	- /firma
	- /skuj
	- /rozkuj
	- /kontrola
	- /alkomat
*/

CMD:alkomat(playerid, params[])
{
	new targetid;

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_POLICE)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(sscanf(params, "d", targetid))
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/alkomat [id gracza]{/b}");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Gracza z tym ID nie ma online.");

	if(GetDistancePlayerToPlayer(playerid, targetid) > 20)
		return Msg(playerid, COLOR_ERROR, "Znajdujesz siê za daleko od gracza.");

	if(GetPlayerState(targetid) != PLAYER_STATE_DRIVER)
		return Msg(playerid, COLOR_ERROR, "Gracz z tym ID nie prowadzi pojazdu.");

	Msg(playerid, COLOR_INFO2, "Trwa sprawdzanie...");
	SetTimerEx("AlcoholTest", 5 * 1000, false, "dd", playerid, targetid);
	return 1;
}

CMD:mandat(playerid, params[])
{
	new forplayerid, monej, pojnts, powod[76];
	new string[256];

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_POLICE)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(sscanf(params, "ddds[76]", forplayerid, monej, pojnts, powod))
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/mandat [id gracza] [kwota] [punkty] [powod]{/b}");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Gracza z tym ID nie ma online.");

	if(GetDistancePlayerToPlayer(playerid, forplayerid)>20)
		return Msg(playerid, COLOR_ERROR, "Znajdujesz siê za daleko od gracza.");

	if(GetPlayerSpeed(playerid) > 0)
		return Msg(playerid, COLOR_ERROR, "Gracz {b}nie mo¿e siê{/b} poruszaæ.");

	if(monej < 0)
		return Msg(playerid, COLOR_ERROR, "Kwota mandatu jest {b}zbyt niska{/b}.");

	if(pojnts < 0)
		return Msg(playerid, COLOR_ERROR, "Iloœæ punktów jest {b}zbyt niska{/b}.");

	format(string,sizeof string,""GCOLOR_YELLOW"Mandat drogowy:\nKwota: {FFFFFF}$%d\n"GCOLOR_YELLOW"Punkty: {FFFFFF}%d\n"GCOLOR_YELLOW"Powód: {FFFFFF}%s\n"GCOLOR_YELLOW"Policjant: {FFFFFF}%s\n"GCOLOR_YELLOW"Przyjmujesz?",monej, pojnts, powod, PlayerName(playerid));
	SetPVarInt(forplayerid,"kontrola",playerid);
	SetPVarInt(forplayerid, "opcja1", monej);
	SetPVarInt(forplayerid, "opcja2", pojnts); 
	return Dialog_Show(forplayerid, POLICE_DIALOG, DIALOG_STYLE_MSGBOX, " ", string, "Tak", "Nie");
}

CMD:aresztuj(playerid, params[])
{
	new forplayerid, czas, string[200];

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_POLICE)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(sscanf(params, "dd", forplayerid, czas))
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/aresztuj [id gracza] [czas]{/b}");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Gracza z tym ID nie ma online.");

	if(GetPVarInt(forplayerid, "Areszt"))
		return Msg(playerid, COLOR_ERROR, "Ten gracz {b}jest ju¿{/b} w areszcie.");

	SetPVarInt(forplayerid, "Areszt", 1);
	SetPVarInt(forplayerid, "Areszttime", czas*60);
	SetTimerEx("Unareszt", czas*(60), false, "d", forplayerid);
	format(string, sizeof string, "Aresztowa³eœ gracza {b}%s{/b} (ID: {b}%d{b}) na {b}%d{/b} minut.", PlayerName(forplayerid), forplayerid, czas);
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "Zosta³eœ aresztowany przez policjanta {b}%s{/b} (ID: {b}%d{/b}) na {b}%d{/b} minut(y).", PlayerName(playerid), playerid, czas);
	Msg(forplayerid, COLOR_INFO, string);

	SetPlayerInterior(forplayerid, 10);
	SetPlayerPos(forplayerid, 219.3761,109.7970,999.0156);
	SetPlayerHealth(forplayerid, 9999999);
	TogglePlayerControllable(forplayerid, 0);
	PlayerInfo[forplayerid][chained] = false;
	return 1;
}

CMD:unareszt(playerid, params[])
{
	new forplayerid, string[200];

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_POLICE)
		return SendClientMessage(playerid, LIGHTRED, "Nie masz uprawnieñ.");

	if(sscanf(params, "d", forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "Wpisz: /unareszt [id gracza]");

	if(!IsPlayerConnected(forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "Gracz ten nie jest obecny na serwerze.");

	if(!GetPVarInt(forplayerid, "Areszt"))
		return SendClientMessage(playerid, LIGHTRED, "Ten gracz nie jest w areszcie.");

	DeletePVar(forplayerid,"Areszt");
	DeletePVar(forplayerid,"Areszttime");

	SetPlayerInterior(forplayerid, 0);
	SetPlayerPos(forplayerid, 2286.5684,2431.8369,10.8203);
	SetPlayerHealth(forplayerid, 100);
	TogglePlayerControllable(forplayerid, 1);

	format(string, sizeof string, "{C0C0C0}Wypuœci³eœ gracza %s.", PlayerName(forplayerid));
	SendClientMessage(playerid, 0x0, string);
	format(string, sizeof string, "{C0C0C0}Zosta³eœ wypuszczony dziêki policjantowi %s.", PlayerName(playerid));
	SendClientMessage(forplayerid, 0x0, string);

	return 1;
}

CMD:wyrzuc(playerid, cmdtext[])
	return cmd_wywal(playerid, cmdtext);

CMD:wywal(playerid, params[])
{
	new forplayerid, string[128];

	if(sscanf(params, "d", forplayerid))
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/wywal [id gracza]{/b}");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Gracza z tym ID nie ma online.");

	if(GetDistancePlayerToPlayer(playerid, forplayerid) > 10)
		return Msg(playerid, COLOR_ERROR, "Znajdujesz siê {b}za daleko{/b} od gracza.");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] == TEAM_TYPE_MEDIC || Firmy[PlayerInfo[playerid][pFirma]][tTyp] == TEAM_TYPE_POLICE)
	{
		RemovePlayerFromVehicle(forplayerid);
		format(string, sizeof string, "Zosta³eœ wyrzucony z pojazdu przez gracza {b}%s{/b} (ID: {b}%d{/b}).", PlayerName(playerid), playerid);
		Msg(forplayerid, COLOR_ERROR, string);
		format(string, sizeof string, "Wyrzuci³eœ z pojazdu gracza {b}%s{/b} (ID: {b}%d{/b}).", PlayerName(forplayerid), forplayerid);
		Msg(playerid, COLOR_ERROR, string);
	} 
		else 
			Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	return 1;
}

CMD:kontrola(playerid, params[])
{
	new targetid,
		string[256],
		vehicleid,
		towarname[40],
		przeglad[32];

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_POLICE)
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnieñ");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(sscanf(params, "d", targetid))
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/kontrola [id gracza]{/b}");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Gracza z tym ID nie ma online.");

	if(GetDistancePlayerToPlayer(playerid, targetid) > 4.0)
		return Msg(playerid, COLOR_ERROR, "Znajdujesz siê za daleko od gracza.");

	if(!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Gracz nie jest w pojeŸdzie.");

	if(IsPlayerInTruck(playerid))
		vehicleid = GetVehicleTrailer(GetPlayerVehicleID(playerid));
	else
		vehicleid = GetPlayerVehicleID(playerid);
	
	if(vehOtherInfo[vehicleid][vCargoID] <= 0)
		strcat(towarname, "Brak");
	else
		strcat(towarname, Towary[vehOtherInfo[vehicleid][vCargoID]][NazwaTowaru]);

	if(Spawned[GetPlayerVehicleID(playerid)])
		strcat(przeglad, "AKTUALNY");
	else
		strcat(przeglad, vehInfo[DBVehID[GetPlayerVehicleID(playerid)]][vPrzeglad]);

	format(string, sizeof string, "Nick:\t%s\nPrawo jazdy:\tPOSIADA\nPrzegl¹d:\t%s\nTowar:\t%s\nWaga:\t%dkg\nPasy:\t%s", PlayerName(targetid), przeglad, towarname, vehOtherInfo[vehicleid][vCargoWeight], (GetPVarInt(playerid, "PASY") > 0) ? ("zapiête") : ("rozpiête"));
	Dialog_Show(playerid, DIALOG_CONTROL, DIALOG_STYLE_TABLIST, "Kontrola", string, "WyjdŸ", #);
	return 1;
}

CMD:skuj(playerid, params[])
{
	new target, szString[84];

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_POLICE)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(sscanf(params, "d", target))
		return Msg(playerid, COLOR_ERROR, "Wpisz: {b}/skuj [id gracza]{/b}");

	if(!IsPlayerConnected(target))
		return Msg(playerid, COLOR_ERROR, "Gracza o podanym ID nie ma online.");

	if(GetDistancePlayerToPlayer(playerid, target) > 3.0)
		return Msg(playerid, COLOR_ERROR, "Znajdujesz siê za daleko od tego gracza.");

	if(IsPlayerConnected(PlayerInfo[target][chained]))
	{
		format(szString, sizeof szString, "Gracz {b}%s{/b} jest ju¿ skuty!", PlayerName(target));
		Msg(playerid, COLOR_ERROR, szString);
	}
	else
	{
		PlayerInfo[target][chained] = playerid;
		SetPlayerSpecialAction(target, SPECIAL_ACTION_CUFFED);

		format(szString, sizeof szString, "Sku³eœ gracza {b}%s{/b} (ID: {b}%d{/b}).", PlayerName(target), target);
		Msg(playerid, COLOR_INFO3, szString);

		format(szString, sizeof szString, "Zosta³eœ skuty przez policjanta {b}%s{/b} (ID: {b}%d{/b}).", PlayerName(playerid), playerid);
		Msg(target, COLOR_INFO3, szString);

		PlayerInfo[target][chainedTimer] = SetTimerEx("SkutyTeleport", 500, true, "dd", target, playerid);
	}
	return 1;
}

CMD:rozkuj(playerid, params[])
{
	new target, szString[84];

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_POLICE)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(sscanf(params, "d", target))
		return Msg(playerid, COLOR_ERROR, "Wpisz: {b}/rozkuj [id gracza]{/b}");

	if(!IsPlayerConnected(target))
		return Msg(playerid, COLOR_ERROR, "Gracza o podanym ID nie ma online.");

	if(GetDistancePlayerToPlayer(playerid, target) > 3.0)
		return Msg(playerid, COLOR_ERROR, "Znajdujesz siê za daleko od tego gracza.");

	if(!IsPlayerConnected(PlayerInfo[target][chained]))
	{
		format(szString, sizeof szString, "Gracz {b}%s{/b} nie jest skuty!", PlayerName(target));
		Msg(playerid, COLOR_ERROR, szString);
	}
	else
	{
		SetPlayerSpecialAction(target, SPECIAL_ACTION_NONE);
		KillTimer(PlayerInfo[target][chainedTimer]);

		PlayerInfo[target][chained] = (-1);

		format(szString, sizeof szString, "Rozku³eœ gracza {b}%s{/b} (ID: {b}%d{/b}).", PlayerName(target), target);
		Msg(playerid, COLOR_INFO3, szString);

		format(szString, sizeof szString, "Zosta³eœ rozkuty przez policjanta {b}%s{/b} (ID: {b}%d{/b}).", PlayerName(playerid), playerid);
		Msg(target, COLOR_INFO3, szString);
	}
	return 1;
}

CMD:stop(playerid, params[])
{
	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_POLICE)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	new string[100], Float:Pos[3];

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_POLICE)
		return SendClientMessage(playerid, LIGHTRED, "Nie masz uprawnieñ.");

	format(string, sizeof string, "Otrzyma³eœ alert od policjanta {FFFFFF}%s.", PlayerName(playerid));

	Loop(player, MAX_PLAYERS)
	{
		if(GetDistancePlayerToPlayer(playerid, player) < 100 && IsPlayerInAnyVehicle(player) && GetPlayerState(player) == PLAYER_STATE_DRIVER)
		{
			TextDrawShowForPlayer(player, AlertTD);
			timer9[player] = SetTimerEx("PolicjaAlert", 10000, 0, "d", player);

			Msg(player, COLOR_INFO3, string, false);

			GetPlayerPos(player, Pos[0], Pos[1], Pos[2]);
			PlayerPlaySound(player, 1147, Pos[0], Pos[1], Pos[2]);
		}
	}
	
	return 1;
}

CMD:kartoteka(playerid, params[])
{
	new forplayerid, vehicleid = GetPlayerVehicleID(playerid), vehuid = DBVehID[vehicleid];

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_POLICE)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ w pojeŸdzie.");

	if(Spawned[vehicleid] || vehInfo[vehuid][vOwnerType] != OWNER_TYPE_TEAM || Firmy[vehInfo[vehuid][vOwnerID]][tTyp] != TEAM_TYPE_POLICE)
		return Msg(playerid, COLOR_ERROR, "Musisz byæ w pojeŸdzie s³u¿bowym.");

	if(sscanf(params, "d", forplayerid))
		return Msg(playerid, COLOR_ERROR, "Wpisz: {b}/kartoteka [id gracza]{/b}");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Gracza o podanym ID nie ma online.");

	if(GetDistancePlayerToPlayer(playerid, forplayerid) > 3.0)
		return Msg(playerid, COLOR_ERROR, "Znajdujesz siê za daleko od tego gracza.");

	new wiadomosc[600], zapytanie[70];
	
	format(wiadomosc, sizeof(wiadomosc), "SELECT `policjant`, `text`, `time` FROM `st_kartoteka` WHERE `ownerid` = '%d' ORDER BY `time` DESC LIMIT 0,20", PlayerInfo[forplayerid][pID]);
	mysql_query(wiadomosc);
	mysql_store_result();

	strdel(wiadomosc, 0, 599);
	strdel(zapytanie, 0, 69);

	if(mysql_num_rows()==0)
		strcat(wiadomosc, "Brak wpisów.");
	else
	{
		strcat(wiadomosc, "Policjant\tWiadomoœæ\tData\n");
		while(mysql_fetch_row(zapytanie, "|")) 
		{
			mysql_fetch_field("policjant", zapytanie);
			strcat(wiadomosc, zapytanie);
			strcat(wiadomosc, "\t");

			mysql_fetch_field("text", zapytanie);
			strcat(wiadomosc, zapytanie);
			strcat(wiadomosc, "\n");

			mysql_fetch_field("time", zapytanie);
			strcat(wiadomosc, zapytanie);
			strcat(wiadomosc, "\n");
		} 
	}

	SetPVarInt(playerid, "Kartoteka_Dodaj", forplayerid);
	Dialog_Show(playerid, DIALOG_ID_KARTOTEKA, (mysql_num_rows() ? DIALOG_STYLE_TABLIST_HEADERS : DIALOG_STYLE_MSGBOX), "Kartoteka", wiadomosc, "Dodaj wpis", "Zamknij");

	return 1;
}

forward Policja_OnPlayerKey(playerid, newkeys, oldkeys);
public Policja_OnPlayerKey(playerid, newkeys, oldkeys)
{
	if((newkeys & KEY_ANALOG_LEFT) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		new vehicleid = GetPlayerVehicleID(playerid);

		if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] == TEAM_TYPE_POLICE)
		{
			if(GetVehicleModel(GetPlayerVehicleID(playerid)) == 411)
			{
		    	if(kpoli[vehicleid] == false)
				{
					kogutPOLI[vehicleid] = CreateObject(18646,0,0,-1000,0,0,0,100);
					AttachObjectToVehicle(kogutPOLI[vehicleid], vehicleid, -0.449999,0.000000,0.730000,0.000000,0.000000,0.000000);
			        kpoli[vehicleid] = true;
				}
			}
			else if(GetVehicleModel(GetPlayerVehicleID(playerid)) == 426)
			{
		    	if(kpoli[vehicleid] == false)
				{
					kogutPOLI[vehicleid] = CreateObject(18646,0,0,-1000,0,0,0,100);
					AttachObjectToVehicle(kogutPOLI[vehicleid], vehicleid, -0.524999,0.000000,0.884999,0.000000,0.000000,0.000000);
					kpoli[vehicleid] = true;
				}
			}
			else if(GetVehicleModel(GetPlayerVehicleID(playerid)) == 507)
			{
		    	if(kpoli[vehicleid] == false)
				{
					kogutPOLI[vehicleid] = CreateObject(18646,0,0,-1000,0,0,0,100);
					AttachObjectToVehicle(kogutPOLI[vehicleid], vehicleid, -0.599999,0.000000,0.850000,0.000000,0.000000,0.000000);
					kpoli[vehicleid] = true;
				}
			}
		}
	}
	else if((newkeys & KEY_ANALOG_RIGHT) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		new vehicleid = GetPlayerVehicleID(playerid);

		if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] == TEAM_TYPE_POLICE)
		{
			if(GetVehicleModel(GetPlayerVehicleID(playerid)) == 411 || GetVehicleModel(GetPlayerVehicleID(playerid)) == 426 || GetVehicleModel(GetPlayerVehicleID(playerid)) == 507)
			{
	    		if(kpoli[vehicleid])
				{
	  				DestroyObject(kogutPOLI[vehicleid]);
		    		kpoli[vehicleid] = false;
				}
			}
		}
	}
	else if((newkeys & KEY_ANALOG_UP) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
		cmd_stop(playerid, "");
}

forward SkutyTeleport(playerid, policjantid);
public SkutyTeleport(playerid, policjantid)
{
	if(!IsPlayerConnected(policjantid))
	{
		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
		return KillTimer(PlayerInfo[playerid][chainedTimer]);
	}

	new Float:Pos[3];
	GetPlayerPos(policjantid, Pos[0], Pos[1], Pos[2]);

	if(GetDistancePlayerToPlayer(playerid, policjantid) > 10.0)
		SetPlayerPos(playerid, Pos[0], Pos[1]+1.0, Pos[2]);
		
	return 1;
}

forward AlcoholTest(playerid, trgtid);
public AlcoholTest(playerid, trgtid)
{
	new szString[128], Float:alcohol = (GetPlayerDrunkLevel(trgtid) > 2000 ? (2000): (GetPlayerDrunkLevel(trgtid))) / 400;
	format(szString, sizeof szString, "{FFFFFF}Gracz {82C412}%s{FFFFFF} jest %s{FFFFFF}\nPosiada {82C412}%.02f{FFFFFF}‰ we krwi.", PlayerName(trgtid), (alcohol <= 0.2) ? ("{82C412}trzeÂŸwy") : ("{BA0B0B}pijany"), alcohol);
	Dialog_Show(playerid, DIALOG_ALCOHOL, DIALOG_STYLE_MSGBOX, "Alkomat", szString, "WyjdŸ", #);
	return 1;
}

forward Policja_OnPlayerEnterJob(playerid);
public Policja_OnPlayerEnterJob(playerid)
{
	SetPlayerTeam(playerid, 1);
	GivePlayerWeapon(playerid, 24, 99999);
	GivePlayerWeapon(playerid, 25, 99999);
	GivePlayerWeapon(playerid, 3, 99999);
	return SetPlayerArmour(playerid, 100);
}

forward PolicjaAlert(playerid);
public PolicjaAlert(playerid)
{
	TextDrawHideForPlayer(playerid,AlertTD);
	return KillTimer(timer9[playerid]);
}

forward Unareszt(playerid);
public Unareszt(playerid)
{
	new seconds = GetPVarInt(playerid, "Areszttime");
	new h, m, s;
	ConvertSeconds(seconds,h,m,s);

	if(seconds == 0)
	{
		DeletePVar(playerid,"Areszt");
		DeletePVar(playerid,"Areszttime");
		SetPlayerInterior(playerid, 0);
		SetPlayerPos(playerid, 2286.5684,2431.8369,10.8203);
		SetPlayerHealth(playerid, 100);
		TogglePlayerControllable(playerid, 1);
	}
	else
	{
		seconds--;
		SetPVarInt(playerid, "Areszttime", seconds);
		SetTimerEx("Unareszt", 1000, false, "d", playerid);
	}

	return 1;
}

Dialog:DIALOG_ID_KARTOTEKA(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	return Dialog_Show(playerid, DIALOG_ID_KARTOTEKA_DODAJ, DIALOG_STYLE_INPUT, "Kartoteka > Dodaj wpis", "Wpisz treœæ:", "Dodaj", "Zamknij");
}

Dialog:DIALOG_ID_KARTOTEKA_DODAJ(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new Name[32], tekst[160], wiadomosc[60];

	format(wiadomosc, sizeof(wiadomosc), "%s", inputtext);
	mysql_real_escape_string(PlayerName(playerid), Name);		
	mysql_real_escape_string(wiadomosc, wiadomosc);

	delstr(wiadomosc, "{");
	delstr(wiadomosc, "}");
	delstr(wiadomosc, "\t");
	delstr(wiadomosc, "\n");

	format(tekst, sizeof(tekst), "INSERT INTO `st_kartoteka` (`ownerid`, `policjant`, `text`) VALUES ('%d', '%s', '%s')", PlayerInfo[GetPVarInt(playerid, "Kartoteka_Dodaj")][pID], Name, wiadomosc);
	mysql_query(tekst);

	format(wiadomosc, sizeof(wiadomosc), "%d", GetPVarInt(playerid, "Kartoteka_Dodaj"));
	return cmd_kartoteka(playerid, wiadomosc);
}
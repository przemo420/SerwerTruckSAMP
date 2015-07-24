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
		towarname[40];

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

	format(string, sizeof string, "Nick:\t%s\nPrawo jazdy:\tPOSIADA\nPrzegl¹d:\tWA¯NY\nTowar:\t%s\nWaga:\t%dkg\nPasy:\t%s", PlayerName(targetid), towarname, vehOtherInfo[vehicleid][vCargoWeight], (GetPVarInt(playerid, "PASY") > 0) ? ("zapiête") : ("rozpiête"));
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

forward Policja_OnPlayerKey(playerid, newkeys, oldkeys);
public Policja_OnPlayerKey(playerid, newkeys, oldkeys)
{
	if((newkeys & KEY_ANALOG_LEFT)  && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
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
	
	if((newkeys & KEY_ANALOG_RIGHT)  && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
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
	GivePlayerWeapon(playerid, 24, 99999);
	GivePlayerWeapon(playerid, 25, 99999);
	GivePlayerWeapon(playerid, 3, 99999);
	
	SetPlayerArmour(playerid, 100);

	return 1;
}
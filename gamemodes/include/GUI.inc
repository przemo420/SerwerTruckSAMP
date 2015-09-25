Dialog:NEVER_DIALOG(playerid, response, listitem, inputtext[])
{
	if(response && !IsPlayerLogged(playerid))
		cmd_faq(playerid, "");
    return 1;
}
    
Dialog:DIALOG_ID_CREATE_VEHICLE(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	switch(listitem)
	{
		case 0:
		{
			new szString[512];

			for(new i = 0; i < sizeof cmdCars; i++)
				format(szString, sizeof szString, "%s{FFFFFF}%s\t%s%d\n", szString, cmdCars[i][1], (GetPVarInt(playerid, "LEVEL") >= cmdCars[i][0]) ? ("{82C412}") : ("{BA0B0B}"), cmdCars[i][0]);
			format(szString, sizeof szString, "Nazwa\tWymagany poziom\n%s", szString);
			Dialog_Show(playerid, DIALOG_ID_CREATE_VEHICLE_TRUCK, DIALOG_STYLE_TABLIST_HEADERS, " ", szString, "Wybierz", "Zamknij");
		}
		case 1:
		{
			if(GetPVarInt(playerid, "pojazd") > 0)
			{
				Spawned[GetPVarInt(playerid, "pojazd")] = false;
				ResetVariablesInEnum(vloadInfo[GetPVarInt(playerid, "pojazd")], eLoadVehicle);
				DestroyVehicle(GetPVarInt(playerid, "pojazd"));
				DeletePVar(playerid, "pojazd");
				Msg(playerid, COLOR_INFO, "Stworzone pojazdy zosta³y usuniête.");
			}
		}
	}
	return 1;
}

Dialog:DIALOG_ID_CREATE_VEHICLE_TRUCK(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new 
		vehicleid, 
		Float:pos[4], 
		model = GetModelIdByName(cmdCars[listitem][1]);

	if(cmdCars[listitem][0] > GetPVarInt(playerid, "LEVEL"))
	{
		Msg(playerid, COLOR_ERROR, "Jeszcze nie odblokowa³es tego pojazdu.");
		DIALOG_ID_CREATE_VEHICLE(playerid, true, 0, "");
		return 1;
	}

	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	GetPlayerFacingAngle(playerid, pos[3]);
	DestroyVehicle(GetPVarInt(playerid, "pojazd"));

	vehicleid = CreateVehicle(model, pos[0], pos[1]+5, pos[2], pos[3], 16, 16, -1);

	vloadInfo[vehicleid][vLoaded] = false;
	strdel(vloadInfo[vehicleid][vOwner], 0, strlen (vloadInfo[vehicleid][vOwner]));
	vloadInfo[vehicleid][vCargo] = 0;

	Spawned[vehicleid] = true;
	SetPVarInt(playerid, "pojazd", vehicleid);

	vehInfo_Temp[vehicleid][vFuel] = MaxFuel(GetVehicleModel(vehicleid)) / 2;
	vehInfo_Temp[vehicleid][vFuelType] = IsVehicleTruck(GetVehicleModel(vehicleid)) ? FUEL_TYPE_TDI : FUEL_TYPE_BENZ;
	vehInfo_Temp[vehicleid][vPrzebieg] = -2.0;

	if(GetOnlineTime(playerid) < 7200)
	{
		Msg(playerid, COLOR_INFO2, "W³aœnie stworzy³eœ swój pojazd do pracy. Wpisz {b}/oferta{/b} i wybierz interesuj¹ce Ciê zlecenie!");
	}
	Msg(playerid, COLOR_INFO, "Pomyœlnie utworzono pojazd.");
	PutPlayerInVehicle(playerid, vehicleid, 0);
	SetVehicleParamsEx(vehicleid, 0, 0, 0, 0, 0, 0, 0);
	return 1;
}

Dialog:DIALOG_ID_SPAWN_SELECT(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;
	
	switch(listitem)
	{
		case 0: Teleport(playerid, 1648.4308, -1053.9159, 23.8984, true);
		case 1: Teleport(playerid, 1398.5642, 1009.4103, 10.9297, true);
		case 2: Teleport(playerid, -240.4012, -231.1620, 2.3985, true);
		case 3: Teleport(playerid, -2136.7207, -126.3667, 36.5191, true);
		case 4: Teleport(playerid, firmInfo[playerInfo[playerid][pFirm]][tSpawnX], firmInfo[playerInfo[playerid][pFirm]][tSpawnY], firmInfo[playerInfo[playerid][pFirm]][tSpawnZ], true);
	}

	return 1;
}

stock Teleport(playerid, Float:x, Float:y, Float:z, bool:gtaVCam = false, bool:preStream = false)
{
	if(!gtaVCam)
		SetPlayerPos(playerid, x, y, z);
	else
		if(GetDistanceFromMeToPoint(playerid, x, y, z) >= 500.0)
			SetPlayerPosCamera(playerid, x, y, z);
		else
			SetPlayerPos(playerid, x, y, z);

	if(preStream && !gtaVCam)
		TogglePlayerControllable(playerid, false), SetTimerEx("Unfreeze", 100, false, "i", playerid);
	return 1;
}

forward Unfreeze(playerid);
public Unfreeze(playerid)
{
	return TogglePlayerControllable(playerid, true);
}

Dialog:DIALOG_ID_REGISTER(playerid, response, listitem, inputtext[])
{
	if(!response)
		return SelectTextDraw(playerid, 0xA82A23FF);

	new string[400], Name[32], ip[16], Y, Mo, D, H, Mi, S;
	getdate(Y, Mo, D);
	gettime(H, Mi, S);
	GetPlayerIp(playerid, ip, sizeof(ip));

	if(inputtext[DIALOG_STYLE_PASSWORD])
	{
		if(isnull(inputtext) || strlen(inputtext) <= 5 || restart(inputtext))
		{
			format(string, sizeof(string), "{FFFFFF}Witaj!\nWygl¹da na to, ¿e konto o nazwie {b}%s{/b} nie jest jeszcze zarejestrowane.\nProszê podaj has³o które bêdzie s³u¿y³o do logowania.\nHas³o musi mieæ {b}minimum 5 znaków{/b}\n\n{b}Aby zacz¹æ rozrywkê, konieczne jest posiadanie konta!{/b}", PlayerName(playerid));

			Dialog_Show(playerid, DIALOG_ID_REGISTER, DIALOG_STYLE_PASSWORD, "Panel > Rejestracja", string, "Rejestruj", "Wstecz");
		}
		else
		{
			mysql_real_escape_string(PlayerName(playerid), Name);
			WP_Hash(string, sizeof(string), inputtext);
			playerInfo[playerid][pSpawn] = random(4)-1;
			if(playerInfo[playerid][pSpawn] < 0) playerInfo[playerid][pSpawn] = 0;
			format(string, sizeof string, "INSERT INTO `Accounts` VALUES(NULL, '0', '%d', 'NULL', '%s', '%s', '2000', '0', '100', '0', '-1', '0', '1', '0', '0', '0', '0', NOW(), '0', '%s', '0', '0', '1', '0', '0', '0.0.0.0.0', 'braknickubrak', '1000')", 
				playerInfo[playerid][pSpawn], Name, string, ip);
			mysql_query(string);

			SetTimerEx("ReqSpawnPlayer", 100, false, "i", playerid);

			PlayerLogged(playerid);
			GiveMoney(playerid, 2000);
			GiveScore(playerid, 0);

			playerInfo[playerid][pID] = mysql_insert_id();
			playerInfo[playerid][pBankomat] = -1;
			playerInfo[playerid][pSkin] = 0;
			playerInfo[playerid][pHunger] = 100.0;
			format(pPoints[playerid][pSkills], 15, "0.0.0.0.0");
			format(playerInfo[playerid][pCBNick], 32, "braknickubrak");

			Msg(playerid, COLOR_INFO, "Zarejestrowano pomyœlie. Mi³ej gry ¿yczy administracja. ;)");
			StopCinematicCameraIntro(playerid);
			ShowPlayerConnect(playerid, false);
			ShowConnect(playerid, false);
			ShowPlayerHud(playerid, true);
			TextDrawShowForPlayer(playerid, MainTextDraws[Time]);

			Msg(playerid, COLOR_INFO3, "Wpisz {b}/faq{/b}, by zobaczyæ pomoc.");
			Msg(playerid, COLOR_INFO3, "Wpisz {b}/cmds{/b}, by zobaczyæ komendy.");

			SetSpawnInfo(playerid, 0, playerInfo[playerid][pSkin], gSpawnvehicleplayer[playerInfo[playerid][pSpawn]][0], gSpawnvehicleplayer[playerInfo[playerid][pSpawn]][1], gSpawnvehicleplayer[playerInfo[playerid][pSpawn]][2], 0.0, 0, 0, 0, 0, 0, 0);
			TogglePlayerSpectating(playerid, false);
			SpawnPlayer(playerid);
		}
	}
	return 1;
}

Dialog:DIALOG_ID_VEHICLE(playerid, response, listitem, inputtext[])
{
	if(!response)
	return 1;
	
	new vehicleid = GetPlayerVehicleID(playerid), vehuid = DBVehID[vehicleid], engine, lights, alarm, doors ,bonnet, boot, objective, string[300], Float:sVehHP;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	
	switch(listitem)
	{
		case 0: // Informacje
		{
			GetVehicleHealth(vehicleid, sVehHP);
			
			if(Spawned[vehicleid])
				format(string, sizeof(string), "ID: \t%d\nTyp: \tZESPAWNOWANY\nModel: \t%d\nHP: \t%0.0f\nPaliwo: \t%0.0f\nTyp paliwa: \t%s\nPrzebieg: %0.1fkm", vehicleid, GetVehicleModel(vehicleid), sVehHP, vehInfo_Temp[vehicleid][vFuel],
				(vehInfo_Temp[vehicleid][vFuelType] == FUEL_TYPE_BENZ ? ("Benzyna") : (vehInfo_Temp[vehicleid][vFuelType] == FUEL_TYPE_TDI ? ("Diesel") : ("Gaz"))),
				vehInfo_Temp[vehicleid][vPrzebieg]);
			else
				format(string, sizeof(string), "UID: \t%d\nID: \t%d\nW³aœciciel: \t%d\nSub w³aœciciel: \t%d\nTyp w³aœciciela: \t%s\nModel: \t%d\nHP: \t%0.0f\nPaliwo: \t%0.0f\nTyp paliwa: \t%s\nPrzebieg: %0.1fkm\nKolor: \t%d:%d\nPrzegl¹d: %s\nButla gazowa: \t%s",
				vehuid,
				vehicleid,
				vehInfo[vehuid][vOwnerID],
				vehInfo[vehuid][vOwnerVC],
				(vehInfo[vehuid][vOwnerType] == OWNER_TYPE_PLAYER ? ("Gracz") : ("Firma")),
				vehInfo[vehuid][vModel],
				sVehHP,
				(vehInfo[vehuid][vGasStatus]) ? (vehInfo[vehuid][vGasAmount]) : (vehInfo[vehuid][vFuel]),
				(vehInfo[vehuid][vFuelType] == FUEL_TYPE_BENZ ? ("Benzyna") : (vehInfo[vehuid][vFuelType] == FUEL_TYPE_TDI ? ("Diesel") : ("Gaz"))),
				vehInfo[vehuid][vPrzebieg],
				vehInfo[vehuid][vColor1],
				vehInfo[vehuid][vColor2],
				vehInfo[vehuid][vPrzeglad],
				(vehInfo[vehuid][vGasBootle]) ? ("tak") : ("nie")
				);
			
			Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_TABLIST, "Pojazd > Informacje", string, "Ok", "");
		}
		
		case 1: if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, !bonnet, boot, objective);
		case 2: if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, !boot, objective);
		case 3: if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
		{
			SetVehicleParamsEx(vehicleid, engine, lights, alarm, !doors, bonnet, boot, objective);
			SetVehicleParamsForPlayer(vehicleid, playerid, 0, 0);
		}
		
		case 4: // Zarz¹dzaj
		{
			if( (vehInfo[vehuid][vOwnerType] == OWNER_TYPE_PLAYER && vehInfo[vehuid][vOwnerID] != playerInfo[playerid][pID]) ||
			(vehInfo[vehuid][vOwnerType] == OWNER_TYPE_TEAM && playerInfo[playerid][pID] != firmInfo[playerInfo[playerid][pFirm]][tChef]))
			return ShowInfo(playerid, "Nie mo¿esz zarz¹dzaæ tym pojazdem.");
			
			Dialog_Show(playerid, DIALOG_ID_VEHICLE_ZARZADZAJ, DIALOG_STYLE_LIST, "Pojazd > Zarz¹dzaj", "Zaparkuj\nUstaw zastêpcê%s", "Ok", "Anuluj", (GetVehicleTrailer(vehicleid) > 0) ? ("\nZaparkuj naczepê") : (""));
		}
	}
	return 1;
}

Dialog:DIALOG_ID_VEHICLE_ZARZADZAJ(playerid, response, listitem, inputtext[])
{
	if(!response)
	return 1;
	
	new vehicleid = GetPlayerVehicleID(playerid), vehuid = DBVehID[vehicleid], Float:Pos[4];
	
	switch(listitem)
	{
		case 0:
		{
			GetVehiclePos(vehicleid, Pos[0], Pos[1], Pos[2]);
			GetVehicleZAngle(vehicleid, Pos[3]);
			
			vehInfo[vehuid][vPosX] = Pos[0];
			vehInfo[vehuid][vPosY] = Pos[1];
			vehInfo[vehuid][vPosZ] = Pos[2];
			vehInfo[vehuid][vPosA] = Pos[3];
			
			SaveVehicle(vehicleid);
			DestroyVehicle(vehicleid);
			DBVehID[vehicleid]=0;
			ResetVariablesInEnum(vehInfo[vehuid], E_VEHICLE);
			
			LadujPojazd(_, _, vehuid);
			PutPlayerInVehicle(playerid, vehInfo[vehuid][vSAMPID], 0);
			Msg(playerid, COLOR_INFO, "Pomyœlnie przeparkowano.");
		}
		case 1: ShowInfo(playerid, "Ustawienie zastêpcy jest chwilowo nie mo¿liwe.");
		case 2:
		{
			new trailerid = GetVehicleTrailer(vehicleid);
			if(Spawned[trailerid])
				return Msg(playerid, COLOR_ERROR, "Nie mo¿na zaparkowaæ tej naczepy.");
			vehuid = DBVehID[trailerid];

			DetachTrailerFromVehicle(vehicleid);

			GetVehiclePos(trailerid, Pos[0], Pos[1], Pos[2]);
			GetVehicleZAngle(trailerid, Pos[3]);
			
			vehInfo[vehuid][vPosX] = Pos[0];
			vehInfo[vehuid][vPosY] = Pos[1];
			vehInfo[vehuid][vPosZ] = Pos[2];
			vehInfo[vehuid][vPosA] = Pos[3];
			
			SaveVehicle(trailerid);
			DestroyVehicle(trailerid);
			DBVehID[trailerid]=0;
			ResetVariablesInEnum(vehInfo[vehuid], E_VEHICLE);
			
			LadujPojazd(_, _, vehuid);
			Msg(playerid, COLOR_INFO, "Pomyœlnie przeparkowano.");
		}
	}
	return 1;
}

Dialog:DIALOG_ID_TELEPHONE(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;
	
	if(NaDyzurze[listitem+1] < 1)
	{
		cmd_telefon(playerid, "");
		return Msg(playerid, COLOR_ERROR, "Brak pracowników on-line, spróbuj póŸniej.");
	}

	new szString[48], name[16];

	switch(listitem)
	{
		case 0: strins(name, "policji", 0); 
		case 1: strins(name, "pogotowia", 0);
		case 2: strins(name, "pomocy drogowej", 0);
	}

	SetPVarInt(playerid, "TELEPHONE", listitem+1);
	format(szString, sizeof szString, "Podaj powód dla którego chcesz wezwaæ jednostkê %s oraz Twoj¹ dok³adn¹ lokalizacjê!", name);
	Dialog_Show(playerid, DIALOG_ID_TELEPHONE2, DIALOG_STYLE_INPUT, " ", szString, "Zadzwoñ", "Wstecz");
	return 1;
}

Dialog:DIALOG_ID_TELEPHONE2(playerid, response, listitem, inputtext[])
{
	if(!response)
		return cmd_telefon(playerid, "");

	if(isnull(inputtext) || strlen(inputtext) < 5)
	{
		DIALOG_ID_TELEPHONE(playerid, true, GetPVarInt(playerid, "TELEPHONE")-1, "");
		Msg(playerid, COLOR_ERROR, "Poda³eœ niepoprawne zg³oszenie.");
		return 1;
	}

	new name[16], szString[256];
	switch(GetPVarInt(playerid, "TELEPHONE")-1)
	{
		case 0: strins(name, "policji", 0); 
		case 1: strins(name, "pogotowia", 0);
		case 2: strins(name, "pomocy drogowej", 0);
	}

	format(szString, sizeof szString, "Nowe wezwanie! Gracz {b}%s{/b} potrzebuje {b}%s{/b}. Powód: {b}%s{/b}.", PlayerName(playerid), name, inputtext);

	foreach (new i : Player)
	{
		if(!IsPlayerLogged(i))
			continue;

		if(GetPVarInt(i, "Worked") && firmInfo[playerInfo[i][pFirm]][tType] == GetPVarInt(playerid, "TELEPHONE"))
		{
			Msg(i, COLOR_INFO, szString);
		}
	}
	for(new d = 0; d < MAX_PLAYERS*2; d++)
	{
		if(callInfo[GetPVarInt(playerid, "TELEPHONE")-1][callUsed][d])
			continue;

		callInfo[GetPVarInt(playerid, "TELEPHONE")-1][callUsed][d] = true;
		callInfo[GetPVarInt(playerid, "TELEPHONE")-1][callAssigned][d] = -1;
		callInfo[GetPVarInt(playerid, "TELEPHONE")-1][callPCalled][d] = playerid;
		break;
	}

	DeletePVar(playerid, "TELEPHONE");
	Msg(playerid, COLOR_INFO, "Wezwanie zosta³o zarejestrowane, odpowiednie s³u¿by pojawi¹ siê jak najszybciej!");
	return 1;
}

Dialog:ANIM1_DIALOG(playerid, response, listitem, inputtext[])
{
	if(response == 0)
	{
		Dialog_Show(playerid, ANIM4_DIALOG, DIALOG_STYLE_LIST, " ", "Bomba\nCeluj\nœmiej siê \nMaska\nNapad\nZa?¿ r?e\nLe¿\nChro?siê nWymiotuj\nMachaj\nKlepnij w dup?nUmieraj", "OK", "Dalej");
	}
	if(response)
	{
		switch(listitem)
		{
			case 0:
			{
				Dialog_Show(playerid, ANIM2_DIALOG, DIALOG_STYLE_LIST, " ", "Taniec 1\nTaniec 2\nTaniec 3\nTaniec 4", "OK", "Anuluj");
			}
			case 1:
			{
				SetPlayerSpecialAction(playerid, 10); //R?e
			}
			case 2:
			{
				SetPlayerSpecialAction(playerid, 22); //Wino
			}
			case 3:
			{
				SetPlayerSpecialAction(playerid, 23); //Sprunk
			}
			case 4:
			{
				SetPlayerSpecialAction(playerid, 21); //Cygaro
			}
			case 5:
			{
				SetPlayerSpecialAction(playerid, 20); //Piwo
			}
			case 6:
			{
				Dialog_Show(playerid, ANIM3_DIALOG, DIALOG_STYLE_LIST, " ", "Telefon (wyci?gnij)\nTelefon (schowaj)", "OK", "Anuluj");
			}
			case 7:
			{
				LoopingAnim(playerid,"BEACH", "ParkSit_M_loop", 4.0, 1, 0, 0, 0, 0); //Siad
			}
			case 8:
			{
				OnePlayAnim(playerid, "DEALER", "DEALER_DEAL", 4.0, 0, 0, 0, 0, 0); //Dilowanie
			}
			case 9:
			{
				OnePlayAnim(playerid, "FOOD", "EAT_Burger", 3.0, 0, 0, 0, 0, 0); //Jedzenie
			}
			case 10:
			{
				OnePlayAnim(playerid,"PED","fucku",4.0,0,0,0,0,0); //Fack You
			}
			case 11:
			{
				LoopingAnim(playerid,"PED","WALK_DRUNK",4.0,1,1,1,1,0); //Pijany
			}
		}
	}
	return 1;
}

Dialog:ANIM2_DIALOG(playerid, response, listitem, inputtext[])
{
	if(response)
	{
		switch(listitem)
		{
			case 0:
			{
				SetPlayerSpecialAction(playerid, 5); //Taniec 1
			}
			case 1:
			{
				SetPlayerSpecialAction(playerid, 6); //Taniec 2
			}
			case 2:
			{
				SetPlayerSpecialAction(playerid, 5); //Taniec 3
			}
			case 3:
			{
				SetPlayerSpecialAction(playerid, 5); //Taniec 4
			}
		}
	}
	return 1;
}

Dialog:ANIM3_DIALOG(playerid, response, listitem, inputtext[])
{
	if(response)
	{
		switch(listitem)
		{
			case 0:
			{
				SetPlayerSpecialAction(playerid, 11); //Telefon (wyci¹gnij)
			}
			case 1:
			{
				SetPlayerSpecialAction(playerid, 13); //Telefon (schowaj)
			}
		}
	}
	return 1;
}

Dialog:ANIM4_DIALOG(playerid, response, listitem, inputtext[])
{
	if(response == 0)
	{
		Dialog_Show(playerid, ANIM5_DIALOG, DIALOG_STYLE_LIST, " ", "Rozmawiaj\nSztuka walki\nMyj r?e\nStyle chodzenia >>\nSikaj\nWal konia\nœpij\nPoca?j\nRanny\nOpieraj siê ", "OK", "Zamknij");
	}
	if(response)
	{
		switch(listitem)
		{
			case 0:
			{
				OnePlayAnim(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0); //Bomba
			}
			case 1:
			{
				LoopingAnim(playerid,"ped", "ARRESTgun", 4.0, 0, 1, 1, 1, -1); //Celowanie
			}
			case 2:
			{
				OnePlayAnim(playerid, "RAPPING", "Laugh_01", 4.0, 0, 0, 0, 0, 0); //œmiech
			}
			case 3:
			{
				OnePlayAnim(playerid, "SHOP", "ROB_Shifty", 4.0, 0, 0, 0, 0, 0); //Maska
			}
			case 4:
			{
				LoopingAnim(playerid, "SHOP", "ROB_Loop_Threat", 4.0, 1, 0, 0, 0, 0); //Napad
			}
			case 5:
			{
				LoopingAnim(playerid, "COP_AMBIENT", "Coplook_loop", 4.0, 0, 1, 1, 1, -1); //Za?¿ r?e
			}
			case 6:
			{
				LoopingAnim(playerid,"BEACH", "bather", 4.0, 1, 0, 0, 0, 0); //Le¿
			}
			case 7:
			{
				LoopingAnim(playerid, "ped", "cower", 3.0, 1, 0, 0, 0, 0); //Chro?siê
			}
			case 8:
			{
				OnePlayAnim(playerid, "FOOD", "EAT_Vomit_P", 3.0, 0, 0, 0, 0, 0); //Wymiotuj
			}
			case 9:
			{
				LoopingAnim(playerid, "ON_LOOKERS", "wave_loop", 4.0, 1, 0, 0, 0, 0); //Machaj
			}
			case 10:
			{
				OnePlayAnim(playerid, "SWEET", "sweet_ass_slap", 4.0, 0, 0, 0, 0, 0); //Klepnij w dup?
			}
			case 11:
			{
				LoopingAnim(playerid, "CRACK", "crckdeth2", 4.0, 1, 0, 0, 0, 0); //Umieraj
			}
		}
	}
	return 1;
}

Dialog:ANIM5_DIALOG(playerid, response, listitem, inputtext[])
{
	if(response)
	{
		switch(listitem)
		{
			case 0:
			{
				OnePlayAnim(playerid,"PED","IDLE_CHAT",4.0,0,0,0,0,0); //Rozmawiaj
			}
			case 1:
			{
				LoopingAnim(playerid,"PARK","Tai_Chi_Loop",4.0,1,0,0,0,0); //Sztuka walki
			}
			case 2:
			{
				LoopingAnim(playerid,"INT_HOUSE","wash_up",4.0,0,0,0,0,0); //Myj r?e
			}
			case 3:
			{
				Dialog_Show(playerid, ANIM6_DIALOG, DIALOG_STYLE_LIST, " ", "Kozackie chodzenie\nChodzenie z wyci¹gni?¹ r?¹\nGangster\nNormalne chodzenie\nSkradanie\nGangsterskie chodzenie\nGangsterskie chodzenie [2]\nZgarbiony\nDamskie chodzenie\nDama\nStarzec\nStarzec [2]", "OK", "Dalej");
			}
			case 4:
			{
				LoopingAnim(playerid,"PAULNMAC","Piss_loop",4.0,0,0,0,0,0); //Sikaj
			}
			case 5:
			{
				LoopingAnim(playerid, "PAULNMAC", "wank_loop", 4.0, 1, 0, 0, 0, 0); //Wal konia
			}
			case 6:
			{
				LoopingAnim(playerid, "INT_HOUSE", "BED_Loop_R", 4.0, 1, 0, 0, 0, 0); //œpij
			}
			case 7:
			{
				LoopingAnim(playerid, "KISSING", "Playa_Kiss_02", 4.0, 0, 0, 0, 0, 0); //Poca?j
			}
			case 8:
			{
				LoopingAnim(playerid, "SWEET", "Sweet_injuredloop", 4.0, 1, 0, 0, 1, 0); //Ranny
			}
			case 9:
			{
				LoopingAnim(playerid, "GANGS", "leanIDLE", 4.0, 1, 0, 0, 0, 0); //Opieraj siê
			}
		}
	}
	return 1;
}

Dialog:ANIM6_DIALOG(playerid, response, listitem, inputtext[])
{
	if(response == 0)
	{
		Dialog_Show(playerid, ANIM7_DIALOG, DIALOG_STYLE_LIST, " ", "Panienka\nSkulony", "OK", "Zamknij");
	}
	if(response)
	{
		switch(listitem)
		{
			case 0:
			{
				LoopingAnim(playerid,"MUSCULAR","MuscleWalk",4.0,1,1,1,1,0); //Kozackie chodzenie
			}
			case 1:
			{
				LoopingAnim(playerid,"ped","Walk_Wuzi",4.0,1,1,1,1,0); //Chodzenie z wyci¹gni?¹ r?¹
			}
			case 2:
			{
				LoopingAnim(playerid,"FAT","FatWalk",4.0,1,1,1,1,0); //Gangster
			}
			case 3:
			{
				LoopingAnim(playerid,"ped","WALK_CIVI",4.0,1,1,1,1,0); //Normalne chodzenie
			}
			case 4:
			{
				LoopingAnim(playerid,"ped","WALK_fatold",4.0,1,1,1,1,0); //Skradanie
			}
			case 5:
			{
				LoopingAnim(playerid,"ped","WALK_gang1",4.0,1,1,1,1,0); //Gangsterskie chodzenie
			}
			case 6:
			{
				LoopingAnim(playerid,"ped","WALK_gang2",4.0,1,1,1,1,0); //Gangsterskie chodzenie [2]
			}
			case 7:
			{
				LoopingAnim(playerid,"ped","WALK_old",4.0,1,1,1,1,0); //Zgarbiony
			}
			case 8:
			{
				LoopingAnim(playerid,"ped","WOMAN_walkbusy",4.0,1,1,1,1,0); //Damskie chodzenie
			}
			case 9:
			{
				LoopingAnim(playerid,"ped","WOMAN_walknorm",4.0,1,1,1,1,0); //Dama
			}
			case 10:
			{
				LoopingAnim(playerid,"ped","WOMAN_walkfatold",4.0,1,1,1,1,0); //Starzec
			}
			case 11:
			{
				LoopingAnim(playerid,"ped","WOMAN_walkold",4.0,1,1,1,1,0); //Starzec [2]
			}
		}
	}
	return 1;
}

Dialog:ANIM7_DIALOG(playerid, response, listitem, inputtext[])
{
	if(response)
	{
		switch(listitem)
		{
			case 0:
			{
				LoopingAnim(playerid,"ped","WOMAN_walksexy",4.0,1,1,1,1,0); //Panienka
			}
			case 2:
			{
				LoopingAnim(playerid,"ped","WOMAN_walkshop",4.0,1,1,1,1,0); //Skulony
			}
		}
	}
	return 1;
}

Dialog:POLICE_DIALOG(playerid, response, listitem, inputtext[])
{
	new string[128];

	if(!response)
	{
		format(string, sizeof string, "Gracz {b}%s{/b} odmówi³ przyjêcia mandatu.", PlayerName(playerid));
		Msg(GetPVarInt(playerid,"kontrola"), COLOR_ERROR, string);
		Msg(playerid, COLOR_ERROR, "Odmówi³eœ przyjêcia mandatu. Z twojego konta zosta³ pobrane podwojone punkty score.");
		GiveScore(playerid, -(GetPVarInt(playerid, "opcja2")*2));

		DeletePVar(playerid, "kontrola");
		DeletePVar(playerid, "opcja1");
		DeletePVar(playerid, "opcja2");
	}
	else
	{
		format(string, sizeof string, "Gracz {b}%s{/b} przyj¹³ mandat.", PlayerName(playerid));
		Msg(GetPVarInt(playerid,"kontrola"), COLOR_INFO, string);
		Msg(playerid, COLOR_INFO, "Przyj¹³eœ mandat.");
		GiveMoney(playerid, -GetPVarInt(playerid, "opcja1"));
		GiveScore(playerid, -GetPVarInt(playerid, "opcja2"));

		DeletePVar(playerid, "kontrola");
		DeletePVar(playerid, "opcja1");
		DeletePVar(playerid, "opcja2");
	}
	return 1;
}

Dialog:DIALOG_ID_LOGIN(playerid, response, listitem, inputtext[])
{
	if(!response)
		return SelectTextDraw(playerid, 0xA82A23FF);

	new 
		szBuffer[600+MAX_PLAYER_NAME+129], 
		nick[32], 
		password[129], 
		ip[16];

	if(isnull(inputtext))
	{
		format(szBuffer, sizeof(szBuffer), "{FFFFFF}Witaj!\nWygl¹da na to, ¿e konto o nazwie {b}%s{/b} jest ju¿ zarejestorwane.\nProszê podaj has³o które wpisa³eœ przy rejestracji.\nWykorzysta³eœ ju¿ [{b}%d/3{/b}] prób zalogowania.\n\n{b}Aby zacz¹æ rozrywkê, konieczne jest zalogowanie!{/b}", PlayerName(playerid), GetPVarInt(playerid, "leci"));

		Dialog_Show(playerid, DIALOG_ID_LOGIN, DIALOG_STYLE_PASSWORD, "Panel > Logowanie", clText(COLOR_INFO2, szBuffer), "Zaloguj", "Zamknij");
		return 1;
	}

	GetPlayerIp(playerid, ip, sizeof(ip));

	WP_Hash(password, sizeof(password), inputtext);
	mysql_real_escape_string(PlayerName(playerid), nick);

	format(szBuffer, sizeof szBuffer, "SELECT ID, Money, Score, Hunger, allowedPoints, skills, pSpawn, pSpawnInfo, TimeOnline, cbradioNick, \
		Admin, Bankomat, Firma, Skin, Tacho, Foto, Toll, Gtime, Worktime, BuildMaster, Adr FROM Accounts WHERE Name = '%s' AND Pass = '%s'", nick, password);
	mysql_query(szBuffer);
	mysql_store_result();

	if(mysql_fetch_row(szBuffer, "|")) 
	{
		PlayerLogged(playerid);
		ResetPlayerMoney(playerid);
		ResetMoney(playerid);

		sscanf(szBuffer, "p<|>dddfds[15]ds[32]ds[32]ddddddddddd",
			playerInfo[playerid][pID],
			playerInfo[playerid][pMoney],
			playerInfo[playerid][pScore],
			playerInfo[playerid][pHunger],
			pPoints[playerid][pAllowedPoints],
			pPoints[playerid][pSkills],
			playerInfo[playerid][pSpawn],
			playerInfo[playerid][pSpawnInfo],
			playerInfo[playerid][pOnlineTime],
			playerInfo[playerid][pCBNick],
			playerInfo[playerid][pAdminAllowed],
			playerInfo[playerid][pBankomat],
			playerInfo[playerid][pFirm],
			playerInfo[playerid][pSkin],
			playerInfo[playerid][pTacho],
			playerInfo[playerid][pPhoto],
			playerInfo[playerid][pToll],
			playerInfo[playerid][pGTime],
			playerInfo[playerid][pWorkTime],
			playerInfo[playerid][pBuildmaster],
			playerInfo[playerid][pADR]);

		mysql_free_result();

		if(IsAccountPursued(playerInfo[playerid][pID]))
			playerInfo[playerid][pPursued] = 1;
		playerInfo[playerid][pPursued] = 0;

		format(szBuffer, sizeof(szBuffer), "UPDATE `Accounts` SET `Online`='1', `IP`='%s' WHERE `name` = '%s'", ip, nick);
		mysql_query(szBuffer);

		format(szBuffer, sizeof(szBuffer), "[IP: %s] Zalogowano poprawnie (próba: %d)", ip, GetPVarInt(playerid, "leci"));
		ToLog(playerInfo[playerid][pID], LOG_TYPE_PLAYER, szBuffer);

		GivePlayerMoney(playerid, playerInfo[playerid][pMoney]);
		SetPlayerScore(playerid, playerInfo[playerid][pScore]);

		Msg(playerid, COLOR_INFO, "Zalogowano pomyœlie. Mi³ej gry ¿yczy administracja. ;)");
		//SetPVarInt(playerid, "TireTimer", SetTimerEx_("ExplodePlayerTires", 0, (15+random(15))*1000, 1, "i", playerid));
		StopCinematicCameraIntro(playerid);
		ShowPlayerConnect(playerid, false);
		ShowConnect(playerid, false);
		ShowPlayerHud(playerid, true);
		TextDrawShowForPlayer(playerid, MainTextDraws[Time]);
		SetPlayerProgressBarValue(playerid, hudInfo[tdHungerProgress][playerid], playerInfo[playerid][pHunger]);
		Crash_OnPlayerConnect(playerid);

		SprawdzPoziom(playerid);
		
		if(playerInfo[playerid][pSpawn] >= 0)
			SetSpawnInfo(playerid, 0, playerInfo[playerid][pSkin], gSpawnvehicleplayer[playerInfo[playerid][pSpawn]][0], gSpawnvehicleplayer[playerInfo[playerid][pSpawn]][1], gSpawnvehicleplayer[playerInfo[playerid][pSpawn]][2], 0.0, -1, -1, -1, -1, -1, -1);
		else
		{
			new Float:pos[4];
			sscanf(playerInfo[playerid][pSpawnInfo], "p<,>ffff", pos[0], pos[1], pos[2], pos[3]);
			SetSpawnInfo(playerid, 0, playerInfo[playerid][pSkin], pos[0], pos[1], pos[2], pos[3], 0, 0, 0, 0, 0, 0);
		}
		TogglePlayerSpectating(playerid, false);
		SpawnPlayer(playerid);
		return 1;
	}

	SetPVarInt(playerid, "leci", GetPVarInt(playerid, "leci")+1);

	if(GetPVarInt(playerid, "leci") == 3)
	{
		CheatKick(playerid, "trzy b³êne logowania");
		timer[playerid] = SetTimerEx("Kickplayer", 500, 0, "i", playerid);
	}

	format(szBuffer, sizeof(szBuffer), "{FFFFFF}Witaj!\nWygl¹da na to, ¿e konto o nazwie {b}%s{/b} jest ju¿ zarejestorwane.\nProszê podaj has³o które wpisa³eœ przy rejestracji.\nWykorzysta³eœ ju¿ [{b}%d/3{/b}] prób zalogowania.\n\n{b}Aby zacz¹æ rozrywkê, konieczne jest zalogowanie!{/b}", PlayerName(playerid), GetPVarInt(playerid, "leci"));

	Dialog_Show(playerid, DIALOG_ID_LOGIN, DIALOG_STYLE_PASSWORD, "Panel > Logowanie", clText(COLOR_INFO2, szBuffer), "Zaloguj", "Zamknij");
	return 1;
}

Dialog:DIALOG_ID_FIRMA_INFO(playerid, response, listitem, inputtext[])
{
	if(!response)
		return cmd_firma(playerid, "");

	DIALOG_ID_FIRMA(playerid, true, 1, "");
	return 1;
}

Dialog:DIALOG_ID_FIRMA_PRACOWNICY(playerid, response, listitem, inputtext[])
{
	if(!response)
		return cmd_firma(playerid, "");

	DIALOG_ID_FIRMA(playerid, true, 2, "");
	return 1;
}

Dialog:DIALOG_ID_FIRMA_NORMAL(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	switch(listitem)
	{
		case 0: DIALOG_ID_FIRMA(playerid, true, 1, "");
		case 1: DIALOG_ID_FIRMA(playerid, true, 2, "");
		case 2: DIALOG_ID_FIRMA(playerid, true, 4, "");
	}
	return 1;
}

Dialog:DIALOG_ID_FIRMA(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new szString[512], firmaid = playerInfo[playerid][pFirm];
	switch(listitem)
	{
		case 1: // Informacje ogólne
		{
			format(szString, sizeof(szString), "{b}FID:{/b} \t%d\n{b}Nazwa:{/b} \t%s\n{b}Szef (UID):{/b} \t%d\n{b}Zastêpca szefa (UID):{/b} \t%d\n{b}Typ:{/b} \t%d\n{b}Saldo:{/b} \t%d\n{b}Skiny:{/b} \t%d, %d, %d, %d, %d\n{b}Kolor:{/b} \t{%06x}-- KOLOREK --", firmInfo[firmaid][tID], firmInfo[firmaid][tName], 
				firmInfo[firmaid][tChef], firmInfo[firmaid][tVcChef], firmInfo[firmaid][tType], firmInfo[firmaid][tBank], firmInfo[firmaid][tSkin1], firmInfo[firmaid][tSkin2], firmInfo[firmaid][tSkin3], firmInfo[firmaid][tSkin4], firmInfo[firmaid][tSkin5], firmInfo[firmaid][tColor]>>>8);
			Dialog_Show(playerid, DIALOG_ID_FIRMA_INFO, DIALOG_STYLE_TABLIST, "Firma > Informacje ogólne", clText(COLOR_INFO2, szString), "Wybierz", "Cofnij");
		}
		case 2: // Pracownicy online
		{
			strcat(szString, "ID\tUID\tNick\n");

			for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
		    	if(playerInfo[i][pFirm] == firmaid)
		    		format(szString, sizeof(szString), "%s{b}%d{/b}\t%d\t%s\n", szString, i, playerInfo[i][pID], PlayerName(i));
		    Dialog_Show(playerid, DIALOG_ID_FIRMA_PRACOWNICY, DIALOG_STYLE_TABLIST_HEADERS, "Firma > Pracownicy online", clText(COLOR_INFO2, szString), "Ok", "Cofnij");
		}

		case 4: // Wp³aæ pieni¹dze
			Dialog_Show(playerid, DIALOG_ID_FIRMA_WPLAC, DIALOG_STYLE_INPUT, "Firma > Wp³aæ pieni¹dze", clText(COLOR_INFO, "{FFFFFF}WprowadŸ iloœæ pieniêdzy, które chcesz wp³aciæ:"), "Wp³aæ", "Cofnij");

		case 5: // Wyp³aæ pieni¹dze
		{
			Msg(playerid, COLOR_ERROR, "Funkcja tymczasowo zablokowana.");
			cmd_firma(playerid, "");
			return 1;
		}

		case 6: // Wyp³aty
		{
			new day[4];
			getdate(day[2], day[1], day[0]);
			day[3] = GetWeekDay(day[0], day[1], day[2]);
			if(day[3] != 6)
			{
				Msg(playerid, COLOR_ERROR, "Wyp³aty mo¿esz rozdawaæ tylko w sobotê.");
				return cmd_firma(playerid, "");
			}
			new szBuffer[256], nums, nick[32], other[3], money, score;
			format(szString, sizeof szString, "SELECT ID, Name, Worktime, rate FROM Accounts WHERE Firma = '%d'", playerInfo[playerid][pFirm]);
			mysql_query(szString);
			mysql_store_result();
			nums = mysql_num_rows();
			szString[0] = EOS;
			for(new i = 0; i < nums; i++)
			{
				if(mysql_fetch_row(szBuffer, "|"))
				{
					sscanf(szBuffer, "p<|>ds[32]dd", other[0], nick, other[1], other[2]);
					money = other[1] * other[2];
					score = floatround(other[1] * (other[2] / 1000));

					if(IsFirm(playerInfo[playerid][pFirm]))
					{
						if(money > 25000)
							money = 25000;
						score = 0;
					}
					else
					{
						if(money > 75000)
							money = 75000;
						if(score > 75)
							score = 75;
					}

					format(szString, sizeof szString, "%s%d - %s\t$ %d\t%02d:%02d:00\t$ %d i %d score\n", szString, other[0], nick, other[2], other[1]/60, other[1]%60, money, score);
				}
			}
			mysql_free_result();
			strins(szString, "Gracz\tStawka\tCzas gry\tZarobek\n", 0);
			Dialog_Show(playerid, DIALOG_ID_PAYMENTS, DIALOG_STYLE_TABLIST_HEADERS, "Firma > Wyp³aty", szString, "Wybierz", "Cofnij");
		}

		case 8: // Zaproœ gracza
			Dialog_Show(playerid, DIALOG_ID_FIRMA_ZAPROS, DIALOG_STYLE_INPUT, "Firma > Zatrudnij", clText(COLOR_INFO, "{FFFFFF}WprowadŸ ID gracza, którego chcesz zaprosiæ:"), "Zaproœ", "Cofnij");

		case 9: // Wyrzuæ gracza
			Dialog_Show(playerid, DIALOG_ID_FIRMA_WYRZUC, DIALOG_STYLE_INPUT, "Firma > Zwolnij", clText(COLOR_INFO, "{FFFFFF}WprowadŸ nazwê gracza, którego chcesz wyrzuciæ:"), "Zwolnij", "Cofnij");


		case 10: // lista pracowników
		{
			new szBuffer[64], nums, nick[32], rate, id;
			format(szString, sizeof szString, "SELECT ID, Name, rate FROM Accounts WHERE Firma = '%d'", playerInfo[playerid][pFirm]);
			mysql_query(szString);
			szString[0] = EOS;
			mysql_store_result();
			nums = mysql_num_rows();
			for(new i = 0; i < nums; i++)
			{
				if(mysql_fetch_row(szBuffer, "|"))
				{
					sscanf(szBuffer, "p<|>ds[32]d", id, nick, rate);
					format(szString, sizeof szString, "%s%d\t%s\t$ %d\n", szString, id, nick, rate);
				}
			}
			mysql_free_result();
			strins(szString, "UID\tGracz\tStawka\t\n", 0);
			Dialog_Show(playerid, DIALOG_ID_WORKERS, DIALOG_STYLE_TABLIST_HEADERS, "Firma > Lista pracowników", szString, "Wybierz", "Cofnij");
		}

		case 12: // Ustaw spawn
		{
			new Float:Pos[3];

			GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);
			firmInfo[firmaid][tSpawnX] = Pos[0];
			firmInfo[firmaid][tSpawnY] = Pos[1];
			firmInfo[firmaid][tSpawnZ] = Pos[2];

			Msg(playerid, COLOR_INFO, "Zmieni³eœ spawn firmy.");
			cmd_firma(playerid, "");
		}

		case 13: // Zmieñ skina
		{
			format(szString, sizeof(szString), "Skin 1 (%d)\nSkin 2 (%d)\nSkin 3 (%d)\nSkin 4 (%d)\nSkin 5 (%d)", firmInfo[firmaid][tSkin1], firmInfo[firmaid][tSkin2], firmInfo[firmaid][tSkin3], firmInfo[firmaid][tSkin4], firmInfo[firmaid][tSkin5]);
			Dialog_Show(playerid, DIALOG_ID_FIRMA_SKIN, DIALOG_STYLE_LIST, "Firma > Skiny", szString, "Zmieñ", "Cofnij");
		}

		case 14: // Lista pojazdów
		{
			Msg(playerid, COLOR_ERROR, "Funkcja tymczasowo zablokowana.");
			cmd_firma(playerid, "");
			return 1;
		}

		case 15: // Zastêpca
		{
			if(firmInfo[firmaid][tChef] != playerInfo[playerid][pID])
			{
				cmd_firma(playerid, "");
				return Msg(playerid, COLOR_ERROR, "Tylko szef mo¿e zmieniæ swojego zastêpce.");
			}

			Dialog_Show(playerid, DIALOG_ID_FIRMA_ZASTEPCA, DIALOG_STYLE_INPUT, "Firma > Zastêpca", clText(COLOR_INFO, "{FFFFFF}WprowadŸ ID gracza, którego chcesz mianowaæ na zastêpcê (-1 by usun¹æ):"), "Wybierz", "Cofnij");
		}
		default: return cmd_firma(playerid, "");
	}
	return 1;
}

Dialog:DIALOG_ID_WORKERS(playerid, response, listitem, inputtext[])
{
	if(!response)
		return cmd_firma(playerid, "");

	new id;
	sscanf(inputtext, "d{s[64]}", id);
	SetPVarInt(playerid, "ClicledID", id);
	Dialog_Show(playerid, DIALOG_ID_WORKERS_SET, DIALOG_STYLE_INPUT, "Firma > Lista pracowników > Stawka", clText(COLOR_INFO2, "{FFFFFF}WprowadŸ stawkê za jak¹ ma pracowaæ Twój pracownik.\nPamiêtaj aby nie by³a ona zbyt niska, ani wysoka.\n \nMinimalna stawka - {b}50${/b}"), "Dalej", "Wstecz");
	return 1;
}

Dialog:DIALOG_ID_WORKERS_SET(playerid, response, listitem, inputtext[])
{
	if(!response)
		return DIALOG_ID_FIRMA(playerid, true, 10, "");

	if(!IsNumeric(inputtext))
		return Dialog_Show(playerid, DIALOG_ID_WORKERS_SET, DIALOG_STYLE_INPUT, "Firma > Lista pracowników > Stawka", clText(COLOR_INFO2, "{b}WprowadŸ prawid³ow¹ liczbê{/b}\n \nWprowadŸ stawkê za jak¹ ma pracowaæ Twój pracownik.\nPamiêtaj aby nie by³a ona zbyt niska, ani wysoka.\n \nMinimalna stawka - {b}50${/b}"), "Dalej", "Wstecz");

	if(IsFirm(playerInfo[playerid][pFirm]))
	{
		if(strval(inputtext) >= 150 || strval(inputtext) < 50)
			return Dialog_Show(playerid, DIALOG_ID_WORKERS_SET, DIALOG_STYLE_INPUT, "Firma > Lista pracowników > Stawka", clText(COLOR_INFO2, "{b}Podaj prawid³ow¹ stawkê.{/b}\n \nWprowadŸ stawkê za jak¹ ma pracowaæ Twój pracownik.\nPamiêtaj aby nie by³a ona zbyt niska, ani wysoka.\n \nMinimalna stawka - {b}50${/b}"), "Dalej", "Wstecz");
	}
	else
	{
		if(strval(inputtext) >= 330 || strval(inputtext) < 50)
			return Dialog_Show(playerid, DIALOG_ID_WORKERS_SET, DIALOG_STYLE_INPUT, "Firma > Lista pracowników > Stawka", clText(COLOR_INFO2, "{b}Podaj prawid³ow¹ stawkê.{/b}\n \nWprowadŸ stawkê za jak¹ ma pracowaæ Twój pracownik.\nPamiêtaj aby nie by³a ona zbyt niska, ani wysoka.\n \nMinimalna stawka - {b}50${/b}"), "Dalej", "Wstecz");
	}

	new id = GetPVarInt(playerid, "ClicledID"), szString[128];
	format(szString, sizeof szString, "UPDATE Accounts SET rate = '%d' WHERE ID = '%d'", strval(inputtext), id);
	mysql_query(szString);

	Msg(playerid, COLOR_INFO, "Stawka zosta³a zmieniona pomyœlnie.");
	DeletePVar(playerid, "ClicledID");
	DIALOG_ID_FIRMA(playerid, true, 10, "");
	return 1;
}

Dialog:DIALOG_ID_PAYMENTS(playerid, response, listitem, inputtext[])
{
	if(!response)
		return cmd_firma(playerid, "");

	new szString[128], szBuffer[128], id, time, money, score, nick[32], rate;
	sscanf(inputtext, "d{s[128]}", id);
	format(szString, sizeof szString, "SELECT Name, Worktime, rate FROM Accounts WHERE ID = '%d'", id);
	mysql_query(szString);
	mysql_store_result();
	if(mysql_fetch_row(szBuffer, "|"))
	{
		sscanf(szBuffer, "p<|>s[32]dd", nick, time, rate);
		if(time < 10)
		{
			mysql_free_result();
			return Msg(playerid, COLOR_ERROR, "Gracz pracowa³ za krótko aby otrzymaæ wyp³atê.");
		}
		format(szString, sizeof szString, "UPDATE Accounts SET Worktime = '0' WHERE Name = '%s';", nick);
		mysql_query(szString);

		new online = GetIDByNick(nick);
		if(online > -1)
		{
			time = playerInfo[playerid][pWorkTime];
			playerInfo[online][pWorkTime] = 0;
			Msg(online, COLOR_INFO, "Twoja wyp³ata jest ju¿ gotowa! {b}Udaj siê do banku aby j¹ odebraæ!{/b}");
		}
		money = time * rate;
		score = floatround(time * (rate / 1000));
	}
	mysql_free_result();
	if(IsFirm(playerInfo[playerid][pFirm]))
	{
		if(money > 25000)
			money = 25000;
		score = 0;
	}
	else
	{
		if(money > 75000)
			money = 75000;
		if(score > 75)
			score = 75;
	}
	format(szString, sizeof szString, "Da³eœ wyp³atê w wysokoœci ${b}%d{/b} i {b}%d{/b} score graczowi {b}%s{/b}.", money, score, nick);
	Msg(playerid, COLOR_INFO2, szString);

	format(szString, sizeof szString, "INSERT INTO payments (ID, Name, money, score) VALUES ('null', '%s', '%d', '%d')", nick, money, score);
	mysql_query(szString);

	DIALOG_ID_FIRMA(playerid, true, 6, "");
	return 1;
}

Dialog:DIALOG_ID_FIRMA_WPLAC(playerid, response, listitem, inputtext[])
{
	if(!response)
	return cmd_firma(playerid, "");
	
	new kasa = strval(inputtext);
	
	if(!IsNumeric(inputtext))
		return Dialog_Show(playerid, DIALOG_ID_FIRMA_WPLAC, DIALOG_STYLE_INPUT, "Firma > Wp³aæ", clText(COLOR_ERROR, "{b}Podaj prawid³ow¹ liczbê.{/b}\n \nWprowadŸ iloœæ pieniêdzy, które chcesz wp³aciæ:"), "Wp³aæ", "Cofnij");
	
	if(kasa > GetMoney(playerid))
		return Dialog_Show(playerid, DIALOG_ID_FIRMA_WPLAC, DIALOG_STYLE_INPUT, "Firma > Wp³aæ", clText(COLOR_ERROR, "{b}Nie posiadasz tyle pieniêdzy.{/b}\n \nWprowadŸ iloœæ pieniêdzy, które chcesz wp³aciæ:"), "Wp³aæ", "Cofnij");
	
	if(kasa < 0 || kasa > 1000000)
		return Dialog_Show(playerid, DIALOG_ID_FIRMA_WPLAC, DIALOG_STYLE_INPUT, "Firma > Wp³aæ", clText(COLOR_ERROR, "{b}Poda³eœ z³¹ liczbê.{/b}\n \nWprowadŸ iloœæ pieniêdzy, które chcesz wp³aciæ:"), "Wp³aæ", "Cofnij");

	GiveMoney(playerid, -kasa);
	firmInfo[playerInfo[playerid][pFirm]][tBank] += kasa;
	
	Dialog_Show(playerid, DIALOG_ID_FIRMA_WPLAC, DIALOG_STYLE_INPUT, "Firma > Wp³aæ", clText(COLOR_INFO, "{b}Wp³acono pieni¹dze na konto{/b}\n \nWprowadŸ iloœæ pieniêdzy, które chcesz wp³aciæ:"), "Wp³aæ", "Cofnij");
	return 1;
}

Dialog:DIALOG_ID_FIRMA_ZAPROS(playerid, response, listitem, inputtext[])
{
	if(!response)
		return cmd_firma(playerid, "");
	
	new forplayerid = strval(inputtext), string[80];
	
	if(!IsNumeric(inputtext))
		return Dialog_Show(playerid, DIALOG_ID_FIRMA_ZAPROS, DIALOG_STYLE_INPUT, "Firma > Zatrudnij", clText(COLOR_ERROR, "{b}WprowadŸ prawid³ow¹ liczbê.{/b}.\n \nWprowadŸ ID gracza, którego chcesz zaprosiæ:"), "Zaproœ", "Cofnij");

	if(!IsPlayerConnected(forplayerid))
		return Dialog_Show(playerid, DIALOG_ID_FIRMA_ZAPROS, DIALOG_STYLE_INPUT, "Firma > Zatrudnij", clText(COLOR_ERROR, "{b}Gracz nie jest na serwerze{/b}.\n \nWprowadŸ ID gracza, którego chcesz zaprosiæ:"), "Zaproœ", "Cofnij");
	
	if(playerInfo[forplayerid][pFirm] == playerInfo[playerid][pFirm])
		return Dialog_Show(playerid, DIALOG_ID_FIRMA_ZAPROS, DIALOG_STYLE_INPUT, "Firma > Zatrudnij", clText(COLOR_ERROR, "{b}Gracz jest ju¿ zatrudniony.{/b}\n \nWprowadŸ ID gracza, którego chcesz zaprosiæ:"), "Zaproœ", "Cofnij");
	
	SetPVarInt(forplayerid, "FIRMA_ZAPROS", playerid);
	format(string, sizeof(string), "Gracz {b}%s{/b} zaprosi³ Ciê do firmy / frakcji {b}%s{/b}.", PlayerName(playerid), firmInfo[playerInfo[playerid][pFirm]][tName]);
	Dialog_Show(forplayerid, DIALOG_ID_FIRMA_ZAPROS_ACCEPT, DIALOG_STYLE_MSGBOX, "Firma > Zatrudnij", clText(COLOR_INFO, string), "Akceptuj", "Odrzuæ");
	Dialog_Show(playerid, DIALOG_ID_FIRMA_ZAPROS, DIALOG_STYLE_INPUT, "Firma > Zatrudnij", clText(COLOR_INFO, "{b}Pomyœlnie wys³ano zaproszenie.{/b}\n \nWprowadŸ ID gracza, którego chcesz zaprosiæ:"), "Zaproœ", "Cofnij");
	return 1;
}

Dialog:DIALOG_ID_FIRMA_ZAPROS_ACCEPT(playerid, response, listitem, inputtext[])
{
	if(!response)
		return Msg(GetPVarInt(playerid, "FIRMA_ZAPROS"), COLOR_INFO, "Gracz odrzuci³ zaproszenie do Twojej firmy.");
	
	playerInfo[playerid][pFirm] = playerInfo[GetPVarInt(playerid, "FIRMA_ZAPROS")][pFirm];
	OnPlayerText(playerid, "#Przyj¹³em zaproszenie do firmy.");
	return 1;
}

Dialog:DIALOG_ID_FIRMA_WYRZUC(playerid, response, listitem, inputtext[])
{
	if(!response)
		return cmd_firma(playerid, "");

	if(strlen(inputtext) < 5 || strlen(inputtext) > 32)
		return Dialog_Show(playerid, DIALOG_ID_FIRMA_WYRZUC, DIALOG_STYLE_INPUT, "Firma > Zwolnij", clText(COLOR_ERROR, "{b}WprowadŸ prawid³ow¹ nazwê gracza{/b}\n \nWprowadŸ nazwê gracza, którego chcesz wyprosiæ:"), "Zwolnij", "Cofnij");
	
	if(!IsAccountExists(inputtext))
		return Dialog_Show(playerid, DIALOG_ID_FIRMA_WYRZUC, DIALOG_STYLE_INPUT, "Firma > Zwolnij", clText(COLOR_ERROR, "{b}Nie odnaleziono takiego gracza{/b}\n \nWprowadŸ nazwê gracza, którego chcesz wyprosiæ:"), "Zwolnij", "Cofnij");
	
	if(!strcmp(PlayerName(playerid), inputtext))
		return Dialog_Show(playerid, DIALOG_ID_FIRMA_WYRZUC, DIALOG_STYLE_INPUT, "Firma > Zwolnij", clText(COLOR_ERROR, "{b}Nie mo¿esz zwolniæ samego siebie{/b}\n \nWprowadŸ nazwê gracza, którego chcesz wyprosiæ:"), "Zwolnij", "Cofnij");
	
	new szString[128], firmid;
	format(szString, sizeof szString, "SELECT Firma FROM Accounts WHERE Name = '%s';", inputtext);
	mysql_query(szString);
	mysql_store_result();
	firmid = mysql_fetch_int();
	mysql_free_result();

	if(firmid != playerInfo[playerid][pFirm])
		return Dialog_Show(playerid, DIALOG_ID_FIRMA_WYRZUC, DIALOG_STYLE_INPUT, "Firma > Zwolnij", clText(COLOR_ERROR, "{b}Gracz nie pracuje w Twojej firmie{/b}\n \nWprowadŸ nazwê gracza, którego chcesz wyprosiæ:"), "Zwolnij", "Cofnij");
	
	new online = GetIDByNick(inputtext);
	if(online > -1)
	{
		Msg(online, COLOR_ERROR, "Zosta³eœ zwolniony z pracy!");
		playerInfo[online][pFirm] = 0;
	}

	format(szString, sizeof szString, "UPDATE Accounts SET Firma = '0' WHERE Name = '%s'", inputtext);
	Dialog_Show(playerid, DIALOG_ID_FIRMA_WYRZUC, DIALOG_STYLE_INPUT, "Firma > Zwolnij", clText(COLOR_INFO, "{b}Pomyœlnie zwioniono.{/b}\n \nWprowadŸ nazwê gracza, którego chcesz wyprosiæ:"), "Zwolnij", "Cofnij");
	return 1;
}

Dialog:DIALOG_ID_FIRMA_ZASTEPCA(playerid, response, listitem, inputtext[])
{
	if(!response)
	return cmd_firma(playerid, "");
	
	new forplayerid = strval(inputtext), string[80], firmaid = playerInfo[playerid][pFirm];
	
	if(forplayerid == -1)
	{
		firmInfo[firmaid][tVcChef] = 0;
		
		format(string, sizeof(string), "UPDATE `Firmy` SET `VcSzef` = '0' WHERE `id` = '%d'", firmaid);
		mysql_query(string);
		
		Dialog_Show(playerid, DIALOG_ID_FIRMA_ZASTEPCA, DIALOG_STYLE_INPUT, "Firma > Zastêpca", clText(COLOR_INFO, "{b}Pomyœlnie usuniêto zastêpcê.{/b}\n \nWprowadŸ ID gracza, którego chcesz mianowaæ na zastêpcê (-1 by usun¹æ):"), "Wybierz", "Cofnij");	
		return 1;
	}
	
	if(!IsPlayerConnected(forplayerid))
		return Dialog_Show(playerid, DIALOG_ID_FIRMA_ZASTEPCA, DIALOG_STYLE_INPUT, "Firma > Zastêpca", clText(COLOR_ERROR, "{b}Gracz jest offline.{/b}\n \nWprowadŸ ID gracza, którego chcesz mianowaæ na zastêpcê (-1 by usun¹æ):"), "Wybierz", "Cofnij");	
	
	if(playerInfo[forplayerid][pFirm] != playerInfo[playerid][pFirm])
		return Dialog_Show(playerid, DIALOG_ID_FIRMA_ZASTEPCA, DIALOG_STYLE_INPUT, "Firma > Zastêpca", clText(COLOR_ERROR, "{b}Gracz nie pracuje w Twojej firmie.{/b}\n \nWprowadŸ ID gracza, którego chcesz mianowaæ na zastêpcê (-1 by usun¹æ):"), "Wybierz", "Cofnij");	
	
	if(!strcmp(PlayerName(playerid), PlayerName(forplayerid)))
		return Dialog_Show(playerid, DIALOG_ID_FIRMA_ZASTEPCA, DIALOG_STYLE_INPUT, "Firma > Zwolnij", clText(COLOR_ERROR, "{b}Nie mo¿esz tego zrobiæ.{/b}\n \nWprowadŸ ID gracza, którego chcesz mianowaæ na zastêpcê (-1 by usun¹æ):"), "Zwolnij", "Cofnij");
	
	format(string, sizeof(string), "Zosta³eœ mianowany przez {b}%s{/b} na zastêpcê firmy {b}%s{/b}.", PlayerName(playerid), firmInfo[firmaid][tName]);
	Msg(forplayerid, COLOR_INFO2, string);
	
	format(string, sizeof(string), "{b}Mianowa³eœ gracza %s na swojego zastêpcê.{/b}\n \nWprowadŸ ID gracza, którego chcesz mianowaæ na zastêpcê (-1 by usun¹æ):", PlayerName(forplayerid));
	Dialog_Show(playerid, DIALOG_ID_FIRMA_ZASTEPCA, DIALOG_STYLE_INPUT, "Firma > Zastêpca", clText(COLOR_INFO, string), "Wybierz", "Cofnij");	
	
	firmInfo[firmaid][tVcChef] = playerInfo[forplayerid][pID];
	
	format(string, sizeof(string), "UPDATE `Firmy` SET `VcSzef` = '%d' WHERE `id` = '%d'", playerInfo[forplayerid][pID], firmaid);
	mysql_query(string);
	return 1;
}

Dialog:DIALOG_ID_FIRMA_SKIN(playerid, response, listitem, inputtext[])
{
	if(!response)
	return cmd_firma(playerid, "");
	
	SetPVarInt(playerid, "FIRMA_SKIN", listitem+1);
	
	new string[80];
	format(string, sizeof(string), "Zmieniasz skin: Skin %d\n\nWprowadŸ numer skina:", listitem+1);
	Dialog_Show(playerid, DIALOG_ID_FIRMA_SKIN_ZMIEN, DIALOG_STYLE_INPUT, "Firma > Skin > Zmieñ", string, "Ok", "Cofnij");
	return 1;
}

Dialog:DIALOG_ID_FIRMA_SKIN_ZMIEN(playerid, response, listitem, inputtext[])
{
	if(!response)
	return cmd_firma(playerid, "");
	
	new skinid = strval(inputtext), firmaid = playerInfo[playerid][pFirm], string[80];
	if(skinid < 0 || skinid > 311)
	return ShowInfo(playerid, "Nieprawid³owe ID skina.");
	
	format(string, sizeof(string), "Zmieni³eœ skin numer %d na %d.", GetPVarInt(playerid, "FIRMA_SKIN"), skinid);
	ShowInfo(playerid, string);
	
	switch(GetPVarInt(playerid, "FIRMA_SKIN"))
	{
		case 1:firmInfo[firmaid][tSkin1] = skinid;
		case 2: firmInfo[firmaid][tSkin2] = skinid;
		case 3: firmInfo[firmaid][tSkin3] = skinid;
		case 4: firmInfo[firmaid][tSkin4] = skinid;
		case 5: firmInfo[firmaid][tSkin5] = skinid;
	}
	
	format(string, sizeof(string), "UPDATE `Firmy` SET `Skin%d` = '%d' WHERE `id` = '%d'", GetPVarInt(playerid, "FIRMA_SKIN"), skinid, firmaid);
	mysql_query(string);
	printf(string);
	return 1;
}
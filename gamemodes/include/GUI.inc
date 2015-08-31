Dialog:NEVER_DIALOG(playerid, response, listitem, inputtext[])
{
	if(!IsPlayerLogged(playerid))
		SelectTextDraw(playerid, 0xA82A23FF);
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

	Spawned[vehicleid] = true;
	SetPVarInt(playerid, "pojazd", vehicleid);

	vehInfo_Temp[vehicleid][vFuel] = MaxFuel(GetVehicleModel(vehicleid)) / 2;
	vehInfo_Temp[vehicleid][vFuelType] = IsVehicleTruck(GetVehicleModel(vehicleid)) ? FUEL_TYPE_TDI : FUEL_TYPE_BENZ;
	vehInfo_Temp[vehicleid][vPrzebieg] = -2.0;

	Msg(playerid, COLOR_INFO, "Pomyœlnie utworzono pojazd.");
	PutPlayerInVehicle(playerid, vehicleid, 0);
	return 1;
}

Dialog:DIALOG_ID_SPAWN_SELECT(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;
	
	switch(listitem)
	{
		case 0: SetPlayerPos(playerid,1648.4308,-1053.9159,23.8984);
		case 1: SetPlayerPos(playerid,1398.5642,1009.4103,10.9297);
		case 2: SetPlayerPos(playerid,-240.4012,-231.1620,2.3985);
		case 3: SetPlayerPos(playerid,-2136.7207,-126.3667,36.5191);
		case 4: SetPlayerPos(playerid, firmInfo[playerInfo[playerid][pFirm]][tSpawnX], firmInfo[playerInfo[playerid][pFirm]][tSpawnY], firmInfo[playerInfo[playerid][pFirm]][tSpawnZ]);
	}

	return 1;
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
			format(string, sizeof(string), "{FFFFFF}Serwer-Truck SAMP - Rejestracja konta.\n\n{a9c4e4}Witaj na naszym serwerze.\n{a9c4e4}Je¿eli chcesz zacz¹æ rozgrywkê zarejestruj konto.\n");
			strcat(string, "{a9c4e4}Zapamiêtaj dobrze swoje has³o, bo bêdzie wymagane po ponownym wejœciu na serwer.\nHas³o musi mieæ co najmniej 5 znaków.\n\n");
			format(string, sizeof(string), "%s{a9c4e4}Nick który chcesz zarejestrowaæ: {FFFFFF}%s\n\n{FFFFFF}www.serwertruck.eu", string, PlayerName(playerid));

			Dialog_Show(playerid, DIALOG_ID_REGISTER, DIALOG_STYLE_PASSWORD, "Panel > Rejestracja", string, "Rejestruj", "");
		}
		else
		{
			mysql_real_escape_string(PlayerName(playerid), Name);
			WP_Hash(string, sizeof(string), inputtext);
			format(string, sizeof string, "INSERT INTO `Accounts` VALUES(NULL, '0', '%s', '%s', '2000', '0', '0', '-1', '0', '1', '0', '0', '0', '0', '%d', '%d', '%d', '%d', '%d', '0', '%s', '0', '0', '1', '0', '0')", 
				Name, string, H, Mi, D, Mo, Y, ip);

			mysql_query(string);

			SetTimerEx("ReqSpawnPlayer", 100, false, "d", playerid);

			PlayerLogged(playerid);
			GiveMoney(playerid, 2000);
			GiveScore(playerid, 0);

			playerInfo[playerid][pID] = mysql_insert_id();
			playerInfo[playerid][pBankomat] = -1;
			playerInfo[playerid][pSkin] = 0;

			Msg(playerid, COLOR_INFO, "Zarejestrowano pomyœlie. Mi³ej gry ¿yczy administracja. ;)");
			StopCinematicCameraIntro(playerid);
			ShowPlayerConnect(playerid, false);
			ShowConnect(playerid, false);
			TextDrawShowForPlayer(playerid, MainTextDraws[Time]);

			SetSpawnInfo(playerid, 0, playerInfo[playerid][pSkin], 1648.4308, -1053.9159, 23.8984, 0.0, 0, 0, 0, 0, 0, 0);
			TogglePlayerSpectating(playerid, false);
			SpawnPlayer(playerid);
		}
	}
	return 1;
}

Dialog:DIALOG_ID_FIRMA(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new string[200], firmaid = playerInfo[playerid][pFirm];
	switch(listitem)
	{
		case 0: // Informacje
		{
			format(string, sizeof(string), "UID: \t%d\nNazwa: \t%s\nSzef (UID): \t%d\nVC Szef (UID): \t%d\nKsiegowy (UID): \t%d\nTyp: \t%d\nSaldo: \t%d\nSkin1: \t%d\nSkin2: \t%d\nSkin3: \t%d\nSkin4: \t%d\nSkin5: \t%d\nKolor: \t{%06x}-- KOLOREK --", firmInfo[firmaid][tID], firmInfo[firmaid][tName], 
				firmInfo[firmaid][tChef], firmInfo[firmaid][tVcChef], firmInfo[firmaid][tAccountant], firmInfo[firmaid][tType], firmInfo[firmaid][tBank], firmInfo[firmaid][tSkin1], firmInfo[firmaid][tSkin2], firmInfo[firmaid][tSkin3], firmInfo[firmaid][tSkin4], firmInfo[firmaid][tSkin5], firmInfo[firmaid][tColor]>>>8);
			Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_TABLIST, "Firma > Informacje", string, "Ok", "");
		}
		case 1: // Pracownicy online
		{
			strcat(string, "ID\tUID\tNick\n");

			for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
		    	if(playerInfo[i][pFirm] == firmaid)
		    		format(string, sizeof(string), "%s%d\t%d\t%s\n", string, i, playerInfo[i][pID], PlayerName(i));
		    Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_TABLIST_HEADERS, "Firma > Pracownicy", string, "Ok", "");
		}

		case 2: // Wp³aæ pieni¹dze
			Dialog_Show(playerid, DIALOG_ID_FIRMA_WPLAC, DIALOG_STYLE_INPUT, "Firma > Wp³aæ", "WprowadŸ iloœæ pieniêdzy, które chcesz wp³aciæ:", "Ok", "Cofnij");

		case 4: // Rozdaj wyp³aty
			ShowInfo(playerid, "Chwilowo niedostêpne.");

		case 5: // Zaproœ gracza
			Dialog_Show(playerid, DIALOG_ID_FIRMA_ZAPROS, DIALOG_STYLE_INPUT, "Firma > Zaproœ", "WprowadŸ ID gracza, którego chcesz zaprosiæ:", "Ok", "Cofnij");

		case 6: // Wyrzuæ gracza
			Dialog_Show(playerid, DIALOG_ID_FIRMA_WYRZUC, DIALOG_STYLE_INPUT, "Firma > Wyrzuæ", "WprowadŸ ID gracza, którego chcesz wyrzuciæ:", "Ok", "Cofnij");

		case 7: // Zastêpca
		{
			if(firmInfo[firmaid][tChef] != playerInfo[playerid][pID])
				return ShowInfo(playerid, "Tylko szef mo¿e zmieniæ VcSzefa.");

			Dialog_Show(playerid, DIALOG_ID_FIRMA_ZASTEPCA, DIALOG_STYLE_INPUT, "Firma > Zastêpca", "WprowadŸ ID gracza, którego chcesz mianowaæ na zastêpcê (-1 by usun¹æ):", "Ok", "Cofnij");
		}

		case 8: // Ksiêgowy
			Dialog_Show(playerid, DIALOG_ID_FIRMA_KSIEGOWY, DIALOG_STYLE_INPUT, "Firma > Ksiêgowy", "WprowadŸ ID gracza, którego chcesz mianowaæ na ksiêgowego (-1 by usun¹æ):", "Ok", "Cofnij");

		case 9: // Ustaw spawn
		{
			new Float:Pos[3];

			GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);
			firmInfo[firmaid][tSpawnX] = Pos[0];
			firmInfo[firmaid][tSpawnY] = Pos[1];
			firmInfo[firmaid][tSpawnZ] = Pos[2];

			ShowInfo(playerid, "Spawn zosta³ zmieniony.");
		}

		case 10: // Wyp³aæ pieni¹dze
			ShowInfo(playerid, "Chwilowo niedostêpne.");

		case 11: // Zmieñ skina
		{
			format(string, sizeof(string), "Skin 1 (%d)\nSkin 2 (%d)\nSkin 3 (%d)\nSkin 4 (%d)\nSkin 5 (%d)", firmInfo[firmaid][tSkin1], firmInfo[firmaid][tSkin2], firmInfo[firmaid][tSkin3], firmInfo[firmaid][tSkin4], firmInfo[firmaid][tSkin5]);
			Dialog_Show(playerid, DIALOG_ID_FIRMA_SKIN, DIALOG_STYLE_LIST, "Firma > Skiny", string, "Zmieñ", "Cofnij");
		}
	}
	return 1;
}

Dialog:DIALOG_ID_FIRMA_WPLAC(playerid, response, listitem, inputtext[])
{
	if(!response)
	return cmd_firma(playerid, "");
	
	new kasa = strval(inputtext);
	
	if(kasa > GetMoney(playerid))
	return Dialog_Show(playerid, DIALOG_ID_FIRMA_WPLAC, DIALOG_STYLE_INPUT, "Firma > Wp³aæ", "{FF0000}Nie posiadasz tyle pieniêdzy.\n \n{000000}WprowadŸ iloœæ pieniêdzy, które chcesz wp³aciæ:", "Ok", "Cofnij");
	
	GiveMoney(playerid, -kasa);
	firmInfo[playerInfo[playerid][pFirm]][tBank] += kasa;
	
	ShowInfo(playerid, "Wp³aci³eœ pieni¹dze na konto firmy.");
	return 1;
}

Dialog:DIALOG_ID_FIRMA_ZAPROS(playerid, response, listitem, inputtext[])
{
	if(!response)
	return cmd_firma(playerid, "");
	
	new forplayerid = strval(inputtext), string[80];
	
	if(!IsPlayerConnected(forplayerid))
	return Dialog_Show(playerid, DIALOG_ID_FIRMA_ZAPROS, DIALOG_STYLE_INPUT, "Firma > Zaproœ", "{FF0000}Gracz nie jest na serwerze.\n \n{000000}WprowadŸ ID gracza, którego chcesz zaprosiæ:", "Ok", "Cofnij");
	
	if(playerInfo[forplayerid][pFirm] != 0)
	return Dialog_Show(playerid, DIALOG_ID_FIRMA_ZAPROS, DIALOG_STYLE_INPUT, "Firma > Zaproœ", "{FF0000}Gracz jest ju¿ zatrudniony.\n \n{000000}WprowadŸ ID gracza, którego chcesz zaprosiæ:", "Ok", "Cofnij");
	
	SetPVarInt(forplayerid, "FIRMA_ZAPROS", playerid);
	format(string, sizeof(string), "%s zaprosi³ Ciê do firmy %s.", PlayerName(playerid), firmInfo[playerInfo[playerid][pFirm]][tName]);
	Dialog_Show(forplayerid, DIALOG_ID_FIRMA_ZAPROS_ACCEPT, DIALOG_STYLE_MSGBOX, "Firma > Zaproœ", string, "Akceptuj", "Odrzuæ");
	return 1;
}

Dialog:DIALOG_ID_FIRMA_ZAPROS_ACCEPT(playerid, response, listitem, inputtext[])
{
	if(!response)
	return ShowInfo(GetPVarInt(playerid, "FIRMA_ZAPROS"), "Gracz odrzuci³ zaproszenie do firmy.");
	
	playerInfo[playerid][pFirm] = playerInfo[GetPVarInt(playerid, "FIRMA_ZAPROS")][pFirm];
	OnPlayerText(playerid, "#Przyj¹³em zaproszenie do firmy.");
	return 1;
}

Dialog:DIALOG_ID_FIRMA_WYRZUC(playerid, response, listitem, inputtext[])
{
	if(!response)
	return cmd_firma(playerid, "");
	
	new forplayerid = strval(inputtext), string[80], firmaid = playerInfo[playerid][pFirm];
	
	if(!IsPlayerConnected(forplayerid))
	return Dialog_Show(playerid, DIALOG_ID_FIRMA_WYRZUC, DIALOG_STYLE_INPUT, "Firma > Wyrzuæ", "{FF0000}Gracz nie jest na serwerze.\n \n{000000}WprowadŸ ID gracza, którego chcesz zaprosiæ:", "Ok", "Cofnij");
	
	if(playerInfo[forplayerid][pFirm] != firmaid)
	return Dialog_Show(playerid, DIALOG_ID_FIRMA_WYRZUC, DIALOG_STYLE_INPUT, "Firma > Wyrzuæ", "{FF0000}Gracz nie pracuje w Twojej firmie.\n \n{000000}WprowadŸ ID gracza, którego chcesz zaprosiæ:", "Ok", "Cofnij");
	
	if(firmInfo[firmaid][tVcChef] == playerInfo[playerid][pID] && firmInfo[firmaid][tChef] == playerInfo[forplayerid][pID])
	return Dialog_Show(playerid, DIALOG_ID_FIRMA_WYRZUC, DIALOG_STYLE_INPUT, "Firma > Wyrzuæ", "{FF0000}Nie mo¿esz wyrzuciæ szefa.\n \n{000000}WprowadŸ ID gracza, którego chcesz zaprosiæ:", "Ok", "Cofnij");
	
	format(string, sizeof(string), "%s wyrzuci³ Ciê z firmy %s.", PlayerName(playerid), firmInfo[playerInfo[playerid][pFirm]][tName]);
	ShowInfo(forplayerid, string);
	
	if(firmInfo[firmaid][tVcChef] == playerInfo[forplayerid][pID])
	{
		format(string, sizeof(string), "UPDATE `Firmy` SET `VcSzef` = '0' WHERE `id` = '%d'", firmaid);
		mysql_query(string);
		
		firmInfo[firmaid][tVcChef] = 0;
	}
	
	if(firmInfo[firmaid][tAccountant] == playerInfo[forplayerid][pID])
	{
		format(string, sizeof(string), "UPDATE `Firmy` SET `Ksiegowy` = '0' WHERE `id` = '%d'", firmaid);
		mysql_query(string);
		firmInfo[firmaid][tAccountant] = 0;
	}
	
	OnPlayerText(forplayerid, "#Zosta³em wyrzucony z firmy.");
	playerInfo[forplayerid][pFirm] = 0;
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
		
		ShowInfo(playerid, "Usun¹³eœ zastêpcê.");
		return 1;
	}
	
	if(!IsPlayerConnected(forplayerid))
	return ShowInfo(playerid, "Gracz jest offline.");
	
	if(playerInfo[playerid][pFirm] != playerInfo[forplayerid][pFirm])
	return ShowInfo(playerid, "Gracz nie jest w Twojej firmie.");
	
	format(string, sizeof(string), "Zosta³eœ mianowany przez %s na zastêpcê firmy %s.", PlayerName(playerid), firmInfo[firmaid][tName]);
	ShowInfo(forplayerid, string);
	
	format(string, sizeof(string), "Mianowa³eœ %s na zastêpcê firmy %s.", PlayerName(forplayerid), firmInfo[firmaid][tName]);
	ShowInfo(playerid, string);
	
	firmInfo[firmaid][tVcChef] = playerInfo[forplayerid][pID];
	
	format(string, sizeof(string), "UPDATE `Firmy` SET `VcSzef` = '%d' WHERE `id` = '%d'", playerInfo[forplayerid][pID], firmaid);
	mysql_query(string);
	return 1;
}

Dialog:DIALOG_ID_FIRMA_KSIEGOWY(playerid, response, listitem, inputtext[])
{
	if(!response)
	return cmd_firma(playerid, "");
	
	new forplayerid = strval(inputtext), string[80], firmaid = playerInfo[playerid][pFirm];
	
	if(forplayerid == -1)
	{
		firmInfo[firmaid][tAccountant] = 0;
		
		format(string, sizeof(string), "UPDATE `Firmy` SET `Ksiegowy` = '0' WHERE `id` = '%d'", firmaid);
		mysql_query(string);
		
		ShowInfo(playerid, "Usun¹³eœ ksiêgowego.");
		return 1;
	}
	
	if(!IsPlayerConnected(forplayerid))
	return ShowInfo(playerid, "Gracz jest offline.");
	
	if(playerInfo[playerid][pFirm] != playerInfo[forplayerid][pFirm])
	return ShowInfo(playerid, "Gracz nie jest w Twojej firmie.");
	
	format(string, sizeof(string), "Zosta³eœ mianowany przez %s na ksiêgowego firmy %s.", PlayerName(playerid), firmInfo[firmaid][tName]);
	ShowInfo(forplayerid, string);
	
	format(string, sizeof(string), "Mianowa³eœ %s na ksiêgowego firmy %s.", PlayerName(forplayerid), firmInfo[firmaid][tName]);
	ShowInfo(playerid, string);
	
	firmInfo[firmaid][tAccountant] = playerInfo[forplayerid][pID];
	
	format(string, sizeof(string), "UPDATE `Firmy` SET `Ksiegowy` = '%d' WHERE `id` = '%d'", playerInfo[forplayerid][pID], firmaid);
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
				format(string, sizeof(string), "UID: \t%d\nID: \t%d\nW³aœciciel: \t%d\nSub w³aœciciel: \t%d\nTyp w³aœciciela: \t%s\nModel: \t%d\nHP: \t%0.0f\nPaliwo: \t%0.0f\nTyp paliwa: \t%s\nPrzebieg: %0.1fkm\nKolor: \t%d:%d\nPrzegl¹d: %s",
				vehuid,
				vehicleid,
				vehInfo[vehuid][vOwnerID],
				vehInfo[vehuid][vOwnerVC],
				(vehInfo[vehuid][vOwnerType] == OWNER_TYPE_PLAYER ? ("Gracz") : ("Firma")),
				vehInfo[vehuid][vModel],
				sVehHP,
				vehInfo[vehuid][vFuel],
				(vehInfo[vehuid][vFuelType] == FUEL_TYPE_BENZ ? ("Benzyna") : (vehInfo[vehuid][vFuelType] == FUEL_TYPE_TDI ? ("Diesel") : ("Gaz"))),
				vehInfo[vehuid][vPrzebieg],
				vehInfo[vehuid][vColor1],
				vehInfo[vehuid][vColor2],
				vehInfo[vehuid][vPrzeglad]);
			
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
			
			Dialog_Show(playerid, DIALOG_ID_VEHICLE_ZARZADZAJ, DIALOG_STYLE_LIST, "Pojazd > Zarz¹dzaj", "Zaparkuj\nUstaw zastêpcê", "Ok", "Anuluj");
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
		}
		
		case 1: ShowInfo(playerid, "Ustawienie zastêpcy jest chwilowo nie mo¿liwe.");
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
		case 0: strins(name, "policja", 0); 
		case 1: strins(name, "pogotowie", 0);
		case 2: strins(name, "pomoc drogowa", 0);
	}

	SetPVarInt(playerid, "TELEPHONE", listitem+1);
	format(szString, sizeof szString, "Halo... tutaj %s.\nCo siê sta³o?", name);
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

	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if(!IsPlayerConnected(i) || !IsPlayerLogged(i))
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


	Dialog:DIALOG_MENU_BAR(playerid, response, listitem, inputtext[])
	{
		if(response == 1)
		{
			new procent = GetPVarInt(playerid, "JEDZENIEPROCENTY");
			new stan, money;

			switch(listitem)
			{
				case 0..1, 6..9:
				{
					cmd_menu(playerid, "");
				}

				case 2:
				{
					stan = 100;
					money = 250;

					SendClientMessage(playerid, 0x008040FF, "Kupi³eœ obiad.");
				}

				case 3:
				{
					stan = 70;
					money = 200;

					SendClientMessage(playerid, 0x008040FF, "Kupi³eœ ?niadanie.");
				}

				case 4:
				{
					stan = 50;
					money = 150;

					SendClientMessage(playerid, 0x008040FF, "Kupi³eœ kolacj?");
				}

				case 5:
				{
					stan = 25;
					money = 100;

					SendClientMessage(playerid, 0x008040FF, "Kupi³eœ deser.");
				}
			}

			if(money)
			{
				GiveMoney(playerid, -money);

				SetPVarInt(playerid, "JEDZENIEPROCENTY", (procent+stan >= 100) ? (100) : (procent+stan));
				//SetPlayerProgressBarValue(playerid, Glod[playerid], float((procent+stan >= 100) ? (100) : (procent+stan)));
				//UpdatePlayerProgressBar(playerid, Glod[playerid]);
			}
		}
		return 1;
	}

/*Dialog:DIALOG_PANEL)
{
if(response)
{
switch(listitem)
{
case 0:
{
new x[500];
strcat(x, "{FFFFFF}Serwer-Truck SAMP - Panel firmowy\n");
strcat(x, " \n");
strcat(x, "{FFFFFF}- {008000}lista pracownik?\n");
strcat(x, "{FFFFFF}- {008000}konto bankowe\n");
strcat(x, "{FFFFFF}- {008000}dodaj pracownika\n");
strcat(x, "{FFFFFF}- {008000}usun pracownika\n");
strcat(x, "{FFFFFF}- {008000}dodaj ksiê owego\n");
strcat(x, "{FFFFFF}- {008000}usu?ksiê owego\n");
strcat(x, " \n");
strcat(x,"{FFFFFF}www.serwertruck.eu");
Dialog_Show(playerid, DIALOG_PANEL, DIALOG_STYLE_LIST, " ", x, "OK", "WyjdŸ");
}
case 1:
{
new x[500];
strcat(x, "{FFFFFF}Serwer-Truck SAMP - Panel firmowy\n");
strcat(x, " \n");
strcat(x, "{FFFFFF}- {008000}lista pracownik?\n");
strcat(x, "{FFFFFF}- {008000}konto bankowe\n");
strcat(x, "{FFFFFF}- {008000}dodaj pracownika\n");
strcat(x, "{FFFFFF}- {008000}usun pracownika\n");
strcat(x, "{FFFFFF}- {008000}dodaj ksiê owego\n");
strcat(x, "{FFFFFF}- {008000}usu?ksiê owego\n");
strcat(x, " \n");
strcat(x,"{FFFFFF}www.serwertruck.eu");
Dialog_Show(playerid, DIALOG_PANEL, DIALOG_STYLE_LIST, " ", x, "OK", "WyjdŸ");
}
case 2:
{
if(gTeam[playerid] == CARGO || gTeam[playerid] == PETROL || gTeam[playerid] == SMLOG || gTeam[playerid] == BUILD)
{
new query[128];
format(query, sizeof query, "SELECT Name, Kursy, Wyplata FROM `PRACOWNICY` WHERE `Team`= '%d'", gTeam[playerid]);
new DBResult:result = db_query(PRACOWNICYDB, query);
new numRows = db_num_rows(result);

new x[500], string[150];
strcat(x, "{FFFFFF}Serwer-Truck SAMP - Informacje o pracownikach.\n");
strcat(x, " \n");
strcat(x, "{008000}Nick, iloœæ kurs?, wyp?ta.\n");
strcat(x, " \n");
for(new i=0; i<numRows; i++)
{
new I[20], K[20], W[20];
db_get_field(result, 0, I, sizeof I);
db_get_field(result, 1, K, sizeof K);
db_get_field(result, 2, W, sizeof W);

new kursy, wypl;
kursy = strval(K);
wypl = strval(W);

format(string, sizeof string, "{FFFFFF}%s, %d, %d\n", I, kursy, wypl);
strcat(x, string);

db_next_row(result);
}
db_free_result(result);
Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, " ", x, "OK", "");
}
else if(gTeam[playerid] == POLICE || gTeam[playerid] == MEDIC || gTeam[playerid] == POMOC)
{
new query[128];
format(query, sizeof query, "SELECT Name, Kursy, Wyplata FROM `PRACOWNICY` WHERE `Team`= '%d'", gTeam[playerid]);
new DBResult:result = db_query(PRACOWNICYDB, query);
new numRows = db_num_rows(result);

new x[500], string[150];
strcat(x, "{FFFFFF}Serwer-Truck SAMP - Informacje o pracownikach.\n");
strcat(x, " \n");
strcat(x, "{008000}Nick, czas pracy, wyp?ta.\n");
strcat(x, " \n");
for(new i=0; i<numRows; i++)
{
new I[20], C[20], W[20];
db_get_field(result, 0, I, sizeof I);
db_get_field(result, 1, C, sizeof C);
db_get_field(result, 2, W, sizeof W);

new czaas, wypl;
czaas = strval(C);
wypl = strval(W);

format(string, sizeof string, "{FFFFFF}%s, %d, %d\n", I, czaas, wypl);
strcat(x, string);

db_next_row(result);
}
db_free_result(result);
Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, " ", x, "OK", "");
}
}
case 3:
{
new x[500], string[200];
strcat(x, "{FFFFFF}Serwer-Truck SAMP - Stan konta firmowego.\n");
strcat(x, " \n");
format(string, sizeof string, "{008000}Kwota w banku: {FFFFFF}%d$\n", KontoFirmowe(playerid));
strcat(x, string);
strcat(x," \n");
strcat(x, "{008000}Wp?æ na konto bankowe.\n");
strcat(x, "{008000}Wyp?æ z konta bankowego.\n");
strcat(x," \n");
strcat(x,"{FFFFFF}www.serwertruck.eu");
Dialog_Show(playerid, DIALOG_PANEL1, DIALOG_STYLE_LIST, " ", x, "OK", "Wstecz");
}
case 4:
{
Dialog_Show(playerid, DIALOG_PANEL4, DIALOG_STYLE_INPUT, " ", "WprowadŸ ID pracownika, kt?ego chcesz zatrudniæ.", "OK", "WyjdŸ");
}
case 5:
{
Dialog_Show(playerid, DIALOG_PANEL5, DIALOG_STYLE_INPUT, " ", "WprowadŸ nick pracownika, kt?ego chcesz zwolniæ.", "OK", "WyjdŸ");
}
case 6:
{
Dialog_Show(playerid, DIALOG_PANEL6, DIALOG_STYLE_INPUT, " ", "WprowadŸ ID pracownika, kt?ego awansowaæ.", "OK", "WyjdŸ");
}
}
}
}

Dialog:DIALOG_PANEL1)
{
if(response == 0)
{
new x[500];
strcat(x, "{FFFFFF}Serwer-Truck SAMP - Panel firmowy\n");
strcat(x, " \n");
strcat(x, "{FFFFFF}- {008000}lista pracownik?\n");
strcat(x, "{FFFFFF}- {008000}konto bankowe\n");
strcat(x, "{FFFFFF}- {008000}dodaj pracownika\n");
strcat(x, "{FFFFFF}- {008000}usun pracownika\n");
strcat(x, "{FFFFFF}- {008000}dodaj ksiê owego\n");
strcat(x, "{FFFFFF}- {008000}usu?ksiê owego\n");
strcat(x, " \n");
strcat(x,"{FFFFFF}www.serwertruck.eu");
Dialog_Show(playerid, DIALOG_PANEL, DIALOG_STYLE_LIST, " ", x, "OK", "WyjdŸ");
}
if(response)
{
switch(listitem)
{
case 0..3:
{
new x[500], string[200];
strcat(x, "{FFFFFF}Serwer-Truck SAMP - Stan konta firmowego.\n");
strcat(x, " \n");
format(string, sizeof string, "{008000}Kwota w banku: {FFFFFF}%d$\n", KontoFirmowe(playerid));
strcat(x, string);
strcat(x," \n");
strcat(x, "{008000}Wp?æ na konto bankowe.\n");
strcat(x, "{008000}Wyp?æ z konta bankowego.\n");
strcat(x," \n");
strcat(x,"{FFFFFF}www.serwertruck.eu");
Dialog_Show(playerid, DIALOG_PANEL1, DIALOG_STYLE_LIST, " ", x, "OK", "Wstecz");
}
case 4:
{
Dialog_Show(playerid, DIALOG_PANEL2, DIALOG_STYLE_INPUT, " ", "WprowadŸ kwot?jak¹ chcesz wp?ciæ na konto bankowe.", "OK", "WyjdŸ");
}
case 5:
{
Dialog_Show(playerid, DIALOG_PANEL3, DIALOG_STYLE_INPUT, " ", "WprowadŸ kwot?jak¹ chcesz wyp?ciæ z konta bankowego.", "OK", "WyjdŸ");
}
case 6..7:
{
new x[500], string[200];
strcat(x, "{FFFFFF}Serwer-Truck SAMP - Stan konta firmowego.\n");
strcat(x, " \n");
format(string, sizeof string, "{008000}Kwota w banku: {FFFFFF}%d$\n", KontoFirmowe(playerid));
strcat(x, string);
strcat(x," \n");
strcat(x, "{008000}Wp?æ na konto bankowe.\n");
strcat(x, "{008000}Wyp?æ z konta bankowego.\n");
strcat(x," \n");
strcat(x,"{FFFFFF}www.serwertruck.eu");
Dialog_Show(playerid, DIALOG_PANEL1, DIALOG_STYLE_LIST, " ", x, "OK", "Wstecz");
}
}
}
}

Dialog:DIALOG_PANEL2)
{
if(inputtext[DIALOG_STYLE_INPUT])
{
new kasa = strval(inputtext);

if(kasa <= 0)
return SendClientMessage(playerid, LIGHTRED, "Podana kwota jest zbyt niska.");

if(kasa > GetMoney(playerid))
return SendClientMessage(playerid, LIGHTRED, "Podana kwota jest zbyt wysoka.");

GiveMoney(playerid, -kasa);
Wplacpieniadzefirma(playerid, kasa);
}
}

Dialog:DIALOG_PANEL3)
{
if(inputtext[DIALOG_STYLE_INPUT])
{
new kasa = strval(inputtext);

if(kasa <= 0)
return SendClientMessage(playerid, LIGHTRED, "Podana kwota jest zbyt niska.");

if(kasa > GetMoney(playerid))
return SendClientMessage(playerid, LIGHTRED, "Podana kwota jest zbyt wysoka.");

GiveMoney(playerid, -kasa);
Wplacpieniadzefirma(playerid, kasa);

}
}

Dialog:DIALOG_PANEL4)
{
if(response)
{
new forplayerid = strval(inputtext);

if(!IsPlayerConnected(forplayerid))
return SendClientMessage(playerid, LIGHTRED, "Ten gracz nie jest obecny na serwerze.");

new string[256];
new Y, Mo, D;
getdate(Y, Mo, D);

if(IsValidZatrudniony(PlayerName(forplayerid)))
return SendClientMessage(playerid, LIGHTRED, "Ten gracz jest ju¿ zatrudniony.");

format(string, sizeof string, "{008040}Gracz {FFFFFF}%s {008040}zosta³dodany do twojego teamu.", PlayerName(forplayerid), gTeam[playerid]);
SendClientMessage(playerid,0x0,string);
format(string, sizeof string, "{008040}zosta³dodany do teamu o id: {FFFFFF}%d {008040}przez {FFFFFF}%s.", gTeam[playerid], PlayerName(playerid));
SendClientMessage(forplayerid,0x0,string);

format(string, sizeof string, "INSERT INTO `PRACOWNICY` VALUES('%s', '%d', '%d', '%d', '%02d', '%02d', '%02d', '%d', '%d')", PlayerName(forplayerid), gTeam[playerid], 0, 0, D, Mo, Y, 0, 0);
db_query(PRACOWNICYDB, string);
}
}

Dialog:DIALOG_PANEL5)
{
if(inputtext[DIALOG_STYLE_INPUT])
{
new nick[20];
strmid(nick,inputtext,0,20);

new string[120];

if(!IsValidZatrudniony(nick))
return SendClientMessage(playerid, LIGHTRED, "Ten gracz nie jest nigdzie zatrudniony.");

format(string, sizeof string, "{008040}Gracz {FFFFFF}%s {008040}zosta³usuni?y z teamu.", nick);
SendClientMessage(playerid,0x0,string);
format(string, sizeof string, "DELETE FROM `PRACOWNICY` WHERE `Name`= '%s'", nick);
db_query(PRACOWNICYDB, string);
}
}

Dialog:DIALOG_PANEL6)
{
if(response)
{
new forplayerid = strval(inputtext);

new string[120];

if(!IsPlayerConnected(forplayerid))
return SendClientMessage(playerid, LIGHTRED, "Ten gracz nie jest obecny na serwerze.");

if(!IsValidZatrudniony(PlayerName(forplayerid)))
return SendClientMessage(playerid, LIGHTRED, "Ten gracz nie jest zatrudniony.");

format(string, sizeof string, "{008040}Gracz {FFFFFF}%s {008040}otrzyma?uprawnienia szefa.", PlayerName(forplayerid));
SendClientMessage(playerid,0x0,string);
format(string, sizeof string, "{008040}Otrzyma?uprawnienia szefa przez {FFFFFF}%s.", PlayerName(playerid));
SendClientMessage(forplayerid,0x0,string);

format(string, sizeof string, "UPDATE `PRACOWNICY` SET `Szef` = '%d' WHERE `Name` = '%s'", 1, PlayerName(forplayerid));
db_free_result(db_query(PRACOWNICYDB,string));
}
}

Dialog:DIALOG_PANEL7)
{
if(inputtext[DIALOG_STYLE_INPUT])
{
new name[20];
strmid(name,inputtext,0,20);

new string[120];

if(!IsValidZatrudniony(name))
return SendClientMessage(playerid, LIGHTRED, "Ten gracz nie jest zatrudniony.");

format(string, sizeof string, "{008040}Gracz {FFFFFF}%s {008040}straci?uprawnienia szefa.", name);
SendClientMessage(playerid,0x0,string);

format(string, sizeof string, "UPDATE `PRACOWNICY` SET `Szef` = '%d' WHERE `Name` = '%s'", 0, name);
db_free_result(db_query(PRACOWNICYDB,string));
}
}*/


Dialog:POLICE_DIALOG(playerid, response, listitem, inputtext[])
{
	new string[128];

	if(!response)
	{
		format(string, sizeof string, ""GCOLOR_YELLOW"Gracz %s odmówi³ przyjêcia mandatu.",PlayerName(playerid));
		SendClientMessage(GetPVarInt(playerid,"kontrola"), 0x0, string);
		SendClientMessage(playerid,0x0,""GCOLOR_YELLOW"Odmówi³eœ przyjêcia mandatu. Z twojego konta zosta³ pobrane podwojone punkty score.");
		GiveScore(playerid, -(GetPVarInt(playerid, "opcja2")*2));

		DeletePVar(playerid, "kontrola");
		DeletePVar(playerid, "opcja1");
		DeletePVar(playerid, "opcja2");
	}
	else
	{
		format(string, sizeof string, ""GCOLOR_YELLOW"Gracz %s przyj¹³ mandat.",PlayerName(playerid));
		SendClientMessage(GetPVarInt(playerid,"kontrola"), 0x0, string);
		SendClientMessage(playerid,0x0,""GCOLOR_YELLOW"Przyj¹³eœ mandat.");
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
		szBuffer[466+MAX_PLAYER_NAME+129], 
		nick[32], 
		password[129], 
		ip[16];

	if(isnull(inputtext))
	{
		format(szBuffer, sizeof(szBuffer), "{FFFFFF}Serwer-Truck SAMP - Logowanie konta.\n\n{a9c4e4}WprowadŸ has³o do konta, aby zacz¹æ rozgrywkê.\n\n");
		format(szBuffer, sizeof(szBuffer), "%s{a9c4e4}Wykorzysta³eœ ju¿ {FFFFFF}[%d/3] {a9c4e4}próby zalogowania.\n\n{FFFFFF}www.serwertruck.eu", szBuffer, GetPVarInt(playerid, "leci"));

		Dialog_Show(playerid, DIALOG_ID_LOGIN, DIALOG_STYLE_PASSWORD, "Panel > Logowanie", szBuffer, "Zaloguj", "Zamknij");
		return 1;
	}

	GetPlayerIp(playerid, ip, sizeof(ip));

	WP_Hash(password, sizeof(password), inputtext);
	mysql_real_escape_string(PlayerName(playerid), nick);

	format(szBuffer, sizeof szBuffer, "SELECT gracze.ID, gracze.Money, gracze.Score, gracze.Admin, gracze.Bankomat, gracze.Firma, gracze.Skin, gracze.Tacho, gracze.Foto, gracze.Toll, gracze.Gtime, gracze.Worktime, gracze.BuildMaster, gracze.Adr, \
		CASE WHEN scigani.id IS NOT NULL THEN scigani.id ELSE 0 END AS sciganiID \
		FROM `Accounts` AS gracze \
		LEFT JOIN `st_scigani` AS scigani ON gracze.ID = scigani.graczid AND scigani.aresztowanyprzez = 0 \
		WHERE gracze.nick = '%s' AND `Pass` = '%s'", nick, password);
	mysql_query(szBuffer);
	mysql_store_result();

	if(mysql_fetch_row(szBuffer, "|")) 
	{
		PlayerLogged(playerid);
		ResetPlayerMoney(playerid);

		sscanf(szBuffer, "e<p<|>dddfdddddddddddd>", playerInfo[playerid]);

		mysql_free_result();

		format(szBuffer, sizeof(szBuffer), "UPDATE `Accounts` SET `Online`='1', `IP`='%s' WHERE `nick` = '%s'", ip, nick);
		mysql_query(szBuffer);

		format(szBuffer, sizeof(szBuffer), "[IP: %s] Zalogowano poprawnie (próba: %d)", ip, GetPVarInt(playerid, "leci"));
		ToLog(playerInfo[playerid][pID], LOG_TYPE_PLAYER, szBuffer);

		GivePlayerMoney(playerid, playerInfo[playerid][pMoney]);
		SetPlayerScore(playerid, playerInfo[playerid][pScore]);

		//LadujPojazd(playerInfo[playerid][pID], OWNER_TYPE_PLAYER);

		Msg(playerid, COLOR_INFO, "Zalogowano pomyœlie. Mi³ej gry ¿yczy administracja. ;)");
		StopCinematicCameraIntro(playerid);
		ShowPlayerConnect(playerid, false);
		ShowConnect(playerid, false);
		ShowPlayerHud(playerid, true);
		TextDrawShowForPlayer(playerid, MainTextDraws[Time]);

		SprawdzPoziom(playerid);
		SetSpawnInfo(playerid, 0, playerInfo[playerid][pSkin], 1648.4308, -1053.9159, 23.8984, 0.0, 0, 0, 0, 0, 0, 0);
		TogglePlayerSpectating(playerid, false);
		SpawnPlayer(playerid);
		return 1;
	}

	SetPVarInt(playerid, "leci", GetPVarInt(playerid, "leci")+1);

	if(GetPVarInt(playerid, "leci") == 3)
	{
		CheatKick(playerid, "trzy b³êne logowania");
		timer[playerid] = SetTimerEx("Kickplayer", 500, 0, "d", playerid);
	}

	format(szBuffer, sizeof(szBuffer), "{FFFFFF}Serwer-Truck SAMP - Logowanie konta.\n\n{a9c4e4}WprowadŸ has³o do konta aby zacz¹æ rozgrywkê.\n\n");
	format(szBuffer, sizeof(szBuffer), "%s{a9c4e4}Wykorzysta³eœ ju¿ {FFFFFF}[%d/3] {a9c4e4}próby zalogowania.\n\n{FFFFFF}www.serwertruck.eu", szBuffer, GetPVarInt(playerid, "leci"));

	Dialog_Show(playerid, DIALOG_ID_LOGIN, DIALOG_STYLE_PASSWORD, "Panel > Logowanie", szBuffer, "Zaloguj", "Zamknij");

	return 1;
}
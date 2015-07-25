/*if(dialogid != DIALOG_ID_LOGIN && dialogid != DIALOG_ID_REGISTER)
	{
		new str[200];
		format(str, sizeof(str), "OnDialogResponse(%d, %d, %d, %d, %s)", playerid, dialogid, response, listitem, inputtext);
		ToLog(PlayerInfo[playerid][pID], LOG_TYPE_PLAYER, str);
	}*/

Dialog:DIALOG_ID_CREATE_VEHICLE(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	switch(listitem)
	{
		case 0: Dialog_Show(playerid, DIALOG_ID_CREATE_VEHICLE_TRUCK, DIALOG_STYLE_LIST, " ", "RoadTrain\nLinerunner\nTanker\nFlatbed\nDFT-30\nTopFun Van\nRumpo", "Wybierz", "Zamknij");
		case 1: Dialog_Show(playerid, DIALOG_ID_CREATE_VEHICLE_TRAILE, DIALOG_STYLE_LIST, " ", "Cysterna\nWywrotka\nPe³na\nCh³odnia", "Wybierz", "Zamknij");
		case 2:
		{
			Spawned[GetPVarInt(playerid, "pojazd")] = false;
			Spawned[GetPVarInt(playerid, "naczepa")] = false;

			SendClientMessage(playerid,0x0080FFFF,"Stworzone pojazdy zosta³y usuniête.");

			vehOtherInfo[GetPVarInt(playerid, "pojazd")][vCargoID] = 0;
			vehOtherInfo[GetPVarInt(playerid, "naczepa")][vCargoID] = 0; 

			DestroyVehicle(GetPVarInt(playerid, "pojazd"));
			DestroyVehicle(GetPVarInt(playerid, "naczepa"));
			DeletePVar(playerid, "pojazd");
			DeletePVar(playerid, "naczepa");
		}
	}
}

Dialog:DIALOG_ID_CREATE_VEHICLE_TRUCK(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new Vehicle, Float:Pos[4];

	GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);
	GetPlayerFacingAngle(playerid, Pos[3]);

	vehOtherInfo[GetPVarInt(playerid, "pojazd")][vCargoID] = 0;
	DestroyVehicle(GetPVarInt(playerid, "pojazd"));

	switch(listitem)
	{
		case 0: Vehicle = CreateVehicle(515, Pos[0], Pos[1]+5, Pos[2], Pos[3], 16, 16, -1);
		case 1: Vehicle = CreateVehicle(403, Pos[0], Pos[1]+5, Pos[2], Pos[3], 16, 16, -1);
		case 2: Vehicle = CreateVehicle(514, Pos[0], Pos[1]+5, Pos[2], Pos[3], 16, 16, -1);
		case 3: Vehicle = CreateVehicle(455, Pos[0], Pos[1]+5, Pos[2], Pos[3], 16, 16, -1);
		case 4: Vehicle = CreateVehicle(578, Pos[0], Pos[1]+5, Pos[2], Pos[3], 16, 16, -1);
		case 5: Vehicle = CreateVehicle(459, Pos[0], Pos[1]+5, Pos[2], Pos[3], 16, 16, -1);
		case 6: Vehicle = CreateVehicle(440, Pos[0], Pos[1]+5, Pos[2], Pos[3], 16, 16, -1);
	}

	Spawned[Vehicle]=true;
	SetPVarInt(playerid, "pojazd", Vehicle);

	vehOtherInfo[Vehicle][vFuel] = MaxFuel(GetVehicleModel(Vehicle))/2;
	vehOtherInfo[Vehicle][vFuelType] = IsVehicleTruck(Vehicle) ? FUEL_TYPE_TDI : FUEL_TYPE_BENZ;
	vehOtherInfo[Vehicle][vPrzebieg] = -2.0;
	vehOtherInfo[Vehicle][vCargoID] = 0;
	vehOtherInfo[Vehicle][vCargoWeight] = 0; 

	return PutPlayerInVehicle(playerid, Vehicle, 0);
}

Dialog:DIALOG_ID_CREATE_VEHICLE_TRAILE(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new Vehicle, Float:Pos[4];

	GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);
	GetPlayerFacingAngle(playerid, Pos[3]);

	vehOtherInfo[GetPVarInt(playerid, "naczepa")][vCargoID] = 0;
	DestroyVehicle(GetPVarInt(playerid, "naczepa"));

	switch(listitem)
	{
		case 0: Vehicle = CreateVehicle(584, Pos[0], Pos[1]+2, Pos[2], Pos[3], 16, 16, -1);
		case 1: Vehicle = CreateVehicle(450, Pos[0], Pos[1]+2, Pos[2], Pos[3], 16, 16, -1);
		case 2: Vehicle = CreateVehicle(435, Pos[0], Pos[1]+2, Pos[2], Pos[3], 16, 16, -1);
		case 3: Vehicle = CreateVehicle(591, Pos[0], Pos[1]+2, Pos[2], Pos[3], 16, 16, -1);
	}

	Spawned[Vehicle]=true;
	SetPVarInt(playerid, "naczepa", Vehicle);

	if(IsPlayerInAnyVehicle(playerid))
		AttachTrailerToVehicle(Vehicle, GetPlayerVehicleID(playerid));

	vehOtherInfo[Vehicle][vFuel] = MaxFuel(Vehicle);
	vehOtherInfo[Vehicle][vFuelType] = IsVehicleTruck(Vehicle) ? FUEL_TYPE_TDI : FUEL_TYPE_BENZ;
	vehOtherInfo[Vehicle][vPrzebieg] = 0.0;
	vehOtherInfo[Vehicle][vCargoID] = 0;
	vehOtherInfo[Vehicle][vCargoWeight] = 0; 
	return 1;
}

Dialog:DIALOG_ID_SPAWN_SELECT(playerid, response, listitem, inputtext[])
{
	switch(listitem)
	{
		case 0: SetPlayerPos(playerid,1648.4308,-1053.9159,23.8984);
		case 1: SetPlayerPos(playerid,1398.5642,1009.4103,10.9297);
		case 2: SetPlayerPos(playerid,-240.4012,-231.1620,2.3985);
		case 3: SetPlayerPos(playerid,-2136.7207,-126.3667,36.5191);
		case 4: SetPlayerPos(playerid, Firmy[PlayerInfo[playerid][pFirma]][tSpawnX], Firmy[PlayerInfo[playerid][pFirma]][tSpawnY], Firmy[PlayerInfo[playerid][pFirma]][tSpawnZ]);
	}

	return 1;
}

Dialog:DIALOG_CREATE_DOK_NAME(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	SetPVarString(playerid, "CreateDok_Name", inputtext);
	return Dialog_Show(playerid, DIALOG_CREATE_DOK_TYPE, DIALOG_STYLE_LIST, " ", "Budowlane\nElektroniczne\nProdukty spo¿ywcze\nArtyku³y odzie¿owe\nNiebezpieczne\nNielegalne", "OK", "Zamknij");
}

Dialog:DIALOG_CREATE_DOK_TYPE(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new dok_name[100], string[256];
	GetPVarString(playerid, "CreateDok_Name", dok_name, sizeof(dok_name));
	DeletePVar(playerid, "CreateDok_Name");

	new Float:playerPos[3];
	GetPlayerPos(playerid, playerPos[0], playerPos[1], playerPos[2]);

	format(string, sizeof string, "INSERT INTO `Doki` VALUES(NULL, '%s', '%f', '%f', '%f', '%d')", dok_name, playerPos[0], playerPos[1], playerPos[2], listitem);
	return mysql_query(string);
}

Dialog:DIALOG_CREATE_TOWAR_NAME(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;
			
	if(isnull(inputtext))
	{
		SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} B³êdna nazwa towaru.");
		//cmd_dodajtowar(playerid, "");
		return 1;
	}

	SetPVarString(playerid, "DodajTowar_Nazwa", inputtext);

	new string[256];
	format(string, sizeof(string), "Nazwa: %s | Wybierz typ:", inputtext);
	return Dialog_Show(playerid, DIALOG_CREATE_TOWAR_TYPE, DIALOG_STYLE_LIST, string, "Budowlane\nElektroniczne\nProdukty spo¿ywcze\nArtyku³y odzie¿owe\nNiebezpieczne\nNielegalne", "Wybierz", "Anuluj");
}

Dialog:DIALOG_CREATE_TOWAR_TYPE(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new string[256];
	GetPVarString(playerid, "DodajTowar_Nazwa", string, sizeof(string));
	SetPVarInt(playerid, "DodajTowar_Typ", listitem);

	format(string, sizeof(string), "Nazwa: %s\nTyp: %d\n\nPodaj stawk?za km (liczba z kropk?, np. 1.0):", string, listitem);
	return Dialog_Show(playerid, DIALOG_CREATE_TOWAR_STAWKA, DIALOG_STYLE_INPUT, " ", string, "Wybierz", "Anuluj");
}

Dialog:DIALOG_CREATE_TOWAR_STAWKA(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new string[256];
	GetPVarString(playerid, "DodajTowar_Nazwa", string, sizeof(string));

	if(floatstr(inputtext) < 0)
	{
		format(string, sizeof(string), "NIEPRAWID£OWA STAWKA!\n\nNazwa: %s\nTyp: %d\n\nPodaj stawk?za km (liczba z kropk?, np. 1.0):", string, GetPVarInt(playerid, "DodajTowar_Typ"));
		return Dialog_Show(playerid, DIALOG_CREATE_TOWAR_STAWKA, DIALOG_STYLE_INPUT, " ", string, "Wybierz", "Anuluj");
	}

	SetPVarFloat(playerid, "DodajTowar_Stawka", floatstr(inputtext));
	format(string, sizeof(string), "Nazwa: %s\nTyp: %d\nStawka: %f\n\nPodaj czas roz³adunku (w sekundach):", string, GetPVarInt(playerid, "DodajTowar_Typ"), GetPVarFloat(playerid, "DodajTowar_Stawka"));
	return Dialog_Show(playerid, DIALOG_CREATE_TOWAR_CZAS, DIALOG_STYLE_INPUT, " ", string, "Wybierz", "Anuluj");
}

Dialog:DIALOG_CREATE_TOWAR_CZAS(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new string[256];
	GetPVarString(playerid, "DodajTowar_Nazwa", string, sizeof(string));

	if(strval(inputtext) < 0)
	{
		format(string, sizeof(string), "NIEPRAWID£OWY CZAS!\n\nNazwa: %s\nTyp: %d\nStawka: %f\n\nPodaj czas roz³adunku (w sekundach):", string, GetPVarInt(playerid, "DodajTowar_Typ"), GetPVarFloat(playerid, "DodajTowar_Stawka"));
		return Dialog_Show(playerid, DIALOG_CREATE_TOWAR_CZAS, DIALOG_STYLE_INPUT, " ", string, "Wybierz", "Anuluj");
	}

	SetPVarInt(playerid, "DodajTowar_Czas", strval(inputtext));

	format(string, sizeof(string), "Nazwa: %s\nTyp: %d\nStawka: %f\n\nPodaj poziom:", string, GetPVarInt(playerid, "DodajTowar_Typ"), GetPVarFloat(playerid, "DodajTowar_Stawka"));
	return Dialog_Show(playerid, DIALOG_CREATE_TOWAR_POZIOM, DIALOG_STYLE_INPUT, " ", string, "Wybierz", "Anuluj");
}

Dialog:DIALOG_CREATE_TOWAR_POZIOM(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new string[256];

	if(strval(inputtext) < 0)
	{
		format(string, sizeof(string), "NIEPRAWID£OWY CZAS!\n\nNazwa: %s\nTyp: %d\nStawka: %f\n\nPodaj poziom:", string, GetPVarInt(playerid, "DodajTowar_Typ"), GetPVarFloat(playerid, "DodajTowar_Stawka"));
		return Dialog_Show(playerid, DIALOG_CREATE_TOWAR_POZIOM, DIALOG_STYLE_INPUT, " ", string, "Wybierz", "Anuluj");
	}

	format(string, sizeof string, "INSERT INTO `Towary` VALUES(NULL, '%s', '%d', '%f', '%d', '%d')", string, GetPVarInt(playerid, "DodajTowar_Typ"), GetPVarFloat(playerid, "DodajTowar_Stawka"), GetPVarInt(playerid, "DodajTowar_Czas"), GetPVarInt(playerid, "DodajTowar_Poziom"));
	mysql_query(string);

	format(string, sizeof(string), "Dodano towar:\n\nID: %d\nNazwa: %s\nTyp: %d\nStawka: %f\nPoziom: %d\n\nDost?ny b?zie po resecie.", mysql_insert_id(), string, GetPVarInt(playerid, "DodajTowar_Typ"), GetPVarFloat(playerid, "DodajTowar_Stawka"), GetPVarInt(playerid, "DodajTowar_Poziom"));
	Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_LIST, " ", string, "Wybierz", "Anuluj");

	DeletePVar(playerid, "DodajTowar_Nazwa");
	DeletePVar(playerid, "DodajTowar_Stawka");
	DeletePVar(playerid, "DodajTowar_Typ");
	DeletePVar(playerid, "DodajTowar_Czas");
	return DeletePVar(playerid, "DodajTowar_Poziom");
}


		
		Dialog:DIALOG_ID_REGISTER(playerid, response, listitem, inputtext[])
		{
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
					format(string, sizeof string, "INSERT INTO `Accounts` VALUES(NULL, '0', '%s', '%s', '2000', '0', '0', '-1', '0', '1', '0', '0', '0', '0', '%d', '%d', '%d', '%d', '%d', '0', '%s', '0', '0', '1', '0')", 
						Name, string, H, Mi, D, Mo, Y, ip);

					mysql_query(string);

					SetTimerEx("ReqSpawnPlayer", 100, false, "d", playerid);

					PlayerLogged(playerid);
					GiveMoney(playerid, 2000);
					GiveScore(playerid, 0);

					PlayerInfo[playerid][pID] = mysql_insert_id();
					PlayerInfo[playerid][pBankomat] = -1;
					PlayerInfo[playerid][pSkin] = 0;
				}
			}
		}

		Dialog:DIALOG_FOTORADAR(playerid, response, listitem, inputtext[])
		{
			if(!response || !inputtext[DIALOG_STYLE_INPUT])
				return 1;

			new fotoradarid = IsPlayerAtFoto(playerid);
			new string[76];
			format(string, sizeof string, "UPDATE `Fotoradary` SET `Maxspeed` = %d WHERE `ID` = '%d'", strval(inputtext), Fotoradary[fotoradarid][fID]);
			mysql_query(string);

			Fotoradary[fotoradarid][fMaxSpeed] = strval(inputtext);

			format(string, sizeof string, "{008000}Edytujesz fotoradar o uid: {FFFFFF}%d.", fotoradarid);
			SendClientMessage(playerid,0x0,string);
			EditObject(playerid, FotoradarOBJ[fotoradarid]);
		}

		Dialog:DIALOG_ID_FIRMA(playerid, response, listitem, inputtext[])
		{
			if(!response)
				return 1;

			new string[200], firmaid = PlayerInfo[playerid][pFirma];
			switch(listitem)
			{
				case 0: // Informacje
				{
					format(string, sizeof(string), "UID: \t%d\nNazwa: \t%s\nSzef (UID): \t%d\nVC Szef (UID): \t%d\nKsiegowy (UID): \t%d\nTyp: \t%d\nSaldo: \t%d\nSkin1: \t%d\nSkin2: \t%d\nSkin3: \t%d\nSkin4: \t%d\nSkin5: \t%d\nKolor: \t{%06x}-- KOLOREK --", Firmy[firmaid][tID], Firmy[firmaid][tName], 
						Firmy[firmaid][tChef], Firmy[firmaid][tVcChef], Firmy[firmaid][tKsiegowy], Firmy[firmaid][tTyp], Firmy[firmaid][tBank], Firmy[firmaid][tSkin1], Firmy[firmaid][tSkin2], Firmy[firmaid][tSkin3], Firmy[firmaid][tSkin4], Firmy[firmaid][tSkin5], Firmy[firmaid][tKolor]>>>8);
					Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_TABLIST, "Firma > Informacje", string, "Ok", "");
				}
				case 1: // Pracownicy online
				{
					strcat(string, "ID\tUID\tNick\n");

					for(new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
				    	if(PlayerInfo[i][pFirma] == firmaid)
				    		format(string, sizeof(string), "%s%d\t%d\t%s\n", string, i, PlayerInfo[i][pID], PlayerName(i));

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
					if(Firmy[firmaid][tChef] != PlayerInfo[playerid][pID])
						return ShowInfo(playerid, "Tylko szef mo¿e zmieniæ VcSzefa.");

					Dialog_Show(playerid, DIALOG_ID_FIRMA_ZASTEPCA, DIALOG_STYLE_INPUT, "Firma > Zastêpca", "WprowadŸ ID gracza, którego chcesz mianowaæ na zastêpcê (-1 by usun¹æ):", "Ok", "Cofnij");
				}

				case 8: // Ksiêgowy
					Dialog_Show(playerid, DIALOG_ID_FIRMA_KSIEGOWY, DIALOG_STYLE_INPUT, "Firma > Ksiêgowy", "WprowadŸ ID gracza, którego chcesz mianowaæ na ksiêgowego (-1 by usun¹æ):", "Ok", "Cofnij");

				case 9: // Ustaw spawn
				{
					new Float:Pos[3];

					GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);
					Firmy[firmaid][tSpawnX] = Pos[0];
					Firmy[firmaid][tSpawnY] = Pos[1];
					Firmy[firmaid][tSpawnZ] = Pos[2];

					ShowInfo(playerid, "Spawn zosta³ zmieniony.");
				}

				case 10: // Wyp³aæ pieni¹dze
					ShowInfo(playerid, "Chwilowo niedostêpne.");

				case 11: // Zmieñ skina
				{
					format(string, sizeof(string), "Skin 1 (%d)\nSkin 2 (%d)\nSkin 3 (%d)\nSkin 4 (%d)\nSkin 5 (%d)", Firmy[firmaid][tSkin1], Firmy[firmaid][tSkin2], Firmy[firmaid][tSkin3], Firmy[firmaid][tSkin4], Firmy[firmaid][tSkin5]);
					Dialog_Show(playerid, DIALOG_ID_FIRMA_SKIN, DIALOG_STYLE_LIST, "Firma > Skiny", string, "Zmieñ", "Cofnij");
				}
			}
		}

		Dialog:DIALOG_ID_FIRMA_WPLAC(playerid, response, listitem, inputtext[])
		{
			if(!response)
				return cmd_firma(playerid, "");

			new kasa = strval(inputtext);

			if(kasa > GetMoney(playerid))
				return Dialog_Show(playerid, DIALOG_ID_FIRMA_WPLAC, DIALOG_STYLE_INPUT, "Firma > Wp³aæ", "{FF0000}Nie posiadasz tyle pieniêdzy.\n\n{000000}WprowadŸ iloœæ pieniêdzy, które chcesz wp³aciæ:", "Ok", "Cofnij");

			GiveMoney(playerid, -kasa);
			Firmy[PlayerInfo[playerid][pFirma]][tBank] += kasa;

			ShowInfo(playerid, "Wp³aci³eœ pieni¹dze na konto firmy.");
		}

		Dialog:DIALOG_ID_FIRMA_ZAPROS(playerid, response, listitem, inputtext[])
		{
			if(!response)
				return cmd_firma(playerid, "");

			new forplayerid = strval(inputtext), string[80];

			if(!IsPlayerConnected(forplayerid))
				return Dialog_Show(playerid, DIALOG_ID_FIRMA_ZAPROS, DIALOG_STYLE_INPUT, "Firma > Zaproœ", "{FF0000}Gracz nie jest na serwerze.\n\n{000000}WprowadŸ ID gracza, którego chcesz zaprosiæ:", "Ok", "Cofnij");

			if(PlayerInfo[forplayerid][pFirma] != 0)
				return Dialog_Show(playerid, DIALOG_ID_FIRMA_ZAPROS, DIALOG_STYLE_INPUT, "Firma > Zaproœ", "{FF0000}Gracz jest ju¿ zatrudniony.\n\n{000000}WprowadŸ ID gracza, którego chcesz zaprosiæ:", "Ok", "Cofnij");

			SetPVarInt(forplayerid, "FIRMA_ZAPROS", playerid);
			format(string, sizeof(string), "%s zaprosi³ Ciê do firmy %s.", PlayerName(playerid), Firmy[PlayerInfo[playerid][pFirma]][tName]);
			Dialog_Show(forplayerid, DIALOG_ID_FIRMA_ZAPROS_ACCEPT, DIALOG_STYLE_MSGBOX, "Firma > Zaproœ", string, "Akceptuj", "Odrzuæ");
		}

		Dialog:DIALOG_ID_FIRMA_ZAPROS_ACCEPT(playerid, response, listitem, inputtext[])
		{
			if(!response)
				return ShowInfo(GetPVarInt(playerid, "FIRMA_ZAPROS"), "Gracz odrzuci³ zaproszenie do firmy.");

			PlayerInfo[playerid][pFirma] = PlayerInfo[GetPVarInt(playerid, "FIRMA_ZAPROS")][pFirma];
			OnPlayerText(playerid, "#Przyj¹³em zaproszenie do firmy.");
		}

		Dialog:DIALOG_ID_FIRMA_WYRZUC(playerid, response, listitem, inputtext[])
		{
			if(!response)
				return cmd_firma(playerid, "");

			new forplayerid = strval(inputtext), string[80], firmaid = PlayerInfo[playerid][pFirma];

			if(!IsPlayerConnected(forplayerid))
				return Dialog_Show(playerid, DIALOG_ID_FIRMA_WYRZUC, DIALOG_STYLE_INPUT, "Firma > Wyrzuæ", "{FF0000}Gracz nie jest na serwerze.\n\n{000000}WprowadŸ ID gracza, którego chcesz zaprosiæ:", "Ok", "Cofnij");

			if(PlayerInfo[forplayerid][pFirma] != firmaid)
				return Dialog_Show(playerid, DIALOG_ID_FIRMA_WYRZUC, DIALOG_STYLE_INPUT, "Firma > Wyrzuæ", "{FF0000}Gracz nie pracuje w Twojej firmie.\n\n{000000}WprowadŸ ID gracza, którego chcesz zaprosiæ:", "Ok", "Cofnij");

			if(Firmy[firmaid][tVcChef] == PlayerInfo[playerid][pID] && Firmy[firmaid][tChef] == PlayerInfo[forplayerid][pID])
				return Dialog_Show(playerid, DIALOG_ID_FIRMA_WYRZUC, DIALOG_STYLE_INPUT, "Firma > Wyrzuæ", "{FF0000}Nie mo¿esz wyrzuciæ szefa.\n\n{000000}WprowadŸ ID gracza, którego chcesz zaprosiæ:", "Ok", "Cofnij");

			format(string, sizeof(string), "%s wyrzuci³ Ciê z firmy %s.", PlayerName(playerid), Firmy[PlayerInfo[playerid][pFirma]][tName]);
			ShowInfo(forplayerid, string);

			if(Firmy[firmaid][tVcChef] == PlayerInfo[forplayerid][pID])
			{
				format(string, sizeof(string), "UPDATE `Firmy` SET `VcSzef` = '0' WHERE `id` = '%d'", firmaid);
				mysql_query(string);

				Firmy[firmaid][tVcChef] = 0;
			}

			if(Firmy[firmaid][tKsiegowy] == PlayerInfo[forplayerid][pID])
			{
				format(string, sizeof(string), "UPDATE `Firmy` SET `Ksiegowy` = '0' WHERE `id` = '%d'", firmaid);
				mysql_query(string);
				Firmy[firmaid][tKsiegowy] = 0;
			}

			OnPlayerText(forplayerid, "#Zosta³em wyrzucony z firmy.");
			PlayerInfo[forplayerid][pFirma] = 0;
		}

		Dialog:DIALOG_ID_FIRMA_ZASTEPCA(playerid, response, listitem, inputtext[])
		{
			if(!response)
				return cmd_firma(playerid, "");

			new forplayerid = strval(inputtext), string[80], firmaid = PlayerInfo[playerid][pFirma];

			if(forplayerid == -1)
			{
				Firmy[firmaid][tVcChef] = 0;

				format(string, sizeof(string), "UPDATE `Firmy` SET `VcSzef` = '0' WHERE `id` = '%d'", firmaid);
				mysql_query(string);

				ShowInfo(playerid, "Usun¹³eœ zastêpcê.");
				return 1;
			}

			if(!IsPlayerConnected(forplayerid))
				return ShowInfo(playerid, "Gracz jest offline.");

			if(PlayerInfo[playerid][pFirma] != PlayerInfo[forplayerid][pFirma])
				return ShowInfo(playerid, "Gracz nie jest w Twojej firmie.");

			format(string, sizeof(string), "Zosta³eœ mianowany przez %s na zastêpcê firmy %s.", PlayerName(playerid), Firmy[firmaid][tName]);
			ShowInfo(forplayerid, string);

			format(string, sizeof(string), "Mianowa³eœ %s na zastêpcê firmy %s.", PlayerName(forplayerid), Firmy[firmaid][tName]);
			ShowInfo(playerid, string);

			Firmy[firmaid][tVcChef] = PlayerInfo[forplayerid][pID];

			format(string, sizeof(string), "UPDATE `Firmy` SET `VcSzef` = '%d' WHERE `id` = '%d'", PlayerInfo[forplayerid][pID], firmaid);
			mysql_query(string);
		}

		Dialog:DIALOG_ID_FIRMA_KSIEGOWY(playerid, response, listitem, inputtext[])
		{
			if(!response)
				return cmd_firma(playerid, "");

			new forplayerid = strval(inputtext), string[80], firmaid = PlayerInfo[playerid][pFirma];

			if(forplayerid == -1)
			{
				Firmy[firmaid][tKsiegowy] = 0;

				format(string, sizeof(string), "UPDATE `Firmy` SET `Ksiegowy` = '0' WHERE `id` = '%d'", firmaid);
				mysql_query(string);

				ShowInfo(playerid, "Usun¹³eœ ksiêgowego.");
				return 1;
			}

			if(!IsPlayerConnected(forplayerid))
				return ShowInfo(playerid, "Gracz jest offline.");

			if(PlayerInfo[playerid][pFirma] != PlayerInfo[forplayerid][pFirma])
				return ShowInfo(playerid, "Gracz nie jest w Twojej firmie.");

			format(string, sizeof(string), "Zosta³eœ mianowany przez %s na ksiêgowego firmy %s.", PlayerName(playerid), Firmy[firmaid][tName]);
			ShowInfo(forplayerid, string);

			format(string, sizeof(string), "Mianowa³eœ %s na ksiêgowego firmy %s.", PlayerName(forplayerid), Firmy[firmaid][tName]);
			ShowInfo(playerid, string);

			Firmy[firmaid][tKsiegowy] = PlayerInfo[forplayerid][pID];

			format(string, sizeof(string), "UPDATE `Firmy` SET `Ksiegowy` = '%d' WHERE `id` = '%d'", PlayerInfo[forplayerid][pID], firmaid);
			mysql_query(string);
		}

		Dialog:DIALOG_ID_FIRMA_SKIN(playerid, response, listitem, inputtext[])
		{
			if(!response)
				return cmd_firma(playerid, "");

			SetPVarInt(playerid, "FIRMA_SKIN", listitem+1);

			new string[80];
			format(string, sizeof(string), "Zmieniasz skin: Skin %d\n\nWprowadŸ numer skina:", listitem+1);
			Dialog_Show(playerid, DIALOG_ID_FIRMA_SKIN_ZMIEN, DIALOG_STYLE_INPUT, "Firma > Skin > Zmieñ", string, "Ok", "Cofnij");
		}

		Dialog:DIALOG_ID_FIRMA_SKIN_ZMIEN(playerid, response, listitem, inputtext[])
		{
			if(!response)
				return cmd_firma(playerid, "");

			new skinid = strval(inputtext), firmaid = PlayerInfo[playerid][pFirma], string[80];
			if(skinid < 0 || skinid > 311)
				return ShowInfo(playerid, "Nieprawid³owe ID skina.");

			format(string, sizeof(string), "Zmieni³eœ skin numer %d na %d.", GetPVarInt(playerid, "FIRMA_SKIN"), skinid);
			ShowInfo(playerid, string);

			switch(GetPVarInt(playerid, "FIRMA_SKIN"))
			{
				case 1:	Firmy[firmaid][tSkin1] = skinid;
				case 2: Firmy[firmaid][tSkin2] = skinid;
				case 3: Firmy[firmaid][tSkin3] = skinid;
				case 4: Firmy[firmaid][tSkin4] = skinid;
				case 5: Firmy[firmaid][tSkin5] = skinid;
			}

			format(string, sizeof(string), "UPDATE `Firmy` SET `Skin%d` = '%d' WHERE `id` = '%d'", GetPVarInt(playerid, "FIRMA_SKIN"), skinid, firmaid);
			mysql_query(string);
			printf(string);
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
						format(string, sizeof(string), "ID: \t%d\nTyp: \tZESPAWNOWANY\nModel: \t%d\nHP: \t%0.0f\nPaliwo: \t%0.0f\nTyp paliwa: \t%s\nPrzebieg: %0.1fkm", vehicleid, GetVehicleModel(vehicleid), sVehHP, vehOtherInfo[vehicleid][vFuel], 
						(vehOtherInfo[vehicleid][vFuelType] == FUEL_TYPE_BENZ ? ("Benzyna") : (vehOtherInfo[vehicleid][vFuelType] == FUEL_TYPE_TDI ? ("Diesel") : ("Gaz"))), 
						vehOtherInfo[vehicleid][vPrzebieg]);
					else
						format(string, sizeof(string), "UID: \t%d\nID: \t%d\nW³aœciciel: \t%d\nSub w³aœciciel: \t%d\nTyp w³aœciciela: \t%s\nModel: \t%d\nHP: \t%0.0f\nPaliwo: \t%0.0f\nTyp paliwa: \t%s\nPrzebieg: %0.1fkm\nKolor: \t%d:%d", 
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
						vehInfo[vehuid][vColor2]);

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
					if( (vehInfo[vehuid][vOwnerType] == OWNER_TYPE_PLAYER && vehInfo[vehuid][vOwnerID] != PlayerInfo[playerid][pID]) ||
						(vehInfo[vehuid][vOwnerType] == OWNER_TYPE_TEAM && PlayerInfo[playerid][pID] != Firmy[PlayerInfo[playerid][pFirma]][tChef]))
						return ShowInfo(playerid, "Nie mo¿esz zarz¹dzaæ tym pojazdem.");

					Dialog_Show(playerid, DIALOG_ID_VEHICLE_ZARZADZAJ, DIALOG_STYLE_LIST, "Pojazd > Zarz¹dzaj", "Zaparkuj\nUstaw zastêpcê", "Ok", "Anuluj");
				}
			}
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
					ResetVariablesInEnum(vehInfo[vehuid], vInfo);

					LadujPojazd(_, _, vehuid);
					PutPlayerInVehicle(playerid, vehInfo[vehuid][vSAMPID], 0);
				}

				case 1: ShowInfo(playerid, "Ustawienie zastêpcy jest chwilowo nie mo¿liwe.");
			}
		}

		Dialog:DIALOG_ID_TELEPHONE(playerid, response, listitem, inputtext[])
		{
			if(!response)
				return 1;

			ShowInfo(playerid, "Brak zasiêgu!");
		}

Dialog:GTIMEG_LIST_DIALOG(playerid, response, listitem, inputtext[])
{
new string[76];

if(response)
{
SetPlayerTime(playerid,TimeGlobal[listitem][rHour],0);
format(string, sizeof string, "{0080C0}Zmieni?e? por?w grze na {FFFFFF}%s.", TimeGlobal[listitem][rName]);
SendClientMessage(playerid, 0x0, string);
}
}

Dialog:ANIM1_DIALOG(playerid, response, listitem, inputtext[])
{
if(response == 0)
{
Dialog_Show(playerid, ANIM4_DIALOG, DIALOG_STYLE_LIST, " ", "Bomba\nCeluj\nœmiej siê nMaska\nNapad\nZa?¿ r?e\nLe¿\nChro?siê nWymiotuj\nMachaj\nKlepnij w dup?nUmieraj", "OK", "Dalej");
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
}

Dialog:DIALOG_TANKOWANIE(playerid, response, listitem, inputtext[])
{
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
				return Kick(playerid);

			new s_buf[500], Name[32], Pass[129], ip[16];

			if(isnull(inputtext))
			{
				format(s_buf, sizeof(s_buf), "{FFFFFF}Serwer-Truck SAMP - Logowanie konta.\n\n{a9c4e4}WprowadŸ has³o do konta, aby zacz¹æ rozgrywkê.\n\n");
				format(s_buf, sizeof(s_buf), "%s{a9c4e4}Wykorzysta³eœ ju¿ {FFFFFF}[%d/3] {a9c4e4}próby zalogowania.\n\n{FFFFFF}www.serwertruck.eu", s_buf, GetPVarInt(playerid, "leci"));

				Dialog_Show(playerid, DIALOG_ID_LOGIN, DIALOG_STYLE_PASSWORD, "Panel > Logowanie", s_buf, "Zaloguj", "Zamknij");
				return 1;
			}

			GetPlayerIp(playerid, ip, sizeof(ip));


			WP_Hash(Pass, sizeof (Pass), inputtext);
			mysql_real_escape_string(PlayerName(playerid), Name);

			format(s_buf, sizeof(s_buf), "SELECT `ID`,`Money`,`Score`,`Admin`,`Bankomat`,`Firma`,`Skin`,`Tacho`,`Foto`,`Toll`,`Gtime`,`Worktime`,`BuildMaster`,`Adr` FROM `Accounts` WHERE `Name`='%s' AND `Pass` = '%s';", Name, Pass);
			mysql_query(s_buf);
			mysql_store_result();

			if(mysql_fetch_row(s_buf, "|")) 
			{
				PlayerLogged(playerid);
				ResetPlayerMoney(playerid);

				sscanf(s_buf, "e<p<|>dddddddddddddd>", PlayerInfo[playerid]);

				mysql_free_result();

				format(s_buf, sizeof(s_buf), "UPDATE `Accounts` SET `Online`='1', `IP`='%s' WHERE `Name` = '%s'", ip, Name);
				mysql_query(s_buf);

				format(s_buf, sizeof(s_buf), "[IP: %s] Zalogowano poprawnie (próba: %d)", ip, GetPVarInt(playerid, "leci"));
				ToLog(PlayerInfo[playerid][pID], LOG_TYPE_PLAYER, s_buf);

				GivePlayerMoney(playerid, PlayerInfo[playerid][pMoney]);
				SetPlayerScore(playerid, PlayerInfo[playerid][pScore]);

				//LadujPojazd(PlayerInfo[playerid][pID], OWNER_TYPE_PLAYER);

				SprawdzPoziom(playerid);
				SetPlayerSkin(playerid, PlayerInfo[playerid][pSkin]);
				SetTimerEx("ReqSpawnPlayer", 100, false, "d", playerid);
				return 1;
			}

			SetPVarInt(playerid, "leci", GetPVarInt(playerid, "leci")+1);

			if(GetPVarInt(playerid, "leci") == 3)
			{
				CheatKick(playerid, "trzy b??ne logowania");
				timer[playerid] = SetTimerEx("Kickplayer", 500, 0, "d", playerid);
			}

			format(s_buf, sizeof(s_buf), "{FFFFFF}Serwer-Truck SAMP - Logowanie konta.\n\n{a9c4e4}WprowadŸ has³o do konta aby zacz¹æ rozgrywkê.\n\n");
			format(s_buf, sizeof(s_buf), "%s{a9c4e4}Wykorzysta³eœ ju¿ {FFFFFF}[%d/3] {a9c4e4}próby zalogowania.\n\n{FFFFFF}www.serwertruck.eu", s_buf, GetPVarInt(playerid, "leci"));

			Dialog_Show(playerid, DIALOG_ID_LOGIN, DIALOG_STYLE_PASSWORD, "Panel > Logowanie", s_buf, "Zaloguj", "Zamknij");
}

Dialog:DIALOG_SET_PLATE(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;
	new fName[32];
	GetPVarString(playerid, "PLATE_Name", fName, sizeof fName);
	if(GetPVarInt(playerid, "PLATE_Count") < 1)
		return cmd_setplate(playerid, fName);

	new Float:fPos[3], count = 0;
	GetPlayerPos(playerid, fPos[0], fPos[1], fPos[2]);
	for(new i = 0; i < MAX_VEHICLES; i++)
	{
		if(IsValidVehicle(i))
		{
			new Float:fDistance;
			fDistance = GetVehicleDistanceFromPoint(i, fPos[0], fPos[1], fPos[2]);
			if(fDistance < 10.0)
			{
				count++;
				if(listitem+1 == count)
				{
					new szTemp[64];
					format(szTemp, sizeof szTemp, "Pomyœlnie %s");
					GetVehiclePos(i, fPos[0], fPos[1], fPos[2]);
					SetTimerEx("SetPos", 1500, false, "dfff", i, fPos[0], fPos[1], fPos[2]);
					SetVehicleNumberPlate(i, fName);
					SetVehicleToRespawn(i);

					DeletePVar(playerid, "PLATE_Name");
					DeletePVar(playerid, "PLATE_Count");
					break;
				}
			}
		}
	}
	return 1;
}
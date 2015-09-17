//CMD:gps(playerid, params[])
	//return cmd_nawigacja(playerid, params);
CMD:gps(playerid, params[])
{
	Msg(playerid, COLOR_ERROR, "Funkcja tymczasowo wy³¹czona.");
	return 1;
	//return Dialog_Show(playerid, DIALOG_ID_GPS, DIALOG_STYLE_LIST, "GPS System", "Doki towarowe\nStacje\nRestauracje\nSklepy ViaToll\nW³asna lokalizacja\nWy³¹cz", "Wybierz", "");
}

forward GPS_OnPlayerDisconnect(playerid);
public GPS_OnPlayerDisconnect(playerid)
	return GPS_Function(playerid, 4);

forward GPS_Function(playerid, listitem);
public GPS_Function(playerid, listitem)
{
	switch(listitem)
	{
		case 1: // Stacje
		{
			if(gpsInfo[playerid][gStations] || gpsInfo[playerid][gLoadings] || gpsInfo[playerid][gRestaurants] || gpsInfo[playerid][gViatoll] || gpsInfo[playerid][gCustom])
				return ShowInfo(playerid, "Ju¿ masz w³¹czonego GPSa, najpierw go wy³¹cz.");

			for(new i; i <= ID_STACJA; i++)
				if(IsValidDynamicMapIcon(StacjeMapIconForAll[i]))
					Streamer_AppendArrayData(STREAMER_TYPE_MAP_ICON, StacjeMapIconForAll[i], E_STREAMER_PLAYER_ID, playerid);

			gpsInfo[playerid][gStations]=true;
		}
		case 2: // Bary
		{
			if(gpsInfo[playerid][gStations] || gpsInfo[playerid][gLoadings] || gpsInfo[playerid][gRestaurants] || gpsInfo[playerid][gViatoll] || gpsInfo[playerid][gCustom])
				return ShowInfo(playerid, "Ju¿ masz w³¹czonego GPSa, najpierw go wy³¹cz.");

			ShowInfo(playerid, "Funkcja chwilowo wy³¹czona.");
		}
		case 3: // ViaToll
		{
			if(gpsInfo[playerid][gStations] || gpsInfo[playerid][gLoadings] || gpsInfo[playerid][gRestaurants] || gpsInfo[playerid][gViatoll] || gpsInfo[playerid][gCustom])
				return ShowInfo(playerid, "Ju¿ masz w³¹czonego GPSa, najpierw go wy³¹cz.");

			for(new i; i <= ID_STACJA; i++)
				if(IsValidDynamicMapIcon(viashop3DForAll[i]))
					Streamer_AppendArrayData(STREAMER_TYPE_MAP_ICON, viashop3DForAll[i], E_STREAMER_PLAYER_ID, playerid);

			gpsInfo[playerid][gViatoll]=true;
		}
		case 4: // w³asna lokalizacja
		{
			if(gpsInfo[playerid][gStations] || gpsInfo[playerid][gLoadings] || gpsInfo[playerid][gRestaurants] || gpsInfo[playerid][gViatoll] || gpsInfo[playerid][gCustom])
				return ShowInfo(playerid, "Ju¿ masz w³¹czonego GPSa, najpierw go wy³¹cz.");

			
		}
		case 5: // wy³¹cz
		{
			if(gpsInfo[playerid][gLoadings])
			{
				for(new i; i <= ID_DOKOW; i++)
					if(IsValidDynamicMapIcon(loadInfo[i][lMapIconForAll]))
						Streamer_RemoveArrayData(STREAMER_TYPE_MAP_ICON, loadInfo[i][lMapIconForAll], E_STREAMER_PLAYER_ID, playerid);

				gpsInfo[playerid][gLoadings]=false;
			}
			else if(gpsInfo[playerid][gStations])
			{
				for(new i; i <= ID_STACJA; i++)
					if(IsValidDynamicMapIcon(StacjeMapIconForAll[i]))
						Streamer_RemoveArrayData(STREAMER_TYPE_MAP_ICON, StacjeMapIconForAll[i], E_STREAMER_PLAYER_ID, playerid);

				gpsInfo[playerid][gStations]=false;
			}
			else if(gpsInfo[playerid][gViatoll])
			{
				for(new i; i <= ID_STACJA; i++)
					if(IsValidDynamicMapIcon(viashop3DForAll[i]))
						Streamer_RemoveArrayData(STREAMER_TYPE_MAP_ICON, viashop3DForAll[i], E_STREAMER_PLAYER_ID, playerid);

				gpsInfo[playerid][gStations]=false;
			}
			else if(gpsInfo[playerid][gCustom])
			{
				gpsInfo[playerid][gCustom] = false;
			}
		}

		case 10..16:
		{
			if(gpsInfo[playerid][gStations] || gpsInfo[playerid][gLoadings] || gpsInfo[playerid][gRestaurants] || gpsInfo[playerid][gViatoll] || gpsInfo[playerid][gCustom])
				return ShowInfo(playerid, "Ju¿ masz w³¹czonego GPSa, najpierw go wy³¹cz.");

			for(new i; i <= ID_DOKOW; i++)
				if(loadInfo[i][lType] == listitem-10)
					if(IsValidDynamicMapIcon(loadInfo[i][lMapIconForAll]))
						Streamer_AppendArrayData(STREAMER_TYPE_MAP_ICON, loadInfo[i][lMapIconForAll], E_STREAMER_PLAYER_ID, playerid);

			gpsInfo[playerid][gLoadings]=true;
		}
	}

	SetPVarInt(playerid, "GPS_OstatniRaz", GetTickCount());
	return 1;
}

Dialog:DIALOG_ID_GPS(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	if(listitem == 0)
		Dialog_Show(playerid, DIALOG_ID_GPS_TYPE, DIALOG_STYLE_LIST, "GPS System => Doki", "Budowlane\nElektroniczne\nArt. spo¿ywcze\nArt. odzie¿owe\nNiebezpieczne\nNielegalne", "Za³aduj", "Anuluj");
	else
	{
		SetTimerEx("GPS_Function", random(300)+200, false, "dd", playerid, listitem);
		ShowInfo(playerid, "Trwa aktualizacja nawigacji...");
	}

	return 1;
}

Dialog:DIALOG_ID_GPS_TYPE(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	SetTimerEx("GPS_Function", random(300)+200, false, "dd", playerid, listitem+10);
	return ShowInfo(playerid, "Trwa aktualizacja nawigacji...");
}
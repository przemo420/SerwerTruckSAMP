//CMD:gps(playerid, params[])
	//return cmd_nawigacja(playerid, params);
CMD:gps(playerid, params[])
	return Dialog_Show(playerid, DIALOG_ID_GPS, DIALOG_STYLE_LIST, "GPS System", "Doki towarowe\nStacje\nBary\nSklepy ViaToll\nWy³¹cz", "Wybierz", "");

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
			if(GPSGracz[playerid][GPSStacje] || GPSGracz[playerid][GPSDoki] || GPSGracz[playerid][GPSBary] || GPSGracz[playerid][GPSViatoll])
				return ShowInfo(playerid, "Ju¿ masz w³¹czonego GPSa, najpierw go wy³¹cz.");

			for(new i; i <= ID_STACJA; i++)
				if(IsValidDynamicMapIcon(StacjeMapIconForAll[i]))
					Streamer_AppendArrayData(STREAMER_TYPE_MAP_ICON, StacjeMapIconForAll[i], E_STREAMER_PLAYER_ID, playerid);

			GPSGracz[playerid][GPSStacje]=true;
		}
		case 2: // Bary
		{
			if(GPSGracz[playerid][GPSStacje] || GPSGracz[playerid][GPSDoki] || GPSGracz[playerid][GPSBary] || GPSGracz[playerid][GPSViatoll])
				return ShowInfo(playerid, "Ju¿ masz w³¹czonego GPSa, najpierw go wy³¹cz.");

			ShowInfo(playerid, "Funkcja chwilowo wy³¹czona.");
		}
		case 3: // ViaToll
		{
			if(GPSGracz[playerid][GPSStacje] || GPSGracz[playerid][GPSDoki] || GPSGracz[playerid][GPSBary] || GPSGracz[playerid][GPSViatoll])
				return ShowInfo(playerid, "Ju¿ masz w³¹czonego GPSa, najpierw go wy³¹cz.");

			for(new i; i <= ID_STACJA; i++)
				if(IsValidDynamicMapIcon(viashop3DForAll[i]))
					Streamer_AppendArrayData(STREAMER_TYPE_MAP_ICON, viashop3DForAll[i], E_STREAMER_PLAYER_ID, playerid);

			GPSGracz[playerid][GPSViatoll]=true;
		}
		case 4: // wy³¹cz
		{
			if(GPSGracz[playerid][GPSDoki])
			{
				for(new i; i <= ID_DOKOW; i++)
					if(IsValidDynamicMapIcon(loadInfo[i][lMapIconForAll]))
						Streamer_RemoveArrayData(STREAMER_TYPE_MAP_ICON, loadInfo[i][lMapIconForAll], E_STREAMER_PLAYER_ID, playerid);

				GPSGracz[playerid][GPSDoki]=false;
			}
			else if(GPSGracz[playerid][GPSStacje])
			{
				for(new i; i <= ID_STACJA; i++)
					if(IsValidDynamicMapIcon(StacjeMapIconForAll[i]))
						Streamer_RemoveArrayData(STREAMER_TYPE_MAP_ICON, StacjeMapIconForAll[i], E_STREAMER_PLAYER_ID, playerid);

				GPSGracz[playerid][GPSStacje]=false;
			}
			else if(GPSGracz[playerid][GPSViatoll])
			{
				for(new i; i <= ID_STACJA; i++)
					if(IsValidDynamicMapIcon(viashop3DForAll[i]))
						Streamer_RemoveArrayData(STREAMER_TYPE_MAP_ICON, viashop3DForAll[i], E_STREAMER_PLAYER_ID, playerid);

				GPSGracz[playerid][GPSStacje]=false;
			}
		}

		case 10..16:
		{
			if(GPSGracz[playerid][GPSStacje] || GPSGracz[playerid][GPSDoki] || GPSGracz[playerid][GPSBary] || GPSGracz[playerid][GPSViatoll])
				return ShowInfo(playerid, "Ju¿ masz w³¹czonego GPSa, najpierw go wy³¹cz.");

			for(new i; i <= ID_DOKOW; i++)
				if(loadInfo[i][lType] == listitem-10)
					if(IsValidDynamicMapIcon(loadInfo[i][lMapIconForAll]))
						Streamer_AppendArrayData(STREAMER_TYPE_MAP_ICON, loadInfo[i][lMapIconForAll], E_STREAMER_PLAYER_ID, playerid);

			GPSGracz[playerid][GPSDoki]=true;
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
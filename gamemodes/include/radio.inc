forward Radio_OnGameModeInit();
public Radio_OnGameModeInit()
{
	mysql_query("SELECT * FROM `Radia`");
	mysql_store_result();

	if(mysql_num_rows()==0)
		return print("[RADIA] Brak radia do zaladowania.");

	new id, string[140];
	while(mysql_fetch_row(string, "|")) 
	{
    	sscanf(string, "e<p<|>ds[32]s[176]>", radioInfo[id]);
		id++;
	} 

	printf("[RADIA] Zaladowano %d stacji radiowych.", id);
	mysql_free_result();
	return 1;
}

CMD:radio(playerid, params[])
{
	new string[1024];

	format(string, sizeof(string), "Wy³¹cz radio\n--\n");

	for(new i=0; i < sizeof(radioInfo); i++)
		if(radioInfo[i][sName])
			format(string, sizeof(string), "%s%s\n", string, radioInfo[i][sName]);

	Dialog_Show(playerid, DIALOG_ID_RADIO, DIALOG_STYLE_LIST, " ", string, "OK", "Zamknij");

	return 1;
}

CMD:dodajradio(playerid, params[])
{
	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	return Dialog_Show(playerid, DIALOG_ID_RADIO_ADD, DIALOG_STYLE_INPUT, " ", "Wpisz nazwê radia:", "Dalej", "Zamknij");
}


Dialog:DIALOG_ID_RADIO_ADD(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new string[126];
	if(isnull(inputtext))
		return Dialog_Show(playerid, DIALOG_ID_RADIO_ADD, DIALOG_STYLE_INPUT, " ", "B³êdna nazwa radia!\n\nWpisz nazwê radia:", "Dalej", "Zamknij");

	SetPVarString(playerid, "DodajRadio_Nazwa", inputtext);

	format(string, sizeof(string), "Nazwa radia: %s\n\nPodaj link:", inputtext);
	return Dialog_Show(playerid, DIALOG_ID_RADIO_LINK, DIALOG_STYLE_INPUT, " ", string, "Dalej", "Zamknij");
}

Dialog:DIALOG_ID_RADIO_LINK(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new string[126], nazwa[40];

	GetPVarString(playerid, "DodajRadio_Nazwa", nazwa, sizeof(nazwa));
	if(isnull(inputtext))
	{
		format(string, sizeof(string), "B??ny link radia!\n\nNazwa: %s\n\nPodaj link radia:", nazwa);
		return Dialog_Show(playerid, DIALOG_ID_RADIO_LINK, DIALOG_STYLE_INPUT, " ", string, "Dalej", "Zamknij");
	}

	format(string, sizeof(string), "Dodano radio:\n\nNazwa: %s\nLink: %sn\n\nDost?ne b?zie po resecie.", nazwa, inputtext);
	Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, " ", string, "Ok", "");

	format(string, sizeof(string), "INSERT INTO `Radia` VALUES (NULL, '%s', '%s');", nazwa, inputtext);
	mysql_query(string);

	Radio_OnGameModeInit();
	return 1;
}

Dialog:DIALOG_TP_LOGIN_SKIN(playerid, response, listitem, inputtext[])
{
	new firmaid = playerInfo[playerid][pFirm];

	switch(listitem)
	{
		case 0: 
		{
			if(firmInfo[firmaid][tChef] != playerInfo[playerid][pID] && firmInfo[firmaid][tVcChef] != playerInfo[playerid][pID])
			{
				SendClientMessage(playerid, LIGHTRED, "Ten skin jest dla szefa!");
				return Dialog_Show(playerid, DIALOG_TP_LOGIN_SKIN, DIALOG_STYLE_LIST, "Firma > Skiny", "Skin 1\nSkin 2\nSkin 3\nSkin 4\nSkin 5", "Ok", "");
			}

			SetPlayerSkin(playerid, firmInfo[firmaid][tSkin1]);
		}
				
		case 1: SetPlayerSkin(playerid, firmInfo[firmaid][tSkin2]);
		case 2: SetPlayerSkin(playerid, firmInfo[firmaid][tSkin3]);
		case 3: SetPlayerSkin(playerid, firmInfo[firmaid][tSkin4]);
		case 4: SetPlayerSkin(playerid, firmInfo[firmaid][tSkin5]);
	}

	return 1;
}

Dialog:DIALOG_ID_RADIO(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new string[70];

	if(listitem == 0)
	{
		if(!GetPVarInt(playerid, "RADIO"))
			return 1;

		SendClientMessage(playerid, 0x0, "{0080C0}Radio zosta³o wy³¹czone.");
		DeletePVar(playerid, "RADIO");
		StopAudioStreamForPlayer(playerid);
	}
	else if(listitem >= 2)
	{
		SetPVarInt(playerid, "RADIO", listitem-2);
		StopAudioStreamForPlayer(playerid);
		PlayAudioStreamForPlayer(playerid, radioInfo[listitem-2][sURL]);
		format(string, sizeof string, "{0080C0}Zmieni³êœ stacjê radiow¹ na: {FFFFFF}%s.", radioInfo[listitem-2][sName]);
		SendClientMessage(playerid, 0x0, string);
	}

	return 1;
}
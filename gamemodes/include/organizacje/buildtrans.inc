CMD:projekt(playerid, params[])
{
	new opcja[10];

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_BUILD)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "s[10]", opcja))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /projekt [lista/stworz/usun/akceptuj]");

	if(strcmp(opcja, "lista", true) == 0) {}
	else if(strcmp(opcja, "stworz", true) == 0) {}
	else if(strcmp(opcja, "usun", true) == 0) {}
	else if(strcmp(opcja, "akceptuj", true) == 0) {}
	else
		Msg(playerid, COLOR_ERROR, "Wpisz: /projekt [lista/stworz/usun/akceptuj]");

	return 1;
}

CMD:obiekt(playerid, params[])
{
	new opcja[10];

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_BUILD)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "s[10]", opcja))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /obiekt [lista/stworz/usun/polozenie]");

	if(strcmp(opcja, "lista", true) == 0) {}
	else if(strcmp(opcja, "stworz", true) == 0) {}
	else if(strcmp(opcja, "usun", true) == 0) {}
	else if(strcmp(opcja, "polozenie", true) == 0) {}
	else
		Msg(playerid, COLOR_ERROR, "Wpisz: /obiekt [lista/stworz/usun/polozenie]");

	return 1;
}

CMD:napis(playerid, params[])
{
	new string[1024], opcja[10], opcja2, firmaid = PlayerInfo[playerid][pFirma];

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_BUILD)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "s[10]D(0)", opcja, opcja2))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /napis [lista/stworz/usun]");

	if(strcmp(opcja, "lista", true) == 0) 
	{
		for(new i=1; i < MAX_BTOBJTD; i++)
		{
			if(BTNapis[i][btnNapis] == Text3D:INVALID_REMONTBT3D_ID)
				continue;

			format(string, sizeof(string), "%s%d\t%s\n", string, i, BTNapis[i][btnStworzyl]);
		}

		if(isnull(string))
			return ShowInfo(playerid, "Brak stworzonych napisów.");
		
		Dialog_Show(playerid, DIALOG_NAPIS_LISTA, DIALOG_STYLE_TABLIST_HEADERS, "Napis > Lista", "ID\tStworzy³\n%s", "SprawdŸ", "Anuluj", string);
	}
	else if(strcmp(opcja, "stworz", true) == 0) 
		Dialog_Show(playerid, DIALOG_NAPIS_STWORZ, DIALOG_STYLE_INPUT, "Napis > Stwórz", "Wpisz treœæ napisu:", "Ok", "Anuluj");
	else if(strcmp(opcja, "usun", true) == 0) 
	{
		if(opcja2 == 0)
			return Msg(playerid, COLOR_ERROR, "Wpisz: /napis usun [id]");

		if(opcja2 >= MAX_BTOBJTD || BTNapis[opcja2][btnNapis] == Text3D:INVALID_REMONTBT3D_ID)
			return Msg(playerid, COLOR_ERROR, "Nieprawid³owe ID.");

		if(strcmp(BTNapis[opcja2][btnStworzyl], PlayerName(playerid), true) == -1 && Firmy[firmaid][tChef] != PlayerInfo[playerid][pID] && Firmy[firmaid][tVcChef] != PlayerInfo[playerid][pID])
			return Msg(playerid, COLOR_ERROR, "Nie mo¿esz zniszczyæ tego napisu - nie Ty go stworzy³eœ.");
		
		SetPVarInt(playerid, "bt_napis_usun", opcja2);
		Dialog_Show(playerid, DIALOG_NAPIS_USUN, DIALOG_STYLE_MSGBOX, "Napis > Usuñ", "Czy na pewno chcesz usun¹æ napis (ID: %d)?", "Tak", "Nie", opcja2);
	}
	else
		Msg(playerid, COLOR_ERROR, "Wpisz: /napis [lista/stworz/usun]");

	return 1;
}

CMD:stworzobiekt(playerid, params[])
{
	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_BUILD)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	new szString[674];
	strcat(szString, "Model\tNazwa\n");
	strcat(szString, "2960\tBelka\n944\tSkrzynie\n3287\tBeczka\n1684\tBudka\n3567\tNaczepa\n11292\tBudka2\n1352\tSwiatla\n8873\tJupiter\n16599\tWodol");
	strcat(szString, "3502\tRura\n3675\tRura2\n12930\tRury\n1685\tKostka\n3572\tKontener\n2699\tKontener2\n1358\tŒmietnik\n1372\tŒmietnik2\n18248\tKoparka\n");
	strcat(szString, "3526\tLampka\n3385\tLampka2\n979\tStrza³ka lewo\n978\tStrza³ka prawo\n973\tBarierka\n3091\tBlokada2981\tBlokada\n1237\tPacholek\n");
	strcat(szString, "1238\tPacholek2\n3504\tToiToi\n1322\tZnak\n3380\tZnak2\n3379\tZnak3\n2780\tDym\n974\tBramka\n16302\t¯wir\n10984\tGruz\n16317\tPiach\n");
	strcat(szString, "3578\tDockbar\n934\tGenerator\n1426\tRusztowanie\n1428\tDrabina\n1437\tDrabina2\n3867\tRusztowania\n1226\tLatarnia");

	return Dialog_Show(playerid, DIALOG_OBIEKT_STWORZ, DIALOG_STYLE_TABLIST_HEADERS, "Wybierz obiekt", szString, "Wybierz", "Anuluj");
}

CMD:changepos(playerid, params[])
{
	new id;
	new string[76];

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_BUILD)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "d", id))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /changepos [id obiektu]");

	if(IsObiektBT[id] == false)
		return Msg(playerid, COLOR_ERROR, "Nie znaleziono tego obiektu.");

	format(string, sizeof string, "{004080}Rozpocz¹³eœ edycjê obiektu o id {FFFFFF}%d.", id);
	SendClientMessage(playerid, 0x0, string);
	EditDynamicObject(playerid, obiektyBT[id]);
	SetPVarInt(playerid, "OBIEKTCHANGEBTID", id);
	return 1;
}

CMD:delone(playerid, params[])
{
	new id;
	new string[76];

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_BUILD)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "d", id))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /delone [id obiektu]");

	if(IsObiektBT[id] == false)
		return Msg(playerid, COLOR_ERROR, "Nie znaleziono tego obiektu.");

	format(string, sizeof string, "{004080}Obiekt o id {FFFFFF}%d {004080}zosta³ usuniêty.", id);
	SendClientMessage(playerid, 0x0, string);
	IsObiektBT[id] = false;
	DestroyDynamicObject(obiektyBT[id]);
	DestroyDynamic3DTextLabel(obiektyBT3D[id]);
	strmid(obiektBTNAME[obiektyBTID],"nothing",0,20);
	strmid(obiektBTNAME2[obiektyBTID],"nothing",0,20);
	return 1;
}

CMD:delall(playerid, params[])
{
	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_BUILD)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	SendClientMessage(playerid, 0x0, "{004080}Wszystkie obiekty zosta³ usuniête.");

	for(new x = 0; x < MAX_BTOBJTD; x++)
	{
		IsObiektBT[x] = false;
		DestroyDynamicObject(obiektyBT[x]);
		DestroyDynamic3DTextLabel(obiektyBT3D[x]);
		strmid(obiektBTNAME[obiektyBTID],"nothing",0,20);
		strmid(obiektBTNAME2[obiektyBTID],"nothing",0,20);
	}
	return 1;
}

CMD:kamera(playerid, params[])
{
	if(GetPVarType(playerid, "FlyMode"))
		return CancelFlyMode(playerid);

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_BUILD)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	new Float:tp[3];
	GetPlayerPos(playerid,tp[0],tp[1],tp[2]);
	SetPVarFloat(playerid, "BTP", tp[0]);
	SetPVarFloat(playerid, "BTP1", tp[1]);
	SetPVarFloat(playerid, "BTP2", tp[2]);
	SetPVarInt(playerid, "FLYKAMERA", 1);
	FlyMode(playerid);
	
	return 1;
}

CMD:obiektname(playerid, params[])
{
	new id;
	new string[200];

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_BUILD)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "d", id))
		return SendClientMessage(playerid, LIGHTRED, "Wpisz: /obiektname [id obiektu]");

	if(IsObiektBT[id] == false)
		return SendClientMessage(playerid, LIGHTRED, "Nie znaleziono takiego obiektu.");

	format(string, sizeof string, "{004080}Obiekt o id {FFFFFF}%d {004080}zosta³ postawiony przez {FFFFFF}%s{004080}, edytowany przez {FFFFFF}%s.", id, obiektBTNAME[id], obiektBTNAME2[id]);
	SendClientMessage(playerid, 0x0, string);
	return 1;
}

Dialog:DIALOG_OBIEKT_STWORZ(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new modelid, Float:Pos;
	sscanf(inputtext, "d{s[20]}", modelid);

	GetPlayerFacingAngle(playerid, Pos);
	PostawObiekt(playerid, modelid, 0, 0, 0, 0.0, 0.0, Pos, 5.0);

	return 1;
}

Dialog:DIALOG_NAPIS_LISTA(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new tekstid;
	sscanf(inputtext, "d{s[20]}", tekstid);

	Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, "Napis > Lista > SprawdŸ", "ID: %d\nStworzy³: %s\nTekst: %s", "Ok", "", tekstid, BTNapis[tekstid][btnStworzyl], BTNapis[tekstid][btnText]);

	return 1;
}

Dialog:DIALOG_NAPIS_STWORZ(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	if(strlen(inputtext) <= 5)
		return cmd_napis(playerid, "stworz");

	new string[80], tekstid = tekstid = GetFreeRemontBTID(), Float:Pos[3];

	GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);

	strcat(string, inputtext);
	delstr(string, "{");
	delstr(string, "}");
	delstr(string, "\t");
	delstr(string, "\n");

	format(BTNapis[tekstid][btnText], 80, "%s", string);
	format(BTNapis[tekstid][btnStworzyl], MAX_PLAYER_NAME, "%s", PlayerName(playerid));
	format(string, sizeof(string), "Build Trans(UID: %d)\nStworzy³: %s\n{FFFFFF}%s", tekstid, PlayerName(playerid), string);

	BTNapis[tekstid][btnNapis] = CreateDynamic3DTextLabel(string, ZIELONY7, Pos[0], Pos[1], Pos[2], 30.0);

	format(string, sizeof(string), "Utworzy³eœ etykietê {b}(ID: %d){/b}.", tekstid);
	Msg(playerid, COLOR_INFO, string);

	return 1;
}

Dialog:DIALOG_NAPIS_USUN(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new opcja2 = GetPVarInt(playerid, "bt_napis_usun"), string[60];

	DeletePVar(playerid, "bt_napis_usun");
	DestroyDynamic3DTextLabel(BTNapis[opcja2][btnNapis]);

	BTNapis[opcja2][btnText][0] = '\0';
	BTNapis[opcja2][btnStworzyl][0] = '\0';
	BTNapis[opcja2][btnNapis] = Text3D:INVALID_REMONTBT3D_ID;

	format(string, sizeof string, "Etykieta {b}(ID: %d){/b} zosta³a usuniêta.", opcja2);
	Msg(playerid, COLOR_INFO, string);

	return 1;
}
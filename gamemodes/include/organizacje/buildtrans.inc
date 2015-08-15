#define MAX_PROJECTS 20

new obiektyBT[MAX_PROJECTS][MAX_BTOBJ];
new Text3D:obiektyBTNapis[MAX_PROJECTS][MAX_BTOBJ];

enum eProjectBT {
	bool:created,
	ProjectBTName[32],
	ProjectBTOwner[32]
}
new projektyBT[MAX_PROJECTS][eProjectBT];

CMD:projekt(playerid, params[])
{
	new opcja[10], firmaid = PlayerInfo[playerid][pFirma], string[1024];

	if(Firmy[firmaid][tTyp] != TEAM_TYPE_BUILD)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "s[10]", opcja))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /projekt [lista/stworz/usun/akceptuj]");

	if(strcmp(opcja, "lista", true) == 0) 
	{
		for(new i; i < MAX_PROJECTS; i++)
			if(projektyBT[i][created])
				format(string, sizeof(string), "%s%d\t%s\t%s\n", string, i, projektyBT[i][ProjectBTOwner], projektyBT[i][ProjectBTName]);

		if(isnull(string))
			Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, "Projekty > Lista", "Brak aktywnych projektów.", "Cofnij", #);
		else
			Dialog_Show(playerid, DIALOG_PROJEKT_AKTYWUJ, DIALOG_STYLE_TABLIST_HEADERS, "Projekty > Lista", "ID\tStworzy³\tNazwa\n%s", "Aktywuj", "Cofnij", string);
	}
	else if(strcmp(opcja, "stworz", true) == 0) 
	{
		new bool:znalazlo;

		for(new i=1; i < MAX_PROJECTS; i++)
			if(!projektyBT[i][created])
			{
				projektyBT[i][created]=true;
				znalazlo=true;
				SetPVarInt(playerid, "projektBT", i);
				break;
			}

		if(!znalazlo)
			return ShowInfo(playerid, "Stworzono maksymaln¹ iloœæ projektów.");

		Dialog_Show(playerid, DIALOG_PROJEKT_STWORZ, DIALOG_STYLE_INPUT, "Projekty > Stwórz", "Wpisz nazwê projektu:", "Stwórz", "Anuluj");
	}
	else if(strcmp(opcja, "usun", true) == 0) 
	{
		//if(Firmy[firmaid][tChef] != PlayerInfo[playerid][pID] && Firmy[firmaid][tVcChef] != PlayerInfo[playerid][pID])
			//return ShowInfo(playerid, "Nie masz uprawnieñ.");

		for(new i; i < MAX_PROJECTS; i++)
			if(projektyBT[i][created])
				format(string, sizeof(string), "%s%d\t%s\t%s\n", string, i, projektyBT[i][ProjectBTOwner], projektyBT[i][ProjectBTName]);

		if(isnull(string))
			Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, "Projekty > Usuñ", "Brak aktywnych projektów.", "Cofnij", #);
		else
			Dialog_Show(playerid, DIALOG_PROJEKT_USUN, DIALOG_STYLE_TABLIST_HEADERS, "Projekty > Usuñ", "ID\tStworzy³\tNazwa\n%s", "Usuñ", "Cofnij", string);
	}
	else if(strcmp(opcja, "akceptuj", true) == 0) 
	{
		//if(Firmy[firmaid][tChef] != PlayerInfo[playerid][pID] && Firmy[firmaid][tVcChef] != PlayerInfo[playerid][pID])
			//return ShowInfo(playerid, "Nie masz uprawnieñ.");

		for(new i; i < MAX_PROJECTS; i++)
			if(projektyBT[i][created])
				format(string, sizeof(string), "%s%d\t%s\t%s\n", string, i, projektyBT[i][ProjectBTOwner], projektyBT[i][ProjectBTName]);

		if(isnull(string))
			Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, "Projekty > Akceptuj", "Brak aktywnych projektów.", "Cofnij", #);
		else
			Dialog_Show(playerid, DIALOG_PROJEKT_AKCEPTUJ, DIALOG_STYLE_TABLIST_HEADERS, "Projekty > Akceptuj", "ID\tStworzy³\tNazwa\n%s", "Akceptuj", "Cofnij", string);
	}
	else
		Msg(playerid, COLOR_ERROR, "Wpisz: /projekt [lista/stworz/usun/akceptuj]");

	return 1;
}

CMD:obiekt(playerid, params[])
{
	new opcja[10], opcja2, projektid = GetPVarInt(playerid, "projektBT"), string[1024];

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_BUILD)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(!projektyBT[projektid][created] || projektid == 0)
			return ShowInfo(playerid, "Najpierw aktywuj swój projekt.");

	if(sscanf(params, "s[10]D(0)", opcja, opcja2))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /obiekt [lista/stworz/usun/polozenie]");

	if(strcmp(opcja, "lista", true) == 0)
	{
		for(new i; i < MAX_BTOBJ; i++)
			if(IsValidDynamicObject(obiektyBT[projektid][i]))
				format(string, sizeof(string), "%s%d\t%d\n", string, i, Streamer_GetIntData(STREAMER_TYPE_OBJECT, obiektyBT[projektid][i], E_STREAMER_MODEL_ID));

		if(isnull(string))
			Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, "Obiekty > Lista", "Brak stworzonych obiektów w tym projekcie.", "Cofnij", #);
		else
			Dialog_Show(playerid, DIALOG_PROJEKT_AKCEPTUJ, DIALOG_STYLE_TABLIST_HEADERS, "Obiekty > Lista", "ID\tModelID\n%s", "Akceptuj", "Cofnij", string);
	}
	else if(strcmp(opcja, "stworz", true) == 0) 
	{
		new szString[674];
		strcat(szString, "Model\tNazwa\n");
		strcat(szString, "2960\tBelka\n944\tSkrzynie\n3287\tBeczka\n1684\tBudka\n3567\tNaczepa\n11292\tBudka2\n1352\tSwiatla\n8873\tJupiter\n16599\tWodol\n");
		strcat(szString, "3502\tRura\n3675\tRura2\n12930\tRury\n1685\tKostka\n3572\tKontener\n2699\tKontener2\n1358\tŒmietnik\n1372\tŒmietnik2\n18248\tKoparka\n");
		strcat(szString, "3526\tLampka\n3385\tLampka2\n979\tStrza³ka lewo\n978\tStrza³ka prawo\n973\tBarierka\n3091\tBlokada2981\tBlokada\n1237\tPacholek\n");
		strcat(szString, "1238\tPacholek2\n3504\tToiToi\n1322\tZnak\n3380\tZnak2\n3379\tZnak3\n2780\tDym\n974\tBramka\n16302\t¯wir\n10984\tGruz\n16317\tPiach\n");
		strcat(szString, "3578\tDockbar\n934\tGenerator\n1426\tRusztowanie\n1428\tDrabina\n1437\tDrabina2\n3867\tRusztowania\n1226\tLatarnia");

		Dialog_Show(playerid, DIALOG_OBIEKT_STWORZ, DIALOG_STYLE_TABLIST_HEADERS, "Wybierz obiekt", szString, "Wybierz", "Anuluj");
	}
	else if(strcmp(opcja, "usun", true) == 0)
		Msg(playerid, COLOR_ERROR, "Niedostepne.");
	else if(strcmp(opcja, "polozenie", true) == 0)
		Msg(playerid, COLOR_ERROR, "Niedostepne.");
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

Dialog:DIALOG_PROJEKT_AKTYWUJ(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new projektid;
	sscanf(inputtext, "d{s[60]}", projektid);

	if(strcmp(projektyBT[projektid][ProjectBTOwner], PlayerName(playerid)))
		return ShowInfo(playerid, "Nie mo¿esz aktywowaæ tego projektu, nie nale¿y do Ciebie.");

	SetPVarInt(playerid, "projektBT", projektid);
	ShowInfo(playerid, "Aktywowa³eœ projekt, mo¿esz tworzyæ obiekty.");
	return 1;
}

Dialog:DIALOG_PROJEKT_STWORZ(playerid, response, listitem, inputtext[])
{
	new projektid = GetPVarInt(playerid, "projektBT");

	if(!response)
	{
		projektyBT[projektid][created]=false;
		return 1;
	}

	if(strlen(inputtext) <= 4)
		return cmd_projekt(playerid, "stworz");

	format(projektyBT[projektid][ProjectBTOwner], 31, "%s", PlayerName(playerid));
	format(projektyBT[projektid][ProjectBTName], 31, "%s", inputtext);
	SetPVarInt(playerid, "projektBTAktywny", projektid);
	ShowInfo(playerid, "Stworzy³eœ projekt, zosta³ aktywowany.\nMo¿esz tworzyæ obiekty.");

	return 1;
}

Dialog:DIALOG_PROJEKT_USUN(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new projektid;
	sscanf(inputtext, "d{s[60]}", projektid);

	projektyBT[projektid][ProjectBTOwner][0] = EOS;
	projektyBT[projektid][ProjectBTName][0] = EOS;
	projektyBT[projektid][created]=false;

	for(new i; i < MAX_BTOBJ; i++)
		if(IsValidDynamicObject(obiektyBT[projektid][i]))
		{
			DestroyDynamicObject(obiektyBT[projektid][i]);
			DestroyDynamic3DTextLabel(obiektyBTNapis[projektid][i]);

			obiektyBT[projektid][i]=INVALID_OBJECT_ID;
			obiektyBT[projektid][i]=INVALID_3DTEXT_ID;
		}

	Msg(playerid, COLOR_INFO3, "Usun¹³eœ projekt.");	
	return 1;
}

Dialog:DIALOG_PROJEKT_AKCEPTUJ(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;
		
	new projektid, modelid, Float:Pos[6], string[200];
	sscanf(inputtext, "d{s[60]}", projektid);

	projektyBT[projektid][ProjectBTOwner][0] = EOS;
	projektyBT[projektid][ProjectBTName][0] = EOS;
	projektyBT[projektid][created]=false;

	for(new i; i < MAX_BTOBJ; i++)
		if(IsValidDynamicObject(obiektyBT[projektid][i]))
		{
			modelid = Streamer_GetIntData(STREAMER_TYPE_OBJECT, obiektyBT[projektid][i], E_STREAMER_MODEL_ID);
			GetDynamicObjectPos(obiektyBT[projektid][i], Pos[0], Pos[1], Pos[2]);
			GetDynamicObjectRot(obiektyBT[projektid][i], Pos[3], Pos[4], Pos[5]);

			format(string, sizeof(string), "INSERT INTO `st_bt_obiekty` VALUES ('', %d, %f, %f, %f, %f, %f, %f);", modelid, Pos[0], Pos[1], Pos[2], Pos[3], Pos[4], Pos[5]);
			mysql_query(string);

			DestroyDynamicObject(obiektyBT[projektid][i]);
			DestroyDynamic3DTextLabel(obiektyBTNapis[projektid][i]);

			obiektyBT[projektid][i]=INVALID_OBJECT_ID;
			obiektyBT[projektid][i]=INVALID_3DTEXT_ID;
		}

	Msg(playerid, COLOR_INFO3, "Zaakceptowa³eœ projekt, trwa zapisywanie obiektów do bazy.");
	return 1;
}

Dialog:DIALOG_OBIEKT_STWORZ(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new projektid = GetPVarInt(playerid, "projektBT");

	new modelid, Float:Pos[3], string[50];
	sscanf(inputtext, "d{s[20]}", modelid);

	GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);

	Msg(playerid, COLOR_INFO2, "Przed pêtl¹.");
	for(new counter; counter < MAX_BTOBJ; counter++)
	{
	    if(obiektyBT[projektid][counter]==0)
		{
			format(string, sizeof string, "{FFFFFF}UID: %d\nProjekt: %s", counter, projektyBT[projektid][ProjectBTName]);

			obiektyBT[projektid][counter] = CreateDynamicObject(modelid, Pos[0], Pos[1], Pos[2], 0, 0, 0);
			obiektyBTNapis[projektid][counter] = CreateDynamic3DTextLabel(string, ZIELONY6, Pos[0], Pos[1], Pos[2], 10.0);

			SetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]+5.0);

			format(string, sizeof(string), "Stworzy³eœ obiekt (ID: %d, projektid: %d, obiektid: %d)", counter, projektid, obiektyBT[projektid][counter]);
			Msg(playerid, COLOR_INFO2, "Stworzy³eœ obiekt.");
			break;
		}
	}
	Msg(playerid, COLOR_INFO2, "Zakoñczono pêtlê.");
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
// TODO:
//		- /changepos
//		- obiekty poprzez /obiekt

#define MAX_PROJECTS 20

new obiektyBT[MAX_PROJECTS][MAX_BTOBJ];
new Text3D:obiektyBTNapis[MAX_PROJECTS][MAX_BTOBJ];

enum eProjectBT {
	bool:created,
	ProjectBTName[32],
	ProjectBTOwner[32]
}
new projektyBT[MAX_PROJECTS][eProjectBT];

new const BTObiects[][][] =
{
	{2960, "Belka"},
	{944, "Skrzynie"},
	{3287, "Beczka"},
	{1684, "Budka"},
	{3567, "Naczepa"},
	{11292, "Budka2"},
	{1352, "Swiatla"},
	{8873, "Jupiter"},
	{16599, "Wodol"},
	{3502, "Rura"},
	{3675, "Rura2"},
	{12930, "Rury"},
	{1685, "Kostka"},
	{3572, "Kontener"},
	{2699, "Kontener2"},
	{1358, "Œmietnik"},
	{1372, "Œmietnik2"},
	{18248, "Koparka"},
	{3526, "Lampka"},
	{3385, "Lampka2"},
	{979, "Strza³ka1"},
	{978, "Strza³ka"},
	{973, "Barierka"},
	{3091, "Blokada"},
	{2981, "Blokada2"},
	{1237, "Pacholek"},
	{1238, "Pacholek2"},
	{3504, "ToiToi"},
	{1322, "Znak"},
	{3380, "Znak2"},
	{3379, "Znak3"},
	{2780, "Dym"},
	{974, "Bramka"},
	{16302, "¯wir"},
	{10984, "Gruz"},
	{16317, "Piach"},
	{3578, "Dockbar"},
	{934, "Generator"},
	{1426, "Rusztowanie"},
	{1428, "Drabina"},
	{1437, "Drabina2"},
	{3867, "Rusztowania"},
	{1226, "Latarnia"}
};

CMD:projekt(playerid, params[])
{
	new opcja[10], firmaid = playerInfo[playerid][pFirm], string[1024];

	if(firmInfo[firmaid][tType] != TEAM_TYPE_BUILD)
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

		if(firmInfo[firmaid][tChef] != playerInfo[playerid][pID] && firmInfo[firmaid][tVcChef] != playerInfo[playerid][pID])
			return ShowInfo(playerid, "Nie masz uprawnieñ.");

		//if(firmInfo[firmaid][tChef] != playerInfo[playerid][pID] && firmInfo[firmaid][tVcChef] != playerInfo[playerid][pID])
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

		if(firmInfo[firmaid][tChef] != playerInfo[playerid][pID] && firmInfo[firmaid][tVcChef] != playerInfo[playerid][pID])
			return ShowInfo(playerid, "Nie masz uprawnieñ.");

		//if(firmInfo[firmaid][tChef] != playerInfo[playerid][pID] && firmInfo[firmaid][tVcChef] != playerInfo[playerid][pID])
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

CMD:changepos(playerid, params[])
{
	format(params, 30, "polozenie %s", params);
	return cmd_obiekt(playerid, params);
}

CMD:delone(playerid, params[])
{
	format(params, 30, "usun %s", params);
	return cmd_obiekt(playerid, params);
}

CMD:obiekt(playerid, params[])
{
	new opcja[10], opcja2, projektid = GetPVarInt(playerid, "projektBT"), string[1024];

	if(firmInfo[playerInfo[playerid][pFirm]][tType] != TEAM_TYPE_BUILD)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(!projektyBT[projektid][created] || projektid == 0)
			return ShowInfo(playerid, "Najpierw aktywuj swój projekt.");

	if(sscanf(params, "s[10]D(5656)", opcja, opcja2))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /obiekt [lista/stworz/usun/polozenie]");

	if(strcmp(opcja, "lista", true) == 0)
	{
		for(new i; i < MAX_BTOBJ; i++)
			if(IsValidDynamicObject(obiektyBT[projektid][i]))
				format(string, sizeof(string), "%s%d\t%d\n", string, i, Streamer_GetIntData(STREAMER_TYPE_OBJECT, obiektyBT[projektid][i], E_STREAMER_MODEL_ID));

		if(isnull(string))
			Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, "Obiekty > Lista", "Brak stworzonych obiektów w tym projekcie.", "Cofnij", #);
		else
			Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_TABLIST_HEADERS, "Obiekty > Lista", "ID\tModelID\n%s", "Cofnij", #, string);
	}
	else if(strcmp(opcja, "stworz", true) == 0) 
	{
		strcat(string, "Model\tNazwa\n");
		
		for (new index = 0; index < sizeof(BTObiects); index++)
		    format(string, sizeof(string), "%d\t%s\n", BTObiects[index][0], BTObiects[index][1]);

		Dialog_Show(playerid, DIALOG_OBIEKT_STWORZ, DIALOG_STYLE_TABLIST_HEADERS, "Obiekt > Stwórz", string, "Wybierz", "Anuluj");
	}
	else if(strcmp(opcja, "usun", true) == 0)
	{
		if(!IsValidDynamicObject(obiektyBT[projektid][opcja2]) || opcja2 == 5656)
			return Msg(playerid, COLOR_ERROR, "Wpisz: /obiekt usun [id] lub /delone [id]");

		DestroyDynamicObject(obiektyBT[projektid][opcja2]);
		DestroyDynamic3DTextLabel(obiektyBTNapis[projektid][opcja2]);

		obiektyBT[projektid][opcja2]=0;
		obiektyBT[projektid][opcja2]=INVALID_3DTEXT_ID;

		format(string, sizeof(string), "Usun¹³eœ obiekt ({b}id: %d{/b}).", opcja2);
		Msg(playerid, COLOR_INFO, string);
	}
	else if(strcmp(opcja, "polozenie", true) == 0)
	{
		if(!IsValidDynamicObject(obiektyBT[projektid][opcja2]) || opcja2 == 5656)
			return Msg(playerid, COLOR_ERROR, "Wpisz: /obiekt polozenie [id] lub /changepos [id]");

		SetPVarInt(playerid, "BTobjID", opcja2);
		EditDynamicObject(playerid, obiektyBT[projektid][opcja2]);
	}
	else
		Msg(playerid, COLOR_ERROR, "Wpisz: /obiekt [lista/stworz/usun/polozenie]");

	return 1;
}

CMD:napis(playerid, params[])
{
	new string[1024], opcja[10], opcja2, firmaid = playerInfo[playerid][pFirm];

	if(firmInfo[playerInfo[playerid][pFirm]][tType] != TEAM_TYPE_BUILD)
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

		if(strcmp(BTNapis[opcja2][btnStworzyl], PlayerName(playerid), true) == -1 && firmInfo[firmaid][tChef] != playerInfo[playerid][pID] && firmInfo[firmaid][tVcChef] != playerInfo[playerid][pID])
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

	if(firmInfo[playerInfo[playerid][pFirm]][tType] != TEAM_TYPE_BUILD)
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
	{
		projektyBT[projektid][created]=false;
		return cmd_projekt(playerid, "stworz");
	}

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

			obiektyBT[projektid][i]=0;
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

			obiektyBT[projektid][i]=0;
			obiektyBT[projektid][i]=INVALID_3DTEXT_ID;
		}

	Msg(playerid, COLOR_INFO3, "Zaakceptowa³eœ projekt, trwa zapisywanie obiektów do bazy.");
	SetTimer("BT_PrzeladujObiekty", 1000, true);
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

	for(new counter; counter < MAX_BTOBJ; counter++)
	{
	    if(obiektyBT[projektid][counter]==0)
		{
			format(string, sizeof string, "{FFFFFF}UID: %d\nProjekt: %s", counter, projektyBT[projektid][ProjectBTName]);

			obiektyBT[projektid][counter] = CreateDynamicObject(modelid, Pos[0], Pos[1], Pos[2], 0, 0, 0);
			obiektyBTNapis[projektid][counter] = CreateDynamic3DTextLabel(string, ZIELONY6, Pos[0], Pos[1], Pos[2], 10.0);

			SetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]+5.0);

			format(string, sizeof(string), "Stworzy³eœ obiekt (ID: %d, projektid: %d, obiektid: %d)", counter, projektid, obiektyBT[projektid][counter]);
			Msg(playerid, COLOR_INFO2, string);
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

forward BT_OnPlayerEditDynObject(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz);
public BT_OnPlayerEditDynObject(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
	if(response == EDIT_RESPONSE_FINAL)
	{
		if(firmInfo[playerInfo[playerid][pFirm]][tType] == TEAM_TYPE_BUILD)
		{
			new id = GetPVarInt(playerid, "BTobjID"), projektid = GetPVarInt(playerid, "projektBT"), text[60];

			SetObjectPos(objectid, x, y, z);
		 	SetObjectRot(objectid, rx, ry, rz);
		 	
		 	GetDynamic3DTextLabelText(obiektyBTNapis[projektid][id], text, sizeof(text));
		 	DestroyDynamic3DTextLabel(obiektyBTNapis[projektid][id]);
		 	obiektyBTNapis[projektid][id] = CreateDynamic3DTextLabel(text, ZIELONY7, x, y, z, 10.0);
		}
	}

	return 1;
}

new ObiektyBT[MAX_BTOBJ];
forward BT_PrzeladujObiekty();
public BT_PrzeladujObiekty()
{
	for(new i; i < MAX_BTOBJ; i++)
		if(IsValidDynamicObject(ObiektyBT[i]))
			DestroyDynamicObject(ObiektyBT[i]);

	CallLocalFunction("BT_OnGameModeInit", "");
	return 1;
}

forward BT_OnGameModeInit();
public BT_OnGameModeInit()
{
	mysql_query("SELECT * FROM `st_bt_obiekty`");
	mysql_store_result();

	if(mysql_num_rows()==0)
		return print("[BT OBIEKTY] Brak obiektow do wczytania.");

	new string[140], id, modelid, Float:Pos[6];
	while(mysql_fetch_row(string, "|")) 
	{
		print(string);
    	sscanf(string, "p<|>ddffffff", id, modelid, Pos[0], Pos[1], Pos[2], Pos[3], Pos[4], Pos[5]);
    	ObiektyBT[id] = CreateDynamicObject(modelid, Pos[0], Pos[1], Pos[2], Pos[3], Pos[4], Pos[5]);
	} 

	mysql_free_result();
	print("[BT OBIEKTY] Wczytano obiekty BT.");
	return 1;
}

forward BT_OnPlayerCommand(playerid, cmdtext[], success);
public BT_OnPlayerCommand(playerid, cmdtext[], success)
{
	new projektid = GetPVarInt(playerid, "projektBT"), i;

	if(firmInfo[playerInfo[playerid][pFirm]][tType] != TEAM_TYPE_BUILD || !projektyBT[projektid][created] || projektid == 0)
		return 0;

	do
	{
        if(!strcmp(cmdtext, BTObiects[i][1], false))
        	return CallLocalFunction("DIALOG_OBIEKT_STWORZ", "ddds", playerid, true, 0, "obiektid\tobiektname\n");
		i++;
	}
	while (i < sizeof(BTObiects));
	return 1;
}
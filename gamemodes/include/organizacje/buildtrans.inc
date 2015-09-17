#define MAX_BT_OBJECTS 	3500
#define MAX_BT_LABELS 	100

#define OBJECT_HEIGHT	0.6

enum E_OBJECTS
{
	btID,
	bool:btCreated,
	bool:btSave,
	btModel,
	btObject,
	Float:btPos[3],
	Float:btRot[3],
	Text3D:btText
}

enum E_LABEL
{
	lbID,
	bool:lbCreated,
	lbAuthor[32],
	lbText[64],
	Float:lbPos[3],
	Text3D:lbTextID
}

new objectInfo[MAX_BT_OBJECTS][E_OBJECTS],
	labelInfo[MAX_BT_LABELS][E_LABEL];

new const BTObiects[][] =
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

CMD:obiekty(playerid)
{
	if(!IsWorked(playerid, TEAM_TYPE_BUILD))
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	new szString[1360];
	for(new i = 0; i < sizeof BTObiects; i++)
	{
		format(szString, sizeof szString, "%s%d\t%s\n", szString, BTObiects[i][0], BTObiects[i][1]);
	}
	strins(szString, "Dostêpne obiekty:\n \n", 0);
	Dialog_Show(playerid, DIALOG_NEVER, DIALOG_STYLE_MSGBOX, " ", szString, "WyjdŸ", #);
	return 1;
}

CMD:ob(playerid, params[])
{
	new opcja[24];

	if(!IsWorked(playerid, TEAM_TYPE_BUILD))
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "s[24]", opcja))
		return Msg(playerid, COLOR_ERROR, "Wpisz /ob [nazwa]");

	new objectFind = -1;
	for(new index = 0; index < sizeof(BTObiects); index++)
	{
		if(strfind(opcja, BTObiects[index][1], true) != -1) 
		{
			objectFind = index;
			break;
		}
	}
	if(objectFind < 0)
		return Msg(playerid, COLOR_ERROR, "Nie odnaleziono obiektu o takiej nazwie.");

	new 
		Float:pos[3], 
		model = BTObiects[objectFind][0];

	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	CreateBTObject(playerid, model, pos[0], pos[1], pos[2]-0.5);
	SetPlayerPos(playerid, pos[0], pos[1], pos[2]+3.0);
	return 1;
}

CMD:kamera(playerid, params[])
{
	if(GetPVarType(playerid, "FlyMode"))
		return CancelFlyMode(playerid);

	if(!IsWorked(playerid, TEAM_TYPE_BUILD))
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ");

	new Float:tp[3];
	GetPlayerPos(playerid,tp[0],tp[1],tp[2]);
	SetPVarFloat(playerid, "BTP", tp[0]);
	SetPVarFloat(playerid, "BTP1", tp[1]);
	SetPVarFloat(playerid, "BTP2", tp[2]);
	SetPVarInt(playerid, "FLYKAMERA", 1);
	FlyMode(playerid);
	return 1;
}

stock CreateBTObject(playerid, model, Float:x, Float:y, Float:z)
{
	new free = -1;
	for(new i = 0; i < MAX_BT_OBJECTS; i++)
	{
		if(IsValidBTObject(i))
			continue;

		free = i;
		break;
	}
	if(free < 0)
		return Msg(playerid, COLOR_ERROR, "Nie mo¿esz stworzyæ wiêcej obiektów.");

	new szString[158];
	format(szString, sizeof szString, "OID: %d", free);
	objectInfo[free][btObject] = CreateDynamicObject(model, x, y, z - OBJECT_HEIGHT, 0.0, 0.0, 0.0);
	objectInfo[free][btText] = CreateDynamic3DTextLabel(szString, -1, x, y, z, 6.0);
	format(szString, sizeof szString, "Pomyœlnie utworzono obiekt ID {b}%d{/b}.", free);
	Msg(playerid, COLOR_INFO2, szString);

	format(szString, sizeof szString, "INSERT INTO objects (ID, modelID, posX, posY, posZ, rotX, rotY, rotZ) VALUES ('0', '%d', '%.03f', '%.03f', '%.03f', '0.0', '0.0', '0.0')",
		model, x, y, z);
	mysql_query(szString);
	objectInfo[free][btID] = mysql_insert_id();
	objectInfo[free][btPos][0] = x;
	objectInfo[free][btPos][1] = y;
	objectInfo[free][btPos][2] = z - OBJECT_HEIGHT;

	objectInfo[free][btRot][0] = 0.0;
	objectInfo[free][btRot][1] = 0.0;
	objectInfo[free][btRot][2] = 0.0;
	objectInfo[free][btCreated] = true;
	return 1;
}

stock DestroyBTObject(playerid, id)
{
	if(!IsValidBTObject(id) || !IsValidDynamicObject(objectInfo[id][btObject]))
		return Msg(playerid, COLOR_ERROR, "Nie odnaleziono obiektu.");

	DestroyDynamic3DTextLabel(objectInfo[id][btText]);
	DestroyDynamicObject(objectInfo[id][btObject]);

	new szString[128];
	format(szString, sizeof szString, "Pomyœlnie usuniêto obiekt ID {b}%d{/b}.", id);
	Msg(playerid, COLOR_INFO2, szString);

	format(szString, sizeof szString, "DELETE FROM objects WHERE id = '%d'", objectInfo[id][btID]);
	mysql_query(szString);

	objectInfo[id][btPos][0] = 0.0;
	objectInfo[id][btPos][1] = 0.0;
	objectInfo[id][btPos][2] = 0.0;
	objectInfo[id][btRot][0] = 0.0;
	objectInfo[id][btRot][1] = 0.0;
	objectInfo[id][btRot][2] = 0.0;
	objectInfo[id][btCreated] = false;
	objectInfo[id][btSave] = false;
	objectInfo[id][btID] = 0;
	return 1;
}

stock ChangePosBTObject(playerid, id, Float:nX, Float:nY, Float:nZ, Float:nRX, Float:nRY, Float:nRZ)
{
	if(!IsValidBTObject(id))
		return Msg(playerid, COLOR_ERROR, "Nie odnaleziono obiektu.");

	objectInfo[id][btSave] = true;
	objectInfo[id][btPos][0] = nX;
	objectInfo[id][btPos][1] = nY;
	objectInfo[id][btPos][2] = nZ;
	objectInfo[id][btRot][0] = nRX;
	objectInfo[id][btRot][1] = nRY;
	objectInfo[id][btRot][2] = nRZ;

	SetDynamicObjectPos(objectInfo[id][btObject], nX, nY, nZ);
	SetDynamicObjectRot(objectInfo[id][btObject], nRX, nRY, nRZ);

	new szString[64];
	format(szString, sizeof szString, "OID: %d", id);
	DestroyDynamic3DTextLabel(objectInfo[id][btText]);
	objectInfo[id][btText] = CreateDynamic3DTextLabel(szString, -1, nX, nY, nZ, 6.0);
	format(szString, sizeof szString, "Pomyœlnie zedytowano obiekt ID {b}%d{/b}.", id);
	Msg(playerid, COLOR_INFO2, szString);
	return 1;
}

stock LoadBTObjects()
{
	new szBuffer[128], szString[48], nums;
	mysql_query("SELECT * FROM objects");
	mysql_store_result();
	nums = mysql_num_rows();
	printf("Zaladowano %d obiektow Build Trans.", nums);
	for(new c = 0; c < nums; c++)
	{
		if(mysql_fetch_row(szBuffer, "|") && c <= MAX_BT_OBJECTS)
		{
			sscanf(szBuffer, "p<|>ddffffff", 
				objectInfo[c][btID], 
				objectInfo[c][btModel], 
				objectInfo[c][btPos][0],
				objectInfo[c][btPos][1],
				objectInfo[c][btPos][2],
				objectInfo[c][btRot][0],
				objectInfo[c][btRot][1],
				objectInfo[c][btRot][2]);

			objectInfo[c][btCreated] = true;
			format(szString, sizeof szString, "OID: %d", c);
			objectInfo[c][btObject] = CreateDynamicObject(objectInfo[c][btModel], objectInfo[c][btPos][0], objectInfo[c][btPos][1], objectInfo[c][btPos][2], objectInfo[c][btRot][0], objectInfo[c][btRot][1], objectInfo[c][btRot][2]);
			objectInfo[c][btText] = CreateDynamic3DTextLabel(szString, -1, objectInfo[c][btPos][0], objectInfo[c][btPos][1], objectInfo[c][btPos][2], 6.0);
		}
	}
	mysql_free_result();
	return 1;
}

stock LoadBTLabels()
{
	new szBuffer[128], szString[128], nums;
	mysql_query("SELECT * FROM labels");
	mysql_store_result();
	nums = mysql_num_rows();
	printf("Zaladowano %d etykiet Build Trans.", nums);
	for(new c = 0; c < nums; c++)
	{
		if(mysql_fetch_row(szBuffer, "|") && c <= MAX_BT_LABELS)
		{
			sscanf(szBuffer, "p<|>ds[32]fffs[64]", 
				labelInfo[c][lbID],
				labelInfo[c][lbAuthor],
				labelInfo[c][lbPos][0],
				labelInfo[c][lbPos][1],
				labelInfo[c][lbPos][2],
				labelInfo[c][lbText]);

			labelInfo[c][lbCreated] = true;
			format(szString, sizeof szString, "{072085}Etykieta Build Trans (%d)\nStworzy³ {FFFFFF}%s\n%s", c, labelInfo[c][lbAuthor], labelInfo[c][lbText]);
			labelInfo[c][lbTextID] = CreateDynamic3DTextLabel(szString, -1, labelInfo[c][lbPos][0], labelInfo[c][lbPos][1], labelInfo[c][lbPos][2], 50.0);
		}
	}
	mysql_free_result();
	return 1;
}

stock IsValidBTObject(id)
{
	if(objectInfo[id][btCreated])
		return true;
	return false;
}

stock SaveBTObjects()
{
	new szString[200];
	for(new i = 0; i < MAX_BT_OBJECTS; i++)
	{
		if(!objectInfo[i][btSave] || !IsValidBTObject(i) || !IsValidDynamicObject(objectInfo[i][btObject]))
			continue;

		format(szString, sizeof szString, "UPDATE objects SET posX = '%.03f', posY = '%.03f', posZ = '%.03f', rotX = '%.03f', rotY = '%.03f', rotZ = '%.03f' WHERE ID = '%d'",
			objectInfo[i][btPos][0], objectInfo[i][btPos][1], objectInfo[i][btPos][2], objectInfo[i][btRot][0], objectInfo[i][btRot][1], objectInfo[i][btRot][2], objectInfo[i][btID]);
		mysql_query(szString);
		objectInfo[i][btSave] = false;
	}
	return 1;
}

CMD:del(playerid, params[])
{
	new option[10], objectid;
	if(!IsWorked(playerid, TEAM_TYPE_BUILD))
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ");
	
	if(sscanf(params, "s[10]D(3501)", option, objectid))
		return Msg(playerid, COLOR_ERROR, "Wpisz /del [one/all] [id]");

	if(!strcmp(option, "one"))
	{
		if(objectid > 3500 || objectid < 0)
			return Msg(playerid, COLOR_ERROR, "Wpisz /del [one/all] [id]");

		DestroyBTObject(playerid, objectid);
	}
	else if(!strcmp(option, "all"))
	{
		if(!(IsFractionBoss(playerid) || IsFractionViceBoss(playerid)))
			return Msg(playerid, COLOR_ERROR, "Musisz byæ szefem lub vice szefem.");

		mysql_query("DELETE FROM objects");
		new count;
		for(new i = 0; i < MAX_BT_OBJECTS; i++)
		{
			if(!IsValidBTObject(i) || !IsValidDynamicObject(objectInfo[i][btObject]))
				continue;

			DestroyDynamic3DTextLabel(objectInfo[i][btText]);
			DestroyDynamicObject(objectInfo[i][btObject]);

			objectInfo[i][btPos][0] = 0.0;
			objectInfo[i][btPos][1] = 0.0;
			objectInfo[i][btPos][2] = 0.0;
			objectInfo[i][btRot][0] = 0.0;
			objectInfo[i][btRot][1] = 0.0;
			objectInfo[i][btRot][2] = 0.0;
			objectInfo[i][btCreated] = false;
			objectInfo[i][btSave] = false;
			objectInfo[i][btID] = 0;
			count++;
		}
		new szString[64];
		format(szString, sizeof szString, "Usuniêto {b}%d{/b} obiekt/ów.", count);
		Msg(playerid, COLOR_INFO2, szString);
	}
	else 
		return Msg(playerid, COLOR_ERROR, "Wpisz /del [one/all] [id]");
	return 1;
}

CMD:changepos(playerid, params[])
{
	new objectid;
	if(GetPVarInt(playerid, "BT_Edit"))
		return Msg(playerid, COLOR_ERROR, "Ju¿ edytujesz jakiœ obiekt.");

	if(!IsWorked(playerid, TEAM_TYPE_BUILD))
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ");

	if(sscanf(params, "d", objectid))
		return Msg(playerid, COLOR_ERROR, "Wpisz /changepos [id]");

	if(!IsValidDynamicObject(objectInfo[objectid][btObject]) || !IsValidBTObject(objectid))
		return Msg(playerid, COLOR_ERROR, "Nie odnaleziono obiektu.");

	EditDynamicObject(playerid, objectInfo[objectid][btObject]);
	SetPVarInt(playerid, "BT_Edit", 1);
	SetPVarInt(playerid, "BT_Object", objectid);
	return 1;
}

CMD:label(playerid, params[])
{
	new option[10], labelid;
	if(!IsWorked(playerid, TEAM_TYPE_BUILD))
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ");

	if(sscanf(params, "s[10]D(101)", option, labelid))
		return Msg(playerid, COLOR_ERROR, "Wpisz /label [stworz/usun] [id]");

	if(!strcmp(option, "stworz"))
	{
		Dialog_Show(playerid, DIALOG_LABEL_CREATE, DIALOG_STYLE_INPUT, " ", "WprowadŸ poni¿ej treœæ swojej etykiety:", "Stwórz", "Anuluj");
	}
	else if(!strcmp(option, "usun"))
	{
		if(labelid > 100 || labelid < 0)
			return Msg(playerid, COLOR_ERROR, "Wpisz /label [stworz/usun] [id]");

		DestroyBTLabel(playerid, labelid);
	}
	else 
		return Msg(playerid, COLOR_ERROR, "Wpisz /label [stworz/usun] [id]");
	return 1;
}

Dialog:DIALOG_LABEL_CREATE(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	if(strlen(inputtext) < 5 || strlen(inputtext) > 64)
		return Dialog_Show(playerid, DIALOG_LABEL_CREATE, DIALOG_STYLE_INPUT, " ", "Wprowadzi³eœ zby krótki lub d³ugi tekst\nWprowadŸ poni¿ej treœæ swojej etykiety:", "Stwórz", "Anuluj");
	
	new Float:pos[3];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	CreateBTLabel(playerid, inputtext, pos[0], pos[1], pos[2]);
	return 1;
}

stock CreateBTLabel(playerid, text[], Float:x, Float:y, Float:z)
{
	new free = -1;
	for(new i = 0; i < MAX_BT_LABELS; i++)
	{
		if(IsValidBTLabel(i))
			continue;

		free = i;
		break;
	}
	if(free < 0)
		return Msg(playerid, COLOR_ERROR, "Nie mo¿esz stworzyæ wiêcej etykiet.");

	new szString[198];
	format(szString, sizeof szString, "{072085}Etykieta Build Trans (%d)\nStworzy³ {FFFFFF}%s\n%s", free, PlayerName(playerid), text);
	labelInfo[free][lbTextID] = CreateDynamic3DTextLabel(szString, -1, x, y, z+0.5, 50.0);
	format(szString, sizeof szString, "Pomyœlnie utworzono etykietê ID {b}%d{/b}.", free);
	Msg(playerid, COLOR_INFO2, szString);

	format(szString, sizeof szString, "INSERT INTO labels (ID, author, posX, posY, posZ, text) VALUES ('0', '%s', '%.03f', '%.03f', '%.03f', '%s')",
		PlayerName(playerid), x, y, z, text);
	mysql_query(szString);
	labelInfo[free][lbID] = mysql_insert_id();
	labelInfo[free][lbPos][0] = x;
	labelInfo[free][lbPos][1] = y;
	labelInfo[free][lbPos][2] = z;
	strcat(labelInfo[free][lbText], text);
	strcat(labelInfo[free][lbAuthor], PlayerName(playerid));

	labelInfo[free][lbCreated] = true;
	return 1;
}

stock IsValidBTLabel(labid)
{
	if(labelInfo[labid][lbCreated])
		return true;
	return false;
}

stock DestroyBTLabel(playerid, id)
{
	if(!IsValidBTLabel(id))
		return Msg(playerid, COLOR_ERROR, "Nie odnaleziono etykiety.");

	DestroyDynamic3DTextLabel(labelInfo[id][lbTextID]);

	new szString[128];
	format(szString, sizeof szString, "Pomyœlnie usuniêto etykietê ID {b}%d{/b}.", id);
	Msg(playerid, COLOR_INFO2, szString);

	format(szString, sizeof szString, "DELETE FROM labels WHERE ID = '%d'", labelInfo[id][lbID]);
	mysql_query(szString);

	labelInfo[id][lbPos][0] = 0.0;
	labelInfo[id][lbPos][1] = 0.0;
	labelInfo[id][lbPos][2] = 0.0;
	labelInfo[id][lbCreated] = false;
	labelInfo[id][lbID] = 0;
	return 1;
}

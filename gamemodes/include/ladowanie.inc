LadujOrganizacje()
{
	mysql_query("SELECT `id`, `Szef`, `VcSzef`, `Ksiegowy`, `Typ`, `Bank`, `SpawnX`, `SpawnY`, `SpawnZ`, `Skin1`, `Skin2`, `Skin3`, `Skin4`, `Skin5`, `Kolor`, `Nazwa` FROM `Firmy`");
	mysql_store_result();

	if(mysql_num_rows()==0)
		return print("[ORGANIZACJE] Brak organizacji do zaladowania.");

	new ile, string[140], id;
	while(mysql_fetch_row(string, "|")) 
	{
		mysql_fetch_field("id", string);
		id = strval(string);
		Firmy[id][tID] = id;

		mysql_fetch_field("Nazwa", Firmy[id][tName]);
		
		mysql_fetch_field("Szef", string);
		Firmy[id][tChef] = strval(string);

		mysql_fetch_field("VcSzef", string);
		Firmy[id][tVcChef] = strval(string);

		mysql_fetch_field("Ksiegowy", string);
		Firmy[id][tKsiegowy] = strval(string);

		mysql_fetch_field("Typ", string);
		Firmy[id][tTyp] = strval(string);

		mysql_fetch_field("Bank", string);
		Firmy[id][tBank] = strval(string);

		mysql_fetch_field("SpawnX", string);
		Firmy[id][tSpawnX] = floatstr(string);

		mysql_fetch_field("SpawnY", string);
		Firmy[id][tSpawnY] = floatstr(string);

		mysql_fetch_field("SpawnZ", string);
		Firmy[id][tSpawnZ] = floatstr(string);

		mysql_fetch_field("Skin1", string);
		Firmy[id][tSkin1] = strval(string);

		mysql_fetch_field("Skin2", string);
		Firmy[id][tSkin2] = strval(string);

		mysql_fetch_field("Skin3", string);
		Firmy[id][tSkin3] = strval(string);

		mysql_fetch_field("Skin4", string);
		Firmy[id][tSkin4] = strval(string);

		mysql_fetch_field("Skin5", string);
		Firmy[id][tSkin5] = strval(string);

		mysql_fetch_field("Kolor", string);
		Firmy[id][tKolor] = strval(string);

		printf("Name: %s Type: %d", Firmy[id][tName], Firmy[id][tTyp]);
		ile++;
	} 

	printf("[ORGANIZACJE] Zaladowano %d organizacji.", ile);
	mysql_free_result();

	for(new i=1; i<=ile; i++)
		LadujPojazd(Firmy[i][tID], OWNER_TYPE_TEAM);
		
	return 1;
}

LadujFotoradary()
{
	mysql_query("SELECT * FROM `Fotoradary`");
	mysql_store_result();

	if(mysql_num_rows()==0)
		return print("[FOTORADARY] Brak fotoradarow do zaladowania.");

	new string[140];
	while(mysql_fetch_row(string, "|")) 
	{
    	sscanf(string, "e<p<|>dffffffddds[30]>", Fotoradary[ID_FOTO]);

    	FotoradarOBJ[ID_FOTO] = CreateObject(18880, Fotoradary[ID_FOTO][fPosX], Fotoradary[ID_FOTO][fPosY], Fotoradary[ID_FOTO][fPosZ], Fotoradary[ID_FOTO][fRX], Fotoradary[ID_FOTO][fRY], Fotoradary[ID_FOTO][fRZ]);

		ID_FOTO++;
	} 

	printf("[FOTORADARY] Zaladowano %d fotoradarow.", ID_FOTO);
	mysql_free_result();
	return 1;
}

LadujSklepyVIA()
{
	mysql_query("SELECT * FROM `VIASHOP`");
	mysql_store_result();

	if(mysql_num_rows()==0)
		return print("[VIASHOP] Brak ViaTolli do zaladowania.");

	new string[140];
	while(mysql_fetch_row(string, "|")) 
	{
    	sscanf(string, "e<p<|>dfff>", ViaToll[ID_VIATOLL]);

		format(string, sizeof string, "{808040}SKLEP VIATOLL {808040}[VID: {FFFFFF}%d{808040}]\n{FFFFFF}/viatoll", ID_VIATOLL);
		viashop3D[ID_VIATOLL] = CreateDynamic3DTextLabel(string, ZIELONY3, ViaToll[ID_VIATOLL][vtPosX], ViaToll[ID_VIATOLL][vtPosY], ViaToll[ID_VIATOLL][vtPosZ]+0.5, 30.0);
		viashopIKON[ID_VIATOLL] = CreateDynamicMapIcon(ViaToll[ID_VIATOLL][vtPosX], ViaToll[ID_VIATOLL][vtPosY], ViaToll[ID_VIATOLL][vtPosZ], 56, LIGHTRED, _, _, _, 300);

		ID_VIATOLL++;
	}

	printf("[VIATOLL] Zaladowano %d ViaTolli.", ID_VIATOLL);
	mysql_free_result();
	return 1;
}

LadujStacje()
{
	mysql_query("SELECT * FROM `Stacje`");
	mysql_store_result();

	if(mysql_num_rows()==0)
		return print("[STACJE] Brak stacji do zaladowania.");

	new string[140];
	while(mysql_fetch_row(string, "|")) 
	{
    	sscanf(string, "e<p<|>dfffd>", Stacje[ID_STACJA]);

    	format(string, sizeof string, "{808040}STACJA BENZYNOWA {808040}[VID: {FFFFFF}%d{808040}]\n[ Zbiorniki: %d / 3000 ]\n{FFFFFF}/tankuj", ID_STACJA, Stacje[ID_STACJA][Zbiornik]);
    	StacjeText3D[ID_STACJA] = CreateDynamic3DTextLabel(string, ZIELONY2, Stacje[ID_STACJA][sPosX], Stacje[ID_STACJA][sPosY], Stacje[ID_STACJA][sPosZ]+0.5, 30.0);

    	for(new playerid; playerid < MAX_PLAYERS; playerid++)
			StacjeMapIcon[ID_STACJA][playerid] = CreateDynamicMapIcon(Stacje[ID_STACJA][sPosX], Stacje[ID_STACJA][sPosY], Stacje[ID_STACJA][sPosZ], 55, 0, _, _, playerid, 300.0);

		ID_STACJA++;
	} 

	printf("[STACJE] Zaladowano %d stacji.", ID_STACJA);
	mysql_free_result();
	return 1;
}

LadujPojazd(ownerid=(-1), ownertype=(-1), vehid=(-1))
{
	new query[80];

	format(query, sizeof(query), "SELECT * FROM `Pojazdy` WHERE ");

	if(vehid > (-1))
		format(query, sizeof(query), "%s `id` = '%d'", query, vehid);
	else
		format(query, sizeof(query), "%s `owner_id` = '%d' AND `owner_type` = '%d'", query, ownerid, ownertype);

	mysql_query(query);
	mysql_store_result();

	if(mysql_num_rows()==0)
		return printf("[POJAZDY] Brak pojazdow do zaladowania (OWNER_ID: %d, OWNER_TYPE: %d, VEHID: %d).", ownerid, ownertype, vehid);

	new id[32], string[200], ids, VehID;
	while(mysql_fetch_row(string, "|")) 
	{
		mysql_fetch_field("id", id);

    	sscanf(string, "e<p<|>ddddffffffdfdddddddds[32]>", vehInfo[strval(id)]);

    	VehID = CreateVehicle(vehInfo[strval(id)][vModel], vehInfo[strval(id)][vPosX], vehInfo[strval(id)][vPosY], vehInfo[strval(id)][vPosZ], vehInfo[strval(id)][vPosA], vehInfo[strval(id)][vColor1], vehInfo[strval(id)][vColor2], -1);

    	vehInfo[strval(id)][vSAMPID] = VehID;
    	SetVehicleParamsEx(VehID, VEHICLE_PARAMS_OFF, VEHICLE_PARAMS_OFF, VEHICLE_PARAMS_OFF, vehInfo[strval(id)][vLocked], VEHICLE_PARAMS_OFF, VEHICLE_PARAMS_OFF, VEHICLE_PARAMS_OFF);
    	SetVehicleHealth(VehID, vehInfo[strval(id)][vHealth]);
    	SetVehicleNumberPlate(VehID, vehInfo[strval(id)][vPlate]);
    	UpdateVehicleDamageStatus(VehID, vehInfo[strval(id)][vPanels], vehInfo[strval(id)][vDoors], vehInfo[strval(id)][vLights], vehInfo[strval(id)][vTires]);

    	DBVehID[VehID] = strval(id);

    	if(strval(id) > HighVehicleID)
    		HighVehicleID = strval(id);

		ids++;
	} 

	printf("[POJAZDY] Zaladowano %d pojazdow (OWNER_ID: %d, OWNER_TYPE: %d, VEHID: %d).", ids, ownerid, ownertype, vehid);
	mysql_free_result();
	return 1;
}

LadujSzybkieBary()
{
/*new DBResult:result;
result = db_query(Cargos, "SELECT ID, PosX, PosY, PosZ FROM `BARYIN`");
new numRows = db_num_rows(result);

for(new i=0; i<numRows; i++)
{
new I[20], Posx[20], Posy[20], Posz[20];
db_get_field(result, 0, I, sizeof I);
db_get_field(result, 1, Posx, sizeof Posx);
db_get_field(result, 2, Posy, sizeof Posy);
db_get_field(result, 3, Posz, sizeof Posz);

new baryinid, Float:Pos[3];
baryinid = strval(I);
Pos[0] = floatstr(Posx);
Pos[1] = floatstr(Posy);
Pos[2] = floatstr(Posz);

new string[76];
format(string, sizeof string, "{400040}Bar [VID: {FFFFFF}%d{400040}]\n{FFFFFF}/menu", baryinid);
baryin3D[baryinid] = CreateDynamic3DTextLabel(string, ZIELONY4, Pos[0], Pos[1], Pos[2]+0.5, 30.0);

db_next_row(result);
}
db_free_result(result);
return 1;*/
}

LadujBary()
{
	/*new DBResult:result;
	result = db_query(Cargos, "SELECT ID, PosX, PosY, PosZ FROM `BARY`");
	new numRows = db_num_rows(result);

	for(new i=0; i<numRows; i++)
	{
		new I[20], Posx[20], Posy[20], Posz[20];
		db_get_field(result, 0, I, sizeof I);
		db_get_field(result, 1, Posx, sizeof Posx);
		db_get_field(result, 2, Posy, sizeof Posy);
		db_get_field(result, 3, Posz, sizeof Posz);

		new barid, Float:Pos[3];
		barid = strval(I);
		Pos[0] = floatstr(Posx);
		Pos[1] = floatstr(Posy);
		Pos[2] = floatstr(Posz);

		new string[76];
		format(string, sizeof string, "{004080}BAR {004080}[VID: {FFFFFF}%d{004080}]\n{FFFFFF}/wejdz", barid);
		bary3D[barid] = CreateDynamic3DTextLabel(string, ZIELONY4, Pos[0], Pos[1], Pos[2]+0.5, 30.0);
		baryIKON[barid] = CreateDynamicMapIcon(Pos[0], Pos[1], Pos[2], 10, LIGHTRED);

		db_next_row(result);
	}
	db_free_result(result);
	return 1;*/
}
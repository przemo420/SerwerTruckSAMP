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
		firmInfo[id][tID] = id;

		mysql_fetch_field("Nazwa", firmInfo[id][tName]);
		
		mysql_fetch_field("Szef", string);
		firmInfo[id][tChef] = strval(string);

		mysql_fetch_field("VcSzef", string);
		firmInfo[id][tVcChef] = strval(string);

		mysql_fetch_field("Ksiegowy", string);
		firmInfo[id][tAccountant] = strval(string);

		mysql_fetch_field("Typ", string);
		firmInfo[id][tType] = strval(string);

		mysql_fetch_field("Bank", string);
		firmInfo[id][tBank] = strval(string);

		mysql_fetch_field("SpawnX", string);
		firmInfo[id][tSpawnX] = floatstr(string);

		mysql_fetch_field("SpawnY", string);
		firmInfo[id][tSpawnY] = floatstr(string);

		mysql_fetch_field("SpawnZ", string);
		firmInfo[id][tSpawnZ] = floatstr(string);

		mysql_fetch_field("Skin1", string);
		firmInfo[id][tSkin1] = strval(string);

		mysql_fetch_field("Skin2", string);
		firmInfo[id][tSkin2] = strval(string);

		mysql_fetch_field("Skin3", string);
		firmInfo[id][tSkin3] = strval(string);

		mysql_fetch_field("Skin4", string);
		firmInfo[id][tSkin4] = strval(string);

		mysql_fetch_field("Skin5", string);
		firmInfo[id][tSkin5] = strval(string);

		mysql_fetch_field("Kolor", string);
		firmInfo[id][tColor] = strval(string);

		ile++;
	} 

	printf("[ORGANIZACJE] Zaladowano %d organizacji.", ile);
	mysql_free_result();

	for(new i=1; i<=ile; i++)
		LadujPojazd(firmInfo[i][tID], OWNER_TYPE_TEAM);
		
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

    	sscanf(string, "e<p<|>ddddffffffdfdddddddds[32]s[32]df>", vehInfo[strval(id)]);

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

LadujBary()
{
	new data[128];
	mysql_query("SELECT * FROM restaurants");
	mysql_store_result(); 
	new rows = mysql_num_rows();
	for(new i = 0; i < rows; i++)
	{
		if(mysql_fetch_row(data) && i < 512)
		{
			new szString[128];
			sscanf(data, "p<|>dfffd", 
			resInfo[i][resId],
			resInfo[i][resPos][0],
			resInfo[i][resPos][1],
			resInfo[i][resPos][2],
			resInfo[i][resInt]);
			resInfo[i][resCreated] = true;

			format(szString, sizeof szString, "Restauracja w {b}%s{/b} (ID: {b}%d{/b}).\nAby u¿yæ wpisz {b}/wejdz{/b}.", GetPointZone(resInfo[i][resPos][0], resInfo[i][resPos][1]), i);
	   		CreateDynamic3DTextLabel(clText(COLOR_INFO2, szString), -1, resInfo[i][resPos][0], resInfo[i][resPos][1], resInfo[i][resPos][2], 20.0);
	   		CreateDynamicMapIcon(resInfo[i][resPos][0], resInfo[i][resPos][1], resInfo[i][resPos][2], 10, LIGHTGREEN, _, _, _, 800.0, MAPICON_LOCAL);
	   		new Float:pos[3];
	   		GetPosRestaurantInterior(resInfo[i][resInt], pos[0], pos[1], pos[2]);
	   		CreateDynamicPickup(1318, 1, pos[0], pos[1], pos[2], i + 1, resInfo[i][resInt]);
	   		GetInteriorMenuPos(resInfo[i][resInt], pos[0], pos[1], pos[2]);
			CreateDynamicCP(pos[0], pos[1], pos[2], 2.0, i + 1, resInfo[i][resInt]);
		}
	}
	mysql_free_result();
	return 1;
}

stock LoadBanWords()
{
	new query[32];
	new data[512];
	format(query, sizeof(query), "SELECT Word FROM words");
	mysql_query(query);
	mysql_store_result(); 
	new rows = mysql_num_rows();
	for(new i = 0; i < rows; i++)
	{
		if(mysql_fetch_row(data) && i < 512)
		{
			sscanf(data, "p<|>s[50]", 
			banwords[i]);
		}
	}
	mysql_free_result();
}
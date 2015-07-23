CMD:via(playerid, params[])
	return cmd_viatoll(playerid, params);
	
CMD:viatoll(playerid, params[])
{
	new viashopid = GetViaShop(playerid);
	if(viashopid == INVALID_VIASHOP_ID)
		return ShowInfo(playerid, "Nie znajdujesz si� przy sklepie ViaToll.");

	Dialog_Show(playerid, DIALOG_ID_VIATOLL, DIALOG_STYLE_LIST, "Sklep ViaToll", "Kup 1 impuls [$25]\nKup 5 impuls�w [$125]\nKup 10 impuls�w [$250]\nKup 20 impuls�w [$500]\nKup 50 impuls�w [$1250]", "Ok", "Anuluj");
	return 1;
}

CMD:dodajviatoll(playerid, params[])
{
	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "Nie masz uprawnie�.");

	new string[128];

	new postawionyID = GetViaShop(playerid);
	if(postawionyID > INVALID_VIASHOP_ID)
	{
		format(string, sizeof string, "Sklep viatoll zosta� ju� utworzony, uid: %d.", postawionyID);
		SendClientMessage(playerid,LIGHTRED,string);
		return 1;
	}

	new Float:Pos[3];
	GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);

	format(string, sizeof string, "INSERT INTO `VIASHOP` VALUES('', '%f', '%f', '%f')", Pos[0], Pos[1], Pos[2]);
	mysql_query(string);

	format(string, sizeof string, "{808040}SKLEP VIATOLL {808040}[VID: {FFFFFF}%d{808040}]\n{FFFFFF}/viatoll", ID_VIATOLL);
	viashop3D[ID_VIATOLL] = CreateDynamic3DTextLabel(string, ZIELONY3, Pos[0], Pos[1], Pos[2]+0.5, 30.0);
	viashopIKON[ID_VIATOLL] = CreateDynamicMapIcon(Pos[0], Pos[1], Pos[2], 56, LIGHTRED);

	format(string, sizeof string, "{008000}Sklep viatoll o uid: {FFFFFF}%d {008000}zosta�pomy�lnie utworzony.", ID_VIATOLL);
	SendClientMessage(playerid,0x0,string);

	ViaToll[ID_VIATOLL][vtID] = mysql_insert_id();
	ViaToll[ID_VIATOLL][vtPosX] = Pos[0];
	ViaToll[ID_VIATOLL][vtPosY] = Pos[1];
	ViaToll[ID_VIATOLL][vtPosZ] = Pos[2];

	ID_VIATOLL++;

	return 1;
}

CMD:usunviatoll(playerid, params[])
{
	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "Nie masz uprawnie�.");

	new string[132];

	new viashopid = GetViaShop(playerid);
	if(viashopid == INVALID_VIASHOP_ID)
		return SendClientMessage(playerid, LIGHTRED, "Nie znajdujesz si� przy sklepie viatoll.");

	format(string, sizeof string, "{008000}Sklep viatoll o uid: {FFFFFF}%d {008000}zosta� pomy?lnie utworzony.", viashopid);
	SendClientMessage(playerid,0x0,string);

	DestroyDynamic3DTextLabel(viashop3D[viashopid]);
	DestroyDynamicMapIcon(viashopIKON[viashopid]);
	format(string, sizeof string, "DELETE FROM `VIASHOP` WHERE `ID`= '%d'", viashopid);
	mysql_query(string);

	ViaToll[viashopid][vtID] = 0;
	ViaToll[viashopid][vtPosX] = 0.0;
	ViaToll[viashopid][vtPosY] = 0.0;
	ViaToll[viashopid][vtPosZ] = 0.0;

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

forward HideViaBlip(playerid);
public HideViaBlip(playerid)
	return SetPVarInt(playerid, "ViaPoint", 0), TextDrawHideForPlayer(playerid, Speedometer[ViaTollPlus]);

ViaToll_Blink(playerid)
{
	if( (IsPlayerInTruck(playerid) || IsPlayerInBus(playerid)) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		if(GetViaMoney(playerid) <= 0)
		{
			if(Speedometer[ViaTollVisible])
				TextDrawShowForPlayer(playerid, Speedometer[ViaTollX]);
			else
				TextDrawHideForPlayer(playerid, Speedometer[ViaTollX]);
		}
		else
			TextDrawHideForPlayer(playerid, Speedometer[ViaTollX]);
	}

	Speedometer[ViaTollVisible] = !Speedometer[ViaTollVisible];
}

ViaToll_Blip(playerid)
{
	Loop(i, sizeof viaPoints)
	{
		new viaTOLL = GetDistanceFromMeToPoint(playerid, viaPoints[i][0], viaPoints[i][1], viaPoints[i][2]);
		if(viaTOLL <= 15)
		{
			if(!GetPVarInt(playerid, "ViaPoint"))
			{
				if(PlayerVehicleIsTruck(playerid))
				{
					GiveViaMoney(playerid, -1);

					if(GetViaMoney(playerid) >= 1)
						TextDrawShowForPlayer(playerid, Speedometer[ViaTollPlus]), SetTimerEx("HideViaBlip", 3000, false, "d", playerid);

					
					SetPVarInt(playerid, "ViaPoint", 1);
				}
			}
		}
	}
}

GetViaShop(playerid)
{
	new Float:playerPos[3], viatollid;
	GetPlayerPos(playerid, playerPos[0], playerPos[1], playerPos[2]);

	for(new i; i<=ID_VIATOLL; i++)
	{
	    if(ViaToll[i][vtID])
			if(IsPlayerInRangeOfPoint(playerid, 10.0, ViaToll[i][vtPosX], ViaToll[i][vtPosY], ViaToll[i][vtPosZ]))
			{
			    viatollid = i;
			    break;
			}
	}

	return viatollid;
}

Dialog:DIALOG_ID_VIATOLL(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new impulsow, koszt, str[40];

	switch(listitem)
	{
		case 0: {impulsow=1; koszt=25;}
		case 2: {impulsow=5; koszt=125;}
		case 3: {impulsow=10; koszt=250;}
		case 4: {impulsow=20; koszt=500;}
		case 5: {impulsow=50; koszt=1250;}
		default: {impulsow=0; koszt=0;}
	}

	if(GetMoney(playerid) < koszt)
		return ShowInfo(playerid, "Nie masz tyle pieni�dzy.");

	GiveMoney(playerid, -koszt);
	GiveViaMoney(playerid, impulsow);
	format(str, sizeof(str), "Kupi�e� %d impuls/�w za $%d.", impulsow, koszt);
	ShowInfo(playerid, str);
	return 1;
}
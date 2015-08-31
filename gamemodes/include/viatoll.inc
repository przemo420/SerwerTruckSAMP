CMD:via(playerid, params[])
	return cmd_viatoll(playerid, params);
	
CMD:viatoll(playerid, params[])
{
	new viashopid = GetViaShop(playerid);
	if(viashopid == INVALID_VIASHOP_ID)
		return ShowInfo(playerid, "Nie znajdujesz siê przy sklepie ViaToll.");

	Dialog_Show(playerid, DIALOG_ID_VIATOLL, DIALOG_STYLE_LIST, "Sklep ViaToll", "Kup 1 impuls [$25]\nKup 5 impulsów [$125]\nKup 10 impulsów [$250]\nKup 20 impulsów [$500]\nKup 50 impulsów [$1250]", "Ok", "Anuluj");
	return 1;
}

CMD:dodajviatoll(playerid, params[])
{
	if(!playerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "Nie masz uprawnieñ.");

	new string[128];

	new postawionyID = GetViaShop(playerid);
	if(postawionyID > INVALID_VIASHOP_ID)
	{
		format(string, sizeof string, "Sklep viatoll został już utworzony, uid: %d.", postawionyID);
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

	format(string, sizeof string, "{008000}Sklep viatoll o uid: {FFFFFF}%d {008000}zosta³pomyœlnie utworzony.", ID_VIATOLL);
	SendClientMessage(playerid, 0x0, string);

	viaInfo[ID_VIATOLL][vtID] = mysql_insert_id();
	viaInfo[ID_VIATOLL][vtPosX] = Pos[0];
	viaInfo[ID_VIATOLL][vtPosY] = Pos[1];
	viaInfo[ID_VIATOLL][vtPosZ] = Pos[2];

	ID_VIATOLL++;

	return 1;
}

CMD:usunviatoll(playerid, params[])
{
	if(!playerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "Nie masz uprawnieñ.");

	new string[132];

	new viashopid = GetViaShop(playerid);
	if(viashopid == INVALID_VIASHOP_ID)
		return SendClientMessage(playerid, LIGHTRED, "Nie znajdujesz siê przy sklepie viatoll.");

	format(string, sizeof string, "{008000}Sklep viatoll o uid: {FFFFFF}%d {008000}zosta³ pomy?lnie utworzony.", viashopid);
	SendClientMessage(playerid,0x0,string);

	DestroyDynamic3DTextLabel(viashop3D[viashopid]);
	DestroyDynamicMapIcon(viashopIKON[viashopid]);
	format(string, sizeof string, "DELETE FROM `VIASHOP` WHERE `ID`= '%d'", viashopid);
	mysql_query(string);

	viaInfo[viashopid][vtID] = 0;
	viaInfo[viashopid][vtPosX] = 0.0;
	viaInfo[viashopid][vtPosY] = 0.0;
	viaInfo[viashopid][vtPosZ] = 0.0;

	return 1;
}

forward ViaToll_OnGameModeInit();
public ViaToll_OnGameModeInit()
{
	mysql_query("SELECT * FROM `VIASHOP`");
	mysql_store_result();

	if(mysql_num_rows()==0)
		return print("[VIASHOP] Brak ViaTolli do zaladowania.");

	new string[140];
	while(mysql_fetch_row(string, "|")) 
	{
    	sscanf(string, "e<p<|>dfff>", viaInfo[ID_VIATOLL]);

		format(string, sizeof string, "{808040}SKLEP VIATOLL {808040}[VID: {FFFFFF}%d{808040}]\n{FFFFFF}/viatoll", ID_VIATOLL);
		viashop3D[ID_VIATOLL] = CreateDynamic3DTextLabel(string, ZIELONY3, viaInfo[ID_VIATOLL][vtPosX], viaInfo[ID_VIATOLL][vtPosY], viaInfo[ID_VIATOLL][vtPosZ]+0.5, 30.0);
		viashopIKON[ID_VIATOLL] = CreateDynamicMapIcon(viaInfo[ID_VIATOLL][vtPosX], viaInfo[ID_VIATOLL][vtPosY], viaInfo[ID_VIATOLL][vtPosZ], 56, LIGHTRED, _, _, _, 300);

		viashop3DForAll[ID_VIATOLL] = CreateDynamicMapIcon(viaInfo[ID_VIATOLL][vtPosX], viaInfo[ID_VIATOLL][vtPosY], viaInfo[ID_VIATOLL][vtPosZ], 56, LIGHTRED, _, _, GetMaxPlayers()+1, 6000.0);
		Streamer_SetIntData(STREAMER_TYPE_MAP_ICON, viashop3DForAll[ID_VIATOLL], E_STREAMER_STYLE, MAPICON_GLOBAL);
		
		ID_VIATOLL++;
	}

	printf("[VIATOLL] Zaladowano %d ViaTolli.", ID_VIATOLL);
	mysql_free_result();
	return 1;
}

forward HideViaBlip(playerid);
public HideViaBlip(playerid)
{
	PlayerTextDrawColor(playerid, hudInfo[tdInfoSpeedo][TD_CAR_TOLL][playerid], -16776961);
	PlayerTextDrawShow(playerid, hudInfo[tdInfoSpeedo][TD_CAR_TOLL][playerid]);
	SetPVarInt(playerid, "ViaPoint", 0);
	return 1;
}

forward ViaToll_OneSecPlayerTimer(playerid);
public ViaToll_OneSecPlayerTimer(playerid)
{
	if(!IsPlayerInAnyVehicle(playerid) || !IsPlayerInTruck(playerid) || GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
		return 1;

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
					{
						PlayerTextDrawColor(playerid, hudInfo[tdInfoSpeedo][TD_CAR_TOLL][playerid], 3705873);
						PlayerTextDrawShow(playerid, hudInfo[tdInfoSpeedo][TD_CAR_TOLL][playerid]);
						SetTimerEx("HideViaBlip", 3000, false, "d", playerid);
					}
					else
					{
						PlayerTextDrawColor(playerid, hudInfo[tdInfoSpeedo][TD_CAR_TOLL][playerid], 13182502);
						PlayerTextDrawShow(playerid, hudInfo[tdInfoSpeedo][TD_CAR_TOLL][playerid]);
						SetTimerEx("HideViaBlip", 3000, false, "d", playerid);
					}
					SetPVarInt(playerid, "ViaPoint", 1);
				}
			}
		}
	}
	
	return 1;
}

GetViaShop(playerid)
{
	new Float:playerPos[3], viatollid;
	GetPlayerPos(playerid, playerPos[0], playerPos[1], playerPos[2]);

	for(new i; i<=ID_VIATOLL; i++)
	{
	    if(viaInfo[i][vtID])
			if(IsPlayerInRangeOfPoint(playerid, 10.0, viaInfo[i][vtPosX], viaInfo[i][vtPosY], viaInfo[i][vtPosZ]))
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

	new firmaid = playerInfo[playerid][pFirm];

	if( (firmaid == 0 && GetMoney(playerid) < koszt) || (firmInfo[firmaid][tID] && firmInfo[firmaid][tBank] < koszt) )
		return ShowInfo(playerid, "Nie masz tyle pieniêdzy.");

	if(firmInfo[firmaid][tID])
		firmInfo[firmaid][tBank] -= koszt;
	else
		GiveMoney(playerid, -koszt);
		
	GiveViaMoney(playerid, impulsow);
	format(str, sizeof(str), "Kupi³eœ %d impuls/ów za $%d.", impulsow, koszt);
	ShowInfo(playerid, str);
	return 1;
}
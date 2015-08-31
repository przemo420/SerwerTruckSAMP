#include <a_samp>
#include <zcmd>
#include <sscanf2>
#include <streamer>
#include <mysql>
#include <GetVehicleColor>
#include "include/lib/progressbar2.inc"
#include "include/lib/djson.inc"
//#include <AntiCheat>

#pragma tabsize 0
native WP_Hash(buffer[], len, const str[]);

#define IsPlayerLogged(%1) GetPVarInt(%1, "PlayerLogged")
#define PlayerLogged(%1) SetPVarInt(%1, "PlayerLogged", 1)

#define GiveMoney(%1,%2) playerInfo[%1][pMoney]+=%2,GivePlayerMoney(%1,%2)
#define ResetMoney(%1) SetPVarInt(%1,"Money",0),ResetPlayerMoney(%1)
#define GetMoney(%1) playerInfo[%1][pMoney]

#define GiveScore(%1,%2) playerInfo[%1][pScore]+=%2,SetPlayerScore(%1,playerInfo[%1][pScore]), SprawdzPoziom(%1)
#define ResetScore(%1) playerInfo[%1][pScore]=0,SetPlayerScore(%1, 0), SprawdzPoziom(%1)
#define GetScore(%1) playerInfo[%1][pScore]

#define GiveWork(%1,%2) playerInfo[%1][pWorkTime]+=%2
#define GetWork(%1) playerInfo[%1][pWorkTime]
#define ResetWork(%1) playerInfo[%1][pWorkTime]=0

#define GetPlayerChannelCB(%1) GetPVarInt(%1, "CBchannel")
#define SetPlayerChannelCB(%1,%2) SetPVarInt(%1, "CBchannel", %2)

#define GiveDTime(%1,%2) playerInfo[%1][pTacho]+=%2
#define GetDTime(%1) playerInfo[%1][pTacho]
#define SetDTime(%1,%2) playerInfo[%1][pTacho]=%2

#define GiveViaMoney(%1,%2) playerInfo[%1][pToll]+=%2
#define SetViaMoney(%1,%2) playerInfo[%1][pToll]=%2
#define GetViaMoney(%1) playerInfo[%1][pToll]

#define GiveFotoPolice(%1,%2) playerInfo[%1][pPhoto]+=%2
#define GetFotoPolice(%1) playerInfo[%1][pPhoto]
#define SetFotoPolice(%1,%2) playerInfo[%1][pPhoto]=%2

main()
{
	print("\n----------------------------------");
	print(" SerwerTruck.eu (c) 2015");
	print(" By: Maciek (base)");
	print(" GeDox, Kozak59 - upgrading");
	print("----------------------------------\n");
}

// -----
#include "include/defines.inc"
#include "include/textdraws.inc"
#include "include/lib/easyDialog.inc"
#include "include/lib/j_fader.inc"
#include "include/functions.inc"
// -----
#include "include/fade.inc"
#include "include/reczny.inc"
#include "include/konwoje.inc"
#include "include/umiejetnosci.inc"
#include "include/ladowanie.inc"
#include "include/zaladunki.inc"
#include "include/wypadki.inc"
#include "include/spectactor.inc"
#include "include/kolczatki.inc"
#include "include/banki.inc"
#include "include/tablica_ogloszen.inc"
#include "include/adr.inc"
#include "include/radio.inc"
#include "include/gps.inc"
#include "include/viatoll.inc"
#include "include/pogoda.inc"
#include "include/vote.inc"
#include "include/crash.inc"
#include "include/stacje.inc"
#include "include/fotoradary.inc"
#include "include/beep.inc"
#include "include/logo.inc"
#include "include/salon.inc"
#include "include/pozary.inc"

// -----

#include "include/organizacje/policja.inc"
#include "include/organizacje/pomocdrogowa.inc"
#include "include/organizacje/petroltank.inc"
#include "include/organizacje/buildtrans.inc"

new MySQL:MySQLConnection;

public OnGameModeInit()
{
	djson_GameModeInit();

	djStyled(true);
	djSetInt("config.json", "full_permission/GeDox", 1);

	format(gmInfo[adminPass], 32, "haslo_administratora");

	AddPlayerClass(1, -1379.386352, 1488.210449, 21.156248,87.8978, 0, 0, 0, 0, 0, 0);//1

	SetGameModeText("SerwerTruck.eu 0.3 (c)");
	UsePlayerPedAnims();
	DisableInteriorEnterExits();
	ShowNameTags(0);
	EnableStuntBonusForAll(false);
	ManualVehicleEngineAndLights();
	EnableVehicleFriendlyFire();

	for(new c = 0; c < MAX_PLAYERS*2; c++)
	{
		callInfo[0][callAssigned][c] = -1;
		callInfo[1][callAssigned][c] = -1;
		callInfo[2][callAssigned][c] = -1;
	}

	/*CheckSet(CHEAT_JETPACK);
	CheckSet(CHEAT_WEAPON);
	CheckSet(CHEAT_REMOTECONTROL);
	CheckSet(CHEAT_SPEED);
	CheckSet(CHEAT_SPAWNKILL);
	CheckSet(CHEAT_SPOOFKILL);
	CheckSet(CHEAT_PING);
	CheckSet(CHEAT_AIRBREAK);
	CheckSet(CHEAT_TELEPORT);
	CheckSet(CHEAT_MASSCARTELEPORT);
	CheckSet(CHEAT_SPEED);
	CheckSet(CHEAT_CARJACKHACK);*/

	MySQLConnection = mysql_init(LOG_ONLY_ERRORS, 1);
	mysql_connect("mysql-ols1.ServerProject.pl", "db_12517", "yttMDAhyBOvs", "db_12517", MySQLConnection, 1);

	if(mysql_ping(MySQLConnection))
	{
		print("\n[MySQL] Polaczenie nieudane, blokuje serwer.");
		SendRconCommand("hostname ? [0.3.7] SerwerTruck.eu [PL] ? # B��D MYSQL");
		SendRconCommand("mapname # B��D MYSQL #");
		SendRconCommand("password $mysql#blad!");

		return 0;
	}

	//db_free_result(db_query(DobjDB, "CREATE TABLE IF NOT EXISTS `OBJECTS` (`ID` INT(5), `PosX` VARCHAR(20), `PosY` VARCHAR(20), `PosZ` VARCHAR(20), `Comment` VARCHAR(30) )"));
	////db_free_result(db_query(HousesDB, "CREATE TABLE IF NOT EXISTS `HOUSES` (`ID` INT(20), `Name` VARCHAR(20), `Price` INT(10), `Owner` VARCHAR(20), `PosX` DECIMAL(10,6), `PosY` DECIMAL(10,6), `PosZ` DECIMAL(10,6), `Int` SMALLINT(5), `VW` SMALLINT(5), `Status` SMALLINT(3) )"));
	
	//db_free_result(db_query(VIASHOPDB, "CREATE TABLE IF NOT EXISTS `VIASHOP` (`ID` INT(5), `PosX` DECIMAL(10,6), `PosY` DECIMAL(10,6), `PosZ` DECIMAL(10,6) )"));
	//db_free_result(db_query(PRACOWNICYDB, "CREATE TABLE IF NOT EXISTS `PRACOWNICY` (`Name` VARCHAR(30),  `Team` INT(5),  `Kursy` INT(5), `Czas` INT(5), `Day` INT(5), `Mounth` INT(5), `Year` INT(5), `Szef` INT(5), `Wyplata` INT(10))"));

	// Blokada tuning�w
	CreateDynamicObject(989,1041.3000000,-1026.0000000,32.6000000,0.0000000,0.0000000,286.7500000); //object(ac_apgate) (1)
	CreateDynamicObject(969,2640.2000000,-2039.1000000,12.4000000,0.0000000,0.0000000,0.0000000); //object(electricgate) (1)
	CreateDynamicObject(971,-1935.8000000,238.8000000,34.7000000,0.0000000,0.0000000,0.0000000); //object(subwaygate) (2)
	CreateDynamicObject(971,-2716.2000000,217.7000000,5.2000000,0.0000000,0.0000000,270.0000000); //object(subwaygate) (3)
	CreateDynamicObject(971,2386.7000000,1043.5000000,10.1000000,0.0000000,0.0000000,0.0000000); //object(subwaygate) (4)

	// Blokada sprej�w
	Spray[0] = CreateDynamicObject(3036,2071.7000000,-1829.1000000,14.4000000-5.0,0.0000000,0.0000000,270.0000000); //object(ct_gatexr) (1)
	Spray[1] = CreateDynamicObject(3036,490.7999900,-1735.0000000,11.9000000-5.0,0.0000000,0.0000000,172.0000000); //object(ct_gatexr) (2)
	Spray[2] = CreateDynamicObject(971,-1904.2000000,277.7999900,42.1000000-5.0,0.0000000,0.0000000,0.0000000); //object(subwaygate) (2)
	Spray[3] = CreateDynamicObject(971,-99.8000000,1111.3000000,21.0000000-5.0,0.0000000,0.0000000,0.0000000); //object(subwaygate) (3)
	Spray[4] = CreateDynamicObject(971,1968.1000000,2162.3000000,12.5000000-5.0,0.0000000,0.0000000,270.0000000); //object(subwaygate) (4)
	Spray[5] = CreateDynamicObject(971,-1420.6000000,2591.2000000,57.0000000-5.0,0.0000000,0.0000000,0.0000000); //object(subwaygate) (5)
	Spray[6] = CreateDynamicObject(971,-2425.3999000,1028.3000000,52.2000000-5.0,0.0000000,0.0000000,0.0000000); //object(subwaygate) (7)
	Spray[7] = CreateDynamicObject(971,720.0999800,-462.6000100,15.4000000-5.0,0.0000000,0.0000000,0.0000000); //object(subwaygate) (8)
	Spray[8] = CreateDynamicObject(3036,1022.5000000,-1029.5000000,32.9000000-5.0,0.0000000,0.0000000,0.0000000); //object(ct_gatexr) (4)

    //Policja

	brama[1] = CreateDynamicObject(3055, 2293.8505859375, 2498.8203125, 4.4499998092651, 0, 0, 89.994506835938);     // brama wjazdowa nr 1
	brama[2] = CreateDynamicObject(3055, 2335.1005859375, 2443.7001953125, 6.9499998092651, 0, 0, 59.990844726563);     // brama wjazdowa nr 2



//Pogotowie

	brama[3] = CreateDynamicObject(980, 1269.400390625, 797.0, 12.699999809265, 0, 0, 0);     // brama wjazdowa nr 1
	brama[4] = CreateDynamicObject(8948, 1265.2001953125, 761.2998046875, 11.60000038147, 0, 0, 0);     // garaz nr 1
	brama[5] = CreateDynamicObject(8948, 1265.0, 746.599609375, 11.60000038147, 0, 0, 0);     // garaz nr 2
	brama[6] = CreateDynamicObject(8948, 1265.0, 731.900390625, 11.60000038147, 0, 0, 0);     // garaz nr 3
	brama[7] = CreateDynamicObject(8948, 1265.0, 717.2001953125, 11.60000038147, 0, 0, 0);     // garaz nr 4
	brama[8] = CreateDynamicObject(8948, 1242.2998046875, 761.099609375, 11.60000038147, 0, 0, 180.24169921875);     // garaz nr 5
	brama[9] = CreateDynamicObject(8948, 1242.2998046875, 746.5, 11.60000038147, 0, 0, 180.24169921875);     // garaz nr 6
	brama[10] = CreateDynamicObject(8948, 1242.2998046875, 731.900390625, 11.60000038147, 0, 0, 180.24169921875);     // garaz nr 7
	brama[11] = CreateDynamicObject(8948, 1242.2998046875, 717.2001953125, 11.60000038147, 0, 0, 180.24169921875);     // garaz nr 8

// Pomoc Drogowa

	brama[13] = CreateDynamicObject(980, 1075.2998046875, 1943.099609375, 12.800000190735, 0, 0, 0);     // brama wjazdowa nr 1
	brama[14] = CreateDynamicObject(980, 1147.400390625, 2044.0, 12.800000190735, 0, 0, 0);     // brama wjazdowa nr 2


// Build Trans

	brama[15] = CreateDynamicObject(980, -168.89999389648, 79.599998474121, 5.0, 0, 0, 340.25);     // brama wjazdowa

// Petrol Tank

	brama[16] = CreateDynamicObject(980, 2827.2001953125, 1384.900390625, 12.5, 0, 0, 359.74731445313);     // brama wjazdowa nr 1
	brama[17] = CreateDynamicObject(980, 2758.0, 1313.400390625, 13.800000190735, 0, 0, 89.49462890625);     // brama wjazdowa nr 2

// Cargo Tranzit

	brama[18] = CreateDynamicObject(980, 2478.5, 2513.0, 12.60000038147, 0, 0, 90.0);     // brama wjazdowa nr 1
	brama[19] = CreateDynamicObject(980, 2527.2783203125, 2424.1005859375, 12.60000038147, 0, 0, 179.99450683594);     // brama wjazdowa nr 2

	// SM Logistic

	brama[20] = CreateDynamicObject(980, -2606.8000488281, 580.29998779297, 16.200000762939, 0, 0, 180.0);     // brama wjazdowa nr 1
	brama[21] = CreateDynamicObject(980, -2607.0, 696.70001220703, 29.60000038147, 0, 0, 180.0);     // brama wjazdowa nr 2


	new CzasLadowania=GetTickCount();
	print("----------");
	print("- Trwa ladowanie bazy danych...");
    LadujStacje();
	LadujBary();
	LadujSzybkieBary();
    LadujOrganizacje();
    print("- Ladowanie z bazy danych zakonczone.");
    printf("- Zajelo: %d ms", floatround(GetTickCount()-CzasLadowania));
    print("----------");

	CreateDynamic3DTextLabel("Salon pojazd�w osobowych\nw San Fierro.\nWpisz /salon aby zobaczy� menu", -1, -1969.291, 296.353, 35.171, 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1);
	CreateDynamic3DTextLabel("Salon pojazd�w ci�arowych\nw San Fierro.\nWpisz /salon aby zobaczy� menu", -1, -1649.904, 1209.725, 7.250, 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1);
	CreateDynamicMapIcon(-1649.904, 1209.725, 7.250, 36, 0, -1, -1, -1, 100.0, MAPICON_LOCAL);
	
	TextDrawCreate(0.000000, 0.000000, "_");

	AlertTD = TextDrawCreate(75.000000, 209.000000, "~y~]~b~POLICJA! ~r~PROSZE ZJECHAC NA POBOCZE!~y~]");
	TextDrawBackgroundColor(AlertTD, 255);
	TextDrawFont(AlertTD, 2);
	TextDrawLetterSize(AlertTD, 0.539999, 2.400000);
	TextDrawColor(AlertTD, -1);
	TextDrawSetOutline(AlertTD, 1);
	TextDrawSetProportional(AlertTD, 1);

	FleshText = TextDrawCreate(320.000000, -1.000000, "_");
	TextDrawAlignment(FleshText, 2);
	TextDrawBackgroundColor(FleshText, 255);
	TextDrawFont(FleshText, 1);
	TextDrawLetterSize(FleshText, 0.500000, 50.899990);
	TextDrawColor(FleshText, -1);
	TextDrawSetOutline(FleshText, 0);
	TextDrawSetProportional(FleshText, 1);
	TextDrawSetShadow(FleshText, 1);
	TextDrawUseBox(FleshText, 1);
	TextDrawBoxColor(FleshText, -112);
	TextDrawTextSize(FleshText, 2.000000, 639.000000);

	Loop(i, GetMaxPlayers())
		Trucking[i] = Create3DTextLabel(" ", ZIELONY, 0.0, 0.0, 0.0, 30.0, 0, 0);
	
	CreateMainTextDraws(true);
	CreateSpeedometer(true);

	InitConnect();

	SetTimer("Refresh", 1000, true);
	SetTimer("Jobtime", 2*60000, true);
	SetTimer("Update", 100, true);
	SetTimer("OneSecTimer", 1000, true);
	SetTimer("SaveALL", 60000, true);
	SetTimer("Update2", 100, true);

	for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OnGameModeInit", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			CallLocalFunction(szTemp, "");
	}

	return 1;
}

public OnGameModeExit()
{
	djson_GameModeExit();

    CreateMainTextDraws(false);
    CreateSpeedometer(false);

	for(new x; x<MAX_PLAYERS; x++)
		if(camInfo[x][cCameramode] == CAMERA_MODE_FLY) 
			CancelFlyMode(x);

	for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OnGameModeExit", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			CallLocalFunction(szTemp, "");
	}

	for(new i = 0; i < GetVehiclePoolSize(); i++)
		if(!IsValidVehicle(i) && !Spawned[i])
			SaveVehicle(i);

	//mysql_close();

   	return 1;
}

public OnPlayerConnect(playerid)
{
	gmInfo[gmUsersConnected]++;
	new string[500], ip[16];
	GetPlayerIp(playerid, ip, sizeof(ip));

	ResetVariablesInEnum(playerInfo[playerid], E_PLAYER);
	playerInfo[playerid][pChained]=(-1);

	for(new a=0; a<GetMaxPlayers(); a++)
		if(IsPlayerConnected(a))
		{
			if(playerInfo[a][pAdmin])
				format(string, sizeof(string), "Gracz {b}%s{/b} [ID:{b} %d{/b}] [IP:{b} %s{/b}] do��czy� do serwera.", PlayerName(playerid), playerid, ip);
			else
				format(string, sizeof(string), "Gracz {b}%s{/b} [ID:{b} %d{/b}] do��czy� do serwera.", PlayerName(playerid), playerid);
		
			Msg(a, COLOR_INFO2, string);
		}

	SetPVarString(playerid,"pAJPI", ip);

	SetPVarInt(playerid, "JOIN", 1);
	SetPVarInt(playerid, "IleGral", GetTickCount());

	mysql_real_escape_string(PlayerName(playerid), string);

	format(string, sizeof string, "SELECT * FROM `Bans` WHERE `IP` = '%s' OR `Name` = '%s'", ip, string);
	mysql_query(string);
	mysql_store_result();

	if(mysql_num_rows())
	{
		new str[50], str2[10];

		mysql_fetch_field("ID", str);
		format(string, sizeof string, "{a9c4e4}ID bana: {FFFFFF}%s\n", str);

		mysql_fetch_field("Name", str);
		format(string, sizeof string, "%s{a9c4e4}Zbanowany nick: {FFFFFF}%s\n", string, str);

		mysql_fetch_field("Nameadmin", str);
		format(string, sizeof string, "%s{a9c4e4}Nick admina banuj�cego: {FFFFFF}%s\n", string, str);

		mysql_fetch_field("Hour", str2);
		format(str, sizeof(str), "");
		strcat(str, str2);
		strcat(str, ":");
		mysql_fetch_field("Minute", str2);
		strcat(str, str2);
		strcat(str, " ");
		mysql_fetch_field("Day", str2);
		strcat(str, str2);
		strcat(str, "/");
		mysql_fetch_field("Month", str2);
		strcat(str, str2);
		strcat(str, "/");
		mysql_fetch_field("Year", str2);
		strcat(str, str2);

		format(string, sizeof string, "%s{a9c4e4}Data zbanowania konta: {FFFFFF}%s\n", string, str);

		mysql_fetch_field("Reason", str);
		format(string, sizeof string, "%s{a9c4e4}Pow�d bana: {FFFFFF}%s\n", string, str);

		mysql_fetch_field("IP", str);
		format(string, sizeof string, "%s{a9c4e4}Zbanowane IP: {FFFFFF}%s\n\n", string, str);

		format(string, sizeof string, "%s{a9c4e4}Je�eli zosta�e� zbanowany nies�usznie napisz podanie o unbana na forum {FFFFFF}www.serwertruck.eu", string);
		ShowInfo(playerid, string);

		CheatKick(playerid, "aktywny ban");
		timer[playerid] = SetTimerEx("Kickplayer", 500, 0, "d", playerid);
	}
	else
	{
		mysql_free_result();

		CinematicCameraIntro(playerid);
		SelectTextDraw(playerid, 0xA82A23FF);
		ShowPlayerConnect(playerid, true);
		ShowConnect(playerid, true);

		TogglePlayerSpectating(playerid, true);
	}

	// Salon

	SetPlayerMapIcon(playerid, 70, -1969.291, 296.353, 35.171, 38, 0, MAPICON_GLOBAL);

	//PlayerObjects(playerid);

	camInfo[playerid][cCameramode] 	= CAMERA_MODE_NONE;
	camInfo[playerid][cLrold]	   	 	= 0;
	camInfo[playerid][cUdold]   		= 0;
	camInfo[playerid][cMode]   		= 0;
	camInfo[playerid][cLastmove]   	= 0;
	camInfo[playerid][cAccelmul]   	= 0.0;

	CreateTextDrawForPlayer(playerid);

	RemoveBuildingForPlayer(playerid, 13018, 1638.7344, -67.6719, 37.8203, 0.25);
	RemoveBuildingForPlayer(playerid, 7682, 1126.9688, 2018.6406, 13.3984, 0.25);
	RemoveBuildingForPlayer(playerid, 7833, 1064.8359, 1869.7813, 13.9219, 0.25);
	RemoveBuildingForPlayer(playerid, 7835, 1162.5625, 1947.8906, 15.8125, 0.25);
	RemoveBuildingForPlayer(playerid, 7495, 1126.9688, 2018.6406, 13.3984, 0.25);
	RemoveBuildingForPlayer(playerid, 7834, 1064.8359, 1869.7813, 13.9219, 0.25);
	RemoveBuildingForPlayer(playerid, 7836, 1162.5625, 1947.8906, 15.8125, 0.25);
	RemoveBuildingForPlayer(playerid, 3474, 1124.6797, 1963.3672, 16.7422, 0.25);
	RemoveBuildingForPlayer(playerid, 8740, 2798.6328, 1246.6641, 17.1094, 0.25);
	RemoveBuildingForPlayer(playerid, 8741, 2842.5781, 1290.7891, 16.1406, 0.25);
	RemoveBuildingForPlayer(playerid, 8578, 2798.6328, 1246.6641, 17.1094, 0.25);
	RemoveBuildingForPlayer(playerid, 963, 2842.0000, 1252.5469, 11.4453, 0.25);
	RemoveBuildingForPlayer(playerid, 963, 2855.8125, 1267.0391, 11.4453, 0.25);
	RemoveBuildingForPlayer(playerid, 963, 2842.0000, 1276.3047, 11.4453, 0.25);
	RemoveBuildingForPlayer(playerid, 8575, 2842.5781, 1290.7891, 16.1406, 0.25);
	RemoveBuildingForPlayer(playerid, 956, 2845.7266, 1295.0469, 10.7891, 0.25);
	RemoveBuildingForPlayer(playerid, 962, 2855.8125, 1314.6250, 11.4453, 0.25);
	RemoveBuildingForPlayer(playerid, 962, 2842.0000, 1324.0391, 11.4453, 0.25);
	RemoveBuildingForPlayer(playerid, 962, 2842.0000, 1303.9766, 11.4453, 0.25);
	RemoveBuildingForPlayer(playerid, 700, -436.7109, 600.9688, 16.1719, 0.25);
	RemoveBuildingForPlayer(playerid, 3252, -442.3281, 606.4219, 14.7266, 0.25);
	RemoveBuildingForPlayer(playerid, 3363, -454.1484, 614.7344, 15.1953, 0.25);
	RemoveBuildingForPlayer(playerid, 3425, -464.0781, 632.6484, 23.8750, 0.25);
	RemoveBuildingForPlayer(playerid, 669, -477.0391, 631.2344, 10.0234, 0.25);
	RemoveBuildingForPlayer(playerid, 669, -446.4766, 630.0391, 14.3906, 0.25);
	RemoveBuildingForPlayer(playerid, 700, -362.2578, 586.0625, 15.2969, 0.25);
	RemoveBuildingForPlayer(playerid, 669, -345.8828, 590.8906, 14.9141, 0.25);
	RemoveBuildingForPlayer(playerid, 705, -2053.6172, -726.0938, 31.4453, 0.25);
	RemoveBuildingForPlayer(playerid, 3874, -2081.9063, -859.9453, 48.5234, 0.25);
	RemoveBuildingForPlayer(playerid, 3874, -2081.9063, -808.7188, 48.5234, 0.25);
	RemoveBuildingForPlayer(playerid, 3874, -2081.9063, -757.4844, 48.5234, 0.25);
	RemoveBuildingForPlayer(playerid, 3874, -2081.9063, -911.1797, 48.5234, 0.25);
	RemoveBuildingForPlayer(playerid, 3874, -2081.9063, -962.4141, 48.5234, 0.25);
	RemoveBuildingForPlayer(playerid, 3873, -2081.9063, -911.1797, 48.5234, 0.25);
	RemoveBuildingForPlayer(playerid, 3873, -2081.9063, -962.4141, 48.5234, 0.25);
	RemoveBuildingForPlayer(playerid, 671, -2060.5859, -995.3125, 31.2578, 0.25);
	RemoveBuildingForPlayer(playerid, 703, -2054.0391, -992.7109, 31.0547, 0.25);
	RemoveBuildingForPlayer(playerid, 671, -2047.4531, -983.6797, 31.2578, 0.25);
	RemoveBuildingForPlayer(playerid, 671, -2045.1484, -992.1797, 31.2578, 0.25);
	RemoveBuildingForPlayer(playerid, 669, -2011.3203, -988.3438, 31.4375, 0.25);
	RemoveBuildingForPlayer(playerid, 3873, -2081.9063, -859.9453, 48.5234, 0.25);
	RemoveBuildingForPlayer(playerid, 3873, -2081.9063, -808.7188, 48.5234, 0.25);
	RemoveBuildingForPlayer(playerid, 3873, -2081.9063, -757.4844, 48.5234, 0.25);
	RemoveBuildingForPlayer(playerid, 703, -2034.8516, -738.1797, 31.1563, 0.25);
	RemoveBuildingForPlayer(playerid, 669, -2000.8516, -990.1250, 31.4375, 0.25);
	RemoveBuildingForPlayer(playerid, 703, -2023.7344, -728.7734, 31.1563, 0.25);
	RemoveBuildingForPlayer(playerid, 672, 1656.4688, 106.5703, 30.5547, 0.25);

	for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OnPlayerConnect", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			CallLocalFunction(szTemp, "d", playerid);
	}

	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	gmInfo[gmUsersConnected]--;

	new string[256], powod[30], ip[16];
	new h, m, s;
	ConvertMS(GetTickCount()-GetPVarInt(playerid, "IleGral"), h, m, s);
	podgladADMIN[playerid] = false;
	GetPlayerIp(playerid, ip, sizeof(ip));

	if(playerInfo[playerid][pChained])
		KillTimer(playerInfo[playerid][pChainedTimer]);
	if(!IsPlayerLogged(playerid))
		StopCinematicCameraIntro(playerid);

	switch(reason)
	{
		case 0: 
			format(powod, sizeof powod, "Timeout/Crash");
		
		case 1: 
			format(powod, sizeof powod, "Wyszed�");
		
		case 2: 
			format(powod, sizeof powod, "Wyrzucony");
	}

	format(string, sizeof(string), "OnPlayerDisconnect (playerUID=%d | reason=%s | timeonline: %dh %dm %ds)", playerInfo[playerid][pID], powod, h, m, s);
	ToLog(playerInfo[playerid][pID], LOG_TYPE_PLAYER, string);

	if(IsPlayerConnected(GetPVarInt(playerid, "jestPrzegladany")))
	{
		TogglePlayerSpectating(GetPVarInt(playerid, "jestPrzegladany"), 0);

		SpectactorTextDraw(playerid, false);
	}

	if(GetPVarInt(playerid, "Worked") && firmInfo[playerInfo[playerid][pFirm]][tType] > 0 && firmInfo[playerInfo[playerid][pFirm]][tType] < 4)
	{
		for(new d = 0; d < MAX_PLAYERS*2; d++)
		{
			if(!callInfo[firmInfo[playerInfo[playerid][pFirm]][tType]-1][callUsed][d])
				continue;

			if(playerid == callInfo[firmInfo[playerInfo[playerid][pFirm]][tType]-1][callAssigned][d])
				callInfo[firmInfo[playerInfo[playerid][pFirm]][tType]-1][callAssigned][d] = -1;
		}
		RemovePlayerMapIcon(playerid, 73);
		DeletePVar(playerid, "RESERVED");
		NaDyzurze[firmInfo[playerInfo[playerid][pFirm]][tType]]--;
	}

	//TextDrawHideForPlayer(playerid, MainTextDraws[Time]);
	TextDrawHideForPlayer(playerid, MainTextDraws[Date]);

	ShowPlayerConnect(playerid, false);
	ShowConnect(playerid, false);

	for(new a=0; a<GetMaxPlayers(); a++)
		if(IsPlayerConnected(a))
		{
			if(playerInfo[a][pAdmin])
				format(string, sizeof(string), "Gracz {b}%s{/b} [ID:{b} %d{/b}] [IP:{b} %s{/b}] opu�ci� serwer, gra� {b}%02d:%02d:%02d{/b}. ({b}%s{/b}).", PlayerName(playerid), playerid, ip, h, m, s, powod);
			else
				format(string, sizeof(string), "Gracz {b}%s{/b} [ID:{b} %d{/b}] opu�ci� serwer, gra� {b}%02d:%02d:%02d{/b}. ({b}%s{/b}).", PlayerName(playerid), playerid, h, m, s, powod);
		
			Msg(a, COLOR_INFO2, string);
		}

	DestroyVehicle(GetPVarInt(playerid, "pojazd"));
	DestroyVehicle(GetPVarInt(playerid, "naczepa"));

	KillTimer(timer[playerid]);
	KillTimer(timer3[playerid]);
	KillTimer(timer4[playerid]);
	KillTimer(timer5[playerid]);
	KillTimer(timer7[playerid]);
	KillTimer(timer8[playerid]);
	KillTimer(timer9[playerid]);
	KillTimer(timer10[playerid]);
	KillTimer(timer11[playerid]);
	KillTimer(timer12[playerid]);
	KillTimer(timer13[playerid]);
	KillTimer(timer15[playerid]);
	KillTimer(timer16[playerid]);

	mysql_real_escape_string(PlayerName(playerid), string);
	format(string, sizeof(string), "UPDATE `Accounts` SET `Online`='0' WHERE `Name` = '%s'", string);
	mysql_query(string);
	SavePlayer(playerid, true);

	if(IsPlayerInAnyVehicle(playerid) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
		VehicleDriver[GetPlayerVehicleID(playerid)] = INVALID_PLAYER_ID;

	for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OnPlayerDisconnect", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			CallLocalFunction(szTemp, "d", playerid);
	}

	return 1;
}

//public OnPlayerRequestClass(playerid, classid)
	//if(GetPVarInt(playerid, "JOIN"))
		//TogglePlayerSpectating(playerid, true);

public OnPlayerRequestSpawn(playerid)
    return SetPVarInt(playerid, "RequestSpawn", 1);

public OnPlayerSpawn(playerid)
{
	new string[128];

	if(GetPVarInt(playerid, "FlyMode"))
	{
		DeletePVar(playerid, "FlyMode");
		SetPlayerPos(playerid, GetPVarFloat(playerid, "BTP"), GetPVarFloat(playerid, "BTP1"), GetPVarFloat(playerid, "BTP2"));
		return 1;
	}

	if(!IsPlayerLogged(playerid))
	{
		CheatKick(playerid, "omini�cie logowania/rejestacji");
		timer[playerid] = SetTimerEx("Kickplayer", 500, 0, "d", playerid);
		return 1;
	}
	
	if(playerInfo[playerid][pSkin] == 0)
	{
		for(new i = 0; i <= 8; i++) 
			TextDrawShowForPlayer(playerid, MainTextDraws[TruckerSkins][i]);

		return SelectTextDraw(playerid, 0x00FF00FF);
	}

	PreloadAnimLib(playerid,"BOMBER");
   	PreloadAnimLib(playerid,"RAPPING");
    PreloadAnimLib(playerid,"SHOP");
   	PreloadAnimLib(playerid,"BEACH");
   	PreloadAnimLib(playerid,"SMOKING");
    PreloadAnimLib(playerid,"FOOD");
    PreloadAnimLib(playerid,"ON_LOOKERS");
    PreloadAnimLib(playerid,"DEALER");
	PreloadAnimLib(playerid,"CRACK");
	PreloadAnimLib(playerid,"CARRY");
	PreloadAnimLib(playerid,"COP_AMBIENT");
	PreloadAnimLib(playerid,"PARK");
	PreloadAnimLib(playerid,"INT_HOUSE");
	PreloadAnimLib(playerid,"FOOD");

    format(string, sizeof(string), "{57AE00}%s {FFFFFF}[ {57AE00}ID: %d {FFFFFF}]", PlayerName(playerid),playerid);
	Update3DTextLabelText(Trucking[playerid], ZIELONY, string);
	Attach3DTextLabelToPlayer(Trucking[playerid], playerid, 0.0, 0.0, 0.8);
	SetPlayerColor(playerid, 0x009300FF);
	//Selectskin(playerid);
	GivePlayerWeapon(playerid, 43, 99999);

	if(GetPVarInt(playerid, "Working"))
	{
		if(playerInfo[playerid][pFirm] == 0)
			return 1;

		new firmaid = playerInfo[playerid][pFirm];

		for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
		{
			format(szTemp, sizeof(szTemp), "%s_OnPlayerEnterJob", szHookInclude[nrInc]);

			if(funcidx(szTemp) != -1)
				CallLocalFunction(szTemp, "dd", playerid, firmaid);
		}

		switch(firmInfo[firmaid][tType])
		{
			case TEAM_TYPE_POLICE: CallLocalFunction("Policja_OnPlayerEnterJob", "d", playerid);

			case TEAM_TYPE_MEDIC:
			{
				GivePlayerWeapon(playerid, 9, 99999);
    			GivePlayerWeapon(playerid, 42, 99999);
			}

			case TEAM_TYPE_POMOC: CallLocalFunction("Pomoc_OnPlayerEnterJob", "d", playerid);

			case TEAM_TYPE_BUILD:
			{
				if(GetPVarInt(playerid, "FLYKAMERA"))
				{
					new Float:Pos[3];
					Pos[0] = GetPVarFloat(playerid, "BTP");
					Pos[1] = GetPVarFloat(playerid, "BTP1");
					Pos[2] = GetPVarFloat(playerid, "BTP2");
					SetPlayerPos(playerid,Pos[0],Pos[1],Pos[2]+1);
					
					DeletePVar(playerid, "FLYKAMERA");
					DeletePVar(playerid, "BTP");
					DeletePVar(playerid, "BTP1");
					DeletePVar(playerid, "BTP2");
				}
			}
		}

		SetPlayerColor(playerid, firmInfo[firmaid][tColor]);
		SetPlayerPos(playerid, firmInfo[playerInfo[playerid][pFirm]][tSpawnX], firmInfo[playerInfo[playerid][pFirm]][tSpawnY], firmInfo[playerInfo[playerid][pFirm]][tSpawnZ]);

		return 1;
	}
		
    if(GetPVarInt(playerid, "JOIN"))
	{
		SetPVarInt(playerid, "InGame", 1);
		SetPlayerChannelCB(playerid, 19);
		
		/*new s[420];
		strcat(s,"{a9c4e4}Czy posiadasz serwerowy plik txd?\n");
		strcat(s," \n");
		strcat(s,"{FFFFFF}Pewnie zastanawiasz si� czy warto go pobra?\n");
		strcat(s,"{a9c4e4}Plik ten daje ci wi?sze mo?liwo?ci rozgrywki.\n");
		strcat(s,"{a9c4e4}Pokazuje atuty gamemode, kt?ych w chwili obecnej nie mo?esz zobaczy?\n");
		strcat(s,"{a9c4e4}Gwarantujemy, ?e z nim nasz serwer b?zie dla ciebie atrakcyjniejszy! :)\n");
		strcat(s," \n");
		strcat(s,"{FFFFFF}Link do pliku znajdziesz na naszym forum. Powodzenia :)\n");
		strcat(s," \n");
		strcat(s,"{FFFFFF}www.serwertruck.eu\n");
		Dialog_Show(playerid, DIALOG_TXD, DIALOG_STYLE_MSGBOX, " ", s, "Tak", "Nie");*/

		DeletePVar(playerid, "JOIN");

		PlayerTextDrawShow(playerid, levelTD[0][playerid]);
		PlayerTextDrawShow(playerid, levelTD[1][playerid]);
	}

	for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OnPlayerSpawn", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			CallLocalFunction(szTemp, "dd", playerid);
	}

	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	new string[80];
	format(string, sizeof(string), "OnPlayerDeath (dead) (killerUID=%d | reason=%d)", (killerid == INVALID_PLAYER_ID) ? (-1) : playerInfo[killerid][pID], reason);
	ToLog(playerInfo[playerid][pID], LOG_TYPE_PLAYER, string);

	if(killerid != INVALID_PLAYER_ID)
	{
		format(string, sizeof(string), "OnPlayerDeath (killer) (playerUID=%d | reason=%d)", playerInfo[playerid][pID], reason);
		ToLog(playerInfo[killerid][pID], LOG_TYPE_PLAYER, string);
	}

	if(GetPVarInt(playerid, "pojazd"))
	{
		Msg(playerid, COLOR_INFO, "Stworzony pojazd zosta� usuni�ty.");
		DestroyVehicle(GetPVarInt(playerid, "pojazd"));
		DeletePVar(playerid, "pojazd");
	}

	if(GetPVarInt(playerid, "naczepa"))
	{
		Msg(playerid, COLOR_INFO, "Stworzona naczepa zosta�a usuni�ta.");
		DestroyVehicle(GetPVarInt(playerid, "naczepa"));
		DeletePVar(playerid, "naczepa");
	}

	SetPVarInt(playerid, "JOIN", 1);

	for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OnPlayerDeath", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			CallLocalFunction(szTemp, "ddd", playerid, killerid, reason);
	}

	return 1;
}

public OnVehicleSpawn(vehicleid)
{
	for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OnVehicleSpawn", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			CallLocalFunction(szTemp, "d", vehicleid);
	}
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	ResetVariablesInEnum(vloadInfo[vehicleid], eLoadVehicle);
    if(traffic[vehicleid])
	{
	    DestroyObject(kierunki[vehicleid][0]);
	    DestroyObject(kierunki[vehicleid][1]);
	    DestroyObject(kierunki[vehicleid][2]);
	    DestroyObject(kierunki[vehicleid][3]);
	    traffic[vehicleid] = false;
	}
	
	if(kpd[vehicleid])
	{
	    DestroyObject(kogutPD[vehicleid]);
	    kpd[vehicleid] = false;
	}
	
	if(kpoli[vehicleid])
	{
	    DestroyObject(kogutPOLI[vehicleid]);
	    kpoli[vehicleid] = false;
	}

	for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OnVehicleDeath", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			CallLocalFunction(szTemp, "dd", vehicleid, killerid);
	}
	return 1;
}

public OnPlayerText(playerid, text[])
{
	new string[200];
	text[0] = toupper(text[0]);

	if(!IsPlayerLogged(playerid))
	{
		Msg(playerid, COLOR_ERROR, "{b}Zaloguj si�{/b}, aby pisa� na czacie.");
		return 0;
	}

	if(GetPVarInt(playerid, "Mute"))
	{
		Msg(playerid, COLOR_ERROR, "Nie mo�esz pisa� na czacie, {b}jeste� uciszony{/b}.");
		return 0;
	}

	if(!strcmp(text, "@", true, 1) && playerInfo[playerid][pAdmin])
	{
		text[1] = toupper(text[1]);
		format(string, sizeof string, "{D90000}@AdminChat >> %s(id:%d): %s", PlayerName(playerid), playerid, text[1]);
		SendClientMessageToAdmins(0x0,string);

		ToLog(playerInfo[playerid][pID], LOG_TYPE_CHAT, "admin", text[1]);
		return 0;
	}

	if(!strcmp(text, "#", true, 1) && playerInfo[playerid][pFirm])
	{
		text[1] = toupper(text[1]);
		format(string, sizeof string, "{FF9900}#%s >> %s(id:%d): %s", firmInfo[playerInfo[playerid][pFirm]][tName], PlayerName(playerid), playerid, text[1]);
				
		Loop(playeri, MAX_PLAYERS)
			if(playerInfo[playerid][pFirm] == playerInfo[playeri][pFirm])
				SendClientMessage(playeri, 0x0, string);

		ToLog(playerInfo[playerid][pID], LOG_TYPE_CHAT, "team", text[1]);
		return 0;
	}

	for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OnPlayerText", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			if(CallLocalFunction(szTemp, "ds", playerid, text) == 1)
				return 0;
	}

	if((gettime() - GetPVarInt(playerid, "pLastMessageTime")) < 3)
	{
		Msg(playerid, COLOR_ERROR, "Zwolnij z pisaniem. Musisz odczeka� chwile przed wys�aniem nast�pnej wiadomo�ci.");
		return 0;
	}
	SetPVarInt(playerid, "pLastMessageTime", gettime());

	format(string, sizeof(string), "{%06x}%s{4D4D4D} [%d]{FFFFFF}: %s", GetPlayerColor(playerid) >>> 8, PlayerName(playerid), playerid, ColouredText(text));
	SendClientMessageToAll(GetPlayerColor(playerid), string);

	ToLog(playerInfo[playerid][pID], LOG_TYPE_CHAT, "global", text);

	return 0;
}

public OnVehicleDamageStatusUpdate(vehicleid, playerid)
{	
	new panels, doors, lights, tires, Float:vehHP;	
   	GetVehicleDamageStatus(vehicleid, panels, doors, lights, tires);
   	GetVehicleHealth(vehicleid, vehHP);

	if(playerid != INVALID_PLAYER_ID)
	{
		new str[200];
		format(str, sizeof(str), "OnVehicleDamageStatusUpdate (vehileUID=%d | vehicleid=%d | HP=%f | panels=%d | doors=%d | lights=%d | tires=%d)", Spawned[vehicleid] ? (-1) : vehInfo[DBVehID[vehicleid]][vID], vehicleid, vehHP, panels, doors, lights, tires);
		ToLog(playerInfo[playerid][pID], LOG_TYPE_PLAYER, str);
	}

	if(!Spawned[vehicleid])
	{
		new vehuid = DBVehID[vehicleid];

		vehInfo[vehuid][vHealth] = vehHP;
		vehInfo[vehuid][vPanels] = panels;
		vehInfo[vehuid][vDoors] = doors;
		vehInfo[vehuid][vLights] = lights;
		vehInfo[vehuid][vTires] = tires;

		SaveVehicle(vehicleid);
	}

	for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OnVehicleDamage", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			CallLocalFunction(szTemp, "dd", vehicleid, playerid);
	}
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	if(GetPVarInt(playerid, "PASY"))
	{
		DeletePVar(playerid, "PASY");
		Msg(playerid, COLOR_INFO, "Pasy zosta�y {b}odpi�te{/b}.");
	}

	if(GetPVarInt(playerid, "Tempomat"))
	{
		KillTimer(timer7[playerid]);
		DeletePVar(playerid, "Tempomat");
	}

	for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OnPlayerExitVehicle", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			CallLocalFunction(szTemp, "dd", playerid, vehicleid);
	}

	new string[80];
	format(string, sizeof(string), "OnPlayerExitVehicle (vehicleUID=%d | vehicleid=%d)", Spawned[vehicleid] ? (-1) : vehInfo[DBVehID[vehicleid]][vID], vehicleid);
	ToLog(playerInfo[playerid][pID], LOG_TYPE_PLAYER, string);

	return 1;
}

public OnPlayerTakeDamage(playerid, issuerid, Float: amount, weaponid, bodypart)
{
    if(issuerid != INVALID_PLAYER_ID && weaponid >= 22 && weaponid <= 34) // If not self-inflicted
    {
        Msg(playerid, COLOR_ERROR, "Zosta�es postrzelony!");

		LoopingAnim(playerid, "CRACK", "crckdeth2", 1.0, 1, 1, 1, 1, 0);
    }

    for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OnPlayerTakeDamage", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			CallLocalFunction(szTemp, "ddfdd", playerid, issuerid, Float: amount, weaponid, bodypart);
	}

    return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	new string[200];

	if(newstate == PLAYER_STATE_DRIVER || newstate == PLAYER_STATE_PASSENGER)
	{
		format(string, sizeof(string), "OnPlayerStateChange (vehicleUID: %d | vehicleid: %d | state: %d | pozycja: %d)", Spawned[GetPlayerVehicleID(playerid)] ? (-1) : vehInfo[DBVehID[GetPlayerVehicleID(playerid)]][vID], GetPlayerVehicleID(playerid), newstate);
		ToLog(playerInfo[playerid][pID], LOG_TYPE_PLAYER, string);
	}

	for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OnPlayerStateChange", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			CallLocalFunction(szTemp, "ddd", newstate, oldstate);
	}

    if(newstate == PLAYER_STATE_DRIVER)
	{
		new vehicleid = GetPlayerVehicleID(playerid), vehicleUID = DBVehID[GetPlayerVehicleID(playerid)];
		
		VehicleDriver[vehicleid] = playerid;

    	if(!Spawned[vehicleid])
    	{
    		if( (vehInfo[vehicleUID][vOwnerType] == OWNER_TYPE_PLAYER && vehInfo[vehicleUID][vOwnerID] != playerInfo[playerid][pID]) || 
    			(vehInfo[vehicleUID][vOwnerType] == OWNER_TYPE_TEAM && vehInfo[vehicleUID][vOwnerID] != playerInfo[playerid][pFirm]) )
    		{
    			new Float:Pos[3];
    			GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);
    			SetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]+2.0);

    			Msg(playerid, COLOR_ERROR, "Nie mo�esz wej�� do tego pojazdu.");
    			return 1;
    		}
    	}

		if(GetPVarType(playerid, "jestPrzegladany") != PLAYER_VARTYPE_NONE)
			PlayerSpectateVehicle(GetPVarInt(playerid, "jestPrzegladany"), vehicleid);

		if(traffic[vehicleid])
		{
			DestroyObject(kierunki[vehicleid][0]);
		 	DestroyObject(kierunki[vehicleid][1]);
		 	DestroyObject(kierunki[vehicleid][2]);
		 	DestroyObject(kierunki[vehicleid][3]);
		 	traffic[vehicleid] = false;
		}
	
		TextDrawShowForPlayer(playerid, Speedometer[Main]);
		TextDrawShowForPlayer(playerid, Speedometer[EngineInfo]);
		TextDrawShowForPlayer(playerid, Speedometer[Box][0]);
		TextDrawShowForPlayer(playerid, Speedometer[Box][1]);
		TextDrawShowForPlayer(playerid, Speedometer[VehicleInfo]);

		PlayerTextDrawShow(playerid, Speedometer[VehicleEngine][playerid]);
		PlayerTextDrawShow(playerid, Speedometer[VehicleHP][playerid]);
		PlayerTextDrawShow(playerid, Speedometer[VehicleSpeed][playerid]);
		PlayerTextDrawShow(playerid, Speedometer[VehicleFuel][playerid]);
		PlayerTextDrawShow(playerid, Speedometer[VehicleMileage][playerid]);
		
		if(GetViaMoney(playerid) <= 0 && (IsPlayerInTruck(playerid) || IsPlayerInBus(playerid)))
			TextDrawShowForPlayer(playerid, Speedometer[ViaTollX]);
	}

	if(oldstate == PLAYER_STATE_DRIVER)
	{
		TextDrawHideForPlayer(playerid, Speedometer[Main]);
		TextDrawHideForPlayer(playerid, Speedometer[EngineInfo]);
		TextDrawHideForPlayer(playerid, Speedometer[CargoInfoTD]);
		TextDrawHideForPlayer(playerid, Speedometer[Box][0]);
		TextDrawHideForPlayer(playerid, Speedometer[Box][1]);
		TextDrawHideForPlayer(playerid, Speedometer[VehicleInfo]);

		PlayerTextDrawHide(playerid, Speedometer[VehicleEngine][playerid]);
		PlayerTextDrawHide(playerid, Speedometer[VehicleHP][playerid]);
		PlayerTextDrawHide(playerid, Speedometer[VehicleCargo][playerid]);
		PlayerTextDrawHide(playerid, Speedometer[VehicleSpeed][playerid]);
		PlayerTextDrawHide(playerid, Speedometer[VehicleFuel][playerid]);
		PlayerTextDrawHide(playerid, Speedometer[VehicleMileage][playerid]);

		TextDrawHideForPlayer(playerid, Speedometer[ViaTollX]);

		if(GetPVarType(playerid, "jestPrzegladany") != PLAYER_VARTYPE_NONE)
			PlayerSpectatePlayer(GetPVarInt(playerid, "jestPrzegladany"), playerid);

		VehicleDriver[GetPlayerVehicleID(playerid)] = INVALID_PLAYER_ID;
	}

	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OnPlayerKey", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			CallLocalFunction(szTemp, "ddd", playerid, newkeys, oldkeys);
	}

	if((newkeys & KEY_SUBMISSION) && (newkeys & KEY_LOOK_RIGHT))
	{
		if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
		{
			new vehicleid = GetPlayerVehicleID(playerid), engine,lights,alarm,doors,bonnet,boot,objective;
	        GetVehicleParamsEx(vehicleid,engine,lights,alarm,doors,bonnet,boot,objective);
	        
	        if(engine != 1)
	        {
				SetVehicleParamsEx(vehicleid,VEHICLE_PARAMS_ON,lights,alarm,doors,bonnet,boot,objective);

				if(GetPVarInt(playerid, "tacho_pauza"))
				{
					DeletePVar(playerid, "tacho_pauza");
					Msg(playerid, COLOR_INFO, "Kr�cenie zosta�o pauzy {b}wy��czone{/b}.");
				}
	        }
			else
				SetVehicleParamsEx(vehicleid,VEHICLE_PARAMS_OFF,lights,alarm,doors,bonnet,boot,objective);
		}
	}

	if((newkeys & KEY_SUBMISSION) && (newkeys & KEY_LOOK_LEFT))
	{
		if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
		{
			new vehicleid = GetPlayerVehicleID(playerid), trailerid = GetVehicleTrailer(vehicleid), engine,lights,alarm,doors,bonnet,boot,objective;
	        GetVehicleParamsEx(vehicleid,engine,lights,alarm,doors,bonnet,boot,objective);
	        if(lights != 1)
    		{
				SetVehicleParamsEx(vehicleid,engine,VEHICLE_PARAMS_ON,alarm,doors,bonnet,boot,objective);
				SetVehicleParamsEx(trailerid,engine,VEHICLE_PARAMS_ON,alarm,doors,bonnet,boot,objective);
			}
			else
			{
				SetVehicleParamsEx(vehicleid,engine,VEHICLE_PARAMS_OFF,alarm,doors,bonnet,boot,objective);
				SetVehicleParamsEx(trailerid,engine,VEHICLE_PARAMS_OFF,alarm,doors,bonnet,boot,objective);
			}
		}
	}
	
	if(PRESSED(KEY_SPRINT) && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
	{
		if(GetPVarInt(playerid, "UsingLoopingAnim") == 1)
		{
			StopLoopingAnim(playerid);
			ClearAnimations(playerid);
		}
	}
	
	if((newkeys & 8 || newkeys & 32 || newkeys & 128 || ((newkeys & KEY_SUBMISSION) && (newkeys & KEY_FIRE))) && GetPVarInt(playerid, "Tempomat"))
	{
		KillTimer(timer7[playerid]);
		DeletePVar(playerid, "Tempomat");
		Msg(playerid, COLOR_INFO, "Tempomat zosta� {b}wy��czony{/b}.");
	}
	else if((newkeys & KEY_SUBMISSION) && (newkeys & KEY_FIRE))
	{
		if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
		{
			new Float:speed;
			new vehicleid = GetPlayerVehicleID(playerid);
			GetVehicleSpeed(vehicleid, speed);
			new engine,lights,alarm,doors,bonnet,boot,objective;
			GetVehicleParamsEx(vehicleid,engine,lights,alarm,doors,bonnet,boot,objective);
	
			if(engine == 1)
			{
				if(floatround(speed) > 30)
				{
					new Float:health;
					GetVehicleHealth(vehicleid, health);
					SetPVarFloat(playerid, "tHealth", health);
					SetPVarInt(playerid, "Tempomat", 1);
					timer7[playerid] = SetTimerEx("Tempomat", 250, false, "ddf", vehicleid, playerid, speed);
					Msg(playerid, COLOR_INFO, "Tempomat zosta� {b}w��czony{/b}.");
				}
			}
		}
	}

    if((newkeys & KEY_ACTION) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		if(GetVehicleModel(GetPlayerVehicleID(playerid)) == 525 || GetVehicleModel(GetPlayerVehicleID(playerid)) == 531)
		{
			new Float:zX,Float:zY,Float:zZ;
			GetPlayerPos(playerid,zX,zY,zZ);
			new Float:vX,Float:vY,Float:vZ;
			new Found=0;
			new vid=0;

			while((vid<GetVehiclePoolSize())&&(!Found))
			{
				vid++;
				GetVehiclePos(vid,vX,vY,vZ);

				if((floatabs(zX-vX)<10.0)&&(floatabs(zY-vY)<10.0)&&(floatabs(zZ-vZ)<10.0)&&(vid!=GetPlayerVehicleID(playerid)))
				{
					Found=1;
					AttachTrailerToVehicle(vid,GetPlayerVehicleID(playerid));
					Msg(playerid, COLOR_INFO, "Pojazd zosta� podczepiony.");
					break;
				}
			}

			if(!Found) 
				return Msg(playerid, COLOR_ERROR, "W pobli�u nie ma �adnego pojazdu.");
		}
	}
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	if(success)
	{
		new pip[16];

		Loop(i, MAX_PLAYERS)
		{
			GetPlayerIp(i, pip, sizeof(pip));
			if(!strcmp(ip, pip, true) && !HasPlayerFullPermission(i))
			{
				SetPVarInt(i, "RCN", GetPVarInt(i, "RCN")+1);

				if(GetPVarInt(i, "RCN") >= 1)
				{
					CheatKick(i, "pr�ba zalogowania na rcon");
					timer[i] = SetTimerEx("Kickplayer", 500, 0, "d", i);
				}

				ToLog(playerInfo[i][pID], LOG_TYPE_PLAYER, "OnRconLoginAttempt");

				break;
			}
		}
	}

	return 1;
}

public OnPlayerUpdate(playerid)
{
	if(camInfo[playerid][cCameramode] == CAMERA_MODE_FLY)
	{
		new keys,ud,lr;
		GetPlayerKeys(playerid,keys,ud,lr);

		if(camInfo[playerid][cMode] && (GetTickCount() - camInfo[playerid][cLastmove] > 100))
		{
		    // If the last move was > 100ms ago, process moving the object the players camera is attached to
		    MoveCamera(playerid);
		}

		// Is the players current key state different than their last keystate?
		if(camInfo[playerid][cUdold] != ud || camInfo[playerid][cLrold] != lr)
		{
			if((camInfo[playerid][cUdold] != 0 || camInfo[playerid][cLrold] != 0) && ud == 0 && lr == 0)
			{   // All keys have been released, stop the object the camera is attached to and reset the acceleration multiplier
				StopPlayerObject(playerid, camInfo[playerid][cFlyobject]);
				camInfo[playerid][cMode]      = 0;
				camInfo[playerid][cAccelmul]  = 0.0;
			}
			else
			{   // Indicates a new key has been pressed

			    // Get the direction the player wants to move as indicated by the keys
				camInfo[playerid][cMode] = GetMoveDirectionFromKeys(ud, lr);

				// Process moving the object the players camera is attached to
				MoveCamera(playerid);
			}
		}
		camInfo[playerid][cUdold] = ud; camInfo[playerid][cLrold] = lr; // Store current keys pressed for comparison next update
		return 0;
	}
	return 1;
}

forward CountDown();
public CountDown()
{
	new string[6];
    switch(gmInfo[gmCountdown])
    {
    	case 5: { GameTextForAll("~b~-~r~ 5 ~b~-", 1100, 3); }
        case 4: { GameTextForAll("~b~-~r~ 4 ~b~-", 1100, 3); }
        case 3: { GameTextForAll("~b~-~r~ 3 ~b~-", 1100, 3); }
        case 2: { GameTextForAll("~b~-~r~ 2 ~b~-", 1100, 3); }
        case 1: { GameTextForAll("~b~-~r~ 1 ~b~-", 1100, 3); }
        case 0:
        {
        	GameTextForAll("~b~ -~g~Start! ~b~-", 2000, 3);
        	KillTimer(gmInfo[gmCountdownTimer]);
        	gmInfo[gmCountdownStarted] = false;

        	if(gmInfo[gmCountdownFreeze])
	        	Loop(player, MAX_PLAYERS)
	        		if(IsPlayerConnected(player))
	        			TogglePlayerControllable(player, 1);
        }
        default:
        {
        	format(string, sizeof(string), "%d", gmInfo[gmCountdown]);
        	GameTextForAll(string, 1100, 3);
        }
    }
    gmInfo[gmCountdown]--;
} 

public OnPlayerCommandReceived(playerid, cmdtext[])
{
	if(!IsPlayerLogged(playerid))
	{
		Msg(playerid, COLOR_ERROR, "Zaloguj si�, aby u�ywa� komend.");
		return 0;
	}

	if(GetPVarInt(playerid, "Areszt") && !playerInfo[playerid][pAdmin])
		if(strcmp(cmdtext, "/spawn") == 0)
			return Msg(playerid, COLOR_ERROR, "W areszcie nie mo�esz u�y� tej komendy.");


	return 1;
}

public OnPlayerCommandPerformed(playerid, cmdtext[], success)
{
	new string[76];

	format(string, sizeof(string), "[SUCCESS: %d] %s", success, cmdtext);
	ToLog(playerInfo[playerid][pID], LOG_TYPE_COMMANDS, string);

	Loop(i, GetMaxPlayers())
	{
		if(IsPlayerConnected(i) && podgladADMIN[i] == 1 && i != playerid)
		{
			format(string, sizeof string, "@EYE {b}%s{/b} [%d]: {b}%s{/b}", PlayerName(playerid), playerid, cmdtext);
			Msg(i, COLOR_ERROR, string);
		}
	}

	if(!success)
	{
		Msg(playerid, COLOR_ERROR, "Wprowadzono niepoprawn� komend�.");
		return 1;
	}
	
	for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OnPlayerCommand", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			CallLocalFunction(szTemp, "dsd", playerid, cmdtext, success);
	}

	return 1;
}

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
	if(!IsPlayerLogged(playerid))
	{
		if(playertextid == connectVarBox[playerid][0]) // Logowanie
		{
			CancelSelectTextDraw(playerid);
			if(!IsAccountExists(PlayerName(playerid)))
			{
				Msg(playerid, COLOR_ERROR, "Nie odnaleziono konta z Twoim nickiem, zarejestruj si�.");
				SelectTextDraw(playerid, 0xA82A23FF);
				return 1;
			}

			new string[256];
			format(string, sizeof(string), "{FFFFFF}Serwer-Truck SAMP - Logowanie konta.\n\n{a9c4e4}Wprowad� has�o do konta, aby zacz�� rozgrywk�.\n\n{FF4040}Pami�taj, �e jest to WERSJA TESTOWA.\n{FFFFFF}SerwerTruck.eu");
			Dialog_Show(playerid, DIALOG_ID_LOGIN, DIALOG_STYLE_PASSWORD, "Panel > Logowanie", string, "Zaloguj", "Wstecz");
		}
		else if(playertextid == connectVarBox[playerid][1]) // Rejestracja
		{
			CancelSelectTextDraw(playerid);
			if(IsAccountExists(PlayerName(playerid)))
			{
				Msg(playerid, COLOR_ERROR, "Konto o tej nazwie jest ju� zarejestrowane, zaloguj si�.");
				SelectTextDraw(playerid, 0xA82A23FF);
				return 1;
			}

			new string[256];
			format(string, sizeof(string), "{FFFFFF}Serwer-Truck SAMP - Rejestracja konta.\n\n{a9c4e4}Witaj na naszym serwerze.\n{a9c4e4}Je�eli chcesz zacz�� rozgrywk� - zarejestruj konto.\n");
			format(string, sizeof(string), "%s\n\n{FF4040}Pami�taj, �e jest to WERSJA TESTOWA.\n{FFFFFF}www.serwertruck.eu", string, PlayerName(playerid));

			Dialog_Show(playerid, DIALOG_ID_REGISTER, DIALOG_STYLE_PASSWORD, "Panel > Rejestracja", string, "Rejestruj", "Wstecz");
			Msg(playerid, COLOR_INFO3, "Wpisz {b}/faq{/b}, by zobaczy� pomoc.");
			Msg(playerid, COLOR_INFO3, "Wpisz {b}/cmds{/b}, by zobaczy� komendy.");
		}
		else if(playertextid == connectVarBox[playerid][2]) // Changelog
		{
			CancelSelectTextDraw(playerid);
			SelectTextDraw(playerid, 0xA82A23FF);
		}
		else if(playertextid == connectVarBox[playerid][3]) // Pomoc
		{
			CancelSelectTextDraw(playerid);
			Dialog_Show(playerid, DIALOG_FAQ, DIALOG_STYLE_LIST, "FAQ - Spis", "Podstawowe informacje\nSpis firm\nPoziomy kierowcy\nSpecjalne zdolno�ci\nOgraniczenia\nKomendy\nAutorzy", "Wybierz", "Wyjd�");
		}
		else if(playertextid == connectVarBox[playerid][4]) // Autorzy
		{
			CancelSelectTextDraw(playerid);
			Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, " ", "Programi�ci:\n- Maciek.\n- GeDox\n- Kozak59\n\nBeta-Testerzy:\n- [ST]Kill_Repeat\n- [ST][DJ]Bass\n- Reiban.\n\nTworzenie obiekt�w:\n- [ST][DJ]Bass\n- TDi\n- Laud.\n- Muscu\n- Marcin[WGM]\n- PolskiJankes\n- Kierowca\n- Kozak59\n- Devil\n- Mati\n - TRAKER", "Zamknij", #);
		}
		else if(playertextid == connectVarBox[playerid][5]) // Wyj�cie
		{
			CancelSelectTextDraw(playerid);
			Kickplayer(playerid);
		}
		else
			Kickplayer(playerid);
	}
	return 1;
}

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	new skinid;
	
	if(clickedid == MainTextDraws[TruckerSkins][1])
		skinid=1;
	else if(clickedid == MainTextDraws[TruckerSkins][2])
		skinid=7;
	else if(clickedid == MainTextDraws[TruckerSkins][3])
		skinid=12;
	else if(clickedid == MainTextDraws[TruckerSkins][4])
		skinid=37;
	else if(clickedid == MainTextDraws[TruckerSkins][5])
		skinid=98;
	else if(clickedid == MainTextDraws[TruckerSkins][6])
		skinid=101;
	else if(clickedid == MainTextDraws[TruckerSkins][7])
		skinid=192;

	if(skinid)
	{
		for(new i = 0; i <= 8; i++) 
			TextDrawHideForPlayer(playerid, MainTextDraws[TruckerSkins][i]);

		playerInfo[playerid][pSkin]=skinid;
		CancelSelectTextDraw(playerid);
		SetTimerEx("ReqSpawnPlayer", 100, false, "d", playerid);
	}

	for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OnPlayerClickTextDraw", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			CallLocalFunction(szTemp, "dd", playerid, _:clickedid);
	}

	return 1;
}

/*PlayerObjects(playerid)
{
new DBResult:result = db_query(DobjDB, "SELECT ID, PosX, PosY, PosZ FROM `OBJECTS`");
new numRows = db_num_rows(result);

for(new i=0; i<numRows; i++)
{
new I[20], X[20], Y[20], Z[20];
db_get_field(result, 0, I, sizeof I);
db_get_field(result, 1, X, sizeof X);
db_get_field(result, 2, Y, sizeof Y);
db_get_field(result, 3, Z, sizeof Z);

new ID,Float:Pos[3];
ID = strval(I);
Pos[0] = floatstr(X);
Pos[1] = floatstr(Y);
Pos[2] = floatstr(Z);

RemoveBuildingForPlayer(playerid, ID, Pos[0], Pos[1], Pos[2], 0.25);

db_next_row(result);
}
db_free_result(result);
return 1;
}*/

forward SendClientMessageToAdmins(color, const message[]);
public SendClientMessageToAdmins(color, const message[])
{
	for(new a=0; a<MAX_PLAYERS; a++)
		if(IsPlayerConnected(a))
			if(playerInfo[a][pAdmin])
				SendClientMessage(a, color, message);

	return 1;
}

forward OneSecTimer();
public OneSecTimer()
{
	new hour, minute, second/*, year, month, day*/, string[176];

	gettime(hour, minute, second);
	//getdate(year, month, day);
 	
	format(string, sizeof string, "%02d:%02d:%02d", hour, minute, second);
	TextDrawSetString(MainTextDraws[Date], string);

	//format(string, sizeof string, "%02d:%02d:%d", day, month, year);
	//TextDrawSetString(MainTextDraws[Date], string);

	for(new i, maxPlayerID=GetPlayerPoolSize(); i <= maxPlayerID; i++)
		if(IsPlayerConnected(i))
		{
			new Float:pos[3];
			GetPlayerPos(i, pos[0], pos[1], pos[2]);
			if(floatround(pos[0] * pos[1] * pos[2]) != playerInfo[i][pLastPos])
				SetPVarInt(i, "isAFK", 0), playerInfo[i][pLastPos] = floatround(pos[0] * pos[1] * pos[2]);

	    	new 
	    		vehicleid = GetPlayerVehicleID(i), 
	    		Float:speed, 
	    		Float:health, 
	    		firmaid = playerInfo[i][pFirm],
				engine, 
				lights, 
				alarm, 
				doors, 
				bonnet, 
				boot, 
				objective;

			GetVehicleParamsEx(vehicleid,engine,lights,alarm,doors,bonnet,boot,objective);
			GetVehicleSpeed(vehicleid, speed);
			GetPlayerHealth(i, health);

  			if(!IsPlayerInTruck(i) || (IsPlayerInTruck(i) && !engine && GetPVarInt(i, "tacho_pauza") && speed == 0))
  				if(GetDTime(i) != 0)
  					GiveDTime(i, -3);	

			if(GetPlayerState(i) == PLAYER_STATE_DRIVER && IsPlayerInTruck(i))
		    	if(engine && !GetPVarInt(i, "tacho_pauza"))
		    		GiveDTime(i, 1);

			for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
			{
				format(szTemp, sizeof(szTemp), "%s_OneSecPlayerTimer", szHookInclude[nrInc]);

				if(funcidx(szTemp) != -1)
					CallLocalFunction(szTemp, "d", i);
			}

			string[0] = EOS;
		    if(firmaid != 0 && GetPVarInt(i, "Working"))
			    format(string, sizeof(string), "{%06x}%s\n", GetPlayerColor(i) >>> 8, firmInfo[firmaid][tName]);
					
			format(string, sizeof(string), "%s{57AE00}%s {FFFFFF}[{57AE00}ID: %d {FFFFFF}]\n{57AE00}HP: %0.1f\n", string, PlayerName(i), i, health);

			if(GetPVarInt(i, "AFK") || GetPVarInt(i, "isAFK"))
				strcat(string, "AFK\n");

			if(playerInfo[i][pPursued])
				strcat(string, "{F81414}! POSZUKIWANY !{FFFFFF}\n");

			Update3DTextLabelText(Trucking[i], ZIELONY, string);
		}

	for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OneSecTimer", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			CallLocalFunction(szTemp, "");
	}

	return 1;
}

GetBaryIn(playerid)
{
	new baryinid = INVALID_BARYIN_ID;
	new Float:playerPos[3];
	GetPlayerPos(playerid, playerPos[0], playerPos[1], playerPos[2]);

	/*new DBResult:result = db_query(Cargos, "SELECT ID, PosX, PosY, PosZ FROM `BARYIN`");
	new numRows = db_num_rows(result);

	for(new i=0; i<numRows; i++)
	{
	    new pos[3][20];
	    db_get_field(result, 1, pos[0], 20);
	    db_get_field(result, 2, pos[1], 20);
		db_get_field(result, 3, pos[2], 20);

		if(IsPlayerInRangeOfPoint(playerid, 10.0, floatstr(pos[0]), floatstr(pos[1]), floatstr(pos[2])))
		{
		    new idstr[20];
		    db_get_field(result, 0, idstr, 20);
		    baryinid = strval(idstr);
		    break;
		}
	    db_next_row(result);
	}

	db_free_result(result);*/
	return baryinid;
}

GetBar(playerid)
{
	new barid = INVALID_BARY_ID;
	new Float:playerPos[3];
	GetPlayerPos(playerid, playerPos[0], playerPos[1], playerPos[2]);

	/*new DBResult:result = db_query(Cargos, "SELECT ID, PosX, PosY, PosZ FROM `BARY`");
	new numRows = db_num_rows(result);

	for(new i=0; i<numRows; i++)
	{
	    new pos[3][20];
	    db_get_field(result, 1, pos[0], 20);
	    db_get_field(result, 2, pos[1], 20);
		db_get_field(result, 3, pos[2], 20);

		if(IsPlayerInRangeOfPoint(playerid, 10.0, floatstr(pos[0]), floatstr(pos[1]), floatstr(pos[2])))
		{
		    new idstr[20];
		    db_get_field(result, 0, idstr, 20);
		    barid = strval(idstr);
		    break;
		}
	    db_next_row(result);
	}

	db_free_result(result);*/
	return barid;
}

forward Selectspawn(playerid);
public Selectspawn(playerid)
{
	if(!GetPVarInt(playerid, "Worked") || playerInfo[playerid][pFirm] == 0)
		Dialog_Show(playerid, DIALOG_ID_SPAWN_SELECT, DIALOG_STYLE_LIST, " ", "Los Santos\nLas Venturas\nRed County\nSan Fierro", "Wybierz", "Wyjd�");
	else
		Dialog_Show(playerid, DIALOG_ID_SPAWN_SELECT, DIALOG_STYLE_LIST, " ", "Los Santos\nLas Venturas\nRed County\nSan Fierro\n{FF0000}Baza firmowa", "Wybierz", "Wyjd�");

	return 1;
}

forward Refresh();
public Refresh()
{
	Loop(playerid, MAX_PLAYERS)
	{
		new Float:speed,
		vehicleid = GetPlayerVehicleID(playerid);
		GetVehicleSpeed(vehicleid, speed);

		if(SECURITYON == 1)
		{
			if(!playerInfo[playerid][pAdmin] && IsPlayerLogged(playerid) && GetPVarInt(playerid, "InGame"))
			{
				if(floatround(speed) > 270)
				{
					RemovePlayerFromVehicle(playerid);
					CheatKick(playerid, "speedhack");
					timer[playerid] = SetTimerEx("Kickplayer", 500, 0, "d", playerid);
				}
					
				if(GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK)
				{
					CheatBan(playerid, "jetpack");
					timer[playerid] = SetTimerEx("Kickplayer", 500, 0, "d", playerid);
				}
					
				if(playerInfo[playerid][pFirm] == 0)
				{
					switch(GetPlayerWeapon(playerid))
					{
						case 1..42:
						{
							CheatBan(playerid, "weaponhack");
							timer[playerid] = SetTimerEx("Kickplayer", 500, 0, "d", playerid);
						}
						case 44..45:
						{
							CheatBan(playerid, "weaponhack");
							timer[playerid] = SetTimerEx("Kickplayer", 500, 0, "d", playerid);
						}
					}
				}
			}
		}
	}

	Loop(vehicleid, GetVehiclePoolSize()+1)
	{
		new Float:fuel, fueltype, engine,lights,alarm,doors,bonnet,boot,objective,Float:speed;
		
		if(!IsValidVehicle(vehicleid) || IsVehicleTrailer(vehicleid))
			continue;

		fuel = (Spawned[vehicleid] ? vehInfo_Temp[vehicleid][vFuel] : vehInfo[DBVehID[vehicleid]][vFuel]);
		fueltype = (Spawned[vehicleid] ? vehInfo_Temp[vehicleid][vFuelType] : vehInfo[DBVehID[vehicleid]][vFuelType]);

		GetVehicleSpeed(vehicleid, speed);
		GetVehicleParamsEx(vehicleid,engine,lights,alarm,doors,bonnet,boot,objective);

		if(fueltype == FUEL_TYPE_GAS)
		{
			new Float:VX, Float:VY, Float:VZ;
			GetVehicleVelocity(vehicleid, VX, VY, VZ);
			SetVehicleVelocity(vehicleid, VX * 0.8, VY * 0.8, VZ);
		}

		if(engine)
		{
			fuel -= (fueltype == FUEL_TYPE_GAS) ? (floatmul(speed, 0.00004)) : (floatmul(speed, 0.00008));

			if(fuel <= 0)
			{
				fuel = 0;
				SetVehicleParamsEx(vehicleid,VEHICLE_PARAMS_OFF,lights,alarm,doors,bonnet,boot,objective);
			}

			if(Spawned[vehicleid])
				vehInfo_Temp[vehicleid][vFuel] = fuel;
			else
				vehInfo[DBVehID[vehicleid]][vFuel] = fuel;
		}
	}

	Wypadek();
	return 1;
}

stock IsVehicleTrailer(vehicleid)
{
	new model = GetVehicleModel(vehicleid);
	if(model < 400)
		return 0;

	if(model == 450 || model == 591 || model == 435 || model == 584)
		return true;
	return false;
}

forward Kickplayer(playerid);
public Kickplayer(playerid)
	return Kick(playerid);

forward ReqSpawnPlayer(playerid);
public ReqSpawnPlayer(playerid)
	return SpawnPlayer(playerid);

stock SavePlayer(playerid, saveTime=0)
{
	if(IsPlayerLogged(playerid))
	{
		new string[512], Name[32], h, mi, s, d, mo, y;

		mysql_real_escape_string(PlayerName(playerid), Name);

		getdate(y, mo, d);
		gettime(h, mi, s);

		format(string, sizeof(string), "UPDATE `Accounts` SET `Money`='%d',`Score`='%d',`Firma`='%d',`Skin`='%d',`Tacho`='%d',`Foto`='%d',`Toll`='%d',`Gtime`='%d',`Hour`='%02d',`Minute`='%02d',\
				`Day`='%02d',`Mounth`='%02d',`Year`='%d',`Worktime`='%d',`Admin`='%d',`Adr`='%d'", playerInfo[playerid][pMoney], playerInfo[playerid][pScore],
				playerInfo[playerid][pFirm],playerInfo[playerid][pSkin],playerInfo[playerid][pTacho],playerInfo[playerid][pPhoto],playerInfo[playerid][pToll],playerInfo[playerid][pGTime],
				h,mi,d,mo,y,playerInfo[playerid][pWorkTime], playerInfo[playerid][pAdminAllowed], playerInfo[playerid][pADR]);
		
		if(saveTime)
		{
			format(string, sizeof(string), "%s, `TimeOnline`=`TimeOnline`+'%d'", string, (floatround((GetTickCount()-GetPVarInt(playerid, "IleGral"))/1000)));
			SetPVarInt(playerid, "IleGral", GetTickCount());
		}

		format(string, sizeof(string), "%s WHERE `Name`='%s'", string, Name);
		mysql_query(string);
	}

	return 1;
}

forward SaveVehicle(vehicleid);
public SaveVehicle(vehicleid)
{
	new vehuid = DBVehID[vehicleid], string[300];

	format(string, sizeof(string), "UPDATE `Pojazdy` SET `PosX`='%f', `PosY`='%f', `PosZ`='%f', `PosA`='%f', `Fuel`='%f', `Przebieg`='%f', `Color1`='%d', `Color2`='%d', `owner_vce`='%d', `Plate`='%s', `Przeglad`='%s' WHERE `id`='%d'",
		vehInfo[vehuid][vPosX],
		vehInfo[vehuid][vPosY],
		vehInfo[vehuid][vPosZ],
		vehInfo[vehuid][vPosA],
		vehInfo[vehuid][vFuel],
		vehInfo[vehuid][vPrzebieg],
		vehInfo[vehuid][vColor1],
		vehInfo[vehuid][vColor2],
		vehInfo[vehuid][vOwnerVC],
		vehInfo[vehuid][vPlate],
		vehInfo[vehuid][vPrzeglad],
		vehInfo[vehuid][vID]);
	mysql_query(string);

	return 1;
}

forward Jobtime();
public Jobtime()
{
	Loop(playerid, MAX_PLAYERS)
		if(IsPlayerConnected(playerid))
			if(firmInfo[playerInfo[playerid][pFirm]][tType] >= TEAM_TYPE_POLICE && firmInfo[playerInfo[playerid][pFirm]][tType] <= TEAM_TYPE_POMOC)
			    if(GetPVarInt(playerid, "Worked") && !GetPVarInt(playerid, "AFK") && !GetPVarInt(playerid, "isAFK"))
			   		GiveWork(playerid, 2);

	return 1;
}

forward Update();
public Update()
{
	Loop(playerid, MAX_PLAYERS)
	{
	    if(IsPlayerConnected(playerid))
	    {
	    	new vehicleid = GetPlayerVehicleID(playerid);
			new trailerid = GetVehicleTrailer(vehicleid);
			new Float:speed;
			new engine,lights,alarm,doors,bonnet,boot,objective;
 			GetVehicleParamsEx(vehicleid,engine,lights,alarm,doors,bonnet,boot,objective);
			GetVehicleSpeed(vehicleid, speed);

 			GetVehiclePos(vehicleid, vPos[vehicleid][3], vPos[vehicleid][4], vPos[vehicleid][5]);

 			StripUpdate(playerid);

 			if(Spawned[vehicleid])
 				vehInfo_Temp[vehicleid][vPrzebieg] += floatsqroot(floatpower(floatsub(vPos[vehicleid][3], vPos[vehicleid][0]), 2) + floatpower(floatsub(vPos[vehicleid][4], vPos[vehicleid][1]), 2)+floatpower(floatsub(vPos[vehicleid][5], vPos[vehicleid][2]), 2));
 			else
				vehInfo[DBVehID[vehicleid]][vPrzebieg] += floatsqroot(floatpower(floatsub(vPos[vehicleid][3], vPos[vehicleid][0]), 2) + floatpower(floatsub(vPos[vehicleid][4], vPos[vehicleid][1]), 2)+floatpower(floatsub(vPos[vehicleid][5], vPos[vehicleid][2]), 2));

 			if(IsTrailerAttachedToVehicle(vehicleid))
 			{
 				if(Spawned[trailerid])
 					vehInfo_Temp[trailerid][vPrzebieg] += floatsqroot(floatpower(floatsub(vPos[trailerid][3], vPos[trailerid][0]), 2) + floatpower(floatsub(vPos[trailerid][4], vPos[trailerid][1]), 2)+floatpower(floatsub(vPos[vehicleid][5], vPos[vehicleid][2]), 2));
 				else
					vehInfo[DBVehID[trailerid]][vPrzebieg] += floatsqroot(floatpower(floatsub(vPos[trailerid][3], vPos[trailerid][0]), 2) + floatpower(floatsub(vPos[trailerid][4], vPos[trailerid][1]), 2)+floatpower(floatsub(vPos[vehicleid][5], vPos[vehicleid][2]), 2));
			}

				GetVehiclePos(GetPlayerVehicleID(playerid),vPos[GetPlayerVehicleID(playerid)][0],vPos[GetPlayerVehicleID(playerid)][1],vPos[GetPlayerVehicleID(playerid)][2]);

			if(firmInfo[playerInfo[playerid][pFirm]][tType] == TEAM_TYPE_POLICE && GetPVarInt(playerid, "RadarOnline"))
			{
				/*new string[40];
				
				new player = GetDistanceToNearestDriver(playerid);
				new Distance = GetDistancePlayerToPlayer(playerid, player);

				if(Distance < 100 && GetPlayerState(player) == PLAYER_STATE_DRIVER)
				{
					new forplayervehicleid = GetPlayerVehicleID(player);
					GetVehicleSpeed(forplayervehicleid, speed);
					format(string,sizeof string,"~w~%s ( %d )",PlayerName(player),player);
					TextDrawSetString(Radarpolice[playerid][1],string);
					format(string,sizeof string,"~r~%d ~w~km/h",floatround(speed));
					TextDrawSetString(Radarpolice[playerid][2],string);
				}*/
			}
		}
	}
}

forward Unmute(playerid);
public Unmute(playerid)
{
	new seconds = GetPVarInt(playerid, "Mutetime");
	new h, m, s;
	ConvertSeconds(seconds,h,m,s);

	if(seconds == 0)
	{
		DeletePVar(playerid,"Mute");
		DeletePVar(playerid,"Mutetime");
	}
	else
	{
		seconds--;
		SetPVarInt(playerid, "Mutetime", seconds);
		SetTimerEx("Unmute", 1000, false, "d", playerid);
	}

	return 1;
}

forward awaryjne(playerid);
public awaryjne(playerid)
{
new vehicleid = GetPlayerVehicleID(playerid);
new model = GetVehicleModel(vehicleid);

if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
{
	if(model == 515)
	{
	for(new x = 0; x < 4; x++) kierunki[vehicleid][x] = CreateObject(19294,0,0,-1000,0,0,0,100);

	AttachObjectToVehicle(kierunki[vehicleid][0], GetPlayerVehicleID(playerid), -1.425000,-4.949999,-1.154999,0.000000,0.000000,0.000000);
	AttachObjectToVehicle(kierunki[vehicleid][1], GetPlayerVehicleID(playerid), 1.364996,-4.949997,-1.134999,0.000000,0.000000,0.000000);
	AttachObjectToVehicle(kierunki[vehicleid][2], GetPlayerVehicleID(playerid), -1.560000,4.424998,-0.790000,0.000000,0.000000,0.000000);
	AttachObjectToVehicle(kierunki[vehicleid][3], GetPlayerVehicleID(playerid), 1.600000,4.459999,-0.804999,0.000000,0.000000,0.000000);
	traffic[vehicleid] = true;
	}
	else if(model == 403)
	{
	for(new x = 0; x < 4; x++) kierunki[vehicleid][x] = CreateObject(19294,0,0,-1000,0,0,0,100);

	AttachObjectToVehicle(kierunki[vehicleid][0], GetPlayerVehicleID(playerid), -0.540000,-4.060001,-0.974999,0.000000,0.000000,0.000000);
	AttachObjectToVehicle(kierunki[vehicleid][1], GetPlayerVehicleID(playerid), 0.474999,-4.060001,-0.974999,0.000000,0.000000,0.000000);
	AttachObjectToVehicle(kierunki[vehicleid][2], GetPlayerVehicleID(playerid), -1.144999,4.214999,-0.225000,0.000000,0.000000,0.000000);
	AttachObjectToVehicle(kierunki[vehicleid][3], GetPlayerVehicleID(playerid), 1.225000,4.214999,-0.225000,0.000000,0.000000,0.000000);
	traffic[vehicleid] = true;
	}
	else if(model == 514)
	{
	for(new x = 0; x < 4; x++) kierunki[vehicleid][x] = CreateObject(19294,0,0,-1000,0,0,0,100);

	AttachObjectToVehicle(kierunki[vehicleid][0], GetPlayerVehicleID(playerid), -0.449999,-5.029996,-0.809999,0.000000,0.000000,0.000000);
	AttachObjectToVehicle(kierunki[vehicleid][1], GetPlayerVehicleID(playerid), 0.394999,-5.029996,-0.809999,0.000000,0.000000,0.000000);
	AttachObjectToVehicle(kierunki[vehicleid][2], GetPlayerVehicleID(playerid), -1.265000,4.199999,0.085000,0.000000,0.000000,0.000000);
	AttachObjectToVehicle(kierunki[vehicleid][3], GetPlayerVehicleID(playerid), 1.229999,4.199999,0.085000,0.000000,0.000000,0.000000);
	traffic[vehicleid] = true;
	}
	else if(model == 455)
	{
	for(new x = 0; x < 4; x++) kierunki[vehicleid][x] = CreateObject(19294,0,0,-1000,0,0,0,100);

	AttachObjectToVehicle(kierunki[vehicleid][0], GetPlayerVehicleID(playerid), -1.380000,-4.424999,-0.789999,0.000000,0.000000,0.000000);
	AttachObjectToVehicle(kierunki[vehicleid][1], GetPlayerVehicleID(playerid), 1.364999,-4.424999,-0.789999,0.000000,0.000000,0.000000);
	AttachObjectToVehicle(kierunki[vehicleid][2], GetPlayerVehicleID(playerid), 1.384998,3.675000,-0.075000,0.000000,0.000000,0.000000);
	AttachObjectToVehicle(kierunki[vehicleid][3], GetPlayerVehicleID(playerid), -1.320001,3.675000,-0.075000,0.000000,0.000000,0.000000);
	traffic[vehicleid] = true;
	}
	else if(model == 578)
	{
	for(new x = 0; x < 4; x++) kierunki[vehicleid][x] = CreateObject(19294,0,0,-1000,0,0,0,100);

	AttachObjectToVehicle(kierunki[vehicleid][0], GetPlayerVehicleID(playerid), -1.174999,4.425000,-0.174999,0.000000,0.000000,0.000000);
	AttachObjectToVehicle(kierunki[vehicleid][1], GetPlayerVehicleID(playerid), 1.180000,4.425000,-0.174999,0.000000,0.000000,0.000000);
	AttachObjectToVehicle(kierunki[vehicleid][2], GetPlayerVehicleID(playerid), -1.295000,-5.415002,-0.500000,0.000000,0.000000,0.000000);
	AttachObjectToVehicle(kierunki[vehicleid][3], GetPlayerVehicleID(playerid), 1.275000,-5.415002,-0.500000,0.000000,0.000000,0.000000);
	traffic[vehicleid] = true;
	}
	else if(model == 459)
	{
	for(new x = 0; x < 4; x++) kierunki[vehicleid][x] = CreateObject(19294,0,0,-1000,0,0,0,100);

	AttachObjectToVehicle(kierunki[vehicleid][0], GetPlayerVehicleID(playerid), -0.919999,-2.474999,0.280000,0.000000,0.000000,0.000000);
	AttachObjectToVehicle(kierunki[vehicleid][1], GetPlayerVehicleID(playerid), 0.879999,-2.474999,0.280000,0.000000,0.000000,0.000000);
	AttachObjectToVehicle(kierunki[vehicleid][2], GetPlayerVehicleID(playerid), -0.974999,2.550000,-0.075000,0.000000,0.000000,0.000000);
	AttachObjectToVehicle(kierunki[vehicleid][3], GetPlayerVehicleID(playerid), 0.974999,2.550000,-0.075000,0.000000,0.000000,0.000000);
	traffic[vehicleid] = true;
	}
	else if(model == 440)
	{
	for(new x = 0; x < 4; x++) kierunki[vehicleid][x] = CreateObject(19294,0,0,-1000,0,0,0,100);

	AttachObjectToVehicle(kierunki[vehicleid][0], GetPlayerVehicleID(playerid), -0.919998,2.549999,-0.300000,0.000000,0.000000,0.000000);
	AttachObjectToVehicle(kierunki[vehicleid][1], GetPlayerVehicleID(playerid), 0.960000,2.549999,-0.300000,0.000000,0.000000,0.000000);
	AttachObjectToVehicle(kierunki[vehicleid][2], GetPlayerVehicleID(playerid), -0.915000,-2.709999,0.075000,0.000000,0.000000,0.000000);
	AttachObjectToVehicle(kierunki[vehicleid][3], GetPlayerVehicleID(playerid), 0.864999,-2.709999,0.075000,0.000000,0.000000,0.000000);
	traffic[vehicleid] = true;
	}
}
return 1;
}


forward SaveALL();
public SaveALL()
{
	new 
		string[128],
		t[3];

	gettime(t[0], t[1], t[2]);
	SetWorldTime(t[0]);

	Loop(playerid, MAX_PLAYERS)
	{
		if(IsPlayerConnected(playerid))
			SetPlayerTime(playerid, t[0], t[1]);

		if(IsPlayerLogged(playerid))
		{
			SavePlayer(playerid);
			if(!GetPVarInt(playerid, "isAFK"))
			{
				new curPos, Float:pos[3];
				GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
				curPos = floatround(pos[0] * pos[1] * pos[2]);

				if(playerInfo[playerid][pLastPos] == curPos)
					SetPVarInt(playerid, "isAFK", 1),
					playerInfo[playerid][pLastPos] = curPos;
			}
		}
	}

	Loop(firmaid, 8)
	{
		if(firmInfo[firmaid][tType])
		{
			format(string, sizeof(string), "UPDATE `Firmy` SET `Bank`=%d WHERE `id`='%d'", firmInfo[firmaid][tBank], firmaid);
			mysql_query(string);
		}
	}

	return 1;
}

forward Update2();
public Update2()
{
	Loop(playerid, MAX_PLAYERS)
	{
		if(IsPlayerConnected(playerid))
		{
			new string[120];
			new vehicleid = GetPlayerVehicleID(playerid);
			new Float:speed, Float:HP, Float:trailerHP, trailerHP2[10], engine, lights, alarm, doors, bonnet, boot, objective;
			GetVehicleSpeed(vehicleid, speed);
			GetVehicleHealth(vehicleid, HP);
			GetVehicleHealth(GetVehicleTrailer(vehicleid), trailerHP);

			GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

			if(GetVehicleTrailer(vehicleid))
				format(trailerHP2, sizeof(trailerHP2), "%0.0f%%", trailerHP/10);
			else
				format(trailerHP2, sizeof(trailerHP2), "~r~Brak");

			if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
			{
				format(string, sizeof(string), "%d Km/H", floatround(speed));
				PlayerTextDrawSetString(playerid, Speedometer[VehicleSpeed][playerid], string);

				format(string, sizeof(string), "%s~n~%s", (lights == VEHICLE_PARAMS_ON) ? ("~g~ON") : ("~r~OFF"), (engine == VEHICLE_PARAMS_ON) ? ("~g~ON") : ("~r~OFF"));
				PlayerTextDrawSetString(playerid, Speedometer[VehicleEngine][playerid], string);

				format(string, sizeof(string), "%0.0f%%~n~%s", HP/10, trailerHP2);
				PlayerTextDrawSetString(playerid, Speedometer[VehicleHP][playerid], string);

				format(string, sizeof(string), "Paliwo:~n~%0.1f/%d L", (Spawned[vehicleid] ? vehInfo_Temp[vehicleid][vFuel] : vehInfo[DBVehID[vehicleid]][vFuel]), MaxFuel(GetVehicleModel(vehicleid)));
				PlayerTextDrawSetString(playerid, Speedometer[VehicleFuel][playerid], string);

				format(string, sizeof(string), "Przebieg:~n~%0.1fkm", (Spawned[vehicleid] ? vehInfo_Temp[vehicleid][vPrzebieg] : vehInfo[DBVehID[vehicleid]][vPrzebieg])/1000);
				PlayerTextDrawSetString(playerid, Speedometer[VehicleMileage][playerid], string);
			}
		}
	}
	return 1;
}

public OnTrailerUpdate(playerid, vehicleid)
{
    if(!Spawned[vehicleid])
	{
		if(firmInfo[playerInfo[playerid][pFirm]][tType] == TEAM_TYPE_POMOC) 
			return 1;

		if( (vehInfo[DBVehID[vehicleid]][vOwnerType] == OWNER_TYPE_PLAYER && vehInfo[DBVehID[vehicleid]][vOwnerID] != playerInfo[playerid][pID]) ||
				(vehInfo[DBVehID[vehicleid]][vOwnerType] == OWNER_TYPE_TEAM && vehInfo[DBVehID[vehicleid]][vOwnerID] != playerInfo[playerid][pFirm]))
		{
			DetachTrailerFromVehicle(GetPlayerVehicleID(playerid));
			Msg(playerid, COLOR_ERROR, "Nie mo�esz podczepi� tej naczepy.");

			new Float:x, Float:y, Float:z, Float:a;
			GetVehiclePos(GetPlayerVehicleID(playerid), x, y, z);
			GetVehicleZAngle(GetPlayerVehicleID(playerid), a);

			SetVehiclePos(GetPlayerVehicleID(playerid), x + 2*floatcos(90+a, degrees), y+ 2*floatsin(90-a, degrees), z);
		}
	}

	for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OnTrailerUpdate", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			CallLocalFunction(szTemp, "dd", playerid, vehicleid);
	}

    return 1;
}

forward HungerUpdate();
public HungerUpdate()
{
	Loop(playerid, MAX_PLAYERS)
	{
		if(IsPlayerLogged(playerid))
		{
			if(playerInfo[playerid][pHunger] == 5.0)
			{
				new Float:health;
				GetPlayerHealth(playerid, health);

				SetPlayerHealth(playerid, health-10);
				
				SetPlayerProgressBarValue(playerid, hudInfo[tdHungerProgress][playerid], 5.0);
				UpdatePlayerProgressBar(playerid, hudInfo[tdHungerProgress][playerid]);
			}
			else
			{
				playerInfo[playerid][pHunger] -= random(8);
				SetPlayerProgressBarValue(playerid, hudInfo[tdHungerProgress][playerid], playerInfo[playerid][pHunger]);
				UpdatePlayerProgressBar(playerid, hudInfo[tdHungerProgress][playerid]);
			}
		}
	}
	return 1;
}

forward HideFlesh(playerid);
public HideFlesh(playerid)
{
	TextDrawHideForPlayer(playerid, FleshText);
	SetPVarInt(playerid, "PhotoPoint", 0);
	KillTimer(timer5[playerid]);
	return 1;
}


public OnPlayerEditDynamicObject(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
	new Float:oldX, Float:oldY, Float:oldZ,
		Float:oldRotX, Float:oldRotY, Float:oldRotZ;
	GetDynamicObjectPos(objectid, oldX, oldY, oldZ);
	GetDynamicObjectRot(objectid, oldRotX, oldRotY, oldRotZ);

	if(response == EDIT_RESPONSE_CANCEL)
	{
		SetDynamicObjectPos(objectid, oldX, oldY, oldZ);
		SetDynamicObjectRot(objectid, oldRotX, oldRotY, oldRotZ);
	}

	for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OnPlayerEditDynObject", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			CallLocalFunction(szTemp, "dddffffff", playerid, objectid, response, x, y, z, rx, ry, rz);
	}

	return 1;
}

public OnPlayerEditObject(playerid, playerobject, objectid, response, Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ)
{
	for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OnPlayerEditObject", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			CallLocalFunction(szTemp, "ddddffffff", playerid, playerobject, objectid, response, fX, fY, fZ, fRotX, fRotY, fRotZ);
	}

	return 1;
}

forward Tempomat(vehicleid, playerid, Float:speed);
public Tempomat(vehicleid, playerid, Float:speed)
{
	if(GetPVarInt(playerid, "Tempomat"))
	{
		new Float:health;
		GetVehicleHealth(vehicleid, health);
		new Float:speed2;
		GetVehicleSpeed(vehicleid, speed2);
		new engine,lights,alarm,doors,bonnet,boot,objective;
		GetVehicleParamsEx(vehicleid,engine,lights,alarm,doors,bonnet,boot,objective);
		if(!engine)
		{
			KillTimer(timer7[playerid]);
			DeletePVar(playerid, "Tempomat");
			Msg(playerid, COLOR_INFO, "{C8FF91}Tempomat zosta�{FFFFFF}wy��czony.");
		}
		if(health == GetPVarFloat(playerid, "tHealth") && (speed2+15) > speed)
		{
			timer7[playerid] = SetTimerEx("Tempomat", 250, false, "ddf", vehicleid, playerid, speed);
			SetVehicleSpeed(vehicleid, speed);
		}
		else
		{
			KillTimer(timer7[playerid]);
			DeletePVar(playerid, "Tempomat");
			Msg(playerid, COLOR_INFO, "{C8FF91}Tempomat zosta�{FFFFFF}wy��czony.");
		}
	}
	return 1;
}

forward cameragobackplayer(playerid);
public cameragobackplayer(playerid)
{
	TogglePlayerControllable(playerid, 1);
	SetCameraBehindPlayer(playerid);
	KillTimer(timer8[playerid]);
	SetPlayerPos(playerid, -1629.4374,1287.6179,7.0391);
	return 1;
}

forward cameragobackplayer2(playerid);
public cameragobackplayer2(playerid)
{
	TogglePlayerControllable(playerid, 1);
	SetCameraBehindPlayer(playerid);
	KillTimer(timer10[playerid]);
	SetPlayerPos(playerid, -1934.4926,272.3068,41.0469);
	return 1;
}

forward Otworzbrame(playerid);
public Otworzbrame(playerid)
{
	switch(firmInfo[playerInfo[playerid][pFirm]][tType])
	{
		case TEAM_TYPE_POLICE:
		{
			MoveDynamicObject(brama[1], 2293.8505859375, 2498.8203125, -0.6 ,2);
			MoveDynamicObject(brama[2], 2335.1005859375, 2443.7001953125, 1.9 ,2);
		}

		case TEAM_TYPE_MEDIC:
			MoveDynamicObject(brama[3], 1269.400390625, 797.0, 7.1 ,2);

		case TEAM_TYPE_POMOC:
		{
			MoveDynamicObject(brama[13], 1075.2998046875, 1943.099609375, 7.0 ,2);
			MoveDynamicObject(brama[14], 1147.400390625, 2044.0, 7.0 ,2);

		}

		case TEAM_TYPE_CARGO:
		{
			MoveDynamicObject(brama[18], 2478.5, 2513.0, 7.0 ,2);
			MoveDynamicObject(brama[19], 2527.2783203125, 2424.1005859375, 7.0 ,2);
		}

		case TEAM_TYPE_PETROL:
		{
			MoveDynamicObject(brama[16], 2827.2001953125, 1384.900390625, 7.0 ,2);
			MoveDynamicObject(brama[17], 2758.0, 1313.400390625, 8.0 ,2);
		}

		case TEAM_TYPE_SMLOG:
		{
			MoveDynamicObject(brama[20], -2606.8000488281, 580.29998779297, 10.5 ,2);
			MoveDynamicObject(brama[21], -2607.0, 696.70001220703, 24.0 ,2);
		}

		case TEAM_TYPE_BUILD:
			MoveDynamicObject(brama[15], -168.89999389648, 79.599998474121, -1.0 ,2);
	}

	return 1;
}

forward Zamknijbrame(playerid);
public Zamknijbrame(playerid)
{
	switch(firmInfo[playerInfo[playerid][pFirm]][tType])
	{
		case TEAM_TYPE_POLICE:
		{
			MoveDynamicObject(brama[1], 2293.8505859375, 2498.8203125, 4.4499998092651 ,2);
			MoveDynamicObject(brama[2], 2335.1005859375, 2443.7001953125, 6.9499998092651 ,2);
		}

		case TEAM_TYPE_MEDIC:
			MoveDynamicObject(brama[3], 1269.400390625, 797.0, 12.699999809265 ,2);

		case TEAM_TYPE_POMOC:
		{
			MoveDynamicObject(brama[13], 1075.2998046875, 1943.099609375, 12.800000190735 ,2);
			MoveDynamicObject(brama[14], 1147.400390625, 2044.0, 12.800000190735 ,2);
		}

		case TEAM_TYPE_CARGO:
		{
			MoveDynamicObject(brama[18], 2478.5, 2513.0, 12.60000038147 ,2);
			MoveDynamicObject(brama[19], 2527.2783203125, 2424.1005859375, 12.60000038147 ,2);
		}

		case TEAM_TYPE_PETROL:
		{
			MoveDynamicObject(brama[16], 2827.2001953125, 1384.900390625, 12.5 ,2);
			MoveDynamicObject(brama[17], 2758.0, 1313.400390625, 13.800000190735 ,2);
		}

		case TEAM_TYPE_SMLOG:
		{
			MoveDynamicObject(brama[20], -2606.8000488281, 580.29998779297, 16.200000762939 ,2);
			MoveDynamicObject(brama[21], -2607.0, 696.70001220703, 29.60000038147 ,2);
		}

		case TEAM_TYPE_BUILD:
			MoveDynamicObject(brama[15], -168.89999389648, 79.599998474121, 5.0 ,2);
	}

	return 1;
}

CMD:ban(playerid, params[])
{
	new forplayerid, Powod[76], string[256];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnie�");

	if(sscanf(params, "ds[76]", forplayerid, Powod))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /ban [id gracza] [pow�d]");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie jest obecny na serwerze.");

	format(string, sizeof string, "Gracz {b}%s{/b} zosta� {b}zbanowany{/b} przez {b}%s{/b} z powodu {b}%s{/b}.", PlayerName(forplayerid), PlayerName(playerid), Powod);
	MsgToAll(COLOR_ERROR, string);

	new Y, Mo, D, H, Mi, S, ip[16];
	getdate(Y, Mo, D);
	gettime(H, Mi, S);
	GetPlayerIp(forplayerid, ip, sizeof(ip));
	format(string, sizeof string, "INSERT INTO `Bans` (`Name`, `Nameadmin`, `Hour`, `Minute`, `Day`, `Month`, `Year`, `Reason`, `IP`) VALUES('%s', '%s', '%02d', '%02d', '%02d', '%02d', '%02d', '%s', '%s')", PlayerName(forplayerid), PlayerName(playerid), H, Mi, D, Mo, Y, Powod, ip);
	mysql_query(string);
	ShowInfo(playerid, string);

	timer[playerid] = SetTimerEx("Kickplayer", 500, 0, "d", forplayerid);
	return 1;
}

CMD:unban(playerid, params[])
{
	new IP[76], string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnie�");

	if(sscanf(params, "s[76]", IP))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /unban [ip gracza]");

	format(string, sizeof string, "Konto {b}%s{/b} zosta�o odbanowane.", IP);
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "DELETE FROM `Bans` WHERE `IP`= '%s'", IP);
	mysql_query(string);

	return 1;
}

CMD:banip(playerid, params[])
{
	new IP[50], Powod[50], string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnie�");

	if(sscanf(params, "s[50]s[50]", IP, Powod))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /banip [ip gracza] [pow�d]");

	format(string, sizeof string, "Konto {b}%s{/b} zosta�o zbanowane.", IP);
	Msg(playerid, COLOR_INFO, string);

	new Y, Mo, D, H, Mi, S;
	getdate(Y, Mo, D);
	gettime(H, Mi, S);
	format(string, sizeof string, "INSERT INTO `Bans` VALUES('-', '%s', '%02d', '%02d', '%02d', '%02d', '%02d', '%s', '%s')", PlayerName(playerid), H, Mi, D, Mo, Y, Powod, IP);
	mysql_query(string);

	return 1;
}

CMD:kick(playerid, params[])
{
	new forplayerid, Powod[76], string[256];

	if(sscanf(params, "ds[76]", forplayerid, Powod))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /kick [id gracza] [pow?]");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie jest obecny na serwerze.");

	if(playerInfo[playerid][pAdmin])
	{
		format(string, sizeof string, "Gracz {b}%s{/b} zosta� wyrzucony przez {b}%s{/b} z powodu {b}%s{/b}.", PlayerName(forplayerid), PlayerName(playerid), Powod);
		MsgToAll(COLOR_ERROR, string);
		timer[playerid] = SetTimerEx("Kickplayer", 500, 0, "d", forplayerid);
	} 
	else Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnie�");
	return 1;
}

CMD:removebuilding(playerid, params[])
{
	new id, X[10], Y[10], Z[10], R[20], string[256];

	if(!playerInfo[playerid][pBuildmaster])
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnie� (BuildMaster).");

	if(sscanf(params, "ds[10]s[10]s[10]s[20]", id, X, Y, Z, R))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /removebuilding [id] [x] [y] [z] [komentarz]");
	
	format(string, sizeof string, "INSERT INTO `st_usuniete_obiekty` VALUES('%d', '%s', '%s', '%s', '%s')", id, X, Y, Z, R);
	mysql_query(string);

	new Float:Pos[3];
	Pos[0] = floatstr(X);
	Pos[1] = floatstr(Y);
	Pos[2] = floatstr(Z);

	Loop(playeri, MAX_PLAYERS)
		RemoveBuildingForPlayer(playeri, id, Pos[0], Pos[1], Pos[2], 0.25);
	
	return 1;
}

CMD:restorebuilding(playerid, params[])
{
	new R[20], string[128];

	if(!playerInfo[playerid][pBuildmaster])
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnie� (BuildMaster).");

	if(sscanf(params, "s[20]", R))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /restorebuilding [komentarz]");

	format(string, sizeof string, "DELETE FROM `st_usuniete_obiekty` WHERE `Comment`= '%s'", R);
	mysql_query(string);

	return 1;
}

CMD:warn(playerid, params[])
{
	new forplayerid, Powod[20], string[200];

	if(sscanf(params, "ds[20]", forplayerid, Powod))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /warn [id gracza] [pow�d]");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie jest obecny na serwerze.");

	if(playerInfo[playerid][pAdmin])
	{
		SetPVarInt(forplayerid, "Warn", GetPVarInt(forplayerid, "Warn")+1);
		format(string, sizeof string, "Gracz {b}%s{/b} zosta� ostrze�ony [{b}%d/3{/b}] przez {b}%s{/b} z powodu {b}%s{/b}.", PlayerName(forplayerid), GetPVarInt(forplayerid, "Warn"), PlayerName(playerid), Powod);
		MsgToAll(COLOR_ERROR, string);
	} 
	else Msg(playerid, COLOR_ERROR, "Nie masz uprawnie�.");

	if(GetPVarInt(forplayerid, "Warn") == 3)
	{
		CheatKick(forplayerid, "trzy ostrze�enia");
		timer[forplayerid] = SetTimerEx("Kickplayer", 500, 0, "d", forplayerid);
	}
	return 1;
}

CMD:unwarn(playerid, params[])
{
	new forplayerid, string[176];

	if(sscanf(params, "d", forplayerid))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /unwarn [id gracza]");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie jest obecny na serwerze.");

	if(GetPVarInt(forplayerid, "Warn") == 0)
		return Msg(playerid, COLOR_ERROR, "Gracz ten nie posiada �adnych ostrze�e?");

	if(playerInfo[playerid][pAdmin])
	{
		SetPVarInt(forplayerid, "Warn", GetPVarInt(forplayerid, "Warn")-1);
		format(string, sizeof string, "Graczowi {b}%s{/b} zosta�o cofni�te ostrze�enie przez administratora {b}%s{/b}.", PlayerName(forplayerid), PlayerName(playerid));
		MsgToAll(COLOR_ERROR, string);
	}
	else
	{
		Msg(playerid, COLOR_ERROR, "Nie masz uprawnie�.");
	}
	return 1;
}

CMD:givemoney(playerid, params[])
{
	new forplayerid, money, string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnie�.");

	if(sscanf(params, "dd", forplayerid, money))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /givemoney [id gracza] [ilo��]");

	GiveMoney(forplayerid, money);
	format(string, sizeof string, "Da�e� {b}%d${/b} graczowi {b}%s{/b}.", money, PlayerName(forplayerid));
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "Otrzyma�e� {b}%d${/b} od administratora {b}%s{/b}.", money, PlayerName(playerid));
	Msg(forplayerid, COLOR_INFO, string);
	return 1;
}

CMD:givescore(playerid, params[])
{
	new forplayerid, money, string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnie�.");

	if(sscanf(params, "dd", forplayerid, money))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /givescore [id gracza] [ilo��]");

	GiveScore(forplayerid, money);
	format(string, sizeof string, "Przekaza�e� {b}%d{/b} punkt�w graczowi {b}%s{/b}.", money, PlayerName(forplayerid));
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "Otrzyma�e� {b}%d{/b} punkt�w od administratora {b}%s{/b}.", money, PlayerName(playerid));
	Msg(forplayerid, COLOR_INFO, string);
	return 1;
}

CMD:resetmoney(playerid, params[])
{
	new forplayerid, string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnie�.");

	if(sscanf(params, "d", forplayerid))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /resetmoney [id gracza]");

	ResetMoney(forplayerid);
	format(string, sizeof string, "Zresetowa�e� pieni�dze graczowi {b}%s{/b}.", PlayerName(forplayerid));
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "Twoje pieni�dze zosta�y zresetowane przez administratora {b}%s{/b}.", PlayerName(playerid));
	Msg(forplayerid, COLOR_INFO, string);
	return 1;
}

CMD:resetscore(playerid, params[])
{
	new forplayerid, string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnie�.");

	if(sscanf(params, "d", forplayerid))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /resetscore [id gracza]");

	ResetScore(forplayerid);
	format(string, sizeof string, "Zresetowa�e� punkty graczowi {b}%s{/b}.", PlayerName(forplayerid));
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "Twoje punkty zosta�y zresetowane przez administratora {b}%s{/b}.", PlayerName(playerid));
	Msg(forplayerid, COLOR_INFO, string);
	return 1;
}

CMD:givemoneyall(playerid, params[])
{
	new money, string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnie�.");

	if(sscanf(params, "d", money))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /givemoneyall [ilo��]");

	Loop(playeri, MAX_PLAYERS)
	{
		if(IsPlayerLogged(playeri))
		{
			GiveMoney(playeri, money);
		}
	}

	format(string, sizeof string, "Wszyscy otrzymali {b}$%d{/b} od administratora {b}%s{/b}.", money, PlayerName(playerid));
	MsgToAll(COLOR_INFO, string);
	return 1;
}

CMD:givescoreall(playerid, params[])
{
	new score, string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnie�.");

	if(sscanf(params, "d", score))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /givescoreall [ilo��]");

	Loop(playeri, MAX_PLAYERS)
	{
		if(IsPlayerLogged(playeri))
		{
			GiveScore(playeri, score);
		}
	}

	format(string, sizeof string, "Wszyscy otrzymali {b}%d{/b} punkt(�w) od administratora {b}%s{/b}.", score, PlayerName(playerid));
	MsgToAll(COLOR_INFO, string);
	return 1;
}

CMD:s(playerid, params[])
{
	new string[128];

	if(isnull(params)) return Msg(playerid, COLOR_ERROR, "Wpisz: /s [tekst]");

	Loop(player, MAX_PLAYERS)
	{
		if(GetDistancePlayerToPlayer(playerid, player) < 30)
		{
			format(string, sizeof string, "%s m�wi: %s", PlayerName(playerid), params);
			SendClientMessage(player, -1, string);
		}
	}
	return 1;
}

CMD:me(playerid, params[])
{
	new string[128];

	if(isnull(params)) return Msg(playerid, COLOR_ERROR, "Wpisz: /me [czynno��]");

	Loop(player, MAX_PLAYERS)
	{
		if(GetDistancePlayerToPlayer(playerid, player) < 30)
		{
			format(string, sizeof string, "** {b}%s %s{/b}", PlayerName(playerid), params);
			Msg(player, COLOR_INFO2, string, true, false);
		}
	}
	return 1;
}

CMD:zw(playerid, params[])
{
	new string[176];

	if(GetPVarInt(playerid, "AFK"))
		return Msg(playerid, COLOR_INFO, "Aktualnie posiadasz status {b}zaraz wracam{/b}.");

	SetPVarInt(playerid, "AFK", 1);

	if(isnull(params))
	{
		format(string, sizeof(string), "Gracz {b}%s{/b} zmieni� status na {b}zaraz wracam{/b}.",PlayerName(playerid));
		MsgToAll(COLOR_INFO2, string);
	}
	else
	{
		format(string, sizeof(string), "Gracz {b}%s{/b} zmieni� status na {b}zaraz wracam{/b}. Pow�d: {b}%s{/b}",PlayerName(playerid), params);
		MsgToAll(COLOR_INFO2, string);
	}
	return 1;
}

CMD:jj(playerid, params[])
{
	new string[176];

	if(!GetPVarInt(playerid, "AFK"))
		return Msg(playerid, COLOR_INFO, "Aktualnie posiadasz status {b}ju� jestem{/b}.");

	DeletePVar(playerid, "AFK");

	format(string, sizeof(string), "Gracz {b}%s{/b} zmieni� status na {b}ju� jestem{/b}.",PlayerName(playerid));
	MsgToAll(COLOR_INFO2, string);
	return 1;
}

CMD:wc(playerid, params[])
{
	new string[176];

	if(GetPVarInt(playerid, "AFK"))
		return Msg(playerid, COLOR_INFO, "Aktualnie posiadasz status {b}zaraz wracam{/b}.");

	SetPVarInt(playerid, "AFK", 1);
	format(string, sizeof(string), "Gracz {b}%s{/b} idzie do toalety.", PlayerName(playerid));
	MsgToAll(COLOR_INFO2, string);
	return 1;
}

CMD:siema(playerid, params[])
{
	new string[176];

	format(string, sizeof(string), "Gracz {b}%s{/b} wita si� z wszystkimi.", PlayerName(playerid));
	MsgToAll(COLOR_INFO2, string);
	return 1;
}

CMD:nara(playerid, params[])
{
	new string[176];

	format(string, sizeof(string), "Gracz {b}%s{/b} �egna si� z wszystkimi.", PlayerName(playerid));
	MsgToAll(COLOR_INFO2, string);
	return 1;
}

CMD:pw(playerid, params[])
	return cmd_pm(playerid, params);

CMD:pm(playerid, params[])
{
	new 
		forplayerid,
		Message[256],
		string[312];

	if(sscanf(params, "ds[76]", forplayerid, Message))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /pm [id] [tre��]");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie jest pod��czony.");

	if(playerid == forplayerid)
		return Msg(playerid, COLOR_ERROR, "Nie mo�esz napisa� do siebie.");

	if(GetPVarInt(forplayerid, "PMOFF"))
		return Msg(playerid, COLOR_ERROR, "Ten gracz wy��czy� prywatne wiadomo�ci.");

	format(string, sizeof(string),"PM od {b}%s{/b} (%d): %s",PlayerName(playerid),playerid, Message);
	Msg(forplayerid, COLOR_INFO, string);
	format(string, sizeof(string),"PM do {b}%s{/b} (%d): %s",PlayerName(forplayerid),forplayerid, Message);
	Msg(playerid, COLOR_INFO, string);

	ToLog(playerInfo[playerid][pID], LOG_TYPE_CHAT, "pm", string);

	return 1;
}

CMD:tog(playerid, params[])
{
	if(strcmp(params, "pw", true) == 0)
	{
		if(GetPVarInt(playerid, "PMOFF"))
		{
			DeletePVar(playerid, "PMOFF");
			Msg(playerid, COLOR_INFO, "Prywatne wiadomo�ci zosta�y {b}w��czone{b}.");
		}
		else
		{
			SetPVarInt(playerid, "PMOFF", 1);
			Msg(playerid, COLOR_INFO, "Prywatne wiadomo�ci zosta�y {b}wy��czone{b}.");
		}
	}
	else if(strcmp(params, "cb", true) == 0)
	{
		if(GetPVarInt(playerid, "CBOFF"))
		{
			DeletePVar(playerid, "CBOFF");
			Msg(playerid, COLOR_INFO, "CB Radio zosta�o {b}w��czone{b}.");
		}
		else
		{
			SetPVarInt(playerid, "CBOFF", 1);
			Msg(playerid, COLOR_INFO, "CB Radio zosta�o {b}wy��czone{b}.");
		}
	}
	else
		Msg(playerid, COLOR_ERROR, "/tog [pw/cb]");

	return 1;
}


CMD:clear(playerid, params[])
{
	new string[128];

	if(playerInfo[playerid][pAdmin])
	{
		for(new i = 0 ; i <= 100 ; i++)
		{
			SendClientMessageToAll(0x0," ");
		}

		format(string, sizeof string, "Czat zosta� oczyszczony przez {b}%s{/b}.",PlayerName(playerid));
		MsgToAll(COLOR_INFO3, string);
	} 
	else 
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnie�.");
	return 1;
}

CMD:cb(playerid, params[])
{
	new string[176];

	if(isnull(params))	
		return Msg(playerid, COLOR_ERROR, "Wpisz: /cb [tekst]");

	if(!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie znajdujesz si� w poje�dzie.");

	Loop(player, MAX_PLAYERS)
	{
		if(IsPlayerInAnyVehicle(player) && GetPlayerChannelCB(player) == GetPlayerChannelCB(playerid) && !playerInfo[player][pAdmin])
		{
			format(string, sizeof string, "{408080}[CB: %d]: {FFFFFF}%s", GetPlayerChannelCB(playerid), params);
			SendClientMessage(player, 0x0, string);
		}
		else if(playerInfo[player][pAdmin])
		{
			format(string, sizeof string, "{408080}[CB: %d] (%s, %d): {FFFFFF}%s", GetPlayerChannelCB(playerid), PlayerName(playerid), playerid, params);
			SendClientMessage(player, 0x0, string);
		}
	}

	ToLog(playerInfo[playerid][pID], LOG_TYPE_CHAT, "cb", params);

	return 1;
}

CMD:cbkanal(playerid, params[])
{
	if(isnull(params))
	{
		Msg(playerid, COLOR_ERROR, "Wpisz: /cbkanal [kana�]");
	}
	else
	{
		new input = strval(params);
		if(input < 5 || input > 30)
		{
			Msg(playerid, COLOR_ERROR, "Wpisz: /cbkanal [5 - 30]");
		}
		else
		{
			SetPlayerChannelCB(playerid, input);
			Msg(playerid, COLOR_INFO, "Zmieni�e� kana� CB Radia.");
		}
	}
	return 1;
}

CMD:mute(playerid, params[])
{
	new forplayerid, czas, reason[20], string[256];

	if(sscanf(params, "dds[20]", forplayerid, czas, reason))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /mute [id gracza] [czas] [pow?]");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Gracz ten nie jest obecny na serwerze.");

	if(GetPVarInt(forplayerid, "Mute"))
		return Msg(playerid, COLOR_ERROR, "Gracz ten jest uciszony.");

	if(playerInfo[playerid][pAdmin])
	{
		SetPVarInt(forplayerid, "Mute", 1);
		SetPVarInt(forplayerid, "Mutetime", czas*60);
		SetTimerEx("Unmute", czas*(60), false, "d", forplayerid);
		format(string, sizeof string, "Gracz {b}%s{/b} zosta� wyciszony na %d minut przez {b}%s{/b} z powodu {b}%s{/b}.", PlayerName(forplayerid), czas, PlayerName(playerid), reason);
		MsgToAll(COLOR_INFO2, string);
	}
	else 
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnie�");
	return 1;
}

CMD:unmute(playerid, params[])
{
	new forplayerid, string[158];

	if(sscanf(params, "d", forplayerid))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /unmute [id gracza]");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Gracz ten nie jest obecny na serwerze.");

	if(GetPVarInt(forplayerid, "Mute") == 0)
		return Msg(playerid, COLOR_ERROR, "Gracz ten nie jest uciszony.");

	if(playerInfo[playerid][pAdmin])
	{
		DeletePVar(forplayerid, "Mute");
		DeletePVar(forplayerid, "Mutetime");
		format(string, sizeof string, "Gracz {b}%s{/b} zosta� odciszony przez {b}%s{/b}.", PlayerName(forplayerid), PlayerName(playerid));
		MsgToAll(COLOR_INFO2, string);
	}
	else 
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnie�");
	return 1;
}

CMD:ochrona(playerid, params[])
{
	new string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnie�.");

	if(SECURITYON == 1)
	{
		SECURITYON = 0;
		format(string, sizeof string, "Ochrona serwera zosta�a {b}wy��czona{/b} przez %s.", PlayerName(playerid));
		MsgToAll(COLOR_INFO2, string);
	}
	else
	{
		format(string, sizeof string, "Ochrona serwera zosta� {b}w��czona{/b} przez %s.", PlayerName(playerid));
		MsgToAll(COLOR_INFO2, string);
		SECURITYON = 1;
	}
	return 1;
}

CMD:report(playerid, params[])
{
	new forplayerid, reason[76], szString[148];

	if(sscanf(params, "ds[76]", forplayerid, reason))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /report [id gracza] [tre��]");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Gracz ten nie jest obecny na serwerze.");

	if(strlen(reason) > 76 || strlen(reason) < 5)
		return Msg(playerid, COLOR_ERROR, "Wprowadzi�e� nieprawid�owy pow�d.");

	if(playerInfo[forplayerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie mo�esz zg�osi� administratora.");

	for(new i = 0; i < MAX_PLAYERS*2; i++)
	{
		if(repInfo[i][repUsed])
			continue;

		repInfo[i][repUsed] = true;
		repInfo[i][repPReport] = playerid;
		repInfo[i][repPReported] = forplayerid;
		strcat(repInfo[i][repReason], reason);
		format(szString, sizeof szString, "Gracz {b}%s{/b} zg�osi� gracza {b}%s{/b}. Wpisz /reports, aby zobaczy� zg�oszenia.", PlayerName(playerid), PlayerName(forplayerid));

		for(new d = 0; d < MAX_PLAYERS; d++)
		{
			if(!IsPlayerConnected(d) || !IsPlayerLogged(d))
				continue;

			if(playerInfo[d][pAdmin] >= 1)
				Msg(d, COLOR_ERROR, szString);
		}
		break;
	}

	format(szString, sizeof szString, "Zg�osi�e� gracza {b}%s{/b}.", PlayerName(forplayerid));
	Msg(playerid, COLOR_INFO, szString);
	return 1;
}

CMD:reports(playerid)
{
	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnie�.");
	new count;
	for(new i = 0; i < MAX_PLAYERS*2; i++)
	{
		if(!repInfo[i][repUsed])
			continue;
		count++;
	}
	if(count < 1)
		return Msg(playerid, COLOR_ERROR, "Brak zg�osze�.");

	new szString[512];
	for(new c = 0; c < MAX_PLAYERS*2; c++)
	{
		if(!repInfo[c][repUsed])
			continue;
		format(szString, sizeof szString, "{FFFFFF}%d\t%s\t%s\t{FF0000}%s\n", c, PlayerName(repInfo[c][repPReport]), PlayerName(repInfo[c][repPReported]), repInfo[c][repReason]);
	}
	strins(szString, "ID\tZg�aszaj�cy\tZg�aszany\tPow�d\n", 0);
	Dialog_Show(playerid, DIALOG_REPORT, DIALOG_STYLE_TABLIST_HEADERS, "Zg�oszenia", szString, "Usu�", "Wyjd�");
	return 1;
}

Dialog:DIALOG_REPORT(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new id;
	sscanf(inputtext, "d{s[64]}", id);
	repInfo[id][repUsed] = false;

	return cmd_reports(playerid);
}

CMD:przelej(playerid, params[])
{
	new forplayerid, kwota, string[128];

	if(sscanf(params, "dd", forplayerid, kwota))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /przelej [id gracza] [kwota]");

	if(kwota > GetMoney(playerid))
		return Msg(playerid, COLOR_ERROR, "Podana kwota jest zbyt wysoka.");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie jest obecny na serwerze.");

	if(kwota <= 0)
		return Msg(playerid, COLOR_ERROR, "Podana kwota jest zbyt niska.");

	GiveMoney(playerid, -kwota);
	GiveMoney(forplayerid, kwota);
	format(string, sizeof string, "Przekaza�e� {b}$%d{/b} graczowi {b}%s{/b}.", kwota, PlayerName(forplayerid));
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "Otrzyma�e� {b}$%d{/b} od gracza {b}%s{/b}.", kwota, PlayerName(playerid));
	Msg(forplayerid, COLOR_INFO, string);
	return 1;
}

CMD:admins(playerid, params[])
{
	new admin = 0,
	string[50];

	Msg(playerid, COLOR_INFO, "Administratorzy on-line:");
	Loop(player, MAX_PLAYERS)
	{
		if(IsPlayerConnected(player) && playerInfo[player][pAdmin] && !GetPVarInt(player, "HIDEME") && !GetPVarInt(player, "AFK") && !GetPVarInt(player, "isAFK"))
		{
			format(string, sizeof string, "- %s %s", PlayerName(player), (playerInfo[player][pAdmin] > 1) ? ("(RCON)") : (""));
			Msg(playerid, COLOR_INFO, string);
			admin++;
		}
		else if(IsPlayerConnected(player) && playerInfo[player][pAdmin] && !GetPVarInt(player, "HIDEME") && (GetPVarInt(player, "AFK") || GetPVarInt(player, "isAFK") == 1))
		{
			format(string, sizeof string, "- {b}%s %s{/b}", PlayerName(player), (playerInfo[player][pAdmin] > 1) ? ("(RCON)") : (""));
			Msg(playerid, COLOR_INFO, string);
			admin++;
		}
	}
	if(admin == 0)
		Msg(playerid, COLOR_INFO, "Brak administrator�w online.");

	return 1;
}

CMD:hideme(playerid, params[])
{
	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnie�.");

	if(GetPVarInt(playerid, "HIDEME"))
		return Msg(playerid, COLOR_ERROR, "Aktualnie jeste� ju� {b}ukryty{/b}.");

	SetPVarInt(playerid, "HIDEME", 1);
	Msg(playerid, COLOR_INFO, "Zosta� {b}ukryty{/b} na li�cie administrator�w.");
	return 1;
}

CMD:showme(playerid, params[])
{
	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnie�.");

	if(!GetPVarInt(playerid, "HIDEME"))
		return Msg(playerid, COLOR_ERROR, "Aktualnie nie jeste� {b}ukryty{/b}.");

	DeletePVar(playerid, "HIDEME");
	Msg(playerid, COLOR_INFO, "Jeste� {b}widoczny{/b} na li�cie administrator�w.");
	return 1;
}

CMD:dodajbar(playerid, params[])
{
	//if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Chwilowo niedost�pne.");

	/*new interior, string[128];

	if(sscanf(params, "d", interior))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /dodajbar [interior]");

	new postawionyID = GetBar(playerid);
	if(postawionyID > INVALID_BARY_ID)
	{
		format(string, sizeof string, "Bar zosta� Ju� utworzony, uid: %d.", postawionyID);
		SendClientMessage(playerid,LIGHTRED,string);
		return 1;
	}

	if(!IsValidBarInterior(interior))
	{
		Msg(playerid, COLOR_ERROR, "Poda?niepoprawny interior.");
		Msg(playerid, COLOR_ERROR, "Prawid?we ID interior?: 1, 4, 5, 6, 9, 10, 17.");
		return 1;
	}

	new Float:Pos[3];
	GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);

	format(string, sizeof string, "INSERT INTO `BARY` VALUES('', '%f', '%f', '%f', '%d', '3000')", Pos[0], Pos[1], Pos[2], interior);
	mysql_query(string);

	new barid = mysql_insert_id();
	if(barid == -1)
		return Msg(playerid, COLOR_ERROR, "Wys?pi? nieoczekiany b??d.");

	format(string, sizeof string, "{004080}BAR {004080}[VID: {FFFFFF}%d{004080}]\n{FFFFFF}/wejdz", barid);
	bary3D[barid] = CreateDynamic3DTextLabel(string, ZIELONY4, Pos[0], Pos[1], Pos[2]+0.5, 30.0);
	baryIKON[barid] = CreateDynamicMapIcon(Pos[0], Pos[1], Pos[2], 10, LIGHTRED);

	format(string, sizeof string, "{008000}bar o uid: {FFFFFF}%d {008000}zosta�pomy�lnie utworzony.", barid);
	SendClientMessage(playerid,0x0,string);
	return 1;*/
}

CMD:usunbar(playerid, params[])
{
	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnie�.");

	new string[132];

	new barid = GetBar(playerid);
	if(barid == INVALID_BARY_ID)
	return Msg(playerid, COLOR_ERROR, "Nie znajdujesz si� przy barze.");

	format(string, sizeof string, "{008000}Bar o uid: {FFFFFF}%d {008000}zosta� pomy�lnie usuni�ty.", barid);
	SendClientMessage(playerid,0x0,string);

	DestroyDynamic3DTextLabel(bary3D[barid]);
	DestroyDynamicMapIcon(baryIKON[barid]);
	format(string, sizeof string, "DELETE FROM `BARY` WHERE `ID`= '%d'", barid);
	mysql_query(string);

	return 1;
}

CMD:wejdz(playerid, params[])
{
	new barid = GetBar(playerid);
	if(barid == INVALID_BARY_ID)
	return Msg(playerid, COLOR_ERROR, "Nie znajdujesz si� przy barze.");

	new barinterior = GetBarInterior(barid);
	new Float:Pos[3];
	GetPosBarInterior(barinterior, Pos[0], Pos[1], Pos[2]);

	SetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);
	SetPlayerInterior(playerid, barinterior);
	SetPlayerVirtualWorld(playerid, barid+1);
	SetPVarInt(playerid, "WBARZE", 1);
	SetPVarInt(playerid, "BARID", barid);
	return 1;
}

CMD:wyjdz(playerid, params[])
{
	if(GetPVarInt(playerid, "WBARZE"))
	{
		new Float:Pos[3];
		new barid = GetPVarInt(playerid, "BARID");
		GetBarPos(barid, Pos[0], Pos[1], Pos[2]);

		DeletePVar(playerid, "WBARZE");
		DeletePVar(playerid, "BARID");
		SetPlayerInterior(playerid, 0);
		SetPlayerVirtualWorld(playerid, 0);
		SetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);
	}
	return 1;
}

CMD:odczep(playerid, params[])
{
	new 
		State = GetPlayerState(playerid),
		trailerid,
		odczep;

	if(!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie znajdujesz si� w poje�dzie.");

	if(State != PLAYER_STATE_DRIVER)
		return Msg(playerid, COLOR_ERROR, "Nie siedzisz za kierownic�.");

	trailerid = GetPlayerVehicleID(playerid);
	odczep = GetVehicleTrailer(trailerid);
	AttachTrailerToVehicle(trailerid, odczep);
	DetachTrailerFromVehicle(trailerid);
	Msg(playerid, COLOR_INFO, "Naczepa zosta�a odczepiona.");
	return 1;
}

CMD:jetpack(playerid, params[])
{
	new forplayerid, string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnie�.");

	if(sscanf(params, "d", forplayerid))
		return Msg(playerid, COLOR_ERROR, "Wpisz: {b}/jetpack [id gracza]{/b}");

	SetPlayerSpecialAction(forplayerid, 2);
	format(string, sizeof string, "Da�e� jetpack graczowi %s.", PlayerName(forplayerid));
	Msg(playerid, COLOR_INFO, string);

	format(string, sizeof string, "Otrzyma�e� {b}jetpacka{/b} od administratora {b}%s{/b}.", PlayerName(playerid));
	Msg(playerid, COLOR_INFO, string);
	return 1;
}

CMD:dodajbarmenu(playerid, params[])
{
	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnie�.");

	new string[128];

	new postawionyID = GetBaryIn(playerid);
	if(postawionyID > INVALID_BARYIN_ID)
	{
		format(string, sizeof string, "Bar zosta� ju� utworzony, uid: %d.", postawionyID);
		SendClientMessage(playerid,LIGHTRED,string);
		return 1;
	}

	new Float:Pos[3];
	GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);

	format(string, sizeof string, "INSERT INTO `BARYIN` VALUES('', '%f', '%f', '%f')", Pos[0], Pos[1], Pos[2]);
	mysql_query(string);

	new baryinid = mysql_insert_id();
	format(string, sizeof string, "{400040}Bar [VID: {FFFFFF}%d{400040}]\n{FFFFFF}/menu", baryinid);
	baryin3D[baryinid] = CreateDynamic3DTextLabel(string, ZIELONY4, Pos[0], Pos[1], Pos[2]+0.5, 30.0);

	format(string, sizeof string, "{008000}Bar o uid: {FFFFFF}%d {008000}zosta� pomy?lnie utworzony.", baryinid);
	SendClientMessage(playerid,0x0,string);
	return 1;
}

CMD:usunbarmenu(playerid, params[])
{
	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnie�.");

	new string[132];

	new baryinid = GetBaryIn(playerid);
	if(baryinid == INVALID_BARYIN_ID)
		return Msg(playerid, COLOR_ERROR, "Nie znajdujesz si� przy barze.");

	format(string, sizeof string, "{008000}Bar o uid: {FFFFFF}%d {008000}zosta�pomy�lnie usuni?y.", baryinid);
	SendClientMessage(playerid,0x0,string);

	DestroyDynamic3DTextLabel(baryin3D[baryinid]);
	format(string, sizeof string, "DELETE FROM `BARYIN` WHERE `ID`= '%d'", baryinid);
	mysql_query(string);

	return 1;
}

CMD:menu(playerid, params[])
{
	new baryinid = GetBaryIn(playerid);
	if(baryinid == INVALID_BARYIN_ID)
		return Msg(playerid, COLOR_ERROR, "Nie znajdujesz si� w barze.");

	new x[300];

	strcat(x, "{FFFFFF}Serwer-Truck SAMP - Bar.\n");
	strcat(x, " \n");
	strcat(x, "{0080C0}Obiad {FFFFFF}(250$): {808080}100%. \n");
	strcat(x, "{0080C0}�niadanie {FFFFFF}(200$): {808080}70%. \n");
	strcat(x, "{0080C0}Kolacja {FFFFFF}(150$): {808080}50%. \n");
	strcat(x, "{0080C0}Deser {FFFFFF}(100$): {808080}25%. \n");
	strcat(x, "  \n");
	strcat(x,"{808080}Kliknij, aby wybra�. \n");
	strcat(x, "  \n");
	strcat(x,"{FFFFFF}www.serwertruck.eu");
	Dialog_Show(playerid, DIALOG_MENU_BAR, DIALOG_STYLE_LIST, " ", x, "OK", "Wyjd?");
	return 1;
}

CMD:uping(playerid, params[])
{
	new forplayerid, high, string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnie�.");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie jest obecny na serwerze..");

	if(sscanf(params, "dd", forplayerid, high))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /uping [id gracza] [wysoko��]");

	new Float:t[3];
	new vehicleid = GetPlayerVehicleID(forplayerid);
	GetPlayerPos(forplayerid, t[0], t[1], t[2]);
	format(string, sizeof string, "Zmieni�e� pozycj� graczowi {b}%s{/b}.", PlayerName(forplayerid));
	Msg(playerid, COLOR_INFO, string);

	format(string, sizeof string, "Twoja pozycja zosta� zmieniona przez {b}%s{/b}.",PlayerName(playerid));
	Msg(playerid, COLOR_INFO, string);

	if(IsPlayerInAnyVehicle(forplayerid))
	{
		SetVehiclePos(vehicleid, t[0], t[1], t[2]+high);
		SetPlayerPos(forplayerid, t[0], t[1], t[2]+high);
		PutPlayerInVehicle(forplayerid, vehicleid, 0);
	}
	else
	{
		SetPlayerPos(forplayerid, t[0], t[1], t[2]+high);
	}
	return 1;
}

CMD:downing(playerid, params[])
{
	new forplayerid, high, string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnie�.");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie jest obecny na serwerze..");

	if(sscanf(params, "dd", forplayerid, high))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /downing [id gracza] [wysoko��]");

	new Float:t[3];
	new vehicleid = GetPlayerVehicleID(forplayerid);
	GetPlayerPos(forplayerid, t[0], t[1], t[2]);
	format(string, sizeof string, "Zmieni�e� pozycj� graczowi {b}%s{/b}.", PlayerName(forplayerid));
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "Twoja pozycja zosta� zmieniona przez {b}%s{/b}.",PlayerName(playerid));
	Msg(playerid, COLOR_INFO, string);

	if(IsPlayerInAnyVehicle(forplayerid))
	{
		SetVehiclePos(vehicleid, t[0], t[1], t[2]-high);
		SetPlayerPos(forplayerid, t[0], t[1], t[2]-high);
		PutPlayerInVehicle(forplayerid, vehicleid, 0);
	}
	else
	{
		SetPlayerPos(forplayerid, t[0], t[1], t[2]-high);
	}
	return 1;
}

forward SprawdzPoziom(playerid);
public SprawdzPoziom(playerid)
{
	new levelChange;
	new score = GetScore(playerid);
	new string[156];

	if(score < 0)
	{
		ResetScore(playerid);
		return 1;
	}

	if(GetPVarInt(playerid, "LEVEL") < GetPlayerLevel(score))
	{
		if(!GetPVarInt(playerid, "JOIN"))
		{
			levelChange = GetPlayerLevel(score) - GetPVarInt(playerid, "LEVEL");
			format(string, sizeof string, "Gratulacje! Gracz {b}%s{/b} awansuje na {b}%d{/b} poziom ({b}%s{/b})!", PlayerName(playerid), GetPlayerLevel(score), GetPlayerLevelName(GetPlayerLevel(score)));
			MsgToAll(COLOR_INFO2, string);

			format(string, sizeof string, "Gratulacje! Awansujesz na {b}%d{/b} poziom ({b}%s{/b}). W nagrod� otrzymujesz {b}$%d{/b}.", GetPlayerLevel(score), GetPlayerLevelName(GetPlayerLevel(score)), levelChange*2500);
			Msg(playerid, COLOR_INFO2, string);

			GiveMoney(playerid, levelChange*2500);
			pPoints[playerid][pAllowedPoints] += (1*levelChange);
		}

		SetPVarInt(playerid, "LEVEL", GetPlayerLevel(score));
		SprawdzPoziom(playerid);
	}
	else if(GetPVarInt(playerid, "LEVEL") > GetPlayerLevel(score))
	{
		levelChange = GetPVarInt(playerid, "LEVEL") - GetPlayerLevel(score);

		format(string, sizeof string, "Gracz {b}%s{/b} spada na {b}%d{/b} poziom ({b}%s{/b})!", PlayerName(playerid), GetPlayerLevel(score), GetPlayerLevelName(GetPlayerLevel(score)));
		MsgToAll(COLOR_INFO2, string);

		format(string, sizeof string, "Niestety :(. Spadasz na {b}%d{/b} poziom ({b}%s{/b}).", GetPlayerLevel(score), GetPlayerLevelName(GetPlayerLevel(score)));
		Msg(playerid, COLOR_INFO2, string);

		GiveMoney(playerid, -2500*levelChange);

		SetPVarInt(playerid, "LEVEL", GetPlayerLevel(score));
		SprawdzPoziom(playerid);
	}

	format(string,sizeof string,"~b~POZIOM:~w~ %d", GetPVarInt(playerid, "LEVEL"));
	PlayerTextDrawSetString(playerid, levelTD[0][playerid], string);
	format(string,sizeof string,"~b~%d/%d", score, DoswiadczeniePoziomy[GetPVarInt(playerid, "LEVEL")+1]);
	PlayerTextDrawSetString(playerid, levelTD[1][playerid], string);
	return 1;
}

CMD:ladownosc(playerid, params[])
{
	new vehicleid = GetPlayerVehicleID(playerid);
	new trailerid = GetVehicleTrailer(vehicleid);
	new string[126];

	if(!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie znajdujesz si� w poje�dzie.");

	new model = GetVehicleModel(vehicleid);

	if(model == 515 || model == 403 || model == 514)
	{
		if(!GetVehicleTrailer(vehicleid))
			return Msg(playerid, COLOR_ERROR, "Nie posiadasz naczepy.");

		format(string, sizeof string, "Ten pojazd/naczepa mie�ci maksymalnie {b}%d{/b} kg.", MaxWeight(GetVehicleModel(trailerid)));
		Msg(playerid, COLOR_ERROR, string);
	}
	else
	{
		if(!IsVehicleVan(model))
			return Msg(playerid, COLOR_ERROR, "Ten pojazd nie nadaje si� do przewo�enia towar�w.");
		format(string, sizeof string, "Ten pojazd/naczepa mie�ci maksymalnie {b}%d{/b} kg.", MaxWeight(GetVehicleModel(vehicleid)));
		Msg(playerid, COLOR_ERROR, string);
	}
	return 1;
}

CMD:open(playerid, params[])
{
	if(playerInfo[playerid][pFirm] == 0 || !GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnie�.");

	Otworzbrame(playerid);
	Msg(playerid, COLOR_INFO, "Brama zosta�a otwarta.");
	return 1;
}

CMD:close(playerid, params[])
{
	if(playerInfo[playerid][pFirm] == 0 || !GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnie�.");

	Zamknijbrame(playerid);
	Msg(playerid, COLOR_INFO, "Brama zosta�a zamkni�ta.");
	return 1;
}

CMD:o(playerid, params[])
{
	if(firmInfo[playerInfo[playerid][pFirm]][tType] != TEAM_TYPE_MEDIC)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnie�.");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz by� na s�u�bie.");

	new gate = strval(params);
	if(gate < 1 || gate > 8)
		return Msg(playerid, COLOR_ERROR, "Poda�e� z�e ID bramy.");

	static Float:gatePos[][3] = {
		{4.0, 1265.2001953125, 761.2998046875},
		{5.0, 1265.0, 746.599609375},
		{6.0, 1265.0, 731.900390625},
		{7.0, 1265.0, 717.2001953125},
		{8.0, 1242.2998046875, 761.099609375},
		{9.0, 1242.2998046875, 746.5},
		{10.0, 1242.2998046875, 731.900390625},
		{11.0, 1242.2998046875, 717.2001953125}
	};

	MoveDynamicObject(brama[floatround(gatePos[gate-1][0])], gatePos[gate-1][1], gatePos[gate-1][2], 8.1 ,2);
	Msg(playerid, COLOR_INFO, "Gara� zosta� {b}otwarty{/b}.");
	return 1;
}

CMD:c(playerid, params[])
{
	if(firmInfo[playerInfo[playerid][pFirm]][tType] != TEAM_TYPE_MEDIC)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnie�.");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz by� na s�u�bie.");

	new gate = strval(params);
	if(gate < 1 || gate > 8)
		return Msg(playerid, COLOR_ERROR, "Poda�e� z�e ID bramy.");

	static Float:gatePos[][4] = {
		{4.0, 1265.2001953125, 761.2998046875, 11.60000038147},
		{5.0, 1265.0, 746.599609375, 11.60000038147},
		{6.0, 1265.0, 731.900390625, 11.60000038147},
		{7.0, 1265.0, 717.2001953125, 11.60000038147},
		{8.0, 1242.2998046875, 761.099609375, 11.60000038147},
		{9.0, 1242.2998046875, 746.5, 11.60000038147},
		{10.0, 1242.2998046875, 731.900390625, 11.60000038147},
		{11.0, 1242.2998046875, 717.2001953125, 11.60000038147}
	};

	MoveDynamicObject(brama[floatround(gatePos[gate-1][0])], gatePos[gate-1][1], gatePos[gate-1][2], gatePos[gate-1][3], 2);
	Msg(playerid, COLOR_INFO, "Gara� zosta� {b}zamkni�ty{/b}.");
	return 1;
}

CMD:explode(playerid, params[])
{
	new forplayerid;

	if(sscanf(params, "d", forplayerid))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /explode [id gracza]");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie jest obecny na serwerze.");

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnie�");

	new Float:Pos[3];
	GetPlayerPos(forplayerid,Float:Pos[0],Float:Pos[1],Float:Pos[2]);
	CreateExplosion(Float:Pos[0], Float:Pos[1], Float:Pos[2], 2, 50);
	return 1;
}

CMD:ann(playerid, params[])
{
	new czas, tekst[20], string[128];

	if(sscanf(params, "ds[20]", czas, tekst))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /ann [czas] [tekst]");

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnie�");

	format(string,sizeof(string),"~w~%s",tekst);
	GameTextForAll(string,(czas*1000),3);
	return 1;
}

CMD:say(playerid, params[])
{
	new tekst[100], string[200];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnie�.");

	if(sscanf(params, "s[100]", tekst))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /say [tekst]");

	format(string, sizeof(string), "* Administrator: %s", tekst);
	MsgToAll(COLOR_INFO2, string, false);

	ToLog(playerInfo[playerid][pID], LOG_TYPE_CHAT, "adminglobal", params);
	return 1;
}

CMD:wyplata(playerid, params[])
{
	new string[176];

	if(playerInfo[playerid][pFirm] == 0)
		return Msg(playerid, COLOR_ERROR, "Nie pracujesz w �adnej organizacji.");

	if(firmInfo[playerInfo[playerid][pFirm]][tChef] != playerInfo[playerid][pID])
		return Msg(playerid, COLOR_ERROR, "Nie jeste� szefem organizacji.");

	new forplayerid;

	if(sscanf(params, "d", forplayerid))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /wyplata [id gracza]");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie jest obecny na serwerze.");

	if(firmInfo[playerInfo[forplayerid][pFirm]][tID] != firmInfo[playerInfo[playerid][pFirm]][tID])
		return Msg(playerid, COLOR_ERROR, "Nie mo�esz dawa� wyp�aty temu graczowi, poniewa� nie pracuje w twojej organizacji.");

	if(GetPVarInt(forplayerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Nie mo�esz dawa� wyp�aty poniewa� aktualnie ten gracz nie pracuje.");

	new Money, Scpor;

	switch(firmInfo[playerInfo[forplayerid][pFirm]][tType])
	{
		case TEAM_TYPE_POLICE, TEAM_TYPE_POMOC, TEAM_TYPE_MEDIC:
		{
			Money = GetWork(playerid)*78;
			Scpor = floatround(GetWork(playerid)/10);

			if(Money > 80000) Money = 80000;
			if(Scpor > 80) Scpor = 80;
		}

		default:
		{
			Money = (GetWork(playerid)*300);

			if(Money > 25000) Money = 25000;
		}
	}

	GiveMoney(playerid, Money);
	GiveScore(playerid, Scpor);
	ResetWork(playerid);

	format(string, sizeof string, "{C8FF91}Otrzyma�e� wyp�at�\nKwota: {FFFFFF}$%d.\n{C8FF91}Score: {FFFFFF}%d.\n{C8FF91}Szef: {FFFFFF}%s.", Money, Scpor, PlayerName(playerid));
	ShowInfo(forplayerid, string);

	format(string, sizeof string, "{C8FF91}Da�e� wyp�at�\nKwota: {FFFFFF}$%d.\n{C8FF91}Score: {FFFFFF}%d.\n{C8FF91}Gracz: {FFFFFF}%s.", Money, Scpor, PlayerName(forplayerid));
	ShowInfo(playerid, string);


	format(string, sizeof string, "UPDATE `ACOUNT` SET `Worktime` = 0 WHERE `Name` = '%s'", PlayerName(forplayerid));
	mysql_query(string);

	playerInfo[playerid][pWorkTime] = 0;

	return 1;
}

#include "include/gui.inc"
#include "include/commands.inc"
/*

	TODO
	- Poprawka stacji,
	- Roz³adowywanie na stacji,
	- Osi¹gniêcia
	x Poprawa panelu firmowego (ZROBIONE)
	x Poprawa salonu firmowego i prywatnego (ZROBIONE)
	x Dodanie systemu gumy (ZROBIONE)
	- Przepisanie po¿arów
	- Dodanie systemu prywatnych domków
	- Statystyki gracza
	- Zapis wiêzienia (!)
	- Przepisanie GPS
	- Drobna poprawa systemu konwoi
	x Poprawa specowania (ZROBIONE)
	- Dokoñczenie sytuacji losowych
	- Dodanie "psucia siê" pojazdu w zale¿noœci od jego stanu
	- Dodanie systemu dnia i nocy (serwerowy dzieñ)
	- Dodanie ograniczenia wo¿enia towarów w ci¹gu jednego dnia
	- Dodanie kanistra
	- Dodanie sortowania za³adunków
	- Dokoñczenie deski pojazdu
	- Dokoñczenie prawa jazdy
	- Dodanie promu
	- Dodanie tutoriala
	- Tuning dla PD

*/

#include <a_samp>
#include <zcmd>
#include <sscanf2>
#include <streamer>
#include <mysql>
#include "include/lib/djson.inc"
#include "crashdetect"
#include "include/defines.inc"
#include "include/lib/foreach.inc"
#include "GetVehicleColor"
#include "include/lib/timerfix.inc"
#include "include/lib/progressbar2.inc"

#pragma tabsize 0
native WP_Hash(buffer[], len, const str[]);

#define IsPlayerLogged(%1) GetPVarInt(%1, "PlayerLogged")
#define PlayerLogged(%1) SetPVarInt(%1, "PlayerLogged", 1)

#define GiveMoney(%1,%2) playerInfo[%1][pMoney]+=%2,GivePlayerMoney(%1,%2)
#define ResetMoney(%1) playerInfo[%1][pMoney]=0,ResetPlayerMoney(%1)
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
	print(" GameMode Owners - GeDox & Kozak59");
	print("----------------------------------\n");
}

// -----

#include "include/textdraws.inc"
#include "include/lib/easyDialog.inc"
#include "include/lib/j_fader.inc"
#include "include/functions.inc"

// -----

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
	djSetInt("config.json", "full_permission/Kozak59", 1);

	format(gmInfo[adminPass], 32, "haslo_administratora");

	AddPlayerClass(1, -1379.386352, 1488.210449, 21.156248,87.8978, 0, 0, 0, 0, 0, 0);//1

	SetGameModeText("Truck Mode RC 1");
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

	MySQLConnection = mysql_init(LOG_ONLY_ERRORS, 1);
	mysql_connect("mysql-ols1.ServerProject.pl", "db_12517", "yttMDAhyBOvs", "db_12517", MySQLConnection, 1);

	if(mysql_ping(MySQLConnection))
	{
		print("\n[MySQL] Po³¹czenie nieudane, serwer zostanie zablokowany.");
		SendRconCommand("hostname ? [0.3.7] SerwerTruck.eu [PL] ? # B£¥D MYSQL");
		SendRconCommand("mapname # B£¥D MYSQL #");
		SendRconCommand("password $mysql#blad!");

		return 0;
	}
	else
		printf("Nawi¹zano po³¹czenie z baz¹ danych.");

	// Blokada tuningów
	CreateDynamicObject(989,1041.3000000,-1026.0000000,32.6000000,0.0000000,0.0000000,286.7500000); //object(ac_apgate) (1)
	CreateDynamicObject(969,2640.2000000,-2039.1000000,12.4000000,0.0000000,0.0000000,0.0000000); //object(electricgate) (1)
	CreateDynamicObject(971,-1935.8000000,238.8000000,34.7000000,0.0000000,0.0000000,0.0000000); //object(subwaygate) (2)
	CreateDynamicObject(971,-2716.2000000,217.7000000,5.2000000,0.0000000,0.0000000,270.0000000); //object(subwaygate) (3)
	CreateDynamicObject(971,2386.7000000,1043.5000000,10.1000000,0.0000000,0.0000000,0.0000000); //object(subwaygate) (4)

	// Blokada sprejów
	Spray[0] = CreateDynamicObject(3036,2071.7000000,-1829.1000000,14.4000000-7.0,0.0000000,0.0000000,270.0000000); //object(ct_gatexr) (1)
	Spray[1] = CreateDynamicObject(3036,490.7999900,-1735.0000000,11.9000000-7.0,0.0000000,0.0000000,172.0000000); //object(ct_gatexr) (2)
	Spray[2] = CreateDynamicObject(971,-1904.2000000,277.7999900,42.1000000-7.0,0.0000000,0.0000000,0.0000000); //object(subwaygate) (2)
	Spray[3] = CreateDynamicObject(971,-99.8000000,1111.3000000,21.0000000-7.0,0.0000000,0.0000000,0.0000000); //object(subwaygate) (3)
	Spray[4] = CreateDynamicObject(971,1968.1000000,2162.3000000,12.5000000-7.0,0.0000000,0.0000000,270.0000000); //object(subwaygate) (4)
	Spray[5] = CreateDynamicObject(971,-1420.6000000,2591.2000000,57.0000000-7.0,0.0000000,0.0000000,0.0000000); //object(subwaygate) (5)
	Spray[6] = CreateDynamicObject(971,-2425.3999000,1028.3000000,52.2000000-7.0,0.0000000,0.0000000,0.0000000); //object(subwaygate) (7)
	Spray[7] = CreateDynamicObject(971,720.0999800,-462.6000100,15.4000000-7.0,0.0000000,0.0000000,0.0000000); //object(subwaygate) (8)
	Spray[8] = CreateDynamicObject(3036,1022.5000000,-1029.5000000,32.9000000-7.0,0.0000000,0.0000000,0.0000000); //object(ct_gatexr) (4)

	//Policja
	brama[1] = CreateDynamicObject(3055, 2293.8505859375, 2498.8203125, 4.4499998092651, 0, 0, 89.994506835938);	 // brama wjazdowa nr 1
	brama[2] = CreateDynamicObject(3055, 2335.1005859375, 2443.7001953125, 6.9499998092651, 0, 0, 59.990844726563);	 // brama wjazdowa nr 2

	//Pogotowie
	brama[3] = CreateDynamicObject(980, 1269.400390625, 797.0, 12.699999809265, 0, 0, 0);	 // brama wjazdowa nr 1
	brama[4] = CreateDynamicObject(8948, 1265.2001953125, 761.2998046875, 11.60000038147, 0, 0, 0);	 // garaz nr 1
	brama[5] = CreateDynamicObject(8948, 1265.0, 746.599609375, 11.60000038147, 0, 0, 0);	 // garaz nr 2
	brama[6] = CreateDynamicObject(8948, 1265.0, 731.900390625, 11.60000038147, 0, 0, 0);	 // garaz nr 3
	brama[7] = CreateDynamicObject(8948, 1265.0, 717.2001953125, 11.60000038147, 0, 0, 0);	 // garaz nr 4
	brama[8] = CreateDynamicObject(8948, 1242.2998046875, 761.099609375, 11.60000038147, 0, 0, 180.24169921875);	 // garaz nr 5
	brama[9] = CreateDynamicObject(8948, 1242.2998046875, 746.5, 11.60000038147, 0, 0, 180.24169921875);	 // garaz nr 6
	brama[10] = CreateDynamicObject(8948, 1242.2998046875, 731.900390625, 11.60000038147, 0, 0, 180.24169921875);	 // garaz nr 7
	brama[11] = CreateDynamicObject(8948, 1242.2998046875, 717.2001953125, 11.60000038147, 0, 0, 180.24169921875);	 // garaz nr 8

	// Pomoc Drogowa
	brama[13] = CreateDynamicObject(980, 1075.2998046875, 1943.099609375, 12.800000190735, 0, 0, 0);	 // brama wjazdowa nr 1
	brama[14] = CreateDynamicObject(980, 1147.400390625, 2044.0, 12.800000190735, 0, 0, 0);	 // brama wjazdowa nr 2

	// Build Trans
	brama[15] = CreateDynamicObject(980, -168.89999389648, 79.599998474121, 5.0, 0, 0, 340.25);	 // brama wjazdowa

	// Petrol Tank
	brama[16] = CreateDynamicObject(980, 2827.2001953125, 1384.900390625, 12.5, 0, 0, 359.74731445313);	 // brama wjazdowa nr 1
	brama[17] = CreateDynamicObject(980, 2758.0, 1313.400390625, 13.800000190735, 0, 0, 89.49462890625);	 // brama wjazdowa nr 2

	// Cargo Tranzit
	brama[18] = CreateDynamicObject(980, 2478.5, 2513.0, 12.60000038147, 0, 0, 90.0);	 // brama wjazdowa nr 1
	brama[19] = CreateDynamicObject(980, 2527.2783203125, 2424.1005859375, 12.60000038147, 0, 0, 179.99450683594);	 // brama wjazdowa nr 2

	// SM Logistic
	brama[20] = CreateDynamicObject(980, -2606.8000488281, 580.29998779297, 16.200000762939, 0, 0, 180.0);	 // brama wjazdowa nr 1
	brama[21] = CreateDynamicObject(980, -2607.0, 696.70001220703, 29.60000038147, 0, 0, 180.0);	 // brama wjazdowa nr 2

	new CzasLadowania = GetTickCount();
	print("----------");
	print("- Trwa ladowanie bazy danych...");
	LadujStacje();
	LadujBary();
	LadujOrganizacje();
	LoadBTObjects();
	LoadBTLabels();
	//LoadBanWords();
	print("- Ladowanie z bazy danych zakonczone.");
	printf("- Zajelo: %d ms", floatround(GetTickCount()-CzasLadowania));
	print("----------");

	CreateDynamic3DTextLabel(clText(COLOR_INFO2, "Salon pojazdów osobowych\nw {b}San Fierro{/b}.\nWpisz {b}/salon{/b} aby zobaczyæ menu"), -1, -1969.291, 296.353, 35.171, 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1);
	CreateDynamic3DTextLabel(clText(COLOR_INFO2, "Salon pojazdów ciê¿arowych\nw {b}San Fierro{/b}.\nWpisz {b}/salon{/b} aby zobaczyæ menu"), -1, -1649.904, 1209.725, 7.250, 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1);
	CreateDynamicMapIcon(-1649.904, 1209.725, 7.250, 36, 0, -1, -1, -1, 100.0, MAPICON_LOCAL);
	
	TextDrawCreate(0.000000, 0.000000, "_");

	AlertTD = TextDrawCreate(75.000000, 209.000000, "~y~]~b~POLICJA! ~r~PROSZE ZJECHAC NA POBOCZE!~y~]");
	TextDrawBackgroundColor(AlertTD, 255);
	TextDrawFont(AlertTD, 2);
	TextDrawLetterSize(AlertTD, 0.539999, 2.400000);
	TextDrawColor(AlertTD, -1);
	TextDrawSetOutline(AlertTD, 1);
	TextDrawSetProportional(AlertTD, 1);

	TireTD = TextDrawCreate(75.000000, 209.000000, "~b~ZLAPALES GUME");
	TextDrawBackgroundColor(TireTD, 255);
	TextDrawFont(TireTD, 2);
	TextDrawLetterSize(TireTD, 0.539999, 2.400000);
	TextDrawColor(TireTD, -1);
	TextDrawSetOutline(TireTD, 1);
	TextDrawSetProportional(TireTD, 1);

	for (new i = 0; i < MAX_PLAYERS; i++)
		Trucking[i] = Create3DTextLabel(" ", ZIELONY, 0.0, 0.0, 0.0, 30.0, 0, 0);
	
	CreateMainTextDraws(true);
	InitStableHud();
	InitStableTachograph();
	InitStableSpeedometer();
	InitConnect();
	SendRandomMessage();

	mysql_query("SELECT ID FROM Accounts");
	mysql_store_result();
	new nums = mysql_num_rows();
	mysql_free_result();
	for(new i = 1; i < nums+1; i++)
	{
		LadujPojazd(i, 0, _, false);
	}
	printf("[POJAZDY] Zaladowano %d pojazdów graczy.", gmInfo[gmLoadedVehicles]);

	SetTimer_("OneSecTimer", 1000, 100, -1);
	SetTimer_("Refresh", 1000, 50, -1);
	SetTimer_("TachographUpdate", 1000, 0, -1);

	SetTimer_("Update", 100, 0, -1);
	SetTimer_("Wypadek", 250, 0, -1);

	SetTimer_("Jobtime", 2*60000, 0, -1);
	SetTimer_("SaveALL", 60000, 0, -1);

	Crash_OnGameModeInit();

	for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OnGameModeInit", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			CallLocalFunction(szTemp, "");
	}
	print("----------");
	printf("- Ca³kowity czas ³adowania %d ms.", floatround(GetTickCount() - CzasLadowania));
	print("----------");
	return 1;
}

public OnGameModeExit()
{
	djson_GameModeExit();
	CreateMainTextDraws(false);
	SaveBTObjects();

	for(new x; x<MAX_PLAYERS; x++)
		if(camInfo[x][cCameramode] == CAMERA_MODE_FLY) 
			CancelFlyMode(x);

	for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OnGameModeExit", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			CallLocalFunction(szTemp, "");
	}

	foreach (new i : Vehicle)
	{
		if(!IsValidVehicle(i) || !Spawned[i])
			continue;

		SaveVehicle(i);
	}
   	return 1;
}

public OnPlayerConnect(playerid)
{
	gmInfo[gmUsersConnected]++;
	new string[500], ip[16];
	GetPlayerIp(playerid, ip, sizeof(ip));

	DeletePVar(playerid, "otherAFK");
	DeletePVar(playerid, "TELEPORT");
	DeletePVar(playerid, "NEWBIE_SETSPAWN");
	DeletePVar(playerid, "ReSpawn");
	DeletePVar(playerid, "changeColor");
	DeletePVar(playerid, "ReSpawnSkin");
	StopPlayerFade(playerid);
	ResetVariablesInEnum(playerInfo[playerid], E_PLAYER);
	playerInfo[playerid][pChained]=(-1);
	playerInfo[playerid][pMagnes] = false;
	TextDrawHideForPlayer(playerid, AlertTD);

	foreach (new a : Player)
		if(IsPlayerConnected(a))
		{
			if(playerInfo[a][pAdmin])
				format(string, sizeof(string), "Gracz {b}%s{/b} [ID:{b} %d{/b}] [IP:{b} %s{/b}] do³¹czy³ do serwera.", PlayerName(playerid), playerid, ip);
			else
				format(string, sizeof(string), "Gracz {b}%s{/b} [ID:{b} %d{/b}] do³¹czy³ do serwera.", PlayerName(playerid), playerid);
		
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
		format(string, sizeof string, "%s{a9c4e4}Nick admina banuj¹cego: {FFFFFF}%s\n", string, str);

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
		format(string, sizeof string, "%s{a9c4e4}Powód bana: {FFFFFF}%s\n", string, str);

		mysql_fetch_field("IP", str);
		format(string, sizeof string, "%s{a9c4e4}Zbanowane IP: {FFFFFF}%s\n\n", string, str);

		format(string, sizeof string, "%s{a9c4e4}Je¿eli zosta³eœ zbanowany nies³usznie napisz podanie o unbana na forum {FFFFFF}www.serwertruck.eu", string);
		ShowInfo(playerid, string);

		CheatKick(playerid, "aktywny ban");
		timer7[playerid] = SetTimerEx_("Kickplayer", 300, 0, 1, "i", playerid);
	}
	else
	{
		CleanChatForPlayer(playerid, 50);
		TogglePlayerSpectating(playerid, true);
		mysql_free_result();

		SetPVarInt(playerid, "INTRO_Camera", 0);
		CallLocalFunction("CinematicCameraIntro", "i", playerid);
		SelectTextDraw(playerid, 0xA82A23FF);
		ShowPlayerConnect(playerid, true);
		ShowConnect(playerid, true);
		CleanChatForPlayer(playerid, 25);
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

	SetPVarInt(playerid, "ept_fps", 30);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	gmInfo[gmUsersConnected]--;

	new string[256], powod[30], h, m, s, ip[24];

	ConvertMS(GetTickCount()-GetPVarInt(playerid, "IleGral"), h, m, s);
	podgladADMIN[playerid] = false;
	GetPVarString(playerid, "pAJPI", ip, sizeof ip);

	if(playerInfo[playerid][pChained])
		KillTimer(playerInfo[playerid][pChainedTimer]);
	if(!IsPlayerLogged(playerid))
		StopCinematicCameraIntro(playerid);

	if(GetPVarInt(playerid, "Wypadek"))
	{
		StopPlayerFade(playerid);
		FadeColorForPlayer(playerid, 181, 51, 36, 125, 0, 0, 0, 0, 5, 0);
		DeletePVar(playerid, "Wypadek");
		DeletePVar(playerid, "Wypadekzmedykiem");
	}

	Crash_OnPlayerDisconnect(playerid, reason);

	switch(reason)
	{
		case 0: 
			format(powod, sizeof powod, "Timeout/Crash");
		
		case 1: 
			format(powod, sizeof powod, "Wyszed³");
		
		case 2: 
			format(powod, sizeof powod, "Wyrzucony");
	}

	format(string, sizeof(string), "OnPlayerDisconnect (playerUID=%d | reason=%s | timeonline: %dh %dm %ds)", playerInfo[playerid][pID], powod, h, m, s);
	ToLog(playerInfo[playerid][pID], LOG_TYPE_PLAYER, string);

	if(IsPlayerConnected(GetPVarInt(playerid, "jestPrzegladany")))
	{
		TogglePlayerSpectating(GetPVarInt(playerid, "jestPrzegladany"), 0);
		SpectactorTextDraw(GetPVarInt(playerid, "jestPrzegladany"), false);
		DeletePVar(GetPVarInt(playerid, "jestPrzegladany"), "Przeglada");
	}

	TextDrawHideForPlayer(playerid, MainTextDraws[Time]);

	if(IsValidDynamicCP(GetPVarInt(playerid, "trailerCP")))
	{
		DestroyDynamicCP(GetPVarInt(playerid, "trailerCP"));
		DeletePVar(playerid, "trailerCP");
	}

	ShowPlayerHud(playerid, false);
	ShowPlayerConnect(playerid, false);
	ShowConnect(playerid, false);

	foreach (new a : Player)
		if(IsPlayerConnected(a))
		{
			format(string, sizeof(string), "Gracz {b}%s{/b} [ID:{b} %d{/b}] opuœci³ serwer, gra³ {b}%02d:%02d:%02d{/b}. ({b}%s{/b}).", PlayerName(playerid), playerid, h, m, s, powod);		
			Msg(a, COLOR_INFO2, string);
		}

	if(IsValidVehicle(GetPVarInt(playerid, "pojazd")) && (GetPVarInt(playerid, "pojazd") != 0))
		DestroyVehicle(GetPVarInt(playerid, "pojazd"));
	
	KillTimer(timer[playerid]);
	KillTimer(timer3[playerid]);
	KillTimer(timer4[playerid]);
	KillTimer(timer5[playerid]);
	KillTimer_(timer7[playerid]);
	KillTimer(timer8[playerid]);
	KillTimer(timer9[playerid]);
	KillTimer(timer10[playerid]);
	KillTimer(timer11[playerid]);
	KillTimer(timer12[playerid]);
	KillTimer(timer13[playerid]);
	KillTimer(timer15[playerid]);
	KillTimer(timer16[playerid]);
	//KillTimer_(GetPVarInt(playerid, "TireTimer"));
	if(GetPVarInt(playerid, "loading"))
		KillTimer_(GetPVarInt(playerid, "loadTimer"));
	if(GetPVarInt(playerid, "unloading"))
		KillTimer_(GetPVarInt(playerid, "unloadTimer"));

	mysql_real_escape_string(PlayerName(playerid), string);
	format(string, sizeof(string), "UPDATE `Accounts` SET `Online`='0' WHERE `Name` = '%s'", string);
	mysql_query(string);
	SavePlayer(playerid, true);

	SetPVarInt(playerid, "Worked", 0);
	playerInfo[playerid][pAdmin] = 0;
	UpdateOnlineWorkers();
	UpdateAdminsOnline();

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

public OnPlayerRequestClass(playerid, classid)
{
	if(GetPVarInt(playerid, "JOIN"))
	{
		if(GetPVarInt(playerid, "ReSpawn"))
		{
			if(playerInfo[playerid][pSpawn] >= 0)
				SetSpawnInfo(playerid, 0, playerInfo[playerid][pSkin], gSpawnvehicleplayer[playerInfo[playerid][pSpawn]][0], gSpawnvehicleplayer[playerInfo[playerid][pSpawn]][1], gSpawnvehicleplayer[playerInfo[playerid][pSpawn]][2], 0.0, -1, -1, -1, -1, -1, -1);
			else
			{
				new Float:pos[4];
				sscanf(playerInfo[playerid][pSpawnInfo], "p<,>ffff", pos[0], pos[1], pos[2], pos[3]);
				SetSpawnInfo(playerid, 0, playerInfo[playerid][pSkin], pos[0], pos[1], pos[2], pos[3], 0, 0, 0, 0, 0, 0);
			}
			DeletePVar(playerid, "ReSpawn");
			SpawnPlayer(playerid);
		}
	}
}

public OnPlayerRequestSpawn(playerid)
{
	if(GetOnlineTime(playerid) < 7200 && !GetPVarInt(playerid, "Worked"))
		Msg(playerid, COLOR_INFO2, "Witaj! Nie wiesz jak zacz¹æ pracê na SerwerTruck? Wpisz {b}/pojazd{/b} i wybierz coœ dla siebie.");
	if(GetPVarInt(playerid, "ReSpawn"))
	{
		if(playerInfo[playerid][pSpawn] >= 0)
		{
			SetSpawnInfo(playerid, 0, playerInfo[playerid][pSkin], gSpawnvehicleplayer[playerInfo[playerid][pSpawn]][0], gSpawnvehicleplayer[playerInfo[playerid][pSpawn]][1], gSpawnvehicleplayer[playerInfo[playerid][pSpawn]][2], 0.0, -1, -1, -1, -1, -1, -1);
		}
		else
		{
			new Float:pos[4];
			sscanf(playerInfo[playerid][pSpawnInfo], "p<,>ffff", pos[0], pos[1], pos[2], pos[3]);
			SetSpawnInfo(playerid, 0, playerInfo[playerid][pSkin], pos[0], pos[1], pos[2], pos[3], 0, 0, 0, 0, 0, 0);
		}
		DeletePVar(playerid, "ReSpawn");
	}
	return SetPVarInt(playerid, "RequestSpawn", 1);
}

public OnPlayerSpawn(playerid)
{
	new string[128];
	AntiDeAMX();
	if(GetPVarInt(playerid, "RequestSpawn"))
		return DeletePVar(playerid, "RequestSpawn");

	SetPlayerInterior(playerid, 0);
	SetPlayerVirtualWorld(playerid, 0);

	if(GetPVarInt(playerid, "FlyMode"))
	{
		DeletePVar(playerid, "FlyMode");
		SetPlayerPos(playerid, GetPVarFloat(playerid, "BTP"), GetPVarFloat(playerid, "BTP1"), GetPVarFloat(playerid, "BTP2"));
		return 1;
	}

	if(!IsPlayerLogged(playerid))
	{
		CheatKick(playerid, "ominiêcie logowania/rejestacji");
		timer[playerid] = SetTimerEx_("Kickplayer", 300, 0, 1, "i", playerid);
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
	GivePlayerWeapon(playerid, 43, 99999);
	CancelSelectTextDraw(playerid);

	if(GetPVarInt(playerid, "ReSpawnSkin") > 0)
	{
		SetPlayerSkin(playerid, GetPVarInt(playerid, "ReSpawnSkin"));
		DeletePVar(playerid, "ReSpawnSkin");
	}


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

		DeletePVar(playerid, "JOIN");
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
		Msg(playerid, COLOR_INFO, "Stworzony pojazd zosta³ usuniêty.");
		DestroyVehicle(GetPVarInt(playerid, "pojazd"));
		DeletePVar(playerid, "pojazd");
	}

	SetPVarInt(playerid, "JOIN", 1);
	SetPVarInt(playerid, "ReSpawn", 1);
	SetPVarInt(playerid, "ReSpawnSkin", GetPlayerSkin(playerid));

	GivePlayerMoney(playerid, 100);

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
	vloadInfo[vehicleid][vLoaded] = false;
	strdel(vloadInfo[vehicleid][vOwner], 0, strlen (vloadInfo[vehicleid][vOwner]));
	vloadInfo[vehicleid][vCargo] = 0;
	vehicleExploded[vehicleid] = false;
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
	KillTimer_(__FlashTime[vehicleid]);
	ResetVariablesInEnum(vloadInfo[vehicleid], eLoadVehicle);
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

	vloadInfo[vehicleid][vLoaded] = false;
	strdel(vloadInfo[vehicleid][vOwner], 0, strlen (vloadInfo[vehicleid][vOwner]));
	vloadInfo[vehicleid][vCargo] = 0;

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
	new string[512];
	text[0] = toupper(text[0]);

	if(!IsPlayerLogged(playerid))
	{
		Msg(playerid, COLOR_ERROR, "{b}Zaloguj siê{/b}, aby pisaæ na czacie.");
		return 0;
	}

	if(GetPVarInt(playerid, "Mute"))
	{
		Msg(playerid, COLOR_ERROR, "Nie mo¿esz pisaæ na czacie, {b}jesteœ uciszony{/b}.");
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

	if(!strcmp(text, "#", true, 1) && playerInfo[playerid][pFirm] && GetPVarInt(playerid, "Worked"))
	{
		text[1] = toupper(text[1]);
		format(string, sizeof string, "{FF9900}#%s >> %s(id:%d): %s", firmInfo[playerInfo[playerid][pFirm]][tName], PlayerName(playerid), playerid, text[1]);
				
		foreach (new playeri : Player)
			if(playerInfo[playerid][pFirm] == playerInfo[playeri][pFirm] && GetPVarInt(playeri, "Worked"))
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

	if((playerInfo[playerid][pAdmin] <= 0) && (gettime() - GetPVarInt(playerid, "pLastMessageTime")) < 3)
	{
		Msg(playerid, COLOR_ERROR, "Zwolnij z pisaniem. Musisz odczekaæ chwile przed wys³aniem nastêpnej wiadomoœci.");
		return 0;
	}
	SetPVarInt(playerid, "pLastMessageTime", gettime());

	format(string, sizeof(string), "{%06x}%s{4D4D4D} [%d]{FFFFFF}: %s", GetPlayerColor(playerid) >>> 8, PlayerName(playerid), playerid, ColouredText(text));
	SendSplitMessageToAll(-1, string);

	SetPlayerChatBubble(playerid, text, 0xFFFFFFFF, 50, 3000);

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
		Msg(playerid, COLOR_INFO, "Pasy zosta³y {b}odpiête{/b}.");
	}

	if(GetPVarInt(playerid, "Tempomat"))
	{
		KillTimer_(timer7[playerid]);
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
		Msg(playerid, COLOR_ERROR, "Zosta³es postrzelony!");
		TogglePlayerControllable(playerid, false);

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
		new 
			vehicleid = GetPlayerVehicleID(playerid), 
			vehicleUID = DBVehID[GetPlayerVehicleID(playerid)], 
			model = GetVehicleModel(vehicleid);

		if((IsVehicleTruck(model) || IsVehicleVan(model)) == true && (GetOnlineTime(playerid) < 3200))
			Msg(playerid, COLOR_INFO, "Chcesz u¿ywaæ CB-Radia ale nie wiesz jak? To bardzo proste! Ustaw sobiê ksywkê komend¹ {b}/cbksywka{/b}!");

		VehicleDriver[vehicleid] = playerid;

		if(!Spawned[vehicleid])
		{
			if( (vehInfo[vehicleUID][vOwnerType] == OWNER_TYPE_PLAYER && vehInfo[vehicleUID][vOwnerID] != playerInfo[playerid][pID]) ||  
				(vehInfo[vehicleUID][vOwnerType] == OWNER_TYPE_TEAM && IsWorked(playerid, vehInfo[vehicleUID][vOwnerID])) )
			{
				if(vehInfo[vehicleUID][vOwnerType] == OWNER_TYPE_PLAYER && vehInfo[vehicleUID][vOwnerVC] != playerInfo[playerid][pID])
				{
					new Float:Pos[3];
					GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);
					SetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]+2.0);

					Msg(playerid, COLOR_ERROR, "Nie mo¿esz wejœæ do tego pojazdu.");
					return 1;
				}
			}
		}

		ShowPlayerSpeedometer(playerid, true);
		if(IsPlayerInTruck(playerid))
			ShowPlayerTacho(playerid, true);
		format(string, sizeof string, "~r~%s", GetVehicleModelName(GetVehicleModel(vehicleid)));
		PlayerTextDrawSetString(playerid, hudInfo[tdInfoSpeedo][TD_CAR_NAME][playerid], string);

		if(GetPVarType(playerid, "jestPrzegladany") != PLAYER_VARTYPE_NONE)
			PlayerSpectateVehicle(GetPVarInt(playerid, "jestPrzegladany"), vehicleid);
	}

	if(oldstate == PLAYER_STATE_DRIVER)
	{
		ShowPlayerSpeedometer(playerid, false);
		ShowPlayerTacho(playerid, false);
		if(GetPVarType(playerid, "jestPrzegladany") != PLAYER_VARTYPE_NONE)
			PlayerSpectatePlayer(GetPVarInt(playerid, "jestPrzegladany"), playerid);
		if(playerInfo[playerid][pMagnes])
			playerInfo[playerid][pMagnes] = false;

		VehicleDriver[GetPlayerVehicleID(playerid)] = INVALID_PLAYER_ID;
	}
	return 1;
}

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	new Float:health;
	if(GetPlayerWeapon(playerid) != weaponid) Kick(playerid);
	switch(hittype)
	{
		case BULLET_HIT_TYPE_VEHICLE:
		{
				GetVehicleHealth(hitid, health);
				if(health >= 0.0)
				{
					switch(weaponid)
					{
						case 22: SetVehicleHealth(hitid, health -25.0);
						case 23: SetVehicleHealth(hitid, health -40.0);
						case 24: SetVehicleHealth(hitid, health -60.0);
						case 25: SetVehicleHealth(hitid, health -70.0);
						case 26: SetVehicleHealth(hitid, health -80.0);
						case 27: SetVehicleHealth(hitid, health -60.0);
						case 28: SetVehicleHealth(hitid, health -20.0);
						case 29: SetVehicleHealth(hitid, health -25.0);
						case 30: SetVehicleHealth(hitid, health -30.0);
						case 31: SetVehicleHealth(hitid, health -35.0);
						case 32: SetVehicleHealth(hitid, health -20.0);
						case 33: SetVehicleHealth(hitid, health -75.0);
						case 34: SetVehicleHealth(hitid, health -125.0);
						case 35: SetVehicleHealth(hitid, health -1000.0);
						case 36: SetVehicleHealth(hitid, health -1000.0);
						case 37: SetVehicleHealth(hitid, health -200.0);
						case 38: SetVehicleHealth(hitid, health -100.0);
						default: SetVehicleHealth(hitid, health -random(30));
					}
				}
				if(health <= 250.0 && !vehicleExploded[hitid])
				{
					vehicleExploded[hitid] = true;
					new Float:X, Float:Y, Float:Z;
					GetVehiclePos(hitid, X, Y, Z );
					CreateExplosion(X, Y, Z, 1, 5);
				}
		}
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
			new vehicleid = GetPlayerVehicleID(playerid), engine,lights,alarm,doors,bonnet,boot,objective, Float:fuel;
			GetVehicleParamsEx(vehicleid,engine,lights,alarm,doors,bonnet,boot,objective);
			
			if(engine != 1)
			{
				SetVehicleParamsEx(vehicleid,VEHICLE_PARAMS_ON,lights,alarm,doors,bonnet,boot,objective);

				if(GetPVarInt(playerid, "tacho_pauza"))
				{
					DeletePVar(playerid, "tacho_pauza");
					Msg(playerid, COLOR_INFO, "Krêcenie zosta³o pauzy {b}wy³¹czone{/b}.");
				}
				if(brInfo[GetPlayerVehicleID(playerid)][manualBrake])
				{
					Msg(playerid, COLOR_INFO, "Hamulec rêczny zosta³ wy³¹czony.");
					brInfo[GetPlayerVehicleID(playerid)][manualBrake] = false;
				}
				if(Spawned[GetPlayerVehicleID(playerid)])
				{
						fuel = vehInfo_Temp[GetPlayerVehicleID(playerid)][vFuel];
				}
				else
				{
					if(!vehInfo[DBVehID[vehicleid]][vGasStatus])
						fuel = vehInfo[DBVehID[vehicleid]][vFuel];

					else if(vehInfo[DBVehID[vehicleid]][vGasStatus])
						fuel = vehInfo[DBVehID[vehicleid]][vGasAmount];
				}
				if(fuel <= 0)
				{
					fuel = 0;
					SetVehicleParamsEx(vehicleid,VEHICLE_PARAMS_OFF,lights,alarm,doors,bonnet,boot,objective);
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
		PlayerCruiseSpeed[playerid] = 0.00;
		DeletePVar(playerid, "Tempomat");
		Msg(playerid, COLOR_INFO, "Tempomat zosta³ {b}wy³¹czony{/b}.");
	}
	else if(PRESSED( KEY_SUBMISSION | KEY_FIRE ) && IsPlayerInAnyVehicle(playerid) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		new vid = GetPlayerVehicleID(playerid), Float:speed; GetVehicleSpeed(vid, speed);
		if (speed < 30) return Msg(playerid, COLOR_ERROR, "Jedziesz za wolno aby u¿ywaæ tempomatu!");
		new Float:x, Float:y, Float:z;
		GetVehicleVelocity(vid, x, y, z);
		DistanceFlat(0, 0, x, y, PlayerCruiseSpeed[playerid]);
		SetTimerEx("CruiseControl", 500, false, "i", playerid);
		Msg(playerid, COLOR_INFO, "Tempomat zosta³ {b}w³¹czony{/b}.");
		SetPVarInt(playerid, "Tempomat", 1);
	}

	if(PRESSED(KEY_NO) && (GetPlayerState(playerid) == PLAYER_STATE_DRIVER))
	{
		new vehicle = GetPlayerVehicleID(playerid);
		if(Spawned[vehicle])
			return 1;

		if(!vehInfo[DBVehID[vehicle]][vGasBootle])
			return 1;

		if(!vehInfo[DBVehID[vehicle]][vGasStatus])
		{
			Msg(playerid, COLOR_INFO, "Prze³¹czono na gaz.");
			vehInfo[DBVehID[vehicle]][vGasStatus] = true;
		}
		else 
		{
			Msg(playerid, COLOR_INFO, "Prze³¹czono na ropê / benzyne.");
			vehInfo[DBVehID[vehicle]][vGasStatus] = false;
		}
	}

	if(PRESSED(KEY_YES) && IsPlayerInAnyVehicle(playerid) && IsWorked(playerid, TEAM_TYPE_POMOC))
	{
		new vehicle = GetPlayerVehicleID(playerid);
		if(!GetPVarInt(playerid, "changeColor"))
		{
			if(RepairVehicle(vehicle))
				Msg(playerid, COLOR_INFO, "Pojazd zosta³ naprawiony.");
			else
				Msg(playerid, COLOR_ERROR, "Nie uda³o siê naprawiæ pojazdu.");
		}
		else
		{
			DeletePVar(playerid, "changeColor");
			ChangeVehicleColor(vehicle, GetPVarInt(playerid, "changeColor_"), GetPVarInt(playerid, "changeColor__"));
			if(!Spawned[vehicle])
			{
				new Float:pos[3];
				GetVehiclePos(vehicle, pos[0], pos[1], pos[2]);
				vehInfo[DBVehID[vehicle]][vColor1] = GetPVarInt(playerid, "changeColor_");
				vehInfo[DBVehID[vehicle]][vColor2] = GetPVarInt(playerid, "changeColor__");

				SaveVehicle(vehicle);
				DestroyVehicle(vehicle);
				ResetVariablesInEnum(vehInfo[DBVehID[vehicle]], E_VEHICLE);
				
				LadujPojazd(_, _, DBVehID[vehicle]);
				SetVehiclePos(vehInfo[DBVehID[vehicle]][vSAMPID], pos[0], pos[1], pos[2]);
				PutPlayerInVehicle(playerid, vehInfo[DBVehID[vehicle]][vSAMPID], 0);
			}

			DeletePVar(playerid, "changeColor_");
			DeletePVar(playerid, "changeColor__");
			Msg(playerid, COLOR_INFO, "Kolor zosta³ pomyœlnie zmieniony.");
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
					Msg(playerid, COLOR_INFO, "Pojazd zosta³ podczepiony.");
					break;
				}
			}

			if(!Found) 
				return Msg(playerid, COLOR_ERROR, "W pobli¿u nie ma ¿adnego pojazdu.");
		}
	}
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	if(success)
	{
		new pip[16];

		foreach (new i : Player)
		{
			GetPlayerIp(i, pip, sizeof(pip));
			if(!strcmp(ip, pip, true) && !HasPlayerFullPermission(i))
			{
				SetPVarInt(i, "RCN", GetPVarInt(i, "RCN")+1);

				if(GetPVarInt(i, "RCN") >= 1)
				{
					CheatKick(i, "próba zalogowania na rcon");
					timer[i] = SetTimerEx_("Kickplayer", 300, 0, 1, "i", i);
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
				camInfo[playerid][cMode]	  = 0;
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
				foreach (new player : Player)
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
		Msg(playerid, COLOR_ERROR, "Zaloguj siê, aby u¿ywaæ komend.");
		return 0;
	}

	if(GetPVarInt(playerid, "TELEPORT"))
	{
		Msg(playerid, COLOR_ERROR, "Poczekaj a¿ zakoñczysz siê teleportowaæ.");
		return 0;
	}

	if(GetPVarInt(playerid, "Areszt"))
	{
		if(strcmp(cmdtext, "/spawn") == 0)
		{
			Msg(playerid, COLOR_ERROR, "W areszcie nie mo¿esz u¿yæ tej komendy.");
			return 0;
		}
	}

	return 1;
}

public OnPlayerCommandPerformed(playerid, cmdtext[], success)
{
	new string[76];

	format(string, sizeof(string), "[SUCCESS: %d] %s", success, cmdtext);
	ToLog(playerInfo[playerid][pID], LOG_TYPE_COMMANDS, string);

	foreach (new i : Player)
	{
		if(podgladADMIN[i] == 1 && i != playerid)
		{
			format(string, sizeof string, "@EYE {b}%s{/b} [%d]: {b}%s{/b}", PlayerName(playerid), playerid, cmdtext);
			Msg(i, COLOR_ERROR, string);
		}
	}

	if(!success)
	{
		Msg(playerid, COLOR_ERROR, "Wprowadzono niepoprawn¹ komendê.");
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
				Msg(playerid, COLOR_ERROR, "Nie odnaleziono konta z Twoim nickiem, zarejestruj siê.");
				SelectTextDraw(playerid, 0xA82A23FF);
				return 1;
			}

			new string[256];
			format(string, sizeof(string), "{FFFFFF}Witaj!\nWygl¹da na to, ¿e konto o nazwie {b}%s{/b} jest ju¿ zarejestorwane.\nProszê podaj has³o które wpisa³eœ przy rejestracji.\n\n{b}Aby zacz¹æ rozrywkê, konieczne jest zalogowanie!{/b}", PlayerName(playerid));
			Dialog_Show(playerid, DIALOG_ID_LOGIN, DIALOG_STYLE_PASSWORD, "Panel > Logowanie", clText(COLOR_INFO2, string), "Zaloguj", "Wstecz");
		}
		else if(playertextid == connectVarBox[playerid][1]) // Rejestracja
		{
			CancelSelectTextDraw(playerid);
			if(IsAccountExists(PlayerName(playerid)))
			{
				Msg(playerid, COLOR_ERROR, "Konto o tej nazwie jest ju¿ zarejestrowane, zaloguj siê.");
				SelectTextDraw(playerid, 0xA82A23FF);
				return 1;
			}

			new string[256];
			format(string, sizeof(string), "{FFFFFF}Witaj!\nWygl¹da na to, ¿e konto o nazwie {b}%s{/b} nie jest jeszcze zarejestrowane.\nProszê podaj has³o które bêdzie s³u¿y³o do logowania.\n\n{b}Aby zacz¹æ rozrywkê, konieczne jest posiadanie konta!{/b}", PlayerName(playerid));

			Dialog_Show(playerid, DIALOG_ID_REGISTER, DIALOG_STYLE_PASSWORD, "Panel > Rejestracja", clText(COLOR_INFO2, string), "Rejestruj", "Wstecz");
		}
		else if(playertextid == connectVarBox[playerid][2]) // Changelog
		{
			CancelSelectTextDraw(playerid);
			SelectTextDraw(playerid, 0xA82A23FF);
		}
		else if(playertextid == connectVarBox[playerid][3]) // Pomoc
		{
			CancelSelectTextDraw(playerid);
			Dialog_Show(playerid, DIALOG_FAQ, DIALOG_STYLE_LIST, "FAQ - Spis", "Podstawowe informacje\nSpis firm\nPoziomy kierowcy\nSpecjalne zdolnoœci\nOgraniczenia\nKomendy\nAutorzy", "Wybierz", "WyjdŸ");
		}
		else if(playertextid == connectVarBox[playerid][4]) // Autorzy
		{
			CancelSelectTextDraw(playerid);
			Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, " ", "Programiœci:\n- Maciek.\n- GeDox\n- Kozak59\n\nBeta-Testerzy:\n- [ST]Kill_Repeat\n- [ST][DJ]Bass\n- Reiban.\n\nTworzenie obiektów:\n- [ST][DJ]Bass\n- TDi\n- Laud.\n- Muscu\n- Marcin[WGM]\n- PolskiJankes\n- Kierowca\n- Kozak59\n- Devil\n- Mati\n - TRAKER", "Zamknij", #);
		}
		else if(playertextid == connectVarBox[playerid][5]) // Wyjœcie
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

		playerInfo[playerid][pSkin] = skinid;
		CancelSelectTextDraw(playerid);
		SetPVarInt(playerid, "NEWBIE_SETSPAWN", 1);
		cmd_setspawn(playerid);
	}

	for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OnPlayerClickTextDraw", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			CallLocalFunction(szTemp, "dd", playerid, _:clickedid);
	}

	return 1;
}

forward SendClientMessageToAdmins(color, const message[]);
public SendClientMessageToAdmins(color, const message[])
{
	foreach (new a : Player)
		if(playerInfo[a][pAdmin])
			SendClientMessage(a, color, message);

	return 1;
}

forward TachographUpdate();
public TachographUpdate()
{
	foreach (new i : Player)
	{
		if(!IsPlayerConnected(i) || !IsPlayerLogged(i))
			continue;

		new 
			vehicleid = GetPlayerVehicleID(i), 
			Float:speed, 
			engine, 
			lights, 
			alarm, 
			doors, 
			bonnet, 
			boot, 
			objective,
			string[64];

		GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
		GetVehicleSpeed(vehicleid, speed);

	  	if(!IsPlayerInTruck(i) || (IsPlayerInTruck(i) && !engine && GetPVarInt(i, "tacho_pauza") && speed == 0))
	  		if(GetDTime(i) > 0)
	  			GiveDTime(i, -3);

	  	if(GetDTime(i) < 0)
	  		SetDTime(i, 0);

	  	if(GetDTime(i) <= 0 && GetPVarInt(i, "tacho_pauza"))
	  	{
	  		SetPVarInt(i, "tacho_pauza", 0);
	  		Msg(i, COLOR_INFO, "Pauza zakoñczona! Mo¿na ruszaæ w drogê!");
	  	}	

		if(GetPlayerState(i) == PLAYER_STATE_DRIVER && IsPlayerInTruck(i))
		{
		   	if(engine && !GetPVarInt(i, "tacho_pauza") && !playerInfo[i][pMagnes])
		   	{
		   		GiveDTime(i, 1);
		   	}
		   	if(playerInfo[i][pMagnes])
		   		playerInfo[i][pMagnesTime] = gettime();

		   	new actualTime[3];
			ConvertSeconds(GetDTime(i), actualTime[0], actualTime[1], actualTime[2]);
			format(string, sizeof string, "%s%s%02d:%02d:%02d", (actualTime[1] >= 40) ? ("~r~") : (""), (playerInfo[i][pMagnes]) ? ("~y~") : (""), actualTime[0], actualTime[1], actualTime[2]);
			PlayerTextDrawSetString(i, hudInfo[tdInfoTacho][i], string);
		}
	}
	return 1;
}

stock UpdateOnlineWorkers()
{
	for(new d = 1; d < 4; d++)
	{
		NaDyzurze[d] = 0;
		foreach (new c : Player)
		{
			if(!IsPlayerLogged(c))
				continue;

			if(playerInfo[c][pFirm] == d && GetPVarInt(c, "Worked") && !(GetPVarInt(c, "AFK") || GetPVarInt(c, "otherAFK")))
				NaDyzurze[d] += 1;
		}
	}
	return 1;
}

forward OneSecTimer();
public OneSecTimer()
{
	new hour, minute, second, string[176];
	static time;

	ReverseBeeper();
	gettime(hour, minute, second);
 	
	format(string, sizeof string, "%02d:%02d", hour, minute);
	TextDrawSetString(MainTextDraws[Time], string);

	foreach (new i : Player)
		if(IsPlayerConnected(i))
		{
			new Float:pos[3];
			GetPlayerPos(i, pos[0], pos[1], pos[2]);
			if(floatround(pos[0] * pos[1] * pos[2]) != playerInfo[i][pLastPos])
				SetPVarInt(i, "isAFK", 0), playerInfo[i][pLastPos] = floatround(pos[0] * pos[1] * pos[2]);

			new 
				Float:health, 
				firmaid = playerInfo[i][pFirm];

			GetPlayerHealth(i, health);

			for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
			{
				format(szTemp, sizeof(szTemp), "%s_OneSecPlayerTimer", szHookInclude[nrInc]);

				if(funcidx(szTemp) != -1)
					CallLocalFunction(szTemp, "d", i);

				if( (time % 60) == 0 )
				{
					format(szTemp, sizeof(szTemp), "%s_OneMinPlayerTimer", szHookInclude[nrInc]);

					if(funcidx(szTemp) != -1)
						CallLocalFunction(szTemp, "d", i);
				}
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
			if(IsPlayerLogged(i))
			{
				format(string, sizeof string, "~r~FPS: ~w~%d~n~~r~Ping: ~w~%d", (GetPlayerFPS(i) < 0) ? (-GetPVarInt(i, "ept_fps")) : (GetPlayerFPS(i)), GetPlayerPing(i));
				PlayerTextDrawSetString(i, hudInfo[tdInfoText][i], string);
			}
		}

	for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OneSecTimer", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			CallLocalFunction(szTemp, "");

		if( (time % 60) == 0 )
		{
			format(szTemp, sizeof(szTemp), "%s_OneMinuteTimer", szHookInclude[nrInc]);

			if(funcidx(szTemp) != -1)
				CallLocalFunction(szTemp, "");
		}
	}

	time++;
	return 1;
}

forward Selectspawn(playerid);
public Selectspawn(playerid)
{
	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
		return Msg(playerid, COLOR_ERROR, "Nie mo¿esz siedzieæ w pojeŸdzie.");
	if(!GetPVarInt(playerid, "Worked") || playerInfo[playerid][pFirm] == 0)
		Dialog_Show(playerid, DIALOG_ID_SPAWN_SELECT, DIALOG_STYLE_LIST, " ", "Los Santos\nLas Venturas\nRed County\nSan Fierro", "Wybierz", "WyjdŸ");
	else
		Dialog_Show(playerid, DIALOG_ID_SPAWN_SELECT, DIALOG_STYLE_LIST, " ", "Los Santos\nLas Venturas\nRed County\nSan Fierro\n{FF0000}Baza firmowa", "Wybierz", "WyjdŸ");

	return 1;
}

forward Refresh();
public Refresh()
{
	foreach (new playerid : Player)
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
					timer[playerid] = SetTimerEx_("Kickplayer", 300, 0, 1, "i", playerid);
				}
					
				if(GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK)
				{
					CheatBan(playerid, "jetpack");
					timer[playerid] = SetTimerEx_("Kickplayer", 300, 0, 1, "i", playerid);
				}
					
				if(playerInfo[playerid][pFirm] == 0)
				{
					switch(GetPlayerWeapon(playerid))
					{
						case 1..42:
						{
							CheatBan(playerid, "weaponhack");
							timer[playerid] = SetTimerEx_("Kickplayer", 300, 0, 1, "i", playerid);
						}
						case 44..45:
						{
							CheatBan(playerid, "weaponhack");
							timer[playerid] = SetTimerEx_("Kickplayer", 300, 0, 1, "i", playerid);
						}
					}
				}
			}
		}
	}

	foreach (new vehicleid : Vehicle)
	{
		new Float:fuel, fueltype, engine,lights,alarm,doors,bonnet,boot,objective,Float:speed;
		
		if(!IsValidVehicle(vehicleid) || IsVehicleTrailer(vehicleid))
			continue;

		if(Spawned[vehicleid])
		{
				fuel = vehInfo_Temp[vehicleid][vFuel];
		}
		else
		{
			if(!vehInfo[DBVehID[vehicleid]][vGasStatus])
				fuel = vehInfo[DBVehID[vehicleid]][vFuel];

			else if(vehInfo[DBVehID[vehicleid]][vGasStatus])
				fuel = vehInfo[DBVehID[vehicleid]][vGasAmount];
		}
		fueltype = (Spawned[vehicleid]) ? (vehInfo_Temp[vehicleid][vFuelType]) : (vehInfo[DBVehID[vehicleid]][vFuelType]);

		GetVehicleSpeed(vehicleid, speed);
		GetVehicleParamsEx(vehicleid,engine,lights,alarm,doors,bonnet,boot,objective);

		if(!Spawned[vehicleid] && vehInfo[DBVehID[vehicleid]][vGasStatus])
		{
			new Float:VX, Float:VY, Float:VZ;
			GetVehicleVelocity(vehicleid, VX, VY, VZ);
			SetVehicleVelocity(vehicleid, VX * 0.95, VY * 0.95, VZ);
		}

		if(engine)
		{
			if(Spawned[vehicleid])
			{
				fuel -= (fueltype == FUEL_TYPE_GAS) ? (floatmul(speed, 0.00015)) : (floatmul(speed, 0.0003));
			}
			else
			{
				if(!vehInfo[DBVehID[vehicleid]][vGasStatus])
					fuel -= (fueltype == FUEL_TYPE_GAS) ? (floatmul(speed, 0.00015)) : (floatmul(speed, 0.0003));
				else if(vehInfo[DBVehID[vehicleid]][vGasStatus])
					fuel -= floatmul(speed, 0.00015);
			}

			if(fuel <= 0)
			{
				fuel = 0;
				SetVehicleParamsEx(vehicleid,VEHICLE_PARAMS_OFF,lights,alarm,doors,bonnet,boot,objective);
			}

			if(Spawned[vehicleid])
			{
				vehInfo_Temp[vehicleid][vFuel] = fuel;
			}
			else
			{
				if(!vehInfo[DBVehID[vehicleid]][vGasStatus])
					vehInfo[DBVehID[vehicleid]][vFuel] = fuel;
				else if(vehInfo[DBVehID[vehicleid]][vGasStatus])
					vehInfo[DBVehID[vehicleid]][vGasAmount] = fuel;
			}
		}
	}
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
{
	return Kick(playerid);
}

forward ReqSpawnPlayer(playerid);
public ReqSpawnPlayer(playerid)
{
	SetPlayerSkin(playerid, playerInfo[playerid][pSkin]);
	SpawnPlayer(playerid);
	return 1;
}

stock SavePlayer(playerid, saveTime = false)
{
	if(IsPlayerLogged(playerid))
	{
		new string[512];

		format(string, sizeof(string), "UPDATE Accounts SET Money = '%d', Score = '%d', Hunger = '%.f', Firma ='%d', Skin ='%d', Tacho='%d', Foto='%d', Toll='%d', Gtime='%d', \
				Worktime='%d', Adr='%d', lastVisit = NOW(), Bankomat = '%d', allowedPoints = '%d', skills = '%s', pSpawn = '%d', pSpawnInfo = '%s', cbradioNick = '%s'", 
				playerInfo[playerid][pMoney], 
				playerInfo[playerid][pScore], 
				playerInfo[playerid][pHunger],
				playerInfo[playerid][pFirm], 
				playerInfo[playerid][pSkin], 
				playerInfo[playerid][pTacho], 
				playerInfo[playerid][pPhoto], 
				playerInfo[playerid][pToll], 
				playerInfo[playerid][pGTime],
				playerInfo[playerid][pWorkTime], 
				playerInfo[playerid][pADR],
				playerInfo[playerid][pBankomat],
				pPoints[playerid][pAllowedPoints],
				pPoints[playerid][pSkills],
				playerInfo[playerid][pSpawn],
				playerInfo[playerid][pSpawnInfo],
				playerInfo[playerid][pCBNick]);
		
		if(saveTime)
		{
			format(string, sizeof(string), "%s, `TimeOnline` = `TimeOnline` + '%d'", string, (floatround((GetTickCount()-GetPVarInt(playerid, "IleGral"))/1000)));
			SetPVarInt(playerid, "IleGral", GetTickCount());
		}

		format(string, sizeof(string), "%s WHERE ID = %d", string, playerInfo[playerid][pID]);
		mysql_query(string);
	}

	return 1;
}

stock GetOnlineTime(playerid)
{
	return playerInfo[playerid][pOnlineTime] + (floatround((GetTickCount()-GetPVarInt(playerid, "IleGral"))/1000));
}

forward SaveVehicle(vehicleid);
public SaveVehicle(vehicleid)
{
	new vehuid = DBVehID[vehicleid], string[400];

	format(string, sizeof(string), "UPDATE `Pojazdy` SET `PosX`='%f', `PosY`='%f', `PosZ`='%f', `PosA`='%f', `Fuel`='%f', `Przebieg`='%f', `Color1`='%d', `Color2`='%d', `owner_vce`='%d', `Plate`='%s', `Przeglad`='%s', `gasBootle`='%d', `gasAmount`='%f' WHERE `id`='%d'",
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
		vehInfo[vehuid][vGasBootle],
		vehInfo[vehuid][vGasAmount],
		vehInfo[vehuid][vID]
		);
	mysql_query(string);

	return 1;
}

forward Jobtime();
public Jobtime()
{
	SaveBTObjects();
	foreach (new playerid : Player)
		if(IsPlayerConnected(playerid) && IsPlayerLogged(playerid))
			if(firmInfo[playerInfo[playerid][pFirm]][tType] >= TEAM_TYPE_POLICE)
				if(GetPVarInt(playerid, "Worked") && !(GetPVarInt(playerid, "AFK") && GetPVarInt(playerid, "isAFK") && GetPVarInt(playerid, "otherAFK")))
			   		GiveWork(playerid, 2);

	return 1;
}

forward Update();
public Update()
{
	foreach (new playerid : Player)
	{
		if(!IsPlayerLogged(playerid) || !IsPlayerSpawned(playerid))
			continue;

		if(IsPlayerConnected(playerid))
		{
			if(GetPVarInt(playerid, "AFK") || GetPVarInt(playerid, "otherAFK"))
			{
				TogglePlayerControllable(playerid, false);
				continue;
			}
			if(IsPlayerInAnyVehicle(playerid))
			{
				new 
					vehicleid = GetPlayerVehicleID(playerid),
					trailerid = GetVehicleTrailer(vehicleid),
					Float:speed;

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

				if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
				{
					new 
						string[55],
						Float:HP, 
						fuel = floatround((!Spawned[vehicleid]) ? (vehInfo[DBVehID[vehicleid]][vFuel]) : (vehInfo_Temp[vehicleid][vFuel])),
						mileage = floatround((!Spawned[vehicleid]) ? (vehInfo[DBVehID[vehicleid]][vPrzebieg] / 1000) : (vehInfo_Temp[vehicleid][vPrzebieg] / 1000));

					if(!Spawned[vehicleid])
					{
						if(vehInfo[DBVehID[vehicleid]][vGasStatus])
						{
							fuel = floatround(vehInfo[DBVehID[vehicleid]][vGasAmount]);
						}
					}

					GetVehicleHealth(vehicleid, HP);
					format(string, sizeof string, "Stan: %d%%~n~Paliwo: %dl~n~Przebieg: %dkm", (((floatround(HP, floatround_round) - 250) * 100) / 750 < 0) ? (0) : (((floatround(HP, floatround_round) - 250) * 100) / 750), fuel, mileage);
					PlayerTextDrawSetString(playerid, hudInfo[tdInfoSpeedo][TD_CAR_INFO][playerid], string);

					format(string, sizeof string, "~r~%d~n~km/h", floatround(speed));
					PlayerTextDrawSetString(playerid, hudInfo[tdInfoSpeedo][TD_CAR_SPEED][playerid], string);
				}
			}
			if(IsWorked(playerid, TEAM_TYPE_POLICE) && GetPVarInt(playerid, "RadarOnline"))
			{
				new id = GetVehicleInfrontID(GetPlayerVehicleID(playerid));
				if(id < 0)
				{
					PlayerTextDrawSetString(playerid, hudInfo[tdInfoRadar][TD_RD_NAME][playerid], "~r~Radar (-)");
					PlayerTextDrawSetString(playerid, hudInfo[tdInfoRadar][TD_RD_SPEED][playerid], "- km/h");
				}
				else
				{
					if(VehicleDriver[id] != INVALID_PLAYER_ID)
					{
						new Float:targetSpeed, szString[18]; GetVehicleSpeed(id, targetSpeed);
						format(szString, sizeof szString, "%d km/h", floatround(targetSpeed));
						PlayerTextDrawSetString(playerid, hudInfo[tdInfoRadar][TD_RD_SPEED][playerid], szString);

						format(szString, sizeof szString, "~r~Radar (%d)", VehicleDriver[id]);
						PlayerTextDrawSetString(playerid, hudInfo[tdInfoRadar][TD_RD_NAME][playerid], szString);
					}
					else
					{
						PlayerTextDrawSetString(playerid, hudInfo[tdInfoRadar][TD_RD_NAME][playerid], "~r~Radar (-)");
						PlayerTextDrawSetString(playerid, hudInfo[tdInfoRadar][TD_RD_SPEED][playerid], "- km/h");
					}
				}
			}
		}
	}
	return 1;
}

CMD:radar(playerid, params[])
{
	if(!IsWorked(playerid, TEAM_TYPE_POLICE))
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(GetPVarInt(playerid, "RadarOnline"))
	{
		ShowPlayerRadar(playerid, false);
		Msg(playerid, COLOR_INFO, "Wy³¹czono radar.");
		DeletePVar(playerid, "RadarOnline");
	}
	else
	{
		ShowPlayerRadar(playerid, true);
		PlayerTextDrawSetString(playerid, hudInfo[tdInfoRadar][TD_RD_NAME][playerid], "~r~Radar (-)");
		PlayerTextDrawSetString(playerid, hudInfo[tdInfoRadar][TD_RD_SPEED][playerid], "- km/h");
		SetPVarInt(playerid, "RadarOnline", 1);
		Msg(playerid, COLOR_INFO, "Za³¹czono radar.");
	}
	return 1;
}

forward Unmute(playerid);
public Unmute(playerid)
{
	new seconds = GetPVarInt(playerid, "Mutetime");

	if(seconds <= 0)
	{
		DeletePVar(playerid,"Mute");
		DeletePVar(playerid,"Mutetime");
		Msg(playerid, COLOR_INFO, "Twoje wyciszenie zosta³o zakoñczone, mo¿esz ju¿ normalnie pisaæ.");
	}
	else
	{
		seconds--;
		SetPVarInt(playerid, "Mutetime", seconds);
		SetTimerEx_("Unmute", 0, 1000, 1, "i", playerid);
	}

	return 1;
}

forward SaveALL();
public SaveALL()
{
	HungerUpdate();

	new 
		string[128],
		t[3];

	gettime(t[0], t[1], t[2]);
	SetWorldTime(t[0]);

	foreach (new playerid : Player)
	{
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

	foreach (new firmaid : Player)
	{
		if(firmInfo[firmaid][tType])
		{
			format(string, sizeof(string), "UPDATE `Firmy` SET `Bank`=%d WHERE `id`='%d'", firmInfo[firmaid][tBank], firmaid);
			mysql_query(string);
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
			Msg(playerid, COLOR_ERROR, "Nie mo¿esz podczepiæ tej naczepy.");

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

stock HungerUpdate()
{
	foreach (new playerid : Player)
	{
		if(IsPlayerLogged(playerid))
		{
			if(playerInfo[playerid][pHunger] <= 5.0)
			{
				new Float:health;
				GetPlayerHealth(playerid, health);
				SetPlayerHealth(playerid, health - (5 + random(5)));
				if(!GetPVarInt(playerid, "Wypadek"))
					FadeColorForPlayer(playerid, 181, 51, 36, 125, 0, 0, 0, 0, 25, 0);
				
				SetPlayerProgressBarValue(playerid, hudInfo[tdHungerProgress][playerid], 5.0);
			}
			else
			{
				playerInfo[playerid][pHunger] -= float(random(2));
				SetPlayerProgressBarValue(playerid, hudInfo[tdHungerProgress][playerid], playerInfo[playerid][pHunger]);
			}
		}
	}
	return 1;
}

public OnPlayerEditDynamicObject(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
	if(GetPVarInt(playerid, "BT_Edit"))
	{
		new _btObject = GetPVarInt(playerid, "BT_Object");
		switch(response)
		{
			case EDIT_RESPONSE_CANCEL:
			{
				SetDynamicObjectPos(objectid, objectInfo[_btObject][btPos][0], objectInfo[_btObject][btPos][1], objectInfo[_btObject][btPos][2]);
				SetDynamicObjectRot(objectid, objectInfo[_btObject][btRot][0], objectInfo[_btObject][btRot][1], objectInfo[_btObject][btRot][2]);
			}
			case EDIT_RESPONSE_FINAL:
			{
				ChangePosBTObject(playerid, _btObject, x, y, z, rx, ry, rz);
			}
			default: return 1;
		}
		DeletePVar(playerid, "BT_Edit");
	}

	for(new nrInc, szTemp[31]; nrInc < sizeof(szHookInclude); nrInc++)
	{
		format(szTemp, sizeof(szTemp), "%s_OnPlayerEditDynObject", szHookInclude[nrInc]);

		if(funcidx(szTemp) != -1)
			CallLocalFunction(szTemp, "dddffffff", playerid, objectid, response, x, y, z, rx, ry, rz);
	}

	return 1;
}

forward CruiseControl(playerid);
public CruiseControl(playerid)
{
	if(GetPVarInt(playerid, "Tempomat"))
	{
		new vid = GetPlayerVehicleID(playerid);
		new Float:x, Float:y, Float:z;
		GetVehicleVelocity(vid, x, y, z);
	 
		new Float:angle, Float:heading, Float:speed;
		GetVehicleZAngle(vid, angle);
		GetVehicleHeadingAngle(vid, heading);
		DistanceFlat(0, 0, x, y, speed);
	 
	 
		if(PlayerCruiseSpeed[playerid] == 0.00 || GetPlayerState(playerid) != PLAYER_STATE_DRIVER || (speed < 0.8 * PlayerCruiseSpeed[playerid]) || z > 1 || (floatabs(angle - heading) > 50 && floatabs(angle - heading) < 310))
		{								   
			PlayerCruiseSpeed[playerid] = 0.00;
			Msg(playerid, COLOR_INFO, "Tempomat zosta³ {b}wy³¹czony{/b}.");
			DeletePVar(playerid, "Tempomat");
			return false;
		}
		GetXYVelocity(vid, x, y, PlayerCruiseSpeed[playerid]);
		SetVehicleVelocity(vid, x, y, z);
		return SetTimerEx("CruiseControl", 500, false, "i", playerid);
	}
	return 1;
}

DistanceFlat(Float:ax, Float:ay, Float:bx,Float:by, &Float:distance)
{
		distance = floatsqroot(floatpower(bx-ax,2)+floatpower(by-ay,2));
		return floatround(distance);
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
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnieñ");

	if(sscanf(params, "ds[76]", forplayerid, Powod))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /ban [id gracza] [powód]");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie jest obecny na serwerze.");

	format(string, sizeof string, "Gracz {b}%s{/b} zosta³ {b}zbanowany{/b} przez {b}%s{/b} z powodu {b}%s{/b}.", PlayerName(forplayerid), PlayerName(playerid), Powod);
	MsgToAll(COLOR_ERROR, string);

	new Y, Mo, D, H, Mi, S, ip[16];
	getdate(Y, Mo, D);
	gettime(H, Mi, S);
	GetPlayerIp(forplayerid, ip, sizeof(ip));
	format(string, sizeof string, "INSERT INTO `Bans` (`Name`, `Nameadmin`, `Hour`, `Minute`, `Day`, `Month`, `Year`, `Reason`, `IP`) VALUES('%s', '%s', '%02d', '%02d', '%02d', '%02d', '%02d', '%s', '%s')", PlayerName(forplayerid), PlayerName(playerid), H, Mi, D, Mo, Y, Powod, ip);
	mysql_query(string);
			

	format(string, sizeof string, "{a9c4e4}ID bana: {FFFFFF}%d\n", mysql_insert_id());
	format(string, sizeof string, "%s{a9c4e4}Zbanowany nick: {FFFFFF}%s\n", string, PlayerName(forplayerid));
	format(string, sizeof string, "%s{a9c4e4}Nick admina banuj¹cego: {FFFFFF}%s\n", string, PlayerName(playerid));
	format(string, sizeof string, "%s{a9c4e4}Data zbanowania konta: {FFFFFF}przed chwil¹\n", string);
	format(string, sizeof string, "%s{a9c4e4}Powód bana: {FFFFFF}%s\n", string, Powod);
	format(string, sizeof string, "%s{a9c4e4}Zbanowane IP: {FFFFFF}%s\n \n", string, ip);
	format(string, sizeof string, "%s{a9c4e4}Je¿eli zosta³eœ zbanowany nies³usznie napisz podanie o unbana na forum {FFFFFF}www.serwertruck.eu", string);
	ShowInfo(playerid, string);

	timer[playerid] = SetTimerEx_("Kickplayer", 300, 0, 1, "i", forplayerid);
	return 1;
}

CMD:unbanip(playerid, params[])
{
	new IP[76], string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnieñ");

	if(sscanf(params, "s[76]", IP))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /unban [ip gracza]");

	format(string, sizeof string, "Konto {b}%s{/b} zosta³o odbanowane.", IP);
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "DELETE FROM `Bans` WHERE `IP`= '%s'", IP);
	mysql_query(string);

	return 1;
}

CMD:unban(playerid, params[])
{
	new IP[76], string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnieñ");

	if(sscanf(params, "s[76]", IP))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /unban [gracz]");

	format(string, sizeof string, "Konto {b}%s{/b} zosta³o odbanowane.", IP);
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "DELETE FROM `Bans` WHERE `Name`= '%s'", IP);
	mysql_query(string);

	return 1;
}

CMD:banip(playerid, params[])
{
	new IP[50], Powod[50], string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnieñ");

	if(sscanf(params, "s[50]s[50]", IP, Powod))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /banip [ip gracza] [powód]");

	format(string, sizeof string, "Konto {b}%s{/b} zosta³o zbanowane.", IP);
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
		return Msg(playerid, COLOR_ERROR, "Wpisz: /kick [id gracza] [powód]");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie jest obecny na serwerze.");

	if(playerInfo[playerid][pAdmin])
	{
		format(string, sizeof string, "Gracz {b}%s{/b} zosta³ wyrzucony przez {b}%s{/b} z powodu {b}%s{/b}.", PlayerName(forplayerid), PlayerName(playerid), Powod);
		MsgToAll(COLOR_ERROR, string);
		timer[forplayerid] = SetTimerEx_("Kickplayer", 300, 0, 1, "i", forplayerid);
	} 
	else Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnieñ");
	return 1;
}

CMD:removebuilding(playerid, params[])
{
	new id, X[10], Y[10], Z[10], R[20], string[256];

	if(!playerInfo[playerid][pBuildmaster])
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnieñ (BuildMaster).");

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
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnieñ (BuildMaster).");

	if(sscanf(params, "s[20]", R))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /restorebuilding [komentarz]");

	format(string, sizeof string, "DELETE FROM `st_usuniete_obiekty` WHERE `Comment`= '%s'", R);
	mysql_query(string);

	return 1;
}

CMD:warn(playerid, params[])
{
	new forplayerid, reason[64], string[200];

	if(sscanf(params, "ds[64]", forplayerid, reason))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /warn [id gracza] [powód]");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie jest obecny na serwerze.");

	if(playerInfo[playerid][pAdmin])
	{
		SetPVarInt(forplayerid, "Warn", GetPVarInt(forplayerid, "Warn")+1);
		format(string, sizeof string, "Gracz {b}%s{/b} zosta³ ostrze¿ony [{b}%d/3{/b}] przez {b}%s{/b} z powodu {b}%s{/b}.", PlayerName(forplayerid), GetPVarInt(forplayerid, "Warn"), PlayerName(playerid), reason);
		MsgToAll(COLOR_ERROR, string);
	} 
	else Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(GetPVarInt(forplayerid, "Warn") == 3)
	{
		CheatKick(forplayerid, "trzy ostrze¿enia");
		timer[forplayerid] = SetTimerEx_("Kickplayer", 300, 0, 1, "i", forplayerid);
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
		return Msg(playerid, COLOR_ERROR, "Gracz ten nie posiada ¿adnych ostrze¿e?");

	if(playerInfo[playerid][pAdmin])
	{
		SetPVarInt(forplayerid, "Warn", GetPVarInt(forplayerid, "Warn")-1);
		format(string, sizeof string, "Graczowi {b}%s{/b} zosta³o cofniête ostrze¿enie przez administratora {b}%s{/b}.", PlayerName(forplayerid), PlayerName(playerid));
		MsgToAll(COLOR_ERROR, string);
	}
	else
	{
		Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");
	}
	return 1;
}

CMD:givemoney(playerid, params[])
{
	new forplayerid, money, string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "dd", forplayerid, money))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /givemoney [id gracza] [iloœæ]");

	GiveMoney(forplayerid, money);
	format(string, sizeof string, "Da³eœ {b}%d${/b} graczowi {b}%s{/b}.", money, PlayerName(forplayerid));
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "Otrzyma³eœ {b}%d${/b} od administratora {b}%s{/b}.", money, PlayerName(playerid));
	Msg(forplayerid, COLOR_INFO, string);
	return 1;
}

CMD:givescore(playerid, params[])
{
	new forplayerid, money, string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "dd", forplayerid, money))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /givescore [id gracza] [iloœæ]");

	GiveScore(forplayerid, money);
	format(string, sizeof string, "Przekaza³eœ {b}%d{/b} punktów graczowi {b}%s{/b}.", money, PlayerName(forplayerid));
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "Otrzyma³eœ {b}%d{/b} punktów od administratora {b}%s{/b}.", money, PlayerName(playerid));
	Msg(forplayerid, COLOR_INFO, string);
	return 1;
}

CMD:resetmoney(playerid, params[])
{
	new forplayerid, string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "d", forplayerid))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /resetmoney [id gracza]");

	ResetMoney(forplayerid);
	format(string, sizeof string, "Zresetowa³eœ pieni¹dze graczowi {b}%s{/b}.", PlayerName(forplayerid));
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "Twoje pieni¹dze zosta³y zresetowane przez administratora {b}%s{/b}.", PlayerName(playerid));
	Msg(forplayerid, COLOR_INFO, string);
	return 1;
}

CMD:resetscore(playerid, params[])
{
	new forplayerid, string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "d", forplayerid))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /resetscore [id gracza]");

	ResetScore(forplayerid);
	format(string, sizeof string, "Zresetowa³eœ punkty graczowi {b}%s{/b}.", PlayerName(forplayerid));
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "Twoje punkty zosta³y zresetowane przez administratora {b}%s{/b}.", PlayerName(playerid));
	Msg(forplayerid, COLOR_INFO, string);
	return 1;
}

CMD:givemoneyall(playerid, params[])
{
	new money, string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "d", money))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /givemoneyall [iloœæ]");

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
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "d", score))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /givescoreall [iloœæ]");

	foreach (new playeri : Player)
	{
		if(IsPlayerLogged(playeri))
		{
			GiveScore(playeri, score);
		}
	}

	format(string, sizeof string, "Wszyscy otrzymali {b}%d{/b} punkt(ów) od administratora {b}%s{/b}.", score, PlayerName(playerid));
	MsgToAll(COLOR_INFO, string);
	return 1;
}

CMD:s(playerid, params[])
{
	new string[128];

	if(isnull(params)) return Msg(playerid, COLOR_ERROR, "Wpisz: /s [tekst]");

	foreach (new player : Player)
	{
		if(GetDistancePlayerToPlayer(playerid, player) < 30)
		{
			format(string, sizeof string, "{b}%s mówi:{/b} %s", PlayerName(playerid), params);
			SendClientMessage(player, -1, clText(COLOR_INFO2, string));
		}
	}
	return 1;
}

CMD:me(playerid, params[])
{
	new string[128];

	if(isnull(params)) return Msg(playerid, COLOR_ERROR, "Wpisz: /me [czynnoœæ]");

	foreach (new player : Player)
	{
		format(string, sizeof string, "%s %s", PlayerName(playerid), params);
		Msg(player, COLOR_INFO2, string, false, true);
	}
	return 1;
}

CMD:zw(playerid, params[])
{
	new string[176];

	if(GetPVarInt(playerid, "AFK"))
		return Msg(playerid, COLOR_INFO, "Aktualnie posiadasz status {b}zaraz wracam{/b}.");

	SetPVarInt(playerid, "AFK", 1);
	TogglePlayerControllable(playerid, false);

	format(string, sizeof(string), "Gracz {b}%s{/b} zmieni³ status na {b}zaraz wracam{/b}.",PlayerName(playerid));
	MsgToAll(COLOR_INFO2, string);
	return 1;
}

CMD:jj(playerid, params[])
{
	new string[176];

	if(!GetPVarInt(playerid, "AFK"))
		return Msg(playerid, COLOR_INFO, "Aktualnie posiadasz status {b}ju¿ jestem{/b}.");

	DeletePVar(playerid, "AFK");
	TogglePlayerControllable(playerid, true);

	format(string, sizeof(string), "Gracz {b}%s{/b} zmieni³ status na {b}ju¿ jestem{/b}.",PlayerName(playerid));
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

	format(string, sizeof(string), "Gracz {b}%s{/b} wita siê z wszystkimi.", PlayerName(playerid));
	MsgToAll(COLOR_INFO2, string);
	return 1;
}

CMD:nara(playerid, params[])
{
	new string[176];

	format(string, sizeof(string), "Gracz {b}%s{/b} ¿egna siê z wszystkimi.", PlayerName(playerid));
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
		return Msg(playerid, COLOR_ERROR, "Wpisz: /pm [id] [treœæ]");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie jest pod³¹czony.");

	if(playerid == forplayerid)
		return Msg(playerid, COLOR_ERROR, "Nie mo¿esz napisaæ do siebie.");

	if(GetPVarInt(forplayerid, "PMOFF"))
		return Msg(playerid, COLOR_ERROR, "Ten gracz wy³¹czy³ prywatne wiadomoœci.");

	format(string, sizeof(string),"PM od {b}%s{/b} (%d): {b}%s{/b}",PlayerName(playerid),playerid, Message);
	Msg(forplayerid, COLOR_INFO3, string);
	format(string, sizeof(string),"PM do {b}%s{/b} (%d): {b}%s{/b}",PlayerName(forplayerid),forplayerid, Message);
	Msg(playerid, COLOR_INFO3, string);

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
			Msg(playerid, COLOR_INFO, "Prywatne wiadomoœci zosta³y {b}w³¹czone{b}.");
		}
		else
		{
			SetPVarInt(playerid, "PMOFF", 1);
			Msg(playerid, COLOR_INFO, "Prywatne wiadomoœci zosta³y {b}wy³¹czone{b}.");
		}
	}
	else if(strcmp(params, "cb", true) == 0)
	{
		if(GetPVarInt(playerid, "CBOFF"))
		{
			DeletePVar(playerid, "CBOFF");
			Msg(playerid, COLOR_INFO, "CB Radio zosta³o {b}w³¹czone{b}.");
		}
		else
		{
			SetPVarInt(playerid, "CBOFF", 1);
			Msg(playerid, COLOR_INFO, "CB Radio zosta³o {b}wy³¹czone{b}.");
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

		format(string, sizeof string, "Czat zosta³ oczyszczony przez {b}%s{/b}.",PlayerName(playerid));
		MsgToAll(COLOR_INFO3, string);
	} 
	else 
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");
	return 1;
}

CMD:cb(playerid, params[])
{
	new string[176], model;

	if(!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie znajdujesz siê w pojeŸdzie.");

	if(sscanf(params, "s[128]", string))	
		return Msg(playerid, COLOR_ERROR, "Wpisz: /cb [tekst]");

	if(!strcmp(playerInfo[playerid][pCBNick], "braknickubrak"))
		return Msg(playerid, COLOR_ERROR, "Nie masz ksywki, wpisz {b}/cbksywka{/b}. Pamiêtaj, swojego pseudonimu nie mo¿na zmieniæ!");

	model = GetVehicleModel(GetPlayerVehicleID(playerid));
	if(!(IsVehicleTruck(model) || IsVehicleVan(model)))
		return Msg(playerid, COLOR_ERROR, "W tym pojeŸdzie nie ma CB-Radia.");

	foreach (new player : Player)
	{
		if(IsPlayerInAnyVehicle(player) && GetPlayerChannelCB(player) == GetPlayerChannelCB(playerid) && !playerInfo[player][pAdmin] && (IsVehicleTruck(model) || IsVehicleVan(model)) == true)
		{
			format(string, sizeof string, "{b}%s{/b} [CB: {b}%d{/b}]: {FFFFFF}%s", playerInfo[playerid][pCBNick], GetPlayerChannelCB(playerid), params);
			Msg(player, COLOR_INFO2, string);
		}
		else if(playerInfo[player][pAdmin])
		{
			format(string, sizeof string, "{b}%s{/b} [CB: {b}%d{/b}] (%s, %d): {FFFFFF}%s", playerInfo[playerid][pCBNick], GetPlayerChannelCB(playerid), PlayerName(playerid), playerid, params);
			Msg(player, COLOR_INFO2, string);
		}
	}

	ToLog(playerInfo[playerid][pID], LOG_TYPE_CHAT, "cb", params);

	return 1;
}

CMD:cbksywka(playerid, params[])
{
	if(!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie znajdujesz siê w pojeŸdzie.");

	new cbNick[32];

	if(sscanf(params, "s[32]", cbNick)) 
		return Msg(playerid, COLOR_ERROR, "Nie poda³eœ swojej ksywki!");

	if(strcmp(playerInfo[playerid][pCBNick], "braknickubrak"))
		return Msg(playerid, COLOR_ERROR, "Nie mo¿esz zmieniæ swojej ksywki.");

	if(strlen(cbNick) > 32 || strlen(cbNick) < 3)
		return Msg(playerid, COLOR_ERROR, "Twoja ksywka jest zbyt d³uga lub krótka. (3 - 32 znaki)");

	new model = GetVehicleModel(GetPlayerVehicleID(playerid));
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER || !(IsVehicleTruck(model) || IsVehicleVan(model)))
		return Msg(playerid, COLOR_ERROR, "Nie mo¿esz tego teraz zrobiæ.");

	format(playerInfo[playerid][pCBNick], 32, "%s", cbNick);
	Msg(playerid, COLOR_INFO, "Ksywka ustawiona! Od teraz mo¿esz pisaæ na CB-Radiu komend¹ {b}/cb{/b}. Szerokoœci!");
	return 1;
}

CMD:cbkanal(playerid, params[])
{
	if(!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie znajdujesz siê w pojeŸdzie.");

	if(strcmp(playerInfo[playerid][pCBNick], "braknickubrak") != -1)
		return Msg(playerid, COLOR_ERROR, "Nie masz ksywki, wpisz {b}/cbksywka{/b}. Pamiêtaj, swojego pseudonimu nie mo¿na zmieniæ!");

	new model = GetVehicleModel(GetPlayerVehicleID(playerid));
	if(!(IsVehicleTruck(model) || IsVehicleVan(model)))
		return Msg(playerid, COLOR_ERROR, "W tym pojeŸdzie nie ma CB-Radia.");

	if(isnull(params))
	{
		Msg(playerid, COLOR_ERROR, "Wpisz: /cbkanal [kana³]");
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
			Msg(playerid, COLOR_INFO, "Zmieni³eœ kana³ CB Radia.");
		}
	}
	return 1;
}

CMD:mute(playerid, params[])
{
	new forplayerid, czas, reason[64], string[256];

	if(sscanf(params, "dds[64]", forplayerid, czas, reason))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /mute [id gracza] [czas] [powód]");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Gracz ten nie jest obecny na serwerze.");

	if(GetPVarInt(forplayerid, "Mute"))
		return Msg(playerid, COLOR_ERROR, "Gracz ten jest uciszony.");

	if(playerInfo[playerid][pAdmin])
	{
		SetPVarInt(forplayerid, "Mute", 1);
		SetPVarInt(forplayerid, "Mutetime", (czas*60));
		SetTimerEx_("Unmute", 100, 0, 1, "i", forplayerid);

		format(string, sizeof string, "Gracz {b}%s{/b} zosta³ wyciszony na {b}%d{/b} minut przez {b}%s{/b} z powodu {b}%s{/b}.", PlayerName(forplayerid), czas, PlayerName(playerid), reason);
		MsgToAll(COLOR_INFO2, string);
	}
	else 
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnieñ");
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
		format(string, sizeof string, "Gracz {b}%s{/b} zosta³ odciszony przez {b}%s{/b}.", PlayerName(forplayerid), PlayerName(playerid));
		MsgToAll(COLOR_INFO2, string);
	}
	else 
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnieñ");
	return 1;
}

CMD:ochrona(playerid, params[])
{
	new string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(SECURITYON == 1)
	{
		SECURITYON = 0;
		format(string, sizeof string, "Ochrona serwera zosta³a {b}wy³¹czona{/b} przez %s.", PlayerName(playerid));
		MsgToAll(COLOR_INFO2, string);
	}
	else
	{
		format(string, sizeof string, "Ochrona serwera zosta³ {b}w³¹czona{/b} przez %s.", PlayerName(playerid));
		MsgToAll(COLOR_INFO2, string);
		SECURITYON = 1;
	}
	return 1;
}

CMD:report(playerid, params[])
{
	new forplayerid, reason[76], szString[148];

	if(sscanf(params, "ds[76]", forplayerid, reason))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /report [id gracza] [treœæ]");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Gracz ten nie jest obecny na serwerze.");

	if(strlen(reason) > 76 || strlen(reason) < 5)
		return Msg(playerid, COLOR_ERROR, "Wprowadzi³eœ nieprawid³owy powód.");

	if(playerInfo[forplayerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie mo¿esz zg³osiæ administratora.");

	for(new i = 0; i < MAX_PLAYERS*2; i++)
	{
		if(repInfo[i][repUsed])
			continue;

		repInfo[i][repUsed] = true;
		repInfo[i][repPReport] = playerid;
		repInfo[i][repPReported] = forplayerid;
		strins(repInfo[i][repReason], reason, 0);
		format(szString, sizeof szString, "Gracz {b}%s{/b} zg³osi³ gracza {b}%s{/b}. Wpisz /reports, aby zobaczyæ zg³oszenia.", PlayerName(playerid), PlayerName(forplayerid));

		foreach (new d : Player)
		{
			if(!IsPlayerLogged(d))
				continue;

			if(playerInfo[d][pAdmin] >= 1)
				Msg(d, COLOR_ERROR, szString);
		}
		break;
	}

	format(szString, sizeof szString, "Zg³osi³eœ gracza {b}%s{/b}.", PlayerName(forplayerid));
	Msg(playerid, COLOR_INFO, szString);
	return 1;
}

CMD:reports(playerid)
{
	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");
	
	new count;
	for(new i = 0; i < MAX_PLAYERS*2; i++)
	{
		if(!repInfo[i][repUsed])
			continue;
		count++;
	}
	if(count < 1)
		return Msg(playerid, COLOR_ERROR, "Brak zg³oszeñ.");

	new szString[512];
	for(new c = 0; c < MAX_PLAYERS*2; c++)
	{
		if(!repInfo[c][repUsed])
			continue;
		format(szString, sizeof szString, "{FFFFFF}%d\t%s\t%s\t{FF0000}%s\n", c, PlayerName(repInfo[c][repPReport]), PlayerName(repInfo[c][repPReported]), repInfo[c][repReason]);
	}
	strins(szString, "ID\tZg³aszaj¹cy\tZg³aszany\tPowód\n", 0);
	Dialog_Show(playerid, DIALOG_REPORT, DIALOG_STYLE_TABLIST_HEADERS, "Zg³oszenia", szString, "Usuñ", "WyjdŸ");
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

	if(GetOnlineTime(playerid) < 7200)
		return Msg(playerid, COLOR_ERROR, "Grasz za krótko aby przelewaæ pieni¹dze!");

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
	format(string, sizeof string, "Przekaza³eœ {b}$%d{/b} graczowi {b}%s{/b}.", kwota, PlayerName(forplayerid));
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "Otrzyma³eœ {b}$%d{/b} od gracza {b}%s{/b}.", kwota, PlayerName(playerid));
	Msg(forplayerid, COLOR_INFO, string);
	return 1;
}

CMD:admins(playerid, params[])
{
	new admin = 0,
	string[50];

	Msg(playerid, COLOR_INFO, "Administratorzy on-line:");
	foreach (new player : Player)
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
		Msg(playerid, COLOR_INFO, "Brak administratorów online.");

	return 1;
}

CMD:hideme(playerid, params[])
{
	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(GetPVarInt(playerid, "HIDEME"))
		return Msg(playerid, COLOR_ERROR, "Aktualnie jesteœ ju¿ {b}ukryty{/b}.");

	SetPVarInt(playerid, "HIDEME", 1);
	Msg(playerid, COLOR_INFO, "Zosta³ {b}ukryty{/b} na liœcie administratorów.");
	return 1;
}

CMD:showme(playerid, params[])
{
	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(!GetPVarInt(playerid, "HIDEME"))
		return Msg(playerid, COLOR_ERROR, "Aktualnie nie jesteœ {b}ukryty{/b}.");

	DeletePVar(playerid, "HIDEME");
	Msg(playerid, COLOR_INFO, "Jesteœ {b}widoczny{/b} na liœcie administratorów.");
	return 1;
}

CMD:odczep(playerid, params[])
{
	new 
		State = GetPlayerState(playerid),
		trailerid,
		odczep;

	if(!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie znajdujesz siê w pojeŸdzie.");

	if(State != PLAYER_STATE_DRIVER)
		return Msg(playerid, COLOR_ERROR, "Nie siedzisz za kierownic¹.");

	trailerid = GetPlayerVehicleID(playerid);
	odczep = GetVehicleTrailer(trailerid);
	AttachTrailerToVehicle(trailerid, odczep);
	DetachTrailerFromVehicle(trailerid);
	Msg(playerid, COLOR_INFO, "Naczepa zosta³a odczepiona.");
	return 1;
}

CMD:jetpack(playerid, params[])
{
	new forplayerid, string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "d", forplayerid))
		return Msg(playerid, COLOR_ERROR, "Wpisz: {b}/jetpack [id gracza]{/b}");

	SetPlayerSpecialAction(forplayerid, 2);
	format(string, sizeof string, "Da³eœ jetpack graczowi %s.", PlayerName(forplayerid));
	Msg(playerid, COLOR_INFO, string);

	format(string, sizeof string, "Otrzyma³eœ {b}jetpacka{/b} od administratora {b}%s{/b}.", PlayerName(playerid));
	Msg(playerid, COLOR_INFO, string);
	return 1;
}

CMD:uping(playerid, params[])
{
	new forplayerid, high, string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie jest obecny na serwerze..");

	if(sscanf(params, "dd", forplayerid, high))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /uping [id gracza] [wysokoœæ]");

	new Float:t[3];
	new vehicleid = GetPlayerVehicleID(forplayerid);
	GetPlayerPos(forplayerid, t[0], t[1], t[2]);
	format(string, sizeof string, "Zmieni³eœ pozycjê graczowi {b}%s{/b}.", PlayerName(forplayerid));
	Msg(playerid, COLOR_INFO, string);

	format(string, sizeof string, "Twoja pozycja zosta³ zmieniona przez {b}%s{/b}.",PlayerName(playerid));
	Msg(forplayerid, COLOR_INFO, string);

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
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie jest obecny na serwerze..");

	if(sscanf(params, "dd", forplayerid, high))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /downing [id gracza] [wysokoœæ]");

	new Float:t[3];
	new vehicleid = GetPlayerVehicleID(forplayerid);
	GetPlayerPos(forplayerid, t[0], t[1], t[2]);
	format(string, sizeof string, "Zmieni³eœ pozycjê graczowi {b}%s{/b}.", PlayerName(forplayerid));
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "Twoja pozycja zosta³ zmieniona przez {b}%s{/b}.",PlayerName(playerid));
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

stock SprawdzPoziom(playerid)
{
	new levelChange;
	new score = GetScore(playerid);
	new string[186];

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

			format(string, sizeof string, "Gratulacje! Awansujesz na {b}%d{/b} poziom ({b}%s{/b}). W nagrodê otrzymujesz {b}$%d{/b} oraz {b}1 punkt zdolnoœci{/b}.", GetPlayerLevel(score), GetPlayerLevelName(GetPlayerLevel(score)), levelChange*2500);
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

	new 
		lastScore = (GetPVarInt(playerid, "LEVEL") > 1) ? (DoswiadczeniePoziomy[GetPVarInt(playerid, "LEVEL")]) : (0),
		Float:percent = floatmul(floatdiv(((GetScore(playerid) - lastScore)), (DoswiadczeniePoziomy[GetPVarInt(playerid, "LEVEL")+1] - DoswiadczeniePoziomy[GetPVarInt(playerid, "LEVEL")])), 100.0);
	SetPlayerProgressBarValue(playerid, hudInfo[tdLevelProgress][playerid], percent);

	return 1;
}

CMD:ladownosc(playerid, params[])
{
	new vehicleid = GetPlayerVehicleID(playerid);
	new trailerid = GetVehicleTrailer(vehicleid);
	new string[126];

	if(!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie znajdujesz siê w pojeŸdzie.");

	new model = GetVehicleModel(vehicleid);

	if(model == 515 || model == 403 || model == 514)
	{
		if(!GetVehicleTrailer(vehicleid))
			return Msg(playerid, COLOR_ERROR, "Nie posiadasz naczepy.");

		format(string, sizeof string, "Ten pojazd/naczepa mieœci maksymalnie {b}%d{/b} t.", MaxWeight(GetVehicleModel(trailerid)));
		Msg(playerid, COLOR_ERROR, string);
	}
	else
	{
		if(!IsVehicleVan(model))
			return Msg(playerid, COLOR_ERROR, "Ten pojazd nie nadaje siê do przewo¿enia towarów.");
		format(string, sizeof string, "Ten pojazd/naczepa mieœci maksymalnie {b}%d{/b} t.", MaxWeight(GetVehicleModel(vehicleid)));
		Msg(playerid, COLOR_ERROR, string);
	}
	return 1;
}

CMD:open(playerid, params[])
{
	if(playerInfo[playerid][pFirm] == 0 || !GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	Otworzbrame(playerid);
	Msg(playerid, COLOR_INFO, "Brama zosta³a otwarta.");
	return 1;
}

CMD:close(playerid, params[])
{
	if(playerInfo[playerid][pFirm] == 0 || !GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	Zamknijbrame(playerid);
	Msg(playerid, COLOR_INFO, "Brama zosta³a zamkniêta.");
	return 1;
}

CMD:o(playerid, params[])
{
	if(firmInfo[playerInfo[playerid][pFirm]][tType] != TEAM_TYPE_MEDIC)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	new gate = strval(params);
	if(gate < 1 || gate > 8)
		return Msg(playerid, COLOR_ERROR, "Poda³eœ z³e ID bramy.");

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
	Msg(playerid, COLOR_INFO, "Gara¿ zosta³ {b}otwarty{/b}.");
	return 1;
}

CMD:c(playerid, params[])
{
	if(firmInfo[playerInfo[playerid][pFirm]][tType] != TEAM_TYPE_MEDIC)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	new gate = strval(params);
	if(gate < 1 || gate > 8)
		return Msg(playerid, COLOR_ERROR, "Poda³eœ z³e ID bramy.");

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
	Msg(playerid, COLOR_INFO, "Gara¿ zosta³ {b}zamkniêty{/b}.");
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
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnieñ");

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
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnieñ");

	format(string,sizeof(string),"~w~%s",tekst);
	GameTextForAll(string,(czas*1000),3);
	return 1;
}

CMD:say(playerid, params[])
{
	new tekst[100], string[200];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnieñ.");

	if(sscanf(params, "s[100]", tekst))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /say [tekst]");

	format(string, sizeof(string), "Administrator: %s", tekst);
	MsgToAll(COLOR_INFO2, string, false);

	ToLog(playerInfo[playerid][pID], LOG_TYPE_CHAT, "adminglobal", params);
	return 1;
}

CMD:wejdz(playerid)
{
	for(new i = 0; i < MAX_RESTAURANTS; i++)
	{
		if(!IsValidRestaurant(i))
			continue;

		if(IsPlayerInRangeOfPoint(playerid, 4.0, resInfo[i][resPos][0], resInfo[i][resPos][1], resInfo[i][resPos][2]))
		{
			new Float:pos[3];
			GetPosRestaurantInterior(GetRestaurantInterior(i), pos[0], pos[1], pos[2]);
			SetPlayerInterior(playerid, GetRestaurantInterior(i));
			SetPlayerPos(playerid, pos[0], pos[1], pos[2]);
			SetPlayerVirtualWorld(playerid, i+1);
			SetPVarInt(playerid, "restaurandID", i+1);
			Msg(playerid, COLOR_INFO, "Witaj w lokalu! Aby coœ zamówiæ, podejdŸ do lady i wpisz {b}/menu{/b}.");
			return 1;
		}
	}
	return 1;
}

CMD:wyjdz(playerid)
{
	if(!GetPVarInt(playerid, "restaurandID"))
		return Msg(playerid, COLOR_ERROR, "Nie jesteœ w restauracji.");

	if(!IsValidRestaurant(GetPVarInt(playerid, "restaurandID")-1))
		return Msg(playerid, COLOR_ERROR, "Nieoczekiwany b³¹d...");

	for(new i = 0; i < 18; i++)
	{
		if(!IsValidRestaurantInterior(i))
			continue;

		new Float:pos[3];
		GetPosRestaurantInterior(i, pos[0], pos[1], pos[2]);
		if(IsPlayerInRangeOfPoint(playerid, 5.0, pos[0], pos[1], pos[2]))
		{
			SetPlayerInterior(playerid, 0);
			SetPlayerVirtualWorld(playerid, 0);
			Teleport(playerid, resInfo[GetPVarInt(playerid, "restaurandID")-1][resPos][0], resInfo[GetPVarInt(playerid, "restaurandID")-1][resPos][1], resInfo[GetPVarInt(playerid, "restaurandID")-1][resPos][2], false, true);
			DeletePVar(playerid, "restaurandID");
			Msg(playerid, COLOR_INFO, "Zapraszamy ponownie!");
			return 1;
		}
	}
	Msg(playerid, COLOR_ERROR, "Nie jesteœ w punkcie wyjœcia z restauracji.");
	return 1;
}

CMD:menu(playerid)
{
	if(!GetPVarInt(playerid, "restaurandID"))
		return Msg(playerid, COLOR_ERROR, "Nie jesteœ w restauracji.");

	if(!IsValidRestaurant(GetPVarInt(playerid, "restaurandID")-1))
		return Msg(playerid, COLOR_ERROR, "Nieoczekiwany b³¹d...");

	for(new i = 0; i < 18; i++)
	{
		if(!IsValidRestaurantInterior(i))
			continue;

		new Float:pos[3];
		GetInteriorMenuPos(i, pos[0], pos[1], pos[2]);
		if(IsPlayerInRangeOfPoint(playerid, 5.0, pos[0], pos[1], pos[2]))
		{
			new szString[256], szTemp[64];
			for(new d = 0; d < sizeof menuList; d++)
			{
				format(szTemp, sizeof szTemp, "%s\t%d $\t%d %%\n", menuList[d][menName], menuList[d][menPrice], menuList[d][menHunger]);
				strcat(szString, szTemp);
			}
			strins(szString, "Nazwa\tCena\tG³ód\n", 0);
			Dialog_Show(playerid, DIALOG_MENU, DIALOG_STYLE_TABLIST_HEADERS, "Menu", szString, "Wybierz", "WyjdŸ");
			return 1;
		}
	}
	return 1;
}

Dialog:DIALOG_MENU(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	if(GetMoney(playerid) < menuList[listitem][menPrice])
	{
		cmd_menu(playerid);
		Msg(playerid, COLOR_ERROR, "Nie posiadasz wystarczaj¹co du¿o gotówki.");
		return 1;
	}
	if(playerInfo[playerid][pHunger] >= 100.0)
	{
		cmd_menu(playerid);
		Msg(playerid, COLOR_ERROR, "Wiêcej jedzenia nie pomieœcisz!");
		return 1;
	}
	playerInfo[playerid][pHunger] += float(menuList[listitem][menHunger]);
	if(playerInfo[playerid][pHunger] > 100.0)
		playerInfo[playerid][pHunger] = 100.0;

	SetPlayerProgressBarValue(playerid, hudInfo[tdHungerProgress][playerid], playerInfo[playerid][pHunger]);
	PreloadAnimLib(playerid, "FOOD");
	ApplyAnimation(playerid, "FOOD", "EAT_Burger", 4.1, 0, 1, 1, 0, 0);
	GiveMoney(playerid, -menuList[listitem][menPrice]);
	cmd_menu(playerid);
	return 1;
}

stock GetInteriorMenuPos(interior, &Float:x, &Float:y, &Float:z)
{
	switch(interior)
	{
		case 1:
		{
			x = -782.5031;  y = 500.4179; z = 1371.7490;
		}
		case 4:
		{
			x = -29.8513; y = -28.5476; z = 1003.5573; // interior_4
		}
		case 5:
		{
	 		x = 372.7253; y = -119.1653; z = 1001.4922; // interior_5
		}
		case 6:
		{
			x = -22.1502; y = -55.3843; z = 1003.5469; // interior_6
		}
		case 9:
		{
			x = 368.1111; y = -6.2089; z = 1001.8516;  // interior_9
		}
		case 10:
		{
			x = 3.1212; y = -29.0141; z = 1003.5494; // interior_10
		}
		case 17:
		{
			x = 380.7486; y = -191.0910; z = 1000.6328; // interior_17
		}
	}
	return 1;
}

stock IsValidRestaurant(resid)
{
	if(resInfo[resid][resCreated])
		return true;
	return false;
}

forward SendRandomMessage();
public SendRandomMessage()
{
	new szString[128];
	format(szString, sizeof szString, "%s", szRandomMessages[random(sizeof(szRandomMessages))]);
	MsgToAll(COLOR_INFO2, szString);
	new nextRandom = random(200) + 300;
	SetTimer_("SendRandomMessage", 0, nextRandom*1000, 1);
	return 1;
}

CMD:afk(playerid)
{
	if(!(IsWorked(playerid, TEAM_TYPE_POMOC) || IsWorked(playerid, TEAM_TYPE_MEDIC) || IsWorked(playerid, TEAM_TYPE_POLICE)))
		return Msg(playerid, COLOR_ERROR, "Nie mo¿esz tego zrobiæ");

	if(GetPVarInt(playerid, "otherAFK"))
	{
		SetPVarInt(playerid, "otherAFK", 0);
		Msg(playerid, COLOR_INFO, "Wróci³eœ z AFK.");
		TogglePlayerControllable(playerid, true);
	}
	else
	{
		SetPVarInt(playerid, "otherAFK", 1);
		Msg(playerid, COLOR_INFO, "Za³¹czy³eœ tryb AFK.");
		TogglePlayerControllable(playerid, false);
	}
	return 1;
}

forward OnLightFlash(vehicleid);
public OnLightFlash(vehicleid)
{
	new panels, doors, lights, tires;
	GetVehicleDamageStatus(vehicleid, panels, doors, lights, tires);

	RepairVehicle(vehicleid);
	switch(_Flash[vehicleid])
	{
		case 0: UpdateVehicleDamageStatus(vehicleid, 0b1111, 0b1111, 2, 0b0000);

		case 1: UpdateVehicleDamageStatus(vehicleid, 0b1111, 0b1111, 5, 0b0000);

		case 2: UpdateVehicleDamageStatus(vehicleid, 0b1111, 0b1111, 2, 0b0000);

		case 3: UpdateVehicleDamageStatus(vehicleid, 0b1111, 0b1111, 4, 0b0000);

		case 4: UpdateVehicleDamageStatus(vehicleid, 0b1111, 0b1111, 5, 0b0000);

		case 5: UpdateVehicleDamageStatus(vehicleid, 0b1111, 0b1111, 4, 0b0000);
	}
	if(_Flash[vehicleid] >=5) _Flash[vehicleid] = 0;
	else _Flash[vehicleid] ++;
	return 1;
}

public OnVehicleSirenStateChange(playerid, vehicleid, newstate)
{
	if((enabledFlashes <= 20) && !Spawned[vehicleid] && (IsWorked(playerid, TEAM_TYPE_MEDIC) || IsWorked(playerid, TEAM_TYPE_POLICE)))
	{
		if(newstate)
		{
			__FlashTime[vehicleid] = SetTimerEx_("OnLightFlash", 0, 600, -1, "i", vehicleid);
			Msg(playerid, COLOR_INFO, "Syreny za³¹czone.");
			enabledFlashes ++;
		}
		
		if(!newstate)
		{
			Msg(playerid, COLOR_INFO, "Syreny wy³¹czone.");
			new panels, doors, lights, tires;
			enabledFlashes --;
			KillTimer_(__FlashTime[vehicleid]);
			
			RepairVehicle(vehicleid);
			GetVehicleDamageStatus(vehicleid, panels, doors, lights, tires);
			UpdateVehicleDamageStatus(vehicleid, panels, doors, 0, tires);
		}
	}
		return 1;
}

CMD:butla(playerid)
{
	if(!IsWorked(playerid, TEAM_TYPE_POMOC))
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");
	new vehicleid = GetPlayerVehicleID(playerid);
	if(vehicleid <= 0)
		return Msg(playerid, COLOR_ERROR, "Nie siedzisz w pojeŸdzie.");
	if(Spawned[vehicleid])
		return Msg(playerid, COLOR_ERROR, "Nie mo¿esz zamontowaæ butli w tym pojeŸdzie.");

	new Float:pos[3], seat = GetPlayerVehicleSeat(playerid), vehid = DBVehID[vehicleid];
	GetVehiclePos(vehicleid, pos[0], pos[1], pos[2]);
	vehInfo[DBVehID[vehicleid]][vGasBootle] = true;
	vehInfo[DBVehID[vehicleid]][vGasAmount] = 25.0;
	SaveVehicle(vehicleid);
	DestroyVehicle(vehicleid);
	DBVehID[vehicleid] = 0;
	ResetVariablesInEnum(vehInfo[DBVehID[vehicleid]], E_VEHICLE);
	LadujPojazd(_, _, vehid);
	PutPlayerInVehicle(playerid, vehInfo[vehid][vSAMPID], seat);

	Msg(playerid, COLOR_INFO, "Butla gazowa zosta³a pomyœlnie zamontowana.");
	return 1;
}

CMD:respawnveh(playerid, params[])
{
	new targetid;
	if(playerInfo[playerid][pAdmin] < 1)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "d", targetid))
		return Msg(playerid, COLOR_ERROR, "Wpisz /respawnveh [id gracza]");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie jest online.");

	if(!IsPlayerInAnyVehicle(targetid))
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie siedzi w pojeŸdzie.");

	if(GetPlayerState(targetid) != PLAYER_STATE_DRIVER)
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie jest kierowc¹.");

	new vehicle = GetPlayerVehicleID(targetid);
	SetVehicleToRespawn(vehicle);

	new szString[128];
	format(szString, sizeof szString, "Administrator {b}%s{/b} zrespawnowa³ Twój pojazd.", PlayerName(playerid));
	Msg(targetid, COLOR_INFO, szString);

	format(szString, sizeof szString, "Zrespawnowa³eœ pojazd graczowi {b}%s{/b}.", PlayerName(targetid));
	Msg(playerid, COLOR_INFO, szString);
	return 1;
}

CMD:weapons(playerid)
{
	if(playerInfo[playerid][pAdmin] < 1)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	SetPlayerHealth(playerid, 100.0);
	SetPlayerArmour(playerid, 100.0);
	GivePlayerWeapon(playerid, 1, 999999);
	GivePlayerWeapon(playerid, 24, 999999);
	GivePlayerWeapon(playerid, 27, 999999);
	GivePlayerWeapon(playerid, 29, 999999);
	GivePlayerWeapon(playerid, 31, 999999);
	GivePlayerWeapon(playerid, 38, 999999);
	GivePlayerWeapon(playerid, 46, 999999);

	Msg(playerid, COLOR_INFO, "Pomyœlnie zabrano zestaw broni administratora.");
	return 1;
}

CMD:infoplayer(playerid, params[])
{
	new targetid;
	if(playerInfo[playerid][pAdmin] < 1)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "d", targetid))
		return Msg(playerid, COLOR_ERROR, "Wpisz /infoplayer [id gracza]");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie jest online.");

	new string[500], czasBiezacy[3], ip[20];

	GetPVarString(targetid, "pAJPI", ip, sizeof ip);

	ConvertSeconds(GetDTime(targetid), czasBiezacy[0], czasBiezacy[1], czasBiezacy[2]);
	format(string, sizeof(string), "UID:\t %d\n", playerInfo[targetid][pID]);
	format(string, sizeof(string), "%sNick:\t %s\n", string, PlayerName(targetid));
	format(string, sizeof(string), "%sPieni¹dze:\t %d\n", string, GetMoney(targetid));
	format(string, sizeof(string), "%sBankomat:\t %d\n", string, playerInfo[targetid][pBankomat]);
	format(string, sizeof(string), "%sPunkty:\t %d\n", string, GetScore(targetid));
	format(string, sizeof(string), "%sPoziom:\t %s (%d)\n", string, GetPlayerLevelName(GetPVarInt(targetid, "LEVEL")), GetPVarInt(targetid, "LEVEL"));
	format(string, sizeof(string), "%sFirma:\t %d\n", string, playerInfo[targetid][pFirm]);
	format(string, sizeof(string), "%sTachograf:\t %dh %dm %ds\n", string, czasBiezacy[0], czasBiezacy[1], czasBiezacy[2]);
	format(string, sizeof(string), "%sViaToll:\t %d\n", string, GetViaMoney(targetid));
	format(string, sizeof(string), "%sADR:\t %d\n", string, playerInfo[targetid][pADR]);
	format(string, sizeof(string), "%sIP:\t %s\n", string, ip);

	Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_TABLIST, " ", string, "WyjdŸ", #);
	return 1;
}

forward ExplodePlayerTires(playerid);
public ExplodePlayerTires(playerid)
{
	if(IsPlayerInAnyVehicle(playerid) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		new tire[4], panels, doors, lights, tires;
		GetVehicleDamageStatus(GetPlayerVehicleID(playerid), panels, doors, lights, tires);
		decode_tires(tires, tire[0], tire[1], tire[2], tire[3]);
		tire[random(3)] = 0;
		tires = encode_tires(tire[0], tire[1], tire[2], tire[3]);
		UpdateVehicleDamageStatus(GetPlayerVehicleID(playerid), panels, doors, lights, tires);
		TextDrawShowForPlayer(playerid, TireTD);
		SetTimerEx_("HideTireTD", 0, 5*1000, 1, "i", playerid);
	}
	new time = 15 + random(15);
	SetPVarInt(playerid, "TireTimer", SetTimerEx_("ExplodePlayerTires", 0, time*1000, 1, "i", playerid));
	return 1;
}

forward HideTireTD(playerid);
public HideTireTD(playerid)
{
	TextDrawHideForPlayer(playerid, TireTD);
	return 1;
}

encode_tires(tire1, tire2, tire3, tire4)
{
	return tire1 | (tire2 << 1) | (tire3 << 2) | (tire4 << 3);
}

decode_tires(tires, &tire1, &tire2, &tire3, &tire4)
{
	tire1 = tires & 1;
	tire2 = tires >> 1 & 1;
	tire3 = tires >> 2 & 1;
	tire4 = tires >> 3 & 1;
}

#include "include/gui.inc"
#include "include/commands.inc"
#undef MAX_PLAYERS

#define MAX_PLAYERS 		25
#define MAX_LOADING_POINTS 	300
#define MAX_TOWAROW 		300
#define MAX_STACJE 			200
#define MAX_FOTORADAROW 	1000
#define MAX_VIASHOP 		200
#define MAX_VEHICLES_DB 	5000
#define MAX_FIRM 			13
#define MAX_SPIKESTRIPS 	25
#define MAX_BARY 			200
#define MAX_BARYIN 			200
#define MAX_BTOBJ 			5000
#define MAX_BTOBJTD 		5000

// ---------------------------

#define INVALID_FOTO_ID 		0
#define INVALID_VIASHOP_ID 		0
#define INVALID_BARY_ID 		0
#define INVALID_BTOBJ_ID 		0
#define INVALID_REMONTBT3D_ID 	0
#define INVALID_DOK_ID 			0
#define INVALID_BARYIN_ID 		0

// ---------------------------
enum {
	TEAM_TYPE_TRUCKER=0,
	TEAM_TYPE_POLICE,
	TEAM_TYPE_MEDIC,
	TEAM_TYPE_POMOC,
	TEAM_TYPE_CARGO,
	TEAM_TYPE_PETROL,
	TEAM_TYPE_SMLOG,
	TEAM_TYPE_BUILD
}

// ---------------------------

native IsValidVehicle(vehicle);

// ---------------------------

#define isnull(%1) ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#define Loop(%1,%2) for(new %1 = 0; %1 < %2; %1++)
#define ResetVariablesInEnum(%0,%1) 	for(new __rvine; %1:__rvine != %1; __rvine++) %0[%1:__rvine]=0
#define ShowInfo(%0,%1) Dialog_Show(%0, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, "{FF0000}Informacja", %1, "Ok", "")

#define LIGHTGREEN 			0xBCEE68FF
#define LIGHTBLUE 			0xA8C3E3FF
#define LIGHTRED 			0xEE5555FF
#define YELLOW 				0xEAAF00FF
#define ORANGE 				0xFFAB00FF
#define WHITE 				0xFFFFFFFF
#define GREY 				0xB2BBBFFF
#define ZIELONY99 			0x00FF00FF
#define ZIELONY5 			0x00FF00FF
#define ZIELONY6 			0x00FF00FF
#define ZIELONY4 			0x00FF00FF
#define LIGHTRED 			0xEE5555FF
#define ZIELONY 			0x00FF00FF
#define ZIELONY2 			0x00FF00FF
#define ZIELONY3 			0x00FF00FF
#define ZIELONY4 			0x00FF00FF

// ---------------------------

#define GCOLOR_GREEN 		"{BCEE68}"
#define GCOLOR_WHITE 		"{FFFFFF}"
#define GCOLOR_RED 			"{FF4040}"
#define GCOLOR_VIOLET 		"{BF3EFF}"
#define GCOLOR_GREY 		"{A9A9A9}"
#define GCOLOR_GUI 			"{a9c4e4}"
#define GCOLOR_YELLOW 		"{FFB90F}"
#define GCOLOR_CREAM 		"{FFD39B}"
#define COLOR_GUI 			"{a9c4e4}"

// ---------------------------

#define COLOR_INFO 			0
#define COLOR_INFO2			1
#define COLOR_INFO3			2
#define COLOR_ERROR			3

// ---------------------------

new szCommands[][] = {
	"Pusto", // all commands
	"/firma /mandat /aresztuj\n/wywal /wyrzuć /praca\n/dyzur /skuj /rozkuj\n/kontrola /alkomat /open\n /close /kolczatka /usunkolczatke\n /usunkolczatki /stop /unareszt",
	"/firma /ulecz /wypadki /wywal /wyrzuć /praca /dyzur /o[1 - 8] /c[1 - 8] /open /close", // pogotowie
	"/firma /vehcolor /rachunek /zatankuj /praca /dyzur /open /close /setplate /przeglad"  // pomoc drogowa
};

// ---------------------------

#define KEY_AIM 			132

#define MAX_ADMIN_VEHICLES 	50
new AdminsVehs[MAX_ADMIN_VEHICLES];
new SpawnedAdminVehicles;

new Text3D:Trucking[MAX_PLAYERS];
new Text:FleshText;
new Text3D:viashop3D[MAX_VIASHOP];
new viashopIKON[MAX_VIASHOP];
new Text3D:bary3D[MAX_BARY];
new baryIKON[MAX_BARY];
new Text3D:baryin3D[MAX_BARYIN];
new podgladADMIN[MAX_PLAYERS] = 0;
new Firehold[MAX_PLAYERS];
new Firehold2[MAX_PLAYERS] = 0;
new kogutPD[MAX_VEHICLES],
    bool:kpd[MAX_VEHICLES];
new obiektyBT[MAX_BTOBJ];
new obiektyBTID = 0;
new kogutPOLI[MAX_VEHICLES],
    bool:kpoli[MAX_VEHICLES];
    
new Text3D:obiektyBT3D[MAX_BTOBJTD];
new bool:IsObiektBT[MAX_BTOBJ];
new Text:AlertTD;

new const DoswiadczeniePoziomy[] = 
{
	(-1),
	0,
	10,
	20,
	35,
	45,
	60,
	80,
	120,
	150,
	195,
	240,
	287,
	355,
	410,
	476,
	524,
	588,
	680,
	735,
	845,
	940,
	1120,
	1300,
	1510,
	1588,
	1630,
	1672,
	1742,
	2000,
	2450,
	2725,
	3010,
	3550,
	3900,
	4450
};

#define MAX_RADIO 100

enum sInfo
{
	sID,
	sName[32],
	sURL[176]
};
new RadioStations[MAX_RADIO][sInfo];

enum {
	FUEL_TYPE_GAS=0,
	FUEL_TYPE_BENZ,
	FUEL_TYPE_TDI
}

enum {
	OWNER_TYPE_PLAYER=0,
	OWNER_TYPE_TEAM,
	OWNER_TYPE_SPAWNED
}
enum vInfo
{
	vID,
	vModel,
	vOwnerID,
	vOwnerType,
	Float:vPosX,
	Float:vPosY,
	Float:vPosZ,
	Float:vPosA,
	Float:vHealth,
	Float:vFuel,
	vFuelType,
	Float:vPrzebieg,
	vColor1,
	vColor2,
	vLocked,
	vPanels,
	vDoors,
	vLights,
	vTires,
	vOwnerVC,
	vPlate[32],
	vPrzeglad[32],
	vSAMPID
};
new vehInfo[MAX_VEHICLES_DB][vInfo];
new Float:vPos[MAX_VEHICLES][6];
new DBVehID[MAX_VEHICLES_DB];
new HighVehicleID=INVALID_VEHICLE_ID;
new VehicleDriver[MAX_VEHICLES]=INVALID_PLAYER_ID;

new timer[MAX_PLAYERS],
	timer3[MAX_PLAYERS],
	timer4[MAX_PLAYERS],
	timer5[MAX_PLAYERS],
	timer7[MAX_PLAYERS],
	timer8[MAX_PLAYERS],
	timer9[MAX_PLAYERS],
	timer10[MAX_PLAYERS],
	timer11[MAX_PLAYERS],
	timer12[MAX_PLAYERS],
	timer13[MAX_PLAYERS],
	timer15[MAX_PLAYERS],
	timer16[MAX_PLAYERS];

new UsersConnected;

enum vOtherInfo {
	Float:vFuel,
	vFuelType,
	Float:vPrzebieg,
	vCargoID,
	vCargoWeight,
	vCargoLoadTime,
	vCargoLoadTimer,
	vCargoLoadPoint,
	vCargoUnLoadTime,
	Color1,
	Color2
}
new vehOtherInfo[MAX_VEHICLES][vOtherInfo];
new bool:Spawned[MAX_VEHICLES];

enum Player {
	pID,
	pMoney,
	pScore,
	pAdmin,
	pBankomat,
	pFirma,
	pSkin,
	pTacho,
	pFoto,
	pToll,
	pGTime,
	pWorkTime,
	pBuildmaster,
	pADR,
	bool:pInvited[8],
	chained,
	chainedTimer
}
new PlayerInfo[MAX_PLAYERS][Player];

enum Stations {
	sID,
	Float:sPosX,
	Float:sPosY,
	Float:sPosZ,
	Zbiornik
}
new Stacje[MAX_STACJE][Stations];
new Text3D:StacjeText3D[MAX_STACJE];
new StacjeMapIcon[MAX_STACJE][MAX_PLAYERS];
new ID_STACJA=1;

enum Teams {
	tID,
	tName[32],
	tChef,
	tVcChef,
	tKsiegowy,
	tTyp,
	tBank,
	Float:tSpawnX,
	Float:tSpawnY,
	Float:tSpawnZ,
	tSkin1,
	tSkin2,
	tSkin3,
	tSkin4,
	tSkin5,
	tKolor
}
new Firmy[MAX_FIRM][Teams];
new NaDyzurze[MAX_FIRM];

new const KategorieTowarow[][] = {
	"Budowlane",
	"Elektroniczne",
	"Art. spożywcze",
	"Art. odzieżowe",
	"Niebezpieczne",
	"Nielegalne"
};

new colors[][] = {
	"niewidoczny", "biały", "pomarańczowy", "czerwony", "różowy",
	"żółty", "brązowy", "zielony", "jasnoniebieski", "niebieski"
};

enum {
	TYPE_BUILD,
	TYPE_ELECTRONIC,
	TYPE_EAT,
	TYPE_CLOTHES,
	TYPE_DANGEROUS,
	TYPE_ILLEGAL
}

enum Points
{
	lID,
	PointName[32],
	Float:PointPosX,
	Float:PointPosY,
	Float:PointPosZ,
	pType
};

new LoadingPoints[MAX_LOADING_POINTS][Points];
new LoadingPoinstMapIcon[MAX_LOADING_POINTS][MAX_PLAYERS];
new Text3D:LoadingPointsText3D[MAX_LOADING_POINTS];
new ID_DOKOW=1;

enum Towarys
{
	tID,
	Typ,
	Float:Stawka,
	MinimalnyCzas,
	Poziom,
	Adr,
	NazwaTowaru[40]
};
new Towary[MAX_TOWAROW][Towarys];

enum GPSs
{
	bool:GPSDoki,
	bool:GPSStacje,
	bool:GPSBary,
	bool:GPSViatoll
}
new GPSGracz[MAX_PLAYERS][GPSs];
new KtosJuzAktualizujeGPSa;

enum eFoto {
	fID,
	Float:fPosX,
	Float:fPosY,
	Float:fPosZ,
	Float:fRX,
	Float:fRY,
	Float:fRZ,
	fDay,
	fYear,
	fMaxSpeed,
	fName[30]
}
new Fotoradary[MAX_FOTORADAROW][eFoto];
new ID_FOTO=1;

enum eViaToll {
	vtID,
	Float:vtPosX,
	Float:vtPosY,
	Float:vtPosZ
}
new ViaToll[MAX_VIASHOP][eViaToll];
new ID_VIATOLL=1;

enum {
	LOG_TYPE_CHAT=0,
	LOG_TYPE_COMMANDS,
	LOG_TYPE_PLAYER
}

enum sxInfo
{
	sCreated,
	Float:sX,
	Float:sY,
	Float:sZ,
	sObject,
	Text3D:sText
}

new ssInfo[MAX_SPIKESTRIPS][sxInfo];

new FotoradarOBJ[MAX_FOTORADAROW];

new VehicleNames[212][] = {
{"Landstalker"},{"Bravura"},{"Buffalo"},{"Linerunner"},{"Perrenial"},{"Sentinel"},{"Dumper"},
{"Firetruck"},{"Trashmaster"},{"Stretch"},{"Manana"},{"Infernus"},{"Voodoo"},{"Pony"},{"Mule"},
{"Cheetah"},{"Ambulance"},{"Leviathan"},{"Moonbeam"},{"Esperanto"},{"Taxi"},{"Washington"},
{"Bobcat"},{"Mr Whoopee"},{"BF Injection"},{"Hunter"},{"Premier"},{"Enforcer"},{"Securicar"},
{"Banshee"},{"Predator"},{"Bus"},{"Rhino"},{"Barracks"},{"Hotknife"},{"Trailer 1"},{"Previon"},
{"Coach"},{"Cabbie"},{"Stallion"},{"Rumpo"},{"RC Bandit"},{"Romero"},{"Packer"},{"Monster"},
{"Admiral"},{"Squalo"},{"Seasparrow"},{"Pizzaboy"},{"Tram"},{"Trailer 2"},{"Turismo"},
{"Speeder"},{"Reefer"},{"Tropic"},{"Flatbed"},{"Yankee"},{"Caddy"},{"Solair"},{"Berkley's RC Van"},
{"Skimmer"},{"PCJ-600"},{"Faggio"},{"Freeway"},{"RC Baron"},{"RC Raider"},{"Glendale"},{"Oceanic"},
{"Sanchez"},{"Sparrow"},{"Patriot"},{"Quad"},{"Coastguard"},{"Dinghy"},{"Hermes"},{"Sabre"},
{"Rustler"},{"ZR-350"},{"Walton"},{"Regina"},{"Comet"},{"BMX"},{"Burrito"},{"Camper"},{"Marquis"},
{"Baggage"},{"Dozer"},{"Maverick"},{"News Chopper"},{"Rancher"},{"FBI Rancher"},{"Virgo"},{"Greenwood"},
{"Jetmax"},{"Hotring"},{"Sandking"},{"Blista Compact"},{"Police Maverick"},{"Boxville"},{"Benson"},
{"Mesa"},{"RC Goblin"},{"Hotring Racer A"},{"Hotring Racer B"},{"Bloodring Banger"},{"Rancher"},
{"Super GT"},{"Elegant"},{"Journey"},{"Bike"},{"Mountain Bike"},{"Beagle"},{"Cropdust"},{"Stunt"},
{"Tanker"}, {"Roadtrain"},{"Nebula"},{"Majestic"},{"Buccaneer"},{"Shamal"},{"Hydra"},{"FCR-900"},
{"NRG-500"},{"HPV1000"},{"Cement Truck"},{"Tow Truck"},{"Fortune"},{"Cadrona"},{"FBI Truck"},
{"Willard"},{"Forklift"},{"Tractor"},{"Combine"},{"Feltzer"},{"Remington"},{"Slamvan"},
{"Blade"},{"Freight"},{"Streak"},{"Vortex"},{"Vincent"},{"Bullet"},{"Clover"},{"Sadler"},
{"Firetruck LA"},{"Hustler"},{"Intruder"},{"Primo"},{"Cargobob"},{"Tampa"},{"Sunrise"},{"Merit"},
{"Utility"},{"Nevada"},{"Yosemite"},{"Windsor"},{"Monster A"},{"Monster B"},{"Uranus"},{"Jester"},
{"Sultan"},{"Stratum"},{"Elegy"},{"Raindance"},{"RC Tiger"},{"Flash"},{"Tahoma"},{"Savanna"},
{"Bandito"},{"Freight Flat"},{"Streak Carriage"},{"Kart"},{"Mower"},{"Duneride"},{"Sweeper"},
{"Broadway"},{"Tornado"},{"AT-400"},{"DFT-30"},{"Huntley"},{"Stafford"},{"BF-400"},{"Newsvan"},
{"Tug"},{"Trailer 3"},{"Emperor"},{"Wayfarer"},{"Euros"},{"Hotdog"},{"Club"},{"Freight Carriage"},
{"Trailer 3"},{"Andromada"},{"Dodo"},{"RC Cam"},{"Launch"},{"Police Car (LSPD)"},{"Police Car (SFPD)"},
{"Police Car (LVPD)"},{"Police Ranger"},{"Picador"},{"S.W.A.T. Van"},{"Alpha"},{"Phoenix"},{"Glendale"},
{"Sadler"},{"Luggage Trailer A"},{"Luggage Trailer B"},{"Stair Trailer"},{"Boxville"},{"Farm Plow"},
{"Utility Trailer"}};

new Float:viaPoints[][] =
{
    {2343.3713,834.1098,6.7344},
	{2342.8552,852.7947,6.7344},
	{2342.8552,852.7947,6.7344},
	{2709.0635,1727.1296,6.7344},
	{1880.1187,2523.7358,6.8112},
	{1887.0364,2506.6235,6.8095},
	{1497.9585,2473.4600,6.7421},
	{1497.2330,2454.0833,6.7344},
	{927.8715,2605.2581,10.4440},
	{918.6038,2589.4097,10.3475},
	{1208.7083,2032.0959,6.7344},
	{1226.3341,2029.1606,6.7344},
	{1207.9796,1339.0520,6.7344},
	{1226.3348,1337.4695,6.7344},
	{1806.9712,1078.4344,6.7344},
	{1786.8424,1080.1672,6.7420},
	{1805.8663,2027.7371,3.9063},
	{1788.1750,2028.1434,3.9063},
	{931.2241,742.9670,10.6972},
	{936.8785,724.9408,10.6991},
	{55.7237,668.2198,5.9933},
	{65.7794,650.0699,6.2311},
	{-1101.6611,1544.8802,27.2211},
	{-1127.2782,1538.1731,21.9345},
	{-1679.2386,2167.9458,20.5576},
	{-1701.1515,2148.0486,18.2075},
	{-2716.1616,2640.0427,88.3590},
	{-2671.3555,2577.1851,75.4093},
	{-1140.9016,1109.9440,38.2559},
	{-1131.0656,1098.9979,38.3143},
	{-1910.8617,-1082.6947,38.2344},
	{-1896.8868,-1082.9814,38.2344},
	{-2900.0620,-1709.4854,22.1113},
	{-2929.8584,-1715.5850,19.0224},
	{-2699.3176,-401.7303,7.9082},
	{-2707.4353,-390.6496,7.9113},
	{-2341.4968,-2580.8574,46.3502},
	{-2349.1631,-2615.9939,58.7623},
	{-1342.6294,-2875.8237,55.3919},
	{-1326.8586,-2919.8411,49.8097},
	{-0.1825,-2607.7610,39.8488},
	{-35.0069,-2613.9233,43.7653},
	{92.8385,-1284.4017,13.8588},
	{76.7223,-1263.8002,14.2713},
	{170.6697,-1602.8448,13.8510},
	{182.8681,-1593.4392,13.8707},
	{1038.4319,-1941.4056,13.2205},
	{1048.3969,-1945.4319,13.2205},
	{1063.6014,-1945.1613,13.2205},
	{1213.6760,-2433.0134,9.6150},
	{1210.0365,-2448.0322,9.5693},
	{2098.5908,-2662.0005,13.3821},
	{2348.7107,-2226.2048,13.3821},
	{2338.7896,-2215.2847,13.3750},
	{2843.0481,-1835.4346,10.8943},
	{2824.4434,-1831.3973,10.8904},
	{2896.9583,-649.9005,10.8359},
	{2877.1814,-647.2960,10.8359},
	{2766.5542,221.4342,20.2656},
	{2747.9395,216.6371,27.0347},
	{1968.9531,292.3683,33.4549},
	{1968.8036,312.4247,33.5116},
	{1746.1255,572.7900,24.7794},
	{1763.3621,568.7740,24.6825},
	{1347.0687,-2281.6086,13.3904},
	{1331.9624,-2279.8809,13.3828}
};
#define DIALOG:%0(%1)\
        forward dial_%0(%1);\
        public dial_%0(%1)
#define DIAL:%0(%1)\
		DIALOG:%0(%1)
        
#undef MAX_PLAYERS

#define MAX_LOADING_POINTS 	300
#define MAX_TOWAROW 		300

#define MAX_PLAYERS 		25
#define MAX_STACJE 			200
#define MAX_FOTORADAROW 	1000
#define MAX_VIASHOP 		200
#define MAX_VEHICLES_DB 	5000
#define MAX_FIRM 			13

// ---------------------------

#define TEAM_TYPE_TRUCKER 	0
#define TEAM_TYPE_POLICE 	1
#define TEAM_TYPE_MEDIC 	2
#define TEAM_TYPE_POMOC 	3
#define TEAM_TYPE_CARGO 	4
#define TEAM_TYPE_PETROL 	5
#define TEAM_TYPE_SMLOG 	6
#define TEAM_TYPE_BUILD 	7

// ---------------------------

#define isnull(%1) ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#define Loop(%1,%2) for(new %1 = 0; %1 < %2; %1++)
#define ResetVariablesInEnum(%0,%1) 	for(new __rvine; %1:__rvine != %1; __rvine++) %0[%1:__rvine]=0
#define ShowInfo(%0,%1) ShowPlayerDialog(%0, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, "{FF0000}Informacja", %1, "Ok", "")

enum
{
	DIALOG_ID_CREATE_VEHICLE=1,
	DIALOG_ID_CREATE_VEHICLE_TRUCK,
	DIALOG_ID_CREATE_VEHICLE_TRAILE,
	DIALOG_ID_TELEPHONE,
	DIALOG_ID_SPAWN_SELECT,
	DIALOG_CREATE_DOK_NAME,
	DIALOG_CREATE_DOK_TYPE,
	DIALOG_CREATE_TOWAR_NAME,
	DIALOG_CREATE_TOWAR_TYPE,
	DIALOG_CREATE_TOWAR_STAWKA,
	DIALOG_CREATE_TOWAR_CZAS,
	DIALOG_CREATE_TOWAR_POZIOM,
	DIALOG_ID_RADIO_ADD,
	DIALOG_ID_RADIO_LINK,
	DIALOG_ID_RADIO,
	DIALOG_ID_LOAD_CARGO,
	DIALOG_ID_LOAD_CARGO_WEIGHT,
	NEVER_DIALOG,
	DIALOG_ID_REGISTER,
	DIALOG_ID_LOGIN,
	COMPANY_DIALOG,
	GTIMEG_DIALOG,
	GTIMEG_LIST_DIALOG,
	DIALOG_BUY_HOUSE,
	DIALOG_BUY2_HOUSE,
	DIALOG_SELL_HOUSE,
	DIALOG_TP_HOUSE,
	ANIM1_DIALOG,
	ANIM2_DIALOG,
	ANIM3_DIALOG,
	ANIM4_DIALOG,
	ANIM5_DIALOG,
	ANIM6_DIALOG,
	ANIM7_DIALOG,
	DIALOG_TACHOGRAF,
	DIALOG_FOTORADAR,
	DIALOG_TANKOWANIE,
	DIALOG_ZAPIS_FIRMOWE,
	DIALOG_ZAPIS_FIRMOWE2,
	DIALOG_ZAPIS_FIRMOWE3,
	DIALOG_BUYCOMPANY_DIALOG,
	DIALOG_PD,
	DIALOG_PD1,
	DIALOG_PD2,
	DIALOG_PD3,
	DIALOG_PD4,
	DIALOG_PD5,
	DIALOG_PD6,
	DIALOG_PD7,
	DIALOG_PD8,
	DIALOG_PANEL,
	DIALOG_PANEL1,
	DIALOG_PANEL2,
	DIALOG_PANEL3,
	DIALOG_PANEL4,
	DIALOG_PANEL5,
	DIALOG_PANEL6,
	DIALOG_PANEL7,
	DIALOG_SALON,
	DIALOG_SALON1,
	DIALOG_ZAPIS_PRYWATNE,
	MISSION_DIALOG,
	REMOVE_MISSION_DIALOG,
	DIALOG_BANK_CONNECT,
	DIALOG_BANK_CONNECT1,
	DIALOG_BANK_CONNECT2,
	POLICE_DIALOG,
	DIALOG_MENU_BAR,
	DIALOG_ID_GPS,
	DIALOG_ID_GPS_TYPE,
	DIALOG_ID_FIRMA,
	DIALOG_ID_FIRMA_WPLAC,
	DIALOG_TP_LOGIN_SKIN,
	DIALOG_ID_FIRMA_KSIEGOWY,
	DIALOG_ID_FIRMA_SKIN,
	DIALOG_ID_FIRMA_ZASTEPCA,
	DIALOG_ID_FIRMA_SKIN_ZMIEN,
	DIALOG_ID_VIATOLL,
	DIALOG_ID_FIRMA_ZAPROS,
	DIALOG_ID_FIRMA_WYRZUC,
	DIALOG_ID_FIRMA_ZAPROS_ACCEPT,
	DIALOG_ID_VEHICLE,
	DIALOG_ID_VEHICLE_ZARZADZAJ,
	DIALOG_CONTROL,
	DIALOG_CRASHES,
	DIALOG_ALCOHOL
}

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

#define KEY_AIM 			132

#define MAX_ADMIN_VEHICLES 	50
new AdminsVehs[MAX_ADMIN_VEHICLES];
new SpawnedAdminVehicles;

new Text3D:Trucking[MAX_PLAYERS];

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
	vCargoID,
	vCargoWeight,
	vSAMPID
};
new vehInfo[MAX_VEHICLES_DB][vInfo];
new Float:vPos[MAX_VEHICLES][6];
new DBVehID[MAX_VEHICLES_DB];
new HighVehicleID;

enum vOtherInfo {
	Float:vFuel,
	vFuelType,
	Float:vPrzebieg,
	vCargoID,
	vCargoWeight,
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
	bool:pInvited[8],
	bool:chained
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

new const KategorieTowarow[][] = {
	"Budowlane",
	"Elektroniczne",
	"Art. spo¿ywcze",
	"Art. odzie¿owe",
	"Niebezpieczne",
	"Nielegalne"
};

new colors[][] = {
	"niewidoczny", "bia³y", "pomarañczowy", "czerwony", "ró¿owy",
	"¿ó³ty", "br¹zowy", "zielony", "jasnoniebieski", "niebieski"
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
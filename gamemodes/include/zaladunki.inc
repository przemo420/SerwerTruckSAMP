/*
	_________________________________
	
	(c) 2015 - Part of SerwerTruck.eu
	@author: Kozak59, GeDox
	_________________________________

	TODO

	- Konwoje
	- Roz³adowywanie na stacji
	- Stan towaru 
	_________________________________

*/

#include "zones"

#define MAX_BASES_OBJECTS	100
#define MAX_SPAWN_POINTS 	25
#define MAX_BASES			5
#define MAX_WARES			25
#define MAX_CARGOS			500
#define MAX_LOADINGS		22

#define WARE_TYPE_TRUCK		0
#define WARE_TYPE_VAN		1
#define WARE_TYPE_MULTIPLE	2

#define VEHICLE_TYPE_TRUCK	0
#define VEHICLE_TYPE_VAN	1

#define BASE_LS				0
#define BASE_RC				1
#define BASE_LV				2
#define BASE_SF				3

new bool:privTrailer[MAX_VEHICLES],
	PlayerBar:pBar[MAX_PLAYERS],
	Float:cargoHealth[MAX_VEHICLES];

forward Loadings_OnGameModeInit();
forward Loadings_OnGameModeExit();
forward Loadings_OnPlayerDisconnect(playerid, reason);
forward Loadings_OnTrailerUpdate(playerid, vehicleid);

enum 
{
	TYPE_BUILD=0,
	TYPE_ELECTRONIC,
	TYPE_EAT,
	TYPE_CLOTHES,
	TYPE_DANGEROUS,
	TYPE_ILLEGAL
}

enum eWares
{
	WID,			// ID Towaru
	wName[32],		// Nazwa towaru
	Float:wRate,	// Stawka za KM
	wLevel,			// Wymagany poziom
	wType,			// Typ towaru
	wAdr,			// Wymagany poziom ADR
	wVehicle		// Wymagany typ pojazdu
}

enum eCargo
{
	CID, 			// ID Oferty
	cZID, 			// Zaladunek ID
	cRID, 			// Rozladunek ID
	cTimeStart, 	// Godzina zarezerwowania
	cTime, 			// Czas na dowóz, od godziny za³adowania
	cTimeEnd,		// Godzina znikniêcia
	cType, 			// ID Towaru - WID
	cWeight, 		// Waga towaru
	Float:cReward, 	// Prawdopodobne wynagrodzenie (bez bonusów)
	bool:cReserved, // Czy towar jest ju¿ zarezerwowany (aby nie usuwa³o przy odœwie¿aniu)
	Float:cDistance,// Dystans za³adunek - roz³adunek
	bool:cDeleted	// Czy oferta jest do usuniêcia
}

enum eBaza
{
	bool:isFreePoint[MAX_SPAWN_POINTS],		// Czy punkt jest wolny
	Float:xPoint[MAX_SPAWN_POINTS],			// Pozycja X
	Float:yPoint[MAX_SPAWN_POINTS],			// Pozycja Y
	Float:zPoint[MAX_SPAWN_POINTS],			// Pozycja Z
	Float:aPoint[MAX_SPAWN_POINTS]			// Pozycja A
}

enum eLoad
{
	LID,			// ID Za³adunku
	Float:lX,		// Pozycja X
	Float:lY,		// Pozycja Y
	Float:lZ,		// Pozycja Z
	lType,			// Typ za³adunku
	lBaza,			// ID Bazy z której korzystamy
	lMapIcon,		// ID Map Icona
	Text3D:lText	// ID Labela
}

enum eLoadVehicle
{
	VID,
	vCargo,
	vOwner[24],
	bool:vLoaded
}

new waresInfo[MAX_WARES][eWares],
	cargoInfo[MAX_CARGOS][eCargo],
	bazaInfo[MAX_BASES][eBaza],
	loadInfo[MAX_LOADINGS][eLoad],
	vloadInfo[MAX_VEHICLES][eLoadVehicle],
	objectsInfo[MAX_BASES_OBJECTS];

public Loadings_OnGameModeInit()
{
	LoadingsInit();
	LoadObjects();
}

public Loadings_OnGameModeExit()
{
	LoadingsExit();
	DestroyObjects();
}

stock LoadingsInit()
{
	new szBuffer[64], nums;

	// Inicjowanie wszystkich dostêpnych towarów

	mysql_query("SELECT * FROM wares");
	mysql_store_result();
	nums = mysql_num_rows();
	for(new i = 0; i < nums; i++)
	{
		if(mysql_fetch_row(szBuffer, "|") && i <= MAX_WARES)
		{
			sscanf(szBuffer, "p<|>ds[32]fdddd", waresInfo[i][WID], waresInfo[i][wName], waresInfo[i][wRate], waresInfo[i][wLevel], waresInfo[i][wAdr], waresInfo[i][wType], waresInfo[i][wVehicle]);
		}
	}
	mysql_free_result();

	// Inicjownaie wszystkich punktów za³adunkowych

	mysql_query("SELECT * FROM loads");
	mysql_store_result();
	nums = mysql_num_rows();
	for(new c = 0; c < nums; c++)
	{
		if(mysql_fetch_row(szBuffer, "|") && c <= MAX_LOADINGS)
		{
			sscanf(szBuffer, "p<|>dfffdd", loadInfo[c][LID], loadInfo[c][lX], loadInfo[c][lY], loadInfo[c][lZ], loadInfo[c][lType], loadInfo[c][lBaza]);

			new szString[256];
			format(szString, sizeof szString, "Za³adunek towarów {b}%s{/b} w {b}%s{/b} (ID: {b}%d{/b})\nAby za³adowaæ towar wpisz {b}/zaladuj{/b}\nAby roz³adowaæ towar wpisz {b}/rozladuj{/b}", GetLoadTypeName(loadInfo[c][lType]), GetPointZone(loadInfo[c][lX], loadInfo[c][lY]), loadInfo[c][LID]);
			loadInfo[c][lText] = CreateDynamic3DTextLabel(clText(COLOR_INFO2, szString), -1, loadInfo[c][lX], loadInfo[c][lY], loadInfo[c][lZ]+1.0, 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1);
			loadInfo[c][lMapIcon] = CreateDynamicMapIcon(loadInfo[c][lX], loadInfo[c][lY], loadInfo[c][lZ], 51, 0);
		}
	}
	mysql_free_result();

	// Inicjowanie wszystkich baz (wypo¿yczalni naczep)

	mysql_query("SELECT * FROM bases");
	mysql_store_result();
	nums = mysql_num_rows();
	for(new c = 0; c < nums; c++)
	{
		if(mysql_fetch_row(szBuffer, "|") && c <= MAX_SPAWN_POINTS*4)
		{
			new id[2], Float:pos[4];
			sscanf(szBuffer, "p<|>{d}ddffff", id[0], id[1], pos[0], pos[1], pos[2], pos[3]);

			bazaInfo[id[1]][xPoint][id[0]] = pos[0];
			bazaInfo[id[1]][yPoint][id[0]] = pos[1];
			bazaInfo[id[1]][zPoint][id[0]] = pos[2];
			bazaInfo[id[1]][aPoint][id[0]] = pos[3];
			bazaInfo[id[1]][isFreePoint][id[0]] = true;
		}
	}
	mysql_free_result();

	CreateCargos();
	SetTimer("RefreshCargos", 2 * 60 * 1000, true);
	SetTimer("RenewCargos", 25 * 60 * 1000, true);
	return 1;
}

stock GetLoadTypeName(type)
{
	new name[32];
	switch(type)
	{
		case TYPE_BUILD:
			strcat(name, "budowlanych");
		case TYPE_ILLEGAL:
			strcat(name, "nielegalnych");
		case TYPE_DANGEROUS:
			strcat(name, "niebezpiecznych");
		case TYPE_EAT:
			strcat(name, "spo¿ywczych");
		case TYPE_ELECTRONIC:
			strcat(name, "elektronicznych");
		case TYPE_CLOTHES:
			strcat(name, "odzie¿owych");
	}
	return name;
}

stock CreateCargos()
{
	new tick = GetTickCount();
	for(new i = 0; i < MAX_LOADINGS; i++)
	{
		for(new c = 0; c < 5; c++)
		{
			new type = GetLoadingType(i),
				id = GetNextCargoID();

			cargoInfo[id][CID] = id;
			cargoInfo[id][cZID] = i;
			cargoInfo[id][cRID] = DrawUnloadingPoint(i, type);
			cargoInfo[id][cTimeStart] = gettime();
			cargoInfo[id][cTimeEnd] = gettime() + (15*60) + (random(30)*60);
			cargoInfo[id][cType] = DrawWare(type);

			cargoInfo[id][cWeight] = waresInfo[cargoInfo[id][cType]][wVehicle] == WARE_TYPE_VAN ? 2 + random(6) : 6 + random(16);
			cargoInfo[id][cDistance] = 0.5 * GetDistance(loadInfo[i][lX], loadInfo[i][lY], loadInfo[i][lZ], loadInfo[cargoInfo[id][cRID]][lX], loadInfo[cargoInfo[id][cRID]][lY], loadInfo[cargoInfo[id][cRID]][lZ]);
			cargoInfo[id][cTime] = GetTimeByDistance(floatround(cargoInfo[id][cDistance])) + 2;
			cargoInfo[id][cReward] = waresInfo[cargoInfo[id][cType]][wRate] * cargoInfo[id][cDistance] + (cargoInfo[id][cWeight] * 15);
			cargoInfo[id][cReserved] = false;
			cargoInfo[id][cDeleted] = false;
		}
	}
	printf("Tworzenie ofert trwa³o %d ms", GetTickCount() - tick);
	return 1;
}

stock GetTimeByDistance(distance)
{
	new Float:time;
	time = distance / 400;
	if(time < 4)
		time = 4;
	if(time > 8)
		time = 8;
	return floatround(time);
}

forward RenewCargos();
public RenewCargos()
{
	new tick = GetTickCount();
	for(new id = 0; id < MAX_CARGOS; id++)
	{
		if(!IsValidOffert(id) || !cargoInfo[id][cDeleted])
			continue;

		cargoInfo[id][cDeleted] = false;
		new type = loadInfo[cargoInfo[id][cZID]][lType];
		cargoInfo[id][cRID] = DrawUnloadingPoint(cargoInfo[id][cZID], type);
		cargoInfo[id][cTimeStart] = gettime();
		cargoInfo[id][cTimeEnd] = gettime() + (15*60) + (random(30)*60);
		cargoInfo[id][cType] = DrawWare(type);

		cargoInfo[id][cWeight] = waresInfo[cargoInfo[id][cType]][wVehicle] == WARE_TYPE_VAN ? 2 + random(6) : 6 + random(16);
		cargoInfo[id][cDistance] = GetDistance(loadInfo[cargoInfo[id][cZID]][lX], loadInfo[cargoInfo[id][cZID]][lY], loadInfo[cargoInfo[id][cZID]][lZ], loadInfo[cargoInfo[id][cRID]][lX], loadInfo[cargoInfo[id][cRID]][lY], loadInfo[cargoInfo[id][cRID]][lZ]);
		cargoInfo[id][cTime] = GetTimeByDistance(floatround(cargoInfo[id][cDistance]));
		cargoInfo[id][cReward] = waresInfo[cargoInfo[id][cType]][wRate] * cargoInfo[id][cDistance] + (cargoInfo[id][cWeight] * 15);
		cargoInfo[id][cReserved] = false;
	}
	printf("Przetworzenie ofert trwa³o %d ms", GetTickCount() - tick);
	return 1;
}

public Loadings_OnPlayerDisconnect(playerid, reason)
{
	if(gettime() > cargoInfo[GetPVarInt(playerid, "cargoID")][cTimeEnd])
		DeleteCargo(GetPVarInt(playerid, "cargoID"));
	else
		cargoInfo[GetPVarInt(playerid, "cargoID")][cReserved] = false;
	DeletePVar(playerid, "cargoID");
	DeletePVar(playerid, "cargoIDAllowed");
	RemovePlayerMapIcon(playerid, 69);
	if(GetPVarInt(playerid, "playerTrailer") > 0)
		DestroyVehicle(GetPVarInt(playerid, "playerTrailer"));
	DeletePVar(playerid, "playerTrailer");
	return 1;
}

forward RefreshCargos();
public RefreshCargos()
{
	for(new id = 0; id < MAX_CARGOS; id++)
	{
		if(!IsValidOffert(id) || cargoInfo[id][cReserved] == true || cargoInfo[id][cDeleted])
			continue;

		if((gettime() - cargoInfo[id][cTimeEnd]) > 0)
			cargoInfo[id][cDeleted] = true;
	}
	return 1;
}

stock LoadingsExit()
{
	mysql_close();
	return 1;
}

stock DrawWare(type)
{
	new id;

	id = random(MAX_WARES);
	while(type != waresInfo[id][wType] || !IsValidWare(id))
	{
		id = random(MAX_WARES);
	}
	return id;
}

stock DrawUnloadingPoint(pointload, type)
{
	new id, Float:fDistance;

	id = random(MAX_LOADINGS);
	fDistance = GetDistance(loadInfo[id][lX], loadInfo[id][lY], loadInfo[id][lZ], loadInfo[pointload][lX], loadInfo[pointload][lY], loadInfo[pointload][lZ]);
	while(type != loadInfo[id][lType] || fDistance < 500.0 || id == pointload || !IsValidLoad(id))
	{
		id = random(MAX_LOADINGS);
		fDistance = GetDistance(loadInfo[id][lX], loadInfo[id][lY], loadInfo[id][lZ], loadInfo[pointload][lX], loadInfo[pointload][lY], loadInfo[pointload][lZ]);
	}

	return id;
}

stock IsValidLoad(loadid)
{
	if(loadInfo[loadid][lType] + loadInfo[loadid][lBaza] + loadInfo[loadid][lZ] > 0)
		return true;
	return false;
}

stock IsValidWare(wareid)
{
	if(waresInfo[wareid][wRate] != 0.0)
		return true;
	return false;
}

stock GetLoadingType(loadingid)
{
	return loadInfo[loadingid][lType];
}

stock GetNextCargoID()
{
	for(new i = 0; i < MAX_CARGOS; i++)
	{
		if((cargoInfo[i][cZID] + cargoInfo[i][cRID] + cargoInfo[i][CID]) == 0)
			return i;
	}
	return 1;
}

stock GetNextCargoIDByTime()
{
	for(new i = 0; i < MAX_CARGOS; i++)
	{
		if(gettime() - cargoInfo[i][cTimeEnd] > 0)
			return i;
	}
	return 1;
}

stock GetDistance( Float:x1, Float:y1, Float:z1, Float:x2, Float:y2, Float:z2 )
{
	return floatround( floatsqroot( ( ( ( x1 - x2 ) * ( x1 - x2 ) ) + ( ( y1 - y2 ) * ( y1 - y2 ) ) + ( ( z1 - z2 ) * ( z1 - z2 ) ) ) ) );
}

stock IsValidOffert(cargoid)
{
	if(cargoInfo[cargoid][cWeight] >= 2)
		return true;
	return false;
}

stock GetOfferts(playerid, type)
{
	new count = 0;
	for(new i = 0; i < MAX_CARGOS; i++)
	{
		if(!IsValidOffert(i) || cargoInfo[i][cReserved] == true || cargoInfo[i][cDeleted] == true || (gettime() - cargoInfo[i][cTimeEnd]) > 0)
			continue;

		if(GetPVarInt(playerid, "LEVEL") < waresInfo[cargoInfo[i][cType]][wLevel] || PlayerInfo[playerid][pADR] < waresInfo[cargoInfo[i][cType]][wAdr])
			continue;

		if(GetAllowedDistance(GetPlayerSkill(playerid, SKILL_DISTANCE)) < cargoInfo[i][cDistance])
			continue;

		if(waresInfo[cargoInfo[i][cType]][wType] == type)
			count++;
	}
	return count;
}

stock IsPlayerDriver(playerid)
{
	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
		return true;
	return false;
}

CMD:oferta(playerid)
{
	if(!(IsVehicleTruck(GetVehicleModel(GetPlayerVehicleID(playerid))) || IsVehicleVan(GetVehicleModel(GetPlayerVehicleID(playerid)))))
		return Msg(playerid, COLOR_ERROR, "Nie siedzisz w ¿adnym pojeŸdzie dostosowanym do przewo¿enia towarów.");

	if(!IsPlayerDriver(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie jesteœ kierowc¹.");

	if(GetPVarInt(playerid, "cargoID") > 0)
		return Msg(playerid, COLOR_ERROR, "Nie ukoñczy³es jeszcze poprzedniego zlecenia.");

	new szString[170];
	DeletePVar(playerid, "selectedType");
	DeletePVar(playerid, "lastCargo");
	format(szString, sizeof szString, "Towary budowlane\t%d\nTowary odzie¿owe\t%d\nTowary elektryczne\t%d\nTowary spo¿ywcze\t%d\nTowary niebezpieczne\t%d\nTowary nielegalne\t%d",
		GetOfferts(playerid, TYPE_BUILD),
		GetOfferts(playerid, TYPE_CLOTHES),
		GetOfferts(playerid, TYPE_ELECTRONIC),
		GetOfferts(playerid, TYPE_EAT),
		GetOfferts(playerid, TYPE_DANGEROUS),
		GetOfferts(playerid, TYPE_ILLEGAL));

	strins(szString, "Kategoria\tIloœæ ofert\n", 0);
	Dialog_Show(playerid, DIALOG_OFFER, DIALOG_STYLE_TABLIST_HEADERS, " ", szString, "Wybierz", "WyjdŸ");
	return 1;
}

Dialog:DIALOG_OFFER(playerid, response, listitem, inputtext[])
{
	if(!response)
	{
		DeletePVar(playerid, "cargoID");
		return 1;
	}

	new type;
	switch(listitem)
	{
		case 0:
			type = TYPE_BUILD;
		case 5:
			type = TYPE_ILLEGAL;
		case 4:
			type = TYPE_DANGEROUS;
		case 3:
			type = TYPE_EAT;
		case 2:
			type = TYPE_ELECTRONIC;
		case 1:
			type = TYPE_CLOTHES;
	}

	SetPVarInt(playerid, "selectedType", listitem);

	new szString[640],
		count,
		minCargo = GetPVarInt(playerid, "lastCargo");

	if(minCargo == 0)
		minCargo = -1;

	for(new i = 0; i < MAX_CARGOS; i++)
	{
		if(!IsValidOffert(i) || (gettime() - cargoInfo[i][cTimeEnd]) > 0 || waresInfo[cargoInfo[i][cType]][wType] != type || cargoInfo[i][cReserved] == true || i < minCargo || cargoInfo[i][cDeleted] == true)
			continue;

		if(GetPVarInt(playerid, "LEVEL") < waresInfo[cargoInfo[i][cType]][wLevel] || PlayerInfo[playerid][pADR] < waresInfo[cargoInfo[i][cType]][wAdr])
			continue;

		if(GetAllowedDistance(GetPlayerSkill(playerid, SKILL_DISTANCE)) < cargoInfo[i][cDistance])
			continue;

		if(count >= 10)
			break;

		count += 1;
		new szTemp[70], 
			ZID = cargoInfo[i][cZID], 
			RID = cargoInfo[i][cRID];

		format(szTemp, sizeof szTemp, "%d\t%s (%dt)\t%s\t%s",
			cargoInfo[i][CID],
			waresInfo[cargoInfo[i][cType]][wName], 
			cargoInfo[i][cWeight],
			GetPointZone(loadInfo[ZID][lX], loadInfo[ZID][lY]),
			GetPointZone(loadInfo[RID][lX], loadInfo[RID][lY])
			);

		strcat(szString, szTemp);
		strcat(szString, "\n");
		SetPVarInt(playerid, "lastCargo", i);
	}

	if(count == 0 || GetOfferts(playerid, type) == 0)
		format(szString, sizeof szString, "Brak ofert do wyœwietlenia.");
	else
	{
		strins(szString, "ID\tTowar\tZa³adunek\tRoz³adunek", 0);
		if(count == 10 && (listitem - GetMinOffert(playerid, type, GetPVarInt(playerid, "lastCargo"))) < 0)
			format(szString, sizeof szString, "%s\nDalej >>", szString);
	}

	Dialog_Show(playerid, DIALOG_OFFER_LIST, count == 0 ? DIALOG_STYLE_LIST : DIALOG_STYLE_TABLIST_HEADERS, " ", szString, "Wybierz", "Wstecz");
	return 1;
}

Dialog:DIALOG_OFFER_LIST(playerid, response, listitem, inputtext[])
{
	if(!response)
		return cmd_oferta(playerid);

	if(strfind(inputtext, "wyœwietlenia") != -1)
		return DIALOG_OFFER(playerid, true, GetPVarInt(playerid, "selectedType"), "");

	if(strfind(inputtext, "Dalej") != -1)
	{
		DIALOG_OFFER(playerid, true, GetPVarInt(playerid, "selectedType"), "");
		return 1;
	}

	new id,
		szString[300];

	sscanf(inputtext, "d{s[32]s[32]s[32]dd}", id);

	new	ZID = cargoInfo[id][cZID], 
		RID = cargoInfo[id][cRID], 
		time[6],
		bonusSkillReward = GetBiggerReward(GetPlayerSkill(playerid, SKILL_REWARD)),
		bonusSkillTime = GetReducedTime(GetPlayerSkill(playerid, SKILL_TIME));

	TimestampToDate(cargoInfo[id][cTimeEnd], time[0], time[1], time[2], time[3], time[4], time[5]);
	format(szString, sizeof szString, "{b}%s{/b} - {b}%dt{/b}\nZarobek: {b}$%.02f{/b} (+%d%) ($%.02f / km)", waresInfo[cargoInfo[id][cType]][wName], cargoInfo[id][cWeight], cargoInfo[id][cReward]*(1+(bonusSkillReward*0.01)), bonusSkillReward, waresInfo[cargoInfo[id][cType]][wRate]);
	strcat(szString, "\n \n");
	format(szString, sizeof szString, "%sTyp:\t\t{b}%s{/b}", szString, GetWareVehicleName(cargoInfo[id][cType]));
	strcat(szString, "\n");
	format(szString, sizeof szString, "%sZa³adunek:\t{b}%s{/b} (ID: {b}%d{/b})", szString, GetPointZone(loadInfo[ZID][lX], loadInfo[ZID][lY]), ZID);
	strcat(szString, "\n");
	format(szString, sizeof szString, "%sRoz³adunek:\t{b}%s{/b} (ID: {b}%d{/b})", szString, GetPointZone(loadInfo[RID][lX], loadInfo[RID][lY]), RID);
	strcat(szString, "\n");
	format(szString, sizeof szString, "%sDystans:\t{b}%.02f{/b} km", szString, cargoInfo[id][cDistance]);
	strcat(szString, "\n");
	format(szString, sizeof szString, "%sCzas na dostawê {b}%d{/b} minut/y.", szString, cargoInfo[id][cTime]+bonusSkillTime);
	strcat(szString, "\n \n");
	format(szString, sizeof szString, "%sOferta wygaœnie o {b}%02d:%02d{/b}.", szString, time[3], time[4]);

	SetPVarInt(playerid, "cargo", id);
	Dialog_Show(playerid, DIALOG_OFFER_SUM, DIALOG_STYLE_MSGBOX, " ", clText(COLOR_INFO2, szString), "Akceptuj", "Wstecz");
	return 1;
}

stock GetWareVehicleName(wareid)
{
	new name[24];
	switch(waresInfo[wareid][wVehicle])
	{
		case WARE_TYPE_TRUCK:
			strcat(name, "naczepa");
		case WARE_TYPE_VAN:
			strcat(name, "dostawczak");
		case WARE_TYPE_MULTIPLE:
			strcat(name, "naczepa / dostawczak");
	}
	return name;
}

Dialog:DIALOG_OFFER_SUM(playerid, response, listitem, inputtext[])
{
	if(!response)
	{
		new first = 0, count = 0, type;
		switch(GetPVarInt(playerid, "selectedType"))
		{
			case 0:
				type = TYPE_BUILD;
			case 5:
				type = TYPE_ILLEGAL;
			case 4:
				type = TYPE_DANGEROUS;
			case 3:
				type = TYPE_EAT;
			case 2:
				type = TYPE_ELECTRONIC;
			case 1:
				type = TYPE_CLOTHES;
		}

		if(GetMaxOffert(playerid, type, GetPVarInt(playerid, "cargo")) <= 8) 
			SetPVarInt(playerid, "lastCargo", GetPVarInt(playerid, "cargo"));
		else
		{
			new i = 500;
			while(i >= 0)
			{
				i--;
				if(!IsValidOffert(i) || i >= GetPVarInt(playerid, "cargo") || cargoInfo[i][cReserved] == true || waresInfo[cargoInfo[i][cType]][wType] != type)
					continue;

				if(GetPVarInt(playerid, "LEVEL") < waresInfo[cargoInfo[i][cType]][wLevel] || PlayerInfo[playerid][pADR] < waresInfo[cargoInfo[i][cType]][wAdr])
					continue;

				if(GetAllowedDistance(GetPlayerSkill(playerid, SKILL_DISTANCE)) < cargoInfo[i][cDistance])
					continue;

				if(count >= 8)
				{
					first = i;
					break;
				}
				count++;
			}
		}
		SetPVarInt(playerid, "lastCargo", first);
		DIALOG_OFFER(playerid, true, GetPVarInt(playerid, "selectedType"), "");
		return 1;
	}

	SetPVarInt(playerid, "cargoID", GetPVarInt(playerid, "cargo"));
	DeletePVar(playerid, "cargo");
	new id = GetPVarInt(playerid, "cargoID");
	cargoInfo[id][cReserved] = true;
	DeletePVar(playerid, "selectedType");
	DeletePVar(playerid, "lastCargo");
	Msg(playerid, COLOR_INFO, "Towar pomyœlnie zosta³ zarezerwowany. Teraz mo¿esz udaæ siê na za³adunek.");
	if(!PlayerInfo[playerid][pFirma] && !GetVehicleTrailer(GetPlayerVehicleID(playerid)) && (GetWareVehicle(cargoInfo[id][cType]) == WARE_TYPE_TRUCK || GetWareVehicle(cargoInfo[id][cType]) == WARE_TYPE_MULTIPLE) && IsVehicleTruck(GetVehicleModel(GetPlayerVehicleID(playerid))))
		Dialog_Show(playerid, DIALOG_TRAILER, DIALOG_STYLE_MSGBOX, " ", "Czy chcia³yœ wypo¿yczyæ naczepê?\nJest to konieczne, je¿eli nie posiadasz w³asnej naczepy a chcesz przewieŸæ towar tylko dla ciê¿arówek.", "Tak", "Nie");
	else
	{
		new ZID = cargoInfo[GetPVarInt(playerid, "cargoID")][cZID];
		SetPlayerMapIcon(playerid, 69, loadInfo[ZID][lX], loadInfo[ZID][lY], loadInfo[ZID][lZ], 41, 0, MAPICON_GLOBAL);
	}
	return 1;
}

stock GetMaxOffert(playerid, type, maxid)
{
	new count = 0;
	for(new i = 0; i < MAX_CARGOS; i++)
	{
		if(!IsValidOffert(i) || cargoInfo[i][cReserved] == true || cargoInfo[i][cDeleted] == true)
			continue;

		if(GetPVarInt(playerid, "LEVEL") < waresInfo[cargoInfo[i][cType]][wLevel] || PlayerInfo[playerid][pADR] < waresInfo[cargoInfo[i][cType]][wAdr])
			continue;

		if(GetAllowedDistance(GetPlayerSkill(playerid, SKILL_DISTANCE)) < cargoInfo[i][cDistance])
			continue;

		if(i >= maxid)
			break;

		if(waresInfo[cargoInfo[i][cType]][wType] == type)
			count++;
	}
	return count;
}

stock GetMinOffert(playerid, type, minid)
{
	new count = 0;
	for(new i = minid; i < MAX_CARGOS; i++)
	{
		if(!IsValidOffert(i) || cargoInfo[i][cReserved] == true || cargoInfo[i][cDeleted] == true)
			continue;

		if(GetPVarInt(playerid, "LEVEL") < waresInfo[cargoInfo[i][cType]][wLevel] || PlayerInfo[playerid][pADR] < waresInfo[cargoInfo[i][cType]][wAdr])
			continue;

		if(GetAllowedDistance(GetPlayerSkill(playerid, SKILL_DISTANCE)) < cargoInfo[i][cDistance])
			continue;

		if(waresInfo[cargoInfo[i][cType]][wType] == type)
			count++;
	}
	return count;
}

Dialog:DIALOG_TRAILER(playerid, response, listitem, inputtext[])
{
	if(!response)
	{
		new ZID = cargoInfo[GetPVarInt(playerid, "cargoID")][cZID];
		SetPlayerMapIcon(playerid, 69, loadInfo[ZID][lX], loadInfo[ZID][lY], loadInfo[ZID][lZ], 41, 0, MAPICON_GLOBAL);
		return 1;
	}
	new id = GetPVarInt(playerid, "cargoID"), base = loadInfo[cargoInfo[id][cZID]][lBaza], point = -1;

	for(new i = 0; i < MAX_SPAWN_POINTS; i++)
	{
		if(!bazaInfo[base][isFreePoint][i])
			continue;

		point = i;
		SetPVarInt(playerid, "playerTrailer", CreateVehicle(GetTrailer(waresInfo[cargoInfo[id][cType]][wType]), bazaInfo[base][xPoint][i], bazaInfo[base][yPoint][i], bazaInfo[base][zPoint][i], bazaInfo[base][aPoint][i], 0, 0, -1));
		privTrailer[GetPVarInt(playerid, "playerTrailer")] = true;
		bazaInfo[base][isFreePoint][i] = false;
		SetPVarInt(playerid, "trailerPoint", i);
		SetPlayerMapIcon(playerid, 69, bazaInfo[base][xPoint][i], bazaInfo[base][yPoint][i], bazaInfo[base][zPoint][i], 42, 0, MAPICON_GLOBAL);
		break;
	}

	new szString[128];
	format(szString, sizeof szString, "Twoja naczepa jest gotowa do odbioru w {b}%s{/b}. Miejsce zosta³o zaznaczone na mapie.", GetPointZone(bazaInfo[base][xPoint][point], bazaInfo[base][yPoint][point]));
	Msg(playerid, COLOR_INFO, szString);
	return 1;
}

stock GetTrailer(id)
{
	new trailer;
	switch(id)
	{
		case TYPE_BUILD:
			trailer = 450;
		case TYPE_ILLEGAL:
			trailer = 435;
		case TYPE_DANGEROUS:
			trailer = 584;
		case TYPE_EAT:
			trailer = 435;
		case TYPE_ELECTRONIC:
			trailer = 591;
		case TYPE_CLOTHES:
			trailer = 591;
	}
	return trailer;
}

stock GetVehicleType(vehicleid)
{
	new model = GetVehicleModel(vehicleid);
	if(IsVehicleTruck(model))
		return VEHICLE_TYPE_TRUCK;
	return VEHICLE_TYPE_VAN;
}

stock GetWareVehicle(wareid)
{
	return waresInfo[wareid][wVehicle];
}

stock IsVehicleTruck(modelid)
{
	if(modelid == 515 || modelid == 514 || modelid == 403)
		return 1;
	return 0;
}

stock IsVehicleVan(modelid)
{
	if(modelid == 440 || modelid == 455 || modelid == 456 || modelid == 459 || modelid == 482 || modelid == 498 || modelid == 499 || modelid == 578 || modelid ==  609)
		return 1;
	return 0;
}

stock IsGoodTrailer(type, trailerid)
{
	switch(type)
	{
		case TYPE_BUILD:
		{
			if(trailerid == 450)
				return true;
			return false;
		}
		case TYPE_ELECTRONIC: 
		{
			if(trailerid == 591)
				return true;
			return false;
		}
		case TYPE_EAT: 
		{
			if(trailerid == 435)
				return true;
			return false;
		}
		case TYPE_CLOTHES: 
		{
			if(trailerid == 591)
				return true;
			return false;
		}
		case TYPE_DANGEROUS: 
		{
			if(trailerid == 584)
				return true;
			return false;
		}
		case TYPE_ILLEGAL: 
		{
			if(trailerid == 450 || trailerid == 591 || trailerid == 435 || trailerid == 584)
				return true;
			return false;
		}
		default: return true;
	}
	return false;
}

stock PlayerNick(playerid)
{
	new szString[MAX_PLAYER_NAME];
	GetPlayerName(playerid, szString, sizeof szString);
	return szString;
}

public Loadings_OnTrailerUpdate(playerid, vehicleid)
{
	if(privTrailer[vehicleid])
	{
		if(GetPVarInt(playerid, "playerTrailer") != vehicleid)
		{
			Msg(playerid, COLOR_ERROR, "To nie twoja naczepa.");
			DetachTrailerFromVehicle(vehicleid);

			new Float:pos[3];
			GetVehiclePos(GetPlayerVehicleID(playerid), pos[0], pos[1], pos[2]);
			GetXYInFrontOfVehicle(vehicleid, pos[0], pos[1], 13.0);
			SetVehiclePos(GetPlayerVehicleID(playerid), pos[0], pos[1], pos[2]);
		}
		else
		{
			new ZID = cargoInfo[GetPVarInt(playerid, "cargoID")][cZID];
			privTrailer[vehicleid] = false;
			bazaInfo[loadInfo[cargoInfo[GetPVarInt(playerid, "cargoID")][cZID]][lBaza]][isFreePoint][GetPVarInt(playerid, "trailerPoint")] = true;
			Msg(playerid, COLOR_INFO, "Naczepa odebrana, teraz mo¿esz udaæ siê na za³adunek.");
			RemovePlayerMapIcon(playerid, 69);
			SetPlayerMapIcon(playerid, 69, loadInfo[ZID][lX], loadInfo[ZID][lY], loadInfo[ZID][lZ], 41, 0, MAPICON_GLOBAL);
			DeletePVar(playerid, "trailerPoint");
		}
	}
	return 1;
}

stock GetXYInFrontOfVehicle(vehicleid, &Float:x, &Float:y, Float:distance)
{
	new Float:z, Float:angle; 
	GetVehicleZAngle(vehicleid, angle);
	GetVehiclePos(vehicleid, x, y, z);
	x += (distance * floatsin(-angle, degrees));
	y += (distance * floatcos(-angle, degrees));
	return 1;
}

CMD:zaladuj(playerid)
{
	new cargo = GetPVarInt(playerid, "cargoID");
	for(new i = 0; i < MAX_LOADINGS; i++)
	{
		if(!IsValidLoad(i))
			continue;

		if(IsPlayerInRangeOfPoint(playerid, 7.0, loadInfo[i][lX], loadInfo[i][lY], loadInfo[i][lZ]))
		{
			new vehicleid = GetPlayerVehicleID(playerid);
			if (GetPVarInt(playerid, "loading")) return Msg(playerid, COLOR_ERROR, "Ju¿ ³adujesz towar.");
			if (!IsPlayerInAnyVehicle(playerid)) return Msg(playerid, COLOR_ERROR, "Nie jesteœ w pojeŸdzie.");
			if (!IsPlayerDriver(playerid)) return Msg(playerid, COLOR_ERROR, "Nie jesteœ kierowc¹.");
			if (GetVehicleType(vehicleid) == VEHICLE_TYPE_TRUCK && !GetVehicleTrailer(vehicleid)) return Msg(playerid, COLOR_ERROR, "A gdzie twoja naczepa?");
			if (GetWareVehicle(cargoInfo[cargo][cType]) == WARE_TYPE_TRUCK && !IsGoodTrailer(waresInfo[cargoInfo[cargo][cType]][wType], GetVehicleModel(GetVehicleTrailer(vehicleid)))) return Msg(playerid, COLOR_ERROR, "Posiadasz z³y typ naczepy do tego towaru.");
			if (GetWareVehicle(cargoInfo[cargo][cType]) == WARE_TYPE_TRUCK && !IsVehicleTruck(GetVehicleModel(vehicleid))) return Msg(playerid, COLOR_ERROR, "Posiadasz z³y pojazd do tego typu oferty.");
			if (GetWareVehicle(cargoInfo[cargo][cType]) == WARE_TYPE_VAN && !IsVehicleVan(GetVehicleModel(vehicleid))) return Msg(playerid, COLOR_ERROR, "Posiadasz z³y pojazd do tego typu oferty.");
			if (GetWareVehicle(cargoInfo[cargo][cType]) == WARE_TYPE_MULTIPLE && !(IsVehicleTruck(GetVehicleModel(vehicleid)) || IsVehicleVan(GetVehicleModel(vehicleid)))) return Msg(playerid, COLOR_ERROR, "Posiadasz z³y pojazd do tego typu oferty.");
			if (cargo < 1) return Msg(playerid, COLOR_ERROR, "Nie posiadasz zarezerwowanego towaru, aby jakiœ wybraæ wpisz {b}/oferta{/b}.");
			if (vloadInfo[IsVehicleTruck(GetVehicleModel(vehicleid)) ? GetVehicleTrailer(vehicleid) : vehicleid][vLoaded] == true) return Msg(playerid, COLOR_ERROR, "Twój pojazd jest ju¿ za³adowany.");
			if (i != cargoInfo[cargo][cZID]) return Msg(playerid, COLOR_ERROR, "Jesteœ w z³ym punkcie za³adunku.");
			if (!IsValidOffert(cargo)) return Msg(playerid, COLOR_ERROR, "Wygl¹da na to, ¿e twój towar nie jest ju¿ dostêpny. Je¿li uwa¿asz, ¿e to b³¹d - zg³oœ to.");
			if (IsVehicleVan(GetVehicleModel(vehicleid)) && cargoInfo[cargo][cWeight] > 8) return Msg(playerid, COLOR_ERROR, "W pojeŸdzie dostawczym maksymalnie mo¿na za³adowaæ 8t.");

			pBar[playerid] = CreatePlayerProgressBar(playerid, 220.00, 435.00, 190.50, 5.50, 1653987839, 0.0);
			ShowPlayerProgressBar(playerid, pBar[playerid]);
			SetPlayerProgressBarMaxValue(playerid, pBar[playerid], 100.0);

			SetPVarInt(playerid, "loading", 1);
			SetPVarInt(playerid, "loadProgress", 0);
			SetPVarInt(playerid, "loadTimerCount", cargoInfo[cargo][cWeight] * 2);
			SetPVarInt(playerid, "loadCountMax", cargoInfo[cargo][cWeight] * 2);
			SetPVarInt(playerid, "loadTimer", SetTimerEx("LoadCargo", 1000, true, "id", playerid, IsVehicleTruck(GetVehicleModel(vehicleid)) ? GetVehicleTrailer(vehicleid) : vehicleid));
			TogglePlayerControllable(playerid, false);
			return 1;
		}
	}
	Msg(playerid, COLOR_ERROR, "Nie jesteœ w ¿adnym punkcie za³adunkowym.");
	return 1;
}

CMD:rozladuj(playerid)
{
	for(new i = 0; i < MAX_LOADINGS; i++)
	{
		if(!IsValidLoad(i))
			continue;

		if(IsPlayerInRangeOfPoint(playerid, 7.0, loadInfo[i][lX], loadInfo[i][lY], loadInfo[i][lZ]))
		{
			new vehicleid = GetPlayerVehicleID(playerid);
			new cargo = vloadInfo[IsVehicleTruck(GetVehicleModel(vehicleid)) ? GetVehicleTrailer(vehicleid) : vehicleid][vCargo];

			if (GetPVarInt(playerid, "unloading")) return Msg(playerid, COLOR_ERROR, "Ju¿ roz³adowywujesz towar.");
			if (!IsPlayerInAnyVehicle(playerid)) return Msg(playerid, COLOR_ERROR, "Nie jesteœ w pojeŸdzie.");
			if (!IsPlayerDriver(playerid)) return Msg(playerid, COLOR_ERROR, "Nie jesteœ kierowc¹.");
			if (GetVehicleType(vehicleid) == VEHICLE_TYPE_TRUCK && !GetVehicleTrailer(vehicleid)) return Msg(playerid, COLOR_ERROR, "A gdzie twoja naczepa?");
			if (strcmp(PlayerNick(playerid), vloadInfo[vehicleid][vOwner])) return Msg(playerid, COLOR_ERROR, "Nie jesteœ w³aœcicielem tego towaru.");
			if (vloadInfo[IsVehicleTruck(GetVehicleModel(vehicleid)) ? GetVehicleTrailer(vehicleid) : vehicleid][vLoaded] == false) return Msg(playerid, COLOR_ERROR, "Twój pojazd nie jest za³adowany.");
			if (i != cargoInfo[cargo][cRID]) return Msg(playerid, COLOR_ERROR, "Jesteœ w z³ym punkcie roza³adunku.");
			if (!IsValidOffert(cargo)) return Msg(playerid, COLOR_ERROR, "Wygl¹da na to, ¿e twój towar nie jest ju¿ dostêpny. Je¿li uwa¿asz, ¿e to b³¹d - zg³oœ to.");

			pBar[playerid] = CreatePlayerProgressBar(playerid, 220.00, 435.00, 190.50, 5.50, 1653987839, 0.0);
			ShowPlayerProgressBar(playerid, pBar[playerid]);
			SetPlayerProgressBarMaxValue(playerid, pBar[playerid], 100.0);

			SetPVarInt(playerid, "unloading", 1);
			SetPVarInt(playerid, "unloadProgress", 0);
			SetPVarInt(playerid, "unloadCountMax", cargoInfo[cargo][cWeight] * 2);
			SetPVarInt(playerid, "unloadTimerCount", cargoInfo[cargo][cWeight] * 2);
			SetPVarInt(playerid, "unloadTimer", SetTimerEx("UnloadCargo", 1000, true, "id", playerid, IsVehicleTruck(GetVehicleModel(vehicleid)) ? GetVehicleTrailer(vehicleid) : vehicleid));
			TogglePlayerControllable(playerid, false);
			return 1;
		}
	}
	Msg(playerid, COLOR_ERROR, "Nie jesteœ w ¿adnym punkcie roz³adunkowym.");
	return 1;
}

forward LoadCargo(playerid, vehicleid);
public LoadCargo(playerid, vehicleid)
{
	new count = GetPVarInt(playerid, "loadTimerCount"), progress = GetPVarInt(playerid, "loadProgress");
	if(count - 1 > 0)
	{
		count--;
		progress++;
		new Float:value = floatmul(floatdiv(progress, GetPVarInt(playerid, "loadCountMax")), 100.0);
		SetPVarInt(playerid, "loadProgress", progress);
		SetPlayerProgressBarValue(playerid, pBar[playerid], value);
		SetPVarInt(playerid, "loadTimerCount", count);
	}
	else
	{
		SetPVarInt(playerid, "cargoIDAllowed", 1);
		cargoInfo[GetPVarInt(playerid, "cargoID")][cTimeStart] = gettime();
		DestroyPlayerProgressBar(playerid, pBar[playerid]);
		new RID = cargoInfo[GetPVarInt(playerid, "cargoID")][cRID];
		KillTimer(GetPVarInt(playerid, "loadTimer"));
		Msg(playerid, COLOR_INFO, "Towar zosta³ pomyœlnie za³adowany.");
		vloadInfo[vehicleid][vCargo] = GetPVarInt(playerid, "cargoID");
		vloadInfo[vehicleid][vOwner] = PlayerNick(playerid);
		vloadInfo[vehicleid][vLoaded] = true;
		cargoHealth[vehicleid] = 1000.0;

		RemovePlayerMapIcon(playerid, 69);
		SetPlayerMapIcon(playerid, 69, loadInfo[RID][lX], loadInfo[RID][lY], loadInfo[RID][lZ], 53, 0, MAPICON_GLOBAL);

		TogglePlayerControllable(playerid, true);
		DeletePVar(playerid, "loadTimer");
		DeletePVar(playerid, "loadTimerCount");
		DeletePVar(playerid, "loadProgress");
		DeletePVar(playerid, "loadTimerCount");
		DeletePVar(playerid, "loading");
	}
	return 1;
}

forward UnloadCargo(playerid, vehicleid);
public UnloadCargo(playerid, vehicleid)
{
	new count = GetPVarInt(playerid, "unloadTimerCount"), progress = GetPVarInt(playerid, "unloadProgress");
	if(count - 1 > 0)
	{
		count--;
		progress++;
		new Float:value = floatmul(floatdiv(progress, GetPVarInt(playerid, "unloadCountMax")), 100.0);
		SetPVarInt(playerid, "unloadProgress", progress);
		SetPlayerProgressBarValue(playerid, pBar[playerid], value);
		SetPVarInt(playerid, "unloadTimerCount", count);
	}
	else
	{
		/*

			Zlecenie zosta³o wykonane na (bardzo wysokim / wysokim / normalnym / niskim) poziomie.
			Twoje wynagrodzenie wynosi $%.02f i %d expa.
			Dystans pomiêdzy za³adunkiem a roz³adunkiem to %.02f km.
			SpóŸnienie: (tak / nie(ile))
			Kara: $%d

			Kara za uszkodzenia = uszkodzenia (%) * (stawka * 2) + (waga * $7.5)
			100% uszkodzeñ przy stawce $0.30 = 600$ kary * (km / 1000) + (waga * $7.5)
			(24t) Przy trasie 2000km i uszkodzeniach 100% z stawk¹ $0.30 dostaniemy $1380 kary z mo¿liwoœci¹ zarobienia max. $960
			(24t) Przy trasie 1000km i uszkodzeniach 100% z stawk¹ $0.30 dostaniemy $780 kary z mo¿liwoœci¹ zarobienia max. $660

			Kara za spóŸnienie = 30$ / minuta

		*/
		new Float:penatly, 
			Float:condition = 0.0, // uszkodzenia 
			late, 
			szString[350], 
			szTemp[32], 
			Float:realReward, 
			score,
			bonus,
			bonusSkillReward = GetBiggerReward(GetPlayerSkill(playerid, SKILL_REWARD)),
			bonusSkillPenatly = GetLessPent(GetPlayerSkill(playerid, SKILL_PENATLY)),
			bonusSkillTime = GetReducedTime(GetPlayerSkill(playerid, SKILL_TIME));

		penatly = condition * (waresInfo[cargoInfo[vloadInfo[vehicleid][vCargo]][cType]][wRate] * 2) * (cargoInfo[vloadInfo[vehicleid][vCargo]][cDistance] * 0.001) + (cargoInfo[vloadInfo[vehicleid][vCargo]][cWeight] * (condition * 0.1));
		late = gettime() - cargoInfo[vloadInfo[vehicleid][vCargo]][cTimeStart];
		late -= (cargoInfo[vloadInfo[vehicleid][vCargo]][cTime] * 60);
		bonus = late;
		late += (bonusSkillTime*60);
		if(late > 0)
			penatly = penatly + ((late / 60) * 30);

		if(bonus < 0)
			bonus = -bonus, bonus = bonus/60, bonus *= 30;
		else
			bonus = 0;
		score = waresInfo[cargoInfo[vloadInfo[vehicleid][vCargo]][cType]][wType] == TYPE_ILLEGAL ? 2 : 1;

		penatly *= (1-(bonusSkillPenatly*0.01));
		realReward = bonus + cargoInfo[vloadInfo[vehicleid][vCargo]][cReward] - penatly;
		realReward *= (1+(bonusSkillReward*0.01));

		format(szString, sizeof szString, "{b}%s{/b} ({b}%d{/b}t)\nZlecenie zosta³o wykonane na {b}%s{/b} poziomie.", waresInfo[cargoInfo[vloadInfo[vehicleid][vCargo]][cType]][wName], cargoInfo[vloadInfo[vehicleid][vCargo]][cWeight], GetCargoConditionName(floatround(penatly)));
		strcat(szString, "\n \n");
		format(szString, sizeof szString, "%sTwoje wynagrodzenie wynosi {b}$%d{/b} (+%d%) oraz {b}%d{/b} expa.", szString, floatround(realReward), bonusSkillReward, score);
		strcat(szString, "\n");
		format(szString, sizeof szString, "%sDystans pomiêdzy za³adunkiem a roz³adunkiem to {b}%.02fj{/b}.", szString, cargoInfo[vloadInfo[vehicleid][vCargo]][cDistance]);
		strcat(szString, "\n");
		format(szString, sizeof szString, "%sBonus za czas: {b}$%d{/b}", szString, bonus);
		strcat(szString, "\n \n");
		format(szTemp, sizeof szTemp, "tak ({b}%d{/b} minut/y)", late/60);
		format(szString, sizeof szString, "%sSpóŸnienie: {b}%s{/b}", szString, late > 0 ? szTemp : "nie");
		strcat(szString, "\n");
		format(szTemp, sizeof szTemp, "(spóŸnienie {b}$%d{/b})", (late/60) * 30);
		format(szString, sizeof szString, "%sKara: {b}$%.02f{/b} %s", szString, penatly, late > 0 ? szTemp : "");
		format(szTemp, sizeof szTemp, "(-%d%)", bonusSkillPenatly);
		format(szString, sizeof szString, "%s %s", szString, penatly > 0 ? szTemp : "");
		GivePlayerMoney(playerid, floatround(realReward));
		Dialog_Show(playerid, DIALOG_OFFER_END, DIALOG_STYLE_MSGBOX, " ", clText(COLOR_INFO2, szString), "Okej", #);

		DestroyPlayerProgressBar(playerid, pBar[playerid]);
		RemovePlayerMapIcon(playerid, 69);
		Msg(playerid, COLOR_INFO, "Towar zosta³ pomyœlnie roz³adowany.");
		DeleteCargo(vloadInfo[vehicleid][vCargo]);
		TogglePlayerControllable(playerid, true);

		KillTimer(GetPVarInt(playerid, "unloadTimer"));

		vloadInfo[vehicleid][vCargo] = 0;
		format(vloadInfo[vehicleid][vOwner], 32, "");
		vloadInfo[vehicleid][vLoaded] = false;

		if(GetPVarInt(playerid, "playerTrailer") > 0)
			DestroyVehicle(GetPVarInt(playerid, "playerTrailer"));
		DeletePVar(playerid, "cargoID");
		DeletePVar(playerid, "unloadTimer");
		DeletePVar(playerid, "unloadTimerCount");
		DeletePVar(playerid, "unloading");
		DeletePVar(playerid, "cargoIDAllowed");
	}
	return 1;
}

stock GetCargoConditionName(condition)
{
	new name[24];
	switch(condition)
	{
		case 0:
			strcat(name, "bardzo wysokim");
		case 1 .. 100:
			strcat(name, "wysokim");
		case 101 .. 200:
			strcat(name, "normalnym");
		default: 
			strcat(name, "niskim");
	}
	return name;
}

stock DeleteCargo(cargoid)
{
	if(!IsValidOffert(cargoid))
		return printf("Fatal error.");

	cargoInfo[cargoid][cDeleted] = true;
	return 1;
}

stock LoadObjects()
{
	new line[255], File:olist, count;
	olist = fopen("/bases/objects.txt", io_read);
	while(fread(olist, line)) 
	{
		count++;
		if (line[0] != '/' && strfind(line, "CreateDynamicObject", true) != -1) 
		{
			new objectid, Float:X, Float:Y, Float:Z, Float:rx, Float:ry, Float:rz, worldid = -1, interiorid = -1, playerid = -1, Float:distance = 300;
			if (sscanf(line, "p<,>'('iffffffD(-1)D(-1)D(-1)p<)>F(-1)", objectid, X, Y, Z, rx, ry, rz, worldid, interiorid, playerid, distance)) 
			{
				if (sscanf(line, "p<,>'('ifffffp<)>f", objectid, X, Y, Z, rx, ry, rz)) 
					continue;
				worldid = -1;interiorid = -1;playerid = -1;distance = -1;
			}
			if (distance == -1) 
				distance = 300.0;
			objectsInfo[count] = CreateDynamicObject(objectid, X, Y, Z, rx, ry, rz, worldid, interiorid, playerid, distance);
		}
	}
	fclose(olist);
	return 1;
}

stock DestroyObjects()
{
	for(new i = 0; i < MAX_BASES_OBJECTS; i++)
	{
		if(!IsValidDynamicObject(i))
			continue;

		DestroyDynamicObject(objectsInfo[i]);
	}
	return 1;
}

CMD:anuluj(playerid)
{
	if(GetPVarInt(playerid, "cargoID") < 1)
		return Msg(playerid, COLOR_ERROR, "Nie posidasz zlecenia.");
	Dialog_Show(playerid, DIALOG_CANCEL_OFFERT, DIALOG_STYLE_MSGBOX, " ", "Czy jesteœ pewien, ¿e chcesz anulowaæ twoje zlecenie?\nZostaniesz obci¹¿ony kosztem $5000.", "Tak", "Nie");
	return 1;
}

Dialog:DIALOG_CANCEL_OFFERT(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	if(gettime() > cargoInfo[GetPVarInt(playerid, "cargoID")][cTimeEnd])
		DeleteCargo(GetPVarInt(playerid, "cargoID"));
	else
		cargoInfo[GetPVarInt(playerid, "cargoID")][cReserved] = false;
	GivePlayerMoney(playerid, -5000);
	Msg(playerid, COLOR_INFO, "Zlecenie zosta³o anulowane.");
	DeletePVar(playerid, "cargoID");
	RemovePlayerMapIcon(playerid, 69);
	if(GetPVarInt(playerid, "playerTrailer") > 0)
		DestroyVehicle(GetPVarInt(playerid, "playerTrailer"));
	DeletePVar(playerid, "playerTrailer");
	DeletePVar(playerid, "cargoIDAllowed");
	return 1;
}

CMD:papiery(playerid)
{
	if(!GetPVarInt(playerid, "cargoID"))
		return Msg(playerid, COLOR_ERROR, "Twój schowek na papiery jest pusty! Nie przewozisz ¿adnego towaru.");
	if(!GetPVarInt(playerid, "cargoIDAllowed"))
		return Msg(playerid, COLOR_ERROR, "Udaj siê na za³adunek aby odebraæ papiery.");
	new	
		szString[170],
		id = GetPVarInt(playerid, "cargoID"),
		ZID = cargoInfo[id][cZID], 
		RID = cargoInfo[id][cRID], 
		time[6],
		bonusSkillTime = GetReducedTime(GetPlayerSkill(playerid, SKILL_TIME));

	TimestampToDate((cargoInfo[id][cTime]*60)+cargoInfo[id][cTimeStart]+(bonusSkillTime*60), time[0], time[1], time[2], time[3], time[4], time[5]);

	format(szString, sizeof szString, "%s ({b}%d{/b}t)", waresInfo[cargoInfo[id][cType]][wName], cargoInfo[id][cWeight]);
	strcat(szString, "\n \n");
	format(szString, sizeof szString, "%sZa³adunek:\t{b}%s{/b} (ID: {b}%d{/b})", szString, GetPointZone(loadInfo[ZID][lX], loadInfo[ZID][lY]), ZID);
	strcat(szString, "\n");
	format(szString, sizeof szString, "%sRoz³adunek:\t{b}%s{/b} (ID: {b}%d{/b})", szString, GetPointZone(loadInfo[RID][lX], loadInfo[RID][lY]), RID);
	strcat(szString, "\n");
	format(szString, sizeof szString, "%sDystans:\t{b}%.02f{/b} km", szString, cargoInfo[id][cDistance]);
	strcat(szString, "\n");
	format(szString, sizeof szString, "%sDostawa: {b}%02d:%02d{/b}.", szString, time[3], time[4]);

	Dialog_Show(playerid, DIALOG_PAPERS, DIALOG_STYLE_MSGBOX, " ", clText(COLOR_INFO2, szString), "Okej", #);
	return 1;
}
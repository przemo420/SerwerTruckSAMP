#define MAX_PHOTORADARS 150

enum ePhoto
{
	phID,
	Float:phX,		// pozycja x
	Float:phY,		// pozycja y
	Float:phZ,		// pozycja z
	Float:phA,		// obrót
	phMaxSpeed,		// maksymalna prêdkoœc foto
	bool:phCity,	// czy teren zabudowany 
	Text3D:phText,	// label
	phObject,		// id obiektu
	Float:phRange 	// zasiêg dzia³ania fotoradaru 
}

new bool:pPhotoed[MAX_PLAYERS],
	phInfo[MAX_PHOTORADARS][ePhoto];

forward Photo_OnGameModeInit();
forward Photo_OnGameModeExit();
forward Photo_OnPlayerConnect(playerid);
forward PhotoradarUpdate();
forward UnlockPhotoradar(playerid);

public Photo_OnGameModeInit()
{
	PhotoradarsInit();
	SetTimer("PhotoradarUpdate", 500, true);
	return 1;
}

public Photo_OnGameModeExit()
{
	PhotoradarsExit();
	return 1;
}


public Photo_OnPlayerConnect(playerid)
{
	UnlockPhotoradar(playerid);
	DeletePVar(playerid, "PHOTO_City");
	DeletePVar(playerid, "PHOTO_ID");
	DeletePVar(playerid, "PHOTO_City");
	DeletePVar(playerid, "PHOTO_Object");
	DeletePVar(playerid, "PHOTO_Range");
	return 1;
}

public PhotoradarUpdate()
{
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if(!IsPlayerConnected(i) || pPhotoed[i] || GetPlayerState(i) != PLAYER_STATE_DRIVER)
			continue;

		if(IsWorked(i, TEAM_TYPE_POLICE) || IsWorked(i, TEAM_TYPE_MEDIC))
			continue;

		new vehicleid = GetPlayerVehicleID(i), 
			photoradar = IsPlayerInPhotoRange(i),
			Float:speed, 
			mandate;

		if(photoradar < 0)
			continue;

		pPhotoed[i] = true;
		SetTimerEx("UnlockPhotoradar", 2*1000, false, "d", i);
		GetVehicleSpeed(vehicleid, speed); speed -= phInfo[photoradar][phMaxSpeed];
		if(speed < 10.0)
			continue;

		mandate = GetMandate(floatround(speed));
		if(mandate < 0) // niby nie mo¿liwe, ale przezorny zawsze ubezpieczony.. ;) 
			continue;

		if(phInfo[photoradar][phCity])
			mandate *= 2;

		new szString[160];
		format(szString, sizeof szString, "Przekrocz³yeœ dozwolon¹ prêdkoœc o {b}%d{/b} km/h. Zosta³eœ obci¹¿ony grzywn¹ {b}$%d{/b}.", floatround(speed), mandate);
		Msg(i, COLOR_INFO2, szString);
		Msg(i, COLOR_INFO2, "Mandat zostanie wypisany podczas kontroli.");
		GiveFotoPolice(i, mandate);
	}
	return 1;
}

public UnlockPhotoradar(playerid)
{
	pPhotoed[playerid] = false;
	return 1;
}

stock PhotoradarsInit()
{
	new szBuffer[64];

	mysql_query("SELECT * FROM photos");
	mysql_store_result();
	new nums = mysql_num_rows();
	for(new i = 0; i < nums; i++)
	{
		if(mysql_fetch_row(szBuffer, "|") && i <= MAX_PHOTORADARS)
		{
			sscanf(szBuffer, "p<|>dfffffdd", phInfo[i][phID], phInfo[i][phRange], phInfo[i][phX], phInfo[i][phY], phInfo[i][phZ], phInfo[i][phA], phInfo[i][phMaxSpeed], phInfo[i][phCity]);
		
			new szString[67];
			format(szString, sizeof szString, "Fotoradar (%d) w terenie %szabudowanym\nOgraniczenie %d km/h\nZasiêg %.02f", i, (phInfo[i][phCity]) ? ("") : ("nie"), phInfo[i][phMaxSpeed], phInfo[i][phRange]);
			phInfo[i][phText] = CreateDynamic3DTextLabel(szString, -1, phInfo[i][phX], phInfo[i][phY], phInfo[i][phZ]+3.0, 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0);
			phInfo[i][phObject] = CreateDynamicObject(18880, phInfo[i][phX], phInfo[i][phY], phInfo[i][phZ], 0.0, 0.0, phInfo[i][phA]);
		}
	}
	mysql_free_result();
	return 1;
}

stock PhotoradarsExit()
{
	return 1;
}

stock GetMandate(speed)
{
	switch(speed)
	{
		case 0 .. 9:
			return -1;

		case 10:
			return 50;

		case 11 .. 20:
			return 100;

		case 21 .. 30:
			return 200;

		case 31 .. 40:
			return 340;

		case 41 .. 50:
			return 500;

		default: return 750;
	}
	return -1;
}

stock IsPlayerInPhotoRange(playerid)
{
	for(new i = 0; i < MAX_PHOTORADARS; i++)
	{
		if(!IsValidPhotoradar(i))
			continue;

		if(IsPlayerInRangeOfPoint(playerid, phInfo[i][phRange], phInfo[i][phX], phInfo[i][phY], phInfo[i][phZ]))
			return i;
	}

	return -1;
}

CMD:foto(playerid, params[])
{

	new option[10], photoid;

	if(sscanf(params, "s[10]D(200)", option, photoid))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /foto [stworz/usun/polozenie/edytuj/rozlicz]");

	if((GetPlayerVirtualWorld(playerid) != 0) || IsPlayerInAnyVehicle(playerid)) 
		return Msg(playerid, COLOR_ERROR, "Nie mo¿esz teraz tego zrobiæ.");

	DeletePVar(playerid, "PHOTO_City");
	DeletePVar(playerid, "PHOTO_ID");
	DeletePVar(playerid, "PHOTO_City");
	DeletePVar(playerid, "PHOTO_Object");
	DeletePVar(playerid, "PHOTO_Range"); 

	if(strcmp(option, "stworz", true) == 0) 
	{
		if(!IsFractionBoss(playerid) || !IsFractionViceBoss(playerid)) return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");
		DeletePVar(playerid, "PHOTO_City");
		Dialog_Show(playerid, DIALOG_PHOTO_CREATE, DIALOG_STYLE_LIST, " ", "Teren zabudowany\nTeren niezabudowany", "Wybierz", "WyjdŸ");
	}
	else if(strcmp(option, "usun", true) == 0) 
	{
		if(!IsFractionBoss(playerid) || !IsFractionViceBoss(playerid)) return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");
		if(!IsValidPhotoradar(photoid)) return Msg(playerid, COLOR_ERROR, "Nie odnaleziono fotoradaru z tym ID.");
		DestroyPhotoradar(photoid);
		Msg(playerid, COLOR_INFO, "Pomyœlnie usuniêto fotoradar.");
	}
	else if(strcmp(option, "polozenie", true) == 0) 
	{
		if(!IsFractionBoss(playerid) || !IsFractionViceBoss(playerid)) return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");
		if(!IsValidPhotoradar(photoid)) return Msg(playerid, COLOR_ERROR, "Nie odnaleziono fotoradaru z tym ID.");
		if(!IsPlayerInRangeOfPoint(playerid, 8.0, phInfo[photoid][phX], phInfo[photoid][phY], phInfo[photoid][phZ])) return Msg(playerid, COLOR_ERROR, "Jesteœ za daleko od fotoradaru który chcesz edytowaæ.");
		SetPVarInt(playerid, "PHOTO_Object", phInfo[photoid][phObject]);
		SetPVarInt(playerid, "PHOTO_ID", photoid);
		EditDynamicObject(playerid, phInfo[photoid][phObject]);
	}
	else if(strcmp(option, "edytuj", true) == 0) 
	{
		if(!IsFractionBoss(playerid) || !IsFractionViceBoss(playerid)) return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");
		if(!IsValidPhotoradar(photoid)) return Msg(playerid, COLOR_ERROR, "Nie odnaleziono fotoradaru z tym ID.");
		Dialog_Show(playerid, DIALOG_PHOTO_EDIT, DIALOG_STYLE_LIST, " ", "Teren zabudowany\nTeren niezabudowany", "Wybierz", "WyjdŸ");
		SetPVarInt(playerid, "PHOTO_ID", photoid);
	}
	else if(strcmp(option, "rozlicz", true) == 0)
	{
		if(!IsPlayerConnected(photoid)) return Msg(playerid, COLOR_ERROR, "Gracza o tym ID nie ma online.");
		if(GetFotoPolice(photoid) == 0) return Msg(playerid, COLOR_ERROR, "Ten gracz nie ma zaleg³ych mandatów.");

		GivePlayerMoney(photoid, -GetFotoPolice(photoid));
		SetFotoPolice(photoid, 0);
		new szString[128];
		format(szString, sizeof szString, "Rozliczy³eœ gracza {b}%s{/b} z zaleg³ych mandatów.", PlayerName(photoid));
		Msg(playerid, COLOR_INFO, szString);

		format(szString, sizeof szString, "Policjant {b}%s{/b} rozliczy³ twoje zaleg³e mandaty.", PlayerName(playerid));
		Msg(photoid, COLOR_INFO, szString);
	}
	else
		return Msg(playerid, COLOR_ERROR, "Wpisz: /foto [stworz/usun/polozenie/edytuj/rozlicz]");
	return 1;
}

Dialog:DIALOG_PHOTO_CREATE(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	switch(listitem)
	{
		case 0:
			SetPVarInt(playerid, "PHOTO_City", 1);
		case 1:
			SetPVarInt(playerid, "PHOTO_City", 0);
	}
	Dialog_Show(playerid, DIALOG_PHOTO_CREATE2, DIALOG_STYLE_INPUT, " ", "Poni¿ej wprowadŸ zasiêg na jaki ma reagowaæ fotoradar.\nZasiêg musi znajdowaæ siê w przedziale 5.0 - 20.0", "Dalej", "Wstecz");
	return 1;
}

Dialog:DIALOG_PHOTO_CREATE2(playerid, response, listitem, inputtext[])
{
	if(!response)
		return cmd_foto(playerid, "stworz");

	if(floatstr(inputtext) < 5.0 || floatstr(inputtext) > 20.0)
		return Dialog_Show(playerid, DIALOG_PHOTO_CREATE2, DIALOG_STYLE_INPUT, " ", "Poni¿ej wprowadŸ zasiêg na jaki ma reagowaæ fotoradar.\nZasiêg musi znajdowaæ siê w przedziale 5.0 - 20.0\n \nPoda³eœ z³y zasiêg.", "Dalej", "Wstecz");
	
	SetPVarFloat(playerid, "PHOTO_Range", floatstr(inputtext));
	Dialog_Show(playerid, DIALOG_PHOTO_CREATE3, DIALOG_STYLE_INPUT, " ", "Poni¿ej wprowadŸ prêdkoœæ przy jakiej ma reagowaæ fotoradar.\nPrêdkoœæ musi znajdowaæ siê w przedziale 40 - 220.", "Dalej", "Wstecz");	
	return 1;
}

Dialog:DIALOG_PHOTO_CREATE3(playerid, response, listitem, inputtext[])
{
	if(!response)
		return Dialog_Show(playerid, DIALOG_PHOTO_CREATE2, DIALOG_STYLE_INPUT, " ", "Poni¿ej wprowadŸ zasiêg na jaki ma reagowaæ fotoradar.\nZasiêg musi znajdowaæ siê w przedziale 5.0 - 20.0", "Dalej", "Wstecz");

	if(strval(inputtext) < 40 || strval(inputtext) > 220)
		return Dialog_Show(playerid, DIALOG_PHOTO_CREATE3, DIALOG_STYLE_INPUT, " ", "Poni¿ej wprowadŸ prêdkoœæ przy jakiej ma reagowaæ fotoradar.\nPrêdkoœæ musi znajdowaæ siê w przedziale 40 - 220.\n \nPoda³eœ z³¹ prêdkoœæ.", "Dalej", "Wstecz");
	
	new 
		Float:pos[4],
		pid,
		szString[48];

	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	GetPlayerFacingAngle(playerid, pos[3]);
	pid = CreatePhotoradar(GetPVarFloat(playerid, "PHOTO_Range"), pos[0], pos[1], pos[2], pos[3], strval(inputtext), (GetPVarInt(playerid, "PHOTO_City") > 0) ? (true) : (false));
	if(pid < 0)
		return Msg(playerid, COLOR_ERROR, "Nie uda³o siê stworzyæ fotoradaru.");
	
	SetPlayerPos(playerid, pos[0], pos[1], pos[2]+5.0);
	format(szString, sizeof szString, "Pomyœlnie stworzono fotoradar (ID: {b}%d{/b}).", pid);
	Msg(playerid, COLOR_INFO, szString);
	return 1;
}

Dialog:DIALOG_PHOTO_EDIT(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	switch(listitem)
	{
		case 0:
			SetPVarInt(playerid, "PHOTO_City", 1);
		case 1:
			SetPVarInt(playerid, "PHOTO_City", 0);
	}
	Dialog_Show(playerid, DIALOG_PHOTO_EDIT2, DIALOG_STYLE_INPUT, " ", "Poni¿ej wprowadŸ zasiêg na jaki ma reagowaæ fotoradar.\nZasiêg musi znajdowaæ siê w przedziale 5.0 - 20.0", "Dalej", "Wstecz");
	return 1;
}

Dialog:DIALOG_PHOTO_EDIT2(playerid, response, listitem, inputtext[])
{
	if(!response)
		return cmd_foto(playerid, "stworz");

	if(floatstr(inputtext) < 5.0 || floatstr(inputtext) > 20.0)
		return Dialog_Show(playerid, DIALOG_PHOTO_EDIT2, DIALOG_STYLE_INPUT, " ", "Poni¿ej wprowadŸ zasiêg na jaki ma reagowaæ fotoradar.\nZasiêg musi znajdowaæ siê w przedziale 5.0 - 20.0\n \nPoda³eœ z³y zasiêg.", "Dalej", "Wstecz");
	
	SetPVarFloat(playerid, "PHOTO_Range", floatstr(inputtext));
	Dialog_Show(playerid, DIALOG_PHOTO_EDIT3, DIALOG_STYLE_INPUT, " ", "Poni¿ej wprowadŸ prêdkoœæ przy jakiej ma reagowaæ fotoradar.\nPrêdkoœæ musi znajdowaæ siê w przedziale 40 - 220.", "Dalej", "Wstecz");	
	return 1;
}

Dialog:DIALOG_PHOTO_EDIT3(playerid, response, listitem, inputtext[])
{
	if(!response)
		return Dialog_Show(playerid, DIALOG_PHOTO_EDIT2, DIALOG_STYLE_INPUT, " ", "Poni¿ej wprowadŸ zasiêg na jaki ma reagowaæ fotoradar.\nZasiêg musi znajdowaæ siê w przedziale 5.0 - 20.0", "Dalej", "Wstecz");

	if(strval(inputtext) < 40 || strval(inputtext) > 220)
		return Dialog_Show(playerid, DIALOG_PHOTO_EDIT3, DIALOG_STYLE_INPUT, " ", "Poni¿ej wprowadŸ prêdkoœæ przy jakiej ma reagowaæ fotoradar.\nPrêdkoœæ musi znajdowaæ siê w przedziale 40 - 220.\n \nPoda³eœ z³¹ prêdkoœæ.", "Dalej", "Wstecz");
	
	new 
		pid = GetPVarInt(playerid, "PHOTO_ID"),
		szString[48];

	EditPhotoradar(pid, GetPVarFloat(playerid, "PHOTO_Range"), phInfo[pid][phX], phInfo[pid][phY], phInfo[pid][phZ], phInfo[pid][phA], strval(inputtext), (GetPVarInt(playerid, "PHOTO_City") > 0) ? (true) : (false));
	DeletePVar(playerid, "PHOTO_City"); 
	DeletePVar(playerid, "PHOTO_ID"); 
	DeletePVar(playerid, "PHOTO_Range"); 

	format(szString, sizeof szString, "Pomyœlnie zedytowano fotoradar (ID: {b}%d{/b}).", pid);
	Msg(playerid, COLOR_INFO, szString);
	return 1;
}

forward Photo_OnPlayerEditDynObject(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz);
public Photo_OnPlayerEditDynObject(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
	if(objectid == GetPVarInt(playerid, "PHOTO_Object"))
	{
		switch(response)
		{
			case EDIT_RESPONSE_CANCEL:
			{
				DeletePVar(playerid, "PHOTO_Object"); DeletePVar(playerid, "PHOTO_ID"); 
				return 1;
			}
			case EDIT_RESPONSE_FINAL:
			{
				EditPhotoradar(GetPVarInt(playerid, "PHOTO_ID"), phInfo[GetPVarInt(playerid, "PHOTO_ID")][phRange], x, y, z, rz, phInfo[GetPVarInt(playerid, "PHOTO_ID")][phMaxSpeed], phInfo[GetPVarInt(playerid, "PHOTO_ID")][phCity]);
				DeletePVar(playerid, "PHOTO_Object"); DeletePVar(playerid, "PHOTO_ID"); 
				Msg(playerid, COLOR_INFO, "Pomyœlnie zmieniono pozycje fotoradaru.");
				return 1;
			}
			case EDIT_RESPONSE_UPDATE:
			{
				return 1;
			}
		}
	}
	return 1;
}

stock IsValidPhotoradar(photoid)
{
	if(phInfo[photoid][phMaxSpeed] > 0)
		return true;
	return false;
}

stock CreatePhotoradar(Float:range, Float:x, Float:y, Float:z, Float:a, maxspeed, bool:city)
{
	for(new i = 0; i < MAX_PHOTORADARS; i++)
	{
		if(IsValidPhotoradar(i))
			continue;

		new szString[67];
		format(szString, sizeof szString, "Fotoradar (%d) w terenie %szabudowanym\nOgraniczenie %d km/h\nZasiêg %.02f", i, (city) ? ("") : ("nie"), maxspeed, range);
		phInfo[i][phText] = CreateDynamic3DTextLabel(szString, -1, x, y, z+3.0, 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0);
		phInfo[i][phObject] = CreateDynamicObject(18880, x, y, z, 0.0, 0.0, a);

		phInfo[i][phX] = x; phInfo[i][phY] = y; phInfo[i][phZ] = z; phInfo[i][phA] = a;
		phInfo[i][phMaxSpeed] = maxspeed; phInfo[i][phCity] = city; phInfo[i][phRange] = range;

		new szQuery[145];
		format(szQuery, sizeof szQuery, "INSERT INTO photos (PID, phRange, phX, phY, phZ, phA, phMaxSpeed, phCity) VALUES ('%d', %.02f, %.03f, %.03f, %.03f, %.03f, %d, %d)", i, range, x, y, z, a, maxspeed, city);
		mysql_query(szQuery);
		return i;
	}
	return -1;
}

stock DestroyPhotoradar(photoid)
{
	if(!IsValidPhotoradar(photoid))
		return 0;

	DestroyDynamicObject(phInfo[photoid][phObject]);
	DestroyDynamic3DTextLabel(phInfo[photoid][phText]);

	phInfo[photoid][phText] = Text3D:0;
	phInfo[photoid][phObject] = 0;
	phInfo[photoid][phMaxSpeed] = 0;
	phInfo[photoid][phRange] = 0.0;
	phInfo[photoid][phX] = 0.0;
	phInfo[photoid][phY] = 0.0;
	phInfo[photoid][phZ] = 0.0;
	phInfo[photoid][phA] = 0.0;
	phInfo[photoid][phCity] = false;

	new szQuery[48];
	format(szQuery, sizeof szQuery, "DELETE FROM photos WHERE PID = '%d'", photoid);
	mysql_query(szQuery);
	return 1;
}

stock EditPhotoradar(photoid, Float:range, Float:x, Float:y, Float:z, Float:a, maxspeed, bool:city)
{
	if(!IsValidPhotoradar(photoid))
		return 0;

	new szQuery[160];
	format(szQuery, sizeof szQuery, "UPDATE photos SET phRange = '%.02f', phX = '%.03f', phY = '%.03f', phZ = '%.03f', phA = '%.03f', phMaxSpeed = '%d', phCity = '%d' WHERE PID = '%d'", range, x, y, z, a, maxspeed, city, photoid);
	mysql_query(szQuery);

	DestroyDynamic3DTextLabel(phInfo[photoid][phText]);
	SetDynamicObjectPos(phInfo[photoid][phObject], x, y, z);
	SetDynamicObjectRot(phInfo[photoid][phObject], 0.0, 0.0, a);
	format(szQuery, sizeof szQuery, "Fotoradar (%d) w terenie %szabudowanym\nOgraniczenie %d km/h\nZasiêg %.02f", photoid, (city) ? ("") : ("nie"), maxspeed, range);
	phInfo[photoid][phText] = CreateDynamic3DTextLabel(szQuery, -1, x, y, z+3.0, 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0);

	phInfo[photoid][phMaxSpeed] = maxspeed;
	phInfo[photoid][phX] = x;
	phInfo[photoid][phY] = y;
	phInfo[photoid][phZ] = z;
	phInfo[photoid][phA] = a;
	phInfo[photoid][phCity] = city;
	phInfo[photoid][phRange] = range;
	return 1;
}
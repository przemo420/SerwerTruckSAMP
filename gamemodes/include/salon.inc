#define VEH_TYPE_NORMAL_CAR 	1
#define VEH_TYPE_BIG_CAR 		2
#define VEH_TYPE_MOTORBIKE 		3
#define VEH_TYPE_BIKE 			4
#define VEH_TYPE_PLANE			5
#define VEH_TYPE_HELI 			6
#define VEH_TYPE_BOAT 			7
#define VEH_TYPE_TRAILER 		8
#define VEH_TYPE_RC 			9
#define VEH_TYPE_SPECIAL	 	10

#define SALOON_PERSONAL			0
#define SALOON_COMPANY			1

#define S_SALOON 				0
#define S_MODEL					1
#define S_PRICE					2
#define S_VMAX					3
#define S_DOORS					4

new saloonVehicles[][] = {
	{SALOON_PERSONAL, 602, 320000, 159, 2}, 
	{SALOON_PERSONAL, 445, 300000, 154, 2}, {SALOON_PERSONAL, 518, 270000, 154, 2},
	{SALOON_PERSONAL, 496, 290000, 153, 2}, {SALOON_PERSONAL, 581, 380000, 180, 2},
	{SALOON_PERSONAL, 575, 180000, 148, 2}, {SALOON_PERSONAL, 536, 310000, 162, 2},
	{SALOON_PERSONAL, 401, 230000, 138, 2}, {SALOON_PERSONAL, 422, 170000, 131, 2},
	{SALOON_PERSONAL, 589, 310000, 153, 2}, {SALOON_PERSONAL, 542, 375000, 154, 2},
	{SALOON_PERSONAL, 527, 220000, 140, 2}, {SALOON_PERSONAL, 480, 510000, 173, 2},
	{SALOON_PERSONAL, 507, 355000, 156, 2}, {SALOON_PERSONAL, 419, 230000, 140, 2},
	{SALOON_PERSONAL, 565, 350000, 155, 2}, {SALOON_PERSONAL, 463, 460000, 155, 2},
	{SALOON_PERSONAL, 526, 270000, 148, 2}, {SALOON_PERSONAL, 533, 310000, 157, 2},
	{SALOON_PERSONAL, 462, 110000, 170, 2}, {SALOON_PERSONAL, 466, 190000, 138, 2},
	{SALOON_PERSONAL, 579, 350000, 148, 2}, {SALOON_PERSONAL, 545, 190000, 138, 2},
	{SALOON_PERSONAL, 474, 190000, 140, 2}, {SALOON_PERSONAL, 559, 480000, 167, 2},
	{SALOON_PERSONAL, 508, 200000, 101, 2}, {SALOON_PERSONAL, 400, 270000, 148, 2},
	{SALOON_PERSONAL, 500, 220000, 132, 2}, {SALOON_PERSONAL, 551, 250000, 148, 2},
	{SALOON_PERSONAL, 410, 130000, 122, 2}, {SALOON_PERSONAL, 467, 230000, 132, 2},
	{SALOON_PERSONAL, 600, 210000, 141, 2}, {SALOON_PERSONAL, 436, 120000, 140, 2},
	{SALOON_PERSONAL, 547, 210000, 134, 2}, {SALOON_PERSONAL, 426, 400000, 163, 2},
	{SALOON_PERSONAL, 603, 410000, 161, 2}, {SALOON_PERSONAL, 461, 450000, 190, 2},
	{SALOON_PERSONAL, 534, 310000, 158, 2}, {SALOON_PERSONAL, 479, 180000, 131, 2},
	{SALOON_PERSONAL, 489, 410000, 131, 2}, {SALOON_PERSONAL, 475, 320000, 162, 2},
	{SALOON_PERSONAL, 495, 480000, 166, 2}, {SALOON_PERSONAL, 535, 370000, 148, 2}, 
	{SALOON_PERSONAL, 561, 320000, 145, 2}, {SALOON_PERSONAL, 405, 335000, 154, 2},
	{SALOON_PERSONAL, 567, 240000, 162, 2}, {SALOON_PERSONAL, 458, 170000, 148, 2},
	{SALOON_PERSONAL, 439, 300000, 158, 2}, {SALOON_PERSONAL, 580, 150000, 143, 2},
	{SALOON_PERSONAL, 543, 140000, 142, 2}, {SALOON_PERSONAL, 550, 270000, 136, 2},
	{SALOON_PERSONAL, 560, 320000, 159, 2}, {SALOON_PERSONAL, 468, 355000, 170, 2},
	{SALOON_PERSONAL, 549, 190000, 144, 2}, {SALOON_PERSONAL, 576, 230000, 148, 2},
	{SALOON_PERSONAL, 566, 200000, 150, 2}, {SALOON_PERSONAL, 558, 400000, 146, 2},
	{SALOON_PERSONAL, 412, 250000, 158, 2}, {SALOON_PERSONAL, 540, 160000, 140, 2},
	{SALOON_PERSONAL, 421, 180000, 144, 2}, {SALOON_PERSONAL, 555, 320000, 148, 2},
	{SALOON_PERSONAL, 529, 230000, 140, 2}, {SALOON_PERSONAL, 478, 120000, 110, 2},
	{SALOON_PERSONAL, 586, 270000, 180, 2}, {SALOON_PERSONAL, 554, 280000, 135, 2},
	{SALOON_PERSONAL, 477, 530000, 175, 2}, {SALOON_PERSONAL, 521, 510000, 190, 2},
	{SALOON_PERSONAL, 587, 325000, 155, 2}, {SALOON_PERSONAL, 562, 500000, 167, 2},
	{SALOON_COMPANY, 482, 550000, 167, 2}, {SALOON_COMPANY, 403, 600000, 155, 2}, 
	{SALOON_COMPANY, 515, 750000, 155, 2}, {SALOON_COMPANY, 514, 700000, 155, 2}, 
	{SALOON_COMPANY, 435, 325000, 0, 0}, {SALOON_COMPANY, 450, 325000, 0, 0},
	{SALOON_COMPANY, 584, 325000, 0, 0}, {SALOON_COMPANY, 591, 325000, 0, 0},
	{SALOON_COMPANY, 440, 350000, 0, 0}, {SALOON_COMPANY, 455, 600000, 0, 0},
	{SALOON_COMPANY, 459, 400000, 0, 0}, {SALOON_COMPANY, 499, 325000, 0, 0},
	{SALOON_COMPANY, 414, 275000, 0, 0}, {SALOON_COMPANY, 578, 400000, 0, 0}
};

CMD:salon(playerid)
{
	if(IsPlayerInRangeOfPoint(playerid, 4.0, -1969.291, 296.353, 35.171)) // salon samochodów osobowych
	{
		if(GetPVarInt(playerid, "SALOON_Block")) return Msg(playerid, COLOR_ERROR, "Teraz nie mo¿esz tego zrobiæ.");
		Dialog_Show(playerid, DIALOG_SALOON_PERSONAL, DIALOG_STYLE_LIST, "Salon samochodów osobowych", "Samochody osobowe\nMotocykle", "Wybierz", "WyjdŸ");
	}
	else if(IsPlayerInRangeOfPoint(playerid, 4.0, -1649.904, 1209.725, 7.250)) // salon pojazdów ciê¿arowych
	{
		if(GetPVarInt(playerid, "SALOON_Block")) return Msg(playerid, COLOR_ERROR, "Teraz nie mo¿esz tego zrobiæ.");
		if(!(IsFractionBoss(playerid) || IsFractionViceBoss(playerid)))
			return Msg(playerid, COLOR_ERROR, "Nie jesteœ szefem ¿adnej firmy.");
		new firmaid = playerInfo[playerid][pFirm];
		if(firmaid < 4)
			return Msg(playerid, COLOR_ERROR, "Twoja frakcja nie musi kupowaæ pojazdów.");
		Dialog_Show(playerid, DIALOG_SALOON_COMPANY, DIALOG_STYLE_LIST, "Salon samochodów ciê¿arowych", "Samochody dostawcze\nCiê¿arówki\nNaczepy", "Wybierz", "WyjdŸ");
	}
	else
		return Msg(playerid, COLOR_ERROR, "Nie jesteœ w punkcie salonu.");
	return 1;
}

Dialog:DIALOG_SALOON_PERSONAL(playerid, response, listitem, inputtext[])
{
	if(!response)
	{
		DeletePVar(playerid, "SALOON_Type");
		DeletePVar(playerid, "SALOON_Vehicle");
		DeletePVar(playerid, "SALOON_Camera");
		DeletePVar(playerid, "SALOON_Timer");
		return 1;
	}

	new type, szString[1200];
	switch(listitem)
	{
		case 0: type = VEH_TYPE_NORMAL_CAR;
		case 1: type = VEH_TYPE_MOTORBIKE;
	}
	for(new i = 0; i < sizeof saloonVehicles; i++)
	{
		if(saloonVehicles[i][S_SALOON] != SALOON_PERSONAL)
			continue;

		if(type != GetVehicleCat(saloonVehicles[i][S_MODEL]))
			continue;

		new szTemp[48];
		format(szTemp, sizeof szTemp, "%d\t%s\t$%d\n", i, GetVehicleModelName(saloonVehicles[i][S_MODEL]), saloonVehicles[i][S_PRICE]);
		strcat(szString, szTemp);
	}
	strins(szString, "ID\tNazwa\tCena\n", 0);
	SetPVarInt(playerid, "SALOON_Type", listitem);
	Dialog_Show(playerid, DIALOG_SALOON_PERSONAL2, DIALOG_STYLE_TABLIST_HEADERS, "Salon samochodów osobowych", szString, "Wybierz", "Wstecz");
	return 1;
}

Dialog:DIALOG_SALOON_PERSONAL2(playerid, response, listitem, inputtext[])
{
	if(!response)
		return cmd_salon(playerid);

	new szString[256], id;
	sscanf(inputtext, "d{s[48]}", id);

	format(szString, sizeof szString, "Pojazd %s (%d)", GetVehicleModelName(saloonVehicles[id][S_MODEL]), saloonVehicles[id][S_MODEL]);
	format(szString, sizeof szString, "%s\n \nCena\t\t$%d", szString, saloonVehicles[id][S_PRICE]);
	format(szString, sizeof szString, "%s\nMoc\t\t350KM", szString);
	format(szString, sizeof szString, "%s\nDrzwi\t\t%d-%s", szString, saloonVehicles[id][S_DOORS], (GetPVarInt(playerid, "SALOON_Type") == 1) ? ("osobowy") : ("drzwiowy"));
	format(szString, sizeof szString, "%s\nZbiornik\t%dl", szString, MaxFuel(saloonVehicles[id][S_MODEL]));
	format(szString, sizeof szString, "%s\nV-Max\t\t%d km/h", szString, saloonVehicles[id][S_VMAX]);
	format(szString, sizeof szString, "%s\n \nKup ten pojazd", szString);
	format(szString, sizeof szString, "%s\nObjerzyj pojazd", szString);
	SetPVarInt(playerid, "SALOON_Vehicle", id);

	Dialog_Show(playerid, DIALOG_SALOON_PERSONAL3, DIALOG_STYLE_LIST, "Salon samochodów osobowych", szString, "Wybierz", "Wstecz");
	return 1;
}

Dialog:DIALOG_SALOON_PERSONAL3(playerid, response, listitem, inputtext[])
{
	if(!response)
		return DIALOG_SALOON_PERSONAL(playerid, true, GetPVarInt(playerid, "SALOON_Type"), "");

	new szString[15];
	format(szString, sizeof szString, "%d\tLinneruner\t$300000\n", GetPVarInt(playerid, "SALOON_Vehicle"));
	switch(listitem)
	{
		case 0 .. 7: return DIALOG_SALOON_PERSONAL2(playerid, true, 0, szString);
		case 8: return BuyPrivateVehicle(playerid);
		case 9: return PreviewPrivateVehicle(playerid);
	}
	return 1;
}

Dialog:DIALOG_SALOON_COMPANY(playerid, response, listitem, inputtext[])
{
	if(!response)
	{
		DeletePVar(playerid, "SALOON_Type");
		DeletePVar(playerid, "SALOON_Vehicle");
		DeletePVar(playerid, "SALOON_Camera");
		DeletePVar(playerid, "SALOON_Timer");
		return 1;
	}

	new type, szString[1200];
	switch(listitem)
	{
		case 0: type = VEH_TYPE_NORMAL_CAR;
		case 1: type = VEH_TYPE_BIG_CAR;
		case 2: type = VEH_TYPE_TRAILER;
	}

	for(new i = 0; i < sizeof saloonVehicles; i++)
	{
		if(saloonVehicles[i][S_SALOON] != SALOON_COMPANY)
			continue;

		if(type != GetVehicleCat(saloonVehicles[i][S_MODEL]))
			continue;

		new szTemp[48];
		format(szTemp, sizeof szTemp, "%d\t%s\t$%d\n", i, GetVehicleModelName(saloonVehicles[i][S_MODEL]), saloonVehicles[i][S_PRICE]);
		strcat(szString, szTemp);
	}

	strins(szString, "ID\tNazwa\tCena\n", 0);
	SetPVarInt(playerid, "SALOON_Type", listitem);
	Dialog_Show(playerid, DIALOG_SALOON_COMPANY2, DIALOG_STYLE_TABLIST_HEADERS, "Salon samochodów ciê¿arowych", szString, "Wybierz", "Wstecz");
	return 1;
}

Dialog:DIALOG_SALOON_COMPANY2(playerid, response, listitem, inputtext[])
{
	if(!response)
		return cmd_salon(playerid);

	new szString[256], id;
	sscanf(inputtext, "d{s[48]}", id);

	format(szString, sizeof szString, "Pojazd %s (%d)", GetVehicleModelName(saloonVehicles[id][S_MODEL]), saloonVehicles[id][S_MODEL]);
	format(szString, sizeof szString, "%s\n \nCena\t\t$%d", szString, saloonVehicles[id][S_PRICE]);
	format(szString, sizeof szString, "%s\nMoc\t\t350KM", szString);
	format(szString, sizeof szString, "%s\nZbiornik\t%dl", szString, MaxFuel(saloonVehicles[id][S_MODEL]));
	format(szString, sizeof szString, "%s\nV-Max\t\t%d km/h", szString, saloonVehicles[id][S_VMAX]);
	format(szString, sizeof szString, "%s\n \nKup ten pojazd", szString);
	SetPVarInt(playerid, "SALOON_Vehicle", id);

	Dialog_Show(playerid, DIALOG_SALOON_COMPANY3, DIALOG_STYLE_LIST, "Salon samochodów ciê¿arowych", szString, "Wybierz", "Wstecz");
	return 1;
}

Dialog:DIALOG_SALOON_COMPANY3(playerid, response, listitem, inputtext[])
{
	if(!response)
		return DIALOG_SALOON_COMPANY(playerid, true, GetPVarInt(playerid, "SALOON_Type"), "");

	new szString[15];
	format(szString, sizeof szString, "%d\tLinneruner\t$300000\n", GetPVarInt(playerid, "SALOON_Vehicle"));
	switch(listitem)
	{
		case 0 .. 6: return DIALOG_SALOON_COMPANY2(playerid, true, 0, szString);
		case 7: return BuyCompanyVehicle(playerid);
	}
	return 1;
}

stock BuyCompanyVehicle(playerid)
{
	new firmaid = playerInfo[playerid][pFirm], szString[390], id = GetPVarInt(playerid, "SALOON_Vehicle"), cash = firmInfo[firmaid][tBank];
	format(szString, sizeof szString, "%d\tLinneruner\t$300000\n", id);
	if(cash < saloonVehicles[id][S_PRICE])
	{
		Msg(playerid, COLOR_ERROR, "Twoja firma nie posiada wystarczaj¹co du¿o funduszy.");
		return DIALOG_SALOON_COMPANY2(playerid, true, 0, szString);
	}

	cash -= saloonVehicles[id][S_PRICE]; firmInfo[firmaid][tBank] = cash;
	DeletePVar(playerid, "SALOON_Type");
	DeletePVar(playerid, "SALOON_Vehicle");
	Msg(playerid, COLOR_INFO, "Pomyœlnie zakupiono pojazd firmowy.");

	new vehicleid = CreateVehicle(saloonVehicles[id][S_MODEL], -1646.6908, 1215.5980, 9.9067, 225.8644, random(50), random(50), -1);

	format(szString, sizeof(szString), "INSERT INTO `Pojazdy` (`model`, `owner_id`, `owner_type`, `PosX`, `PosY`, `PosZ`, `PosA`, `Health`, `Fuel`, `FuelType`, `Color1`, `Color2`, `Plate`, `Plate`, `Przeglad`, `gasBootle`, `gasAmount`) VALUES ('%d', '%d', '1', '-1646.6908', '1215.5980', '9.9067', '225.8644', '1000.0', '%d', '%d', '0', '0', 'SerwerTruck', '%d', '0', '0.0')", GetVehicleModel(vehicleid), firmaid, MaxFuel(GetVehicleModel(vehicleid)), 
			(IsVehicleTruck(vehicleid) ? FUEL_TYPE_TDI : FUEL_TYPE_BENZ), gettime());
	mysql_query(szString);

	DestroyVehicle(vehicleid);
	Spawned[vehicleid] = false;

	LadujPojazd(_, _, mysql_insert_id());
	return 1;
}

stock BuyPrivateVehicle(playerid)
{
	new szString[390], id = GetPVarInt(playerid, "SALOON_Vehicle");
	format(szString, sizeof szString, "%d\tLinneruner\t$300000\n", id);
	
	if(GetPlayerMoney(playerid) < saloonVehicles[id][S_PRICE])
	{
		Msg(playerid, COLOR_ERROR, "Nie posiadasz wystarczaj¹co pieniêdzy aby kupiæ ten pojazd.");
		return DIALOG_SALOON_PERSONAL2(playerid, true, 0, szString);
	}

	GiveMoney(playerid, -saloonVehicles[id][S_PRICE]);
	new vehicleid = CreateVehicle(saloonVehicles[id][S_MODEL], -1988.6996, 275.1513, 34.8315, 87.2160, random(50), random(50), -1);
	format(szString, sizeof(szString), "INSERT INTO `Pojazdy` (`model`, `owner_id`, `owner_type`, `PosX`, `PosY`, `PosZ`, `PosA`, `Health`, `Fuel`, `FuelType`, `Color1`, `Color2`, `Plate`, `Plate`, `Przeglad`, `gasBootle`, `gasAmount`) VALUES ('%d', '%d', '0', '-1646.6908', '1215.5980', '9.9067', '225.8644', '1000.0', '%d', '%d', '0', '0', 'SerwerTruck', NOW(), '0', '0.0')", GetVehicleModel(vehicleid), playerInfo[playerid][pID], MaxFuel(GetVehicleModel(vehicleid)), 
		FUEL_TYPE_BENZ);
	mysql_query(szString);

	DestroyVehicle(vehicleid);
	Spawned[vehicleid] = false;

	LadujPojazd(_, _, mysql_insert_id());

	DeletePVar(playerid, "SALOON_Type");
	DeletePVar(playerid, "SALOON_Vehicle");
	Msg(playerid, COLOR_INFO, "Pomyœlnie zakupiono prywatny pojazd.");
	return 1;
}

stock PreviewPrivateVehicle(playerid)
{
	new id = GetPVarInt(playerid, "SALOON_Vehicle");
	SetPVarInt(playerid, "SALOON_Camera", 0);
	CinematicCamera(playerid);
	SetPVarInt(playerid, "SALOON_Temp", CreateVehicle(saloonVehicles[id][S_MODEL], -1982.1302, 257.6473, 34.9003, 263.6914, random(50), random(50), -1));
	SetPlayerVirtualWorld(playerid, playerid+1);
	SetVehicleVirtualWorld(GetPVarInt(playerid, "SALOON_Temp"), playerid+1);
	return 1;
}

forward CinematicCamera(playerid);
public CinematicCamera(playerid)
{
	switch(GetPVarInt(playerid, "SALOON_Camera"))
	{
		case 0:
		{
			SetPVarInt(playerid, "SALOON_Block", 1);
			SetPVarInt(playerid, "SALOON_Timer", SetTimerEx("CinematicCamera", 3000, true, "d", playerid));
			InterpolateCameraPos(playerid, -1977.852539, 264.268585, 36.433998, -1974.435791, 258.456665, 35.010204, 3000, 1);
			InterpolateCameraLookAt(playerid, -1978.263061, 263.412017, 36.121303, -1978.359863, 257.681823, 35.044998, 3000, 1);
			SetPVarInt(playerid, "SALOON_Camera", 1);
		}
		case 1:
		{
			InterpolateCameraPos(playerid, -1974.435791, 258.456665, 35.010204, -1982.306152, 263.883911, 35.026458, 3000, 1);
			InterpolateCameraLookAt(playerid, -1978.359863, 257.681823, 35.044998, -1981.447753, 259.983306, 34.806152, 3000, 1);
			SetPVarInt(playerid, "SALOON_Camera", 2);
		}
		case 2:
		{
			InterpolateCameraPos(playerid, -1982.306152, 263.883911, 35.026458, -1988.523071, 256.768646, 34.972404, 3000, 1);
			InterpolateCameraLookAt(playerid, -1981.447753, 259.983306, 34.806152, -1984.542846, 256.385345, 35.078639, 3000, 1);
			SetPVarInt(playerid, "SALOON_Camera", 3);
		}
		case 3:
		{
			InterpolateCameraPos(playerid, -1988.523071, 256.768646, 34.972404, -1984.660888, 251.412506, 34.973289, 3000, 1);
			InterpolateCameraLookAt(playerid, -1984.542846, 256.385345, 35.078639, -1983.531982, 255.249801, 34.946830, 3000, 1);
			SetPVarInt(playerid, "SALOON_Camera", 4);
		}
		case 4:
		{
			InterpolateCameraPos(playerid, -1984.660888, 251.412506, 34.973289, -1975.060180, 258.134490, 36.675754, 3000, 1);
			InterpolateCameraLookAt(playerid, -1983.531982, 255.249801, 34.946830, -1978.959106, 257.751251, 35.868831, 3000, 1);
			SetPVarInt(playerid, "SALOON_Camera", 5);
		}
		case 5:
		{
			SetCameraBehindPlayer(playerid);
			KillTimer(GetPVarInt(playerid, "SALOON_Timer"));
			SetPVarInt(playerid, "SALOON_Camera", 0);
			DestroyVehicle(GetPVarInt(playerid, "SALOON_Temp"));
			SetPlayerVirtualWorld(playerid, 0);
			SetPVarInt(playerid, "SALOON_Block", 0);

			new szString[27];
			format(szString, sizeof szString, "%d\tLinneruner\t$300000\n", GetPVarInt(playerid, "SALOON_Vehicle"));
			DIALOG_SALOON_PERSONAL2(playerid, true, 0, szString);
		}
	}
	return 1;
}

stock GetVehicleCat(model) //by Minokon
{
	switch(model)
	{
		case 400..402, 404, 405, 409..416, 418..424, 426, 429, 434, 436, 438..440, 442, 451, 457..459, 466, 467, 470, 474, 475, 477..480, 482, 483, 485, 489..492, 494..496, 498..500, 502..508, 516..518, 526..536, 540..547, 549..552, 554, 555, 558..562, 565..568, 571, 572, 574..576, 579, 580, 582, 583, 585, 587..589, 596..600, 602, 603, 609:
			return VEH_TYPE_NORMAL_CAR;
		case 403, 407, 408, 427, 428, 431, 433, 437, 443, 444, 455, 456, 486, 514, 515, 524, 525, 556, 557, 573, 578, 601:
			return VEH_TYPE_BIG_CAR;
		case 448, 461..463, 468, 471, 521..523, 581, 586:
			return VEH_TYPE_MOTORBIKE;
		case 481, 509, 510:
			return VEH_TYPE_BIKE;
		case 460, 476, 511..513, 519, 520, 553, 577, 592, 593:
			return VEH_TYPE_PLANE;
		case 417, 425, 447, 469, 487, 488, 497, 548, 563:
			return VEH_TYPE_HELI;
		case 430, 446, 452..454, 472, 473, 484, 493, 539, 595:
			return VEH_TYPE_BOAT;
		case 435, 450, 584, 590, 591, 606..608, 610, 611:
			return VEH_TYPE_TRAILER;
		case 441, 464, 465, 501, 564, 594:
			return VEH_TYPE_RC;
		case 449, 537, 538, 569, 570, 604, 605:
			return VEH_TYPE_SPECIAL;
	}
	return 0;
}
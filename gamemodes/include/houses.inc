/*
	native CreateHouse(const type, const price, const interior, const Float:x, const Float:y, const Float:z);
	native DeleteHouse(const houseid);
	native EditHouse(const houseid, const type, const label[], const price, const interior, const owner, const vice_owner, const status, const door);
	native SellHouse(const houseid, const price, const new_owner);

	native LoadHouses();
*/

#define MAX_HOUSES 500



enum E_HOUSE
{
	hsID,
	bool:hsCreated,
	hsLabel[48],
	hsPrice,
	hsInterior,
	hsOwner,
	hsVcOwner,
	Float:hsPos[3],
	hsStatus,
	Text3D:hsLabelID,
	hsPickup,
	hsIcon,
	hsDoor,
	hsPickupExit,
	bool:hsSave
}
new houseInfo[MAX_HOUSES][E_HOUSE];

enum 
{
	STATUS_FREE,
	STATUS_SOLD
}

enum
{
	DOOR_CLOSE,
	DOOR_OPEN
}

new Float:InteriorPos[][4] = {
	{2.0, 267.317, 304.892, 999.148},
	{1.0, 223.194, 1287.787, 1082.140},
	{7.0, 225.713, 1022.303, 1084.015},
	{15.0, 327.959, 1478.367, 1084.437},
	{15.0, 386.199, 1471.805, 1080.187},
	{10.0, 2269.555, -1210.509, 1047.562},
	{1.0, -2158.72, 641.29, 1052.38},
	{1.0, 244.588, 305.142, 999.148},
	{2.0, 2468.007, -1698.257, 1013.507},
	{3.0, 2495.985, -1692.508, 1014.742},
	{2.0, 266.951, 305.145, 999.148},
	{5.0, 1261.664, -785.581, 1091.906},
	{10.0, 421.962, 2536.441, 10.000},
	{17.0, 493.552, -23.939, 1000.679},
	{5.0, 2233.596, -1114.422, 1050.882},
	{9.0, 2317.640, -1026.136, 1050.217},
	{10.0, 2260.188, -1135.787, 1050.632},
	{3.0, 235.236, 1187.381, 1080.257},
	{2.0, 225.946, 1239.919, 1082.140},
	{15.0, 295.209, 1472.925, 1080.257},
	{12.0, 446.721, 507.570, 1001.419},
	{2.0, 446.885, 1398.034, 1084.304},
	{10.0, 23.965, 1340.683, 1084.375},
	{12.0, 2324.244, -1148.722, 1050.710},
	{4.0, -261.665, 1456.754, 1084.367},
	{5.0, 140.368, 1366.478, 1083.859},
	{8.0, 2807.531, -1174.035, 1025.570},
	{1.0, 2217.294, -1076.240, 1050.484},
	{2.0, 2237.545, -1080.424, 1049.023},
	{8.0, 2365.135, -1134.775, 1050.875}
};

stock CreateHouse(price, interior, Float:x, Float:y, Float:z)
{
	new free = -1;
	for(new i = 0; i < MAX_HOUSES; i++)
	{
		if(IsValidHouse(i))
			continue;
		free = i;
		break;
	}
	if(free <= -1)
		return -1;
	
	new szString[356];
	format(szString, sizeof szString, "INSERT INTO `houses`(`ID`, `label`, `price`, `owner`, `vcowner`, `x`, `y`, `z`, `interior`, `status`)");
	format(szString, sizeof szString, "%s VALUES ('null', 'Dom na sprzeda¿', '%d', '0', '0', '%.02f', '%.02f', '%.02f', '%d', '0')",
		szString,
		price,
		x,
		y,
		z,
		interior);
	mysql_query(szString);

	houseInfo[free][hsID] = mysql_insert_id();
	format(houseInfo[free][hsLabel], 48, "Dom na sprzeda¿.");
	houseInfo[free][hsCreated] = true;
	houseInfo[free][hsPrice] = price;
	houseInfo[free][hsInterior] = interior;
	houseInfo[free][hsStatus] = STATUS_FREE;
	houseInfo[free][hsPos][0] = x;
	houseInfo[free][hsPos][1] = y;
	houseInfo[free][hsPos][2] = z;
	houseInfo[free][hsSave] = false;

	format(szString, sizeof szString, "%s", HouseFormat3DLabel(free));

	houseInfo[free][hsLabelID] = CreateDynamic3DTextLabel(clText(COLOR_INFO2, szString), -1, x, y, z, 25.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1);	
	houseInfo[free][hsIcon] = CreateDynamicMapIcon(x, y, z, 31, 0, -1, -1, -1, 100.0, MAPICON_LOCAL);
	houseInfo[free][hsPickup] = CreateDynamicPickup(1273, 1, x, y, z);
	new Float:pos[3], int;
	GetInteriorPos(houseInfo[free][hsInterior], int, pos[0], pos[1], pos[2]);
	houseInfo[free][hsPickupExit] = CreateDynamicPickup(1559, 1, pos[0], pos[1], pos[2], free + 1, int);
	return free;
}

stock EditHouse(houseid, label[], price, interior, owner, vice_owner, status, door)
{

	return 1;
}

stock BuyHouse(playerid, houseid)
{
	if(!IsValidHouse(houseid))
		return 0;

	new szString[256];
	DestroyDynamic3DTextLabel(houseInfo[houseid][hsLabelID]);
	DestroyDynamicMapIcon(houseInfo[houseid][hsIcon]);
	DestroyDynamicPickup(houseInfo[houseid][hsPickup]);
	houseInfo[houseid][hsStatus] = STATUS_SOLD;
	houseInfo[houseid][hsOwner] = playerInfo[playerid][pID];
	houseInfo[houseid][hsDoor] = DOOR_OPEN;
	format(houseInfo[houseid][hsLabel], 48, "Dom gracza %s", PlayerName(playerid));

	format(szString, sizeof szString, "UPDATE houses SET owner = '%d', status = '%d', door = '%d', label = '%s' WHERE ID = '%d'", 
		houseInfo[houseid][hsOwner],
		houseInfo[houseid][hsStatus],
		houseInfo[houseid][hsDoor],
		houseInfo[houseid][hsLabel],
		houseInfo[houseid][hsID]);
	mysql_query(szString);

	format(szString, sizeof szString, "%s", HouseFormat3DLabel(houseid));
	houseInfo[houseid][hsLabelID] = CreateDynamic3DTextLabel(clText(COLOR_INFO2, szString), -1, houseInfo[houseid][hsPos][0], houseInfo[houseid][hsPos][1], houseInfo[houseid][hsPos][2], 25.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1);	
	houseInfo[houseid][hsIcon] = CreateDynamicMapIcon(houseInfo[houseid][hsPos][0], houseInfo[houseid][hsPos][1], houseInfo[houseid][hsPos][2], 32, 0, -1, -1, -1, 100.0, MAPICON_LOCAL);
	houseInfo[houseid][hsPickup] = CreateDynamicPickup(1272, 1, houseInfo[houseid][hsPos][0], houseInfo[houseid][hsPos][1], houseInfo[houseid][hsPos][2]);
	return 1;
}

stock DeleteHouse(houseid)
{
	if(!IsValidHouse(houseid))
		return 0;

	new szString[128];
	format(szString, sizeof szString, "DELETE FROM houses WHERE ID = '%d'", houseInfo[houseid][hsID]);
	mysql_query(szString);

	DestroyDynamic3DTextLabel(houseInfo[houseid][hsLabelID]);
	DestroyDynamicMapIcon(houseInfo[houseid][hsIcon]);
	DestroyDynamicPickup(houseInfo[houseid][hsPickup]);
	DestroyDynamicPickup(houseInfo[houseid][hsPickupExit]);

	ResetVariablesInEnum(houseInfo[houseid], E_HOUSE);
	return 1;
}

stock IsValidHouse(houseid)
{
	if(houseid >= MAX_HOUSES) 
		return false;
	if(houseInfo[houseid][hsCreated])
		return true;
	return false;
}

stock HouseFormat3DLabel(houseid)
{
	new szString[256];
	if(!IsValidHouse(houseid))
		return szString;
	switch(houseInfo[houseid][hsStatus])
	{
		case STATUS_FREE:
		{
			format(szString, sizeof szString, "Aby kupiæ wpisz /buyhouse\n{b}Dom na sprzeda¿ (%d)!{/b}\n{b}Cena:{/b} $%d\n{b}Interior:{/b} %d",
				houseid,
				houseInfo[houseid][hsPrice],
				houseInfo[houseid][hsInterior]);
			return szString;
		}
		case STATUS_SOLD:
		{
			format(szString, sizeof szString, "{b}Dom gracza %s (%d){/b}\n{b}Etykieta:{/b} %s\n{b}Wartosæ:{/b} $%d\n{b}Drzwi:{/b} %s\nAby wejœæ wciœnij 'F'",
				GetPlayerNameFromSQL(houseInfo[houseid][hsOwner]),
				houseid,
				houseInfo[houseid][hsLabel],
				houseInfo[houseid][hsPrice],
				GetHouseDoorStatus(houseid));
			return szString;
		}
	}
	return szString;
}

stock GetPlayerNameFromSQL(useid)
{
	new szString[64], szName[32];
	format(szString, sizeof szString, "SELECT Name FROM Accounts WHERE ID = '%d'", useid);
	mysql_query(szString);
	mysql_store_result();
	mysql_fetch_string(szName);
	mysql_free_result();
	return szName;
}

stock GetHouseDoorStatus(houseid)
{
	new szString[18];

	switch(houseInfo[houseid][hsDoor])
	{
		case 0: strcat(szString, "zamkniête");
		case 1: strcat(szString, "otwarte");
	}
	return szString;
}

stock LoadHouses()
{
	new data[128], szString[256], count, rows;
	mysql_query("SELECT * FROM houses");
	mysql_store_result(); 
	rows = mysql_num_rows();
	for(new i = 0; i < rows; i++)
	{
		if(mysql_fetch_row(data, "|") && i < MAX_HOUSES)
		{
			sscanf(data, "p<|>ds[48]dddfffddd", 
			houseInfo[i][hsID],
			houseInfo[i][hsLabel],
			houseInfo[i][hsPrice],
			houseInfo[i][hsOwner],
			houseInfo[i][hsVcOwner],
			houseInfo[i][hsPos][0],
			houseInfo[i][hsPos][1],
			houseInfo[i][hsPos][2],
			houseInfo[i][hsInterior],
			houseInfo[i][hsStatus],
			houseInfo[i][hsDoor]);
			
			houseInfo[i][hsCreated] = true;
			houseInfo[i][hsSave] = false;

			houseInfo[i][hsIcon] = CreateDynamicMapIcon(houseInfo[i][hsPos][0], houseInfo[i][hsPos][1], houseInfo[i][hsPos][2], houseInfo[i][hsStatus] ? 32 : 31, 0, -1, -1, -1, 100.0, MAPICON_LOCAL);
			houseInfo[i][hsPickup] = CreateDynamicPickup(houseInfo[i][hsStatus] ? 1272 : 1273, 1, houseInfo[i][hsPos][0], houseInfo[i][hsPos][1], houseInfo[i][hsPos][2]);
			new Float:pos[3], int;
			GetInteriorPos(houseInfo[i][hsInterior], int, pos[0], pos[1], pos[2]);
			houseInfo[i][hsPickupExit] = CreateDynamicPickup(1559, 1, pos[0], pos[1], pos[2], i + 1, int);
			count++;
		}
	}
	printf("[DOMKI] Za³adowano %d domków.", count);
	mysql_free_result();
	for(new i = 0; i < rows; i++)
	{
		format(szString, sizeof szString, "%s", HouseFormat3DLabel(i));
		houseInfo[i][hsLabelID] = CreateDynamic3DTextLabel(clText(COLOR_INFO2, szString), -1, houseInfo[i][hsPos][0], houseInfo[i][hsPos][1], houseInfo[i][hsPos][2], 25.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1);	
	}
	return 1;
}

stock GetInteriorPos(interiorid, &interior, &Float:x, &Float:y, &Float:z)
{
	if(interiorid >= sizeof InteriorPos)
		return 0;
	x = InteriorPos[interiorid][1];
	y = InteriorPos[interiorid][2];
	z = InteriorPos[interiorid][3];
	interior = floatround(InteriorPos[interiorid][0]);
	return 1;
}

CMD:createhouse(playerid, params[])
{
	if(playerInfo[playerid][pAdmin] < 1)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if((GetPlayerVirtualWorld(playerid) > 0) || (GetPlayerInterior(playerid) > 0))
		return Msg(playerid, COLOR_ERROR, "Nie mo¿esz tego teraz zrobiæ.");

	new price, interior, Float:pos[3];
	if(sscanf(params, "dd", price, interior))
		return Msg(playerid, COLOR_ERROR, "Wpisz /createhouse [cena] [interior]");

	if(interior < 0 || interior > 32)
		return Msg(playerid, COLOR_ERROR, "Podaj interior w przedziale 0 - 32");

	if(price < 0 || price > 2000000)
		return Msg(playerid, COLOR_ERROR, "Podaj prawid³ow¹ cene.");

	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	new house = CreateHouse(price, interior, pos[0], pos[1], pos[2]);
	if(house > -1)
		return Msg(playerid, COLOR_INFO, "Pomyœlnie utworzono dom.");
	Msg(playerid, COLOR_ERROR, "Nie uda³o siê utworzyæ domu.");
	return 1;
}

CMD:deletehouse(playerid, params[])
{
	new houseid;
	if(playerInfo[playerid][pAdmin] < 1)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "d", houseid))
		return Msg(playerid, COLOR_ERROR, "Wpisz /deletehouse [id]");

	if(!IsValidHouse(houseid))
		return Msg(playerid, COLOR_ERROR, "Nie odnaleziono domu z tym ID.");

	new result = DeleteHouse(houseid);
	if(!result)
		return Msg(playerid, COLOR_ERROR, "Nie uda³o siê usun¹æ tego domku.");
	Msg(playerid, COLOR_INFO, "Pomyœlnie usuniêto ten dom.");
	return 1;
}

CMD:changeprice(playerid, params[])
{
	new houseid, price;
	if(playerInfo[playerid][pAdmin] < 1)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "dd", houseid, price))
		return Msg(playerid, COLOR_ERROR, "Wpisz /changeprice [id] [cena]");

	if(!IsValidHouse(houseid))
		return Msg(playerid, COLOR_ERROR, "Nie odnaleziono domu z tym ID.");

	if(price < 0 || price > 2000000)
		return Msg(playerid, COLOR_ERROR, "WprowadŸ prawid³ow¹ cenê.");

	houseInfo[houseid][hsPrice] = price;
	houseInfo[houseid][hsSave] = true;
	Msg(playerid, COLOR_INFO, "Pomyœlnie zmieniono cenê.");

	new szString[256], id = houseid;
	format(szString, sizeof szString, "%s", HouseFormat3DLabel(id));
	DestroyDynamic3DTextLabel(houseInfo[id][hsLabelID]);
	houseInfo[id][hsLabelID] = CreateDynamic3DTextLabel(clText(COLOR_INFO2, szString), -1, houseInfo[id][hsPos][0], houseInfo[id][hsPos][1], houseInfo[id][hsPos][2], 25.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1);
	return 1;
}

CMD:changeinterior(playerid, params[])
{
	new houseid, price;
	if(playerInfo[playerid][pAdmin] < 1)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "dd", houseid, price))
		return Msg(playerid, COLOR_ERROR, "Wpisz /changeinterior [id] [interior]");

	if(!IsValidHouse(houseid))
		return Msg(playerid, COLOR_ERROR, "Nie odnaleziono domu z tym ID.");

	if(price < 0 || price >= sizeof InteriorPos)
		return Msg(playerid, COLOR_ERROR, "WprowadŸ prawid³owy interior.");

	houseInfo[houseid][hsInterior] = price;
	houseInfo[houseid][hsSave] = true;
	Msg(playerid, COLOR_INFO, "Pomyœlnie zmieniono interior.");

	new szString[256], id = houseid;
	format(szString, sizeof szString, "%s", HouseFormat3DLabel(id));
	DestroyDynamic3DTextLabel(houseInfo[id][hsLabelID]);
	DestroyDynamicPickup(houseInfo[id][hsPickupExit]);
	houseInfo[id][hsLabelID] = CreateDynamic3DTextLabel(clText(COLOR_INFO2, szString), -1, houseInfo[id][hsPos][0], houseInfo[id][hsPos][1], houseInfo[id][hsPos][2], 25.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1);
	
	new Float:pos[3], int;
	GetInteriorPos(houseInfo[id][hsInterior], int, pos[0], pos[1], pos[2]);
	houseInfo[id][hsPickupExit] = CreateDynamicPickup(1559, 1, pos[0], pos[1], pos[2], id + 1, int);
	return 1;
}

CMD:buyhouse(playerid)
{
	if((GetPlayerVirtualWorld(playerid) > 0) || (GetPlayerInterior(playerid) > 0))
		return Msg(playerid, COLOR_ERROR, "Nie mo¿esz tego teraz zrobiæ.");

	for(new i = 0; i < MAX_HOUSES; i++)
	{
		if(!IsValidHouse(i))
			continue;

		if(IsPlayerInRangeOfPoint(playerid, 4.0, houseInfo[i][hsPos][0], houseInfo[i][hsPos][1], houseInfo[i][hsPos][2]))
		{
			if(houseInfo[i][hsStatus] != STATUS_FREE)
				return Msg(playerid, COLOR_ERROR, "Ten dom nie jest na sprzeda¿.");
			
			if(GetMoney(playerid) < houseInfo[i][hsPrice])
				return Msg(playerid, COLOR_ERROR, "Nie masz wystarczaj¹co du¿o gotówki.");

			GiveMoney(playerid, -houseInfo[i][hsPrice]);
			new result = BuyHouse(playerid, i);
			if(!result)
				return Msg(playerid, COLOR_ERROR, "Nie uda³o zakupiæ siê domku.");
			Msg(playerid, COLOR_INFO, "Pomyœlnie zakupiono dom.");
			return 1;
		}
	}
	Msg(playerid, COLOR_ERROR, "Nie jesteœ w pobli¿u ¿adnej posiad³oœci.");
	return 1;
}

CMD:dom(playerid, params[])
{
	if(GetPVarInt(playerid, "InHouse") != 0)
	{
		new houseid = GetPVarInt(playerid, "InHouse") - 1;
		if(playerInfo[playerid][pID] == houseInfo[houseid][hsOwner])
		{
			new string[256];
			strcat(string, "{b}1) Informacje{/b}\n");
			strcat(string, " × Ogólne\n");
			strcat(string, "{b}2) Czynnoœci{/b}\n");
			strcat(string, " × Zabierz kanapke\n");
			strcat(string, " × Zabierz piwo\n");
			strcat(string, "{b}3) Zarz¹dzanie{/b}\n");
			strcat(string, " × Zmieñ etykietê\n");
			strcat(string, " × Zmieñ wspó³lokatora\n");
			strcat(string, " × Zmieñ zamek\n");
			strcat(string, " × Sprzedaj dom graczowi\n");
			strcat(string, " × Sprzedaj dom serwerowi");
			format(string, sizeof string, "%s\n × %s drzwi", string, (houseInfo[houseid][hsDoor] == DOOR_CLOSE) ? ("Otwórz") : ("Zamknij"));
			Dialog_Show(playerid, DIALOG_HOUSE_MAIN, DIALOG_STYLE_LIST, "Dom", clText(COLOR_INFO2, string), "Wybierz", "WyjdŸ");
		}
		else
		{
			new string[256];
			strcat(string, "{b}1) Informacje{/b}\n");
			strcat(string, " × Ogólne\n");
			strcat(string, "{b}2) Czynnoœci{/b}\n");
			strcat(string, " × Zabierz kanapke\n");
			strcat(string, " × Zabierz piwo\n");
			Dialog_Show(playerid, DIALOG_HOUSE_MAIN_FAKE, DIALOG_STYLE_LIST, "Dom", clText(COLOR_INFO2, string), "Wybierz", "WyjdŸ");
		}
	}
	else
		return Msg(playerid, COLOR_ERROR, "Nie jesteœ w ¿adnym lokalu.");
	
	return 1;
}

Dialog:DIALOG_HOUSE_MAIN(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new szString[412], id = GetPVarInt(playerid, "InHouse")-1;
	switch(listitem)
	{
		case 1: // info ogólne
		{
			format(szString, sizeof szString, "{b}ID Domu:{/b}\t%d\n{b}Lokator:{/b}\t%s\n{b}Wspó³lokator:{/b}\t%s\n{b}Wartoœæ:{/b}\t$%d\n{b}Interior:{/b}\t%d\n{b}Drzwi:{/b}\t%s\n{b}Lokalizacja:{/b}\t%s",
				houseInfo[id][hsID],
				GetPlayerNameFromSQL(houseInfo[id][hsOwner]),
				GetPlayerNameFromSQL(houseInfo[id][hsVcOwner]),
				houseInfo[id][hsPrice],
				houseInfo[id][hsInterior],
				(houseInfo[id][hsDoor]==DOOR_CLOSE) ? ("zamkniête") : ("otwarte"),
				GetPointZone(houseInfo[id][hsPos][0], houseInfo[id][hsPos][1]));
			Dialog_Show(playerid, DIALOG_HOUSE_MAIN_INFO, DIALOG_STYLE_TABLIST, "Dom > Informacje ogólne", clText(COLOR_INFO2, szString), "Wstecz", #);
		}
		case 3: // kanapka
		{
			new Float:health;
			GetPlayerHealth(playerid, health);
			health += float(5+random(15)); if(health > 100.0) health = 100.0;
			playerInfo[playerid][pHunger] += float(5+random(10)); if(playerInfo[playerid][pHunger] > 100.0) playerInfo[playerid][pHunger] = 100.0;
			SetPlayerHealth(playerid, health);
			Msg(playerid, COLOR_INFO2, "Z³ad³eœ kanapkê.");
			PreloadAnimLib(playerid, "FOOD");
			ApplyAnimation(playerid, "FOOD", "EAT_Burger", 4.1, 0, 1, 1, 0, 0);
			SetPlayerProgressBarValue(playerid, hudInfo[tdHungerProgress][playerid], playerInfo[playerid][pHunger]);
			return cmd_dom(playerid, "");
		}
		case 4: // alko
		{
			SetPlayerSpecialAction(playerid, 20);
			Msg(playerid, COLOR_INFO2, "Zabra³eœ piwo.");
			return cmd_dom(playerid, "");
		}
		case 6: // etykieta
		{
			Dialog_Show(playerid, DIALOG_HOUSE_MAIN_LABEL, DIALOG_STYLE_INPUT, "Dom > Etykieta", clText(COLOR_INFO2, "{FFFFFF}WprowadŸ poni¿ej tekst swojej etykiety.\n{b}Jego treœæ nie mo¿e przekraczaæ 48 znaków.{/b}"), "Wybierz", "Wstecz");
		}
		case 7: // wspó³lok
		{
			new Float:pos[3], count;
			GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
			foreach (new i : Player)
			{
				if(IsPlayerInRangeOfPoint(i, 10.0, pos[0], pos[1], pos[2]) && i != playerid)
				{
					count++;
					format(szString, sizeof szString, "%s%d\t%s\n", szString, i, PlayerName(i));
				}
			}
			if(count <= 0)
				format(szString, sizeof szString, "Nikogo nie ma w Twoim otoczeniu.");
			else
				strins(szString, "ID Gracza\tNick\n", 0);
			Dialog_Show(playerid, DIALOG_HOUSE_MAIN_ADD, count <= 0 ? DIALOG_STYLE_LIST : DIALOG_STYLE_TABLIST_HEADERS, "Dom > Wspó³lokator", szString, "Wybierz", "Wstecz");
		}
		case 8: // zamek
		{
			Dialog_Show(playerid, DIALOG_HOUSE_MAIN_REMOVE, DIALOG_STYLE_MSGBOX, "Dom > Zmiana zamków", clText(COLOR_INFO2, "{FFFFFF}Czy jesteœ pewien, ¿e chcesz zmieniæ zamki?\nSpowoduje to, ¿e twój wspó³lokator nie bêdzie móg³ wejœæ do domu.{/b}"), "Wybierz", "Wstecz");
		}
		case 9: // sprzedaj serwer
		{
			Msg(playerid, COLOR_ERROR, "Opcja tymczasowo niedostêpna.");
			return cmd_dom(playerid, "");
		}
		case 10: // sprzedaj graczo
		{
			Msg(playerid, COLOR_ERROR, "Opcja tymczasowo niedostêpna.");
			return cmd_dom(playerid, "");
		}
		case 11: // drzwi
		{
			switch(houseInfo[id][hsDoor])
			{
				case DOOR_CLOSE:
				{
					houseInfo[id][hsDoor] = DOOR_OPEN;
					Msg(playerid, COLOR_INFO2, "Drzwi zosta³y {b}otwarte{/b}.");
				}	
				case DOOR_OPEN:
				{
					houseInfo[id][hsDoor] = DOOR_CLOSE;
					Msg(playerid, COLOR_INFO2, "Drzwi zosta³y {b}zamkniête{/b}.");
				}
			}
			houseInfo[id][hsSave] = true;
			format(szString, sizeof szString, "%s", HouseFormat3DLabel(id));
			DestroyDynamic3DTextLabel(houseInfo[id][hsLabelID]);
			houseInfo[id][hsLabelID] = CreateDynamic3DTextLabel(clText(COLOR_INFO2, szString), -1, houseInfo[id][hsPos][0], houseInfo[id][hsPos][1], houseInfo[id][hsPos][2], 25.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1);
			return cmd_dom(playerid, "");
		}
		default: return cmd_dom(playerid, "");
	}
	return 1;
}

Dialog:DIALOG_HOUSE_MAIN_INFO(playerid, response, listitem, inputtext[])
{
	cmd_dom(playerid, "");
	return 1;
}

Dialog:DIALOG_HOUSE_MAIN_LABEL(playerid, response, listitem, inputtext[])
{
	if(!response)
		return cmd_dom(playerid, "");

	if(strlen(inputtext) > 48)
		return Dialog_Show(playerid, DIALOG_HOUSE_MAIN_LABEL, DIALOG_STYLE_INPUT, "Dom > Etykieta", clText(COLOR_ERROR, "{b}Wprowadzona treœæ jest za d³uga.{/b}\n \nWprowadŸ poni¿ej tekst swojej etykiety.\n{b}Jego treœæ nie mo¿e przekraczaæ 48 znaków.{/b}"), "Wybierz", "Wstecz");
	
	new id = GetPVarInt(playerid, "InHouse") - 1;
	format(houseInfo[id][hsLabel], 48, "%s", inputtext);
	Msg(playerid, COLOR_INFO2, "Pomyœlnie zmieniono etykietê.");

	new szString[256];
	format(szString, sizeof szString, "%s", HouseFormat3DLabel(id));
	DestroyDynamic3DTextLabel(houseInfo[id][hsLabelID]);
	houseInfo[id][hsLabelID] = CreateDynamic3DTextLabel(clText(COLOR_INFO2, szString), -1, houseInfo[id][hsPos][0], houseInfo[id][hsPos][1], houseInfo[id][hsPos][2], 25.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1);	
	cmd_dom(playerid, "");
	houseInfo[id][hsSave] = true;
	return 1;
}

Dialog:DIALOG_HOUSE_MAIN_REMOVE(playerid, response, listitem, inputtext[])
{
	if(!response)
		return cmd_dom(playerid, "");

	new id = GetPVarInt(playerid, "InHouse") - 1;
	houseInfo[id][hsVcOwner] = 0;
	Msg(playerid, COLOR_INFO2, "Pomyœlnie zmieniono zamki.");
	cmd_dom(playerid, "");
	houseInfo[id][hsSave] = true;
	return 1;
}

Dialog:DIALOG_HOUSE_MAIN_ADD(playerid, response, listitem, inputtext[])
{
	if(!response)
		return cmd_dom(playerid, "");

	if(strfind(inputtext, "otoczeniu") != -1)
		return DIALOG_HOUSE_MAIN(playerid, true, 7, "");

	new id, house, szString[64];
	sscanf(inputtext, "d{s[32]}", id);
	format(szString, sizeof szString, "Da³eœ klucze graczowi {b}%s{/b}.", PlayerName(id));
	Msg(playerid, COLOR_INFO2, szString);

	Msg(id, COLOR_INFO2, "W³aœciciel domu da³ Ci klucze.");
	house = GetPVarInt(playerid, "InHouse") - 1;
	houseInfo[house][hsVcOwner] = playerInfo[id][pID];
	cmd_dom(playerid, "");
	houseInfo[house][hsSave] = true;
	return 1;
}

Dialog:DIALOG_HOUSE_MAIN_FAKE(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	switch(listitem)
	{
		case 1: DIALOG_HOUSE_MAIN(playerid, true, 1, "");
		case 3: DIALOG_HOUSE_MAIN(playerid, true, 3, "");
		case 4: DIALOG_HOUSE_MAIN(playerid, true, 4, "");
		default: return cmd_dom(playerid, "");
	}
	return 1;
}

stock SaveHouses()
{
	new szString[300];
	for(new i = 0; i < MAX_HOUSES; i++)
	{
		if(!houseInfo[i][hsSave] || !IsValidHouse(i))
			continue;

		format(szString, sizeof szString, "UPDATE houses SET label = '%s', price = '%d', owner = '%d', vcowner = '%d', interior = '%d', status = '%d', door = '%d' WHERE ID = '%d'",
			houseInfo[i][hsLabel],
			houseInfo[i][hsPrice],
			houseInfo[i][hsOwner],
			houseInfo[i][hsVcOwner],
			houseInfo[i][hsInterior],
			houseInfo[i][hsStatus],
			houseInfo[i][hsDoor],
			houseInfo[i][hsID]);
		mysql_query(szString);
		houseInfo[i][hsSave] = false;
	}
	return 1;
}
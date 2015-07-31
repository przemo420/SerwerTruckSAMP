CMD:tankuj(playerid, params[])
{
	if(!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie znajdujesz siê w pojeŸdzie.");

	new stacjaid = GetStacja(playerid);
	if(!stacjaid)
		return Msg(playerid, COLOR_ERROR, "Nie znajdujesz siê przy stacji.");

	new string[180], vehicleid=GetPlayerVehicleID(playerid), Float:fuel, fueltype[10];

	if(Spawned[vehicle])
	{

	}

	format(string, sizeof(string), "Zbiornik gazu:\t%dL\nZbiornik ropy:\t%dL\nZbiornik benzyny:\t%dL\n\nCena gazu:\t$%.02f\nCena ropy:\t$%0.2f\nCena benzyny:\t$%0.2f\n\nRodzaj paliwa pojazdu:\t%s", 
			Stacje[stacjaid][ZbiornikGazu], Stacje[stacjaid][ZbiornikRopy], Stacje[stacjaid][ZbiornikBenzyny], Stacje[stacjaid][CenaGazu], Stacje[stacjaid][CenaRopy], Stacje[stacjaid][CenaBenzyny], );
/*if(PlayerVehicleIsDiesel(playerid))
{
FuelTYP[playerid] = 1;
format(string, sizeof string, "{a9c4e4}Rodzaj paliwa tego pojazdu to: {DFDF00}DIESEL\n \n{a9c4e4}Mo¿esz zatankowaæ: {DFDF00}%d L\n{a9c4e4}Czy chcesz rozpocz¹æ tankowanie?", MaxFuel(GetVehicleModel(GetPlayerVehicleID(playerid))) - vFuel[GetPlayerVehicleID(playerid)]);
Dialog_Show(playerid, DIALOG_TANKOWANIE, DIALOG_STYLE_MSGBOX, " ", string, "Tak", "Nie");
}
else
{
FuelTYP[playerid] = 0;
format(string, sizeof string, "{a9c4e4}Rodzaj paliwa tego pojazdu to: {DFDF00}BENZYNA\n \n{a9c4e4}Mo¿esz zatankowaæ: {DFDF00}%d L\n{a9c4e4}Czy chcesz rozpocz¹æ tankowanie?", MaxFuel(GetVehicleModel(GetPlayerVehicleID(playerid))) - vFuel[GetPlayerVehicleID(playerid)]);
Dialog_Show(playerid, DIALOG_TANKOWANIE, DIALOG_STYLE_MSGBOX, " ", string, "Tak", "Nie");
}*/
	ShowInfo(playerid, "Stacja chwilowo nieczynna.");
	return 1;
}

LadujStacje()
{
	mysql_query("SELECT * FROM `Stacje`");
	mysql_store_result();

	if(mysql_num_rows()==0)
		return print("[STACJE] Brak stacji do zaladowania.");

	new string[140];
	while(mysql_fetch_row(string, "|")) 
	{
    	sscanf(string, "e<p<|>dfffddd>", Stacje[ID_STACJA]);

    	format(string, sizeof string, "{808040}STACJA BENZYNOWA {808040}[VID: {FFFFFF}%d{808040}]\n{FFFFFF}/tankuj", ID_STACJA);
    	StacjeText3D[ID_STACJA] = CreateDynamic3DTextLabel(string, ZIELONY2, Stacje[ID_STACJA][sPosX], Stacje[ID_STACJA][sPosY], Stacje[ID_STACJA][sPosZ]+0.5, 30.0);

    	for(new playerid; playerid < MAX_PLAYERS; playerid++)
			StacjeMapIcon[ID_STACJA][playerid] = CreateDynamicMapIcon(Stacje[ID_STACJA][sPosX], Stacje[ID_STACJA][sPosY], Stacje[ID_STACJA][sPosZ], 55, 0, _, _, playerid, 300.0);

		ID_STACJA++;
	} 

	printf("[STACJE] Zaladowano %d stacji.", ID_STACJA);
	mysql_free_result();
	return 1;
}

Dialog:DIALOG_TANKOWANIE(playerid, response, listitem, inputtext[])
{
	return 1;
}
enum eTablicaOgloszen
{
	UID,
	Float:PostionX,
	Float:PostionY,
	Float:PostionZ
};

new Float:TablicaOgloszen[][eTablicaOgloszen] = {
	{0, 1623.4889,-1045.8835,23.8984},
	{1, 1396.1667,1007.4241,10.9297},
	{2, -242.1355,-236.4762,2.4297},
	{3, -2137.5225,-135.9968,35.3203}
};

forward TablicaOgloszen_OnGameModeInit();
public TablicaOgloszen_OnGameModeInit()
{
	Loop(i, sizeof TablicaOgloszen)
	{
		CreateDynamicObject(2737, TablicaOgloszen[i][PostionX], TablicaOgloszen[i][PostionY], TablicaOgloszen[i][PostionZ], 0.0, 0.0, 0.0, -1, -1, -1, 100);
   		CreateDynamic3DTextLabel("/tablica", 0x0, TablicaOgloszen[i][PostionX], TablicaOgloszen[i][PostionY], TablicaOgloszen[i][PostionZ] + 1, 7.0);
   		CreateDynamicMapIcon(TablicaOgloszen[i][PostionX], TablicaOgloszen[i][PostionY], TablicaOgloszen[i][PostionZ], 53, LIGHTGREEN, _, _, _, 800.0, MAPICON_LOCAL);
	}
}

CMD:tablica(playerid, params[])
{
	Loop(i, sizeof TablicaOgloszen)
		if(GetDistanceFromMeToPoint(playerid, TablicaOgloszen[i][PostionX], TablicaOgloszen[i][PostionY], TablicaOgloszen[i][PostionZ]) <= 10)
		{
			new wiadomosc[600], zapytanie[70];

			mysql_query("SELECT `owner`, `text`, `time` FROM `st_tablica_ogloszen`");
			mysql_store_result();

			if(mysql_num_rows()==0)
				strcat(wiadomosc, "Brak og�osze�.");
			else
			{
				strcat(wiadomosc, "Gracz\tWiadomo��\tCzas\n");
				while(mysql_fetch_row(zapytanie, "|")) 
				{
			    	mysql_fetch_field("owner", zapytanie);
			    	strcat(wiadomosc, zapytanie);
			    	strcat(wiadomosc, "\t");

			    	mysql_fetch_field("text", zapytanie);
			    	strcat(wiadomosc, zapytanie);
			    	strcat(wiadomosc, "\t");

			    	mysql_fetch_field("time", zapytanie);
			    	strcat(wiadomosc, zapytanie);
			    	strcat(wiadomosc, "\n");
				} 
			}

			ShowPlayerDialog(playerid, NEVER_DIALOG, DIALOG_STYLE_TABLIST_HEADERS, "Og�oszenia", wiadomosc, "Zamknij", #);
			break;
		}

	return 1;
}
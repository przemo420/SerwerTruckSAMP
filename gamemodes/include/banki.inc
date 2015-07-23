enum eBanky
{
	UID,
	Float:PostionX,
	Float:PostionY,
	Float:PostionZ
};

new Float:Banky[][eBanky] = {
	{0, 1047.9852,827.3018,11.1205},
	{1, 1063.0681,1869.1995,10.8203},
	{2, -448.5209,636.0540,18.0008},
	{3, 962.6727,655.3333,11.7153},
	{4, 1828.0359,765.0137,10.8330},
	{5, 2122.7534,897.6885,11.1797},
	{6, 2799.2695,1348.8024,10.8984},
	{7, 2631.8250,1128.5344,11.1797},
	{8, 1580.7454,68.4214,37.9205},
	{9, 999.3474,-917.7577,42.3281},
	{10, 1929.3793,-1779.4159,13.5469},
	{11, 5124.6890,-1984.0631,13.9192},
	{12, 59.6640,1212.9369,18.8413},
	{13, 973.5370,1358.7407,13.5659},
	{14, -228.1691,77.9150,3.1920},
	{15, -93.5920,-1200.6500,2.8906},
	{16, -1158.3931,-1043.2175,129.2406},
	{17, 4966.7090,3206.1633,21.2170},
	{18, -2406.4678,1854.5674,5.5670},
	{19, -1316.9796,2699.6750,50.2663},
	{20, -2553.4932,647.5170,27.8125},
	{21, -1706.4426,403.1194,7.1797}
};

forward Banki_OnGameModeInit();
public Banki_OnGameModeInit()
{
	Loop(i, sizeof Banky)
	{
		CreateDynamicObject(2754, Banky[i][PostionX], Banky[i][PostionY], Banky[i][PostionZ], 0.0, 0.0, 0.0, -1, -1, -1, 100);
   		CreateDynamic3DTextLabel("/bankomat", LIGHTGREEN, Banky[i][PostionX], Banky[i][PostionY], Banky[i][PostionZ], 20.0);
   		CreateDynamicMapIcon(Banky[i][PostionX], Banky[i][PostionY], Banky[i][PostionZ], 52, LIGHTGREEN, _, _, _, 800.0, MAPICON_LOCAL);
	}

	return 1;
}

CMD:bankomat(playerid, params[])
{
	new bool:find=false;

	Loop(i, sizeof Banky)
		if(GetDistanceFromMeToPoint(playerid, Banky[i][PostionX], Banky[i][PostionY], Banky[i][PostionZ]) <= 20)
		{
			if(PlayerInfo[playerid][pBankomat] == (-1))
				return Dialog_Show(playerid, DIALOG_BANK_ZALOZ, DIALOG_STYLE_MSGBOX, " ", "{FFB90F}Nie posiadasz konta bankowego, czy chcesz je za³o¿yæ za kwotê{FFFFFF}5000$?", "Tak", "Nie");

			new x[200];
			new string[120];
			strcat(x, "{FFFFFF}Serwer-Truck SAMP - bankomaty.\n");
			strcat(x, " \n");
			strcat(x, "{FFB90F}- wp³aæ pieni¹dze na konto\n");
			strcat(x, "{FFB90F}- wyp³aæ pieni¹dze z konta\n");
			strcat(x, " \n");
			format(string, sizeof string, "{FFB90F}Stan konta: {FFFFFF}%d$\n", PlayerInfo[playerid][pBankomat]);
			strcat(x, string);
			strcat(x, " \n");
			strcat(x,"{FFFFFF}www.serwertruck.eu");

			Dialog_Show(playerid, DIALOG_BANK, DIALOG_STYLE_LIST, " ", x, "Wybierz", "Wyjd?");
			find=true;
			break;
		}

	if(!find)
		ShowInfo(playerid, "Nie znajdujesz siê obok bankomatu.");

	return 1;
}

Dialog:DIALOG_BANK_ZALOZ(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	if(GetMoney(playerid) < 5000)
		return ShowInfo(playerid, "Nie masz wystarczaj¹co pieniêdzy.");

	GiveMoney(playerid, -5000);
	PlayerInfo[playerid][pBankomat] = 0;

	return ShowInfo(playerid, "Za³o¿y³eœ konto bankowe.\nZ Twojego portfela zosta³o zabrane $5000.");
}

Dialog:DIALOG_BANK(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	switch(listitem)
	{
		case 2:
			Dialog_Show(playerid, DIALOG_BANK_WPLAC, DIALOG_STYLE_INPUT, " ", "Wpisz kwotê, któr¹ chcesz wp³aciæ na konto:", "Dalej", "WyjdŸ");

		case 3:
			Dialog_Show(playerid, DIALOG_BANK_WYPLAC, DIALOG_STYLE_INPUT, " ", "Wpisz kwotê, któr¹ chcesz wyp³aciæ z konta:", "Dalej", "WyjdŸ");

		default: cmd_bankomat(playerid, "");
	}

	return 1;
}

Dialog:DIALOG_BANK_WPLAC(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new string[70];
	new pPieniadze = strval(inputtext);

	if(pPieniadze <= 0)
		return ShowInfo(playerid, "Zbyt ma³a kwota.");
		
	/*switch(GetPVarInt(playerid, "Banklisitem"))
	{
			case 1:
			{
			if(GetMoney(playerid) < pPieniadze)
				return ShowInfo(playerid, "Nie posiadasz tylu pieniêdzy.");	
			
			GiveMoney(playerid, -pPieniadze);
			PlayerInfo[playerid][pBankomat] += pPieniadze;
			format(string, sizeof(string), "Wp?aci?e? {FFB90F}$%d {FFFFFF}na konto bankowe.", pPieniadze);
			ShowInfo(playerid, string);
			}
			
			case 2:
		{
			if(pPieniadze > PlayerInfo[playerid][pBankomat])
				return ShowInfo(playerid, "Konto nie posiada takiego zasobu got?ki.");
			GiveMoney(playerid, pPieniadze);
			PlayerInfo[playerid][pBankomat] -= pPieniadze;
			format(string, sizeof string, "Wyp?aci?e? {FFB90F}$%d {FFFFFF}z konta bankowego.", pPieniadze);
			ShowInfo(playerid, string);
		}
	}*/
}

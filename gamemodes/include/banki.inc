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
   		CreateDynamic3DTextLabel("/bankomat", 0x0, Banky[i][PostionX], Banky[i][PostionY], Banky[i][PostionZ] + 1, 7.0);
   		CreateDynamicMapIcon(Banky[i][PostionX], Banky[i][PostionY], Banky[i][PostionZ], 52, LIGHTGREEN, _, _, _, 800.0, MAPICON_LOCAL);
	}

	return 1;
}

CMD:bankomat(playerid, params[])
{
	Loop(i, sizeof Banky)
		if(GetDistanceFromMeToPoint(playerid, Banky[i][PostionX], Banky[i][PostionY], Banky[i][PostionZ]) <= 10)
		{
			if(PlayerInfo[playerid][pBankomat] == (-1))
				return ShowPlayerDialog(playerid, DIALOG_BANK_CONNECT, DIALOG_STYLE_MSGBOX, " ", "{FFB90F}Nie posiadasz konta bankowego, czy chcesz je za³o¿yæ za kwotê{FFFFFF}5000$?", "Tak", "Nie");

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

			ShowPlayerDialog(playerid, DIALOG_BANK_CONNECT1, DIALOG_STYLE_LIST, " ", x, "Wybierz", "Wyjd?");
			DeletePVar(playerid, "Banklisitem");
			break;
		}

	return 1;
}
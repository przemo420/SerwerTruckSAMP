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
	new szString[158];
	Loop(i, sizeof Banky)
	{
		format(szString, sizeof szString, "Bankomat w {b}%s{/b} (ID: {b}%d{/b}).\nAby u¿yæ wpisz {b}/bankomat{/b}.", GetPointZone(Banky[i][PostionX], Banky[i][PostionY]), i);
		CreateDynamicObject(2754, Banky[i][PostionX], Banky[i][PostionY], Banky[i][PostionZ], 0.0, 0.0, 0.0, -1, -1, -1, 100);
   		CreateDynamic3DTextLabel(clText(COLOR_INFO2, szString), -1, Banky[i][PostionX], Banky[i][PostionY], Banky[i][PostionZ], 20.0);
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
			if(playerInfo[playerid][pBankomat] == (-1))
				return Dialog_Show(playerid, DIALOG_BANK_ZALOZ, DIALOG_STYLE_MSGBOX, "Bankomat > Za³ó¿", clText(COLOR_INFO2, "{FFFFFF}Nie posiadasz konta bankowego, czy chcesz je za³o¿yæ za kwotê {b}3000${/b}?"), "Tak", "Nie");

			new szString[128];
			UpdateAccountBalance(playerid);
			format(szString, sizeof szString, "{FFFFFF}Stan konta {b}%d{/b}$\n \nWp³aæ pieni¹dze\nWyp³aæ pieni¹dze\nPrzelej pieni¹dze", playerInfo[playerid][pBankomat]);
			Dialog_Show(playerid, DIALOG_BANK, DIALOG_STYLE_LIST, "Bankomat", clText(COLOR_INFO, szString), "Wybierz", "WyjdŸ");
			find = true;
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

	if(GetMoney(playerid) < 3000)
		return Msg(playerid, COLOR_ERROR, "Nie masz wystarczaj¹co pieniêdzy.");

	GiveMoney(playerid, -3000);
	playerInfo[playerid][pBankomat] = 0;

	SaveAccountBalance(playerid);
	Msg(playerid, COLOR_INFO, "Za³o¿y³eœ konto bankowe. Z Twojego portfela zosta³o zabrane {b}$3000{/b}.");
	cmd_bankomat(playerid, "");
	return 1;
}

Dialog:DIALOG_BANK(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	switch(listitem)
	{
		case 2:
			Dialog_Show(playerid, DIALOG_BANK_WPLAC, DIALOG_STYLE_INPUT, "Bankomat > Wp³aæ", "{FFFFFF}Wpisz kwotê, któr¹ chcesz wp³aciæ na konto:", "Dalej", "Wstecz");

		case 3:
			Dialog_Show(playerid, DIALOG_BANK_WYPLAC, DIALOG_STYLE_INPUT, "Bankomat > Wyp³aæ", "{FFFFFF}Wpisz kwotê, któr¹ chcesz wyp³aciæ z konta:", "Dalej", "Wstecz");

		case 4:
			Dialog_Show(playerid, DIALOG_BANK_PRZELEJ, DIALOG_STYLE_INPUT, "Bankomat > Przelej", clText(COLOR_INFO, "{FFFFFF}Wpisz kwotê, któr¹ chcesz przelaæ, oraz nick odbiorcy.\nPrzyk³adowo {b}3000|Kozak59{/b}."), "Dalej", "Wstecz");

		default: cmd_bankomat(playerid, "");
	}

	return 1;
}

Dialog:DIALOG_BANK_WPLAC(playerid, response, listitem, inputtext[])
{
	if(!response)
		return cmd_bankomat(playerid, "");

	new pPieniadze = strval(inputtext);

	if(pPieniadze <= 0)
		return Dialog_Show(playerid, DIALOG_BANK_WPLAC, DIALOG_STYLE_INPUT, "Bankomat > Wp³aæ", clText(COLOR_ERROR, "{b}Poda³eœ zbyt ma³¹ kwotê.{/b}\nWpisz kwotê, któr¹ chcesz wp³aciæ na konto:"), "Dalej", "Wstecz");
		
	if(GetMoney(playerid) < pPieniadze)
		return Dialog_Show(playerid, DIALOG_BANK_WPLAC, DIALOG_STYLE_INPUT, "Bankomat > Wp³aæ", clText(COLOR_ERROR, "{b}Nie posiadasz takiej sumy pieniêdzy.{/b}\nWpisz kwotê, któr¹ chcesz wp³aciæ na konto:"), "Dalej", "Wstecz");

	GiveMoney(playerid, -pPieniadze);
	playerInfo[playerid][pBankomat] += pPieniadze;
	new szString[128];
	format(szString, sizeof szString, "Wp³aci³eœ {b}$%d{/b} na konto bankowe.\nWpisz kwotê, któr¹ chcesz wp³aciæ na konto:", pPieniadze);
	SaveAccountBalance(playerid);
	return Dialog_Show(playerid, DIALOG_BANK_PLAC, DIALOG_STYLE_INPUT, "Bankomat > Wp³aæ", clText(COLOR_INFO, szString), "Dalej", "WyjdŸ");
}

Dialog:DIALOG_BANK_WYPLAC(playerid, response, listitem, inputtext[])
{
	if(!response)
		return cmd_bankomat(playerid, "");

	new pPieniadze = strval(inputtext);

	if(pPieniadze <= 0)
		return Dialog_Show(playerid, DIALOG_BANK_WYPLAC, DIALOG_STYLE_INPUT, "Bankomat > Wyp³aæ", clText(COLOR_ERROR, "{b}Poda³eœ zbyt ma³¹ kwotê.{/b}\nWpisz kwotê, któr¹ chcesz wyp³aciæ z konta:"), "Dalej", "Wstecz");

	if(pPieniadze > playerInfo[playerid][pBankomat])
		return Dialog_Show(playerid, DIALOG_BANK_WYPLAC, DIALOG_STYLE_INPUT, "Bankomat > Wyp³aæ", clText(COLOR_ERROR, "{b}Nie posiadasz takiej sumy pieniêdzy na koncie.{/b}\nWpisz kwotê, któr¹ chcesz wyp³aciæ z konta:"), "Dalej", "Wstecz");


	GiveMoney(playerid, pPieniadze);
	playerInfo[playerid][pBankomat] -= pPieniadze;
	new szString[128];
	format(szString, sizeof szString, "Wyp³aci³eœ {b}$%d{/b} z konta bankowego.\nWpisz kwotê, któr¹ chcesz wyp³aciæ z konta:", pPieniadze);
	SaveAccountBalance(playerid);
	return Dialog_Show(playerid, DIALOG_BANK_WYPLAC, DIALOG_STYLE_INPUT, "Bankomat > Wyp³aæ", clText(COLOR_INFO, szString), "Dalej", "WyjdŸ");
}

Dialog:DIALOG_BANK_PRZELEJ(playerid, response, listitem, inputtext[])
{
	if(!response)
		return cmd_bankomat(playerid, "");

	new money, user[32], balance, szString[180];
	if(sscanf(inputtext, "p<|>ds[32]", money, user))
		return Dialog_Show(playerid, DIALOG_BANK_PRZELEJ, DIALOG_STYLE_INPUT, "Bankomat > Przelej", clText(COLOR_ERROR, "{b}Nie mo¿esz zostawiæ tego pola pustego.{/b}\nWpisz kwotê, któr¹ chcesz przelaæ, oraz nick odbiorcy.\nPrzyk³adowo {b}3000|Kozak59{/b}."), "Dalej", "Wstecz");

	if(money <= 0)
		return Dialog_Show(playerid, DIALOG_BANK_PRZELEJ, DIALOG_STYLE_INPUT, "Bankomat > Przelej", clText(COLOR_ERROR, "{b}Poda³eœ zbyt ma³¹ kwotê.{/b}\nWpisz kwotê, któr¹ chcesz przelaæ, oraz nick odbiorcy.\nPrzyk³adowo {b}3000|Kozak59{/b}."), "Dalej", "Wstecz");

	if(money > playerInfo[playerid][pBankomat])
		return Dialog_Show(playerid, DIALOG_BANK_PRZELEJ, DIALOG_STYLE_INPUT, "Bankomat > Przelej", clText(COLOR_ERROR, "{b}Nie posiadasz takiej sumy pieniêdzy na koncie.{/b}\nWpisz kwotê, któr¹ chcesz przelaæ, oraz nick odbiorcy.\nPrzyk³adowo {b}3000|Kozak59{/b}."), "Dalej", "Wstecz");

	if(!strcmp(PlayerName(playerid), user))
		return Dialog_Show(playerid, DIALOG_BANK_PRZELEJ, DIALOG_STYLE_INPUT, "Bankomat > Przelej", clText(COLOR_ERROR, "{b}Nie mo¿esz przelaæ pieniêdzy samemu sobie.{/b}\nWpisz kwotê, któr¹ chcesz przelaæ, oraz nick odbiorcy.\nPrzyk³adowo {b}3000|Kozak59{/b}."), "Dalej", "Wstecz");

	if(!IsAccountExists(user))
		return Dialog_Show(playerid, DIALOG_BANK_PRZELEJ, DIALOG_STYLE_INPUT, "Bankomat > Przelej", clText(COLOR_ERROR, "{b}Nie odnaleziono takiego u¿ytkownika w bazie danych.{/b}\nWpisz kwotê, któr¹ chcesz przelaæ, oraz nick odbiorcy.\nPrzyk³adowo {b}3000|Kozak59{/b}."), "Dalej", "Wstecz");

	balance = GetAccountBalance(user);
	if(balance < 0)
		return Dialog_Show(playerid, DIALOG_BANK_PRZELEJ, DIALOG_STYLE_INPUT, "Bankomat > Przelej", clText(COLOR_ERROR, "{b}Wybrany gracz nie posiada konta bankowego.{/b}\nWpisz kwotê, któr¹ chcesz przelaæ, oraz nick odbiorcy.\nPrzyk³adowo {b}3000|Kozak59{/b}."), "Dalej", "Wstecz");

	format(szString, sizeof szString, "UPDATE Accounts SET pBankomat = 'pBankomat' + '%d' WHERE Name = '%s'", money, user);
	mysql_query(szString);

	format(szString, sizeof szString, "{b}Pomyœlnie przelano $%d graczowi %s{/b}\nWpisz kwotê, któr¹ chcesz przelaæ, oraz nick odbiorcy.\nPrzyk³adowo {b}3000|Kozak59{/b}.", money, user);
	Dialog_Show(playerid, DIALOG_BANK_PRZELEJ, DIALOG_STYLE_INPUT, "Bankomat > Przelej", clText(COLOR_INFO, szString), "Dalej", "Wstecz");
	playerInfo[playerid][pBankomat] -= money;
	SaveAccountBalance(playerid);
	return 1;
}

stock GetAccountBalance(name[])
{
	new string[84], result;
	mysql_real_escape_string(name, string);
	format(string, sizeof(string), "SELECT Bankomat FROM Accounts WHERE `Name` = '%s' LIMIT 1;", string);
	mysql_query(string);
	mysql_store_result();
	result = mysql_fetch_int();
	mysql_free_result();
	return result;
}

stock UpdateAccountBalance(playerid)
{
	new string[84];
	format(string, sizeof(string), "SELECT Bankomat FROM Accounts WHERE `Name` = '%s' LIMIT 1;", PlayerName(playerid));
	mysql_query(string);
	mysql_store_result();
	playerInfo[playerid][pBankomat] = mysql_fetch_int();
	mysql_free_result();
	return 1;
}

stock SaveAccountBalance(playerid)
{
	new string[84];
	format(string, sizeof(string), "UPDATE Accounts SET Bankomat = '%d' WHERE `Name` = '%s' LIMIT 1;", playerInfo[playerid][pBankomat], PlayerName(playerid));
	mysql_query(string);
	return 1;
}
#include "zones"

// KOMENDY GRACZA
 /* 
 	- /pomoc /help /faq
	- /toll
	- /animacja /animacje /anims /anim
	- /autorzy /autor
	- /telefon /tel /phone
	- /pojazd
	- /spawn
	- /pasy
	- /stats
	- /firma
	- /viatoll
*/

CMD:pomoc(playerid, params[])
	return cmd_faq(playerid, params);

CMD:help(playerid, params[])
	return cmd_faq(playerid, params);

CMD:faq(playerid, params[])
{
	Dialog_Show(playerid, DIALOG_FAQ, DIALOG_STYLE_LIST, "FAQ - Spis", "Podstawowe informacje\nSpis firm\nPoziomy kierowcy\nSpecjalne zdolnoœci\nOgraniczenia\nKomendy\nAutorzy", "Wybierz", "WyjdŸ");
	return 1;
}

Dialog:DIALOG_FAQ(playerid, response, listitem, inputtext[])
{
	if(!response)
	{
		if(!IsPlayerLogged(playerid))
			SelectTextDraw(playerid, 0xA82A23FF);
		return 1;
	}

	switch(listitem)
	{
		case 0: return cmd_faq123321(playerid);
		case 1: return cmd_spisfirm(playerid);
		case 2: return cmd_poziomy(playerid);
		case 3: return cmd_zdolnosci(playerid, "info");
		case 4: return cmd_faq123321(playerid);
		case 5: return cmd_cmd(playerid);
		case 6: return cmd_autorzy(playerid, "");
	}
	return 1;
}

CMD:faq123321(playerid)
{
	new s[2000];
	strcat(s,""GCOLOR_YELLOW"I. Czym jest Serwer-Truck?\n");
	strcat(s,""COLOR_GUI"Serwer Truck jest serwerem na którym jeŸdzimy pojazdami ciê¿arowymi lub dostawczymi i rozwozimy towary.\n");
	strcat(s,""COLOR_GUI"Po zarobieniu odpowiedniej iloœci pieniêdzy mo¿esz kupiæ pojazd prywatny w salonie, znajduj¹cym siê w Doherty San Fierro.\n");
	strcat(s,""COLOR_GUI"Oczywiœcie przed zakupieniem pojazdu prywatnego musisz posiadaæ domek.\n");
	strcat(s," \n");
	strcat(s,""GCOLOR_YELLOW"II. Jak pracowaæ i rozwoziæ towary?\n");
	strcat(s,""COLOR_GUI"Na pocz¹tku musimy zrespiæ pojazd za pomoc¹ komendy /pojazd. Nastêpnie udajemy siê do doku, gdzie ³adujemy towar.\n");
	strcat(s," \n");
	strcat(s,""GCOLOR_YELLOW"III. viaToll - co to takiego?\n");
	strcat(s,""COLOR_GUI"viaToll to system poboru p³atnoœci za autostrady, do³adujesz go w sklepie viaToll, znajduj¹cym siê\n\t na ka¿dej stacji benzynowej(¿ó³ta kropka na mapie).\n");
	strcat(s," \n");
	strcat(s,""GCOLOR_YELLOW"IV. Jak w³¹czyæ pojazd i œwiat³a?\n");
	strcat(s,""COLOR_GUI"To bardzo proste. Aby uruchomiæ silnik nale¿y u¿yæ kombinacji klawiszy 2 + E.\nW³¹czanie œwiate³ jest bardzo podobne, wystarczy u¿yæ kombinacji klawiszy 2 + Q.\n");
	strcat(s," \n");
	//strcat(s,""GCOLOR_YELLOW"V. Jak uzupe³niæ pasek g³odu?\n");
	//strcat(s,""COLOR_GUI"Pasek g³odu uzupe³niamy jedz¹c posi³ki w barach(hamburger na mapie).\n");
	//strcat(s," \n");
	//strcat(s,""GCOLOR_YELLOW"VI. Jak wyzerowaæ czas jazdy / jak zrobiæ pauzê?\n");
	//strcat(s,""COLOR_GUI"Pauzê robimy za pomoc¹ komendy /tachograf, po wpisaniu tej komendy do wyboru mamy dwie opcje a jedn¹ z nich jest pauza.\n");
	//strcat(s," \n");
	//strcat(s,""GCOLOR_YELLOW"VII. Jak kupiæ domek?\n");
	//strcat(s,""COLOR_GUI"JeÂ¿eli chcesz kupiæ dom jedziesz do jakiegokolwiek domku na mapce, zg³aszasz sie do administratora\n\t o wycenê - on stawia dom. Cennik domów znajduje siê na forum.\n");
	//strcat(s," \n");
	strcat(s,""GCOLOR_YELLOW"V. Co zrobiæ gdy ktoœ utrudnia grê?\n");
	strcat(s,""COLOR_GUI"Sk³adamy na niego raport, u¿ywamy komendy /report. W razie spotkania z cheaterem - nie pisz na czacie.\n");
	strcat(s," \n");
	strcat(s,""GCOLOR_YELLOW"Mi³ej gry ¿yczy administracja.\n");
	
	return Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, " ", s, "Wstecz", #);
}

CMD:poziomy(playerid)
{
	new s[940];
	strcat(s, ""GCOLOR_YELLOW"I. Jak dzia³aj¹ poziomy?");
	strcat(s, "\n");
	strcat(s, ""COLOR_GUI"Poziomy s¹ okreœlane na podstawie posiadanych przez Ciebie punktów.");
	strcat(s, "\n");
	strcat(s, ""COLOR_GUI"Im wiêcej punktów posiadasz, tym wiêkszy jest twój poziom.");
	strcat(s, "\n");
	strcat(s, ""GCOLOR_YELLOW"II. Ile poziomów mogê zdobyæ?");
	strcat(s, "\n");
	strcat(s, ""COLOR_GUI"Na naszym serwerze mo¿na maksymalnie zdobyæ 40 poziomów, które s¹ podzielone na tzw. grupy.");
	strcat(s, "\n");
	strcat(s, ""GCOLOR_YELLOW"III. Zaraz, o co chodzi z tymi grupami?");
	strcat(s, "\n");
	strcat(s, ""COLOR_GUI"Grupy to nic innego jak nazwy poziomów. Grupy zmieniaj¹ siê co 5 poziomów.");
	strcat(s, "\n");
	strcat(s, ""GCOLOR_YELLOW"IV. Wszystkie grupy");
	strcat(s, "\n");
	strcat(s, ""COLOR_GUI"Poziom 1 - 4\t\tNowicjusz");
	strcat(s, "\n");
	strcat(s, ""COLOR_GUI"Poziom 5 - 9\t\tEntuzjasta");
	strcat(s, "\n");
	strcat(s, ""COLOR_GUI"Poziom 10 - 14\t\tUtalentowany pracownik");
	strcat(s, "\n");
	strcat(s, ""COLOR_GUI"Poziom 15 - 19\t\tProfesjonalista");
	strcat(s, "\n");
	strcat(s, ""COLOR_GUI"Poziom 20 - 24\t\tMistrz");
	strcat(s, "\n");
	strcat(s, ""COLOR_GUI"Poziom 25 - 29\t\tInstruktor");
	strcat(s, "\n");
	strcat(s, ""COLOR_GUI"Poziom 30 - 34\t\tElita");
	strcat(s, "\n");
	strcat(s, ""COLOR_GUI"Poziom 35 - 39\t\tKról szos");
	strcat(s, "\n");
	strcat(s, ""COLOR_GUI"Poziom 40\t\tLegenda");
	return Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, " ", s, (IsPlayerLogged(playerid)) ? ("WyjdŸ") : ("Wstecz"), #);
}

CMD:spisfirm(playerid)
{
	new s[150];
	strcat(s, ""GCOLOR_YELLOW"Frakcje");
	strcat(s, "\n");
	strcat(s, ""COLOR_GUI"Policja\nPogotowie\nPomoc drogowa");
	strcat(s, "\n \n");
	strcat(s, ""GCOLOR_YELLOW"Firmy");
	strcat(s, "\n");
	strcat(s, ""COLOR_GUI"SM Logistic\nCargo Tranzit\nBuild Trans\nPetrol Tank");

	return Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, " ", s, "Wstecz", #);
}

CMD:animacje(playerid, cmdtext[])
	return cmd_anim(playerid, cmdtext);

CMD:animacja(playerid, cmdtext[])
	return cmd_anim(playerid, cmdtext);

CMD:anims(playerid, cmdtext[])
	return cmd_anim(playerid, cmdtext);

CMD:anim(playerid, cmdtext[])
	return Dialog_Show(playerid, ANIM1_DIALOG, DIALOG_STYLE_LIST, " ", "Tañce >>\nRêce\nWino\nSprunk\nCygaro\nPiwo\nTelefon >>\nSiad\nDilowanie\nJedzenie\nWal siê\nPijany", "OK", "Dalej");

CMD:autor(playerid, params[])
	return cmd_autorzy(playerid, params);

CMD:autorzy(playerid, params[])
	return Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, " ", "Programiœci:\n- Maciek.\n- GeDox\n- Kozak59\n\nBeta-Testerzy:\n- [ST]Kill_Repeat\n- [ST][DJ]Bass\n- [ST]Reiban.\n\nTworzenie obiektów:\n- [ST][DJ]Bass\n- TDi\n- Laud.\n- Muscu\n- Marcin[WGM]\n- PolskiJankes\n- Kierowca\n- Kozak59\n- Devil\n- Mati\n- TRAKER\n- Zeruœ\n- Prezes\n- KillerPL", "Wstecz", #);

CMD:tel(playerid, params[])
	return cmd_telefon(playerid, params);

CMD:phone(playerid, params[])
	return cmd_telefon(playerid, params);

CMD:telefon(playerid, params[])
{
	new szString[64];
	UpdateOnlineWorkers();
	format(szString, sizeof szString, "Policja\t\t\t(%d)\nPogotowie\t\t(%d)\nPomoc drogowa\t(%d)", NaDyzurze[1], NaDyzurze[2], NaDyzurze[3]);
	return Dialog_Show(playerid, DIALOG_ID_TELEPHONE, DIALOG_STYLE_LIST, " ", szString, "Wybierz", "Anuluj");
}

CMD:pojazd(playerid, params[])
{
	new bool:find;

	if(GetPVarInt(playerid, "Worked") > 0)
		return Msg(playerid, COLOR_ERROR, "Jesteœ aktualnie na s³u¿bie, nie mo¿esz tworzyæ pojazdów.");

	Loop(i, sizeof gSpawnvehicleplayer)
	{
		new baseTRUCK = GetDistanceFromMeToPoint(playerid, gSpawnvehicleplayer[i][0], gSpawnvehicleplayer[i][1], gSpawnvehicleplayer[i][2]);

		if(baseTRUCK <= 100)
		{
			find = true;
			Dialog_Show(playerid, DIALOG_ID_CREATE_VEHICLE, DIALOG_STYLE_LIST, " ", "Stworz pojazd\nUsuñ stworzony pojazd", "OK", "WyjdŸ");
			break;
		}
	}

	if(!find)
		Msg(playerid, COLOR_ERROR, "Pojazdy mo¿esz tworzyæ {b}tylko{/b} przy spawnie.");

	return 1;
}

CMD:pasy(playerid, params[])
{
	if(!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie {b}znajdujesz siê{/b} w pojeŸdzie.");

	if(!GetPVarInt(playerid, "PASY"))
	{
		SetPVarInt(playerid, "PASY", 1);
		Msg(playerid, COLOR_INFO, "Pasy zosta³y {b}zapiête{/b}.");
	}
	else
	{
		DeletePVar(playerid, "PASY");
		Msg(playerid, COLOR_INFO, "Pasy zosta³y {b}odpiête.{/b}");
	}

	return 1;
}

CMD:spawn(playerid, params[])
	return Selectspawn(playerid);

CMD:zapisz(playerid, params[])
	return SavePlayer(playerid);

CMD:firma(playerid, params[])
{
	if(!playerInfo[playerid][pFirm])
		return ShowInfo(playerid, "Nie pracujesz w ¿adnej firmie.");

	new string[300];
	strcat(string, "Informacje\nPracownicy online\nWp³aæ pieni¹dze");

	if(firmInfo[playerInfo[playerid][pFirm]][tChef] == playerInfo[playerid][pID] || firmInfo[playerInfo[playerid][pFirm]][tVcChef] == playerInfo[playerid][pID] || firmInfo[playerInfo[playerid][pFirm]][tChef] == playerInfo[playerid][pID] || firmInfo[playerInfo[playerid][pFirm]][tAccountant] == playerInfo[playerid][pID])
		strcat(string, "\n----\nRozdaj wyp³aty");

	if(firmInfo[playerInfo[playerid][pFirm]][tChef] == playerInfo[playerid][pID] || firmInfo[playerInfo[playerid][pFirm]][tVcChef] == playerInfo[playerid][pID])
		strcat(string, "\nZaproœ gracza\nWyrzuæ gracza\nZastêpca\nKsiêgowy\nUstaw spawn\nWyp³aæ pieni¹dze\nZmieñ skina");

	Dialog_Show(playerid, DIALOG_ID_FIRMA, DIALOG_STYLE_LIST, "Firma", string, "Dalej", "Anuluj");
	return 1;
}

CMD:stats(playerid, params[])
{
	new string[500], czasBiezacy[3];

	ConvertSeconds(GetDTime(playerid), czasBiezacy[0], czasBiezacy[1], czasBiezacy[2]);
	format(string, sizeof(string), "UID:\t %d\n", playerInfo[playerid][pID]);
	format(string, sizeof(string), "%sNick:\t %s\n", string, PlayerName(playerid));
	format(string, sizeof(string), "%sPieni¹dze:\t %d\n", string, GetMoney(playerid));
	format(string, sizeof(string), "%sBankomat:\t %d\n", string, playerInfo[playerid][pBankomat]);
	format(string, sizeof(string), "%sPunkty:\t %d\n", string, GetScore(playerid));
	format(string, sizeof(string), "%sPoziom:\t %s (%d)\n", string, GetPlayerLevelName(GetPVarInt(playerid, "LEVEL")), GetPVarInt(playerid, "LEVEL"));
	format(string, sizeof(string), "%sPoziom uprawnieñ:\t %d\n", string, playerInfo[playerid][pAdmin]);
	format(string, sizeof(string), "%sFirma:\t %d\n", string, playerInfo[playerid][pFirm]);
	format(string, sizeof(string), "%sSkin:\t %d\n", string, playerInfo[playerid][pSkin]);
	format(string, sizeof(string), "%sTachograf:\t %dh %dm %ds\n", string, czasBiezacy[0], czasBiezacy[1], czasBiezacy[2]);
	format(string, sizeof(string), "%sViaToll:\t %d\n", string, GetViaMoney(playerid));
	format(string, sizeof(string), "%sADR:\t %d\n", string, playerInfo[playerid][pADR]);
	format(string, sizeof(string), "%sCzas pracy:\t %dm\n", string, GetWork(playerid));
	format(string, sizeof(string), "%sBuildMaster:\t %d\n", string, playerInfo[playerid][pBuildmaster]);

	ConvertMS(GetTickCount()-GetPVarInt(playerid, "IleGral"), czasBiezacy[0], czasBiezacy[1], czasBiezacy[2]);

	format(string, sizeof(string), "%sCzas online (bie¿¹cy):\t %dh %dm %ds\n", string, czasBiezacy[0], czasBiezacy[1], czasBiezacy[2]);
	Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_TABLIST, "Statystyki gracza", string, "Ok", "");
	return 1;
}

CMD:pauza(playerid, params[])
{
	new vehicleid = GetPlayerVehicleID(playerid), engine,lights,alarm,doors,bonnet,boot,objective;
	GetVehicleParamsEx(vehicleid,engine,lights,alarm,doors,bonnet,boot,objective);

	if(!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie jesteœ w pojeŸdzie.");

	if(!IsPlayerInTruck(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie mo¿esz krêciæ pauzy w tym samochodzie.");

	if(engine)
		return Msg(playerid, COLOR_ERROR, "By krêciæ pauzê wy³¹cz silnik.");

	SetPVarInt(playerid, "tacho_pauza", 1);
	Msg(playerid, COLOR_INFO, "Krêcenie pauzy {b}w³¹czone{/b}. By zobaczyæ czas tachografu wpisz {b}/stats{/b}");
	return 1;
}

CMD:magnes(playerid, params[])
{
	if(!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie jesteœ w pojeŸdzie.");

	if(!IsPlayerInTruck(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie mo¿esz u¿ywaæ magnesu w tym samochodzie.");

	if(!playerInfo[playerid][pMagnes])
	{
		playerInfo[playerid][pMagnesTime] = gettime();
		playerInfo[playerid][pMagnes] = true;
		Msg(playerid, COLOR_INFO, "Magnes zosta³ w³¹czony.");
	}
	else
	{
		playerInfo[playerid][pMagnesTime] = gettime();
		playerInfo[playerid][pMagnes] = false;
		Msg(playerid, COLOR_INFO, "Magnes zosta³ wy³¹czony.");
	}
	return 1;
}

CMD:v(playerid, params[])
{
	new string[100], vehicleid = GetPlayerVehicleID(playerid), engine, lights, alarm, doors ,bonnet, boot, objective;

	GetVehicleParamsEx(vehicleid,engine,lights,alarm,doors,bonnet,boot,objective);

	if(!IsPlayerInAnyVehicle(playerid))
		return ShowInfo(playerid, "Nie jesteœ w ¿adnym pojeŸdzie.");

	format(string, sizeof(string), "Informacje\nMaska ( %s )\nBaga¿nik ( %s )\nZamek ( %s )", bonnet ? ("OTWARTA") : ("ZAMKNIÊTA"), boot ? ("OTWARTY") : ("ZAMKNIÊTY"), doors ? ("OTWARTE") : ("ZAMKNIÊTE"));

	if(!Spawned[vehicleid])
		strcat(string, "\nZarz¹dzaj");

	Dialog_Show(playerid, DIALOG_ID_VEHICLE, DIALOG_STYLE_LIST, "Pojazd", string, "Ok", "Anuluj");
	return 1;
}

CMD:cmd(playerid)
{
	new szString[1306];
	strcat(szString, "Komendy wszystkich:\n \n");
	strcat(szString, "/pomoc - Wyœwietla menu pomocy\n");
	strcat(szString, "/pauza - Krêcenie pauzy / odpoczynek\n");
	strcat(szString, "/animacja - Wyœwietla dostêpne na serwerze animacje\n");
	strcat(szString, "/autorzy - Pokazuje twórców mapy\n");
	strcat(szString, "/telefon - S³u¿y do wzywania frakcji uprzywilejowanych\n");
	strcat(szString, "/pojazd - Tworzenie pojazdu\n");
	strcat(szString, "/spawn - Przeniesienie gracza na spawn\n");
	strcat(szString, "/pasy - Zapinanie pasów w pojeŸdzie\n");
	strcat(szString, "/stats -Statystyki gracza\n");
	strcat(szString, "/firma - Wyœwietla menu firmy w której pracujemy\n");
	strcat(szString, "/viatoll - Kupowanie viatollu na stacji\n");
	strcat(szString, "/lastpos - Teleportacja do ostatniej pozycji\n");
	strcat(szString, "/v - Otwiera menu pojazdu\n");
	strcat(szString, "/tankuj - Tankowanie pojazdu\n");
	strcat(szString, "/praca - Rozpoczyna pracê\n");
	strcat(szString, "/dyzur - Rozpoczyna dy¿ur\n");
	strcat(szString, "/adr - Kurs adr\n");
	strcat(szString, "/pogodynka - wWyœwietla prognozê pogody\n");
	strcat(szString, "/reczny - Zaci¹ganie hamulca rêcznego\n");
	strcat(szString, "/konwoj - Tworzenie konwoju\n");
	strcat(szString, "/zaproszenia - Zaproszenia do konwoju\n");
	strcat(szString, "/radio - W³¹czanie radia\n");
	strcat(szString, "/bankomat - Otwiera menu bankomatu\n");
	strcat(szString, "/tablica - Tablica og³oszeñ\n");
	strcat(szString, "/spisfirm - Spis firm/frakcji dostêpnych na serwerze\n");
	strcat(szString, "/zdolnosci - Menu zdolnoœci kierowcy\n");
	strcat(szString, "/poziomy - Informacje o poziomach\n");
	strcat(szString, "/magnes - uruchamia magnes tachografu\n");
	strcat(szString, "/gps - otwiera menu GPSu\n");
	strcat(szString, "/anuluj - anuluje obecne zlecenie");

	return Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, "Dostêpne komendy", szString, (IsPlayerLogged(playerid)) ? ("WyjdŸ") : ("Wstecz"), #);
}

CMD:cmds(playerid)
{
	new firm = playerInfo[playerid][pFirm];
	if(firm < 1)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Aktualnie nie pracujesz.");

	new szString[330];
	strcat(szString, szCommands[firm]);
	return Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, "Dostêpne komendy", szString, "WyjdŸ", #);
}

CMD:komendy(playerid)
{
	return cmd_cmd(playerid);
}

CMD:tachograf(playerid)
	return ShowInfo(playerid, "Tachograf - jest to urz¹dzenie pomiarowe, które pe³ni funkcjê zegara.\nZe wzglêdu na bezpieczeñstwo na drogach, wprowadzono ograniczenia\n czasu pracy kierowców pojazdów o DMC > 6t.\nBy zbiæ czas, nale¿y zjechaæ na pobocze i zgasiæ silnik.");

// KOMENDY POGOTOWIA
/*
	- /ulecz
	- /wywal /wyrzuc
	- /praca
	- /firma
	- /wypadki
*/

CMD:acmd(playerid)
{
	if(playerInfo[playerid][pAdmin] < 1)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	new szString[2000];

	strcat(szString, "/mute - wycisza gracza\n");
	strcat(szString, "/unmute - koniec wyciszenia gracza\n");
	strcat(szString, "/warn - daje ostrze¿enie graczowi\n");
	strcat(szString, "/unwarn - zdejmuje ostrze¿enie z gracza\n");
	strcat(szString, "/kick - wyrzuca gracza z serwera\n");
	strcat(szString, "/ban - banuje gracza na dany okres czasu\n");
	strcat(szString, "/pogoda - zmiana pogody na serwerze\n");
	strcat(szString, "/clear - czyœcie czat\n");
	strcat(szString, "/refuelveh - tankuje pojazd\n");
	strcat(szString, "/repair - naprawia pojazd\n");
	strcat(szString, "/colorplayer - zmienia kolor nicku gracza\n");
	strcat(szString, "/createveh - tworzy pojazd admina\n");
	strcat(szString, "/destroyveh - usuwa wybrany pojazd admina\n");
	strcat(szString, "/destroyvehall - usuwa wszystkie pojazdy stworzone przez admina\n");
	strcat(szString, "/setcash - ustawia stan gotówki gracza\n");
	strcat(szString, "/givemoney - dodaje gotówkê\n");
	strcat(szString, "/resetscore - resetuje iloœæ score gracza\n");
	strcat(szString, "/resetmoney - resetuje iloœæ pieniêdzy gracza\n");
	strcat(szString, "/setscore - ustawia iloœæ score danego gracza\n");
	strcat(szString, "/givescore - dodaje score\n");
	strcat(szString, "/givescoreall - dodaje score wszystkim graczom na serwerze\n");
	strcat(szString, "/givemoneyall - dodaje pieni¹dze wszystkim graczom na serwerze\n");
	strcat(szString, "/dodajpracownika - dodaje pracownika do firmy/frakcji\n");
	strcat(szString, "/ustawlidera - ustawia lidera firmy/frakcji\n");
	strcat(szString, "/hideme - ukrywa administratora na liœcie\n");
	strcat(szString, "/showme - pokazuje administratora na liœcie\n");
	strcat(szString, "/respawn - respawn nieu¿ywanych pojazdów\n");
	strcat(szString, "/givegun - daje broñ danemu graczowi\n");
	strcat(szString, "/disarm - rozbraja gracza\n");
	strcat(szString, "/disarmall - rozbraja wszystkich graczy\n");
	strcat(szString, "/setskin - ustawia skin danemu graczowi\n");
	strcat(szString, "/heal - ulecza gracza\n");
	strcat(szString, "/armor - dajesz armor graczowi\n");
	strcat(szString, "/killp - zabija gracza\n");
	strcat(szString, "/explode - exploduje gracza\n");
	strcat(szString, "/remove - wyrzuca gracza z pojazdu\n");
	strcat(szString, "/freeze - zamra¿a gracza\n");
	strcat(szString, "/unfreeze - odmra¿a gracza\n");
	strcat(szString, "/freezeall - zamra¿a wszystkich graczy na serwerze\n");
	strcat(szString, "/unfreezeall - odmra¿a wszystkich graczy na serwerze\n");
	strcat(szString, "/cd - rozpoczyna odliczanie z zamro¿eniem wszystkich graczy lub bez\n");
	strcat(szString, "/uping - podnosi gracza\n");
	strcat(szString, "/downing - opuszcza gracza\n");
	strcat(szString, "/specon - podgl¹dasz gracza\n");
	strcat(szString, "/specoff - wy³¹cza podgl¹d gracza\n");
	strcat(szString, "/tp - teleportuje gracza\n");
	strcat(szString, "/tphere - teleportuje gracza do nas\n");
	strcat(szString, "/setloc - ustawia teleport\n");
	strcat(szString, "/tploc - teleportuje do ustawionego teleportu\n");
	strcat(szString, "/ann - og³oszenie na œrodku ekranu");

	return Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, "Dostêpne komendy", szString, "WyjdŸ", #);
}

CMD:ulecz(playerid, params[])
{
	new targetid, string[90];

	if(firmInfo[playerInfo[playerid][pFirm]][tType] != TEAM_TYPE_MEDIC)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(sscanf(params, "d", targetid))
		return Msg(playerid, COLOR_ERROR, "Wpisz: {b}/ulecz [id gracza]{/b}");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Gracza z tym ID nie ma online.");

	if(GetDistancePlayerToPlayer(playerid, targetid) > 3.0)
		return Msg(playerid, COLOR_ERROR, "Znajdujesz siê za daleko od gracza.");

	if(GetPlayerSpeed(playerid) > 0)
		return Msg(playerid, COLOR_ERROR, "Gracz nie mo¿e siê poruszaæ.");

	if(GetPVarInt(targetid, "Wypadek"))
	{
		FadeColorForPlayer(targetid, 204, 58, 41, 125, 0, 0, 0, 0, 25, 0);
		DeletePVar(targetid, "Wypadek");
		DeletePVar(targetid, "Wypadekzmedykiem");
	}

	KillTimer(GetPVarInt(playerid, "healTimer"));
	DeletePVar(playerid, "healTimer");

	TogglePlayerControllable(targetid, 1);
	SetPlayerHealth(targetid, 100.0);

	format(string, sizeof(string), "Uleczy³eœ gracza {b}%s{/b} (ID: {b}%d{/b}).", PlayerName(targetid), targetid);
	Msg(playerid, COLOR_INFO3, string);

	format(string, sizeof(string), "Zosta³eœ uleczony przez ratownika {b}%s{/b} (ID: {b}%d{/b}).", PlayerName(playerid), playerid);
	Msg(targetid, COLOR_INFO3, string);
	return 1;
}

CMD:wypadki(playerid)
{
	if(firmInfo[playerInfo[playerid][pFirm]][tType] != TEAM_TYPE_MEDIC)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");
	return cmd_zgloszenia(playerid);
}

CMD:zgloszenia(playerid)
{
	new szString[1024], count, firmaid = playerInfo[playerid][pFirm];

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(firmInfo[firmaid][tType] != TEAM_TYPE_POLICE && firmInfo[firmaid][tType] != TEAM_TYPE_MEDIC && firmInfo[firmaid][tType] != TEAM_TYPE_POMOC)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	new type = firmInfo[firmaid][tType]-1;

	for(new i = 0; i < MAX_PLAYERS*2; i++)
	{
		if(callInfo[type][callUsed][i])
		{
			if(!IsPlayerConnected(callInfo[type][callPCalled][i]))
			{
				callInfo[type][callUsed][i] = false;
				continue;
			}
			count++;
			format(szString, sizeof szString, "%s%d\t%s\t%s\t%s\n", szString, i, (callInfo[type][callAssigned][i] > -1) ? ("+") : ("-"), PlayerName(callInfo[type][callPCalled][i]), GetPlayerZone(callInfo[type][callPCalled][i]));
		}
	}
	strins(szString, "ID\tStatus\tNick\tLokalizacja\n", 0);
	if(count < 1)
		return Msg(playerid, COLOR_ERROR, "Brak wypadków / zg³oszeñ.");

	Dialog_Show(playerid, DIALOG_CALL, DIALOG_STYLE_TABLIST_HEADERS, "Lista zg³oszeñ", szString, "Wybierz", "WyjdŸ");
	return 1;
}

CMD:wezwania(playerid)
{
	return cmd_zgloszenia(playerid);
}

Dialog:DIALOG_CALL(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new id, type = firmInfo[playerInfo[playerid][pFirm]][tType]-1;
	sscanf(inputtext, "d{s[128]}", id);

	if(GetPVarInt(playerid, "RESERVED") && (callInfo[type][callAssigned][id] != playerid))
	{
		cmd_zgloszenia(playerid);
		return Msg(playerid, COLOR_ERROR, "Ju¿ przyj¹³eœ inne zg³oszenie.");
	}

	if(!IsPlayerConnected(callInfo[type][callPCalled][id]))
	{
		cmd_zgloszenia(playerid);
		return Msg(playerid, COLOR_ERROR, "Oops. Wygl¹da na to, ¿e ten gracz opuœci³ serwer.");
	}

	SetPVarInt(playerid, "CRASH_ID", id);

	Dialog_Show(playerid, DIALOG_CALL2, DIALOG_STYLE_LIST, " ", "Zaznacz zg³oszenie\nUsuñ zg³oszenie", "Wybierz", "Wstecz");
	return 1;
}

Dialog:DIALOG_CALL2(playerid, response, listitem, inputtext[])
{
	if(!response)
		return cmd_zgloszenia(playerid);

	new id = GetPVarInt(playerid, "CRASH_ID"), type = firmInfo[playerInfo[playerid][pFirm]][tType]-1;
	switch(listitem)
	{
		case 0:
		{
			if(callInfo[type][callAssigned][id] > -1)
			{
				Msg(playerid, COLOR_ERROR, "To zg³oszenie zosta³o ju¿ przyjête.");
				return cmd_zgloszenia(playerid);
			}
			callInfo[type][callAssigned][id] = playerid;
			Msg(playerid, COLOR_INFO, "Pomyœlnie przyjêto zg³oszenie.");
			Msg(callInfo[type][callPCalled], COLOR_INFO, "Zg³oszenie zosta³o przyjête! Ju¿ ktoœ do Ciebie jedzie!");
			SetPVarInt(playerid, "RESERVED", 1);

			new Float:pos[3], player = callInfo[type][callPCalled][id];
			GetPlayerPos(player, pos[0], pos[1], pos[2]);
			SetPlayerMapIcon(playerid, 73, pos[0], pos[1], pos[2], 41, 0, MAPICON_GLOBAL);
		}
		case 1:
		{
			if(callInfo[type][callAssigned][id] != playerid)
				return Msg(playerid, COLOR_ERROR, "Nie mo¿esz usun¹æ tego zg³oszenia je¿eli go nie przyjmowa³eœ.");
			else 
			{
				RemovePlayerMapIcon(playerid, 73);
				callInfo[type][callUsed][id] = false;
				Msg(playerid, COLOR_INFO, "Zg³oszenie zosta³o pomyœlnie usuniête.");
				DeletePVar(playerid, "RESERVED");
			}
		}
	}
	return 1;
}

CMD:dyzur(playerid, params[])
{
	new string[120], firmaid=playerInfo[playerid][pFirm], Float:ObjectPos[3];

	if(firmInfo[firmaid][tType] != TEAM_TYPE_POLICE && firmInfo[firmaid][tType] != TEAM_TYPE_MEDIC && firmInfo[firmaid][tType] != TEAM_TYPE_POMOC)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	DeletePVar(playerid, "RESERVED");

	if(!GetPVarInt(playerid, "Working"))
		return Msg(playerid, COLOR_ERROR, "Musisz zacz¹æ pracê. Wpisz {b}/praca{/b}.");

	if(!GetPVarInt(playerid, "Worked"))
	{
		SetPVarInt(playerid, "Worked", 1);
		
		Loop(player, MAX_PLAYERS)
		{
			format(string, sizeof string, "Gracz {b}%s{/b} rozpocz¹³ dy¿ur w: {b}%s{/b}.", PlayerName(playerid), firmInfo[firmaid][tName]);
			Msg(player, COLOR_INFO2, string);
		}

		if(firmInfo[firmaid][tType] == TEAM_TYPE_MEDIC)
			EnablePlayerCameraTarget(playerid, 1);
	}
	else
	{
		DeletePVar(playerid, "Worked");
		
		Loop(player, MAX_PLAYERS)
		{
			if(GetDistancePlayerToPlayer(playerid, player) < 30)
			{
				format(string, sizeof string, "Gracz {b}%s{/b} zakoñczy³ dy¿ur w: {b}%s{/b}.", PlayerName(playerid), firmInfo[firmaid][tName]);
				Msg(player, COLOR_INFO2, string);
			}
		}

		if(GetPVarInt(playerid, "Worked") && firmInfo[playerInfo[playerid][pFirm]][tType] > 0 && firmInfo[playerInfo[playerid][pFirm]][tType] < 4)
		{
			for(new d = 0; d < MAX_PLAYERS*2; d++)
			{
				if(!callInfo[firmInfo[playerInfo[playerid][pFirm]][tType]-1][callUsed][d])
					continue;

				if(playerid == callInfo[firmInfo[playerInfo[playerid][pFirm]][tType]-1][callAssigned][d])
					callInfo[firmInfo[playerInfo[playerid][pFirm]][tType]-1][callAssigned][d] = -1;
			}
			RemovePlayerMapIcon(playerid, 73);
		}

		if(GetPVarInt(playerid, "RadarOnline"))
		{
			ShowPlayerRadar(playerid, false);
			Msg(playerid, COLOR_INFO, "Wy³¹czono radar.");
			DeletePVar(playerid, "RadarOnline");
		}

		if(firmInfo[firmaid][tType] == TEAM_TYPE_MEDIC)
			EnablePlayerCameraTarget(playerid, 0);
	}

	UpdateOnlineWorkers();

	if(firmInfo[firmaid][tType] == TEAM_TYPE_POMOC)
	{
		if(NaDyzurze[firmaid] == 1)
		{
			for(new i; i <= 8; i++)
				GetDynamicObjectPos(Spray[i], ObjectPos[0], ObjectPos[1], ObjectPos[2]),
				MoveDynamicObject(Spray[i], ObjectPos[0], ObjectPos[1], ObjectPos[2]+7.0, 10);
		}
		else
		{
			for(new i; i <= 8; i++)
				GetDynamicObjectPos(Spray[i], ObjectPos[0], ObjectPos[1], ObjectPos[2]),
				MoveDynamicObject(Spray[i], ObjectPos[0], ObjectPos[1], ObjectPos[2]-7.0, 10);
		}
	}

	return 1;
}

CMD:praca(playerid, params[])
{
	new firmaid = playerInfo[playerid][pFirm];

	if(!firmaid)
		return ShowInfo(playerid, "Nie posiadasz ¿adnej pracy.");

	if(GetPVarInt(playerid, "Working"))
	{
		DeletePVar(playerid, "Worked");
		DeletePVar(playerid, "Working");

		if(GetPVarInt(playerid, "RadarOnline"))
		{
			ShowPlayerRadar(playerid, false);
			Msg(playerid, COLOR_INFO, "Wy³¹czono radar.");
			DeletePVar(playerid, "RadarOnline");
		}

		if(firmInfo[playerInfo[playerid][pFirm]][tType] > 0 && firmInfo[playerInfo[playerid][pFirm]][tType] < 4)
		{
			for(new d = 0; d < MAX_PLAYERS*2; d++)
			{
				if(!callInfo[firmInfo[playerInfo[playerid][pFirm]][tType]-1][callUsed][d])
					continue;

				if(playerid == callInfo[firmInfo[playerInfo[playerid][pFirm]][tType]-1][callAssigned][d])
					callInfo[firmInfo[playerInfo[playerid][pFirm]][tType]-1][callAssigned][d] = -1;
			}
			RemovePlayerMapIcon(playerid, 73);
		}

		SetPlayerSkin(playerid, playerInfo[playerid][pSkin]);
		Msg(playerid, COLOR_INFO, "Zakoñczy³eœ pracê.");
		ResetPlayerWeapons(playerid);
		SetPlayerColor(playerid, 0x009300FF);
		SetPlayerArmour(playerid, 0);
	}
	else
	{
		if(GetPVarInt(playerid, "pojazd") > 0)
		{
			Spawned[GetPVarInt(playerid, "pojazd")] = false;
			ResetVariablesInEnum(vloadInfo[GetPVarInt(playerid, "pojazd")], eLoadVehicle);
			DestroyVehicle(GetPVarInt(playerid, "pojazd"));
			DeletePVar(playerid, "pojazd");
		}

		switch(firmInfo[firmaid][tType])
		{
			case TEAM_TYPE_POLICE: CallLocalFunction("Policja_OnPlayerEnterJob", "d", playerid);

			case TEAM_TYPE_MEDIC:
			{
				GivePlayerWeapon(playerid, 9, 99999);
    			GivePlayerWeapon(playerid, 42, 99999);
			}

			case TEAM_TYPE_POMOC: CallLocalFunction("Pomoc_OnPlayerEnterJob", "d", playerid);

			case TEAM_TYPE_BUILD:
			{
				if(GetPVarInt(playerid, "FLYKAMERA"))
				{
					new Float:Pos[3];
					Pos[0] = GetPVarFloat(playerid, "BTP");
					Pos[1] = GetPVarFloat(playerid, "BTP1");
					Pos[2] = GetPVarFloat(playerid, "BTP2");
					SetPlayerPos(playerid,Pos[0],Pos[1],Pos[2]+1);
					
					DeletePVar(playerid, "FLYKAMERA");
					DeletePVar(playerid, "BTP");
					DeletePVar(playerid, "BTP1");
					DeletePVar(playerid, "BTP2");
				}
			}
		}

		if(playerInfo[playerid][pFirm] <= 3)
			Msg(playerid, COLOR_INFO, "Zaczynasz pracê. By móc u¿ywaæ komend wpisz {b}/dyzur{/b}.");
		else
			SetPVarInt(playerid, "Worked", 1);

		SetPVarInt(playerid, "Working", 1);
		SetPlayerColor(playerid, firmInfo[firmaid][tColor]);
		UpdateOnlineWorkers();

		Dialog_Show(playerid, DIALOG_TP_LOGIN_SKIN, DIALOG_STYLE_LIST, "Firma > Skiny", "Skin 1\nSkin 2\nSkin 3\nSkin 4\nSkin 5", "Ok", "");
	}

	return 1;
}

// KOMENDY ADMINA
/*
	- /god
	- /repair
	- /tpto
	- /tphere
	- /tp
	- /eye
	- /flip /f
	- /setloc
	- /tploc
	- /setscore
	- /setcash
	- /say
	- /refuelveh
	- /refuelvehall
	- /givegun
	- /givegunall
	- /disarm
	- /disarmall
	- /food
	- /foodall
	- /remove
	- /createveh
	- /destroyveh
	- /destroyvehall
	- /put
	- /specon
	- /specoff
	- /cd
	- /restart
	- /freeze
	- /freezeall
	- /unfreeze
	- /unfreezeall
	- /killp
	- /colorplayer
	- /dodajadmin
	- /usunadmin
	- /setname
	- /dodajdok
	- /usundok
	- /respawniko
	- /dodajtowar
	- /usuntowar
	- /dodajstacje
	- /usunstacje
	- /dodajpojazd
	- /ustawlidera
	- /pogoda
	- /dodajpracownika
	- /usunpracownika
*/

CMD:god(playerid, params[])
{
	new targetid, string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "d", targetid))
		return Msg(playerid, COLOR_ERROR, "Wpisz: {b}/god [id gracza]{/b}");

	SetPlayerHealth(targetid, 9999999);
	format(string, sizeof string, "Da³eœ nieœmiertelnoœæ graczowi {b}%s{/b} (ID: %d).", PlayerName(targetid), targetid);
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "Administrator {b}%s{/b} (ID: %d) da³ Ci nieœmiertelnoœæ.", PlayerName(playerid), playerid);
	
	return Msg(playerid, COLOR_INFO, string);
}

CMD:repair(playerid, params[])
{
	new targetid, string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "d", targetid))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /repair [id gracza]");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie jest obecny na serwerze.");

	if(!IsPlayerInAnyVehicle(targetid))
		return Msg(playerid, COLOR_ERROR, "Gracz ten nie znajduje siê w pojeŸdzie.");

	RepairVehicle(GetPlayerVehicleID(targetid));
	
	if(IsTrailerAttachedToVehicle(GetPlayerVehicleID(targetid)))
		RepairVehicle(GetVehicleTrailer(GetPlayerVehicleID(targetid)));

	format(string, sizeof string, "Naprawi³eœ pojazd graczowi {b}%s{/b} (ID: %d).", PlayerName(targetid), targetid);
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "Administrator {b}%s{/b} (ID: %d) naprawi³ ci pojazd.", PlayerName(playerid), playerid);

	return Msg(targetid, COLOR_INFO, string);
}

CMD:tpto(playerid, params[])
{
	new targetid;
	if(sscanf(params, "d", targetid))
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/tpto [id gracza]{/b}");

	new string[10];
	format(string, sizeof(string), "%d %d", playerid, targetid);
	return cmd_tp(playerid, string);
}

CMD:tphere(playerid, params[])
{
	if(isnull(params))
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/tphere [id gracza]{/b}");

	new string[10];
	format(string, sizeof(string), "%s %d", params, playerid);
	return cmd_tp(playerid, string);
}

CMD:tp(playerid, params[])
{
	new targetid, wheplayerid, string[138];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "dd", targetid, wheplayerid))
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/tp [id gracza] [id gracza]{/b}");

	new Float:x, Float:y, Float:z;
	new vehicleid = GetPlayerVehicleID(targetid);
	GetPlayerPos(wheplayerid, x,y,z);

	if(IsPlayerInAnyVehicle(targetid))
	{
		SetVehiclePos(vehicleid, x,y,z);
		SetPlayerPos(targetid, x,y,z);
		PutPlayerInVehicle(targetid, vehicleid, 0);
	}
	else
		SetPlayerPos(targetid, x,y,z);
	
	if(playerid == targetid)
	{
		format(string, sizeof string, "Teleportowa³eœ siê do {b}%s{/b} (ID: {b}%d{/b}).", PlayerName(wheplayerid), wheplayerid);
		Msg(playerid, COLOR_INFO2, string);
		format(string, sizeof string, "Administrator {b}%s{/b} (ID: {b}%d{/b}) teleportowa³ siê do Ciebie.", PlayerName(playerid), playerid);
		Msg(wheplayerid, COLOR_INFO2, string);
	}
	else if(playerid == wheplayerid)
	{
		format(string, sizeof string, "Teleportowa³eœ {b}%s{/b} (ID: {b}%d{/b}) do siebie.", PlayerName(targetid), targetid);
		Msg(playerid, COLOR_INFO2, string);
		format(string, sizeof string, "Administrator {b}%s{/b} (ID: {b}%d{/b}) teleportowa³ siê do ciebie.", PlayerName(playerid), playerid);
		Msg(targetid, COLOR_INFO2, string);
	}
	else
	{
		format(string, sizeof string, "Teleportowa³eœ gracza {b}%s{/b} (ID: {b}%d{/b}) do {b}%s{/b} (ID: {b}%d{/b}).", PlayerName(targetid), targetid, PlayerName(wheplayerid), wheplayerid);
		Msg(playerid, COLOR_INFO2, string);
		format(string, sizeof string, "Administrator {b}%s{/b} (ID: {b}%d{/b}) teleportowa³ ciê do {b}%s{/b} (ID: {b}%d{/b}).", PlayerName(playerid), playerid, PlayerName(wheplayerid), wheplayerid);
		Msg(targetid, COLOR_INFO2, string);
		format(string, sizeof string, "Administrator {b}%s{/b} (ID: {b}%d{/b}) teleportowa³ do ciebie {b}%s{/b} (ID: {b}%d{/b}).", PlayerName(playerid), playerid, PlayerName(targetid), targetid);
		Msg(wheplayerid, COLOR_INFO2, string);
	}

	return 1;
}

CMD:eye(playerid, params[])
{
	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(podgladADMIN[playerid])
		Msg(playerid, COLOR_INFO, "{b}Wy³¹czy³eœ{/b} podgl¹d CMD."); 
	else
		Msg(playerid, COLOR_INFO, "{b}W³¹czy³eœ{/b} podgl¹d CMD."); 

	podgladADMIN[playerid] = !podgladADMIN[playerid];

	return 1;
}

CMD:f(playerid, params[])
	return cmd_flip(playerid, params);

CMD:flip(playerid, params[])
{
	new State = GetPlayerState(playerid);
	new vehicleid = GetPlayerVehicleID(playerid);

	if(!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie znajdujesz siê w pojeŸdzie.");

	if(State != PLAYER_STATE_DRIVER)
		return Msg(playerid, COLOR_ERROR, "Nie jesteœ {b}kierowc¹{/n}.");

	new Float:angle;
    GetVehicleZAngle(vehicleid, angle);
    SetVehicleZAngle(vehicleid, angle);

	return 1;
}

CMD:setloc(playerid, params[])
{
	new string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	savedpos++;
	new Float:X, Float:Y, Float:Z, Float:A, I;
	GetPlayerPos(playerid, X, Y, Z);
	GetPlayerFacingAngle(playerid, A);
	I = GetPlayerInterior(playerid);
	savpos[savedpos][0] = X;
	savpos[savedpos][1] = Y;
	savpos[savedpos][2] = Z;
	savpos[savedpos][3] = A;
	savpos[savedpos][4] = I;
	format(string, sizeof string, "Ustawi³eœ lokalizacje o id: {b}%d{/b}, interiorze: {b}%d{/b}", savedpos, I);
	Msg(playerid, COLOR_INFO, string);
	return 1;
}

CMD:tploc(playerid, params[])
{
	new locid, string[128];

	if(sscanf(params, "d", locid))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /tploc [id]");

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(!locid) 
		return Msg(playerid, COLOR_ERROR, "Wpisz: /tploc [id]");

	if(locid>savedpos) 
		return Msg(playerid, COLOR_ERROR, "Wpisz: /tploc [id]");

	if(IsPlayerInAnyVehicle(playerid))
	{
		new VehicleID;
		VehicleID = GetPlayerVehicleID(playerid);

		SetVehiclePos(VehicleID, savpos[locid][0],savpos[locid][1], savpos[locid][2]);
		SetPlayerFacingAngle(playerid, savpos[locid][3]);
		SetPlayerInterior(playerid, floatround(savpos[locid][4]));
	}
	else
	{
		SetPlayerPos(playerid, savpos[locid][0],savpos[locid][1], savpos[locid][2]);
		SetPlayerFacingAngle(playerid, savpos[locid][3]);
		SetPlayerInterior(playerid, floatround(savpos[locid][4]));
	}

	format(string, sizeof string, "Teleportowa³eœ siê do lokalizacji o id: %d", locid);
	Msg(playerid, COLOR_INFO, string);
	return 1;
}

CMD:setscore(playerid, params[])
{
	new targetid, score;

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "dd", targetid, score))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /setscore [id gracza] [score]");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Gracz nie jest na serwerze.");

	if(score < 0)
		return Msg(playerid, COLOR_ERROR, "Nie mo¿esz ustawiæ mniej ni¿ 0.");

	ResetScore(targetid);
	GiveScore(targetid, score);

	new string[126];
	format(string, sizeof(string), "Administrator {b}%s{/b} (ID: {b}%d{/b}) zmieni³ Ci punkty score na {b}%d{/b}.", PlayerName(playerid), playerid, score);
	Msg(targetid, COLOR_INFO, string);
	format(string, sizeof(string), "Ustawi³eœ graczowi {b}%s{/b} (ID: {b}%d{/b}) punkty score na {b}%d{/b}.", PlayerName(targetid), targetid, score);
	Msg(playerid, COLOR_INFO, string);

	return 1;
}

CMD:setcash(playerid, params[])
{
	new targetid, kasa;

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "dd", targetid, kasa))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /setcash [id gracza] [kasa]");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Gracz nie jest na serwerze.");

	if(kasa < 0)
		return Msg(playerid, COLOR_ERROR, "Nie mo¿esz ustawiæ mniej ni¿ 0.");

	ResetMoney(targetid);
	GiveMoney(targetid, kasa);

	new string[126];
	format(string, sizeof(string), "Administrator {b}%s{/b} (ID: {b}%d{/b}) ustawi³ ci kasê na {b}%d${/b}.", PlayerName(playerid), playerid, kasa);
	Msg(targetid, COLOR_INFO, string);
	format(string, sizeof(string), "Ustawi³eœ graczowi {b}%s{/b} (ID: {b}%d{/b}) kasê na {b}%d${/b}.", PlayerName(targetid), targetid, kasa);
	Msg(playerid, COLOR_INFO, string);

	return 1;
}

CMD:refuelvehall(playerid, params[])
{
	new param[30];
	format(param, sizeof(param), "%d 1", playerid);
	return cmd_refuelveh(playerid, param);
}

CMD:refuelveh(playerid, params[])
{
	new targetid, all, string[90];
	new vehicleid;

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "dD(0)", targetid, all))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /refuelveh [id gracza]");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Gracz nie jest pod³¹czony.");

	if(all)
	{
		Loop(vehicle, GetVehiclePoolSize())
			if(IsValidVehicle(vehicle))
			{
				if(Spawned[vehicle])
					vehInfo_Temp[vehicle][vFuel] = MaxFuel(GetVehicleModel(vehicle));
				else
					vehInfo[DBVehID[vehicle]][vFuel] = MaxFuel(GetVehicleModel(vehicle));
			}

		format(string, sizeof(string), "Administrator {b}%s{/b} (ID: {b}%d{/b}) zatankowa³ wszystkie pojazdy.", PlayerName(playerid), playerid);
		MsgToAll(COLOR_INFO, string);
	}
	else
	{
		vehicleid = GetPlayerVehicleID(targetid);

		if(Spawned[vehicleid])
			vehInfo_Temp[vehicleid][vFuel] = MaxFuel(GetVehicleModel(vehicleid));
		else
			vehInfo[DBVehID[vehicleid]][vFuel] = MaxFuel(GetVehicleModel(vehicleid));

		format(string, sizeof(string), "Administrator {b}%s{/b} (ID: {b}%d{/b}) zatankowa³ Ci pojazd.", PlayerName(playerid), playerid);
		Msg(targetid, COLOR_INFO, string);

		format(string, sizeof(string), "Zatankowa³eœ pojazd graczowi {b}%s{/b} (ID: {b}%d{/b}).", PlayerName(targetid), targetid);
		Msg(playerid, COLOR_INFO, string);
	}
	return 1;
}

CMD:givegunall(playerid, params[])
{
	new param[30];
	format(param, sizeof(param), "%d %s 1", playerid, params);
	return cmd_givegun(playerid, param);
}

CMD:givegun(playerid, params[])
{
	new targetid, weapon, all, string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "ddD(0)", targetid, weapon, all))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /givegun [id gracza] [id broni]");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Gracz nie jest pod³¹czony.");

	if(weapon < 0 || weapon > 46)
		return Msg(playerid, COLOR_ERROR, "B³êdne ID broni ({b}1-46{/b}).");

	if(all && targetid == playerid)
	{
		Loop(player, MAX_PLAYERS)
			if(IsPlayerConnected(player))
				GivePlayerWeapon(player, weapon, 9999999);

		format(string, sizeof(string), "Administrator {b}%s{/b} (ID: {b}%d{/b}) da³ wszystkim broñ (%s).", PlayerName(playerid), playerid, GetWeaponNameEx(weapon));
		MsgToAll(COLOR_INFO, string);
	}
	else
	{
		GivePlayerWeapon(targetid, weapon, 9999999);

		format(string, sizeof(string), "Administrator {b}%s{/b} (ID: {b}%d{/b}) da³ Ci broñ ({b}%s{/b}).", PlayerName(playerid), playerid, GetWeaponNameEx(weapon));
		Msg(targetid, COLOR_INFO, string);

		format(string, sizeof(string), "Da³eœ %s graczowi %s (ID: {b}%d{/b}).", GetWeaponNameEx(weapon), PlayerName(targetid), targetid);
		Msg(playerid, COLOR_INFO, string);
	}
	return 1;
}

CMD:disarmall(playerid, params[])
{
	new param[30];
	format(param, sizeof(param), "%d 1", playerid);
	return cmd_disarm(playerid, param);
}

CMD:disarm(playerid, params[])
{
	new targetid, all, string[90];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "ddD(0)", targetid, all))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /disarm [id gracza]");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Gracz nie jest pod³¹czony.");

	if(all)
	{
		Loop(player, MAX_PLAYERS)
			if(IsPlayerConnected(player))
				ResetPlayerWeapons(player);

		format(string, sizeof(string), "Administrator {b}%s{/b} (ID: {b}%d{/b}) zabra³ wszystkim bronie.", PlayerName(playerid), playerid);
		MsgToAll(COLOR_INFO, string);
	}
	else
	{
		ResetPlayerWeapons(targetid);

		format(string, sizeof(string), "Administrator {b}%s{/b} (ID: {b}%d{/b}) zresetowa³ ci bronie.", PlayerName(playerid), playerid);
		Msg(targetid, COLOR_INFO, string);

		format(string, sizeof(string), "Zresetowa³eœ bronie graczowi {b}%s{/b} (ID: {b}%d{/b}).", PlayerName(targetid), targetid);
		Msg(playerid, COLOR_INFO, string);
	}
	return 1;
}

CMD:foodall(playerid, params[])
{
	new param[30];
	format(param, sizeof(param), "%d 1", playerid);
	return cmd_food(playerid, param);
}

CMD:food(playerid, params[])
{
	new targetid, all, string[120];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "dD(0)", targetid, all))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /food [id gracza]");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Gracz nie jest pod³¹czony.");

	if(all)
	{

		format(string, sizeof(string), "Administrator {b}%s{/b} (ID: {b}%d{/b}) zmniejszy³ wszystkim g³ód.", PlayerName(playerid), playerid);
		MsgToAll(COLOR_INFO, string);
	}
	else
	{

		format(string, sizeof(string), "Administrator {b}%s{/b} (ID: {b}%d{/b}) zmniejszy³ Ci g³ód.", PlayerName(playerid), playerid);
		Msg(targetid, COLOR_INFO, string);

		format(string, sizeof(string), "Zmniejszy³eœ g³ód graczowi {b}%s{/b} (ID: {b}%d{/b}).", PlayerName(targetid), targetid);
		Msg(playerid, COLOR_INFO, string);
	}
	return 1;
}

CMD:remove(playerid, params[])
{
	new targetid, string[100];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "d", targetid)) 
		return Msg(playerid, COLOR_ERROR, "Wpisz: /remove [id gracza]");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Gracz nie jest pod³¹czony.");

	if(!IsPlayerInAnyVehicle(targetid))
		return Msg(playerid, COLOR_ERROR, "Gracz nie jest w pojeŸdzie.");

	RemovePlayerFromVehicle(targetid);

	format(string, sizeof(string), "Administrator {b}%s{/b} (ID: {b}%d{/b}) wyrzuci³ ciê z pojazdu.", PlayerName(playerid), playerid);
	Msg(targetid, COLOR_INFO, string);

	format(string, sizeof(string), "Wyrzuci³eœ z pojazdu {b}%s{/b} (ID: {b}%d{/b}).", PlayerName(targetid), targetid);
	Msg(playerid, COLOR_INFO, string);
	
	return 1;
}

CMD:createveh(playerid, params[])
{
	new vehicleID, vehicleModel, vehicleColor[2], Float:vehiclePos[4], string[60];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(SpawnedAdminVehicles == MAX_ADMIN_VEHICLES)
		return Msg(playerid, COLOR_ERROR, "Nie mo¿na stworzyæ wiêcej pojazdów. Limit wyczerpany ({b}50{/b}).");	
	
	if(sscanf(params, "s[32]dd", string, vehicleColor[0], vehicleColor[1]))
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/createveh [cModel] [kolor 1] [kolor2]{/b}");

	vehicleModel = GetModelIdByName(string);

	if(vehicleModel < 0)
	{
		vehicleModel = strval(string);
		if(vehicleModel < 400 || vehicleModel > 611)
			return Msg(playerid, COLOR_ERROR, "Nieprawid³owy pojazd.");
	}

	if( (vehicleColor[0] < 0 || vehicleColor[0] > 255) || (vehicleColor[1] < 0 || vehicleColor[1] > 255) )
		return Msg(playerid, COLOR_ERROR, " Nieprawid³owy kolor (0-255)");

	for(new i; i < MAX_ADMIN_VEHICLES; i++)
		if(!IsValidVehicle(AdminsVehs[i]))
		{
			vehicleID = i;
			break;
		}

	GetPlayerPos(playerid, vehiclePos[0], vehiclePos[1], vehiclePos[2]);
	GetPlayerFacingAngle(playerid, vehiclePos[3]);

	AdminsVehs[vehicleID] = AddStaticVehicleEx(vehicleModel, vehiclePos[0], vehiclePos[1], vehiclePos[2], vehiclePos[3], vehicleColor[0], vehicleColor[1], -1);
	SpawnedAdminVehicles++;

	Spawned[AdminsVehs[vehicleID]]=true;

	vehInfo_Temp[AdminsVehs[vehicleID]][vFuel] = MaxFuel(GetVehicleModel(vehicleID));
	vehInfo_Temp[AdminsVehs[vehicleID]][vFuelType] = IsVehicleTruck(vehicleID) ? FUEL_TYPE_TDI : FUEL_TYPE_BENZ;
	vehInfo_Temp[AdminsVehs[vehicleID]][vPrzebieg] = -2.0;
	vehInfo_Temp[AdminsVehs[vehicleID]][Color1] = vehicleColor[0]; 
	vehInfo_Temp[AdminsVehs[vehicleID]][Color2] = vehicleColor[1]; 

	format(string, sizeof(string), "Stworzy³eœ pojazd o {b}%s{/b} (ID: {b}%d{/b}).", GetVehicleModelName(vehicleModel), vehicleID);
	Msg(playerid, COLOR_INFO, string);
	PutPlayerInVehicle(playerid, AdminsVehs[vehicleID], 0);
	return 1;
}

CMD:destroyvehall(playerid, params[])
	return cmd_destroyveh(playerid, "0 1");

CMD:destroyveh(playerid, params[])
{
	new vehicleID, all, string[60];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "dD(0)", vehicleID, all))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /destroyveh [id pojazdu]");

	if(!AdminsVehs[vehicleID] && !all)
		return Msg(playerid, COLOR_ERROR, "Dany pojazd nie istnieje.");

	if(all)
	{
		for(new i; i < MAX_ADMIN_VEHICLES; i++)
		{
			if(IsValidVehicle(AdminsVehs[i]))
				DestroyVehicle(AdminsVehs[i]);

			AdminsVehs[i] = INVALID_VEHICLE_ID;
		}

		SpawnedAdminVehicles = 0;
		SendClientMessage(playerid, 0x0, "Zniszczy³eœ wsystkie pojazdy adminów.");
	}
	else
	{
		DestroyVehicle(AdminsVehs[vehicleID]);
		AdminsVehs[vehicleID] = INVALID_VEHICLE_ID;
		SpawnedAdminVehicles--;

		format(string, sizeof(string), "Zniszczy³eœ pojazd o ID: %d", vehicleID);
		Msg(playerid, COLOR_INFO, string);
	}

	return 1;
}

CMD:put(playerid, params[])
{
	new vehiclePlayerID, secondPlayerID, seat, string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "ddd", secondPlayerID, vehiclePlayerID, seat))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /put [id gracza] [id gracza] [miejsce>1]");

	if(!IsPlayerConnected(vehiclePlayerID) || !IsPlayerConnected(secondPlayerID))
		return Msg(playerid, COLOR_ERROR, "Gracz nie jest na serwerze.");

	if(seat < 1)
		return Msg(playerid, COLOR_ERROR, "Nieprawid³owe miejsce.");

	if(!IsPlayerInAnyVehicle(vehiclePlayerID))
		return Msg(playerid, COLOR_ERROR, "Gracz nie jest w pojeŸdzie.");

	RemovePlayerFromVehicle(secondPlayerID);
	PutPlayerInVehicle(secondPlayerID, GetPlayerVehicleID(vehiclePlayerID), seat);

	format(string, sizeof(string), "Administrator {b}%s{/b} (ID: %d) wrzuci³ ciê do innego pojazdu.", PlayerName(playerid), playerid);
	Msg(secondPlayerID, COLOR_ERROR, string);
	format(string, sizeof(string), "Wrzuci³eœ {b}%s{/b} (ID: %d) do innego pojazdu.", PlayerName(secondPlayerID), secondPlayerID);
	Msg(playerid, COLOR_ERROR, string);
	format(string, sizeof(string), "Administrator {b}%s{/b} (ID: %d) wrzuci³ {b}%s{/b} (ID: %d) do Twojego pojazdu.", PlayerName(playerid), playerid, PlayerName(secondPlayerID), secondPlayerID);
	Msg(playerid, COLOR_ERROR, string);

	return 1;
}

CMD:restart(playerid, params[])
{
	if(!HasPlayerFullPermission(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ ({b}specjalne{/b}).");

	new string[176];
	format(string, sizeof string, "Administrator {b}%s{/b} (ID: %d) zrestartowa³ serwer.", PlayerName(playerid), playerid);
	MsgToAll(COLOR_INFO, string);
	SendRconCommand("gmx");
	return 1;
}

CMD:cd(playerid, params[]) 
{
	new string[96], czas;

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

    if(sscanf(params, "dD(0)", czas, gmInfo[gmCountdownFreeze]))
    	return Msg(playerid, COLOR_ERROR, "Wpisz: /cd [czas] [freeze]");

    if(czas > 100)
    	return Msg(playerid, COLOR_ERROR, "Limit na 100 sekund.");

	if(gmInfo[gmCountdownStarted] == false)
	{
     	gmInfo[gmCountdown] = strval(params);
    	gmInfo[gmCountdownTimer] = SetTimer("CountDown", 1000, 1);
    	gmInfo[gmCountdownStarted] = true;
            
        format(string, sizeof(string), "Administrator {b}%s{/b} (ID: %d) wystartowa³ odliczanie!", PlayerName(playerid), playerid);
        MsgToAll(COLOR_INFO2, string);

        if(gmInfo[gmCountdownFreeze])
        	Loop(player, MAX_PLAYERS)
        		if(IsPlayerConnected(player))
        			TogglePlayerControllable(player, 0);
    }
	else
         Msg(playerid, COLOR_ERROR, "Ju¿ odlicza.");

    return 1;
}  

CMD:freezeall(playerid, params[])
	return cmd_freeze(playerid, "0 1");

CMD:freeze(playerid, params[])
{
	new targetid, all, string[96];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "dD(0)", targetid, all))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /freeze [id gracza]");

	if(!IsPlayerConnected(targetid) && !all)
		return Msg(playerid, COLOR_ERROR, "Gracz nie jest online.");

	if(all)
	{
		Loop(player, MAX_PLAYERS)
			if(IsPlayerConnected(player))
				TogglePlayerControllable(player, 0);

		format(string, sizeof(string), "Administrator {b}%s{/b} (ID: %d) zamrozi³ wszystkich graczy.", PlayerName(playerid), playerid);
		MsgToAll(COLOR_INFO, string);
	}
	else
	{
		TogglePlayerControllable(targetid, 0);

		format(string, sizeof(string), "Administrator {b}%s{/b} (ID: %d) zamrozi³ Ciê.", PlayerName(playerid), playerid);
		Msg(targetid, COLOR_INFO, string);
		format(string, sizeof(string), "Zamrozi³eœ {b}%s{/b} (ID: %d).", PlayerName(targetid), targetid);
		Msg(playerid, COLOR_INFO, string);
	}

	return 1;
}

CMD:unfreezeall(playerid, params[])
	return cmd_unfreeze(playerid, "0 1");

CMD:unfreeze(playerid, params[])
{
	new targetid, all, string[96];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "dD(0)", targetid, all))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /unfreeze [id gracza]");

	if(!IsPlayerConnected(targetid) && !all)
		return Msg(playerid, COLOR_ERROR, "Gracz nie jest online.");

	if(all)
	{
		Loop(player, MAX_PLAYERS)
			if(IsPlayerConnected(player))
				TogglePlayerControllable(player, 1);

		format(string, sizeof(string), "Administrator {b}%s{/b} (ID: %d) odmrozi³ wszystkich graczy.", PlayerName(playerid), playerid);
		MsgToAll(COLOR_INFO, string);
	}
	else
	{
		TogglePlayerControllable(targetid, 1);

		format(string, sizeof(string), "Administrator {b}%s{/b} (ID: %d) odmrozi³ Ciê.", PlayerName(playerid), playerid);
		Msg(targetid, COLOR_INFO, string);
		format(string, sizeof(string), "Odmrozi³eœ {b}%s{/b} (ID: %d).", PlayerName(targetid), targetid);
		Msg(playerid, COLOR_INFO, string);
	}

	return 1;
}


CMD:killp(playerid, params[])
{
	new targetid, string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "d", targetid))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /killp [id gracza]");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie jest obecny na serwerze.");

	SetPlayerHealth(targetid, 0);
	format(string, sizeof string, "Zabi³eœ gracza {b}%s{/b} (ID: %d).", PlayerName(targetid), targetid);
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "Administrator {b}%s{/b} (ID: %d) zabi³ ciê.", PlayerName(playerid), playerid);
	Msg(targetid, COLOR_INFO, string);
	return 1;
}

CMD:colorplayer(playerid, params[])
{
	new targetid, kolor[16], string[120], colorId;

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "ds[16]", targetid, kolor))
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/colorplayer [id gracza] [kolor]{/b}");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Gracza z tym ID nie ma online.");

	colorId = -1;
	for(new i = 0; i < sizeof colors; i++)
	{
		if(strfind(colors[i], kolor, true) != -1)
		{
			colorId = i;
			break;
		}
	}
	if(colorId < 0)
	{
		colorId = strval(kolor);
		if(colorId > 9 || colorId < 0)
			return Msg(playerid, COLOR_ERROR, "Nie odnaleziono takiego koloru.");
	}

	switch(colorId)
	{
		case 0: SetPlayerColor(targetid, 0xFFFFFF00); // niewidoczny
		case 1: SetPlayerColor(targetid, 0xFFFFFFFF); // bia³y
		case 2: SetPlayerColor(targetid, 0xFF9900FF); // pomarañczowy
		case 3: SetPlayerColor(targetid, 0xFF0000FF); // czerwony
		case 4: SetPlayerColor(targetid, 0xFF00FFFF); // ró¿owy
		case 5: SetPlayerColor(targetid, 0xFFFF00FF); // ¿ó³ty
		case 6: SetPlayerColor(targetid, 0x663300FF); // br¹zowy
		case 7: SetPlayerColor(targetid, 0x009933FF); // zielony
		case 8: SetPlayerColor(targetid, 0x00FFFFFF); // jasnoniebieski
		case 9: SetPlayerColor(targetid, 0x0000CCFF); // niebieski
	}

	format(string, sizeof string, "Zmieni³eœ kolor graczowi {b}%s{/b} na {b}%s{/b}.", PlayerName(targetid), colors[colorId]);
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "Administrator {b}%s{/b} zmieni³ Ci kolor na {b}%s{/b}.", PlayerName(playerid), colors[colorId]);
	Msg(targetid, COLOR_INFO, string);
	return 1;
}

CMD:dodajadmin(playerid, params[])
{
	new targetid, poziom, string[128];

	if(!HasPlayerFullPermission(playerid) && playerInfo[playerid][pAdmin] != 2)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "dd", targetid, poziom))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /dodajadmin [id] [poziom]");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Gracz nie jest na serwerze.");

	playerInfo[targetid][pAdminAllowed] = poziom;
	format(string, sizeof string, "Gracz {b}%s{/b} zosta³ dodany do grupy administratorów.", PlayerName(targetid));
	Msg(playerid, COLOR_INFO, string);

	mysql_real_escape_string(PlayerName(targetid), string);
	format(string, sizeof string, "UPDATE `Accounts` SET `Admin` = '%d' WHERE `Name` = '%s'", poziom, string);
	mysql_query(string);
	return 1;
}

CMD:usunadmin(playerid, params[])
{
	new targetid, string[128];

	if(!HasPlayerFullPermission(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "d", targetid))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /usunadmin [id]");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Gracz nie jest na serwerze.");

	playerInfo[playerid][pAdmin] = 0;
	format(string, sizeof string, "Gracz {b}%s{/b} zosta³ usuniêty z grupy administratorów.", PlayerName(targetid));
	Msg(playerid, COLOR_INFO, string);
	
	return 1;
}

CMD:setname(playerid, params[])
{
	new targetid, nick[20], string[128];

	if(playerInfo[playerid][pAdmin] < 2)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "ds[20]", targetid, nick))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /setname [id gracza] [nick]");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Gracz nie jest pod³¹czony.");

	format(string, sizeof string, "UPDATE `Accounts` SET `Name` = '%s' WHERE `Name` = '%s'", nick, PlayerName(targetid));
	mysql_query(string);

	format(string, sizeof string, "Administrator {b}%s{/b} (ID: %d) zmieni³ ci nick na %s.", PlayerName(playerid), playerid, nick);
	Msg(targetid, COLOR_INFO, string);
	format(string, sizeof string, "Zmieni³eœ graczowi nick z {b}%s{/b} (ID: %d) na %s.", PlayerName(targetid), playerid, nick);
	Msg(targetid, COLOR_INFO, string);

	SetPlayerName(targetid, nick);
	return 1;
}

CMD:respawniko(playerid, params[])
{
	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	new bool:Uzywany[MAX_VEHICLES] = false,v;

	for(new i = 0; i < GetMaxPlayers(); i++)
	{
		if(IsPlayerInAnyVehicle(i))
		{
			v = GetPlayerVehicleID(i);
			Uzywany[v] = true;

			if(IsTrailerAttachedToVehicle(v)) 
				Uzywany[GetVehicleTrailer(v)] = true;
		}
	}

	for(new nr = 1; nr < GetVehiclePoolSize(); nr++)
		if(Uzywany[nr] == false)
			SetVehicleToRespawn(nr);

	MsgToAll(COLOR_INFO, "Wszystkie pojazdy powróci³y na miejsce spawnu.");
	return 1;
}

CMD:respawn(playerid, params[])
{
	return cmd_respawniko(playerid, params);
}

CMD:dodajstacje(playerid, params[])
{
	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	new string[128];
	new Float:Pos[3];
	GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);

	format(string, sizeof string, "INSERT INTO `Stacje` (`PosX`, `PosY`, `PosZ`) VALUES('%f', '%f', '%f')", Pos[0], Pos[1], Pos[2]);
	mysql_query(string);

	stationInfo[ID_STACJA][sPosX] = Pos[0];
	stationInfo[ID_STACJA][sPosY] = Pos[1];
	stationInfo[ID_STACJA][sPosZ] = Pos[2];

	format(string, sizeof string, "Stacja paliwowa w {b}%s{/b} (ID: {b}%d{/b})\nAby zatankowaæ wpisz {b}/tankuj{/b}", GetPointZone(stationInfo[ID_STACJA][sPosX], stationInfo[ID_STACJA][sPosY]), ID_STACJA);
	StacjeText3D[ID_STACJA] = CreateDynamic3DTextLabel(string, ZIELONY2, Pos[0], Pos[1], Pos[2]+0.5, 30.0);
	
	StacjeMapIcon[ID_STACJA] = CreateDynamicMapIcon(stationInfo[ID_STACJA][sPosX], stationInfo[ID_STACJA][sPosY], stationInfo[ID_STACJA][sPosZ], 55, 0, _, _, _, 300.0);

	format(string, sizeof string, "Stacja o ID {b}%d{/b} zosta³a pomyœlnie utworzona.", ID_STACJA);
	Msg(playerid, COLOR_INFO, string);
	ID_STACJA++;
	return 1;
}

CMD:usunstacje(playerid, params[])
{
	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	new string[132];

	new stacjaid = GetStacja(playerid);
	if(stacjaid == 0)
		return Msg(playerid, COLOR_ERROR, "Nie znajdujesz siê przy stacji.");

	format(string, sizeof string, "Stacja o ID {b}%d{/b} zosta³a pomyœlnie usuniêta.", stacjaid);
	Msg(playerid, COLOR_INFO, string);

	DestroyDynamic3DTextLabel(StacjeText3D[stacjaid]);
	DestroyDynamicMapIcon(StacjeMapIcon[stacjaid]);

	format(string, sizeof string, "DELETE FROM `Stacje` WHERE `ID`= '%d'", stacjaid);
	mysql_query(string);
	return 1;
}

CMD:pogoda(playerid, params[])
{
	new string[90];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	new weatherid;
	if(sscanf(params, "d", weatherid))
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/pogoda [id]{/b}."); 

	if(weatherid < 0 || weatherid > 20)
		return Msg(playerid, COLOR_ERROR, "Nieprawid³owe ID pogody {b}(0-20){/b}.");

	SetWeather(weatherid);

	format(string, sizeof string, "Pogoda zosta³a zmieniona przez {b}%s{/b} na pogodê ID: {b}%d{/b}.", PlayerName(playerid), weatherid);
	MsgToAll(COLOR_INFO2, string);
	return 1;
}

CMD:ustawlidera(playerid, params[])
{
	new targetid, firmaid, string[100];
	
	if(playerInfo[playerid][pAdmin] != 2)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "dd", targetid, firmaid))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /ustawlidera [id gracza] [id firmy]");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Gracz nie jest pod³¹czony.");

	if(firmInfo[firmaid][tID] == 0)
		return Msg(playerid, COLOR_ERROR, "B³êdne ID firmy.");

	firmInfo[firmaid][tChef] = playerInfo[targetid][pID];
	playerInfo[targetid][pFirm] = firmaid;
	
	format(string, sizeof(string), "UPDATE `Firmy` SET `Szef` = '%d' WHERE `id` = '%d'", playerInfo[targetid][pID], firmaid);
	mysql_query(string);

	format(string, sizeof(string), "Administrator {b}%s{/b} zarz¹dzi³ Ciê szefem firmy {b}%s{/b}.", PlayerName(playerid), firmInfo[firmaid][tName]);
	Msg(targetid, COLOR_INFO, string);
	
	format(string, sizeof(string), "{b}%s{/b} sta³ siê szefem firmy {b}%s{/b}.", PlayerName(targetid), firmInfo[firmaid][tName]);
	Msg(playerid, COLOR_INFO, string);
	
	return 1;
}

CMD:dodajpojazd(playerid, params[])
{
	new string[500], vehicleid = GetPlayerVehicleID(playerid), Float:Pos[4], firmaid, naczepa;
	
	if(playerInfo[playerid][pAdmin] != 2)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "dD(0)", firmaid, naczepa))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /dodajpojazd [id firmy]");

	if(!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ w pojeŸdzie.");

	if(naczepa)
	{
		if(IsTrailerAttachedToVehicle(vehicleid))
			vehicleid = GetVehicleTrailer(vehicleid);
		else
			return  Msg(playerid, COLOR_ERROR, "Nie masz pod³¹czonej naczepy.");
	}

	if(!Spawned[vehicleid])
		return Msg(playerid, COLOR_ERROR, "Stwórz pojazd poprzez /createveh");

	GetVehiclePos(vehicleid, Pos[0], Pos[1], Pos[2]);
	GetVehicleZAngle(vehicleid, Pos[3]);

	format(string, sizeof(string), "INSERT INTO `Pojazdy` (`model`, `owner_id`, `owner_type`, `PosX`, `PosY`, `PosZ`, `PosA`, `Health`, `Fuel`, `FuelType`, `Color1`, `Color2`, `Plate`, `Przeglad`, `gasBootle`, `gasAmount`) VALUES ('%d', '%d', '1', '%f', '%f', '%f', '%f', '1000.0', '%d', '%d', '%d', '%d', 'Rej. Cipa', NOW(), '0', '0.0')", GetVehicleModel(vehicleid), firmaid, Pos[0], Pos[1], Pos[2], Pos[3], MaxFuel(GetVehicleModel(vehicleid)), 
			(IsVehicleTruck(vehicleid) ? FUEL_TYPE_TDI : FUEL_TYPE_BENZ), vehInfo_Temp[vehicleid][Color1], vehInfo_Temp[vehicleid][Color2]);
	mysql_query(string);

	DestroyVehicle(vehicleid);
	Spawned[vehicleid] = false;

	LadujPojazd(_, _, mysql_insert_id());
	return 1;
}

CMD:dodajpracownika(playerid, params[])
{
	new targetid, firmaid, string[80];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "dd", targetid, firmaid))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /dodajpracownika [graczid] [firmaid]");

	if(firmaid > 7 || firmaid < 0)
		return Msg(playerid, COLOR_ERROR, "ID firmy musi byæ w przedziale 0-7.");

	playerInfo[targetid][pFirm] = firmaid;

	format(string, sizeof(string), "Administrator {b}%s{/b} przydzieli³ Ciê do firmy {b}%s{/b}.", PlayerName(playerid), firmInfo[firmaid][tName]);
	Msg(targetid, COLOR_INFO, string);
	
	format(string, sizeof(string), "Przydzieli³eœ {b}%s{/b} do firmy {b}%s{/b}.", PlayerName(targetid), firmInfo[firmaid][tName]);
	Msg(playerid, COLOR_INFO, string);
	return 1;
}

CMD:usunpracownika(playerid, params[])
{
	new targetid, string[80];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "d", targetid))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /usunpracownika [graczid]");
		
	playerInfo[targetid][pFirm] = 0;
	format(string, sizeof(string), "Administrator {b}%s{/b} wyrzuci³ Ciê z firmy.", PlayerName(playerid));
	Msg(targetid, COLOR_INFO, string);
	
	format(string, sizeof(string), "Wyrzuci³eœ {b}%s{/b} z firmy.", PlayerName(targetid));
	Msg(playerid, COLOR_INFO, string);
	return 1;
}

CMD:heal(playerid, params[])
{
	new targetid, string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "d", targetid))
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/heal [id gracza]{/b}");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie jest obecny na serwerze.");

	SetPlayerHealth(targetid, 100.0);
	format(string, sizeof string, "Uzdrowi³eœ gracza {b}%s{/b}.", PlayerName(targetid));
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "Administrator {b}%s{/b} uzdrowi³ Ciê.", PlayerName(playerid));
	Msg(targetid, COLOR_INFO, string);
	return 1;
}

CMD:armor(playerid, params[])
	return cmd_armour(playerid, params);

CMD:armour(playerid, params[])
{
	new targetid, string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "d", targetid))
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/armour [id gracza]{/b}");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie jest obecny na serwerze.");

	SetPlayerArmour(targetid, 100.0);
	format(string, sizeof string, "Da³eœ pancerz graczowi {b}%s{/b}.", PlayerName(targetid));
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "Administrator {b}%s{/b} da³ Ci pancerz.", PlayerName(playerid));
	Msg(targetid, COLOR_INFO, string);
	return 1;
}

CMD:setskin(playerid, params[])
{
	new targetid, skin, string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "dd", targetid, skin))
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/setskin [id gracza] [id skinu 0-311]{/b}");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Ten gracz nie jest obecny na serwerze.");

	if(skin < 0 || skin > 311)
		return Msg(playerid, COLOR_ERROR, "B³êdne ID skina.");

	SetPlayerSkin(targetid, skin);

	format(string, sizeof string, "Zmieni³eœ skin graczowi {b}%s{/b}.", PlayerName(targetid));
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "Administrator {b}%s{/b} zmieni³ Ci skin na {b}%d{/b}.", PlayerName(playerid), skin);
	Msg(targetid, COLOR_INFO, string);
	return 1;
}

CMD:dodajzaladunek(playerid, params[])
{
	new type[15], typeid;
	if(sscanf(params, "s[15]", type))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /dodajzaladunek [budowlany/odzie¿owy/elektroniczny/spo¿ywczy/niebezpieczny/nielegalny]");

	if(!strcmp(type, "budowlany"))
	{
		typeid = TYPE_BUILD;
	}
	else if(!strcmp(type, "odzie¿owy"))
	{
		typeid = TYPE_CLOTHES;
	}
	else if(!strcmp(type, "elektroniczny"))
	{
		typeid = TYPE_ELECTRONIC;
	}
	else if(!strcmp(type, "spo¿ywczy"))
	{
		typeid = TYPE_EAT;
	}
	else if(!strcmp(type, "niebezpieczny"))
	{
		typeid = TYPE_DANGEROUS;
	}
	else if(!strcmp(type, "nielegalny"))
	{
		typeid = TYPE_ILLEGAL;
	}
	else 
		return Msg(playerid, COLOR_ERROR, "Wpisz: /dodajzaladunek [budowlany/odzie¿owy/elektroniczny/spo¿ywczy/niebezpieczny/nielegalny]");

	new Float:pos[3], szString[256], baseid, load = -1;
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	baseid = GetClosestBase(pos[0], pos[1], pos[2]);
	for(new i = 0; i < MAX_LOADINGS; i++)
	{
		if(IsValidLoad(i))
			continue;

		load = i;
		break;
	}
	if(load < 0)
		return Msg(playerid, COLOR_ERROR, "Nie mo¿na dodaæ wiêcej za³adunków.");
	GetPlayerPos(playerid, loadInfo[load][lX], loadInfo[load][lY], loadInfo[load][lZ]);
	loadInfo[load][LID] = load; loadInfo[load][lType] = typeid; loadInfo[load][lBaza] = baseid;
	format(szString, sizeof szString, "INSERT INTO loads (LID, lX, lY, lZ, lType, lBase) VALUES ('null', '%.04f', '%.04f', '%.04f', '%d', '%d')", loadInfo[load][lX], loadInfo[load][lY], loadInfo[load][lZ], typeid, baseid);
	mysql_query(szString);
	
	format(szString, sizeof szString, "Za³adunek towarów {b}%s{/b} w {b}%s{/b} (ID: {b}%d{/b})\nAby za³adowaæ towar wpisz {b}/zaladuj{/b}\nAby roz³adowaæ towar wpisz {b}/rozladuj{/b}", GetLoadTypeName(loadInfo[load][lType]), GetPointZone(loadInfo[load][lX], loadInfo[load][lY]), loadInfo[load][LID]);
	loadInfo[load][lText] = CreateDynamic3DTextLabel(clText(COLOR_INFO2, szString), -1, loadInfo[load][lX], loadInfo[load][lY], loadInfo[load][lZ]+1.0, 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1);
	loadInfo[load][lMapIcon] = CreateDynamicMapIcon(loadInfo[load][lX], loadInfo[load][lY], loadInfo[load][lZ], 51, 0);
	loadInfo[load][lMapIconForAll] = CreateDynamicMapIcon(loadInfo[load][lX], loadInfo[load][lY], loadInfo[load][lZ], 51, 0, _, _, GetMaxPlayers()+1, 6000.0);
	Streamer_SetIntData(STREAMER_TYPE_MAP_ICON, loadInfo[load][lMapIconForAll], E_STREAMER_STYLE, MAPICON_GLOBAL);
	ID_DOKOW=load;
	systemInfo[0] += 1;
	Msg(playerid, COLOR_INFO, "Pomyœlnie utworzono za³adunek.");
	return 1;
}

CMD:za(playerid, params[])
	return cmd_alogin(playerid, params);

CMD:alogin(playerid, params[])
{
	new inputpass[32];
	if(sscanf(params, "s[32]", inputpass)) return Msg(playerid, COLOR_ERROR, "/(za) - (alogin) [has³o]");
	if(playerInfo[playerid][pAdmin] > 0) return Msg(playerid, COLOR_INFO, "Jesteœ ju¿ zalogowany jako administrator.");

	if(playerInfo[playerid][pAdminAllowed] < 1)
	{
		Msg(playerid, COLOR_ERROR, "Nie mo¿esz tego zrobiæ.");
		
		SetPVarInt(playerid, "Warn", GetPVarInt(playerid, "Warn") + 1);
		if(GetPVarInt(playerid, "Warn") == 3)
		{
			CheatKick(playerid, "trzy ostrze¿enia");
			timer[playerid] = SetTimerEx("Kickplayer", 500, 0, "d", playerid);
		}
		return 1;
	}
	else
	{
		if(!strcmp(inputpass, gmInfo[adminPass], false))
		{
			Msg(playerid, COLOR_INFO, "Pomyœlnie zalogowa³eœ siê jako administrator.");
			playerInfo[playerid][pAdmin] = playerInfo[playerid][pAdminAllowed];
			UpdateAdminsOnline();
		}
		else
		{
			SetPVarInt(playerid, "Warn", GetPVarInt(playerid, "Warn") + 1);
			if(GetPVarInt(playerid, "Warn") == 3)
			{
				CheatKick(playerid, "trzy ostrze¿enia");
				timer[playerid] = SetTimerEx("Kickplayer", 500, 0, "d", playerid);
			}

			Msg(playerid, COLOR_ERROR, "B³êdne has³o, chcesz zgadywaæ dalej?");
		}
	}
	return 1;
}

CMD:setspawn(playerid)
{
	if(GetPVarInt(playerid, "NEWBIE_SETSPAWN"))
		Dialog_Show(playerid, DIALOG_SETSPAWN, DIALOG_STYLE_LIST, "Wybierz spawn", "Los Santos\nLas Venturas\nRed County\nSan Fierro", "Wybierz", #);
	else
	{
		new szContent[128];
		format(szContent, sizeof szContent, "%sLos Santos\n%sLas Venturas\n%sRed County\n%sSan Fierro\n%sW³asny spawn", 
			(playerInfo[playerid][pSpawn] == 0) ? ("{33CC66}") : (""),
			(playerInfo[playerid][pSpawn] == 1) ? ("{33CC66}") : (""),
			(playerInfo[playerid][pSpawn] == 2) ? ("{33CC66}") : (""),
			(playerInfo[playerid][pSpawn] == 3) ? ("{33CC66}") : (""),
			(playerInfo[playerid][pSpawn] == -1) ? ("{33CC66}") : ("")
		);
		Dialog_Show(playerid, DIALOG_SETSPAWN, DIALOG_STYLE_LIST, "Wybierz spawn", szContent, "Wybierz", "Anuluj");
	}
	return 1;
}

Dialog:DIALOG_SETSPAWN(playerid, response, listitem, inputtext[])
{
	if(!response && !GetPVarInt(playerid, "NEWBIE_SETSPAWN"))
		return 1;
	else if(!response && !GetPVarInt(playerid, "NEWBIE_SETSPAWN"))
		return cmd_setspawn(playerid);

	switch(listitem)
	{
		case 0: playerInfo[playerid][pSpawn] = 0;
		case 1: playerInfo[playerid][pSpawn] = 1;
		case 2: playerInfo[playerid][pSpawn] = 2;
		case 3: playerInfo[playerid][pSpawn] = 3;
		case 4: playerInfo[playerid][pSpawn] = -1;
	}
	if(!GetPVarInt(playerid, "NEWBIE_SETSPAWN"))
	{
		if(playerInfo[playerid][pSpawn] < 0)
		{
			new Float:pos[4];
			GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
			GetPlayerFacingAngle(playerid, pos[3]);
			format(playerInfo[playerid][pSpawnInfo], 32, "%.02f,%.02f,%.02f,%.02f", pos[0], pos[1], pos[2], pos[3]);
		}
		cmd_setspawn(playerid);
		Msg(playerid, COLOR_INFO, "Pomyœlnie zmieniono {b}startowy{/b} spawn.");
	}
	else
	{
		DeletePVar(playerid, "NEWBIE_SETSPAWN");
		SetTimerEx("ReqSpawnPlayer", 100, false, "d", playerid);
		Msg(playerid, COLOR_INFO, "Pomyœlnie ustawiono {b}startowy{/b} spawn. Aby go zmieniæ u¿yj komendy {b}/setspawn{/b}.");
	}
	return 1;
}
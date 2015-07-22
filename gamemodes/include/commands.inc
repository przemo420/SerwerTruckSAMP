#include "zones"

// KOMENDY GRACZA
 /* 
 	- /pomoc /help /faq
	- /time /czas
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
	//strcat(s,""GCOLOR_YELLOW"IV. Jak uzupe³niæ pasek g³odu?\n");
	//strcat(s,""COLOR_GUI"Pasek g³odu uzupe³niamy jedz¹c posi³ki w barach(hamburger na mapie).\n");
	//strcat(s," \n");
	//strcat(s,""GCOLOR_YELLOW"V. Jak wyzerowaæ czas jazdy / jak zrobiæ pauzê?\n");
	//strcat(s,""COLOR_GUI"Pauzê robimy za pomoc¹ komendy /tachograf, po wpisaniu tej komendy do wyboru mamy dwie opcje a jedn¹ z nich jest pauza.\n");
	//strcat(s," \n");
	//strcat(s,""GCOLOR_YELLOW"VI. Jak kupiæ domek?\n");
	//strcat(s,""COLOR_GUI"JeÂ¿eli chcesz kupiæ dom jedziesz do jakiegokolwiek domku na mapce, zg³aszasz sie do administratora\n\t o wycenê - on stawia dom. Cennik domów znajduje siê na forum.\n");
	//strcat(s," \n");
	strcat(s,""GCOLOR_YELLOW"IV. Co zrobiæ gdy ktoœ utrudnia grê?\n");
	strcat(s,""COLOR_GUI"Sk³adamy na niego raport, u¿ywamy komendy /report. W razie spotkania z cheaterem - nie pisz na czacie.\n");
	strcat(s," \n");
	strcat(s,""GCOLOR_YELLOW"Mi³ej gry ¿yczy administracja.\n");
	
	return ShowPlayerDialog(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, "FAQ", s, "Zamknij", "");
}

CMD:time(playerid, params[])
	return cmd_czas(playerid, params);

CMD:czas(playerid, params[])
	return ShowPlayerDialog(playerid, GTIMEG_LIST_DIALOG, DIALOG_STYLE_LIST, " ", "Poranek.\nDzieñ.\nWieczór.\nNoc.", "OK", "Zamknij");

CMD:toll(playerid, params[])
{
	new string[128];

	if(!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie znajdujesz siê w pojeŸdzie.");

	if(PlayerVehicleIsTruck(playerid))
	{
		format(string, sizeof string, "{CC0000}>> {000040}Stan konta viaTOLL to: {FFFFFF}%d impulsy/ów.", GetViaMoney(playerid));
		SendClientMessage(playerid,0x0,string);
	}

	return 1;
}

CMD:animacje(playerid, cmdtext[])
	return cmd_anim(playerid, cmdtext);

CMD:animacja(playerid, cmdtext[])
	return cmd_anim(playerid, cmdtext);

CMD:anims(playerid, cmdtext[])
	return cmd_anim(playerid, cmdtext);

CMD:anim(playerid, cmdtext[])
	return ShowPlayerDialog(playerid, ANIM1_DIALOG, DIALOG_STYLE_LIST, " ", "Tañce >>\nRêce\nWino\nSprunk\nCygaro\nPiwo\nTelefon >>\nSiad\nDilowanie\nJedzenie\nWal siê\nPijany", "OK", "Dalej");

CMD:autor(playerid, params[])
	return cmd_autorzy(playerid, params);

CMD:autorzy(playerid, params[])
	return ShowPlayerDialog(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, " ", "Programiœci:\n- Maciek.\n- GeDox\n\nBeta-Testerzy:\n- [ST]Kill_Repeat\n- [ST][DJ]Bass\n- Reiban.\n\nTworzenie obiektów:\n- [ST][DJ]Bass\n- TDi\n- Laud.\n- Muscu\n- Marcin[WGM]\n- PolskiJankes\n- Kierowca\n- Kozak59\n- Devil", "Zamknij", "");

CMD:tel(playerid, params[])
	return cmd_telefon(playerid, params);

CMD:phone(playerid, params[])
	return cmd_telefon(playerid, params);

CMD:telefon(playerid, params[])
	return ShowPlayerDialog(playerid, DIALOG_ID_TELEPHONE, DIALOG_STYLE_LIST, " ", "Policja\nPogotowie\nPomoc Drogowa", "Wybierz", "Anuluj");

CMD:radio(playerid, params[])
{
	new string[1024];

	format(string, sizeof(string), "Wy³¹cz radio\n--\n");

	for(new i=0; i < sizeof(RadioStations); i++)
		if(RadioStations[i][sName])
			format(string, sizeof(string), "%s%s\n", string, RadioStations[i][sName]);

	ShowPlayerDialog(playerid, DIALOG_ID_RADIO, DIALOG_STYLE_LIST, " ", string, "OK", "Zamknij");

	return 1;
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
			ShowPlayerDialog(playerid, DIALOG_ID_CREATE_VEHICLE, DIALOG_STYLE_LIST, " ", "Pojazdy.\nNaczepy.\nUsuñ stworzone pojazdy.", "OK", "WyjdŸ");
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

CMD:via(playerid, params[])
	return cmd_viatoll(playerid, params);
	
CMD:viatoll(playerid, params[])
{
	new viashopid = GetViaShop(playerid);
	if(viashopid == INVALID_VIASHOP_ID)
		return ShowInfo(playerid, "Nie znajdujesz siê przy sklepie ViaToll.");

	ShowPlayerDialog(playerid, DIALOG_ID_VIATOLL, DIALOG_STYLE_LIST, "Sklep ViaToll", "Kup 1 impuls [$25]\nKup 2 impulsy [$50]\nKup 5 impulsów [$125]\nKup 10 impulsów [$250]\nKup 20 impulsów [$500]\nKup 50 impulsów [$1250]", "Ok", "Anuluj");
	return 1;
}

CMD:zapisz(playerid, params[])
	return SavePlayer(playerid);

//CMD:gps(playerid, params[])
	//return cmd_nawigacja(playerid, params);
CMD:nawigacjaNIEWLACZAC(playerid, params[])
{
	if(GetPVarType(playerid, "GPS_OstatniRaz") == PLAYER_VARTYPE_INT)
		if( (GetTickCount() - GetPVarInt(playerid, "GPS_OstatniRaz"))/1000 < 1.0 )
			return ShowInfo(playerid, "Odczekaj sekundê przed nastêpn¹ aktualizacj¹ GPSa.");

	ShowPlayerDialog(playerid, DIALOG_ID_GPS, DIALOG_STYLE_LIST, "GPS System", "Doki towarowe\nStacje\nBary\nSklepy ViaToll\nWy³¹cz", "Wybierz", "");
	
	return 1;
}

CMD:firma(playerid, params[])
{
	if(!PlayerInfo[playerid][pFirma])
		return ShowInfo(playerid, "Nie pracujesz w ¿adnej firmie.");

	new string[300];
	strcat(string, "Informacje\nPracownicy online\nWp³aæ pieni¹dze");

	if(Firmy[PlayerInfo[playerid][pFirma]][tChef] == PlayerInfo[playerid][pID] || Firmy[PlayerInfo[playerid][pFirma]][tVcChef] == PlayerInfo[playerid][pID] || Firmy[PlayerInfo[playerid][pFirma]][tChef] == PlayerInfo[playerid][pID] || Firmy[PlayerInfo[playerid][pFirma]][tKsiegowy] == PlayerInfo[playerid][pID])
		strcat(string, "\n----\nRozdaj wyp³aty");

	if(Firmy[PlayerInfo[playerid][pFirma]][tChef] == PlayerInfo[playerid][pID] || Firmy[PlayerInfo[playerid][pFirma]][tVcChef] == PlayerInfo[playerid][pID])
		strcat(string, "\nZaproœ gracza\nWyrzuæ gracza\nZastêpca\nKsiêgowy\nUstaw spawn\nWyp³aæ pieni¹dze\nZmieñ skina");

	ShowPlayerDialog(playerid, DIALOG_ID_FIRMA, DIALOG_STYLE_LIST, "Firma", string, "Dalej", "Anuluj");
	return 1;
}

CMD:stats(playerid, params[])
{
	new string[500], czasBierzacy[3];
	format(string, sizeof(string), "UID:\t %d\n", PlayerInfo[playerid][pID]);
	format(string, sizeof(string), "%sNick:\t %s\n", string, PlayerName(playerid));
	format(string, sizeof(string), "%sPieni¹dze:\t %d\n", string, GetMoney(playerid));
	format(string, sizeof(string), "%sBankomat:\t %d\n", string, PlayerInfo[playerid][pBankomat]);
	format(string, sizeof(string), "%sPunkty:\t %d\n", string, GetScore(playerid));
	format(string, sizeof(string), "%sPoziom:\t %d\n", string, GetPVarInt(playerid, "LEVEL"));
	format(string, sizeof(string), "%sPoziom uprawnieñ:\t %d\n", string, PlayerInfo[playerid][pAdmin]);
	format(string, sizeof(string), "%sFirma:\t %d\n", string, PlayerInfo[playerid][pFirma]);
	format(string, sizeof(string), "%sSkin:\t %d\n", string, PlayerInfo[playerid][pSkin]);
	format(string, sizeof(string), "%sTachograf:\t ---\n", string);
	format(string, sizeof(string), "%sViaToll:\t %d\n", string, GetViaMoney(playerid));
	format(string, sizeof(string), "%sADR:\t BRAK\n", string);
	format(string, sizeof(string), "%sCzas pracy:\t %dh\n", string, GetWork(playerid));
	format(string, sizeof(string), "%sBuildMaster:\t %d\n", string, PlayerInfo[playerid][pBuildmaster]);

	ConvertMS(GetTickCount()-GetPVarInt(playerid, "IleGral"), czasBierzacy[0], czasBierzacy[1], czasBierzacy[2]);

	format(string, sizeof(string), "%sCzas online (bie¿¹cy):\t %dh %dm %ds\n", string, czasBierzacy[0], czasBierzacy[1], czasBierzacy[2]);
	ShowPlayerDialog(playerid, NEVER_DIALOG, DIALOG_STYLE_TABLIST, "Statystyki gracza", string, "Ok", "");
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

	ShowPlayerDialog(playerid, DIALOG_ID_VEHICLE, DIALOG_STYLE_LIST, "Pojazd", string, "Ok", "Anuluj");
	return 1;
}

CMD:cmds(playerid)
{
	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	new frm = Firmy[PlayerInfo[playerid][pFirma]][tTyp];
	ShowPlayerDialog(playerid, DIALOG_CMDS, DIALOG_STYLE_MSGBOX, "Dostêpne komendy", szCommands[frm], "WyjdŸ", #);
	return 1;
}

// KOMENDY POLICJI
/*
	- /mandat
	- /aresztuj
	- /wywal /wyrzuc
	- /praca
	- /firma
	- /skuj
	- /rozkuj
	- /kontrola
	- /alkomat
*/

CMD:alkomat(playerid, params[])
{
	new targetid;

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_POLICE)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(sscanf(params, "d", targetid))
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/alkomat [id gracza]{/b}");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Gracza z tym ID nie ma online.");

	if(GetDistancePlayerToPlayer(playerid, targetid) > 20)
		return Msg(playerid, COLOR_ERROR, "Znajdujesz siê za daleko od gracza.");

	if(GetPlayerState(targetid) != PLAYER_STATE_DRIVER)
		return Msg(playerid, COLOR_ERROR, "Gracz z tym ID nie prowadzi pojazdu.");

	Msg(playerid, COLOR_INFO2, "Trwa sprawdzanie...");
	SetTimerEx("AlcoholTest", 5 * 1000, false, "dd", playerid, targetid);
	return 1;
}

CMD:mandat(playerid, params[])
{
	new forplayerid, monej, pojnts, powod[76];
	new string[256];

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_POLICE)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(sscanf(params, "ddds[76]", forplayerid, monej, pojnts, powod))
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/mandat [id gracza] [kwota] [punkty] [powod]{/b}");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Gracza z tym ID nie ma online.");

	if(GetDistancePlayerToPlayer(playerid, forplayerid)>20)
		return Msg(playerid, COLOR_ERROR, "Znajdujesz siê za daleko od gracza.");

	if(GetPlayerSpeed(playerid) > 0)
		return Msg(playerid, COLOR_ERROR, "Gracz {b}nie mo¿e siê{/b} poruszaæ.");

	if(monej < 0)
		return Msg(playerid, COLOR_ERROR, "Kwota mandatu jest {b}zbyt niska{/b}.");

	if(pojnts < 0)
		return Msg(playerid, COLOR_ERROR, "Iloœæ punktów jest {b}zbyt niska{/b}.");

	format(string,sizeof string,""GCOLOR_YELLOW"Mandat drogowy:\nKwota: {FFFFFF}$%d\n"GCOLOR_YELLOW"Punkty: {FFFFFF}%d\n"GCOLOR_YELLOW"Powód: {FFFFFF}%s\n"GCOLOR_YELLOW"Policjant: {FFFFFF}%s\n"GCOLOR_YELLOW"Przyjmujesz?",monej, pojnts, powod, PlayerName(playerid));
	SetPVarInt(forplayerid,"kontrola",playerid);
	SetPVarInt(forplayerid, "opcja1", monej);
	SetPVarInt(forplayerid, "opcja2", pojnts); 
	return ShowPlayerDialog(forplayerid, POLICE_DIALOG, DIALOG_STYLE_MSGBOX, " ", string, "Tak", "Nie");
}

CMD:aresztuj(playerid, params[])
{
	new forplayerid, czas, string[200];

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_POLICE)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(sscanf(params, "dd", forplayerid, czas))
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/aresztuj [id gracza] [czas]{/b}");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Gracza z tym ID nie ma online.");

	if(GetPVarInt(forplayerid, "Areszt"))
		return Msg(playerid, COLOR_ERROR, "Ten gracz {b}jest ju¿{/b} w areszcie.");

	SetPVarInt(forplayerid, "Areszt", 1);
	SetPVarInt(forplayerid, "Areszttime", czas*60);
	SetTimerEx("Unareszt", czas*(60), false, "d", forplayerid);
	format(string, sizeof string, "Aresztowa³eœ gracza {b}%s{/b} (ID: {b}%d{b}) na {b}%d{/b} minut.", PlayerName(forplayerid), forplayerid, czas);
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "Zosta³eœ aresztowany przez policjanta {b}%s{/b} (ID: {b}%d{/b}) na {b}%d{/b} minut(y).", PlayerName(playerid), playerid, czas);
	Msg(forplayerid, COLOR_INFO, string);

	SetPlayerInterior(forplayerid, 10);
	SetPlayerPos(forplayerid, 219.3761,109.7970,999.0156);
	SetPlayerHealth(forplayerid, 9999999);
	TogglePlayerControllable(forplayerid, 0);
	PlayerInfo[forplayerid][chained] = false;
	return 1;
}

CMD:wyrzuc(playerid, cmdtext[])
	return cmd_wywal(playerid, cmdtext);

CMD:wywal(playerid, params[])
{
	new forplayerid, string[128];

	if(sscanf(params, "d", forplayerid))
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/wywal [id gracza]{/b}");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Gracza z tym ID nie ma online.");

	if(GetDistancePlayerToPlayer(playerid, forplayerid) > 10)
		return Msg(playerid, COLOR_ERROR, "Znajdujesz siê {b}za daleko{/b} od gracza.");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] == TEAM_TYPE_MEDIC || Firmy[PlayerInfo[playerid][pFirma]][tTyp] == TEAM_TYPE_POLICE)
	{
		RemovePlayerFromVehicle(forplayerid);
		format(string, sizeof string, "Zosta³eœ wyrzucony z pojazdu przez gracza {b}%s{/b} (ID: {b}%d{/b}).", PlayerName(playerid), playerid);
		Msg(forplayerid, COLOR_ERROR, string);
		format(string, sizeof string, "Wyrzuci³eœ z pojazdu gracza {b}%s{/b} (ID: {b}%d{/b}).", PlayerName(forplayerid), forplayerid);
		Msg(playerid, COLOR_ERROR, string);
	} 
		else 
			Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	return 1;
}

CMD:kontrola(playerid, params[])
{
	new targetid,
		string[256],
		vehicleid,
		vehicleUID,
		cargoId = -1,
		weight;

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_POLICE)
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnieñ");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(sscanf(params, "d", targetid))
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/kontrola [id gracza]{/b}");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Gracza z tym ID nie ma online.");

	if(GetDistancePlayerToPlayer(playerid, targetid) > 4.0)
		return Msg(playerid, COLOR_ERROR, "Znajdujesz siê za daleko od gracza.");

	if(!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Gracz nie jest w pojeŸdzie.");

	if(IsPlayerInTruck(playerid))
	{
		vehicleid = GetVehicleTrailer(GetPlayerVehicleID(playerid));
		if(!Spawned[vehicleid])
			vehicleUID = DBVehID[vehicleid];
	}
	else
	{
		vehicleid = GetPlayerVehicleID(playerid);
		if(!Spawned[vehicleid])
			vehicleUID = DBVehID[vehicleid];
	}
	if(Spawned[vehicleid])
	{
		cargoId = vehOtherInfo[vehicleid][vCargoID];
		weight = vehOtherInfo[vehicleid][vCargoWeight]; 
	}
	else
	{
		cargoId = vehInfo[vehicleUID][vCargoID]; 
		weight = vehInfo[vehicleUID][vCargoWeight];
	}

	new towar[32];
	strins(towar, Towary[cargoId][NazwaTowaru], 0);
	format(string, sizeof string, "Nick:\t%s\nPrawo jazdy:\tPOSIADA\nPrzegl¹d:\tWA¯NY\nTowar:\t%s\nWaga:\t%dkg", PlayerName(targetid), (cargoId <= 0) ? ("brak") : (towar), weight);
	ShowPlayerDialog(playerid, DIALOG_CONTROL, DIALOG_STYLE_MSGBOX, "Kontrola", string, "WyjdŸ", #);
	return 1;
}

CMD:skuj(playerid, params[])
{
	new target, szString[84];

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_POLICE)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(sscanf(params, "d", target))
		return Msg(playerid, COLOR_ERROR, "Wpisz: {b}/skuj [id gracza]{/b}");

	if(!IsPlayerConnected(target))
		return Msg(playerid, COLOR_ERROR, "Gracza o podanym ID nie ma online.");

	if(GetDistancePlayerToPlayer(playerid, target) > 10.0)
		return Msg(playerid, COLOR_ERROR, "Znajdujesz siê za daleko od tego gracza.");

	if(PlayerInfo[target][chained] == true)
	{
		format(szString, sizeof szString, "Gracz {b}%s{/b} jest ju¿ skuty!", PlayerName(target));
		Msg(playerid, COLOR_ERROR, szString);
	}
	else
	{
		PlayerInfo[target][chained] = true;
		TogglePlayerControllable(target, 0);

		format(szString, sizeof szString, "Sku³eœ gracza {b}%s{/b} (ID: {b}%d{/b}).", PlayerName(target), target);
		Msg(playerid, COLOR_INFO3, szString);

		format(szString, sizeof szString, "Zosta³eœ skuty przez policjanta {b}%s{/b} (ID: {b}%d{/b}).", PlayerName(playerid), playerid);
		Msg(target, COLOR_INFO3, szString);
	}
	return 1;
}

CMD:rozkuj(playerid, params[])
{
	new target, szString[84];

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_POLICE)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(sscanf(params, "d", target))
		return Msg(playerid, COLOR_ERROR, "Wpisz: {b}/rozkuj [id gracza]{/b}");

	if(!IsPlayerConnected(target))
		return Msg(playerid, COLOR_ERROR, "Gracza o podanym ID nie ma online.");

	if(GetDistancePlayerToPlayer(playerid, target) > 10.0)
		return Msg(playerid, COLOR_ERROR, "Znajdujesz siê za daleko od tego gracza.");

	if(PlayerInfo[target][chained] == false)
	{
		format(szString, sizeof szString, "Gracz {b}%s{/b} nie jest skuty!", PlayerName(target));
		Msg(playerid, COLOR_ERROR, szString);
	}
	else
	{
		PlayerInfo[target][chained] = false;
		TogglePlayerControllable(target, 1);

		format(szString, sizeof szString, "Rozku³eœ gracza {b}%s{/b} (ID: {b}%d{/b}).", PlayerName(target), target);
		Msg(playerid, COLOR_INFO3, szString);

		format(szString, sizeof szString, "Zosta³eœ rozkuty przez policjanta {b}%s{/b} (ID: {b}%d{/b}).", PlayerName(playerid), playerid);
		Msg(target, COLOR_INFO3, szString);
	}
	return 1;
}
// KOMENDY POGOTOWIA
/*
	- /ulecz
	- /wywal /wyrzuc
	- /praca
	- /firma
	- /wypadki
*/

CMD:ulecz(playerid, params[])
{
	new forplayerid, string[90];

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_MEDIC)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(sscanf(params, "d", forplayerid))
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/ulecz [id gracza]{/b}");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Gracza z tym ID nie ma online.");

	if(GetDistancePlayerToPlayer(playerid, forplayerid) > 20)
		return Msg(playerid, COLOR_ERROR, "Znajdujesz siê za daleko od gracza.");

	if(GetPlayerSpeed(playerid) > 0)
		return Msg(playerid, COLOR_ERROR, "Gracz nie mo¿e siê poruszaæ.");

	if(GetPVarInt(forplayerid, "Wypadek"))
	{
		DeletePVar(forplayerid, "Wypadek");
		DeletePVar(forplayerid, "Wypadekzmedykiem");

		Loop(i, MAX_PLAYERS)
			if(Firmy[PlayerInfo[i][pFirma]][tTyp] == TEAM_TYPE_MEDIC && !GetPVarInt(i, "AFK"))
				RemovePlayerMapIcon(i, forplayerid);
	}

	KillTimer(GetPVarInt(playerid, "healTimer"));
	DeletePVar(playerid, "healTimer");

	TogglePlayerControllable(forplayerid, 1);
	SetPlayerHealth(forplayerid, 100.0);

	format(string, sizeof(string), "Uleczy³eœ gracza {b}%s{/b} (ID: {b}%d{/b}).", PlayerName(forplayerid), forplayerid);
	Msg(playerid, COLOR_INFO3, string);

	format(string, sizeof(string), "Zosta³eœ uleczony przez ratownika {b}%s{/b} (ID: {b}%d{/b}).", PlayerName(playerid), playerid);
	Msg(forplayerid, COLOR_INFO3, string);
	return 1;
}

CMD:wypadki(playerid)
{
	new szString[256], count;

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_MEDIC)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if(IsPlayerConnected(i) && GetPVarInt(i, "Areszt") <= 0)
		{
			if(GetPVarInt(i, "Wypadekzmedykiem"))
			{
				count++;
				format(szString, sizeof szString, "%s%s\t%s\n", szString, PlayerName(i), GetPlayerZone(i));
			}
		}
	}
	strins(szString, "Nick\tLokalizacja\n", 0);
	if(count < 1)
		format(szString, sizeof szString, "%sBrak wypadków.", szString);
	ShowPlayerDialog(playerid, DIALOG_CRASHES, DIALOG_STYLE_TABLIST_HEADERS, "Lista wypadków", szString, "WyjdŸ", #);
	return 1;
}

// KOMENDY POMOCY DROGOWEJ
/*
	- /praca
	- /firma
	- /zatankuj
	- /rachunek
*/

CMD:zatankuj(playerid)
{
	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_POMOC)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie siedzisz w ¿adnym pojeŸdzie.");

	new vehicleid = GetPlayerVehicleID(playerid), string[80];

	if(Spawned[vehicleid])
		vehOtherInfo[vehicleid][vFuel] = MaxFuel(GetVehicleModel(vehicleid));
	else
		vehInfo[DBVehID[vehicleid]][vFuel] = MaxFuel(GetVehicleModel(vehicleid));

	format(string, sizeof(string), "Zatankowa³eœ pojazd {b}ID: %d{/b} do pe³na.", vehicleid);
	Msg(playerid, COLOR_INFO3, string);
	return 1;
}

CMD:dyzur(playerid, params[])
{
	new string[120];

	switch(Firmy[PlayerInfo[playerid][pFirma]][tTyp])
	{
		case TEAM_TYPE_POLICE, TEAM_TYPE_MEDIC, TEAM_TYPE_POMOC:
		{
			if(!GetPVarInt(playerid, "Worked"))
			{
				SetPVarInt(playerid, "Worked", 1);
				
				Loop(player, MAX_PLAYERS)
				{
					if(GetDistancePlayerToPlayer(playerid, player) < 30)
					{
						format(string, sizeof string, "{008000}Gracz %s rozpocz¹³ dy¿ur w: {FBFB00}%s.", PlayerName(playerid), Firmy[PlayerInfo[playerid][pFirma]][tName]);
						SendClientMessage(player, 0x0, string);
					}
				}
			}
			else
			{
				DeletePVar(playerid, "Worked");
				
				Loop(player, MAX_PLAYERS)
				{
					if(GetDistancePlayerToPlayer(playerid, player) < 30)
					{
						format(string, sizeof string, "{008000}Gracz %s zakoñczy³ dy¿ur w: {FBFB00}%s.", PlayerName(playerid), Firmy[PlayerInfo[playerid][pFirma]][tName]);
						SendClientMessage(player, 0x0, string);
					}
				}
			}
		}

		default: SendClientMessage(playerid, LIGHTRED, "Nie masz uprawnieñ.");
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
	new forplayerid, string[128];

	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie masz uprawnieÃ±.");

	if(sscanf(params, "d", forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /god [id gracza]");

	SetPlayerHealth(forplayerid, 9999999);
	format(string, sizeof string, "{CC0000}>> {C0C0C0}Da³eœ nieœmiertelnoœæ graczowi %s (ID: %d).", PlayerName(forplayerid), forplayerid);
	SendClientMessage(playerid, 0x0, string);
	format(string, sizeof string, "{CC0000}>> {C0C0C0}Administrator %s (ID: %d) zrobi³ ciê {FFFFFF}nieœmiertelnym.", PlayerName(playerid), playerid);
	
	return SendClientMessage(forplayerid, 0x0, string);
}

CMD:repair(playerid, params[])
{
	new forplayerid, string[128];

	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie masz uprawnieñ.");

	if(sscanf(params, "d", forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /repair [id gracza]");

	if(!IsPlayerConnected(forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Ten gracz nie jest obecny na serwerze.");

	if(!IsPlayerInAnyVehicle(forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Gracz ten nie znajduje siê w pojeŸdzie.");

	RepairVehicle(GetPlayerVehicleID(forplayerid));
	
	if(IsTrailerAttachedToVehicle(GetPlayerVehicleID(forplayerid)))
		RepairVehicle(GetVehicleTrailer(GetPlayerVehicleID(forplayerid)));

	format(string, sizeof string, "{CC0000}>> {C0C0C0}Naprawi³eœ pojazd graczowi %s (ID: %d).", PlayerName(forplayerid), forplayerid);
	SendClientMessage(playerid, 0x0, string);
	format(string, sizeof string, "{CC0000}>> {C0C0C0}Administrator %s (ID: %d) naprawi³ ci pojazd.", PlayerName(playerid), playerid);

	return SendClientMessage(forplayerid, 0x0, string);
}

CMD:tpto(playerid, params[])
{
	if(isnull(params))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /tpto [id gracza]");

	new string[10];
	format(string, sizeof(string), "%d %s", playerid, params);
	return cmd_tp(playerid, string);
}

CMD:tphere(playerid, params[])
{
	if(isnull(params))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /tphere [id gracza]");

	new string[10];
	format(string, sizeof(string), "%s %d", params, playerid);
	return cmd_tp(playerid, string);
}

CMD:tp(playerid, params[])
{
	new forplayerid, wheplayerid, string[128];

	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie masz uprawnieñ.");

	if(sscanf(params, "dd", forplayerid, wheplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /tp [id gracza] [id gracza]");

	new Float:x, Float:y, Float:z;
	new vehicleid = GetPlayerVehicleID(forplayerid);
	GetPlayerPos(wheplayerid, x,y,z);

	if(IsPlayerInAnyVehicle(forplayerid))
	{
		SetVehiclePos(vehicleid, x,y,z);
		SetPlayerPos(forplayerid, x,y,z);
		PutPlayerInVehicle(forplayerid, vehicleid, 0);
	}
	else
		SetPlayerPos(forplayerid, x,y,z);
	
	if(playerid == forplayerid)
	{
		format(string, sizeof string, "{CC0000}>> {C0C0C0}Teleportowa³eœ siê do %s (ID: %d).", PlayerName(wheplayerid), wheplayerid);
		SendClientMessage(playerid, 0x0, string);
		format(string, sizeof string, "{CC0000}>> {C0C0C0}Administrator %s (ID: %d) teleportowa³ ciê do Ciebie.", PlayerName(playerid), playerid);
		SendClientMessage(wheplayerid, 0x0, string);
	}
	else if(playerid == wheplayerid)
	{
		format(string, sizeof string, "{CC0000}>> {C0C0C0}Teleportowa³eœ %s (ID: %d) do siebie.", PlayerName(forplayerid), forplayerid);
		SendClientMessage(playerid, 0x0, string);
		format(string, sizeof string, "{CC0000}>> {C0C0C0}Administrator %s (ID: %d) teleportowa³ ciê do ciebie.", PlayerName(playerid), playerid);
		SendClientMessage(forplayerid, 0x0, string);
	}
	else
	{
		format(string, sizeof string, "{CC0000}>> {C0C0C0}Teleportowa³eœ gracza %s (ID: %d) do %s (ID: %d).", PlayerName(forplayerid), forplayerid, PlayerName(wheplayerid), wheplayerid);
		SendClientMessage(playerid, 0x0, string);
		format(string, sizeof string, "{CC0000}>> {C0C0C0}Administrator %s (ID: %d) teleportowa³ ciê do %s (ID: %d).", PlayerName(playerid), playerid, PlayerName(wheplayerid), wheplayerid);
		SendClientMessage(forplayerid, 0x0, string);
		format(string, sizeof string, "{CC0000}>> {C0C0C0}Administrator %s (ID: %d) teleportowa³ do ciebie %s (ID: %d).", PlayerName(playerid), playerid, PlayerName(forplayerid), forplayerid);
		SendClientMessage(wheplayerid, 0x0, string);
	}

	return 1;
}

CMD:eye(playerid, params[])
{
	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie masz uprawnieñ.");

	if(podgladADMIN[playerid])
		SendClientMessage(playerid, LIGHTBLUE, "{CC0000}>>{FFFFFF} Wy³¹czy³eœ podgl¹d CMD."); 
	else
		SendClientMessage(playerid, LIGHTBLUE, "{CC0000}>>{FFFFFF} W³¹czy³eœ podgl¹d CMD."); 

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
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie znajdujesz siê w pojeŸdzie.");

	if(State != PLAYER_STATE_DRIVER)
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie jesteœ kierowc¹.");

	new Float:angle;
    GetVehicleZAngle(vehicleid, angle);
    SetVehicleZAngle(vehicleid, angle);

	return 1;
}

CMD:setloc(playerid, params[])
{
	new string[128];

	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie posiadasz uprawnieñ.");

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
	format(string, sizeof string, "{CC0000}>> {004080}Ustawi³eœ lokalizacje o id: %d, interiorze: %d", savedpos, I);
	SendClientMessage(playerid,0x0, string);
	return 1;
}

CMD:tploc(playerid, params[])
{
	new locid, string[128];

	if(sscanf(params, "d", locid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /tploc [id]");

	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie posiadasz uprawnieñ.");

	if(!locid) 
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /tploc [id]");

	if(locid>savedpos) 
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /tploc [id]");

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

	format(string, sizeof string, "{CC0000}>> {004080}Teleportowa³eœ siê do lokalizacji o id: %d", locid);
	SendClientMessage(playerid,0x0, string);
	return 1;
}

CMD:setscore(playerid, params[])
{
	new forplayerid, score;

	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie masz uprawnieñ.");

	if(sscanf(params, "dd", forplayerid, score))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /setscore [id gracza] [score]");

	if(!IsPlayerConnected(forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Gracz nie jest na serwerze.");

	if(score < 0)
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie mo¿esz ustawiæ mniej ni¿ 0.");

	ResetScore(forplayerid);
	GiveScore(forplayerid, score);

	new string[126];
	format(string, sizeof(string), "{CC0000}>> {004080}Administrator %s (ID: %d) zmieni³ Ci punkty score na %d.", PlayerName(playerid), playerid, score);
	SendClientMessage(forplayerid, 0x0, string);
	format(string, sizeof(string), "{CC0000}>> {004080}Ustawi³eœ graczowi %s (ID: %d) punkty score na %d.", PlayerName(forplayerid), forplayerid, score);
	SendClientMessage(playerid, 0x0, string);

	return 1;
}

CMD:setcash(playerid, params[])
{
	new forplayerid, kasa;

	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie masz uprawnieñ.");

	if(sscanf(params, "dd", forplayerid, kasa))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /setcash [id gracza] [kasa]");

	if(!IsPlayerConnected(forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Gracz nie jest na serwerze.");

	if(kasa < 0)
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie mo¿esz ustawiæ mniej ni¿ 0.");

	ResetMoney(forplayerid);
	GiveMoney(forplayerid, kasa);

	new string[126];
	format(string, sizeof(string), "{CC0000}>> {004080}Administrator %s (ID: %d) ustawi³ ci kasê na %d$.", PlayerName(playerid), playerid, kasa);
	SendClientMessage(forplayerid, 0x0, string);
	format(string, sizeof(string), "{CC0000}>> {004080}Ustawi³eœ graczowi %s (ID: %d) kasê na %d$.", PlayerName(forplayerid), forplayerid, kasa);
	SendClientMessage(playerid, 0x0, string);

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
	new forplayerid, all, string[90];
	new vehicleid;

	if(!PlayerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnieñ.");

	if(sscanf(params, "dD(0)", forplayerid, all))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /refuelveh [id gracza]");

	if(!IsPlayerConnected(forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Gracz nie jest pod³¹czony.");

	if(all)
	{
		Loop(vehicle, GetVehiclePoolSize())
			if(IsValidVehicle(vehicle))
			{
				if(Spawned[vehicle])
					vehOtherInfo[vehicle][vFuel] = MaxFuel(GetVehicleModel(vehicle));
				else
					vehInfo[DBVehID[vehicle]][vFuel] = MaxFuel(GetVehicleModel(vehicle));
			}

		format(string, sizeof(string), "{CC0000}>> {004080}Administrator %s (ID: %d) zatankowa³ wszystkie pojazdy.", PlayerName(playerid), playerid);
		SendClientMessageToAll(0x0, string);
	}
	else
	{
		vehicleid = GetPlayerVehicleID(forplayerid);

		if(Spawned[vehicleid])
			vehOtherInfo[vehicleid][vFuel] = MaxFuel(GetVehicleModel(vehicleid));
		else
			vehInfo[DBVehID[vehicleid]][vFuel] = MaxFuel(GetVehicleModel(vehicleid));

		format(string, sizeof(string), "{CC0000}>> {004080}Administrator %s (ID: %d) zatankowa³ Ci pojazd.", PlayerName(playerid), playerid);
		SendClientMessage(forplayerid, 0x0, string);

		format(string, sizeof(string), "{CC0000}>> {004080}Zatankowa³eœ pojazd graczowi %s (ID: %d).", PlayerName(forplayerid), forplayerid);
		SendClientMessage(playerid, 0x0, string);
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
	new forplayerid, weapon, all, string[90];

	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie posiadasz uprawnieñ.");

	if(sscanf(params, "ddD(0)", forplayerid, weapon, all))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /givegun [id gracza] [id broni]");

	if(!IsPlayerConnected(forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Gracz nie jest pod³¹czony.");

	if(weapon < 0 || weapon > 46)
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} B³êdne ID broni (1-46).");

	if(all && forplayerid == playerid)
	{
		Loop(player, MAX_PLAYERS)
			if(IsPlayerConnected(player))
				GivePlayerWeapon(player, weapon, 9999999);

		format(string, sizeof(string), "{CC0000}>> {004080}Administrator %s (ID: %d) da³ wszystkim broñ (%s).", PlayerName(playerid), playerid, GetWeaponNameEx(weapon));
		SendClientMessageToAll(0x0, string);
	}
	else
	{
		GivePlayerWeapon(forplayerid, weapon, 9999999);

		format(string, sizeof(string), "{CC0000}>> {004080}Administrator %s (ID: %d) da³ Ci broñ (%s).", PlayerName(playerid), playerid, GetWeaponNameEx(weapon));
		SendClientMessage(forplayerid, 0x0, string);

		format(string, sizeof(string), "{CC0000}>> {004080}Da³eœ %s graczowi %s (ID: %d).", GetWeaponNameEx(weapon), PlayerName(forplayerid), forplayerid);
		SendClientMessage(playerid, 0x0, string);
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
	new forplayerid, all, string[90];

	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie posiadasz uprawnieñ.");

	if(sscanf(params, "ddD(0)", forplayerid, all))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /disarm [id gracza]");

	if(!IsPlayerConnected(forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Gracz nie jest pod³¹czony.");

	if(all)
	{
		Loop(player, MAX_PLAYERS)
			if(IsPlayerConnected(player))
				ResetPlayerWeapons(player);

		format(string, sizeof(string), "{CC0000}>> {004080}Administrator %s (ID: %d) zabra³ wszystkim bronie.", PlayerName(playerid), playerid);
		SendClientMessageToAll(0x0, string);
	}
	else
	{
		ResetPlayerWeapons(forplayerid);

		format(string, sizeof(string), "{CC0000}>> {004080}Administrator %s (ID: %d) zresetowa³ ci bronie.", PlayerName(playerid), playerid);
		SendClientMessage(forplayerid, 0x0, string);

		format(string, sizeof(string), "{CC0000}>> {004080}Zresetowa³eœ bronie graczowi %s (ID: %d).", PlayerName(forplayerid), forplayerid);
		SendClientMessage(playerid, 0x0, string);
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
	new forplayerid, all, string[120];

	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie posiadasz uprawnieñ.");

	if(sscanf(params, "dD(0)", forplayerid, all))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /food [id gracza]");

	if(!IsPlayerConnected(forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Gracz nie jest pod³¹czony.");

	if(all)
	{
		Loop(player, MAX_PLAYERS)
			if(IsPlayerConnected(player))
			{
				//SetPVarInt(player, "JEDZENIEPROCENTY", 100);
				//SetPlayerProgressBarValue(player, Glod[player], 100.0);
				//UpdatePlayerProgressBar(player, Glod[player]);
			}
		format(string, sizeof(string), "{CC0000}>> {004080}Administrator %s (ID: %d) zmniejszy³ wszystkim g³ód.", PlayerName(playerid), playerid);
		SendClientMessageToAll(0x0, string);
	}
	else
	{
		//SetPVarInt(forplayerid, "JEDZENIEPROCENTY", 100);
		//SetPlayerProgressBarValue(forplayerid, Glod[forplayerid], 100.0);
		//UpdatePlayerProgressBar(forplayerid, Glod[forplayerid]);

		format(string, sizeof(string), "{CC0000}>> {004080}Administrator %s (ID: %d) zmniejszy³ Ci g³ód.", PlayerName(playerid), playerid);
		SendClientMessage(forplayerid, 0x0, string);

		format(string, sizeof(string), "{CC0000}>> {004080}Zmniejszy³eœ g³ód graczowi %s (ID: %d).", PlayerName(forplayerid), forplayerid);
		SendClientMessage(playerid, 0x0, string);
	}
	return 1;
}

CMD:remove(playerid, params[])
{
	new forplayerid, string[60];

	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie posiadasz uprawnieñ.");

	if(sscanf(params, "d", forplayerid)) 
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /remove [id gracza]");

	if(!IsPlayerConnected(forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Gracz nie jest pod³¹czony.");

	if(!IsPlayerInAnyVehicle(forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Gracz nie jest w pojeŸdzie.");

	RemovePlayerFromVehicle(forplayerid);

	format(string, sizeof(string), "{CC0000}>> {004080}Administrator %s (ID: %d) wyrzuci³ ciê z pojazdu.", PlayerName(playerid), playerid);
	SendClientMessage(forplayerid, 0x0, string);

	format(string, sizeof(string), "{CC0000}>> {004080}Wyrzuci³eœ z pojazdu %s (ID: %d).", PlayerName(forplayerid), forplayerid);
	SendClientMessage(playerid, 0x0, string);
	
	return 1;
}

CMD:createveh(playerid, params[])
{
	new vehicleID, vehicleModel, vehicleColor[2], Float:vehiclePos[4], string[60];

	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie posiadasz uprawnieñ.");

	if(SpawnedAdminVehicles == MAX_ADMIN_VEHICLES)
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie mo¿na zespawnowaæ wiêcej pojazdów (50).");	
	
	if(sscanf(params, "ddd", vehicleModel, vehicleColor[0], vehicleColor[1]))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /createveh [model] [kolor 1] [kolor2]");

	if(vehicleModel < 400 || vehicleModel > 611)
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nieprawid³owy model (400-611)");

	if( (vehicleColor[0] < 0 || vehicleColor[0] > 255) || (vehicleColor[1] < 0 || vehicleColor[1] > 255) )
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nieprawid³owy kolor (0-255)");

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

	vehOtherInfo[AdminsVehs[vehicleID]][vFuel] = MaxFuel(vehicleID);
	vehOtherInfo[AdminsVehs[vehicleID]][vFuelType] = IsVehicleTruck(vehicleID) ? FUEL_TYPE_TDI : FUEL_TYPE_BENZ;
	vehOtherInfo[AdminsVehs[vehicleID]][vPrzebieg] = 0.0;
	vehOtherInfo[AdminsVehs[vehicleID]][vCargoID] = 0;
	vehOtherInfo[AdminsVehs[vehicleID]][vCargoWeight] = 0; 
	vehOtherInfo[AdminsVehs[vehicleID]][Color1] = vehicleColor[0]; 
	vehOtherInfo[AdminsVehs[vehicleID]][Color2] = vehicleColor[1]; 

	format(string, sizeof(string), "{CC0000}>> {004080}Stworzy³eœ pojazd o ID: %d", vehicleID);
	SendClientMessage(playerid, 0x0, string);

	return 1;
}

CMD:destroyvehall(playerid, params[])
	return cmd_destroyveh(playerid, "0 1");

CMD:destroyveh(playerid, params[])
{
	new vehicleID, all, string[60];

	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie posiadasz uprawnieñ.");

	if(sscanf(params, "dD(0)", vehicleID, all))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /destroyveh [id pojazdu]");

	if(!AdminsVehs[vehicleID] && !all)
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Dany pojazd nie istnieje.");

	if(all)
	{
		for(new i; i < MAX_ADMIN_VEHICLES; i++)
		{
			if(IsValidVehicle(AdminsVehs[i]))
				DestroyVehicle(AdminsVehs[i]);

			AdminsVehs[i] = INVALID_VEHICLE_ID;
		}

		SpawnedAdminVehicles = 0;
		SendClientMessage(playerid, 0x0, "{CC0000}>> {004080}Zniszczy³eœ wsystkie pojazdy adminów.");
	}
	else
	{
		DestroyVehicle(AdminsVehs[vehicleID]);
		AdminsVehs[vehicleID] = INVALID_VEHICLE_ID;
		SpawnedAdminVehicles--;

		format(string, sizeof(string), "{CC0000}>> {004080}Zniszczy³eœ pojazd o ID: %d", vehicleID);
		SendClientMessage(playerid, 0x0, string);
	}

	return 1;
}

CMD:put(playerid, params[])
{
	new vehiclePlayerID, secondPlayerID, seat, string[120];

	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie posiadasz uprawnieñ.");

	if(sscanf(params, "ddd", secondPlayerID, vehiclePlayerID, seat))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /put [id gracza] [id gracza] [miejsce>1]");

	if(!IsPlayerConnected(vehiclePlayerID) || !IsPlayerConnected(secondPlayerID))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Gracz nie jest na serwerze.");

	if(seat < 1)
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nieprawid³owe miejsce.");

	if(!IsPlayerInAnyVehicle(vehiclePlayerID))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Gracz nie jest w pojeŸdzie.");

	RemovePlayerFromVehicle(secondPlayerID);
	PutPlayerInVehicle(secondPlayerID, GetPlayerVehicleID(vehiclePlayerID), seat);

	format(string, sizeof(string), "{CC0000}>> {004080}Administrator %s (ID: %d) wrzuci³ ciê do innego pojazdu.", PlayerName(playerid), playerid);
	SendClientMessage(secondPlayerID, LIGHTRED, string);
	format(string, sizeof(string), "{CC0000}>> {004080}Wrzuci³eœ %s (ID: %d) do innego pojazdu.", PlayerName(secondPlayerID), secondPlayerID);
	SendClientMessage(playerid, LIGHTRED, string);
	format(string, sizeof(string), "{CC0000}>> {004080}Administrator %s (ID: %d) wrzuci³ %s (ID: %d) do Twojego pojazdu.", PlayerName(playerid), playerid, PlayerName(secondPlayerID), secondPlayerID);
	SendClientMessage(playerid, LIGHTRED, string);

	return 1;
}

CMD:specon(playerid, params[])
{
	new forplayerid;

	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie posiadasz uprawnieñ.");

	if(sscanf(params, "ddd", forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /specon [id gracza]");

	if(!IsPlayerConnected(forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Gracz nie jest pod³¹czony.");

	if(GetPVarInt(forplayerid, "Przeglada"))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Gracz ju¿ kogoœ przegl¹da.");

	if(forplayerid == playerid)
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie mo¿esz przegl¹daæ siebie.");

	if(GetPVarType(playerid, "Przeglada") == PLAYER_VARTYPE_NONE)
	{
		new Float:Pos[3];
		GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);

		SetPVarFloat(playerid, "specPosX", Pos[0]);
		SetPVarFloat(playerid, "specPosY", Pos[1]);
		SetPVarFloat(playerid, "specPosZ", Pos[2]);
	}

	SpectactorTextDraw(playerid, true);
	TogglePlayerSpectating(playerid, 1);

	if(IsPlayerInAnyVehicle(forplayerid))
		PlayerSpectateVehicle(playerid, GetPlayerVehicleID(forplayerid));
	else
		PlayerSpectatePlayer(playerid, forplayerid);

	SetPVarInt(playerid, "Przeglada", forplayerid);
	SetPVarInt(forplayerid, "jestPrzegladany", playerid);

	return 1;
}

CMD:specoff(playerid, params[])
{
	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie posiadasz uprawnieñ.");

	if(GetPVarType(playerid, "Przeglada") == PLAYER_VARTYPE_NONE)
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie przegl¹dasz ¿adnego gracza.");

	TogglePlayerSpectating(playerid, 0);
	SpectactorTextDraw(playerid, false);

	return 1;
}

CMD:restart(playerid, params[])
{
	if(!HasPlayerFullPermission(playerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie masz uprawnieñ.");

	new string[176];
	format(string, sizeof string, "{CC0000}>> {C0C0C0}Administrator {FFFFFF}%s{C0C0C0} (ID: %d) zrestartowa³ serwer.", PlayerName(playerid), playerid);
	SendClientMessageToAll(0x0, string);
	SendRconCommand("gmx");
	return 1;
}

CMD:cd(playerid, params[]) 
{
	new string[80], czas;

    if(sscanf(params, "dD(0)", czas, OdliczanieFreeze))
    	return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /cd [czas] [freeze]");

    if(czas > 100)
    	return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Limit na 100 sekund.");

	if(OdliczanieWystartowalo == false)
	{
     	Odliczanie = strval(params);
    	OdliczanieTimer = SetTimer("CountDown", 1000, 1);
    	OdliczanieWystartowalo = true;
            
        format(string, sizeof(string), "{CC0000}>>{FFFFFF} Administrator %s (ID: %d) wystartowa³ odliczanie!", PlayerName(playerid), playerid);
        SendClientMessageToAll(LIGHTBLUE, string);

        if(OdliczanieFreeze)
        	Loop(player, MAX_PLAYERS)
        		if(IsPlayerConnected(player))
        			TogglePlayerControllable(player, 0);
    }
	else
         SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Ju¿ odlicza.");

    return 1;
}  

CMD:freezeall(playerid, params[])
	return cmd_freeze(playerid, "0 1");

CMD:freeze(playerid, params[])
{
	new forplayerid, all, string[60];

	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie posiadasz uprawnieñ.");

	if(sscanf(params, "dD(0)", forplayerid, all))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /freeze [id gracza]");

	if(!IsPlayerConnected(forplayerid) && !all)
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Gracz nie jest online.");

	if(all)
	{
		Loop(player, MAX_PLAYERS)
			if(IsPlayerConnected(player))
				TogglePlayerControllable(player, 0);

		format(string, sizeof(string), "{CC0000}>> {008800}Administrator %s (ID: %d) zamrozi³ wszystkich graczy.", PlayerName(playerid), playerid);
		SendClientMessageToAll(0x0, string);
	}
	else
	{
		TogglePlayerControllable(forplayerid, 0);

		format(string, sizeof(string), "{CC0000}>> {008800}Administrator %s (ID: %d) zamrozi³ Ciê.", PlayerName(playerid), playerid);
		SendClientMessage(forplayerid, 0x0, string);
		format(string, sizeof(string), "{CC0000}>> {008800}Zamrozi³eœ %s (ID: %d).", PlayerName(forplayerid), forplayerid);
		SendClientMessage(playerid, 0x0, string);
	}

	return 1;
}

CMD:unfreezeall(playerid, params[])
	return cmd_unfreeze(playerid, "0 1");

CMD:unfreeze(playerid, params[])
{
	new forplayerid, all, string[60];

	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie posiadasz uprawnieñ.");

	if(sscanf(params, "dD(0)", forplayerid, all))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /unfreeze [id gracza]");

	if(!IsPlayerConnected(forplayerid) && !all)
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Gracz nie jest online.");

	if(all)
	{
		Loop(player, MAX_PLAYERS)
			if(IsPlayerConnected(player))
				TogglePlayerControllable(player, 1);

		format(string, sizeof(string), "{CC0000}>> {008800}Administrator %s (ID: %d) odmrozi³ wszystkich graczy.", PlayerName(playerid), playerid);
		SendClientMessageToAll(0x0, string);
	}
	else
	{
		TogglePlayerControllable(forplayerid, 1);

		format(string, sizeof(string), "{CC0000}>> {008800}Administrator %s (ID: %d) odmrozi³ Ciê.", PlayerName(playerid), playerid);
		SendClientMessage(forplayerid, 0x0, string);
		format(string, sizeof(string), "{CC0000}>> {008800}Odmrozi³eœ %s (ID: %d).", PlayerName(forplayerid), forplayerid);
		SendClientMessage(playerid, 0x0, string);
	}

	return 1;
}


CMD:killp(playerid, params[])
{
	new forplayerid, string[128];

	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie masz uprawnieñ.");

	if(sscanf(params, "d", forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /killp [id gracza]");

	if(!IsPlayerConnected(forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Ten gracz nie jest obecny na serwerze.");

	SetPlayerHealth(forplayerid, 0);
	format(string, sizeof string, "{CC0000}>> {C0C0C0}Zabi³eœ gracza %s (ID: %d).", PlayerName(forplayerid), forplayerid);
	SendClientMessage(playerid, 0x0, string);
	format(string, sizeof string, "{CC0000}>> {C0C0C0}Administrator %s (ID: %d) zabi³ ciê.", PlayerName(playerid), playerid);
	SendClientMessage(forplayerid, 0x0, string);
	return 1;
}

CMD:colorplayer(playerid, params[])
{
	new forplayerid, kolor[16], string[120], colorId;

	if(!PlayerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnieñ.");

	if(sscanf(params, "ds[16]", forplayerid, kolor))
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/colorplayer [id gracza] [kolor]{/b}");

	if(!IsPlayerConnected(forplayerid))
		return Msg(playerid, COLOR_ERROR, "Gracza z tym ID nie ma online.");

	colorId = -1;
	for(new i = 0; i < sizeof colors; i++)
	{
		if(strfind(colors[i], kolor, false) != -1)
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
		case 0: SetPlayerColor(forplayerid, 0xFFFFFF00); // niewidoczny
		case 1: SetPlayerColor(forplayerid, 0xFFFFFFFF); // bia³y
		case 2: SetPlayerColor(forplayerid, 0xFF9900FF); // pomarañczowy
		case 3: SetPlayerColor(forplayerid, 0xFF0000FF); // czerwony
		case 4: SetPlayerColor(forplayerid, 0xFF00FFFF); // ró¿owy
		case 5: SetPlayerColor(forplayerid, 0xFFFF00FF); // ¿ó³ty
		case 6: SetPlayerColor(forplayerid, 0x663300FF); // br¹zowy
		case 7: SetPlayerColor(forplayerid, 0x009933FF); // zielony
		case 8: SetPlayerColor(forplayerid, 0x00FFFFFF); // jasnoniebieski
		case 9: SetPlayerColor(forplayerid, 0x0000CCFF); // niebieski
	}

	format(string, sizeof string, "Zmieni³eœ kolor graczowi {b}%s{/b} na {b}%s{/b}.", PlayerName(forplayerid), colors[colorId]);
	Msg(playerid, COLOR_INFO, string);
	format(string, sizeof string, "Administrator {b}%s{/b} zmieni³ Ci kolor na {b}%s{/b}.", PlayerName(playerid), colors[colorId]);
	Msg(playerid, COLOR_INFO, string);
	return 1;
}

CMD:dodajadmin(playerid, params[])
{
	new forplayerid, poziom, string[128];

	if(!HasPlayerFullPermission(playerid) && PlayerInfo[playerid][pAdmin] != 2)
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Brak uprawnieñ.");

	if(sscanf(params, "dd", forplayerid, poziom))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /dodajadmin [id] [poziom]");

	if(!IsPlayerConnected(forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Gracz nie jest na serwerze.");

	PlayerInfo[forplayerid][pAdmin] = poziom;
	format(string, sizeof string, "{CC0000}>> {008000}Gracz {FFFFFF}%s {008000}zostaæ dodany do grupy administratorów.", PlayerName(forplayerid));
	SendClientMessage(playerid,0x0,string);

	return 1;
}

CMD:usunadmin(playerid, params[])
{
	new forplayerid, string[128];

	if(!HasPlayerFullPermission(playerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Brak uprawnieñ.");

	if(sscanf(params, "d", forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /usunadmin [id]");

	if(!IsPlayerConnected(forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Gracz nie jest na serwerze.");

	PlayerInfo[playerid][pAdmin] = 0;
	format(string, sizeof string, "{CC0000}>> {008000}Gracz {FFFFFF}%s{008000} zosta³ usuniêty z grupy administratorów.", PlayerName(forplayerid));
	SendClientMessage(playerid,0x0,string);
	
	return 1;
}

CMD:setname(playerid, params[])
{
	new forplayerid, nick[20], string[128];

	if(!HasPlayerFullPermission(playerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Brak uprawnieñ.");

	if(sscanf(params, "ds[20]", forplayerid, nick))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /setname [id gracza] [nick]");

	if(!IsPlayerConnected(forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Gracz nie jest pod³¹czony.");

	format(string, sizeof string, "UPDATE `Accounts` SET `Name` = '%s' WHERE `Name` = '%s'", nick, PlayerName(forplayerid));
	mysql_query(string);

	format(string, sizeof string, "{CC0000}>> {008000}Administrator {FFFFFF}%s{008000} (ID: %d) zmieni³ ci nick na %s.", PlayerName(playerid), playerid, nick);
	SendClientMessage(forplayerid,0x0,string);
	format(string, sizeof string, "{CC0000}>> {008000}Zmieni³eœ graczowi nick z {FFFFFF}%s{008000} (ID: %d) na %s.", PlayerName(forplayerid), playerid, nick);
	SendClientMessage(forplayerid,0x0,string);

	SetPlayerName(forplayerid, nick);
	return 1;
}

CMD:dodajdok(playerid, params[])
{
	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie masz uprawnieñ.");

	return ShowPlayerDialog(playerid, DIALOG_CREATE_DOK_NAME, DIALOG_STYLE_INPUT, " ", "Wpisz nazwê doku:", "Ok", "Zamknij");
}

CMD:usundok(playerid, params[])
{
	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie masz uprawnieñ.");

	if(isnull(params))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /usundok [id doku]");

	new dokid = strval(params), string[120];

	if(!LoadingPoints[dokid][lID])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nieprawid³owe ID doku.");

	format(string, sizeof string, "{CC0000}>>{FFFFFF} Dok o ID {FFFFFF}%d {008000} zosta³ usuniêty.", dokid);
	SendClientMessage(playerid,0x0,string);

	LoadingPoints[dokid][lID] = 0;

	DestroyDynamic3DTextLabel(LoadingPointsText3D[dokid]);

	for(new playerids; playerids < MAX_PLAYERS; playerids++)
		DestroyDynamicMapIcon(LoadingPoinstMapIcon[dokid][playerids]);

	format(string, sizeof string, "DELETE FROM `Doki` WHERE `ID`= '%d'", dokid);
	mysql_query(string);

	return 1;
}

CMD:respawniko(playerid, params[])
{
	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie masz uprawnieñ.");

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

	SendClientMessageToAll(0x0, "{CC0000}>>{FFFFFF} Wszystkie pojazdy powróci³y na miejsce spawnu.");
	return 1;
}

CMD:dodajtowar(playerid, params[])
{
	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie masz uprawnieñ.");

	return ShowPlayerDialog(playerid, DIALOG_CREATE_TOWAR_NAME, DIALOG_STYLE_INPUT, " ", "Wpisz nazwê towaru:", "Ok", "Zamknij");
}

CMD:usuntowar(playerid, params[])
{
	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie masz uprawnieñ.");

	if(isnull(params))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /usuntowar [id towaru]");

	new idtowar = strval(params), string[100];

	if(!Towary[idtowar][tID])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wybrany towar nie istnieje.");

	Towary[idtowar][tID] = 0;
	
	format(string, sizeof string, "DELETE FROM `Towary` WHERE `ID`= '%d'", idtowar);
	mysql_query(string);

	return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Usuniêto towar.");
}

CMD:dodajradio(playerid, params[])
{
	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie masz uprawnieñ.");

	return ShowPlayerDialog(playerid, DIALOG_ID_RADIO_ADD, DIALOG_STYLE_INPUT, " ", "Wpisz nazwê radia:", "Dalej", "Zamknij");
}

CMD:dodajstacje(playerid, params[])
{
	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie masz uprawnieñ.");

	new string[128];
	new Float:Pos[3];
	GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);

	format(string, sizeof string, "INSERT INTO `Stacje` (`PosX`, `PosY`, `PosZ`) VALUES('%f', '%f', '%f')", Pos[0], Pos[1], Pos[2]);
	mysql_query(string);

	format(string, sizeof string, "{808040}STACJA BENZYNOWA {808040}[VID: {FFFFFF}%d{808040}]\n[ Zbiorniki: 3000/3000 ]\n{FFFFFF}/tankuj", ID_STACJA);
	StacjeText3D[ID_STACJA] = CreateDynamic3DTextLabel(string, ZIELONY2, Pos[0], Pos[1], Pos[2]+0.5, 30.0);
	
	for(new playerids; playerids < MAX_PLAYERS; playerids++)
		StacjeMapIcon[ID_STACJA][playerids] = CreateDynamicMapIcon(Stacje[ID_STACJA][sPosX], Stacje[ID_STACJA][sPosY], Stacje[ID_STACJA][sPosZ], 55, 0, _, _, playerids, 300.0);

	format(string, sizeof string, "{CC0000}>>{FFFFFF} {008000}Stacja o uid: {FFFFFF}%d {008000}zosta³a pomyœlnie utworzona.", ID_STACJA);
	SendClientMessage(playerid,0x0,string);
	ID_STACJA++;
	return 1;
}

CMD:usunstacje(playerid, params[])
{
	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie masz uprawnieñ.");

	new string[132];

	new stacjaid = GetStacja(playerid);
	if(stacjaid == 0)
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie znajdujesz siê przy stacji.");

	format(string, sizeof string, "{CC0000}>>{FFFFFF} {008000}Stacja o uid: {FFFFFF}%d {008000}zosta³a pomyœlnie usuniêta.", stacjaid);
	SendClientMessage(playerid,0x0,string);

	DestroyDynamic3DTextLabel(StacjeText3D[stacjaid]);

	for(new playerids; playerids < MAX_PLAYERS; playerids++)
		DestroyDynamicMapIcon(StacjeMapIcon[ID_STACJA][playerids]);

	format(string, sizeof string, "DELETE FROM `Stacje` WHERE `ID`= '%d'", stacjaid);
	mysql_query(string);
	return 1;
}

CMD:pogoda(playerid, params[])
{
	new string[90];

	if(!PlayerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie posiadasz uprawnieñ.");

	new weatherid = strval(params);

	if(weatherid < 0 || weatherid > 20)
		return Msg(playerid, COLOR_ERROR, "Nieprawid³owe ID pogody {b}(0-20){/b}.");

	SetWeather(weatherid);

	format(string, sizeof string, "Pogoda zosta³ zmieniona przez {b}%s{/b} na pogodê ID: {b}%d{/b}.", PlayerName(playerid), weatherid);
	MsgToAll(COLOR_INFO2, string);
	return 1;
}

CMD:ustawlidera(playerid, params[])
{
	new forplayerid, firmaid, string[100];
	
	if(PlayerInfo[playerid][pAdmin] != 2)
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie masz uprawnieñ.");

	if(sscanf(params, "dd", forplayerid, firmaid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /ustawlidera [id gracza] [id firmy]");

	if(!IsPlayerConnected(forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Gracz nie jest pod³¹czony.");

	if(Firmy[firmaid][tID] == 0)
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} B³êdne ID firmy.");

	Firmy[firmaid][tChef] = PlayerInfo[forplayerid][pID];
	PlayerInfo[forplayerid][pFirma] = firmaid;
	
	format(string, sizeof(string), "UPDATE `Firmy` SET `Szef` = '%d' WHERE `id` = '%d'", PlayerInfo[forplayerid][pID], firmaid);
	mysql_query(string);

	format(string, sizeof(string), "Administrator %s zarz¹dzi³ Ciê szefem firmy %s.", PlayerName(playerid), Firmy[firmaid][tName]);
	ShowInfo(forplayerid, string);
	
	format(string, sizeof(string), "%s sta³ siê szefem firmy %s.", PlayerName(forplayerid), Firmy[firmaid][tName]);
	ShowInfo(playerid, string);
	
	return 1;
}

CMD:dodajpojazd(playerid, params[])
{
	new string[500], vehicleid = GetPlayerVehicleID(playerid), Float:Pos[4], firmaid, naczepa;
	
	if(sscanf(params, "dD(0)", firmaid, naczepa))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /dodajpojazd [id firmy]");

	if(!IsPlayerInAnyVehicle(playerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Musisz byæ w pojeŸdzie.");

	if(naczepa)
	{
		if(IsTrailerAttachedToVehicle(vehicleid))
			vehicleid = GetVehicleTrailer(vehicleid);
		else
			return  SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie masz pod³¹czonej naczepy.");
	}

	if(!Spawned[vehicleid])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Stwórz pojazd poprzez /createveh");

	GetVehiclePos(vehicleid, Pos[0], Pos[1], Pos[2]);
	GetVehicleZAngle(vehicleid, Pos[3]);

	format(string, sizeof(string), "INSERT INTO `Pojazdy` (`model`, `owner_id`, `owner_type`, `PosX`, `PosY`, `PosZ`, `PosA`, `Health`, `Fuel`, `FuelType`, `Color1`, `Color2`, `Plate`) VALUES ('%d', '%d', '1', '%f', '%f', '%f', '%f', '1000.0', '%d', '%d', '%d', '%d', 'Rej. Cipa')", GetVehicleModel(vehicleid), firmaid, Pos[0], Pos[1], Pos[2], Pos[3], MaxFuel(GetVehicleModel(vehicleid)), 
			(IsVehicleTruck(vehicleid) ? FUEL_TYPE_TDI : FUEL_TYPE_BENZ), vehOtherInfo[vehicleid][Color1], vehOtherInfo[vehicleid][Color2]);
	mysql_query(string);
	printf(string);

	DestroyVehicle(vehicleid);
	Spawned[vehicleid] = false;

	LadujPojazd(_, _, mysql_insert_id());
	return 1;
}

CMD:debug(playerid, params[])
{
	new string[500], pVarName[128], pVarNameA[80];
	new PVarUpperIndex = GetPVarsUpperIndex(playerid) + 1;

	strcat(string, "Nazwa\tWartoœæ\n");

	for(new i=0; i != PVarUpperIndex; i++)
	{
	    GetPVarNameAtIndex(playerid, i, pVarName, sizeof(pVarName));
	    
	    switch(GetPVarType(playerid, pVarName))
	    {
	    	case PLAYER_VARTYPE_INT: format(string, sizeof(string), "%s%s\t%d\n", string, pVarName, GetPVarInt(playerid, pVarName));
	    	case PLAYER_VARTYPE_STRING: format(string, sizeof(string), "%s%s\t%f\n", string, pVarName, GetPVarFloat(playerid, pVarName));
	    	case PLAYER_VARTYPE_FLOAT:
	    	{
	    		GetPVarString(playerid, pVarName, pVarNameA, sizeof(pVarNameA));
	    		format(string, sizeof(string), "%s%s\t%s\n", string, pVarName, pVarNameA);
	    	}
	    }
	}
 
	ShowPlayerDialog(playerid, NEVER_DIALOG, DIALOG_STYLE_TABLIST_HEADERS, "Debug PVAR", string, "Ok", "");

	return 1;
}

CMD:dodajpracownika(playerid, params[])
{
	new forplayerid, firmaid, string[80];

	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie masz uprawnieñ.");

	if(sscanf(params, "dd", forplayerid, firmaid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /dodajpracownika [graczid] [firmaid]");

	if(firmaid > 7 || firmaid < 0)
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} ID firmy musi byæ w przedziale 0-7.");

	PlayerInfo[forplayerid][pFirma] = firmaid;

	format(string, sizeof(string), "Administrator %s przydzieli³ Ciê do firmy %s.", PlayerName(playerid), Firmy[firmaid][tName]);
	ShowInfo(forplayerid, string);
	
	format(string, sizeof(string), "Przydzieli³eœ %s do firmy %s.", PlayerName(forplayerid), Firmy[firmaid][tName]);
	ShowInfo(playerid, string);

	return 1;
}

CMD:usunpracownika(playerid, params[])
{
	new forplayerid, string[80];

	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie masz uprawnieñ.");

	if(sscanf(params, "d", forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /usunpracownika [graczid]");
		
	PlayerInfo[forplayerid][pFirma] = 0;
	format(string, sizeof(string), "Administrator %s wyrzuci³ Ciê z firmy.", PlayerName(playerid));
	ShowInfo(forplayerid, string);
	
	format(string, sizeof(string), "Wyrzuci³eœ %s z firmy.", PlayerName(forplayerid));
	ShowInfo(playerid, string);

	return 1;
}
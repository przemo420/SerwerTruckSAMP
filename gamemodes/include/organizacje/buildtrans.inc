CMD:changepos(playerid, params[])
{
new id;
new string[76];

if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_BUILD)
return SendClientMessage(playerid, LIGHTRED, "Nie masz uprawnieñ.");

if(sscanf(params, "d", id))
return SendClientMessage(playerid, LIGHTRED, "Wpisz: /changepos [id obiektu]");

if(IsObiektBT[id] == false)
return SendClientMessage(playerid, LIGHTRED, "Nie znaleziono takiego obiektu.");

format(string, sizeof string, "{004080}Rozpocz¹³eœ edycjê obiektu o id {FFFFFF}%d.", id);
SendClientMessage(playerid, 0x0, string);
EditObject(playerid, obiektyBT[id]);
SetPVarInt(playerid, "OBIEKTCHANGEBTID", id);
return 1;
}

CMD:delone(playerid, params[])
{
new id;
new string[76];

if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_BUILD)
return SendClientMessage(playerid, LIGHTRED, "Nie masz uprawnieñ.");

if(sscanf(params, "d", id))
return SendClientMessage(playerid, LIGHTRED, "Wpisz: /delone [id obiektu]");

if(IsObiektBT[id] == false)
return SendClientMessage(playerid, LIGHTRED, "Nie znaleziono takiego obiektu.");

format(string, sizeof string, "{004080}Obiekt o id {FFFFFF}%d {004080}zosta³ usuniêty.", id);
SendClientMessage(playerid, 0x0, string);
IsObiektBT[id] = false;
DestroyObject(obiektyBT[id]);
DestroyDynamic3DTextLabel(obiektyBT3D[id]);
strmid(obiektBTNAME[obiektyBTID],"nothing",0,20);
strmid(obiektBTNAME2[obiektyBTID],"nothing",0,20);
return 1;
}

CMD:delall(playerid, params[])
{
if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_BUILD)
return SendClientMessage(playerid, LIGHTRED, "Nie masz uprawnieñ.");

SendClientMessage(playerid, 0x0, "{004080}Wszystkie obiekty zosta³ usuniête.");

for(new x = 0; x < MAX_BTOBJTD; x++)
{
IsObiektBT[x] = false;
DestroyObject(obiektyBT[x]);
DestroyDynamic3DTextLabel(obiektyBT3D[x]);
strmid(obiektBTNAME[obiektyBTID],"nothing",0,20);
strmid(obiektBTNAME2[obiektyBTID],"nothing",0,20);
}
return 1;
}

CMD:kamera(playerid, params[])
{
if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_BUILD)
return SendClientMessage(playerid, LIGHTRED, "Nie masz uprawnieñ.");

if(GetPVarType(playerid, "FlyMode"))
{
CancelFlyMode(playerid);
}
else
{
new Float:tp[3];
GetPlayerPos(playerid,tp[0],tp[1],tp[2]);
SetPVarFloat(playerid, "BTP", tp[0]);
SetPVarFloat(playerid, "BTP1", tp[1]);
SetPVarFloat(playerid, "BTP2", tp[2]);
SetPVarInt(playerid, "FLYKAMERA", 1);
FlyMode(playerid);
}
return 1;
}

CMD:obiektname(playerid, params[])
{
new id;
new string[200];

if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_BUILD)
return SendClientMessage(playerid, LIGHTRED, "Nie masz uprawnieñ.");

if(sscanf(params, "d", id))
return SendClientMessage(playerid, LIGHTRED, "Wpisz: /obiektname [id obiektu]");

if(IsObiektBT[id] == false)
return SendClientMessage(playerid, LIGHTRED, "Nie znaleziono takiego obiektu.");

format(string, sizeof string, "{004080}Obiekt o id {FFFFFF}%d {004080}zosta³ postawiony przez {FFFFFF}%s{004080}, edytowany przez {FFFFFF}%s.", id, obiektBTNAME[id], obiektBTNAME2[id]);
SendClientMessage(playerid, 0x0, string);
return 1;
}

CMD:stworzobiekt(playerid, params[])
{
	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_BUILD)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	new szString[674];
	strcat(szString, "Model\tNazwa\n");
	strcat(szString, "2960\tBelka\n944\tSkrzynie\n3287\tBeczka\n1684\tBudka\n3567\t11292\tBudka2\nNaczepa\n1352\nSwiatla\n8873\tJupiter\n16599\tWodol");
	strcat(szString, "3502\tRura\n3675\tRura2\n12930\tRury\n1685\tKostka\n3572\tKontener\n2699\tKontener2\n1358\tŒmietnik\n1372\tŒmietnik2\n18248\tKoparka\n");
	strcat(szString, "3526\tLampka\n3385\tLampka2\n979\tStrza³ka lewo\n978\tStrza³ka prawo\n973\tBarierka\n3091\tBlokada2981\tBlokada\n1237\tPacholek\n");
	strcat(szString, "1238\tPacholek2\n3504\tToiToi\n1322\tZnak\n3380\tZnak2\n3379\tZnak3\n2780\tDym\n974\tBramka\n16302\t¯wir\n10984\tGruz\n16317\tPiach\n");
	strcat(szString, "3578\tDockbar\n934\tGenerator\n1426\tRusztowanie\n1428\tDrabina\n1437\tDrabina2\n3867\tRusztowania\n1226\tLatarnia");

	return Dialog_Show(playerid, DIALOG_OBIEKT, DIALOG_STYLE_TABLIST_HEADERS, "Wybierz obiekt", szString, "Wybierz", "Anuluj");
}

CMD:createlabel(playerid, params[])
{
new string[256];

if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_BUILD)
return SendClientMessage(playerid, LIGHTRED, "Nie masz uprawnieñ.");

if(isnull(params)) return SendClientMessage(playerid, LIGHTRED, "Wpisz: /createlabel [tekst]");

new Float:Pos[3];
GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);
new tekstid = GetFreeRemontBTID();
format(string, sizeof string, "{004080}Build Trans UID {FFFFFF}(%d).\n%s", tekstid, params);
remontBTTD[tekstid] = CreateDynamic3DTextLabel(string, ZIELONY7, Pos[0],Pos[1],Pos[2], 30.0);
format(string, sizeof string, "{004080}Utworzy?etykiet?o id {FFFFFF}(%d).", tekstid);
SendClientMessage(playerid, 0x0, string);
IsRemontBTID[tekstid] = true;
return 1;
}

CMD:deletelabel(playerid, params[])
{
new string[128];

if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_BUILD)
return SendClientMessage(playerid, LIGHTRED, "Nie masz uprawnieñ.");

if(isnull(params)) return SendClientMessage(playerid, LIGHTRED, "Wpisz: /createlabel [tekst]");

new tekstid = strval(params);
DestroyDynamic3DTextLabel(remontBTTD[tekstid]);
IsRemontBTID[tekstid] = false;
format(string, sizeof string, "{004080}Etykieta o id {FFFFFF}(%d) {004080}zosta³usuni?a.", tekstid);
SendClientMessage(playerid, 0x0, string);
return 1;
}

CMD:deletelabels(playerid, params[])
{
if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_BUILD)
return SendClientMessage(playerid, LIGHTRED, "Nie masz uprawnieñ.");

for(new x = 0; x < MAX_BTOBJTD; x++)
{
DestroyDynamic3DTextLabel(remontBTTD[x]);
IsRemontBTID[x] = false;
}
SendClientMessage(playerid, 0x0, "{004080}Etykiety zosta³ usuniête.");
return 1;
}

Dialog:DIALOG_OBIEKT(playerid, response, listitem, inputtext[])
	return 1;
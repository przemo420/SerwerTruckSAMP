enum E_VOTE
{
	voteTimer,
	bool:vote,
	voteYes,
	voteNo,
	Text:voteTD[5]
}

new voteInfo[E_VOTE];

// ---------

forward VoteEnd();

// ---------

forward Vote_OnPlayerDisconnect(playerid, reason);
public Vote_OnPlayerDisconnect(playerid, reason)
{
	DeletePVar(playerid, "voted");
	return 1;
}

forward Vote_OnGameModeInit();
public Vote_OnGameModeInit()
{
	VoteInit();
	return 1;
}


forward Vote_OnGameModeExit();
public Vote_OnGameModeExit()
{
	VoteExit();
}

stock VoteInit()
{
	voteInfo[voteTD][0] = TextDrawCreate(84.000000, 256.000000, "___");
	TextDrawAlignment(voteInfo[voteTD][0], 2);
	TextDrawBackgroundColor(voteInfo[voteTD][0], 255);
	TextDrawFont(voteInfo[voteTD][0], 1);
	TextDrawLetterSize(voteInfo[voteTD][0], 0.500000, 5.200000);
	TextDrawColor(voteInfo[voteTD][0], -1);
	TextDrawSetOutline(voteInfo[voteTD][0], 0);
	TextDrawSetProportional(voteInfo[voteTD][0], 1);
	TextDrawSetShadow(voteInfo[voteTD][0], 1);
	TextDrawUseBox(voteInfo[voteTD][0], 1);
	TextDrawBoxColor(voteInfo[voteTD][0], 125);
	TextDrawTextSize(voteInfo[voteTD][0], 606.000000, -99.000000);

	voteInfo[voteTD][1] = TextDrawCreate(45.000000, 257.000000, "~r~Glosowanie");
	TextDrawBackgroundColor(voteInfo[voteTD][1], 60);
	TextDrawFont(voteInfo[voteTD][1], 2);
	TextDrawLetterSize(voteInfo[voteTD][1], 0.289999, 0.800000);
	TextDrawColor(voteInfo[voteTD][1], -1);
	TextDrawSetOutline(voteInfo[voteTD][1], 1);
	TextDrawSetProportional(voteInfo[voteTD][1], 1);

	voteInfo[voteTD][2] = TextDrawCreate(84.000000, 269.000000, "___");
	TextDrawAlignment(voteInfo[voteTD][2], 2);
	TextDrawBackgroundColor(voteInfo[voteTD][2], 255);
	TextDrawFont(voteInfo[voteTD][2], 1);
	TextDrawLetterSize(voteInfo[voteTD][2], 0.500000, -0.299998);
	TextDrawColor(voteInfo[voteTD][2], -1);
	TextDrawSetOutline(voteInfo[voteTD][2], 0);
	TextDrawSetProportional(voteInfo[voteTD][2], 1);
	TextDrawSetShadow(voteInfo[voteTD][2], 1);
	TextDrawUseBox(voteInfo[voteTD][2], 1);
	TextDrawBoxColor(voteInfo[voteTD][2], 125);
	TextDrawTextSize(voteInfo[voteTD][2], 606.000000, -86.000000);

	voteInfo[voteTD][3] = TextDrawCreate(40.000000, 271.000000, "Czy rudy jest fajnym kolorem?");
	TextDrawBackgroundColor(voteInfo[voteTD][3], 60);
	TextDrawFont(voteInfo[voteTD][3], 2);
	TextDrawLetterSize(voteInfo[voteTD][3], 0.270000, 0.600000);
	TextDrawColor(voteInfo[voteTD][3], -1);
	TextDrawSetOutline(voteInfo[voteTD][3], 1);
	TextDrawSetProportional(voteInfo[voteTD][3], 1);
	TextDrawUseBox(voteInfo[voteTD][3], 1);
	TextDrawBoxColor(voteInfo[voteTD][3], 0);
	TextDrawTextSize(voteInfo[voteTD][3], 128.000000, 61.000000);

	voteInfo[voteTD][4] = TextDrawCreate(42.000000, 296.000000, "~g~0 - tak__~r~0 - nie");
	TextDrawBackgroundColor(voteInfo[voteTD][4], 60);
	TextDrawFont(voteInfo[voteTD][4], 2);
	TextDrawLetterSize(voteInfo[voteTD][4], 0.270000, 0.600000);
	TextDrawColor(voteInfo[voteTD][4], -1);
	TextDrawSetOutline(voteInfo[voteTD][4], 1);
	TextDrawSetProportional(voteInfo[voteTD][4], 1);
}

stock VoteExit()
{
	TextDrawDestroy(voteInfo[voteTD][0]);
	TextDrawDestroy(voteInfo[voteTD][1]);
	TextDrawDestroy(voteInfo[voteTD][2]);
	TextDrawDestroy(voteInfo[voteTD][3]);
}

stock VoteStart(text[], time)
{
	// tutaj trzeba daæ warunek je¿eli jest zalogowany
	TextDrawSetString(voteInfo[voteTD][3], text);
	TextDrawSetString(voteInfo[voteTD][4], "~g~0 - Tak___~r~0 - Nie");

	TextDrawShowForAll(voteInfo[voteTD][0]);
	TextDrawShowForAll(voteInfo[voteTD][1]);
	TextDrawShowForAll(voteInfo[voteTD][2]);
	TextDrawShowForAll(voteInfo[voteTD][3]);
	TextDrawShowForAll(voteInfo[voteTD][4]);

	voteInfo[voteTimer] = SetTimer("VoteEnd", time * 1000, false);
	voteInfo[vote] = true;
}

public VoteEnd()
{
	voteInfo[vote] = false;
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if(!IsPlayerConnected(i))
			continue;
		if(GetPVarInt(i, "voted"))
			DeletePVar(i, "voted");
	}

	TextDrawHideForAll(voteInfo[voteTD][0]);
	TextDrawHideForAll(voteInfo[voteTD][1]);
	TextDrawHideForAll(voteInfo[voteTD][2]);
	TextDrawHideForAll(voteInfo[voteTD][3]);
	TextDrawHideForAll(voteInfo[voteTD][4]);

	new szString[128];
	format(szString, sizeof szString, "G³osowanie zakoñczone rezultatem: {b}%d g³os%s na tak, %d g³os%s na nie.", voteInfo[voteYes], (voteInfo[voteYes] == 1) ? ("") : ("ów"), voteInfo[voteNo], (voteInfo[voteNo] == 1) ? ("") : ("ów"));
	MsgToAll(COLOR_INFO2, szString);
	
	voteInfo[voteYes] = 0;
	voteInfo[voteNo] = 0;
	return 1;
}

stock VoteUpdate()
{
	new szString[64];
	format(szString, sizeof szString, "~g~%d - Tak___~r~%d - Nie", voteInfo[voteYes], voteInfo[voteNo]);
	TextDrawSetString(voteInfo[voteTD][4], szString);
	return 1;
}

CMD:vote(playerid, params[])
{
	new text[24];
	if(sscanf(params, "s[24]", text))
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/vote [tak / nie / start / stop]{/b}");
	if(!strcmp(text, "tak", true))
	{
		if(!voteInfo[vote])
			return Msg(playerid, COLOR_ERROR, "Aktualnie nie ma ¿adnego g³osowania.");
		if(GetPVarInt(playerid, "voted"))
			return Msg(playerid, COLOR_ERROR, "Ju¿ g³osowa³eœ");

		SetPVarInt(playerid, "voted", 1);
		voteInfo[voteYes]++;
		VoteUpdate();
	}
	else if(!strcmp(text, "nie", true))
	{
		if(!voteInfo[vote])
			return Msg(playerid, COLOR_ERROR, "Aktualnie nie ma ¿adnego g³osowania.");
		if(GetPVarInt(playerid, "voted"))
			return Msg(playerid, COLOR_ERROR, "Ju¿ g³osowa³eœ");

		SetPVarInt(playerid, "voted", 1);
		voteInfo[voteNo]++;
		VoteUpdate();
	}
	else if(!strcmp(text, "start", true))
	{
		if(!playerInfo[playerid][pAdmin])
			return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");
		if(voteInfo[vote])
			return Msg(playerid, COLOR_ERROR, "G³osowanie ju¿ istnieje.");

		Dialog_Show(playerid, DIALOG_VOTE, DIALOG_STYLE_INPUT, " ", "WprowadŸ treœæ g³osowania. (maksymalnie 50 znaków)", "Dalej", "Anuluj");
	}
	else if(!strcmp(text, "stop", true))
	{
		if(!playerInfo[playerid][pAdmin])
			return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");
		if(!voteInfo[vote])
			return Msg(playerid, COLOR_ERROR, "Aktualnie nie ma ¿adnego g³osowania.");

		VoteEnd();
		KillTimer(voteInfo[voteTimer]);
	}
	else
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/vote [tak / nie / start / stop]{/b}");
	return 1;
}

CMD:tak(playerid)
{
	cmd_vote(playerid, "tak");
	return 1;
}

CMD:nie(playerid)
{
	cmd_vote(playerid, "nie");
	return 1;
}

Dialog:DIALOG_VOTE(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;
	if(strlen(inputtext) < 5 || strlen(inputtext) > 50)
		return Dialog_Show(playerid, DIALOG_VOTE, DIALOG_STYLE_INPUT, " ", "WprowadŸ treœæ g³osowania. (maksymalnie 50 znaków)\nWprowadzona treœæ jest za krótka albo d³uga.", "Dalej", "Anuluj");
	
	SetPVarString(playerid, "voteQuest", inputtext);
	Dialog_Show(playerid, DIALOG_VOTE2, DIALOG_STYLE_INPUT, " ", "WprowadŸ czas trwania g³osowania w sekundach.", "Start", "Wstecz");
	return 1;
}

Dialog:DIALOG_VOTE2(playerid, response, listitem, inputtext[])
{
	if(!response)
		return Dialog_Show(playerid, DIALOG_VOTE, DIALOG_STYLE_INPUT, " ", "WprowadŸ treœæ g³osowania. (maksymalnie 50 znaków)", "Dalej", "Anuluj");
	if(strval(inputtext) < 10 || strval(inputtext) > 300)
		return Dialog_Show(playerid, DIALOG_VOTE2, DIALOG_STYLE_INPUT, " ", "WprowadŸ czas trwania g³osowania w sekundach.\nCzas musi siê znajdowaæ w przedziale 10 - 500 sekund.", "Start", "Wstecz");
	
	new text[56];
	GetPVarString(playerid, "voteQuest", text, sizeof text);
	VoteStart(text, strval(inputtext));
	DeletePVar(playerid, "voteQuest");
	return 1;
}
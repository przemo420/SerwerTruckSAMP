
enum {
	PlayerText:SPECTACTOR_TEXTDRAW_BOX,
	PlayerText:SPECTACTOR_TEXTDRAW_BOX2,
	PlayerText:SPECTACTOR_TEXTDRAW_INFO,
	PlayerText:SPECTACTOR_TEXTDRAW_INFO2,
	SPECTACTOR_TIMER
};
new SpectactorsTD[MAX_PLAYERS][5];

stock SpectactorTextDraw(playerid, create=false)
{
	if(create)
	{
		SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX] = CreatePlayerTextDraw(playerid, 320.000000, 8.000000, "__");
		PlayerTextDrawAlignment(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX], 2);
		PlayerTextDrawBackgroundColor(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX], 255);
		PlayerTextDrawFont(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX], 1);
		PlayerTextDrawLetterSize(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX], 0.500000, 5.000000);
		PlayerTextDrawColor(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX], -1);
		PlayerTextDrawSetOutline(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX], 0);
		PlayerTextDrawSetProportional(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX], 1);
		PlayerTextDrawSetShadow(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX], 1);
		PlayerTextDrawUseBox(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX], 1);
		PlayerTextDrawBoxColor(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX], 125);
		PlayerTextDrawTextSize(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX], 50.000000, -220.000000);

		SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO] = CreatePlayerTextDraw(playerid, 215.000000, 8.000000, "ID:~n~Nick:~n~Kasa:~n~Score:~n~HP:~n~HP Poj:~n~Predkosc:");
		PlayerTextDrawBackgroundColor(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO], 255);
		PlayerTextDrawFont(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO], 2);
		PlayerTextDrawLetterSize(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO], 0.270000, 0.699999);
		PlayerTextDrawColor(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO], -1);
		PlayerTextDrawSetOutline(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO], 0);
		PlayerTextDrawSetProportional(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO], 1);
		PlayerTextDrawSetShadow(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO], 1);

		PlayerTextDrawShow(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX]);
		PlayerTextDrawShow(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO]);

		SpectactorsTD[playerid][SPECTACTOR_TIMER] = SetTimerEx("SpectactorUpdate", 250, true, "d", playerid);
	}
	else
	{
		PlayerTextDrawHide(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX]);
		PlayerTextDrawHide(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO]);

		PlayerTextDrawDestroy(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX]);
		PlayerTextDrawDestroy(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO]);

		KillTimer(SpectactorsTD[playerid][SPECTACTOR_TIMER]);

		TogglePlayerSpectating(playerid, false);
	}

	return 1;
}

enum Main_TD 
{
	Text:Time,
	Text:TruckerSkins[9]
}

new MainTextDraws[Main_TD];
new MainTextDrawTimer;

CreateMainTextDraws(create)
{
	if(create)
	{
		MainTextDraws[Time] = TextDrawCreate(572.000000, 22.000000, "23:59");
		TextDrawAlignment(MainTextDraws[Time], 2);
		TextDrawBackgroundColor(MainTextDraws[Time], 65);
		TextDrawFont(MainTextDraws[Time], 3);
		TextDrawLetterSize(MainTextDraws[Time], 0.500000, 1.800000);
		TextDrawColor(MainTextDraws[Time], -1);
		TextDrawSetOutline(MainTextDraws[Time], 1);
		TextDrawSetProportional(MainTextDraws[Time], 1);

		MainTextDrawTimer = SetTimer("OneSecTimer", 1000, true);

		MainTextDraws[TruckerSkins][0] = TextDrawCreate(156.000000, 163.000000, "_");
		TextDrawBackgroundColor(MainTextDraws[TruckerSkins][0], 255);
		TextDrawFont(MainTextDraws[TruckerSkins][0], 1);
		TextDrawLetterSize(MainTextDraws[TruckerSkins][0], 0.629999, 18.200002);
		TextDrawColor(MainTextDraws[TruckerSkins][0], -1);
		TextDrawSetOutline(MainTextDraws[TruckerSkins][0], 0);
		TextDrawSetProportional(MainTextDraws[TruckerSkins][0], 1);
		TextDrawSetShadow(MainTextDraws[TruckerSkins][0], 1);
		TextDrawUseBox(MainTextDraws[TruckerSkins][0], 1);
		TextDrawBoxColor(MainTextDraws[TruckerSkins][0], 119);
		TextDrawTextSize(MainTextDraws[TruckerSkins][0], 490.000000, 82.000000);
		TextDrawSetSelectable(MainTextDraws[TruckerSkins][0], 0);

		MainTextDraws[TruckerSkins][1] = TextDrawCreate(166.000000, 247.000000, "SKIN: 1");
		TextDrawBackgroundColor(MainTextDraws[TruckerSkins][1], 0xFFFFFF00);
		TextDrawFont(MainTextDraws[TruckerSkins][1], 5);
		TextDrawLetterSize(MainTextDraws[TruckerSkins][1], 0.500000, 1.000000);
		TextDrawColor(MainTextDraws[TruckerSkins][1], -1);
		TextDrawSetOutline(MainTextDraws[TruckerSkins][1], 0);
		TextDrawSetProportional(MainTextDraws[TruckerSkins][1], 1);
		TextDrawSetShadow(MainTextDraws[TruckerSkins][1], 1);
		TextDrawUseBox(MainTextDraws[TruckerSkins][1], 1);
		TextDrawBoxColor(MainTextDraws[TruckerSkins][1], 0xFFFFFF00);
		TextDrawTextSize(MainTextDraws[TruckerSkins][1], 38.000000, 69.000000);
		TextDrawSetSelectable(MainTextDraws[TruckerSkins][1], 1);
		TextDrawSetPreviewModel(MainTextDraws[TruckerSkins][1], 1);

		MainTextDraws[TruckerSkins][2] = TextDrawCreate(213.000000, 247.000000, "SKIN: 7");
		TextDrawBackgroundColor(MainTextDraws[TruckerSkins][2], 0xFFFFFF00);
		TextDrawFont(MainTextDraws[TruckerSkins][2], 5);
		TextDrawLetterSize(MainTextDraws[TruckerSkins][2], 0.500000, 1.000000);
		TextDrawColor(MainTextDraws[TruckerSkins][2], -1);
		TextDrawSetOutline(MainTextDraws[TruckerSkins][2], 0);
		TextDrawSetProportional(MainTextDraws[TruckerSkins][2], 1);
		TextDrawSetShadow(MainTextDraws[TruckerSkins][2], 1);
		TextDrawUseBox(MainTextDraws[TruckerSkins][2], 1);
		TextDrawBoxColor(MainTextDraws[TruckerSkins][2], 0xFFFFFF00);
		TextDrawTextSize(MainTextDraws[TruckerSkins][2], 38.000000, 69.000000);
		TextDrawSetSelectable(MainTextDraws[TruckerSkins][2], 1);
		TextDrawSetPreviewModel(MainTextDraws[TruckerSkins][2], 7);

		MainTextDraws[TruckerSkins][3] = TextDrawCreate(261.000000, 247.000000, "SKIN: 12");
		TextDrawBackgroundColor(MainTextDraws[TruckerSkins][3], 0xFFFFFF00);
		TextDrawFont(MainTextDraws[TruckerSkins][3], 5);
		TextDrawLetterSize(MainTextDraws[TruckerSkins][3], 0.500000, 1.000000);
		TextDrawColor(MainTextDraws[TruckerSkins][3], -1);
		TextDrawSetOutline(MainTextDraws[TruckerSkins][3], 0);
		TextDrawSetProportional(MainTextDraws[TruckerSkins][3], 1);
		TextDrawSetShadow(MainTextDraws[TruckerSkins][3], 1);
		TextDrawUseBox(MainTextDraws[TruckerSkins][3], 1);
		TextDrawBoxColor(MainTextDraws[TruckerSkins][3], 0xFFFFFF00);
		TextDrawTextSize(MainTextDraws[TruckerSkins][3], 38.000000, 69.000000);
		TextDrawSetSelectable(MainTextDraws[TruckerSkins][3], 1);
		TextDrawSetPreviewModel(MainTextDraws[TruckerSkins][3], 12);

		MainTextDraws[TruckerSkins][4] = TextDrawCreate(302.000000, 247.000000, "SKIN: 37");
		TextDrawBackgroundColor(MainTextDraws[TruckerSkins][4], 0xFFFFFF00);
		TextDrawFont(MainTextDraws[TruckerSkins][4], 5);
		TextDrawLetterSize(MainTextDraws[TruckerSkins][4], 0.500000, 1.000000);
		TextDrawColor(MainTextDraws[TruckerSkins][4], -1);
		TextDrawSetOutline(MainTextDraws[TruckerSkins][4], 0);
		TextDrawSetProportional(MainTextDraws[TruckerSkins][4], 1);
		TextDrawSetShadow(MainTextDraws[TruckerSkins][4], 1);
		TextDrawUseBox(MainTextDraws[TruckerSkins][4], 1);
		TextDrawBoxColor(MainTextDraws[TruckerSkins][4], 0xFFFFFF00);
		TextDrawTextSize(MainTextDraws[TruckerSkins][4], 38.000000, 69.000000);
		TextDrawSetSelectable(MainTextDraws[TruckerSkins][4], 1);
		TextDrawSetPreviewModel(MainTextDraws[TruckerSkins][4], 37);

		MainTextDraws[TruckerSkins][5] = TextDrawCreate(344.000000, 247.000000, "SKIN: 98");
		TextDrawBackgroundColor(MainTextDraws[TruckerSkins][5], 0xFFFFFF00);
		TextDrawFont(MainTextDraws[TruckerSkins][5], 5);
		TextDrawLetterSize(MainTextDraws[TruckerSkins][5], 0.500000, 1.000000);
		TextDrawColor(MainTextDraws[TruckerSkins][5], -1);
		TextDrawSetOutline(MainTextDraws[TruckerSkins][5], 0);
		TextDrawSetProportional(MainTextDraws[TruckerSkins][5], 1);
		TextDrawSetShadow(MainTextDraws[TruckerSkins][5], 1);
		TextDrawUseBox(MainTextDraws[TruckerSkins][5], 1);
		TextDrawBoxColor(MainTextDraws[TruckerSkins][5], 0xFFFFFF00);
		TextDrawTextSize(MainTextDraws[TruckerSkins][5], 38.000000, 69.000000);
		TextDrawSetSelectable(MainTextDraws[TruckerSkins][5], 1);
		TextDrawSetPreviewModel(MainTextDraws[TruckerSkins][5], 98);

		MainTextDraws[TruckerSkins][6] = TextDrawCreate(393.000000, 247.000000, "SKIN: 101");
		TextDrawBackgroundColor(MainTextDraws[TruckerSkins][6], 0xFFFFFF00);
		TextDrawFont(MainTextDraws[TruckerSkins][6], 5);
		TextDrawLetterSize(MainTextDraws[TruckerSkins][6], 0.500000, 1.000000);
		TextDrawColor(MainTextDraws[TruckerSkins][6], -1);
		TextDrawSetOutline(MainTextDraws[TruckerSkins][6], 0);
		TextDrawSetProportional(MainTextDraws[TruckerSkins][6], 1);
		TextDrawSetShadow(MainTextDraws[TruckerSkins][6], 1);
		TextDrawUseBox(MainTextDraws[TruckerSkins][6], 1);
		TextDrawBoxColor(MainTextDraws[TruckerSkins][6], 0xFFFFFF00);
		TextDrawTextSize(MainTextDraws[TruckerSkins][6], 38.000000, 69.000000);
		TextDrawSetSelectable(MainTextDraws[TruckerSkins][6], 1);
		TextDrawSetPreviewModel(MainTextDraws[TruckerSkins][6], 101);

		MainTextDraws[TruckerSkins][7] = TextDrawCreate(442.000000, 247.000000, "SKIN: 192");
		TextDrawBackgroundColor(MainTextDraws[TruckerSkins][7], 0xFFFFFF00);
		TextDrawFont(MainTextDraws[TruckerSkins][7], 5);
		TextDrawLetterSize(MainTextDraws[TruckerSkins][7], 0.500000, 1.000000);
		TextDrawColor(MainTextDraws[TruckerSkins][7], -1);
		TextDrawSetOutline(MainTextDraws[TruckerSkins][7], 0);
		TextDrawSetProportional(MainTextDraws[TruckerSkins][7], 1);
		TextDrawSetShadow(MainTextDraws[TruckerSkins][7], 1);
		TextDrawUseBox(MainTextDraws[TruckerSkins][7], 1);
		TextDrawBoxColor(MainTextDraws[TruckerSkins][7], 0xFFFFFF00);
		TextDrawTextSize(MainTextDraws[TruckerSkins][7], 38.000000, 69.000000);
		TextDrawSetSelectable(MainTextDraws[TruckerSkins][7], 1);
		TextDrawSetPreviewModel(MainTextDraws[TruckerSkins][7], 192);

		MainTextDraws[TruckerSkins][8] = TextDrawCreate(326.000000, 182.000000, "Wybierz podstawowego skina.~n~Pamietaj, ze nie bedziesz mogl go zmienic.");
		TextDrawAlignment(MainTextDraws[TruckerSkins][8], 2);
		TextDrawBackgroundColor(MainTextDraws[TruckerSkins][8], 255);
		TextDrawFont(MainTextDraws[TruckerSkins][8], 1);
		TextDrawLetterSize(MainTextDraws[TruckerSkins][8], 0.390000, 1.400000);
		TextDrawColor(MainTextDraws[TruckerSkins][8], -1);
		TextDrawSetOutline(MainTextDraws[TruckerSkins][8], 0);
		TextDrawSetProportional(MainTextDraws[TruckerSkins][8], 1);
		TextDrawSetShadow(MainTextDraws[TruckerSkins][8], 1);
		TextDrawSetSelectable(MainTextDraws[TruckerSkins][8], 0);
	}
	else
	{
		TextDrawHideForAll(MainTextDraws[Time]);

		TextDrawDestroy(MainTextDraws[Time]);

		for(new i; i <=8; i++)
		{
			TextDrawHideForAll(MainTextDraws[TruckerSkins][i]);
			TextDrawDestroy(MainTextDraws[TruckerSkins][i]);
		}

		KillTimer(MainTextDrawTimer);
	}
}

new Text:connectStableBox[8],
	PlayerText:connectVarBox[MAX_PLAYERS][6];

stock InitConnect()
{	
	connectStableBox[0] = TextDrawCreate(0.000000, 0.000000, "______");
	TextDrawBackgroundColor(connectStableBox[0], 255);
	TextDrawFont(connectStableBox[0], 1);
	TextDrawLetterSize(connectStableBox[0], 0.500000, 10.000000);
	TextDrawColor(connectStableBox[0], -1);
	TextDrawSetOutline(connectStableBox[0], 0);
	TextDrawSetProportional(connectStableBox[0], 1);
	TextDrawSetShadow(connectStableBox[0], 1);
	TextDrawUseBox(connectStableBox[0], 1);
	TextDrawBoxColor(connectStableBox[0], 125);
	TextDrawTextSize(connectStableBox[0], 660.000000, 0.000000);

	connectStableBox[1] = TextDrawCreate(0.000000, 359.000000, "______");
	TextDrawBackgroundColor(connectStableBox[1], 255);
	TextDrawFont(connectStableBox[1], 1);
	TextDrawLetterSize(connectStableBox[1], 0.500000, 10.000000);
	TextDrawColor(connectStableBox[1], -1);
	TextDrawSetOutline(connectStableBox[1], 0);
	TextDrawSetProportional(connectStableBox[1], 1);
	TextDrawSetShadow(connectStableBox[1], 1);
	TextDrawUseBox(connectStableBox[1], 1);
	TextDrawBoxColor(connectStableBox[1], 125);
	TextDrawTextSize(connectStableBox[1], 660.000000, 0.000000);

	connectStableBox[2] = TextDrawCreate(51.000000, 175.000000, "______");
	TextDrawBackgroundColor(connectStableBox[2], 255);
	TextDrawFont(connectStableBox[2], 1);
	TextDrawLetterSize(connectStableBox[2], 0.500000, 11.200000);
	TextDrawColor(connectStableBox[2], -1);
	TextDrawSetOutline(connectStableBox[2], 0);
	TextDrawSetProportional(connectStableBox[2], 1);
	TextDrawSetShadow(connectStableBox[2], 1);
	TextDrawUseBox(connectStableBox[2], 1);
	TextDrawBoxColor(connectStableBox[2], 125);
	TextDrawTextSize(connectStableBox[2], 130.000000, -30.000000);

	connectStableBox[3] = TextDrawCreate(278.000000, 36.000000, "serwer truck");
	TextDrawBackgroundColor(connectStableBox[3], 255);
	TextDrawFont(connectStableBox[3], 2);
	TextDrawLetterSize(connectStableBox[3], 0.369998, 1.600000);
	TextDrawColor(connectStableBox[3], -1);
	TextDrawSetOutline(connectStableBox[3], 1);
	TextDrawSetProportional(connectStableBox[3], 1);

	connectStableBox[4] = TextDrawCreate(244.000000, 51.000000, "-");
	TextDrawBackgroundColor(connectStableBox[4], 255);
	TextDrawFont(connectStableBox[4], 2);
	TextDrawLetterSize(connectStableBox[4], 15.000000, 0.599999);
	TextDrawColor(connectStableBox[4], -10399850);
	TextDrawSetOutline(connectStableBox[4], 0);
	TextDrawSetProportional(connectStableBox[4], 1);
	TextDrawSetShadow(connectStableBox[4], 0);

	connectStableBox[5] = TextDrawCreate(292.000000, 56.000000, "najlepszy polski");
	TextDrawBackgroundColor(connectStableBox[5], 255);
	TextDrawFont(connectStableBox[5], 2);
	TextDrawLetterSize(connectStableBox[5], 0.219999, 0.699999);
	TextDrawColor(connectStableBox[5], -1);
	TextDrawSetOutline(connectStableBox[5], 1);
	TextDrawSetProportional(connectStableBox[5], 1);

	connectStableBox[6] = TextDrawCreate(291.000000, 63.000000, "serwer ~r~truckerski");
	TextDrawBackgroundColor(connectStableBox[6], 255);
	TextDrawFont(connectStableBox[6], 2);
	TextDrawLetterSize(connectStableBox[6], 0.209998, 0.600000);
	TextDrawColor(connectStableBox[6], -1);
	TextDrawSetOutline(connectStableBox[6], 1);
	TextDrawSetProportional(connectStableBox[6], 1);

	connectStableBox[7] = TextDrawCreate(517.000000, 431.000000, "forum: serwertruck.eu~n~ts3: 178.217.190.46");
	TextDrawBackgroundColor(connectStableBox[7], 255);
	TextDrawFont(connectStableBox[7], 2);
	TextDrawLetterSize(connectStableBox[7], 0.219999, 0.699999);
	TextDrawColor(connectStableBox[7], -1);
	TextDrawSetOutline(connectStableBox[7], 1);
	TextDrawSetProportional(connectStableBox[7], 1);
}

stock ShowPlayerConnect(playerid, var = true)
{

	if(!var)
		for(new i = 0; i < 6; i++)
			PlayerTextDrawHide(playerid, connectVarBox[playerid][i]), PlayerTextDrawDestroy(playerid, connectVarBox[playerid][i]);
	else
	{
		connectVarBox[playerid][0] = CreatePlayerTextDraw(playerid, 56.000000, 183.000000, "Logowanie");
		PlayerTextDrawBackgroundColor(playerid, connectVarBox[playerid][0], 255);
		PlayerTextDrawFont(playerid, connectVarBox[playerid][0], 2);
		PlayerTextDrawLetterSize(playerid, connectVarBox[playerid][0], 0.239999, 0.900000);
		PlayerTextDrawColor(playerid, connectVarBox[playerid][0], -1);
		PlayerTextDrawSetOutline(playerid, connectVarBox[playerid][0], 1);
		PlayerTextDrawSetProportional(playerid, connectVarBox[playerid][0], 1);
		PlayerTextDrawUseBox(playerid, connectVarBox[playerid][0], 1);
		PlayerTextDrawBoxColor(playerid, connectVarBox[playerid][0], 0);
		PlayerTextDrawTextSize(playerid, connectVarBox[playerid][0], 116.000000, 10.000000);
		PlayerTextDrawSetSelectable(playerid, connectVarBox[playerid][0], 1);

		connectVarBox[playerid][1] = CreatePlayerTextDraw(playerid, 56.000000, 194.000000, "~r~Rejestracja");
		PlayerTextDrawBackgroundColor(playerid, connectVarBox[playerid][1], 255);
		PlayerTextDrawFont(playerid, connectVarBox[playerid][1], 2);
		PlayerTextDrawLetterSize(playerid, connectVarBox[playerid][1], 0.239999, 0.900000);
		PlayerTextDrawColor(playerid, connectVarBox[playerid][1], -1);
		PlayerTextDrawSetOutline(playerid, connectVarBox[playerid][1], 1);
		PlayerTextDrawSetProportional(playerid, connectVarBox[playerid][1], 1);
		PlayerTextDrawUseBox(playerid, connectVarBox[playerid][1], 1);
		PlayerTextDrawBoxColor(playerid, connectVarBox[playerid][1], 0);
		PlayerTextDrawTextSize(playerid, connectVarBox[playerid][1], 127.000000, 10.000000);
		PlayerTextDrawSetSelectable(playerid, connectVarBox[playerid][1], 1);

		connectVarBox[playerid][2] = CreatePlayerTextDraw(playerid, 56.000000, 205.000000, "Changelog");
		PlayerTextDrawBackgroundColor(playerid, connectVarBox[playerid][2], 255);
		PlayerTextDrawFont(playerid, connectVarBox[playerid][2], 2);
		PlayerTextDrawLetterSize(playerid, connectVarBox[playerid][2], 0.239999, 0.900000);
		PlayerTextDrawColor(playerid, connectVarBox[playerid][2], -1);
		PlayerTextDrawSetOutline(playerid, connectVarBox[playerid][2], 1);
		PlayerTextDrawSetProportional(playerid, connectVarBox[playerid][2], 1);
		PlayerTextDrawUseBox(playerid, connectVarBox[playerid][2], 1);
		PlayerTextDrawBoxColor(playerid, connectVarBox[playerid][2], 0);
		PlayerTextDrawTextSize(playerid, connectVarBox[playerid][2], 119.000000, 10.000000);
		PlayerTextDrawSetSelectable(playerid, connectVarBox[playerid][2], 1);

		connectVarBox[playerid][3] = CreatePlayerTextDraw(playerid, 56.000000, 216.000000, "Pomoc");
		PlayerTextDrawBackgroundColor(playerid, connectVarBox[playerid][3], 255);
		PlayerTextDrawFont(playerid, connectVarBox[playerid][3], 2);
		PlayerTextDrawLetterSize(playerid, connectVarBox[playerid][3], 0.239999, 0.900000);
		PlayerTextDrawColor(playerid, connectVarBox[playerid][3], -1);
		PlayerTextDrawSetOutline(playerid, connectVarBox[playerid][3], 1);
		PlayerTextDrawSetProportional(playerid, connectVarBox[playerid][3], 1);
		PlayerTextDrawUseBox(playerid, connectVarBox[playerid][3], 1);
		PlayerTextDrawBoxColor(playerid, connectVarBox[playerid][3], 0);
		PlayerTextDrawTextSize(playerid, connectVarBox[playerid][3], 93.000000, 10.000000);
		PlayerTextDrawSetSelectable(playerid, connectVarBox[playerid][3], 1);

		connectVarBox[playerid][4] = CreatePlayerTextDraw(playerid, 56.000000, 226.000000, "Autorzy");
		PlayerTextDrawBackgroundColor(playerid, connectVarBox[playerid][4], 255);
		PlayerTextDrawFont(playerid, connectVarBox[playerid][4], 2);
		PlayerTextDrawLetterSize(playerid, connectVarBox[playerid][4], 0.239999, 0.900000);
		PlayerTextDrawColor(playerid, connectVarBox[playerid][4], -1);
		PlayerTextDrawSetOutline(playerid, connectVarBox[playerid][4], 1);
		PlayerTextDrawSetProportional(playerid, connectVarBox[playerid][4], 1);
		PlayerTextDrawUseBox(playerid, connectVarBox[playerid][4], 1);
		PlayerTextDrawBoxColor(playerid, connectVarBox[playerid][4], 0);
		PlayerTextDrawTextSize(playerid, connectVarBox[playerid][4], 103.000000, 10.000000);
		PlayerTextDrawSetSelectable(playerid, connectVarBox[playerid][4], 1);

		connectVarBox[playerid][5] = CreatePlayerTextDraw(playerid, 56.000000, 263.000000, "~r~Rozlacz");
		PlayerTextDrawBackgroundColor(playerid, connectVarBox[playerid][5], 255);
		PlayerTextDrawFont(playerid, connectVarBox[playerid][5], 2);
		PlayerTextDrawLetterSize(playerid, connectVarBox[playerid][5], 0.239999, 0.900000);
		PlayerTextDrawColor(playerid, connectVarBox[playerid][5], -1);
		PlayerTextDrawSetOutline(playerid, connectVarBox[playerid][5], 1);
		PlayerTextDrawSetProportional(playerid, connectVarBox[playerid][5], 1);
		PlayerTextDrawUseBox(playerid, connectVarBox[playerid][5], 1);
		PlayerTextDrawBoxColor(playerid, connectVarBox[playerid][5], 0);
		PlayerTextDrawTextSize(playerid, connectVarBox[playerid][5], 100.000000, 10.000000);
		PlayerTextDrawSetSelectable(playerid, connectVarBox[playerid][5], 1);
		
		for(new i = 0; i < 6; i++)
			PlayerTextDrawShow(playerid, connectVarBox[playerid][i]);
	}
	return 1;
}

stock ShowConnect(playerid, var = true)
{
	if(!var)
		for(new i = 0; i < 8; i++)
			TextDrawHideForPlayer(playerid, connectStableBox[i]);
	else
		for(new i = 0; i < 8; i++)
			TextDrawShowForPlayer(playerid, connectStableBox[i]);
	return 1;
}

enum E_SPEEDO
{
	PlayerText:TD_CAR_NAME[MAX_PLAYERS],
	PlayerText:TD_CAR_INFO[MAX_PLAYERS],
	PlayerText:TD_CAR_FUEL[MAX_PLAYERS],
	PlayerText:TD_CAR_BELTS[MAX_PLAYERS],
	PlayerText:TD_CAR_TOLL[MAX_PLAYERS],
	PlayerText:TD_CAR_SPEED[MAX_PLAYERS]
}

enum E_RADAR
{
	PlayerText:TD_RD_MAIN[MAX_PLAYERS],
	PlayerText:TD_RD_NAME[MAX_PLAYERS],
	PlayerText:TD_RD_LINE[MAX_PLAYERS],
	PlayerText:TD_RD_SPEED[MAX_PLAYERS]
}

enum E_TEXTDRAWS
{
	Text:tdMainBox,
	Text:tdMainIcons[2],
	PlayerBar:tdLevelProgress[MAX_PLAYERS],
	PlayerBar:tdHungerProgress[MAX_PLAYERS],
	PlayerText:tdInfoBox[MAX_PLAYERS],
	PlayerText:tdInfoText[MAX_PLAYERS],
	Text:tdMainTacho[3],
	PlayerText:tdInfoTacho[MAX_PLAYERS],
	Text:tdMainSpeedo[3],
	PlayerText:tdInfoSpeedo[E_SPEEDO],
	PlayerText:tdInfoRadar[E_RADAR]
}

new hudInfo[E_TEXTDRAWS];

stock InitStableHud()
{
	hudInfo[tdMainBox] = TextDrawCreate(554.000000, 106.000000, "___");
	TextDrawAlignment(hudInfo[tdMainBox], 2);
	TextDrawBackgroundColor(hudInfo[tdMainBox], 255);
	TextDrawFont(hudInfo[tdMainBox], 1);
	TextDrawLetterSize(hudInfo[tdMainBox], 0.500000, 2.999999);
	TextDrawColor(hudInfo[tdMainBox], -1);
	TextDrawSetOutline(hudInfo[tdMainBox], 0);
	TextDrawSetProportional(hudInfo[tdMainBox], 1);
	TextDrawSetShadow(hudInfo[tdMainBox], 1);
	TextDrawUseBox(hudInfo[tdMainBox], 1);
	TextDrawBoxColor(hudInfo[tdMainBox], 75);
	TextDrawTextSize(hudInfo[tdMainBox], 606.000000, -87.000000);

	hudInfo[tdMainIcons][0] = TextDrawCreate(515.000000, 121.000000, "hud:radar_burgershot");
	TextDrawAlignment(hudInfo[tdMainIcons][0], 2);
	TextDrawBackgroundColor(hudInfo[tdMainIcons][0], 60);
	TextDrawFont(hudInfo[tdMainIcons][0], 4);
	TextDrawLetterSize(hudInfo[tdMainIcons][0], 0.270000, 0.600000);
	TextDrawColor(hudInfo[tdMainIcons][0], -1);
	TextDrawSetOutline(hudInfo[tdMainIcons][0], 1);
	TextDrawSetProportional(hudInfo[tdMainIcons][0], 1);
	TextDrawUseBox(hudInfo[tdMainIcons][0], 1);
	TextDrawBoxColor(hudInfo[tdMainIcons][0], 255);
	TextDrawTextSize(hudInfo[tdMainIcons][0], 12.000000, 9.000000);

	hudInfo[tdMainIcons][1] = TextDrawCreate(526.000000, 109.000000, "][");
	TextDrawAlignment(hudInfo[tdMainIcons][1], 2);
	TextDrawBackgroundColor(hudInfo[tdMainIcons][1], 60);
	TextDrawFont(hudInfo[tdMainIcons][1], 2);
	TextDrawLetterSize(hudInfo[tdMainIcons][1], 0.349998, 1.100000);
	TextDrawColor(hudInfo[tdMainIcons][1], -1);
	TextDrawSetOutline(hudInfo[tdMainIcons][1], 1);
	TextDrawSetProportional(hudInfo[tdMainIcons][1], 1);
	return 1;
}

stock InitHud(playerid)
{	
	hudInfo[tdInfoBox][playerid] = CreatePlayerTextDraw(playerid, 619.000000, 5.000000, "___");
	PlayerTextDrawAlignment(playerid, hudInfo[tdInfoBox][playerid], 2);
	PlayerTextDrawBackgroundColor(playerid, hudInfo[tdInfoBox][playerid], 255);
	PlayerTextDrawFont(playerid, hudInfo[tdInfoBox][playerid], 1);
	PlayerTextDrawLetterSize(playerid, hudInfo[tdInfoBox][playerid], 0.500000, 1.600000);
	PlayerTextDrawColor(playerid, hudInfo[tdInfoBox][playerid], -1);
	PlayerTextDrawSetOutline(playerid, hudInfo[tdInfoBox][playerid], 0);
	PlayerTextDrawSetProportional(playerid, hudInfo[tdInfoBox][playerid], 1);
	PlayerTextDrawSetShadow(playerid, hudInfo[tdInfoBox][playerid], 1);
	PlayerTextDrawUseBox(playerid, hudInfo[tdInfoBox][playerid], 1);
	PlayerTextDrawBoxColor(playerid, hudInfo[tdInfoBox][playerid], 125);
	PlayerTextDrawTextSize(playerid, hudInfo[tdInfoBox][playerid], 606.000000, -39.000000);

	hudInfo[tdInfoText][playerid] = CreatePlayerTextDraw(playerid, 605.000000, 6.000000, "~r~FPS: ~w~400~n~~r~Ping: ~w~500");
	PlayerTextDrawBackgroundColor(playerid, hudInfo[tdInfoText][playerid], 60);
	PlayerTextDrawFont(playerid, hudInfo[tdInfoText][playerid], 2);
	PlayerTextDrawLetterSize(playerid, hudInfo[tdInfoText][playerid], 0.140000, 0.699999);
	PlayerTextDrawColor(playerid, hudInfo[tdInfoText][playerid], -1);
	PlayerTextDrawSetOutline(playerid, hudInfo[tdInfoText][playerid], 1);
	PlayerTextDrawSetProportional(playerid, hudInfo[tdInfoText][playerid], 1);

	hudInfo[tdLevelProgress][playerid] = CreatePlayerProgressBar(playerid, 531.00, 112.00, 58.50, 4.19, 1131010559, 0.0);
	hudInfo[tdHungerProgress][playerid] = CreatePlayerProgressBar(playerid, 531.00, 124.00, 58.50, 4.19, -642563073, 0.0);
	
	ShowPlayerProgressBar(playerid, hudInfo[tdLevelProgress][playerid]);
	SetPlayerProgressBarMaxValue(playerid, hudInfo[tdLevelProgress][playerid], 100.0);

	ShowPlayerProgressBar(playerid, hudInfo[tdHungerProgress][playerid]);
	SetPlayerProgressBarMaxValue(playerid, hudInfo[tdHungerProgress][playerid], 100.0);
	return 1;
}

stock ShowPlayerHud(playerid, var = true)
{
	if(!var)
	{
		TextDrawHideForPlayer(playerid, hudInfo[tdMainBox]);
		TextDrawHideForPlayer(playerid, hudInfo[tdMainIcons][0]);
		TextDrawHideForPlayer(playerid, hudInfo[tdMainIcons][1]);
		PlayerTextDrawHide(playerid, hudInfo[tdInfoBox][playerid]);
		PlayerTextDrawHide(playerid, hudInfo[tdInfoText][playerid]);
		ExitHud(playerid);
	}
	else
	{
		TextDrawShowForPlayer(playerid, hudInfo[tdMainBox]);
		TextDrawShowForPlayer(playerid, hudInfo[tdMainIcons][0]);
		TextDrawShowForPlayer(playerid, hudInfo[tdMainIcons][1]);
		InitHud(playerid);
		PlayerTextDrawShow(playerid, hudInfo[tdInfoBox][playerid]);
		PlayerTextDrawShow(playerid, hudInfo[tdInfoText][playerid]);
	}
	return 1;
}

stock ExitHud(playerid)
{	
	PlayerTextDrawDestroy(playerid, hudInfo[tdInfoBox][playerid]);
	PlayerTextDrawDestroy(playerid, hudInfo[tdInfoText][playerid]);
	
	DestroyPlayerProgressBar(playerid, hudInfo[tdLevelProgress][playerid]);
	DestroyPlayerProgressBar(playerid, hudInfo[tdHungerProgress][playerid]);
	return 1;
}

stock InitStableTachograph()
{
	hudInfo[tdMainTacho][0] = TextDrawCreate(585.000000, 357.000000, "___");
	TextDrawAlignment(hudInfo[tdMainTacho][0], 2);
	TextDrawBackgroundColor(hudInfo[tdMainTacho][0], 255);
	TextDrawFont(hudInfo[tdMainTacho][0], 1);
	TextDrawLetterSize(hudInfo[tdMainTacho][0], 0.500000, 2.599999);
	TextDrawColor(hudInfo[tdMainTacho][0], -1);
	TextDrawSetOutline(hudInfo[tdMainTacho][0], 0);
	TextDrawSetProportional(hudInfo[tdMainTacho][0], 1);
	TextDrawSetShadow(hudInfo[tdMainTacho][0], 1);
	TextDrawUseBox(hudInfo[tdMainTacho][0], 1);
	TextDrawBoxColor(hudInfo[tdMainTacho][0], 125);
	TextDrawTextSize(hudInfo[tdMainTacho][0], 606.000000, -87.000000);

	hudInfo[tdMainTacho][1] = TextDrawCreate(548.000000, 357.000000, "~r~tachograf");
	TextDrawBackgroundColor(hudInfo[tdMainTacho][1], 60);
	TextDrawFont(hudInfo[tdMainTacho][1], 2);
	TextDrawLetterSize(hudInfo[tdMainTacho][1], 0.289999, 0.800000);
	TextDrawColor(hudInfo[tdMainTacho][1], -1);
	TextDrawSetOutline(hudInfo[tdMainTacho][1], 1);
	TextDrawSetProportional(hudInfo[tdMainTacho][1], 1);

	hudInfo[tdMainTacho][2] = TextDrawCreate(586.000000, 368.000000, "___");
	TextDrawAlignment(hudInfo[tdMainTacho][2], 2);
	TextDrawBackgroundColor(hudInfo[tdMainTacho][2], 255);
	TextDrawFont(hudInfo[tdMainTacho][2], 1);
	TextDrawLetterSize(hudInfo[tdMainTacho][2], 0.500000, -0.299998);
	TextDrawColor(hudInfo[tdMainTacho][2], -1);
	TextDrawSetOutline(hudInfo[tdMainTacho][2], 0);
	TextDrawSetProportional(hudInfo[tdMainTacho][2], 1);
	TextDrawSetShadow(hudInfo[tdMainTacho][2], 1);
	TextDrawUseBox(hudInfo[tdMainTacho][2], 1);
	TextDrawBoxColor(hudInfo[tdMainTacho][2], 125);
	TextDrawTextSize(hudInfo[tdMainTacho][2], 606.000000, -76.000000);
	return 1;
}

stock InitTachograph(playerid)
{
	hudInfo[tdInfoTacho][playerid] = CreatePlayerTextDraw(playerid, 585.000000, 371.000000, "00:00:00");
	PlayerTextDrawAlignment(playerid, hudInfo[tdInfoTacho][playerid], 2);
	PlayerTextDrawBackgroundColor(playerid, hudInfo[tdInfoTacho][playerid], 60);
	PlayerTextDrawFont(playerid, hudInfo[tdInfoTacho][playerid], 2);
	PlayerTextDrawLetterSize(playerid, hudInfo[tdInfoTacho][playerid], 0.270000, 0.600000);
	PlayerTextDrawColor(playerid, hudInfo[tdInfoTacho][playerid], -1);
	PlayerTextDrawSetOutline(playerid, hudInfo[tdInfoTacho][playerid], 1);
	PlayerTextDrawSetProportional(playerid, hudInfo[tdInfoTacho][playerid], 1);
	return 1;
}

stock ShowPlayerTacho(playerid, var = true)
{
	if(!var)
	{
		TextDrawHideForPlayer(playerid, hudInfo[tdMainTacho][0]);
		TextDrawHideForPlayer(playerid, hudInfo[tdMainTacho][1]);
		TextDrawHideForPlayer(playerid, hudInfo[tdMainTacho][2]);
		PlayerTextDrawHide(playerid, hudInfo[tdInfoTacho][playerid]);
		PlayerTextDrawDestroy(playerid, hudInfo[tdInfoTacho][playerid]);
	}
	else
	{
		TextDrawShowForPlayer(playerid, hudInfo[tdMainTacho][0]);
		TextDrawShowForPlayer(playerid, hudInfo[tdMainTacho][1]);
		TextDrawShowForPlayer(playerid, hudInfo[tdMainTacho][2]);
		InitTachograph(playerid);
		PlayerTextDrawShow(playerid, hudInfo[tdInfoTacho][playerid]);
	}
	return 1;
}

stock InitStableSpeedometer()
{
	hudInfo[tdMainSpeedo][0] = TextDrawCreate(546.000000, 387.000000, "___");
	TextDrawAlignment(hudInfo[tdMainSpeedo][0], 2);
	TextDrawBackgroundColor(hudInfo[tdMainSpeedo][0], 255);
	TextDrawFont(hudInfo[tdMainSpeedo][0], 1);
	TextDrawLetterSize(hudInfo[tdMainSpeedo][0], 0.500000, 4.599999);
	TextDrawColor(hudInfo[tdMainSpeedo][0], -1);
	TextDrawSetOutline(hudInfo[tdMainSpeedo][0], 0);
	TextDrawSetProportional(hudInfo[tdMainSpeedo][0], 1);
	TextDrawSetShadow(hudInfo[tdMainSpeedo][0], 1);
	TextDrawUseBox(hudInfo[tdMainSpeedo][0], 1);
	TextDrawBoxColor(hudInfo[tdMainSpeedo][0], 125);
	TextDrawTextSize(hudInfo[tdMainSpeedo][0], 606.000000, -166.000000);

	hudInfo[tdMainSpeedo][1] = TextDrawCreate(546.000000, 401.000000, "___");
	TextDrawAlignment(hudInfo[tdMainSpeedo][1], 2);
	TextDrawBackgroundColor(hudInfo[tdMainSpeedo][1], 255);
	TextDrawFont(hudInfo[tdMainSpeedo][1], 1);
	TextDrawLetterSize(hudInfo[tdMainSpeedo][1], 0.500000, -0.299998);
	TextDrawColor(hudInfo[tdMainSpeedo][1], -1);
	TextDrawSetOutline(hudInfo[tdMainSpeedo][1], 0);
	TextDrawSetProportional(hudInfo[tdMainSpeedo][1], 1);
	TextDrawSetShadow(hudInfo[tdMainSpeedo][1], 1);
	TextDrawUseBox(hudInfo[tdMainSpeedo][1], 1);
	TextDrawBoxColor(hudInfo[tdMainSpeedo][1], 125);
	TextDrawTextSize(hudInfo[tdMainSpeedo][1], 606.000000, -146.000000);

	hudInfo[tdMainSpeedo][2] = TextDrawCreate(546.000000, 424.000000, "___");
	TextDrawAlignment(hudInfo[tdMainSpeedo][2], 2);
	TextDrawBackgroundColor(hudInfo[tdMainSpeedo][2], 255);
	TextDrawFont(hudInfo[tdMainSpeedo][2], 1);
	TextDrawLetterSize(hudInfo[tdMainSpeedo][2], 0.500000, -0.299998);
	TextDrawColor(hudInfo[tdMainSpeedo][2], -1);
	TextDrawSetOutline(hudInfo[tdMainSpeedo][2], 0);
	TextDrawSetProportional(hudInfo[tdMainSpeedo][2], 1);
	TextDrawSetShadow(hudInfo[tdMainSpeedo][2], 1);
	TextDrawUseBox(hudInfo[tdMainSpeedo][2], 1);
	TextDrawBoxColor(hudInfo[tdMainSpeedo][2], 125);
	TextDrawTextSize(hudInfo[tdMainSpeedo][2], 606.000000, -146.000000);
	return 1;
}

stock InitSpeedometer(playerid)
{
	hudInfo[tdInfoSpeedo][TD_CAR_NAME][playerid] = CreatePlayerTextDraw(playerid, 469.000000, 389.000000, "~r~Police Car");
	PlayerTextDrawBackgroundColor(playerid, hudInfo[tdInfoSpeedo][TD_CAR_NAME][playerid], 60);
	PlayerTextDrawFont(playerid, hudInfo[tdInfoSpeedo][TD_CAR_NAME][playerid], 2);
	PlayerTextDrawLetterSize(playerid, hudInfo[tdInfoSpeedo][TD_CAR_NAME][playerid], 0.289999, 0.800000);
	PlayerTextDrawColor(playerid, hudInfo[tdInfoSpeedo][TD_CAR_NAME][playerid], -1);
	PlayerTextDrawSetOutline(playerid, hudInfo[tdInfoSpeedo][TD_CAR_NAME][playerid], 1);
	PlayerTextDrawSetProportional(playerid, hudInfo[tdInfoSpeedo][TD_CAR_NAME][playerid], 1);

	hudInfo[tdInfoSpeedo][TD_CAR_INFO][playerid] = CreatePlayerTextDraw(playerid, 470.000000, 402.000000, "Stan: 100%~n~Paliwo: 30l~n~Przebieg: 9999km");
	PlayerTextDrawBackgroundColor(playerid, hudInfo[tdInfoSpeedo][TD_CAR_INFO][playerid], 60);
	PlayerTextDrawFont(playerid, hudInfo[tdInfoSpeedo][TD_CAR_INFO][playerid], 2);
	PlayerTextDrawLetterSize(playerid, hudInfo[tdInfoSpeedo][TD_CAR_INFO][playerid], 0.270000, 0.600000);
	PlayerTextDrawColor(playerid, hudInfo[tdInfoSpeedo][TD_CAR_INFO][playerid], -1);
	PlayerTextDrawSetOutline(playerid, hudInfo[tdInfoSpeedo][TD_CAR_INFO][playerid], 1);
	PlayerTextDrawSetProportional(playerid, hudInfo[tdInfoSpeedo][TD_CAR_INFO][playerid], 1);

	hudInfo[tdInfoSpeedo][TD_CAR_FUEL][playerid] = CreatePlayerTextDraw(playerid, 584.000000, 388.000000, "hud:radar_centre");
	PlayerTextDrawAlignment(playerid, hudInfo[tdInfoSpeedo][TD_CAR_FUEL][playerid], 2);
	PlayerTextDrawBackgroundColor(playerid, hudInfo[tdInfoSpeedo][TD_CAR_FUEL][playerid], 60);
	PlayerTextDrawFont(playerid, hudInfo[tdInfoSpeedo][TD_CAR_FUEL][playerid], 4);
	PlayerTextDrawLetterSize(playerid, hudInfo[tdInfoSpeedo][TD_CAR_FUEL][playerid], 0.270000, 0.600000);
	PlayerTextDrawColor(playerid, hudInfo[tdInfoSpeedo][TD_CAR_FUEL][playerid], -16776961);
	PlayerTextDrawSetOutline(playerid, hudInfo[tdInfoSpeedo][TD_CAR_FUEL][playerid], 1);
	PlayerTextDrawSetProportional(playerid, hudInfo[tdInfoSpeedo][TD_CAR_FUEL][playerid], 1);
	PlayerTextDrawUseBox(playerid, hudInfo[tdInfoSpeedo][TD_CAR_FUEL][playerid], 1);
	PlayerTextDrawBoxColor(playerid, hudInfo[tdInfoSpeedo][TD_CAR_FUEL][playerid], 255);
	PlayerTextDrawTextSize(playerid, hudInfo[tdInfoSpeedo][TD_CAR_FUEL][playerid], 11.000000, 8.000000);

	hudInfo[tdInfoSpeedo][TD_CAR_TOLL][playerid] = CreatePlayerTextDraw(playerid, 596.000000, 387.000000, "hud:radar_light");
	PlayerTextDrawAlignment(playerid, hudInfo[tdInfoSpeedo][TD_CAR_TOLL][playerid], 2);
	PlayerTextDrawBackgroundColor(playerid, hudInfo[tdInfoSpeedo][TD_CAR_TOLL][playerid], 60);
	PlayerTextDrawFont(playerid, hudInfo[tdInfoSpeedo][TD_CAR_TOLL][playerid], 4);
	PlayerTextDrawLetterSize(playerid, hudInfo[tdInfoSpeedo][TD_CAR_TOLL][playerid], 0.270000, 0.600000);
	PlayerTextDrawColor(playerid, hudInfo[tdInfoSpeedo][TD_CAR_TOLL][playerid], -16776961);
	PlayerTextDrawSetOutline(playerid, hudInfo[tdInfoSpeedo][TD_CAR_TOLL][playerid], 1);
	PlayerTextDrawSetProportional(playerid, hudInfo[tdInfoSpeedo][TD_CAR_TOLL][playerid], 1);
	PlayerTextDrawUseBox(playerid, hudInfo[tdInfoSpeedo][TD_CAR_TOLL][playerid], 1);
	PlayerTextDrawBoxColor(playerid, hudInfo[tdInfoSpeedo][TD_CAR_TOLL][playerid], 255);
	PlayerTextDrawTextSize(playerid, hudInfo[tdInfoSpeedo][TD_CAR_TOLL][playerid], 12.000000, 11.000000);

	hudInfo[tdInfoSpeedo][TD_CAR_BELTS][playerid] = CreatePlayerTextDraw(playerid, 615.000000, 387.000000, "!");
	PlayerTextDrawAlignment(playerid, hudInfo[tdInfoSpeedo][TD_CAR_BELTS][playerid], 2);
	PlayerTextDrawBackgroundColor(playerid, hudInfo[tdInfoSpeedo][TD_CAR_BELTS][playerid], 255);
	PlayerTextDrawFont(playerid, hudInfo[tdInfoSpeedo][TD_CAR_BELTS][playerid], 2);
	PlayerTextDrawLetterSize(playerid, hudInfo[tdInfoSpeedo][TD_CAR_BELTS][playerid], 0.379999, 1.000000);
	PlayerTextDrawColor(playerid, hudInfo[tdInfoSpeedo][TD_CAR_TOLL][playerid], -16776961);
	PlayerTextDrawSetOutline(playerid, hudInfo[tdInfoSpeedo][TD_CAR_BELTS][playerid], 1);
	PlayerTextDrawSetProportional(playerid, hudInfo[tdInfoSpeedo][TD_CAR_BELTS][playerid], 1);

	hudInfo[tdInfoSpeedo][TD_CAR_SPEED][playerid] = CreatePlayerTextDraw(playerid, 601.000000, 402.000000, "~r~133~n~km/h");
	PlayerTextDrawAlignment(playerid, hudInfo[tdInfoSpeedo][TD_CAR_SPEED][playerid], 2);
	PlayerTextDrawBackgroundColor(playerid, hudInfo[tdInfoSpeedo][TD_CAR_SPEED][playerid], 60);
	PlayerTextDrawFont(playerid, hudInfo[tdInfoSpeedo][TD_CAR_SPEED][playerid], 2);
	PlayerTextDrawLetterSize(playerid, hudInfo[tdInfoSpeedo][TD_CAR_SPEED][playerid], 0.270000, 0.600000);
	PlayerTextDrawColor(playerid, hudInfo[tdInfoSpeedo][TD_CAR_SPEED][playerid], -1);
	PlayerTextDrawSetOutline(playerid, hudInfo[tdInfoSpeedo][TD_CAR_SPEED][playerid], 1);
	PlayerTextDrawSetProportional(playerid, hudInfo[tdInfoSpeedo][TD_CAR_SPEED][playerid], 1);
	return 1;
}

stock ShowPlayerSpeedometer(playerid, var = true)
{
	if(!var)
	{
		TextDrawHideForPlayer(playerid, hudInfo[tdMainSpeedo][0]);
		TextDrawHideForPlayer(playerid, hudInfo[tdMainSpeedo][1]);
		TextDrawHideForPlayer(playerid, hudInfo[tdMainSpeedo][2]);
		PlayerTextDrawHide(playerid, hudInfo[tdInfoSpeedo][TD_CAR_NAME][playerid]);
		PlayerTextDrawHide(playerid, hudInfo[tdInfoSpeedo][TD_CAR_INFO][playerid]);
		PlayerTextDrawHide(playerid, hudInfo[tdInfoSpeedo][TD_CAR_FUEL][playerid]);
		PlayerTextDrawHide(playerid, hudInfo[tdInfoSpeedo][TD_CAR_TOLL][playerid]);
		PlayerTextDrawHide(playerid, hudInfo[tdInfoSpeedo][TD_CAR_BELTS][playerid]);
		PlayerTextDrawHide(playerid, hudInfo[tdInfoSpeedo][TD_CAR_SPEED][playerid]);
		ExitSpeedometer(playerid);
	}
	else
	{
		TextDrawShowForPlayer(playerid, hudInfo[tdMainSpeedo][0]);
		TextDrawShowForPlayer(playerid, hudInfo[tdMainSpeedo][1]);
		TextDrawShowForPlayer(playerid, hudInfo[tdMainSpeedo][2]);
		InitSpeedometer(playerid);	
		PlayerTextDrawShow(playerid, hudInfo[tdInfoSpeedo][TD_CAR_NAME][playerid]);
		PlayerTextDrawShow(playerid, hudInfo[tdInfoSpeedo][TD_CAR_INFO][playerid]);
		PlayerTextDrawShow(playerid, hudInfo[tdInfoSpeedo][TD_CAR_FUEL][playerid]);
		PlayerTextDrawShow(playerid, hudInfo[tdInfoSpeedo][TD_CAR_TOLL][playerid]);
		PlayerTextDrawShow(playerid, hudInfo[tdInfoSpeedo][TD_CAR_BELTS][playerid]);
		PlayerTextDrawShow(playerid, hudInfo[tdInfoSpeedo][TD_CAR_SPEED][playerid]);

	}
	return 1;
}

stock ExitSpeedometer(playerid)
{
	PlayerTextDrawDestroy(playerid, hudInfo[tdInfoSpeedo][TD_CAR_NAME][playerid]);
	PlayerTextDrawDestroy(playerid, hudInfo[tdInfoSpeedo][TD_CAR_INFO][playerid]);
	PlayerTextDrawDestroy(playerid, hudInfo[tdInfoSpeedo][TD_CAR_FUEL][playerid]);
	PlayerTextDrawDestroy(playerid, hudInfo[tdInfoSpeedo][TD_CAR_TOLL][playerid]);
	PlayerTextDrawDestroy(playerid, hudInfo[tdInfoSpeedo][TD_CAR_BELTS][playerid]);
	PlayerTextDrawDestroy(playerid, hudInfo[tdInfoSpeedo][TD_CAR_SPEED][playerid]);
	return 1;
}

stock InitRadar(playerid)
{
	hudInfo[tdInfoRadar][TD_RD_MAIN][playerid] = CreatePlayerTextDraw(playerid, 502.000000, 357.000000, "___");
	PlayerTextDrawAlignment(playerid, hudInfo[tdInfoRadar][TD_RD_MAIN][playerid], 2);
	PlayerTextDrawBackgroundColor(playerid, hudInfo[tdInfoRadar][TD_RD_MAIN][playerid], 255);
	PlayerTextDrawFont(playerid, hudInfo[tdInfoRadar][TD_RD_MAIN][playerid], 1);
	PlayerTextDrawLetterSize(playerid, hudInfo[tdInfoRadar][TD_RD_MAIN][playerid], 0.500000, 2.599999);
	PlayerTextDrawColor(playerid, hudInfo[tdInfoRadar][TD_RD_MAIN][playerid], -1);
	PlayerTextDrawSetOutline(playerid, hudInfo[tdInfoRadar][TD_RD_MAIN][playerid], 0);
	PlayerTextDrawSetProportional(playerid, hudInfo[tdInfoRadar][TD_RD_MAIN][playerid], 1);
	PlayerTextDrawSetShadow(playerid, hudInfo[tdInfoRadar][TD_RD_MAIN][playerid], 1);
	PlayerTextDrawUseBox(playerid, hudInfo[tdInfoRadar][TD_RD_MAIN][playerid], 1);
	PlayerTextDrawBoxColor(playerid, hudInfo[tdInfoRadar][TD_RD_MAIN][playerid], 125);
	PlayerTextDrawTextSize(playerid, hudInfo[tdInfoRadar][TD_RD_MAIN][playerid], 606.000000, -77.000000);

	hudInfo[tdInfoRadar][TD_RD_NAME][playerid] = CreatePlayerTextDraw(playerid, 503.000000, 357.000000, "~r~Radar (20)");
	PlayerTextDrawAlignment(playerid, hudInfo[tdInfoRadar][TD_RD_NAME][playerid], 2);
	PlayerTextDrawBackgroundColor(playerid, hudInfo[tdInfoRadar][TD_RD_NAME][playerid], 60);
	PlayerTextDrawFont(playerid, hudInfo[tdInfoRadar][TD_RD_NAME][playerid], 2);
	PlayerTextDrawLetterSize(playerid, hudInfo[tdInfoRadar][TD_RD_NAME][playerid], 0.279999, 0.800000);
	PlayerTextDrawColor(playerid, hudInfo[tdInfoRadar][TD_RD_NAME][playerid], -1);
	PlayerTextDrawSetOutline(playerid, hudInfo[tdInfoRadar][TD_RD_NAME][playerid], 1);
	PlayerTextDrawSetProportional(playerid, hudInfo[tdInfoRadar][TD_RD_NAME][playerid], 1);

	hudInfo[tdInfoRadar][TD_RD_LINE][playerid] = CreatePlayerTextDraw(playerid, 502.000000, 368.000000, "___");
	PlayerTextDrawAlignment(playerid, hudInfo[tdInfoRadar][TD_RD_LINE][playerid], 2);
	PlayerTextDrawBackgroundColor(playerid, hudInfo[tdInfoRadar][TD_RD_LINE][playerid], 255);
	PlayerTextDrawFont(playerid, hudInfo[tdInfoRadar][TD_RD_LINE][playerid], 1);
	PlayerTextDrawLetterSize(playerid, hudInfo[tdInfoRadar][TD_RD_LINE][playerid], 0.500000, -0.299998);
	PlayerTextDrawColor(playerid, hudInfo[tdInfoRadar][TD_RD_LINE][playerid], -1);
	PlayerTextDrawSetOutline(playerid, hudInfo[tdInfoRadar][TD_RD_LINE][playerid], 0);
	PlayerTextDrawSetProportional(playerid, hudInfo[tdInfoRadar][TD_RD_LINE][playerid], 1);
	PlayerTextDrawSetShadow(playerid, hudInfo[tdInfoRadar][TD_RD_LINE][playerid], 1);
	PlayerTextDrawUseBox(playerid, hudInfo[tdInfoRadar][TD_RD_LINE][playerid], 1);
	PlayerTextDrawBoxColor(playerid, hudInfo[tdInfoRadar][TD_RD_LINE][playerid], 125);
	PlayerTextDrawTextSize(playerid, hudInfo[tdInfoRadar][TD_RD_LINE][playerid], 606.000000, -66.000000);

	hudInfo[tdInfoRadar][TD_RD_SPEED][playerid] = CreatePlayerTextDraw(playerid, 504.000000, 371.000000, "200 km/h");
	PlayerTextDrawAlignment(playerid, hudInfo[tdInfoRadar][TD_RD_SPEED][playerid], 2);
	PlayerTextDrawBackgroundColor(playerid, hudInfo[tdInfoRadar][TD_RD_SPEED][playerid], 60);
	PlayerTextDrawFont(playerid, hudInfo[tdInfoRadar][TD_RD_SPEED][playerid], 2);
	PlayerTextDrawLetterSize(playerid, hudInfo[tdInfoRadar][TD_RD_SPEED][playerid], 0.270000, 0.600000);
	PlayerTextDrawColor(playerid, hudInfo[tdInfoRadar][TD_RD_SPEED][playerid], -1);
	PlayerTextDrawSetOutline(playerid, hudInfo[tdInfoRadar][TD_RD_SPEED][playerid], 1);
	PlayerTextDrawSetProportional(playerid, hudInfo[tdInfoRadar][TD_RD_SPEED][playerid], 1);
	return 1;
}

stock ShowPlayerRadar(playerid, var = true)
{
	if(!var)
	{
		PlayerTextDrawHide(playerid, hudInfo[tdInfoRadar][TD_RD_MAIN][playerid]);
		PlayerTextDrawHide(playerid, hudInfo[tdInfoRadar][TD_RD_NAME][playerid]);
		PlayerTextDrawHide(playerid, hudInfo[tdInfoRadar][TD_RD_LINE][playerid]);
		PlayerTextDrawHide(playerid, hudInfo[tdInfoRadar][TD_RD_SPEED][playerid]);
		ExitRadar(playerid);
	}
	else
	{
		InitRadar(playerid);	
		PlayerTextDrawShow(playerid, hudInfo[tdInfoRadar][TD_RD_MAIN][playerid]);
		PlayerTextDrawShow(playerid, hudInfo[tdInfoRadar][TD_RD_NAME][playerid]);
		PlayerTextDrawShow(playerid, hudInfo[tdInfoRadar][TD_RD_LINE][playerid]);
		PlayerTextDrawShow(playerid, hudInfo[tdInfoRadar][TD_RD_SPEED][playerid]);

	}
	return 1;
}

stock ExitRadar(playerid)
{
	PlayerTextDrawDestroy(playerid, hudInfo[tdInfoRadar][TD_RD_MAIN][playerid]);
	PlayerTextDrawDestroy(playerid, hudInfo[tdInfoRadar][TD_RD_NAME][playerid]);
	PlayerTextDrawDestroy(playerid, hudInfo[tdInfoRadar][TD_RD_LINE][playerid]);
	PlayerTextDrawDestroy(playerid, hudInfo[tdInfoRadar][TD_RD_SPEED][playerid]);
	return 1;
}

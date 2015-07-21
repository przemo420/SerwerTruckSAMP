
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
		SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX] = CreatePlayerTextDraw(playerid, 22.942945, 210.216552, "_");
		PlayerTextDrawLetterSize(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX], 0.385006, 6.692498);
		PlayerTextDrawTextSize(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX], 117.291351, 10.000000);
		PlayerTextDrawAlignment(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX], 1);
		PlayerTextDrawColor(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX], -1);
		PlayerTextDrawUseBox(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX], 1);
		PlayerTextDrawBoxColor(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX], 16711748);
		PlayerTextDrawSetShadow(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX], 0);
		PlayerTextDrawSetOutline(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX], 0);
		PlayerTextDrawBackgroundColor(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX], -16776961);
		PlayerTextDrawFont(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX], 1);
		PlayerTextDrawSetProportional(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX], 1);
		PlayerTextDrawSetShadow(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX], 0);

		SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX2] = CreatePlayerTextDraw(playerid, 9951, 211.583358, "_");
		PlayerTextDrawLetterSize(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX2], 0.435137, 6.371667);
		PlayerTextDrawTextSize(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX2], 116.024887, 0.000000);
		PlayerTextDrawAlignment(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX2], 1);
		PlayerTextDrawColor(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX2], -1);
		PlayerTextDrawUseBox(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX2], 1);
		PlayerTextDrawBoxColor(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX2], 170);
		PlayerTextDrawSetShadow(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX2], 0);
		PlayerTextDrawSetOutline(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX2], 0);
		PlayerTextDrawBackgroundColor(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX2], -16776961);
		PlayerTextDrawFont(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX2], 1);
		PlayerTextDrawSetProportional(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX2], 1);
		PlayerTextDrawSetShadow(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX2], 0);

		SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO] = CreatePlayerTextDraw(playerid, 7001, 209.249969, "ID:~n~Nick:~n~Kasa:~n~Score:~n~HP:~n~HP Poj:~n~Predkosc:");
		PlayerTextDrawLetterSize(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO], 0.291770, 1.098333);
		PlayerTextDrawAlignment(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO], 1);
		PlayerTextDrawColor(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO], -1);
		PlayerTextDrawSetShadow(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO], 0);
		PlayerTextDrawSetOutline(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO], 0);
		PlayerTextDrawBackgroundColor(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO], 255);
		PlayerTextDrawFont(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO], 1);
		PlayerTextDrawSetProportional(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO], 1);
		PlayerTextDrawSetShadow(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO], 0);

		SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO2] = CreatePlayerTextDraw(playerid, 5803, 209.249954, "_");
		PlayerTextDrawLetterSize(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO2], 0.291770, 1.098333);
		PlayerTextDrawAlignment(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO2], 1);
		PlayerTextDrawColor(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO2], -1);
		PlayerTextDrawSetShadow(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO2], 0);
		PlayerTextDrawSetOutline(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO2], 0);
		PlayerTextDrawBackgroundColor(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO2], 255);
		PlayerTextDrawFont(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO2], 1);
		PlayerTextDrawSetProportional(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO2], 1);
		PlayerTextDrawSetShadow(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO2], 0);

		PlayerTextDrawShow(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX]);
		PlayerTextDrawShow(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX2]);
		PlayerTextDrawShow(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO]);
		PlayerTextDrawShow(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO2]);

		SpectactorsTD[playerid][SPECTACTOR_TIMER] = SetTimerEx("SpectactorUpdate", 250, true, "d", playerid);
	}
	else
	{
		PlayerTextDrawHide(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX]);
		PlayerTextDrawHide(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX2]);
		PlayerTextDrawHide(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO]);
		PlayerTextDrawHide(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO2]);

		PlayerTextDrawDestroy(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX]);
		PlayerTextDrawDestroy(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_BOX2]);
		PlayerTextDrawDestroy(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO]);
		PlayerTextDrawDestroy(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO2]);

		KillTimer(SpectactorsTD[playerid][SPECTACTOR_TIMER]);

		TogglePlayerSpectating(playerid, false);
	}

	return 1;
}

enum Main_TD {
	Text:SerwerName,
	Text:Time,
	Text:Date,
	Text:TruckerSkins[9]
}
new MainTextDraws[Main_TD];
new MainTextDrawTimer;

CreateMainTextDraws(create)
{
	if(create)
	{
		MainTextDraws[SerwerName] = TextDrawCreate(497.086853, 3.333339, "www.serwertruck.eu");
		TextDrawLetterSize(MainTextDraws[SerwerName], 0.400000, 1.600000);
		TextDrawAlignment(MainTextDraws[SerwerName], 1);
		TextDrawColor(MainTextDraws[SerwerName], -1);
		TextDrawSetShadow(MainTextDraws[SerwerName], 10);
		TextDrawSetOutline(MainTextDraws[SerwerName], 1);
		TextDrawBackgroundColor(MainTextDraws[SerwerName], 255);
		TextDrawFont(MainTextDraws[SerwerName], 1);
		TextDrawSetProportional(MainTextDraws[SerwerName], 1);
		TextDrawSetShadow(MainTextDraws[SerwerName], 10);

		MainTextDraws[Time] = TextDrawCreate(71.769081, 316.167205, "00:00");
		TextDrawLetterSize(MainTextDraws[Time], 0.257099, 0.958330);
		TextDrawAlignment(MainTextDraws[Time], 1);
		TextDrawColor(MainTextDraws[Time], 8388863);
		TextDrawSetShadow(MainTextDraws[Time], 2);
		TextDrawSetOutline(MainTextDraws[Time], 1);
		TextDrawBackgroundColor(MainTextDraws[Time], 255);
		TextDrawFont(MainTextDraws[Time], 1);
		TextDrawSetProportional(MainTextDraws[Time], 1);
		TextDrawSetShadow(MainTextDraws[Time], 2);

		MainTextDraws[Date] = TextDrawCreate(62.398658, 327.667114, "01.01.2015");
		TextDrawLetterSize(MainTextDraws[Date], 0.257099, 0.958330);
		TextDrawAlignment(MainTextDraws[Date], 1);
		TextDrawColor(MainTextDraws[Date], 8388863);
		TextDrawSetShadow(MainTextDraws[Date], 2);
		TextDrawSetOutline(MainTextDraws[Date], 1);
		TextDrawBackgroundColor(MainTextDraws[Date], 255);
		TextDrawFont(MainTextDraws[Date], 1);
		TextDrawSetProportional(MainTextDraws[Date], 1);
		TextDrawSetShadow(MainTextDraws[Date], 2);

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
		TextDrawHideForAll(MainTextDraws[SerwerName]);
		TextDrawHideForAll(MainTextDraws[Time]);
		TextDrawHideForAll(MainTextDraws[Date]);

		TextDrawDestroy(MainTextDraws[SerwerName]);
		TextDrawDestroy(MainTextDraws[Time]);
		TextDrawDestroy(MainTextDraws[Date]);

		for(new i; i <=8; i++)
		{
			TextDrawHideForAll(MainTextDraws[TruckerSkins][i]);
			TextDrawDestroy(MainTextDraws[TruckerSkins][i]);
		}

		KillTimer(MainTextDrawTimer);
	}
}

enum SpeedometerTD
{
	Text:Main,
	Text:EngineInfo,
	Text:VehicleInfo,
	Text:CargoInfoTD,
	Text:Box[2],
	PlayerText:VehicleEngine[MAX_PLAYERS],
	PlayerText:VehicleHP[MAX_PLAYERS],
	PlayerText:VehicleCargo[MAX_PLAYERS],
	PlayerText:VehicleSpeed[MAX_PLAYERS],
	Text:ViaTollX,
	Text:ViaTollPlus,
	PlayerText:VehicleFuel[MAX_PLAYERS],
	PlayerText:VehicleMileage[MAX_PLAYERS],
	bool:ViaTollVisible
}
new Speedometer[SpeedometerTD];

CreateSpeedometer(create)
{
	if(create)
	{
		Speedometer[Main] = TextDrawCreate(457.200103, 333.908538, "_");
		TextDrawLetterSize(Speedometer[Main], 0.874400, 11.747200);
		TextDrawTextSize(Speedometer[Main], 631.800354, 0.000000);
		TextDrawAlignment(Speedometer[Main], 1);
		TextDrawColor(Speedometer[Main], -1);
		TextDrawUseBox(Speedometer[Main], 1);
		TextDrawBoxColor(Speedometer[Main], 45);
		TextDrawSetShadow(Speedometer[Main], 0);
		TextDrawSetOutline(Speedometer[Main], 0);
		TextDrawBackgroundColor(Speedometer[Main], -1061109505);
		TextDrawFont(Speedometer[Main], 1);
		TextDrawSetProportional(Speedometer[Main], 0);
		TextDrawSetShadow(Speedometer[Main], 0);

		Speedometer[EngineInfo] = TextDrawCreate(460.100067, 415.695587, "Swiatla:~n~Silnik:");
		TextDrawLetterSize(Speedometer[EngineInfo], 0.304798, 1.241598);
		TextDrawAlignment(Speedometer[EngineInfo], 1);
		TextDrawColor(Speedometer[EngineInfo], -1);
		TextDrawSetShadow(Speedometer[EngineInfo], 0);
		TextDrawSetOutline(Speedometer[EngineInfo], 1);
		TextDrawBackgroundColor(Speedometer[EngineInfo], 255);
		TextDrawFont(Speedometer[EngineInfo], 1);
		TextDrawSetProportional(Speedometer[EngineInfo], 1);
		TextDrawSetShadow(Speedometer[EngineInfo], 0);

		Speedometer[CargoInfoTD] = TextDrawCreate(550.193908, 415.695587, "Towar:~n~Waga:");
		TextDrawLetterSize(Speedometer[CargoInfoTD], 0.304798, 1.241598);
		TextDrawAlignment(Speedometer[CargoInfoTD], 1);
		TextDrawColor(Speedometer[CargoInfoTD], -1);
		TextDrawSetShadow(Speedometer[CargoInfoTD], 0);
		TextDrawSetOutline(Speedometer[CargoInfoTD], 1);
		TextDrawBackgroundColor(Speedometer[CargoInfoTD], 255);
		TextDrawFont(Speedometer[CargoInfoTD], 1);
		TextDrawSetProportional(Speedometer[CargoInfoTD], 1);
		TextDrawSetShadow(Speedometer[CargoInfoTD], 0);

		Speedometer[Box][0] = TextDrawCreate(462.800048, 411.683288, "_");
		TextDrawLetterSize(Speedometer[Box][0], 0.425599, -0.020266);
		TextDrawTextSize(Speedometer[Box][0], 625.550048, -1.409999);
		TextDrawAlignment(Speedometer[Box][0], 1);
		TextDrawColor(Speedometer[Box][0], -1);
		TextDrawUseBox(Speedometer[Box][0], 1);
		TextDrawBoxColor(Speedometer[Box][0], 128);
		TextDrawSetShadow(Speedometer[Box][0], 0);
		TextDrawSetOutline(Speedometer[Box][0], 0);
		TextDrawBackgroundColor(Speedometer[Box][0], 255);
		TextDrawFont(Speedometer[Box][0], 1);
		TextDrawSetProportional(Speedometer[Box][0], 1);
		TextDrawSetShadow(Speedometer[Box][0], 0);

		Speedometer[Box][1] = TextDrawCreate(463.600036, 371.780853, "_");
		TextDrawLetterSize(Speedometer[Box][1], 0.425599, -0.020266);
		TextDrawTextSize(Speedometer[Box][1], 626.350036, -1.409999);
		TextDrawAlignment(Speedometer[Box][1], 1);
		TextDrawColor(Speedometer[Box][1], -1);
		TextDrawUseBox(Speedometer[Box][1], 1);
		TextDrawBoxColor(Speedometer[Box][1], 128);
		TextDrawSetShadow(Speedometer[Box][1], 0);
		TextDrawSetOutline(Speedometer[Box][1], 0);
		TextDrawBackgroundColor(Speedometer[Box][1], 255);
		TextDrawFont(Speedometer[Box][1], 1);
		TextDrawSetProportional(Speedometer[Box][1], 1);
		TextDrawSetShadow(Speedometer[Box][1], 0);

		Speedometer[VehicleInfo] = TextDrawCreate(490.801727, 379.440002, "Stan pojazdu:~n~Stan towaru:");
		TextDrawLetterSize(Speedometer[VehicleInfo], 0.304798, 1.241598);
		TextDrawAlignment(Speedometer[VehicleInfo], 1);
		TextDrawColor(Speedometer[VehicleInfo], -1);
		TextDrawSetShadow(Speedometer[VehicleInfo], 0);
		TextDrawSetOutline(Speedometer[VehicleInfo], 1);
		TextDrawBackgroundColor(Speedometer[VehicleInfo], 255);
		TextDrawFont(Speedometer[VehicleInfo], 1);
		TextDrawSetProportional(Speedometer[VehicleInfo], 1);
		TextDrawSetShadow(Speedometer[VehicleInfo], 0);

		Speedometer[ViaTollX] = TextDrawCreate(471.599945, 384.133422, "X");
		TextDrawLetterSize(Speedometer[ViaTollX], 0.400000, 1.600000);
		TextDrawAlignment(Speedometer[ViaTollX], 1);
		TextDrawColor(Speedometer[ViaTollX], -16776961);
		TextDrawSetShadow(Speedometer[ViaTollX], 0);
		TextDrawSetOutline(Speedometer[ViaTollX], 1);
		TextDrawBackgroundColor(Speedometer[ViaTollX], 255);
		TextDrawFont(Speedometer[ViaTollX], 1);
		TextDrawSetProportional(Speedometer[ViaTollX], 1);
		TextDrawSetShadow(Speedometer[ViaTollX], 0);

		Speedometer[ViaTollPlus] = TextDrawCreate(475.999786, 381.946746, "+");
		TextDrawLetterSize(Speedometer[ViaTollPlus], 0.602400, 1.995731);
		TextDrawAlignment(Speedometer[ViaTollPlus], 2);
		TextDrawColor(Speedometer[ViaTollPlus], 16711935);
		TextDrawSetShadow(Speedometer[ViaTollPlus], 0);
		TextDrawSetOutline(Speedometer[ViaTollPlus], 1);
		TextDrawBackgroundColor(Speedometer[ViaTollPlus], 255);
		TextDrawFont(Speedometer[ViaTollPlus], 1);
		TextDrawSetProportional(Speedometer[ViaTollPlus], 1);
		TextDrawSetShadow(Speedometer[ViaTollPlus], 0);
	}
	else
	{
		TextDrawHideForAll(Speedometer[Main]);
		TextDrawHideForAll(Speedometer[EngineInfo]);
		TextDrawHideForAll(Speedometer[VehicleInfo]);
		TextDrawHideForAll(Speedometer[CargoInfoTD]);
		TextDrawHideForAll(Speedometer[ViaTollX]);
		TextDrawHideForAll(Speedometer[ViaTollPlus]);
		TextDrawHideForAll(Speedometer[Box][0]);
		TextDrawHideForAll(Speedometer[Box][1]);

		TextDrawDestroy(Speedometer[Main]);
		TextDrawDestroy(Speedometer[EngineInfo]);
		TextDrawDestroy(Speedometer[VehicleInfo]);
		TextDrawDestroy(Speedometer[CargoInfoTD]);
		TextDrawDestroy(Speedometer[ViaTollX]);
		TextDrawDestroy(Speedometer[ViaTollPlus]);
		TextDrawDestroy(Speedometer[Box][0]);
		TextDrawDestroy(Speedometer[Box][1]);
	}
}

enum CargoTD
{
	Text:CargoTDBox,
	PlayerText:CargoTDInfo[MAX_PLAYERS]
}
new CargoTextDraws[CargoTD];

CreateCargoTextdraws(create)
{
	if(create)
	{
		CargoTextDraws[CargoTDBox] = TextDrawCreate(233.199935, 6.319984, "_");
		TextDrawLetterSize(CargoTextDraws[CargoTDBox], 0.408000, 2.697599);
		TextDrawTextSize(CargoTextDraws[CargoTDBox], 387.800018, 0.000000);
		TextDrawAlignment(CargoTextDraws[CargoTDBox], 1);
		TextDrawColor(CargoTextDraws[CargoTDBox], -154);
		TextDrawUseBox(CargoTextDraws[CargoTDBox], 1);
		TextDrawBoxColor(CargoTextDraws[CargoTDBox], 85);
		TextDrawSetShadow(CargoTextDraws[CargoTDBox], 0);
		TextDrawSetOutline(CargoTextDraws[CargoTDBox], 0);
		TextDrawBackgroundColor(CargoTextDraws[CargoTDBox], -246);
		TextDrawFont(CargoTextDraws[CargoTDBox], 1);
		TextDrawSetProportional(CargoTextDraws[CargoTDBox], 1);
		TextDrawSetShadow(CargoTextDraws[CargoTDBox], 0);
	}
	else
	{
		TextDrawHideForAll(CargoTextDraws[CargoTDBox]);
		TextDrawDestroy(CargoTextDraws[CargoTDBox]);
	}
}

new PlayerText:levelTD[2][MAX_PLAYERS];

CreateTextDrawForPlayer(playerid)
{
	CargoTextDraws[CargoTDInfo][playerid] = CreatePlayerTextDraw(playerid, 309.100006, 6.473295, "Do wyladunku zostalo:~n~5m 1s");
	PlayerTextDrawLetterSize(playerid, CargoTextDraws[CargoTDInfo][playerid], 0.293599, 1.323733);
	PlayerTextDrawAlignment(playerid, CargoTextDraws[CargoTDInfo][playerid], 2);
	PlayerTextDrawColor(playerid, CargoTextDraws[CargoTDInfo][playerid], -1);
	PlayerTextDrawSetShadow(playerid, CargoTextDraws[CargoTDInfo][playerid], 0);
	PlayerTextDrawSetOutline(playerid, CargoTextDraws[CargoTDInfo][playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, CargoTextDraws[CargoTDInfo][playerid], 255);
	PlayerTextDrawFont(playerid, CargoTextDraws[CargoTDInfo][playerid], 1);
	PlayerTextDrawSetProportional(playerid, CargoTextDraws[CargoTDInfo][playerid], 1);
	PlayerTextDrawSetShadow(playerid, CargoTextDraws[CargoTDInfo][playerid], 0);

	Speedometer[VehicleEngine][playerid] = CreatePlayerTextDraw(playerid, 506.802917, 415.695587, "_");
	PlayerTextDrawLetterSize(playerid, Speedometer[VehicleEngine][playerid], 0.304798, 1.241598);
	PlayerTextDrawAlignment(playerid, Speedometer[VehicleEngine][playerid], 1);
	PlayerTextDrawColor(playerid, Speedometer[VehicleEngine][playerid], -1);
	PlayerTextDrawSetShadow(playerid, Speedometer[VehicleEngine][playerid], 0);
	PlayerTextDrawSetOutline(playerid, Speedometer[VehicleEngine][playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, Speedometer[VehicleEngine][playerid], 255);
	PlayerTextDrawFont(playerid, Speedometer[VehicleEngine][playerid], 1);
	PlayerTextDrawSetProportional(playerid, Speedometer[VehicleEngine][playerid], 1);
	PlayerTextDrawSetShadow(playerid, Speedometer[VehicleEngine][playerid], 0);

	Speedometer[VehicleHP][playerid] = CreatePlayerTextDraw(playerid, 570.088867, 379.493377, "_");
	PlayerTextDrawLetterSize(playerid, Speedometer[VehicleHP][playerid], 0.304798, 1.241598);
	PlayerTextDrawAlignment(playerid, Speedometer[VehicleHP][playerid], 1);
	PlayerTextDrawColor(playerid, Speedometer[VehicleHP][playerid], -1);
	PlayerTextDrawSetShadow(playerid, Speedometer[VehicleHP][playerid], 0);
	PlayerTextDrawSetOutline(playerid, Speedometer[VehicleHP][playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, Speedometer[VehicleHP][playerid], 255);
	PlayerTextDrawFont(playerid, Speedometer[VehicleHP][playerid], 1);
	PlayerTextDrawSetProportional(playerid, Speedometer[VehicleHP][playerid], 1);
	PlayerTextDrawSetShadow(playerid, Speedometer[VehicleHP][playerid], 0);

	Speedometer[VehicleCargo][playerid] = CreatePlayerTextDraw(playerid, 610.079467, 415.695587, "_");
	PlayerTextDrawLetterSize(playerid, Speedometer[VehicleCargo][playerid], 0.304798, 1.241598);
	PlayerTextDrawAlignment(playerid, Speedometer[VehicleCargo][playerid], 2);
	PlayerTextDrawColor(playerid, Speedometer[VehicleCargo][playerid], -1);
	PlayerTextDrawSetShadow(playerid, Speedometer[VehicleCargo][playerid], 0);
	PlayerTextDrawSetOutline(playerid, Speedometer[VehicleCargo][playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, Speedometer[VehicleCargo][playerid], 255);
	PlayerTextDrawFont(playerid, Speedometer[VehicleCargo][playerid], 1);
	PlayerTextDrawSetProportional(playerid, Speedometer[VehicleCargo][playerid], 1);
	PlayerTextDrawSetShadow(playerid, Speedometer[VehicleCargo][playerid], 0);

	Speedometer[VehicleSpeed][playerid] = CreatePlayerTextDraw(playerid, 543.299621, 336.655456, "_");
	PlayerTextDrawLetterSize(playerid, Speedometer[VehicleSpeed][playerid], 0.418399, 1.375998);
	PlayerTextDrawAlignment(playerid, Speedometer[VehicleSpeed][playerid], 2);
	PlayerTextDrawColor(playerid, Speedometer[VehicleSpeed][playerid], -1);
	PlayerTextDrawSetShadow(playerid, Speedometer[VehicleSpeed][playerid], 0);
	PlayerTextDrawSetOutline(playerid, Speedometer[VehicleSpeed][playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, Speedometer[VehicleSpeed][playerid], 255);
	PlayerTextDrawFont(playerid, Speedometer[VehicleSpeed][playerid], 1);
	PlayerTextDrawSetProportional(playerid, Speedometer[VehicleSpeed][playerid], 1);
	PlayerTextDrawSetShadow(playerid, Speedometer[VehicleSpeed][playerid], 0);

	Speedometer[VehicleFuel][playerid] = CreatePlayerTextDraw(playerid, 480.400024, 339.333404, "_");
	PlayerTextDrawLetterSize(playerid, Speedometer[VehicleFuel][playerid], 0.243199, 0.987730);
	PlayerTextDrawAlignment(playerid, Speedometer[VehicleFuel][playerid], 2);
	PlayerTextDrawColor(playerid, Speedometer[VehicleFuel][playerid], -1);
	PlayerTextDrawSetShadow(playerid, Speedometer[VehicleFuel][playerid], 0);
	PlayerTextDrawSetOutline(playerid, Speedometer[VehicleFuel][playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, Speedometer[VehicleFuel][playerid], 255);
	PlayerTextDrawFont(playerid, Speedometer[VehicleFuel][playerid], 1);
	PlayerTextDrawSetProportional(playerid, Speedometer[VehicleFuel][playerid], 1);
	PlayerTextDrawSetShadow(playerid, Speedometer[VehicleFuel][playerid], 0);

	Speedometer[VehicleMileage][playerid] = CreatePlayerTextDraw(playerid, 601.480163, 339.333343, "_");
	PlayerTextDrawLetterSize(playerid, Speedometer[VehicleMileage][playerid], 0.243199, 0.987730);
	PlayerTextDrawAlignment(playerid, Speedometer[VehicleMileage][playerid], 2);
	PlayerTextDrawColor(playerid, Speedometer[VehicleMileage][playerid], -1);
	PlayerTextDrawSetShadow(playerid, Speedometer[VehicleMileage][playerid], 0);
	PlayerTextDrawSetOutline(playerid, Speedometer[VehicleMileage][playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, Speedometer[VehicleMileage][playerid], 255);
	PlayerTextDrawFont(playerid, Speedometer[VehicleMileage][playerid], 1);
	PlayerTextDrawSetProportional(playerid, Speedometer[VehicleMileage][playerid], 1);
	PlayerTextDrawSetShadow(playerid, Speedometer[VehicleMileage][playerid], 0);

	levelTD[0][playerid] = CreatePlayerTextDraw(playerid, 501.000000, 99.000000, "LEVEL: 1");
	PlayerTextDrawBackgroundColor(playerid, levelTD[0][playerid], 255);
	PlayerTextDrawFont(playerid, levelTD[0][playerid], 1);
	PlayerTextDrawLetterSize(playerid, levelTD[0][playerid], 0.270000, 1.000000);
	PlayerTextDrawColor(playerid, levelTD[0][playerid], -1);
	PlayerTextDrawSetOutline(playerid, levelTD[0][playerid], 1);
	PlayerTextDrawSetProportional(playerid, levelTD[0][playerid], 1);
	PlayerTextDrawSetSelectable(playerid, levelTD[0][playerid], 0);

	levelTD[1][playerid] = CreatePlayerTextDraw(playerid, 574.000000, 99.000000, "10/20");
	PlayerTextDrawBackgroundColor(playerid, levelTD[1][playerid], 255);
	PlayerTextDrawFont(playerid, levelTD[1][playerid], 1);
	PlayerTextDrawLetterSize(playerid, levelTD[1][playerid], 0.270000, 1.000000);
	PlayerTextDrawColor(playerid, levelTD[1][playerid], -1);
	PlayerTextDrawSetOutline(playerid, levelTD[1][playerid], 1);
	PlayerTextDrawSetProportional(playerid, levelTD[1][playerid], 1);
	PlayerTextDrawSetSelectable(playerid, levelTD[1][playerid], 0);
}
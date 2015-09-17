#define LEVEL_CHAR			"×"
#define LEVEL_COLOR_A 		"{50C878}"
#define LEVEL_COLOR_B 		"{FFFFFF}"

#define MAX_SKILL_POINTS 	5

enum ePlayerPoints
{
	pAllowedPoints,
	pSkills[15]
}

new pPoints[MAX_PLAYERS][ePlayerPoints];

enum 
{
	SKILL_ECO = 0,	// do zrobienia
	SKILL_TIME,		// zrobione
	SKILL_DISTANCE,	// zrobione
	SKILL_REWARD,	// zrobione
	SKILL_PENATLY	// zrobione
}

new skillNames[5][32] = {
	"Ekonomiczna jazda", "Dowóz na czas", "D³ugi dystans", "Du¿a wartoœæ", "Mniejsza kara"
};

stock SetPlayerSkill(playerid, skill, level)
{
	if(level > 5 || level < 0)
		return 0;

	new levels[5];
	sscanf(pPoints[playerid][pSkills], "p<.>didid", levels[0], levels[1], levels[2], levels[3], levels[4]);
	levels[skill] = level;
	format(pPoints[playerid][pSkills], 15, "%d.%i.%d.%i.%d", levels[0], levels[1], levels[2], levels[3], levels[4]);
	return 1;
}

stock GetPlayerSkill(playerid, skill)
{
	new levels[5];
	sscanf(pPoints[playerid][pSkills], "p<.>didid", levels[0], levels[1], levels[2], levels[3], levels[4]);
	switch(skill)
	{
		case SKILL_ECO:
			return levels[0];
		case SKILL_TIME:
			return levels[1];
		case SKILL_DISTANCE:
			return levels[2];
		case SKILL_REWARD:
			return levels[3];
		case SKILL_PENATLY:
			return levels[4];
	}
	return 0;
}

stock GetColoredSkills(playerid, skill)
{
	new szString[48], skillLevel = GetPlayerSkill(playerid, skill);
	for(new i = 0; i < MAX_SKILL_POINTS; i++)
	{
		strcat(szString, LEVEL_CHAR);
	}
	if(skillLevel > 0)
	{	
		strins(szString, LEVEL_COLOR_B, skillLevel);
		strins(szString, LEVEL_COLOR_A, 0);
	}
	else
		strins(szString, LEVEL_COLOR_B, 0);
	return szString;
}

CMD:zdolnosci(playerid, params[])
{
	new option[10];
	if(sscanf(params, "s[10]", option))
		return Msg(playerid, COLOR_ERROR, "Wpisz: /zdolnosci [rozdaj/info]");

	if(strcmp(option, "rozdaj", true) == 0)
	{
		new szString[256];
		format(szString, sizeof szString, "Dostêpne punkty %d.\n \n", pPoints[playerid][pAllowedPoints]);
		for(new i = 0; i < 5; i++)
		{
			if(i < 1)
				strcat(szString, "{FFFFFF}");
			strcat(szString, skillNames[i]);
			strcat(szString, "\t");
			if(i > 0)
				strcat(szString, "\t");
			strcat(szString, GetColoredSkills(playerid, i));
			strcat(szString, "\n");
		}
		Dialog_Show(playerid, DIALOG_SKILL_INFO, DIALOG_STYLE_LIST, " ", szString, "Wybierz", "WyjdŸ");
	}
	else if(strcmp(option, "info", true) == 0)
	{
		new szString[720];
		strcat(szString, "{FFFFFF}");
		strcat(szString, "{50C878}Ekonomiczna jazda{FFFFFF} - specjalna umiejêtnoœæ która zmniejsza paliwo spalane przez pojazd (2%, 5%, 7%, 9%, 10%).");
		strcat(szString, "\n");
		strcat(szString, "{50C878}Dowóz na czas{FFFFFF} - specjalna umiejêtnoœæ która zwiêksza czas na dowóz towaru (1 minuta, 2 minuty, 3 minuty, 4 minuty, 5 minut).");
		strcat(szString, "\n");
		strcat(szString, "{50C878}D³ugi dystans{FFFFFF} - specjalna umiejêtnoœæ która umo¿liwia przewo¿enie towarów na d³u¿szych dystansach (1500j, 1750j, 2000j, 2250j, 2250j +).");
		strcat(szString, "\n");
		strcat(szString, "{50C878}Du¿a wartoœæ{FFFFFF} - specjalna umiejêtnoœæ która zwiêksza wynagrodzenie z towaru (2%, 5%, 7%, 9%, 10%).");
		strcat(szString, "\n");
		strcat(szString, "{50C878}Mniejsza kara{FFFFFF} - specjalna umiejêtnoœæ która zmniejsza karê z towaru (2%, 5%, 7%, 9%, 10%).");
		strcat(szString, "\n \n");
		strcat(szString, "{50C878}Wydanych punktów nie mo¿na przywróciæ.");
		Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, " ", szString, (!IsPlayerLogged(playerid)) ? ("Wstecz") : ("WyjdŸ"), #);
	}
	else
		return Msg(playerid, COLOR_ERROR, "Wpisz: /zdolnosci [rozdaj/info]");
	return 1;
}

stock GetReducedFuel(level)
{
	new percents;

	switch(level)
	{
		case 0: percents = 0;
		case 1: percents = 2;
		case 2: percents = 5;
		case 3: percents = 7;
		case 4: percents = 9;
		case 5: percents = 10;
	}
	return percents;
}

stock GetReducedTime(level)
{
	new time;

	switch(level)
	{
		case 0: time = 0;
		case 1: time = 1;
		case 2: time = 2;
		case 3: time = 3;
		case 4: time = 4;
		case 5: time = 5;
	}
	return time;
}

stock GetAllowedDistance(level)
{
	new distance;

	switch(level)
	{
		case 0: distance = 1500;
		case 1: distance = 1750;
		case 2: distance = 2000;
		case 3: distance = 2250;
		case 4: distance = 2500;
		case 5: distance = 10000;
	}
	return distance;
}

stock GetBiggerReward(level)
{
	new percents;

	switch(level)
	{
		case 0: percents = 0;
		case 1: percents = 2;
		case 2: percents = 5;
		case 3: percents = 7;
		case 4: percents = 9;
		case 5: percents = 10;
	}
	return percents;
}

stock GetLessPent(level)
{
	new percents;

	switch(level)
	{
		case 0: percents = 0;
		case 1: percents = 2;
		case 2: percents = 5;
		case 3: percents = 7;
		case 4: percents = 9;
		case 5: percents = 10;
	}
	return percents;
}

Dialog:DIALOG_SKILL_INFO(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	switch(listitem)
	{
		case 0: return cmd_zdolnosci(playerid, "rozdaj");
		case 1: return cmd_zdolnosci(playerid, "rozdaj");
		default: 
		{
			if(pPoints[playerid][pAllowedPoints] < 1)
				return cmd_zdolnosci(playerid, "rozdaj"), Msg(playerid, COLOR_ERROR, "Nie posiadasz punktów do rozdania.");
			if(GetPlayerSkill(playerid, listitem - 2) > 4)
				return cmd_zdolnosci(playerid, "rozdaj"), Msg(playerid, COLOR_ERROR, "Ta umiejêtnoœæ jest ju¿ maksymalnie rozwiniêta.");

			new szString[128], level = GetPlayerSkill(playerid, listitem - 2);
			SetPlayerSkill(playerid, listitem - 2, level+1);
			pPoints[playerid][pAllowedPoints]--;
			format(szString, sizeof szString, "Rozwin¹³eœ umiejêtnoœc {b}%s{/b} na poziom {b}%d{/b}.", skillNames[listitem-2], level+1);
			Msg(playerid, COLOR_INFO, szString);
			return cmd_zdolnosci(playerid, "rozdaj");
		}
	}
	return 1;
}
/*

	Made by Joe Staff.
	Edited by SerwerTruck.eu.

*/

#define _UPDATER 			100

forward UpdateFadeLogoTimer();
forward OnFadeLogoComplete(beforehold);
forward Logo_OnGameModeInit();
forward Logo_OnGameModeExit();

new EndColor[4], 
	CurrentColor[4], 
	RateColor[4], 
	Step, 
	Hold;

new Text:sLogo[3],
	LogoFade;

stock FadeLogoColor(RR1, GG1, BB1, AA1, RR2, GG2, BB2, AA2, steps, holds)
{
	Step = steps;
	new tmpsteps;
	Hold = holds; 
	EndColor[0] = 		RR2;
	EndColor[1] = 		GG2;
	EndColor[2] = 		BB2;
	EndColor[3] = 		AA2;
	CurrentColor[0] = 	RR1;
	CurrentColor[1] = 	GG1;
	CurrentColor[2] = 	BB1;
	CurrentColor[3] = 	AA1;
	RateColor[0] = 		(RR1 - RR2) / steps;
	RateColor[1] = 		(GG1 - GG2) / steps;
	RateColor[2] = 		(BB1 - BB2) / steps;
	RateColor[3] = 		(AA1 - AA2) / steps;

	if(RateColor[0] != 0)
		if(((RR1-RR2) / RateColor[0]) > steps)
			tmpsteps = ((RR1-RR2) / RateColor[0]) - steps;
	if(RateColor[1] != 0)
		if(((GG1-GG2) / RateColor[1]) > steps + tmpsteps) 
			tmpsteps = ((GG1-GG2)/RateColor[1])-steps;
	if(RateColor[2] != 0)
		if(((BB1-BB2) / RateColor[2]) > steps + tmpsteps)
			tmpsteps = ((BB1-BB2)/RateColor[2]) - steps;
	if(RateColor[3] != 0)
		if(((AA1-AA2) / RateColor[3]) > steps + tmpsteps)
			tmpsteps = ((AA1-AA2)/RateColor[3]) - steps;
	if(tmpsteps)
		Step += tmpsteps+1;
}

stock FadeLogoStop()
{
	TextDrawHideForAll(sLogo[0]);
	TextDrawHideForAll(sLogo[1]);
	TextDrawHideForAll(sLogo[2]);
}

stock FadeLogoInit()
{
	sLogo[0] = TextDrawCreate(36.000000, 424.000000, "Serwer Truck");
	TextDrawBackgroundColor(sLogo[0], 255);
	TextDrawFont(sLogo[0], 2);
	TextDrawLetterSize(sLogo[0], 0.320000, 0.899999);
	TextDrawColor(sLogo[0], -1);
	TextDrawSetOutline(sLogo[0], 1);
	TextDrawSetProportional(sLogo[0], 1);

	sLogo[1] = TextDrawCreate(111.000000, 416.000000, ".");
	TextDrawAlignment(sLogo[1], 2);
	TextDrawBackgroundColor(sLogo[1], 0);
	TextDrawFont(sLogo[1], 2);
	TextDrawLetterSize(sLogo[1], 20.000000, 2.599999);
	TextDrawColor(sLogo[1], -1);
	TextDrawSetOutline(sLogo[1], 0);
	TextDrawSetProportional(sLogo[1], 1);
	TextDrawSetShadow(sLogo[1], 0);

	sLogo[2] = TextDrawCreate(43.000000, 437.000000, "v0.3a-06/08/2015");
	TextDrawBackgroundColor(sLogo[2], 255);
	TextDrawFont(sLogo[2], 2);
	TextDrawLetterSize(sLogo[2], 0.210000, 0.699999);
	TextDrawColor(sLogo[2], -1);
	TextDrawSetOutline(sLogo[2], 1);
	TextDrawSetProportional(sLogo[2], 1);

	SetTimer("UpdateFadeLogoTimer", _UPDATER, true);
	return 1;
}

stock FadeLogoExit()
{
	TextDrawDestroy(sLogo[0]);
	TextDrawDestroy(sLogo[1]);
	TextDrawDestroy(sLogo[2]);
	return 1;
}

public UpdateFadeLogoTimer()
{
	if(Step)
	{
		Step--;
		for(new color; color < 4; color++)
		{
			CurrentColor[color] -= RateColor[color];
			if(RateColor[color] > 0)
			{
				if(CurrentColor[color] < EndColor[color])
					CurrentColor[color] = EndColor[color];
			}
			if(RateColor[color] < 0)
			{
				if(CurrentColor[color] > EndColor[color])
					CurrentColor[color] = EndColor[color];
			}
			if(CurrentColor[color] < 0)
				CurrentColor[color] = 0;
			if(CurrentColor[color] > 255)
				CurrentColor[color] = 255;
		}
		TextDrawColor(sLogo[0], ConvertToColor(CurrentColor[0], CurrentColor[1], CurrentColor[2], CurrentColor[3]));
		TextDrawColor(sLogo[1], ConvertToColor(CurrentColor[0], CurrentColor[1], CurrentColor[2], CurrentColor[3]));
		TextDrawColor(sLogo[2], ConvertToColor(CurrentColor[0], CurrentColor[1], CurrentColor[2], CurrentColor[3]));
		TextDrawShowForAll(sLogo[0]);
		TextDrawShowForAll(sLogo[1]);
		TextDrawShowForAll(sLogo[2]);
	}
	else if(Hold)
	{
		Hold--;
	}
	else if(Hold == 0)
	{
		Hold = -1;

		CallLocalFunction("OnFadeLogoComplete", "i", 0);
	}
}

public Logo_OnGameModeInit()
{
	FadeLogoInit();
	return 1;
}

public Logo_OnGameModeExit()
{
	FadeLogoExit();
	return 1;
}

public OnFadeLogoComplete(beforehold)
{
	switch(LogoFade)
	{
		case 0:
		{
			FadeLogoColor(245, 88, 88, 255, 245, 88, 245, 255, 50, 10);
			LogoFade = 1;
		}
		case 1:
		{
			FadeLogoColor(245, 88, 245, 255, 88, 88, 245, 255, 50, 10);
			LogoFade = 2;
		}
		case 2:
		{
			FadeLogoColor(88, 88, 245, 255, 88, 245, 245, 255, 50, 10);
			LogoFade = 3;
		}
		case 3:
		{
			FadeLogoColor(88, 245, 245, 255, 88, 245, 88, 255, 50, 10);
			LogoFade = 4;
		}
		case 4:
		{
			FadeLogoColor(88, 245, 88, 255, 245, 88, 88, 255, 50, 10);
			LogoFade = 5;
		}
		case 5:
		{
			FadeLogoColor(245, 88, 88, 255, 245, 88, 88, 255, 50, 10);
			LogoFade = 0;
		}
	}
	return 1;
}
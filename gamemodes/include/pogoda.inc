enum eWeather
{
	wActual,
	wChange,
	wTimeStart,
	wTime,
	wWeather,
	wNextWeather,
	wNextTime,
	wText[170]
}

new wInfo[eWeather];

stock SetWeatherEx(weather)
{
	SetWeather(weather);
	wInfo[wActual] = weather;
	wInfo[wTimeStart] = gettime();
 	return 1;   
}

#if defined _ALS_SetWeather
  #undef SetWeather 
#else 
#define _ALS_SetWeather 
#endif

#define SetWeather SetWeatherEx

new allowedWeathers[] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18
};

new weatherNames[][] = {
	"bardzo s³onecznie",
	"s³onecznie",
	"bardzo s³onecznie ze smogiem",
	"s³onecznie ze smogiem",
	"pochmurnie",
	"s³onecznie",
	"bardzo s³onecznie",
	"pochmurnie",
	"deszczowo",
	"mgliœcie",
	"s³onecznie",
	"bardzo s³onecznie",
	"pochmurnie",
	"bardzo s³onecznie",
	"s³onecznie",
	"pochmurnie",
	"deszczowo",
	"bardzo s³onecznie",
	"s³onecznie"
};

stock GetWeather()
{
	return wInfo[wActual];
}

stock GetWeatherName(weather)
{
	new szName[34];
	format(szName, sizeof szName, "%s", weatherNames[weather]);
	return szName;
}

forward Pogoda_OnGameModeInit();
public Pogoda_OnGameModeInit()
{
	DrawWeather();
	WeatherChange();
}

stock DrawWeather()
{
	wInfo[wWeather] = allowedWeathers[random(sizeof allowedWeathers)];
	wInfo[wNextWeather] = allowedWeathers[random(sizeof allowedWeathers)];
	wInfo[wTime] = 5 + random(15);
	wInfo[wNextTime] = 5 + random(15);
	return 1;
}

forward WeatherChange();
public WeatherChange()
{
	if(wInfo[wChange] == 0)
	{
		SetWeather(wInfo[wWeather]);
		SetTimer("WeatherChange", wInfo[wTime] * 60 * 1000, false);
		wInfo[wChange] = 1;
	}
	else 
	{
		SetWeather(wInfo[wNextWeather]);
		SetTimer("WeatherChange", wInfo[wNextTime] * 60 * 1000, false);
		wInfo[wChange] = 0;
		DrawWeather();
	}
	WeatherInfo();
	return 1;
}

stock WeatherInfo()
{
	new 
		probability = random(50) + 30, 
		weather, 
		time[6],
		timestamp = (GetWeatherTime()*60) + wInfo[wTimeStart];

	TimestampToDate(timestamp, time[0], time[1], time[2], time[3], time[4], time[5]);

	if(chanceEx(probability))
		weather = GetNextWeather();
	else
		weather = allowedWeathers[random(sizeof allowedWeathers)];

	format(wInfo[wText], sizeof wInfo[wText], "Obecna pogoda bêdzie utrzymywa³a siê do godziny {b}%02d:%02d{/b}. \nNastêpnie ma byæ {b}%s{/b} przez {b}%d{/b} minut. Poprawnoœæ wyników wynosi {b}%d{/b} %%.", 
		time[3], 
		time[4],
		GetWeatherName(weather),
		GetNextWeatherTime(),
		probability
	);
	return 1;
}

stock chanceEx(chance)
{
	new result = random(100);
	if(result <= chance)
		return true;
	return false;
}

stock GetNextWeather()
{
	if(wInfo[wChange] == 0)
		return wInfo[wWeather];
	else 
		return wInfo[wNextWeather];
}

stock GetWeatherTime()
{
	if(wInfo[wChange] == 0)
		return wInfo[wTime];
	else 
		return wInfo[wNextTime];
}

stock GetNextWeatherTime()
{
	if(wInfo[wChange] == 1)
		return wInfo[wTime];
	else 
		return wInfo[wNextTime];
}

new MonthTimes[12][4] =
{
		{ 31, 31, 2678400, 2678400 },
		{ 28, 29, 2419200, 2505600 },
		{ 31, 31, 2678400, 2678400 },
		{ 30, 30, 2592000, 2592000 },
		{ 31, 31, 2678400, 2678400 },
		{ 30, 30, 2592000, 2592000 },
		{ 31, 31, 2678400, 2678400 },
		{ 31, 31, 2678400, 2678400 },
		{ 30, 30, 2592000, 2592000 },
		{ 31, 31, 2678400, 2678400 },
		{ 30, 30, 2592000, 2592000 },
		{ 31, 31, 2678400, 2678400 }
};
 
stock IsLeapYear(year)
{
		if(year % 4 == 0) return 1;
		else return 0;
}
 
stock TimestampToDate(Timestamp, &year, &month, &day, &hour, &minute, &second, HourGMT = 2, MinuteGMT = 0)
{
		new tmp = 2;
		year = 1970;
		month = 1;
		Timestamp -= 172800;
		for(;;)
		{
				if(Timestamp >= 31536000)
				{
						year ++;
						Timestamp -= 31536000;
						tmp ++;
						if(tmp == 4)
						{
								if(Timestamp >= 31622400)
								{
										tmp = 0;
										year ++;
										Timestamp -= 31622400;
								}
								else break;
						}
				}
				else break;
		}			  
		for(new i = 0; i < 12; i ++)
		{
				if(Timestamp >= MonthTimes[i][2 + IsLeapYear(year)])
				{
						month ++;
						Timestamp -= MonthTimes[i][2 + IsLeapYear(year)];
				}
				else break;
		}
		day = 1 + (Timestamp / 86400);
		Timestamp %= 86400;
		hour = HourGMT + (Timestamp / 3600);
		Timestamp %= 3600;
		minute = MinuteGMT + (Timestamp / 60);
		second = (Timestamp % 60);
		if(minute > 59)
		{
				minute = 0;
				hour ++;
		}
		if(hour > 23)
		{
				hour -= 24;
				day ++;
		}	  
		if(day > MonthTimes[month][IsLeapYear(year)])
		{
				day = 1;
				month ++;
		}
		if(month > 12)
		{
				month = 1;
				year ++;
		}
		return 1;
}

CMD:pogodynka(playerid)
{
	new szTemp[200];
	copy(wInfo[wText], szTemp);
	replace(szTemp, "{b}", "{488CDB}");
	replace(szTemp, "{/b}", "{FFFFFF}");
	Dialog_Show(playerid, DIALOG_WEATHER, DIALOG_STYLE_MSGBOX, " ", szTemp, "WyjdŸ", #);
	return 1;
}

Dialog:DIALOG_WEATHER(playerid, response, listitem, inputtext[])
{
	return 1;
}

stock replace(src[], const sub[], const rep[], idx = 0)
{
	new
		f = strfind(src, sub, false, idx),
		srclen = strlen(src),
		sublen = strlen(sub),
		replen = strlen(rep),
		count;
 
	if(!srclen || !sublen || !replen)
		return 0;
 
	if(idx >= sublen)
		return 0;
 
	while(f >= 0)
	{
		count++;
		strdel(src, f + idx, f + sublen);
		strins(src, rep, f + idx, f + replen);
		f = strfind(src, sub, false, idx);
	}
	return count;
}
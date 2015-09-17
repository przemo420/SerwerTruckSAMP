enum eWeather
{
	wActual,
	wChange,
	wTimeStart,
	wTime,
	wWeather,
	wNextWeather,
	wNextTime,
	wText[256]
}

new weatherInfo[eWeather];

stock SetWeatherEx(weather)
{
	SetWeather(weather);
	weatherInfo[wActual] = weather;
	weatherInfo[wTimeStart] = gettime();
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
	return weatherInfo[wActual];
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
	weatherInfo[wWeather] = allowedWeathers[random(sizeof allowedWeathers)];
	weatherInfo[wNextWeather] = allowedWeathers[random(sizeof allowedWeathers)];
	weatherInfo[wTime] = 5 + random(15);
	weatherInfo[wNextTime] = 5 + random(15);
	return 1;
}

forward WeatherChange();
public WeatherChange()
{
	if(weatherInfo[wChange] == 0)
	{
		SetWeather(weatherInfo[wWeather]);
		SetTimer("WeatherChange", weatherInfo[wTime] * 60 * 1000, false);
		weatherInfo[wChange] = 1;
	}
	else 
	{
		SetWeather(weatherInfo[wNextWeather]);
		SetTimer("WeatherChange", weatherInfo[wNextTime] * 60 * 1000, false);
		weatherInfo[wChange] = 0;
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
		timestamp = (GetWeatherTime()*60) + weatherInfo[wTimeStart];

	TimestampToDate(timestamp, time[0], time[1], time[2], time[3], time[4], time[5]);

	if(chanceEx(probability))
		weather = GetNextWeather();
	else
		weather = allowedWeathers[random(sizeof allowedWeathers)];

	format(weatherInfo[wText], sizeof weatherInfo[wText], "Obecna pogoda {488CDB}(%s){FFFFFF} bêdzie utrzymywa³a siê do godziny {488CDB}%02d:%02d{FFFFFF}. \nNastêpnie ma byæ {488CDB}%s{FFFFFF} przez {488CDB}%d{FFFFFF} minut. Poprawnoœæ wyników wynosi {488CDB}%d{FFFFFF} %%.", 
		weatherNames[weatherInfo[wWeather]], /* przy GetWeatherName undefined symbol dawa³o, haha :D */
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
	if(weatherInfo[wChange] == 0)
		return weatherInfo[wWeather];
	else 
		return weatherInfo[wNextWeather];
}

stock GetWeatherTime()
{
	if(weatherInfo[wChange] == 0)
		return weatherInfo[wTime];
	else 
		return weatherInfo[wNextTime];
}

stock GetNextWeatherTime()
{
	if(weatherInfo[wChange] == 1)
		return weatherInfo[wTime];
	else 
		return weatherInfo[wNextTime];
}


CMD:pogodynka(playerid)
	return Dialog_Show(playerid, DIALOG_WEATHER, DIALOG_STYLE_MSGBOX, " ", weatherInfo[wText], "WyjdŸ", #);

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
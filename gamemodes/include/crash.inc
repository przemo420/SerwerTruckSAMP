#define MAX_CRASHS 80

enum E_CRASH
{
	crashNick[MAX_PLAYER_NAME],
	Float:crashPos[3],
	crashTime
}

new pCrash[MAX_CRASHS][E_CRASH];
new Float:pTempPos[MAX_PLAYERS][3];

forward Crash_OnGameModeInit();
public Crash_OnGameModeInit()
{
	SetTimer("TruncateCrashTable", 5 * 60 * 1000, true);
	return 1;
}

forward Crash_OnPlayerConnect(playerid);
public Crash_OnPlayerConnect(playerid)
{
	for(new i = 0; i < MAX_CRASHS; i++)
	{
		if(!IsValidCrash(i))
			continue;

		if(!strcmp(PlayerName(playerid), pCrash[i][crashNick]))
		{
			if((gettime() - pCrash[i][crashTime]) <= 300) // ca³e 5 minut na powrót
			{
				Msg(playerid, COLOR_INFO, "Twoje statystyki zosta³y przywrócone. Mo¿esz teleportowaæ siê do pozycji przed crashem/timeoutem u¿ywaj¹c komendy {b}/lastpos{/b}.");
				pTempPos[playerid][0] = pCrash[i][crashPos][0];
				pTempPos[playerid][1] = pCrash[i][crashPos][1];
				pTempPos[playerid][2] = pCrash[i][crashPos][2];
			}

			pCrash[i][crashTime] = 0;
			pCrash[i][crashPos][0] = 0.0;
			pCrash[i][crashPos][1] = 0.0;
			pCrash[i][crashPos][2] = 0.0;
			pCrash[i][crashNick] = EOS;
			break;
		}
	}
	return 1;
}

forward Crash_OnPlayerDisconnect(playerid, reason);
public Crash_OnPlayerDisconnect(playerid, reason)
{
	if(reason == 0)
	{
		for(new i = 0; i < MAX_CRASHS; i++)
		{
			if(IsFreeCrash(i))
			{
				strcat(pCrash[i][crashNick], PlayerName(playerid));
				GetPlayerPos(playerid, pCrash[i][crashPos][0], pCrash[i][crashPos][1], pCrash[i][crashPos][2]);
				pCrash[i][crashTime] = gettime();
				break;
			}
		}
	}
	if(pTempPos[playerid][0] != 0 && pTempPos[playerid][1] != 0 && pTempPos[playerid][2] != 0)
	{
		pTempPos[playerid][0] = 0.0;
		pTempPos[playerid][1] = 0.0;
		pTempPos[playerid][2] = 0.0;
	}
	return 1;
}

stock IsValidCrash(crashid)
{
	if(pCrash[crashid][crashTime] <= 0)
		return false;
	return true;
}

stock IsFreeCrash(crashid)
{
	if(pCrash[crashid][crashTime] <= 0)
		return true;
	return false;
}

forward TruncateCrashTable();
public TruncateCrashTable()
{
	for(new i = 0; i < MAX_CRASHS; i++)
	{
		if((gettime() - pCrash[i][crashTime]) > 300) // ca³e 5 minut na powrót, teraz tesetujemy je¿eli gracz nie wróci³ ;P 
		{
			pCrash[i][crashTime] = 0;
			pCrash[i][crashPos][0] = 0.0;
			pCrash[i][crashPos][1] = 0.0;
			pCrash[i][crashPos][2] = 0.0;
			pCrash[i][crashNick] = EOS;
		}
	}
	return 1;
}

CMD:lastpos(playerid)
{
	if(pTempPos[playerid][0] == 0 && pTempPos[playerid][1] == 0 && pTempPos[playerid][2] == 0)
		return Msg(playerid, COLOR_INFO, "Brak pozycji do wczytania.");
	Teleport(playerid, pTempPos[playerid][0], pTempPos[playerid][1], pTempPos[playerid][2], true);
	Msg(playerid, COLOR_INFO, "Pozycja {b}crashu / timeoutu{/b} zosta³a wczytana.");
	pTempPos[playerid][0] = 0.0;
	pTempPos[playerid][1] = 0.0;
	pTempPos[playerid][2] = 0.0;
	return 1;
}
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/*     iAchieve - Dynamic Achievement System, Release by iPLEOMAX 2011 ©      */
/* Just another include which saves players achievements                      */
/* And Notifies them on Unlock. You can also give Money,Score,Car w/e..       */
/* Callback:   OnPlayerAchieve( playerid, achid ) {}                          */
/*                                                                            */
/* This system uses Y_INI, Y_HOOKS & foreach by Y_Less. (BIG THANKS!)         */
/* In order to save things correctly, make sure you create this folder:       */
/* >- scriptfiles\achievements\user (Here is where all ach-userfiles save)    */
/*                                                                            */
/* Note: Do not release any other edit of this INC without my permission.     */
/* Thanks to;                                                                 */
/*           - Y_Less for his includes                                        */
/*           - LCI Ach System by Deskoft for inspiration. Not it's edit btw.  */
/*           - Me for everything else.                                        */
/*                                                                            */
/* Please do not remove any credits!  Thanks for downloading!                 */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

#if defined iach
	#endinput
#endif

#if !defined _samp_included
	#tryinclude <a_samp>
	#if !defined _samp_included
		#error Unable to load a_samp.inc
	#endif
#endif
#if !defined MAX_INI_STRING
    #tryinclude <YSI\y_ini>
    #if !defined MAX_INI_STRING
        #error Unable to load YSI\y_ini.inc Please check your includes.
    #endif
#endif
#if !defined Hook
    #tryinclude <YSI\y_hooks>
    #if !defined Hook
        #error Unable to load YSI\y_hooks.inc Please check your includes.
    #endif
#endif
#if !defined foreach
    #tryinclude <foreach>
    #if !defined foreach
        #error Unable to load foreach.inc Please check your includes.
    #endif
#endif

#define iach                                                                    //ignore this.
#define MAX_ACHIEVEMENTS 		25                                              //Number of achievements you'll have in your GM.
#define ACH_TIMER_UPDATE        2 												//Check for Unlocks Per Player Per Second.
#define ACH_USER_NAME			"achievements/user/%s.ini"                      //Path where user achievements are saving.
#define ACH_SHOW_TIME           8                                               //Seconds for the "Ach Unlocked" to stay on screen.
#define ACH_SOUND_ID            1130                                            //You can put a music id here..
//#define ACH_ANIM_ON 															//use this if you want the textdraw to shine (LAG).

enum PLAYER_ACH_DATA
{
	achprogress[MAX_ACHIEVEMENTS+1],
	achieved[MAX_ACHIEVEMENTS+1]
};

enum MAIN_ACH_DATA
{
    ACH_WINVALUES,
    ACH_TITLE[32],
	ACH_INFO[128]
};

new
	bool:Ach_JustJoined[MAX_PLAYERS],
    Text:DynamicAchTD[MAX_PLAYERS][2],
    Text:AchTD[6],
	ACH_NUM,
	ACH_TIMER,
	ACH_USER[MAX_PLAYERS][PLAYER_ACH_DATA],
	ACH_MAIN[MAX_ACHIEVEMENTS][MAIN_ACH_DATA]
;

/* Achievement Natives:
native CreateAchievement(title[], info[], unlockvalue);
native LoadPlayerAchievements(playerid);
native SetPlayerAchievement(playerid, achid, value);
native GivePlayerAchievement(playerid, achid, value);
native GetPlayerAchievement(playerid, achid);
native bool:DidPlayerAchieve(playerid, achid);
*/

#if defined FILTERSCRIPT
Hook:ach_OnFilterScriptInit() return Init_AchSystem();
Hook:ach_OnFilterScripExit() return Exit_AchSystem();
#else
Hook:ach_OnGameModeInit() return Init_AchSystem();
Hook:ach_OnGameModeExit() return Exit_AchSystem();
#endif


Hook:ach_OnPlayerConnect(playerid)
{
    Ach_JustJoined[playerid] = true;
    SetTimerEx("Ach_CJJ", 5000, false, "d", playerid);
    
    LoadPlayerAchievements(playerid);

    HideAch(playerid);

    DynamicAchTD[playerid][0] = TextDrawCreate(313.000000, 213.000000, "TITLE");
	TextDrawAlignment(DynamicAchTD[playerid][0], 2);
	TextDrawBackgroundColor(DynamicAchTD[playerid][0], 255);
	TextDrawFont(DynamicAchTD[playerid][0], 2);
	TextDrawLetterSize(DynamicAchTD[playerid][0], 0.519999, 1.700000);
	TextDrawColor(DynamicAchTD[playerid][0], -1);
	TextDrawSetOutline(DynamicAchTD[playerid][0], 0);
	TextDrawSetProportional(DynamicAchTD[playerid][0], 1);
	TextDrawSetShadow(DynamicAchTD[playerid][0], 0);

	DynamicAchTD[playerid][1] = TextDrawCreate(313.000000, 235.000000, "INFO");
	TextDrawAlignment(DynamicAchTD[playerid][1], 2);
	TextDrawBackgroundColor(DynamicAchTD[playerid][1], 255);
	TextDrawFont(DynamicAchTD[playerid][1], 2);
	TextDrawLetterSize(DynamicAchTD[playerid][1], 0.290000, 1.200000);
	TextDrawColor(DynamicAchTD[playerid][1], -1);
	TextDrawSetOutline(DynamicAchTD[playerid][1], 0);
	TextDrawSetProportional(DynamicAchTD[playerid][1], 1);
	TextDrawSetShadow(DynamicAchTD[playerid][1], 0);
	return true;
}

forward Ach_CJJ(playerid);
public Ach_CJJ(playerid)
{
	Ach_JustJoined[playerid] = false;
	return true;
}

Hook:ach_OnPlayerDisconnect(playerid, reason)
{
    SavePlayerAchievements(playerid);
    
    for(new a=0; a<MAX_ACHIEVEMENTS; a++)
	{
	    ACH_USER[playerid][achprogress][a] = 0;
	    ACH_USER[playerid][achieved][a] = 0; //Fixed.
	}
	
	return true;
}

forward Init_AchSystem();
public Init_AchSystem()
{
	print("\n  * iAchieve :: Advanced Achievement System by iPLEOMAX, 2011 *\n");

	ACH_TIMER = SetTimer("AchCheck", ACH_TIMER_UPDATE*1000, true);

	#if defined ACH_ANIM_ON
		AnimateAchTD(0);
	#endif

    AchTD[0] = TextDrawCreate(495.000000, 190.000000, "Box0");
	TextDrawBackgroundColor(AchTD[0], 255);
	TextDrawFont(AchTD[0], 1);
	TextDrawLetterSize(AchTD[0], 0.000000, 9.100000);
	TextDrawColor(AchTD[0], -1);
	TextDrawSetOutline(AchTD[0], 0);
	TextDrawSetProportional(AchTD[0], 1);
	TextDrawSetShadow(AchTD[0], 1);
	TextDrawUseBox(AchTD[0], 1);
	TextDrawBoxColor(AchTD[0], 90);
	TextDrawTextSize(AchTD[0], 141.000000, 0.000000);

	AchTD[1] = TextDrawCreate(495.000000, 190.000000, "Box1");
	TextDrawBackgroundColor(AchTD[1], 255);
	TextDrawFont(AchTD[1], 1);
	TextDrawLetterSize(AchTD[1], 0.000000, 4.299999);
	TextDrawColor(AchTD[1], -1);
	TextDrawSetOutline(AchTD[1], 0);
	TextDrawSetProportional(AchTD[1], 1);
	TextDrawSetShadow(AchTD[1], 1);
	TextDrawUseBox(AchTD[1], 1);
	TextDrawBoxColor(AchTD[1], -241);
	TextDrawTextSize(AchTD[1], 141.000000, 0.000000);

	AchTD[2] = TextDrawCreate(187.000000, 191.000000, "ACHIEVEMENT UNLOCKED");
	TextDrawBackgroundColor(AchTD[2], 255);
	TextDrawFont(AchTD[2], 1);
	TextDrawLetterSize(AchTD[2], 0.649999, 2.000000);
	TextDrawColor(AchTD[2], 16711935);
	TextDrawSetOutline(AchTD[2], 0);
	TextDrawSetProportional(AchTD[2], 1);
	TextDrawSetShadow(AchTD[2], 0);

	AchTD[3] = TextDrawCreate(495.000000, 194.000000, "Box2");
	TextDrawBackgroundColor(AchTD[3], 255);
	TextDrawFont(AchTD[3], 1);
	TextDrawLetterSize(AchTD[3], 0.000000, 9.100000);
	TextDrawColor(AchTD[3], -1);
	TextDrawSetOutline(AchTD[3], 0);
	TextDrawSetProportional(AchTD[3], 1);
	TextDrawSetShadow(AchTD[3], 1);
	TextDrawUseBox(AchTD[3], 1);
	TextDrawBoxColor(AchTD[3], 90);
	TextDrawTextSize(AchTD[3], 141.000000, 0.000000);

	AchTD[4] = TextDrawCreate(450.000000, 192.000000, "Box3");
	TextDrawBackgroundColor(AchTD[4], 255);
	TextDrawFont(AchTD[4], 1);
	TextDrawLetterSize(AchTD[4], 0.000000, 1.700000);
	TextDrawColor(AchTD[4], -1);
	TextDrawSetOutline(AchTD[4], 0);
	TextDrawSetProportional(AchTD[4], 1);
	TextDrawSetShadow(AchTD[4], 1);
	TextDrawUseBox(AchTD[4], 1);
	TextDrawBoxColor(AchTD[4], 16711690);
	TextDrawTextSize(AchTD[4], 182.000000, 0.000000);

	AchTD[5] = TextDrawCreate(317.000000, 193.000000, "]                                            ]");
	TextDrawAlignment(AchTD[5], 2);
	TextDrawBackgroundColor(AchTD[5], 255);
	TextDrawFont(AchTD[5], 2);
	TextDrawLetterSize(AchTD[5], 0.500000, 1.600000);
	TextDrawColor(AchTD[5], 16711935);
	TextDrawSetOutline(AchTD[5], 0);
	TextDrawSetProportional(AchTD[5], 1);
	TextDrawSetShadow(AchTD[5], 0);

	return true;
}

forward Exit_AchSystem();
public Exit_AchSystem()
{
    KillTimer(ACH_TIMER);
    return true;
}

forward CreateAchievement(title[], info[], unlockvalue);
public CreateAchievement(title[], info[], unlockvalue)
{
	format(ACH_MAIN[ACH_NUM][ACH_TITLE], 32, "%s", title);
	format(ACH_MAIN[ACH_NUM][ACH_INFO], 128, "%s", info);
	ACH_MAIN[ACH_NUM][ACH_WINVALUES] = unlockvalue;

	if(ACH_NUM > MAX_ACHIEVEMENTS) printf("  **[WARNING] Number of achievements created is exceeding the max allowed limit.");

	return ACH_NUM, ACH_NUM++;
}

stock GivePlayerAchievement(playerid, achid, value)
{
	ACH_USER[playerid][achprogress][achid] += value;
	return true;
}

stock SetPlayerAchievement(playerid, achid, value)
{
	ACH_USER[playerid][achprogress][achid] = value; //Fix2
	return true;
}
stock GetPlayerAchievement(playerid, achid)
{
	return ACH_USER[playerid][achprogress][achid];
}
stock DidPlayerAchieve(playerid, achid)
{
	return ACH_USER[playerid][achieved][achid];
}

forward SavePlayerAchievements(playerid);
public SavePlayerAchievements(playerid)
{
    new achtemp[128], achpname[MAX_PLAYER_NAME];
	GetPlayerName( playerid, achpname, sizeof achpname );
	format( achtemp, sizeof achtemp, "achievements/user/%s.ini", achpname ); /*IMP*/
	new INI:ACHNAME = INI_Open(achtemp);

	for(new a=0; a<MAX_ACHIEVEMENTS; a++)
	{
	    format(achtemp, sizeof achtemp, "ach%i", a);
     	INI_WriteInt(ACHNAME, achtemp, ACH_USER[playerid][achprogress][a]);
     	format(achtemp, sizeof achtemp, "unl%i", a);
		INI_WriteInt(ACHNAME, achtemp, ACH_USER[playerid][achieved][a]);
	}
	INI_Close(ACHNAME);
	return true;
}

forward LoadPlayerAchievements(playerid);
public LoadPlayerAchievements(playerid)
{
    new achtemp[128], achpname[MAX_PLAYER_NAME];
	GetPlayerName( playerid, achpname, sizeof achpname );
	format( achtemp, sizeof achtemp, "achievements/user/%s.ini", achpname );
	if(fexist(achtemp))
	{
		INI_ParseFile(achtemp, "load_achfile", .bExtra = true, .extra = playerid);
	}
	return true;
}

forward load_achfile(playerid, name[], value[]);
public load_achfile(playerid, name[], value[])
{
	new achentry[6];
	for(new a=0; a<MAX_ACHIEVEMENTS; a++)
	{
		format(achentry, sizeof achentry, "ach%i", a);
		INI_Int(achentry, ACH_USER[playerid][achprogress][a]);
		format(achentry, sizeof achentry, "unl%i", a);
		INI_Int(achentry, ACH_USER[playerid][achieved][a]);
	}
    return true;
}

forward AchCheck();
public  AchCheck()
{
	foreach(Player, i)
	{
		if(Ach_JustJoined[i]) continue;
		for(new a=0; a<MAX_ACHIEVEMENTS; a++)
		{
			if(ACH_USER[i][achieved][a] != 1 && ACH_MAIN[a][ACH_WINVALUES] != 0)
			{
		    	if(ACH_USER[i][achprogress][a] >= ACH_MAIN[a][ACH_WINVALUES])
				{
                    ACH_USER[i][achieved][a] = 1;
                    ShowPlayerAchievement(i, a);
                    CallLocalFunction("OnPlayerAchieve", "dd", i, a);
				}
			}
		}
	}
	return true;
}

forward ShowPlayerAchievement(playerid, achid);
public ShowPlayerAchievement(playerid, achid)
{
	TextDrawShowForPlayer(playerid, AchTD[0]);
	TextDrawShowForPlayer(playerid, AchTD[1]);
	TextDrawShowForPlayer(playerid, AchTD[2]);
	TextDrawShowForPlayer(playerid, AchTD[3]);
	TextDrawShowForPlayer(playerid, AchTD[4]);
	TextDrawShowForPlayer(playerid, AchTD[5]);
	TextDrawSetString(DynamicAchTD[playerid][0], ACH_MAIN[achid][ACH_TITLE]);
	TextDrawSetString(DynamicAchTD[playerid][1], ACH_MAIN[achid][ACH_INFO]);
	TextDrawShowForPlayer(playerid, DynamicAchTD[playerid][0]);
	TextDrawShowForPlayer(playerid, DynamicAchTD[playerid][1]);
	PlayerPlaySound(playerid, ACH_SOUND_ID, 0, 0, 0);
	SetTimerEx("HideAch", 7000, false, "d", playerid);
	return true;
}

forward HideAch(playerid);
public HideAch(playerid)
{
    PlayerPlaySound(playerid, 0, 0, 0, 0);
    TextDrawHideForPlayer(playerid, AchTD[0]);
	TextDrawHideForPlayer(playerid, AchTD[1]);
	TextDrawHideForPlayer(playerid, AchTD[2]);
	TextDrawHideForPlayer(playerid, AchTD[3]);
	TextDrawHideForPlayer(playerid, AchTD[4]);
	TextDrawHideForPlayer(playerid, AchTD[5]);
	TextDrawHideForPlayer(playerid, DynamicAchTD[playerid][0]);
	TextDrawHideForPlayer(playerid, DynamicAchTD[playerid][1]);
	return true;
}

forward AnimateAchTD(var); //(Creates Lag i assume)
public AnimateAchTD(var)
{
	switch(var)
	{
	    case 0: TextDrawSetString(AchTD[2], "~g~~w~A~g~CHIEVEMENT UNLOCKED");
	    case 1: TextDrawSetString(AchTD[2], "~g~A~w~C~g~HIEVEMENT UNLOCKED");
	    case 2: TextDrawSetString(AchTD[2], "~g~AC~w~H~g~IEVEMENT UNLOCKED");
	    case 3: TextDrawSetString(AchTD[2], "~g~ACH~w~I~g~EVEMENT UNLOCKED");
	    case 4: TextDrawSetString(AchTD[2], "~g~ACHI~w~E~g~VEMENT UNLOCKED");
	    case 5: TextDrawSetString(AchTD[2], "~g~ACHIE~w~V~g~EMENT UNLOCKED");
	    case 6: TextDrawSetString(AchTD[2], "~g~ACHIEV~w~E~g~MENT UNLOCKED");
	    case 7: TextDrawSetString(AchTD[2], "~g~ACHIEVE~w~M~g~ENT UNLOCKED");
	    case 8: TextDrawSetString(AchTD[2], "~g~ACHIEVEM~w~E~g~NT UNLOCKED");
	    case 9: TextDrawSetString(AchTD[2], "~g~ACHIEVEME~w~N~g~T UNLOCKED");
	    case 10: TextDrawSetString(AchTD[2], "~g~ACHIEVEMEN~w~T~g~ UNLOCKED");
	    case 11: TextDrawSetString(AchTD[2], "~g~ACHIEVEMENT ~w~U~g~NLOCKED");
	    case 12: TextDrawSetString(AchTD[2], "~g~ACHIEVEMENT U~w~N~g~LOCKED");
	    case 13: TextDrawSetString(AchTD[2], "~g~ACHIEVEMENT UN~w~L~g~OCKED");
	    case 14: TextDrawSetString(AchTD[2], "~g~ACHIEVEMENT UNL~w~O~g~CKED");
	    case 15: TextDrawSetString(AchTD[2], "~g~ACHIEVEMENT UNLO~w~C~g~KED");
	    case 16: TextDrawSetString(AchTD[2], "~g~ACHIEVEMENT UNLOC~w~K~g~ED");
	    case 17: TextDrawSetString(AchTD[2], "~g~ACHIEVEMENT UNLOCK~w~E~g~D");
	    case 18: TextDrawSetString(AchTD[2], "~g~ACHIEVEMENT UNLOCKE~w~D");
	}
	if(var >= 18 && var != 1337) { var = 0; } else { var++; }
	if(var != 1337) { SetTimerEx("AnimateAchTD", 100, false, "d", var); }
	else { TextDrawSetString(AchTD[2], "~g~ACHIEVEMENT UNLOCKED");}
	return true;
}

forward OnPlayerAchieve(playerid, achid);

/* ~~~~  ~~~~~~~~~~~~~~~~~~ End of File || ENJOY! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
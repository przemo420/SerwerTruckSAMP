enum E_FADE {
	fadeSkin,
	Float:fadePos[3]
}
new fadePlayer[MAX_PLAYERS][E_FADE];

public OnFadeComplete(playerid,beforehold)
{
	if(beforehold)
	{
		if(fadePlayer[playerid][fadeSkin] != -1)
		{
			SetPlayerSkin(playerid, fadePlayer[playerid][fadeSkin]);
			fadePlayer[playerid][fadeSkin] = (-1);

			FadeColorForPlayer(playerid,0,0,0,255,0,0,0,0,15,0);
		}

		if(fadePlayer[playerid][fadePos][0] != 0.01 && fadePlayer[playerid][fadePos][1] != 0.01 && fadePlayer[playerid][fadePos][2] != 0.01)
		{
			SetPlayerPos(playerid, fadePlayer[playerid][fadePos][0], fadePlayer[playerid][fadePos][1], fadePlayer[playerid][fadePos][2]);
			
			fadePlayer[playerid][fadePos][0] = 0.01;
			fadePlayer[playerid][fadePos][1] = 0.01;
			fadePlayer[playerid][fadePos][2] = 0.01;

			FadeColorForPlayer(playerid,0,0,0,255,0,0,0,0,15,0);
		}
  	}
}

stock SetPlayerSkinEx(playerid, skinid)
{
	fadePlayer[playerid][fadeSkin] = skinid;
	StopPlayerFade(playerid);

	FadeColorForPlayer(playerid,0,0,0,0,0,0,0,255,15,0);
	return 1;
}

#if defined _ALS_SetPlayerSkin
	#undef SetPlayerSkin
#else 
	#define _ALS_SetPlayerSkin
#endif 

#define SetPlayerSkin SetPlayerSkinEx

stock SetPlayerPosEx(playerid, Float:fadePosX, Float:fadePosY, Float:fadePosZ)
{
	fadePlayer[playerid][fadePos][0] = fadePosX;
	fadePlayer[playerid][fadePos][1] = fadePosY;
	fadePlayer[playerid][fadePos][2] = fadePosZ;

	StopPlayerFade(playerid);

	return FadeColorForPlayer(playerid,0,0,0,0,0,0,0,255,15,0); // Zakryj ekran
}

#if defined _ALS_SetPlayerPos
	#undef SetPlayerPos
#else 
	#define _ALS_SetPlayerPos
#endif 

#define SetPlayerPos SetPlayerPosEx

forward FadePlayer_OnPlayerConnect(playerid);
public FadePlayer_OnPlayerConnect(playerid)
{
	fadePlayer[playerid][fadeSkin] = (-1);

	fadePlayer[playerid][fadePos][0] = 0.01;
	fadePlayer[playerid][fadePos][1] = 0.01;
	fadePlayer[playerid][fadePos][2] = 0.01;

	return 1;
}

forward FadePlayer_OnPlayerDeath(playerid);
public FadePlayer_OnPlayerDeath(playerid)
	return FadeColorForPlayer(playerid,0,0,0,0,0,0,0,255,15,0); // zakryj ekran

forward FadePlayer_OnPlayerSpawn(playerid);
public FadePlayer_OnPlayerSpawn(playerid)
	return FadeColorForPlayer(playerid,0,0,0,255,0,0,0,0,15,0); // odkryj ekran
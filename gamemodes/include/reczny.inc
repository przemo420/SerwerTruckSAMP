/*
	(c) 2015 - Part of SerwerTruck.eu
	@Commands:
		- /reczny

	Testowane na Linuxie, dzia³a w 100%. 
	PS Hook na SetVehiclePos musi byæ gdzieœ u samej góry GameModa, inaczej teleportacja pojzdu podczas zal¹czonego rêcznego jest nie mo¿liwa :P 
*/

enum E_BRAKE
{
	Float:PosX,
	Float:PosY,
	Float:PosZ,
	Float:PosA,
	Float:PosXTemp,
	Float:PosYTemp,
	Float:PosZTemp,
	Float:PosATemp,
	bool:exited,
	bool:InUse,
	bool:manualBrake
}

new brInfo[MAX_VEHICLES][E_BRAKE];

stock SetVehiclePosEx(vehicleid, Float:x, Float:y, Float:z)
{
	SetVehiclePos(vehicleid, x, y, z);
	if(brInfo[vehicleid][manualBrake] == true)
	{
		brInfo[vehicleid][manualBrake] = false;

		brInfo[vehicleid][PosX] = x;
		brInfo[vehicleid][PosY] = y;
		brInfo[vehicleid][PosZ] = z;

		SetTimerEx("FreezeVehicle", 500, false, "d", vehicleid);
	}
	return 1;
}

#if defined _ALS_SetVehiclePos
  #undef SetVehiclePos
#else 
#define _ALS_SetVehiclePos
#endif 

#define SetVehiclePos SetVehiclePosEx

forward FreezeVehicle(vehicleid);
public FreezeVehicle(vehicleid)
{
	brInfo[vehicleid][manualBrake] = true;
	return 1;
}

forward Reczny_OnGameModeInit();
public Reczny_OnGameModeInit()
{
	SetTimer("CarReseter", 1000, true);
	for(new i; i < MAX_VEHICLES; i++)
	{
		GetVehiclePos(i, brInfo[i][PosX], brInfo[i][PosY], brInfo[i][PosZ]);
		GetVehicleZAngle(i, brInfo[i][PosA]);
		brInfo[i][exited] = false;
		brInfo[i][manualBrake] = false;
	}
	return 1;
}

forward Reczny_OnVehicleSpawn(vehicleid);
public Reczny_OnVehicleSpawn(vehicleid)
{
	if(brInfo[vehicleid][manualBrake] == true)
	{
		GetVehiclePos(vehicleid, brInfo[vehicleid][PosX], brInfo[vehicleid][PosY], brInfo[vehicleid][PosZ]);
		GetVehicleZAngle(vehicleid, brInfo[vehicleid][PosA]);
		brInfo[vehicleid][exited] = false;
	}
	return 1;
}

forward Reczny_OnPlayerExitVehicle(playerid, vehicleid);
public Reczny_OnPlayerExitVehicle(playerid, vehicleid)
{
	if(brInfo[vehicleid][manualBrake] == true && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		brInfo[vehicleid][exited] = true;
		brInfo[vehicleid][InUse] = false;
	}
	return 1;
}

forward Reczny_OnPlayerStateChange(playerid, newstate, oldstate);
public Reczny_OnPlayerStateChange(playerid, newstate, oldstate)
{
	if(newstate == PLAYER_STATE_DRIVER && brInfo[GetPlayerVehicleID(playerid)][manualBrake] == true)
	{
		new vehicleid = GetPlayerVehicleID(playerid);
		brInfo[vehicleid][InUse] = true;
		brInfo[vehicleid][exited] = false;
	}
	return 1;
}

forward CarReseter();
public CarReseter()
{
	for(new i; i < MAX_VEHICLES; i++)
	{
		if(!IsValidVehicle(i))
			continue;
		if(!brInfo[i][manualBrake])
			continue;

		new Float:Xx, Float:Yy, Float:Zz,Float:Aa;
		GetVehiclePos(i, Xx, Yy, Zz);
		GetVehicleZAngle(i, Aa);
		if(brInfo[i][exited] == false)
		{
			if(brInfo[i][InUse] == false)
			{
				if(Xx != brInfo[i][PosX] || Yy != brInfo[i][PosY] || Zz != brInfo[i][PosZ])
				{
					SetVehiclePos(i, brInfo[i][PosX], brInfo[i][PosY], brInfo[i][PosZ]);
					SetVehicleZAngle(i, brInfo[i][PosA]);
				}
			}
		}
		else
		{
			GetVehiclePos(i, brInfo[i][PosXTemp], brInfo[i][PosYTemp], brInfo[i][PosZTemp]);
			GetVehicleZAngle(i, brInfo[i][PosATemp]);
			SetTimerEx("GetCarPos", 280, false, "i", i);
		}
	}
}

forward GetCarPos(vehicleid);
public GetCarPos(vehicleid)
{
	new Float:X, Float:Y, Float:Z,Float:A;
	GetVehiclePos(vehicleid, X, Y, Z);
	GetVehicleZAngle(vehicleid, A);
	
	if(brInfo[vehicleid][PosXTemp] == X && brInfo[vehicleid][PosYTemp] == Y && brInfo[vehicleid][PosZTemp] == Z)
	{
		brInfo[vehicleid][PosX] = X;
		brInfo[vehicleid][PosY] = Y;
		brInfo[vehicleid][PosZ] = Z;
		brInfo[vehicleid][PosA] = A;
		brInfo[vehicleid][exited] = false;
		return 1;
	}
	return 1;
}

CMD:reczny(playerid)
{
	if(!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie jesteœ w pojeŸdzie.");
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
		return Msg(playerid, COLOR_ERROR, "Nie jestes kierowc¹ tego pojazdu");
	if(!brInfo[GetPlayerVehicleID(playerid)][manualBrake])
	{
		GetVehiclePos(GetPlayerVehicleID(playerid), brInfo[GetPlayerVehicleID(playerid)][PosX], brInfo[GetPlayerVehicleID(playerid)][PosY], brInfo[GetPlayerVehicleID(playerid)][PosZ]);
		GetVehicleZAngle(GetPlayerVehicleID(playerid), brInfo[GetPlayerVehicleID(playerid)][PosA]);
		brInfo[GetPlayerVehicleID(playerid)][manualBrake] = true;
		Msg(playerid, COLOR_INFO, "Hamulec rêczny {b}zaci¹gniêty{/b}.");
	}
	else
	{
		brInfo[GetPlayerVehicleID(playerid)][manualBrake] = false;
		Msg(playerid, COLOR_INFO, "Hamulec rêczny {b}wyl¹czony{/b}.");
	}
	return 1;
}
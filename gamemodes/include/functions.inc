PlayerName(playerid)
{
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, MAX_PLAYER_NAME);
	return name;
}

GetDistanceFromMeToPoint(playerid, Float:X, Float:Y, Float:Z)
{
	new Float:pos[4];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	pos[3] = floatsqroot(floatpower(floatabs(floatsub(X, pos[0])), 2) + floatpower(floatabs(floatsub(Y, pos[1])), 2) + floatpower(floatabs(floatsub(Z, pos[2])), 2));
	return floatround(pos[3]);
}

LoopingAnim(playerid,animlib[],animname[], Float:Speed, looping, lockx, locky, lockz, lp)
{
	SetPVarInt(playerid, "UsingLoopingAnim", 1);
	return ApplyAnimation(playerid, animlib, animname, Speed, looping, lockx, locky, lockz, lp);
}

OnePlayAnim(playerid,animlib[],animname[], Float:Speed, looping, lockx, locky, lockz, lp)
	return ApplyAnimation(playerid, animlib, animname, Speed, looping, lockx, locky, lockz, lp);

StopLoopingAnim(playerid)
	return DeletePVar(playerid, "UsingLoopingAnim");

GetVehicleSpeed(vehicleid, &Float:speed)
{
	new Float:x,
		Float:y,
		Float:z;
	GetVehicleVelocity(vehicleid,x,y,z);
	speed = floatsqroot(x*x + y*y + z*z) * 169;
}

CheatKick(playerid, reason[])
{
	new string[200];
	format(string, sizeof string, "{C0C0C0}Gracz %s zostaæ {FFFFFF}wyrzucony {C0C0C0}z serwera z powodu {FFFFFF}%s.", PlayerName(playerid), reason);
	SendClientMessageToAll(0x0, string);
}

CheatBan(playerid, reason[])
{
	new string[256], Y, Mo, D, H, Mi, S, ip[16];
	getdate(Y, Mo, D);
	gettime(H, Mi, S);
	GetPlayerIp(playerid, ip, sizeof(ip));

	format(string, sizeof string, "{C0C0C0}Gracz %s zostaæ {FFFFFF}zbanowany {C0C0C0}z powodu {FFFFFF}%s.", PlayerName(playerid), reason);
	SendClientMessageToAll(0x0, string);
	format(string, sizeof string, "INSERT INTO `BANS` (`Name`, `Nameadmin`, `Hour`, `Minute`, `Day`, `Month, `Year`, `Reason`, `IP`) VALUES('%s', 'Serwer', '%02d', '%02d', '%02d', '%02d', '%02d', '%s', '%s')", PlayerName(playerid), H, Mi, D, Mo, Y, reason, ip);
	mysql_query(string);
}

GetDistancePlayerToPlayer(playerid, twoplayerid)
{
	new Float:dis, Float: x1, Float: y1, Float: z1, Float: x2, Float: y2, Float: z2;
	GetPlayerPos(playerid, x1, y1, z1);
	GetPlayerPos(twoplayerid, x2, y2, z2);
	dis = floatsqroot(floatpower(floatabs(floatsub(x2,x1)),2)+floatpower(floatabs(floatsub(y2,y1)),2)+floatpower(floatabs(floatsub(z2,z1)),2));
	return floatround(dis);
}

IsNumeric(const str[])
{
	for (new i = 0, j = strlen(str); i < j; i++)
   		if (str[i] > '9' || str[i] < '0') 
       		return 0;

	return 1;
}

ConvertSeconds(secs,&hours,&minutes,&seconds)
{
	if(secs<0) 
		return false;

	minutes = secs / 60;
	seconds = secs % 60;
	hours = minutes / 60;
	minutes = minutes % 60;
	return 1;
}

PlayerVehicleIsTruck(playerid)
{
	switch(GetVehicleModel(GetPlayerVehicleID(playerid)))
	{
		case 403: return 1;
        case 514: return 1;
        case 515: return 1;
        case 455: return 1;
        case 578: return 1;
        case 459: return 1;
        case 440: return 1;
		case 482: return 1;
		case 498: return 1;
	}

	return 0;
}

GetDistanceToNearestDriver(playerid)
{
	new Float:Distance;
	Distance = -1;
	new ReturnPlayerid;
	ReturnPlayerid = -1;

	Loop(player, MAX_PLAYERS)
	{
		if(IsPlayerConnected(player) && player != playerid && GetPlayerState(player) == PLAYER_STATE_DRIVER)
		{
			new Distance2 = GetDistancePlayerToPlayer(playerid, player);

			if(Distance > Distance2 || Distance == -1)
			{
				Distance = Distance2;
				ReturnPlayerid = player;
			}
		}
	}

	return ReturnPlayerid;
}

stock GetWeaponNameEx(weaponid)
{
	new weapon[32];
    switch(weaponid)
    {
        case 18: strcat(weapon, "Molotov Cocktail", 31);
        case 44: strcat(weapon, "Night Vision Goggles", 31);
        case 45:  strcat(weapon, "Thermal Goggles", 31);
        default: GetWeaponName(weaponid, weapon, 31);
    }

    return weapon;
}

IsPlayerInTruck(playerid)
{
	new vehicleid = GetVehicleModel(GetPlayerVehicleID(playerid));

	if(!IsPlayerInAnyVehicle(playerid))
		return 0;

	if(vehicleid == 515 || vehicleid == 514 || vehicleid == 403)
		return 1;

	return 0;
}

IsVehicleTruck(vehicleid)
{
	vehicleid = GetVehicleModel(vehicleid);

	if(vehicleid == 515 || vehicleid == 514 || vehicleid == 403)
		return 1;

	return 0;
}

IsPlayerInBus(playerid)
{
	new vehicleid = GetVehicleModel(GetPlayerVehicleID(playerid));

	if(!IsPlayerInAnyVehicle(playerid))
		return 0;

	if(vehicleid == 440 || vehicleid == 455 || vehicleid == 456 || vehicleid == 459 || vehicleid == 482 || vehicleid == 498 || vehicleid == 499 || vehicleid == 578 || vehicleid ==  609)
		return 1;

	return 0;
}

IsValidTrailerType(vehicleid, type)
{
	new trailerid = GetVehicleModel(vehicleid);
	
	if(type == TYPE_ILLEGAL)
		return 1;

	switch(trailerid)
	{
		case 450: 
			if(type == TYPE_BUILD)
				return 1;

		case 584:
			if(type == TYPE_DANGEROUS)
				return 1;

		case 435:
			if(type == TYPE_CLOTHES || type == TYPE_ELECTRONIC)
				return 1;

		case 591:
			if(type == TYPE_EAT)
				return 1;
	}

	return 0;
}

ToLog(ownerid, typelog, param1[], param2[]="")
{
	new string[300];

	mysql_real_escape_string(param1, param1);
	mysql_real_escape_string(param2, param2);

	switch(typelog)
	{
		case LOG_TYPE_CHAT: 
			format(string, sizeof(string), "INSERT INTO `st_logs_chat` (ownerid, type, text) VALUES (%d, '%s', '%s')", ownerid, param1, param2);
		
		case LOG_TYPE_COMMANDS: 
			format(string, sizeof(string), "INSERT INTO `st_logs_commands` (ownerid, command) VALUES (%d, '%s')", ownerid, param1);

		case LOG_TYPE_PLAYER:
			format(string, sizeof(string), "INSERT INTO `st_logs_player` (ownerid, text) VALUES (%d, '%s')", ownerid, param1);
	}

	mysql_query(string);
}

GetVehicleCargoID(vehicleid)
{
	new vehicleuid, cargoid;

	if(IsPlayerInTruck(playerid))
	{
		vehicleid = GetVehicleTrailer(vehicleid);

		if(!Spawned[vehicleid])
			vehicleUID = DBVehID[vehicleid];
	}
	else
	{
		if(!Spawned[vehicleid])
			vehicleUID = DBVehID[vehicleid];
	}

	if()
	{
		cargoid = vehOtherInfo[vehicleid][vCargoID]
		vehOtherInfo[vehicleid][vCargoWeight] = ladowany;
	}
	else
	{
		vehInfo[vehicleUID][vCargoID] = towar; 
		vehInfo[vehicleUID][vCargoWeight] = ladowany;
	}

	return (Spawned[vehicleid])cargoid;
}

GetVehicleCargoVeight(vehicleid)
{

}
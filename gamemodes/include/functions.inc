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
	return ApplyAnimation(playerid, animlib, animname, Speed, looping, lockx, locky, lockz, lp, 1);
}

OnePlayAnim(playerid,animlib[],animname[], Float:Speed, looping, lockx, locky, lockz, lp)
	return ApplyAnimation(playerid, animlib, animname, Speed, looping, lockx, locky, lockz, lp);

StopLoopingAnim(playerid)
	return DeletePVar(playerid, "UsingLoopingAnim");

PreloadAnimLib(playerid, animlib[])
	return ApplyAnimation(playerid,animlib,"null",0.0,0,0,0,0,0);

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
	format(string, sizeof string, "Gracz {b}%s{/b} zosta³ {b}wyrzucony{/b} z serwera z powodu {b}%s{/b}.", PlayerName(playerid), reason);
	MsgToAll(COLOR_ERROR, string);
}

CheatBan(playerid, reason[])
{
	new string[300], Y, Mo, D, H, Mi, S, ip[16];
	getdate(Y, Mo, D);
	gettime(H, Mi, S);
	GetPlayerIp(playerid, ip, sizeof(ip));

	format(string, sizeof string, "Gracz {b}%s{/b} zosta³ {b}zbanowany{/b} z powodu {b}%s{/b}.", PlayerName(playerid), reason);
	MsgToAll(COLOR_ERROR, string);
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

IsVehicleTruck(modelid)
{
	if(modelid == 515 || modelid == 514 || modelid == 403)
		return 1;

	return 0;
}

IsPlayerInTruck(playerid)
{
	if(!IsPlayerInAnyVehicle(playerid))
		return 0;
		
	return IsVehicleTruck(GetVehicleModel(GetPlayerVehicleID(playerid)));
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

stock Msg(playerid, colorid, text[], bool:playSound = true, bool:displayPrefix = true)
{
	new
		buffer[256],
		curColor,
		tmpColor[16],
		tmpColorCode;

	switch(colorid)
	{
		case COLOR_INFO:
		{
			curColor = HexToInt("82C412");
			tmpColorCode = HexToInt("BCEE68");
		}
		case COLOR_INFO2:
		{
			curColor = HexToInt("488CDB");
			tmpColorCode = HexToInt("A8C3E3");
		}
		case COLOR_INFO3:
		{
			curColor = HexToInt("FFAB00");
			tmpColorCode = HexToInt("EBBF65");
		}
		case COLOR_ERROR:
		{
			curColor = HexToInt("BA0B0B");
			tmpColorCode = HexToInt("D64F4F");
		}
		default: curColor = colorid;
	}

	copy(text, buffer);
	format(tmpColor, sizeof tmpColor, "{%06x}", tmpColorCode);

	new f = strfind( buffer, "{b}" );
	while ( f >= 0)
	{
		strdel(buffer, f, f + 3);
		strins( buffer, tmpColor, f);
		f = strfind(buffer, "{b}");
	}

	format(tmpColor, sizeof tmpColor, "{%06x}", curColor);

	f = strfind( buffer, "{/b}" );
	while ( f >= 0)
	{
		strdel(buffer, f, f + 4);
		strins(buffer, tmpColor, f);
		f = strfind(buffer, "{/b}");
	}

	if(displayPrefix)
		strins(buffer, "??? ", 0);

	SendClientMessage(playerid, curColor * 256, buffer);

	if(playSound)
		if(colorid == COLOR_ERROR)
			PlaySound(playerid, 1085);
		else
			PlaySound(playerid, 1150);
	return 1;
}

stock MsgToAll(colorid, text[], bool:displayPrefix = true)
{
	new
		buffer[256],
		curColor,
		tmpColor[16],
		tmpColorCode;

	switch(colorid)
	{
		case COLOR_INFO: // zielony
		{
			curColor = HexToInt("82C412");
			tmpColorCode = HexToInt("BCEE68");
		}
		case COLOR_INFO2: // niebieski
		{
			curColor = HexToInt("488CDB");
			tmpColorCode = HexToInt("A8C3E3");
		}
		case COLOR_INFO3: // pomara?zowy
		{
			curColor = HexToInt("FFAB00");
			tmpColorCode = HexToInt("EBBF65");
		}
		case COLOR_ERROR: // czerwony
		{
			curColor = HexToInt("BA0B0B");
			tmpColorCode = HexToInt("D64F4F");
		}
		default: curColor = colorid;
	}

	copy(text, buffer);
	format(tmpColor, sizeof tmpColor, "{%06x}", tmpColorCode);

	new f = strfind( buffer, "{b}" );
	while ( f >= 0)
	{
		strdel(buffer, f, f + 3);
		strins( buffer, tmpColor, f);
		f = strfind(buffer, "{b}");
	}

	format(tmpColor, sizeof tmpColor, "{%06x}", curColor);

	f = strfind( buffer, "{/b}" );
	while ( f >= 0)
	{
		strdel(buffer, f, f + 4);
		strins(buffer, tmpColor, f);
		f = strfind(buffer, "{/b}");
	}

	if(displayPrefix)
		strins(buffer, "››› ", 0);

	SendClientMessageToAll(curColor * 256, buffer);

	return 1;
}

stock copy(string1[], string2[])
{
	for(new i = 0; i < strlen(string1); i++)
		string2[i] = string1[i];

	string2[strlen(string1)] = EOS;
}

stock PlaySound(playerID, soundID)
{
	new Float:posX;
	new Float:posY;
	new Float:posZ;
	GetPlayerPos(playerID, posX, posY, posZ);
	PlayerPlaySound(playerID, soundID, posX, posY, posZ);
}

stock HexToInt(string[])
{
	if(string[0] == 0) return 0;
	new
		cur = 1,
		res = 0;

	for(new i = strlen(string); i > 0; i--)
	{
		if(string[i - 1] < 58)
			 res = res + cur * (string[i - 1] - 48);
		else
			res = res + cur * (string[i - 1] - 65 + 10);
		cur = cur * 16;
	}
	return res;
}

stock GetVehicleModelName(id)
{
	new szString[32];
	format(szString, sizeof szString, "%s", VehicleNames[id - 400]);
	return szString;
}

stock GetModelIdByName(name[])
{
	for(new i = 0; i < 211; i++)
	{
		if(strfind(VehicleNames[i], name, true) != -1)
			return i + 400;
	}
	return -1;
}

stock delstr(src[], const sub[], idx = 0)
{
	new
		f = strfind(src, sub, false, idx),
		srclen = strlen(src),
		sublen = strlen(sub),
		count;
 
	if(!srclen || !sublen)
		return 0;
 
	if(idx >= sublen)
		return 0;
 
	while(f >= 0)
	{
		count++;
		strdel(src, f + idx, f + sublen);
		f = strfind(src, sub, false, idx);
	}
	return count;
}

stock GetIDByNick(nick[])
{
	new result = -1,
		foundCount = 0;

	if(IsNumeric(nick))
	{
		result = strval(nick);
		if(!IsPlayerConnected(result)) return -1;
	}
	else
	{
		Loop(i, GetMaxPlayers())
		{
			if(FindString(PlayerName(i), nick))
			{
				foundCount++;
				result = i;
			}
		}
		if(foundCount < 1) return -1;
		else if(foundCount > 1) return -1;
	}

	return result;
}

stock FindString(string1[], string2[], &start = 0, &end = 0)
{
	if(!strlen(string1) || !strlen(string2)) return false;

	new
		result;

	for(new i = 0; i < strlen(string1) - strlen(string2) + 1; i++)
	{
		result = 0;

		for(new g = 0; g < strlen(string2); g++)
		{
			if(toupper(string1[i + g]) == toupper(string2[g])) result++;
		}

		if(result == strlen(string2))
		{
			start = i;
			end = i + strlen(string2);
			return true;
		}
	}
	return false;
}

GetNearestVehicle(playerid, Float:Distance = 1000.0)
{
	Distance = floatabs(Distance);

	if(Distance == 0.0) 
		Distance = 1000.0;
	new Float:X[2], Float:Y[2], Float:Z[2];
	new Float:NearestPos = Distance;
	new NearestVehicle = INVALID_VEHICLE_ID;
	GetPlayerPos(playerid, X[0], Y[0], Z[0]);

	for(new i; i<GetVehiclePoolSize(); i++)
	{
		if(!IsVehicleStreamedIn(i, playerid) || i == GetPlayerVehicleID(playerid)) 
			continue;

		GetVehiclePos(i, X[1], Y[1], Z[1]);

		if(NearestPos > GetDistanceBetweenPoints(X[0], Y[0], Z[0], X[1], Y[1], Z[1])) 
			NearestPos = GetDistanceBetweenPoints(X[0], Y[0], Z[0], X[1], Y[1], Z[1]), NearestVehicle = i;
	}

	if(NearestPos < Distance) 
		return NearestVehicle;

	return INVALID_VEHICLE_ID;
}

IsPlayerFacingVehicle(playerid,vehicleid)
{
	new Float:zX,Float:zY,Float:zZ,Float:X,Float:Y,Float:Z,Float:ang;

	if(!IsPlayerConnected(playerid)) 
		return 0;

	GetVehiclePos(vehicleid, X, Y, Z);
	GetPlayerPos(playerid, zX, zY, zZ);

	if( Y > zY ) 
		ang = (-acos((X - zX) / floatsqroot((X - zX)*(X - zX) + (Y - zY)*(Y - zY))) - 90.0);
	else if( Y < zY && X < zX ) 
		ang = (acos((X - zX) / floatsqroot((X - zX)*(X - zX) + (Y - zY)*(Y - zY))) - 450.0);
	else if( Y < zY ) 
		ang = (acos((X - zX) / floatsqroot((X - zX)*(X - zX) + (Y - zY)*(Y - zY))) - 90.0);

	if(X > zX) 
		ang = (floatabs(floatabs(ang) + 180.0));
	else 
		ang = (floatabs(ang) - 180.0);

	new Float:russia;
	GetPlayerFacingAngle(playerid,russia);

	if(ang-russia<-130 || ang-russia>130) 
		return 0;

	return 1;
}
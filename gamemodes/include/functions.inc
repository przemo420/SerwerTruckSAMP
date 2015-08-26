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

IsWorked(playerid, fraction)
{
	if(GetPVarInt(playerid, "Worked") && firmInfo[playerInfo[playerid][pFirm]][tType] == fraction)
		return true;
	return false;
}

IsFractionBoss(playerid)
{
	if(firmInfo[playerInfo[playerid][pFirm]][tChef] != playerInfo[playerid][pID])
		return false;
	return true;
}

IsFractionViceBoss(playerid)
{
	if(firmInfo[playerInfo[playerid][pFirm]][tVcChef] != playerInfo[playerid][pID])
		return false;
	return true;
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
		strins(buffer, "››› ", 0);

	SendClientMessage(playerid, curColor * 256, buffer);

	if(playSound)
		if(colorid == COLOR_ERROR)
			PlaySound(playerid, 1085);
		else
			PlaySound(playerid, 1150);
	return 1;
}

stock MsgTo(colorid, text[], teamtypeid)
{
	for(new i; i <= GetPlayerPoolSize(); i++)
		if(IsPlayerConnected(i))
			if(firmInfo[playerInfo[i][pFirm]][tType] == teamtypeid)
				Msg(i, colorid, text);

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

HasPlayerFullPermission(playerid)
{
	new str[32];
	format(str, sizeof(str), "full_permission/%s", PlayerName(playerid));
	
	if(djInt("config.json", str))
		return 1;

	return 0;
}

GetPlayerSpeed(playerid)
{
	new Float:ST[ 4 ];
	GetPlayerVelocity(playerid, ST[ 0 ], ST[ 1 ], ST[ 2 ]);
	ST[3] = floatsqroot(floatpower(floatabs(ST[ 0 ]), 2.0) + floatpower(floatabs(ST[ 1 ]), 2.0) + floatpower(floatabs(ST[ 2 ]), 2.0)) * 169;
	return floatround(ST[ 3 ])/2;
}

MaxWeight(vehicletype)
{
	switch(vehicletype)
	{
  		case 403: return 24;
        case 514: return 24;
        case 515: return 24;
        case 455: return 13;
        case 578: return 13;
        case 459: return 8;
        case 440: return 8;
		case 482: return 8;
		case 498: return 8;
		case 435: return 24;
		case 450: return 24;
		case 584: return 24;
		case 591: return 24;
	}
	return 0;
}

ConvertMS(Miliseconds, &hours, &minutes, &seconds)
{
   hours = Miliseconds / 3600000;
   Miliseconds -= hours * 3600000;
   minutes = Miliseconds / 60000;
   Miliseconds -= minutes * 60000;
   seconds = Miliseconds / 1000;
   Miliseconds -= seconds * 1000;
}

GetFreeRemontBTID()
{
	new obiektid = INVALID_REMONTBT3D_ID, counter = 0;
	while(obiektid == INVALID_REMONTBT3D_ID)
	{
	    counter++;
	    if(BTNapis[counter][btnNapis] == Text3D:INVALID_REMONTBT3D_ID)
		 	obiektid = counter;
	}
	return obiektid;
}

GetMoveDirectionFromKeys(ud, lr)
{
	new direction = 0;

    if(lr < 0)
	{
		if(ud < 0) 		direction = MOVE_FORWARD_LEFT; 	// Up & Left key pressed
		else if(ud > 0) direction = MOVE_BACK_LEFT; 	// Back & Left key pressed
		else            direction = MOVE_LEFT;          // Left key pressed
	}
	else if(lr > 0) 	// Right pressed
	{
		if(ud < 0)      direction = MOVE_FORWARD_RIGHT;  // Up & Right key pressed
		else if(ud > 0) direction = MOVE_BACK_RIGHT;     // Back & Right key pressed
		else			direction = MOVE_RIGHT;          // Right key pressed
	}
	else if(ud < 0) 	direction = MOVE_FORWARD; 	// Up key pressed
	else if(ud > 0) 	direction = MOVE_BACK;		// Down key pressed

	return direction;
}

//--------------------------------------------------

MoveCamera(playerid)
{
	new Float:FV[3], Float:CP[3];
	GetPlayerCameraPos(playerid, CP[0], CP[1], CP[2]);          // 	Cameras position in space
    GetPlayerCameraFrontVector(playerid, FV[0], FV[1], FV[2]);  //  Where the camera is looking at

	// Increases the acceleration multiplier the longer the key is held
	if(noclipdata[playerid][accelmul] <= 1) noclipdata[playerid][accelmul] += ACCEL_RATE;

	// Determine the speed to move the camera based on the acceleration multiplier
	new Float:speed = MOVE_SPEED * noclipdata[playerid][accelmul];

	// Calculate the cameras next position based on their current position and the direction their camera is facing
	new Float:X, Float:Y, Float:Z;
	GetNextCameraPosition(noclipdata[playerid][mode], CP, FV, X, Y, Z);
	MovePlayerObject(playerid, noclipdata[playerid][flyobject], X, Y, Z, speed);

	// Store the last time the camera was moved as now
	noclipdata[playerid][lastmove] = GetTickCount();
	return 1;
}

//--------------------------------------------------

GetNextCameraPosition(move_mode, Float:CP[3], Float:FV[3], &Float:X, &Float:Y, &Float:Z)
{
    // Calculate the cameras next position based on their current position and the direction their camera is facing
    #define OFFSET_X (FV[0]*6000.0)
	#define OFFSET_Y (FV[1]*6000.0)
	#define OFFSET_Z (FV[2]*6000.0)
	switch(move_mode)
	{
		case MOVE_FORWARD:
		{
			X = CP[0]+OFFSET_X;
			Y = CP[1]+OFFSET_Y;
			Z = CP[2]+OFFSET_Z;
		}
		case MOVE_BACK:
		{
			X = CP[0]-OFFSET_X;
			Y = CP[1]-OFFSET_Y;
			Z = CP[2]-OFFSET_Z;
		}
		case MOVE_LEFT:
		{
			X = CP[0]-OFFSET_Y;
			Y = CP[1]+OFFSET_X;
			Z = CP[2];
		}
		case MOVE_RIGHT:
		{
			X = CP[0]+OFFSET_Y;
			Y = CP[1]-OFFSET_X;
			Z = CP[2];
		}
		case MOVE_BACK_LEFT:
		{
			X = CP[0]+(-OFFSET_X - OFFSET_Y);
 			Y = CP[1]+(-OFFSET_Y + OFFSET_X);
		 	Z = CP[2]-OFFSET_Z;
		}
		case MOVE_BACK_RIGHT:
		{
			X = CP[0]+(-OFFSET_X + OFFSET_Y);
 			Y = CP[1]+(-OFFSET_Y - OFFSET_X);
		 	Z = CP[2]-OFFSET_Z;
		}
		case MOVE_FORWARD_LEFT:
		{
			X = CP[0]+(OFFSET_X  - OFFSET_Y);
			Y = CP[1]+(OFFSET_Y  + OFFSET_X);
			Z = CP[2]+OFFSET_Z;
		}
		case MOVE_FORWARD_RIGHT:
		{
			X = CP[0]+(OFFSET_X  + OFFSET_Y);
			Y = CP[1]+(OFFSET_Y  - OFFSET_X);
			Z = CP[2]+OFFSET_Z;
		}
	}
}

CancelFlyMode(playerid)
{
	CancelEdit(playerid);
	TogglePlayerSpectating(playerid, false);

	DestroyPlayerObject(playerid, noclipdata[playerid][flyobject]);
	noclipdata[playerid][cameramode] = CAMERA_MODE_NONE;
	return 1;
}

FlyMode(playerid)
{
	// Create an invisible object for the players camera to be attached to
	new Float:X, Float:Y, Float:Z;
	GetPlayerPos(playerid, X, Y, Z);
	noclipdata[playerid][flyobject] = CreatePlayerObject(playerid, 19300, X, Y, Z, 0.0, 0.0, 0.0);

	// Place the player in spectating mode so objects will be streamed based on camera location
	TogglePlayerSpectating(playerid, true);
	// Attach the players camera to the created object
	AttachCameraToPlayerObject(playerid, noclipdata[playerid][flyobject]);

	SetPVarInt(playerid, "FlyMode", 1);
	noclipdata[playerid][cameramode] = CAMERA_MODE_FLY;
	return 1;
}

stock IsValidBarInterior(interiorid)
{
	switch(interiorid)
	{
	    case 1, 4, 5, 6, 9, 10, 17: return true;
	}
	return false;
}

GetPosBarInterior(interior, &Float:X, &Float:Y, &Float:Z)
{
	switch(interior)
	{
	    case 1:
	    {
	        X = -794.806396;
			Y = 497.738037;
			Z = 1376.195312;
	    }
	    case 4:
	    {
	        X = -27.312299;
			Y = -29.277599;
			Z = 1003.557250;
	    }
	    case 5:
	    {
	        X = 372.3370;
			Y = -132.7720;
			Z = 1001.4922;
	    }
	    case 6:
	    {
	        X = -26.691598;
			Y = -55.714897;
			Z = 1003.546875;
	    }
	    case 9:
	    {
	        X = 365.0424;
			Y = -11.1190;
			Z = 1001.8516;
	    }
	    case 10:
	    {
	        X = 6.091179;
			Y = -29.271898;
			Z = 1003.549438;
	    }
	    case 17:
	    {
	        X = 377.2009;
			Y = -192.7142;
			Z = 1000.6401;
	    }
	}
	return 1;
}

GetBarInterior(barid)
{
	new query[128];
	format(query, sizeof query, "SELECT Int FROM `BARY` WHERE `ID` = '%d'", barid);
	new DBResult:result;// = db_query(Cargos, query);
	if(!db_num_rows(result))
	{
	    db_free_result(result);
	    return 0;
	}
	new interiorStr[3];
	db_get_field(result, 0, interiorStr, sizeof interiorStr);
	db_free_result(result);
	new interior = strval(interiorStr);
	return interior;
}

GetBarPos(barid, &Float:X, &Float:Y, &Float:Z)
{
    new query[128];
	format(query, sizeof query, "SELECT PosX, PosY, PosZ FROM `BARY` WHERE `ID` = '%d'", barid);
	new DBResult:result;// = db_query(Cargos, query);
	if(!db_num_rows(result))
	{
	    db_free_result(result);
	    return 0;
	}

	new pos[3][20];
	db_get_field(result, 0, pos[0], 20);
	db_get_field(result, 1, pos[1], 20);
    db_get_field(result, 2, pos[2], 20);
	db_free_result(result);

	X = floatstr(pos[0]);
	Y = floatstr(pos[1]);
	Z = floatstr(pos[2]);
	return 1;
}

Float:GetHeadingAngle(Float:targetX, Float:targetY, Float:entityX, Float:entityY)
{
	new
		Float:vectorX = targetX - entityX,
		Float:vectorY = targetY - entityY,
		Float:HeadingAngle;

	HeadingAngle = atan(-(vectorX/vectorY));

	if(vectorY < 0)
		HeadingAngle = (HeadingAngle >= 180) ? HeadingAngle - 180.0 : HeadingAngle + 180.0;

	return HeadingAngle;
}

SetVehicleSpeed(vehicleid, Float:speed)
{
	new
        Float:x,
        Float:y,
        Float:z,
        Float:angle;

	GetVehicleVelocity(vehicleid, x, y, z);
	if(x == 0.0 && y == 0.0 && z == 0.0)
	{
		GetVehicleZAngle(vehicleid, angle);
	}
	else
	{
		angle = Float:GetHeadingAngle(x, y, 0.0, 0.0);
	}

    speed = speed/169;
    x = speed * floatsin(-angle, degrees);
    y = speed * floatcos(-angle, degrees);
    SetVehicleVelocity(vehicleid, x, y, z);
}

forward Float:GetDistanceBetweenPoints(Float:X, Float:Y, Float:Z, Float:PointX, Float:PointY, Float:PointZ);
public Float:GetDistanceBetweenPoints(Float:X, Float:Y, Float:Z, Float:PointX, Float:PointY, Float:PointZ)
	return floatabs(floatsub(X, PointX)) + floatabs(floatsub(Y, PointY)) + floatabs(floatsub(Z, PointZ));

restart(text[])
{
if(strfind(text,"s%s%s%",true)!=-1
|| strfind(text,"%s%s",true)!=-1
|| strfind(text,"%s",true)!=-1
|| strfind(text,"%s%s%s%s",true)!=-1
|| strfind(text,"s%s%s%s%s%",true)!=-1
|| strfind(text,"s%s%s%s%s%s%",true)!=-1
|| strfind(text,"s%s%s%s%s%s%s%",true)!=-1
|| strfind(text,"s%s%s%s%s%s%s%s%",true)!=-1
|| strfind(text,"s%s%s%s%s%s%s%s%s%",true)!=-1
|| strfind(text,"s%s%s%s%s%s%s%s%s%s%",true)!=-1
|| strfind(text,"s%s%s%s%s%s%s%s%s%s%s%",true)!=-1
|| strfind(text,"s%s%s%s%s%s%s%s%s%s%s%s%",true)!=-1
|| strfind(text,"s%s%s%s%s%s%s%s%s%s%s%s%s%",true)!=-1
|| strfind(text,"s%s%s%s%s%s%s%s%s%s%s%s%s%s%",true)!=-1
|| strfind(text,"s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%",true)!=-1
|| strfind(text,"s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%",true)!=-1
|| strfind(text,"s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%",true)!=-1
|| strfind(text,"s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%",true)!=-1
|| strfind(text,"s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%",true)!=-1
|| strfind(text,"s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%",true)!=-1
|| strfind(text,"s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%",true)!=-1
|| strfind(text,"s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%",true)!=-1
|| strfind(text,"s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%",true)!=-1
|| strfind(text,"s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%",true)!=-1
|| strfind(text,"s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%",true)!=-1
|| strfind(text,"s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%",true)!=-1
|| strfind(text,"%%%",true)!=-1
|| strfind(text,"%%",true)!=-1
|| strfind(text,"%",true)!=-1
|| strfind(text,"%%%%",true)!=-1
|| strfind(text,"%%%%%",true)!=-1
|| strfind(text,"%%%%%%",true)!=-1
|| strfind(text,"%%%%%%%",true)!=-1
|| strfind(text,"%%%%%%%%",true)!=-1
|| strfind(text,"%%%%%%%%%",true)!=-1
|| strfind(text,"%%%%%%%%%%",true)!=-1
|| strfind(text,"%%%%%%%%%%%",true)!=-1
|| strfind(text,"%%%%%%%%%%%%",true)!=-1
|| strfind(text,"%%%%%%%%%%%%%",true)!=-1
|| strfind(text,"%%%%%%%%%%%%%%",true)!=-1
|| strfind(text,"%%%%%%%%%%%%%%%",true)!=-1
|| strfind(text,"%%%%%%%%%%%%%%%%",true)!=-1
|| strfind(text,"%%%%%%%%%%%%%%%%%",true)!=-1
|| strfind(text,"%%%%%%%%%%%%%%%%%%",true)!=-1
|| strfind(text,"%%%%%%%%%%%%%%%%%%%",true)!=-1
|| strfind(text,"%%%%%%%%%%%%%%%%%%%%",true)!=-1
|| strfind(text,"%%%%%%%%%%%%%%%%%%%%%",true)!=-1
|| strfind(text,"%%%%%%%%%%%%%%%%%%%%%%",true)!=-1
|| strfind(text,"%%%%%%%%%%%%%%%%%%%%%%%",true)!=-1
|| strfind(text,"%%%%%%%%%%%%%%%%%%%%%%%%",true)!=-1
|| strfind(text,"%%%%%%%%%%%%%%%%%%%%%%%%%",true)!=-1
|| strfind(text,"%%%%%%%%%%%%%%%%%%%%%%%%%%",true)!=-1)
return 1;
return 0;
}

MaxFuel(vehicletype)
{
switch(vehicletype)
{
case 403: return 1000;
case 514: return 800;
case 515: return 750;
case 455: return 500;
case 578: return 380;
case 459: return 150;
case 440: return 190;
case 602: return 75;
case 518: return 80;
case 496: return 55;
case 581: return 30;
case 536: return 70;
case 589: return 45;
case 542: return 60;
case 480: return 80;
case 419: return 80;
case 565: return 45;
case 463: return 30;
case 533: return 60;
case 462: return 10;
case 559: return 65;
case 426: return 70;
case 603: return 75;
case 461: return 30;
case 475: return 70;
case 535: return 75;
case 405: return 65;
case 439: return 55;
case 468: return 25;
case 558: return 65;
case 412: return 75;
case 586: return 30;
case 477: return 65;
case 521: return 25;
case 587: return 60;
case 562: return 60;
case 401: return 50;
case 527: return 50;
case 507: return 70;
case 526: return 45;
case 551: return 75;
case 410: return 40;
case 600: return 60;
case 436: return 45;
case 445: return 55;
case 575: return 50;
case 422: return 50;
case 466: return 65;
case 579: return 80;
case 545: return 45;
case 474: return 60;
case 508: return 100;
case 400: return 75;
case 500: return 65;
case 467: return 75;
case 534: return 70;
case 479: return 55;
case 489: return 90;
case 495: return 85;
case 567: return 70;
case 580: return 75;
case 543: return 60;
case 576: return 75;
case 421: return 65;
case 478: return 60;
case 554: return 90;
case 547: return 60;
case 561: return 60;
case 458: return 60;
case 550: return 60;
case 560: return 75;
case 549: return 70;
case 566: return 65;
case 540: return 55;
case 555: return 55;
case 529: return 60;
}
return 50;
}

stock IsValidInterior(interiorid)
{
	switch(interiorid)
	{
	    case 1..3, 5, 7, 8, 10, 12: return true;
	}
	return false;
}

stock GetInteriorInsidePos(interior, &Float:X, &Float:Y, &Float:Z)
{
	switch(interior)
	{
	    case 1:
	    {
	        X = 223.1425;
	        Y = 1287.2781;
	        Z = 1082.1406;
	    }
	    case 2:
	    {
	        X = 226.005401;
	        Y = 1240.039062;
	        Z = 1082.140625;
	    }
	    case 3:
	    {
	        X = 235.3299;
	        Y = 1187.1647;
	        Z = 1080.2578;
	    }
	    case 5:
	    {
     		X = 318.6448;
	        Y = 1114.4795;
	        Z = 1083.8828;
	    }
	    case 7:
	    {
	        X = 225.7482;
	        Y = 1022.0986;
	        Z = 1084.0159;
	    }
	    case 8:
	    {
	        X = 2365.3176;
	        Y = -1134.9512;
	        Z = 1050.8750;
	    }
	    case 10:
	    {
	        X = 2269.7822;
	        Y = -1210.4670;
	        Z = 1047.5625;
	    }
	    case 12:
	    {
 	        X = 2324.419921;
	        Y = -1145.568359;
	        Z = 1050.710083;
	    }
	}
	return 1;
}

ColouredText(text[])
{
    enum colorEnum
    {
    	colorName[16],
     	colorID[7]
    };
    new	colorInfo[][colorEnum] =
    {
    	{ "BLUE",           "1B1BE0" },
     	{ "PINK",           "E81CC9" },
      	{ "YELLOW",         "DBED15" },
       	{ "LIGHTGREEN",     "8CED15" },
        { "LIGHTBLUE",      "15D4ED" },
        { "RED",            "FF0000" },
        { "GREY",           "BABABA" },
        { "WHITE",          "FFFFFF" },
        { "ORANGE",         "DB881A" },
        { "GREEN",          "37DB45" },
        { "PURPLE",         "7340DB" }
        },
        string[(128 + 32)],
        tempString[16],
        pos = -1,
        x;
    strmid(string, text, 0, 128, sizeof(string));

    for( ; x != sizeof(colorInfo); ++x)
    {
        format(tempString, sizeof(tempString), "#%s", colorInfo[x][colorName]);

        while((pos = strfind(string, tempString, true, (pos + 1))) != -1)
        {
            new
                tempLen = strlen(tempString),
                tempVar,
                i = pos
            ;
            format(tempString, sizeof(tempString), "{%s}", colorInfo[x][colorID]);

            if(tempLen < 8)
            {
                for(new j; j != (8 - tempLen); ++j)
                {
                    strins(string, " ", pos);
                }
            }
            for( ; ((string[i] != 0) && (tempVar != 8)) ; ++i, ++tempVar)
            {
                string[i] = tempString[tempVar];
            }
            if(tempLen > 8)
            {
                strdel(string, i, (i + (tempLen - 8)));
            }
            x = -1;
        }
    }
    return string;
}

GetStacja(playerid)
{
	new Float:playerPos[3], stacjaid=(-1);
	GetPlayerPos(playerid, playerPos[0], playerPos[1], playerPos[2]);

	for(new i; i<MAX_STACJE; i++)
	{
	    if(stationInfo[i][sID])
			if(IsPlayerInRangeOfPoint(playerid, 10.0, stationInfo[i][sPosX], stationInfo[i][sPosY], stationInfo[i][sPosZ]))
			{
			    stacjaid = i;
			    break;
			}
	}

	return stacjaid;
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

stock clText(colorid, text[])
{
	new
		buffer[400],
		curColor,
		tmpColor[16],
		tmpColorCode;

	switch(colorid)
	{
		case COLOR_INFO:
		{
			curColor = HexToInt("FFFFFF");
			tmpColorCode = HexToInt("82C412");
		}
		case COLOR_INFO2:
		{
			curColor = HexToInt("FFFFFF");
			tmpColorCode = HexToInt("488CDB");
		}
		case COLOR_INFO3:
		{
			curColor = HexToInt("FFFFFF");
			tmpColorCode = HexToInt("FFAB00");
		}
		case COLOR_ERROR:
		{
			curColor = HexToInt("FFFFFF");
			tmpColorCode = HexToInt("BA0B0B");
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
	return buffer;
}

stock GetPlayerLevelName(level)
{
	new name[32];
	switch(level)
	{
		case 0: {name="Unranked haha :siara:"; }
		case 1 .. 4: {name = "Nowicjusz";}
		case 5 .. 9: {name = "Entuzjasta";}
		case 10 .. 14: {name = "Utalentowany pracownik";}
		case 15 .. 19: {name = "Profesjonalista";}
		case 20 .. 24: {name = "Mistrz";}
		case 25 .. 29: {name = "Instruktor";}
		case 30 .. 34: {name = "Elita";}
		case 35 .. 39: {name = "Król szos";}
		default: {name = "Legenda";}
	}
	return name;
}

stock GetPlayerLevel(exp)
{
    static levels[] = {
        (-1), 0, 10, 20,
		35, 45, 60, 80,
		120, 150, 195, 240,
		287, 355, 410, 476,
		524, 588, 680, 735,
		845, 940, 1120, 1300,
		1510, 1588, 1630, 1672,
		1742, 2000, 2450, 2725,
		3010, 3550, 3900, 4450
    };
    new level = 0;
    for (new i = 1;
        i < sizeof(levels) && exp > levels[i];
        i++, level++
    ) {}
    return level;
}
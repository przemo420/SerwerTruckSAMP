enum E_FIRES
{
	ffId,
	Float:ffX,
	Float:ffY,
	Float:ffZ,
	Float:ffHealth,
	ffObject,
	ffSmoke,
	Text3D:ffText,
	bool:ffUse
}

enum E_STABLE_FIRES
{
	ffId,
	Float:ffX, 
	Float:ffY,
	Float:ffZ
}

#define MAX_FIRES 512

new sfInfo[MAX_FIRES][E_STABLE_FIRES];
new ffInfo[MAX_FIRES][E_FIRES];

stock IsValidAnyFire()
{
	for(new i = 0; i < MAX_ACTUAL_FIRES; i++)
	{
		if(gmInfo[actFires][i])
			return true;
	}
	return false;
}

stock OnFireUpdate(playerid)
{
	new newkeys, u, l;
	GetPlayerKeys(playerid, newkeys, u, l);
	if(HOLDING(KEY_FIRE) && (IsValidAnyFire() != 0))
	{
		new fireid = GetClosestFire(playerid);
		if(GetPlayerWeapon(playerid) == 42)
		{
			if(IsPlayerInRangeOfPoint(playerid, 6.0, ffInfo[fireid][ffX], ffInfo[fireid][ffY], ffInfo[fireid][ffZ]))
			{
				if(PlayerFaces(playerid, ffInfo[fireid][ffX], ffInfo[fireid][ffY], ffInfo[fireid][ffZ], 1.0))
				{
					new szString[128];
					ffInfo[fireid][ffHealth] -= float(random(5));
					if(ffInfo[fireid][ffHealth] <= 0.0)
					{
						DestroyFire(fireid);
						format(szString, sizeof szString, "Stra¿ak {b}%s{b} ugasi³ po¿ar w {b}%s{b}!", PlayerName(playerid), GetPlayerZone(playerid));
						MsgToAll(COLOR_INFO2, szString);
						return 1;
					}
					format(szString, sizeof szString, "%d/100", floatround(ffInfo[fireid][ffHealth]));
					UpdateDynamic3DTextLabelText(ffInfo[fireid][ffText], -1, szString);
				}
			}
		}
		else if(IsInWaterCar(playerid))
		{
			if(IsPlayerInRangeOfPoint(playerid, 10.0, ffInfo[fireid][ffX], ffInfo[fireid][ffY], ffInfo[fireid][ffZ]))
			{
				if(IsPlayerLookingAtPoint(playerid, ffInfo[fireid][ffX], ffInfo[fireid][ffY], ffInfo[fireid][ffZ]))
				{
					new szString[128];
					ffInfo[fireid][ffHealth] -= float(random(5));
					if(ffInfo[fireid][ffHealth] <= 0.0)
					{
						DestroyFire(fireid);
						format(szString, sizeof szString, "Stra¿ak {b}%s{b} ugasi³ po¿ar w {b}%s{b}!", PlayerName(playerid), GetPlayerZone(playerid));
						return 1;
					}
					format(szString, sizeof szString, "%d/100", floatround(ffInfo[fireid][ffHealth]));
					UpdateDynamic3DTextLabelText(ffInfo[fireid][ffText], -1, szString);
				}
			}
		}
	}
	return 1;
}

stock LoadFireSystem()
{
	new szBuff[64], count;
	mysql_query("SELECT * FROM fires");
	mysql_store_result();
	new rows = mysql_num_rows();
	for(new i = 0; i < rows; i++)
	{
		if(mysql_fetch_row(szBuff, "|") && i < MAX_FIRES)
		{
			count++;
			sscanf(szBuff, "p<|>dfff", sfInfo[i][ffId], sfInfo[i][ffX], sfInfo[i][ffY], sfInfo[i][ffZ]);
		}
	}
	printf("Za³adowano %d po¿arów.", count);
	return 1;
}

stock IsValidFire(fireid)
{
	if(fireid > MAX_FIRES)
		return false;

	if((ffInfo[fireid][ffX] + ffInfo[fireid][ffY] + ffInfo[fireid][ffZ]) != 0.0)
		return true;
	return false;
}

stock IsValidStableFire(fireid)
{
	if(fireid > MAX_FIRES)
		return false;

	if((sfInfo[fireid][ffX] + sfInfo[fireid][ffY] + sfInfo[fireid][ffZ]) != 0.0)
		return true;
	return false;
}

stock GetClosestFire(playerid)
{
	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	new fire, Float:fDistance = 10000.0, Float:fTemp;
	for(new i = 0; i < MAX_FIRES; i++)
	{
		if(!IsValidFire(i) || !ffInfo[i][ffUse])
			continue;

		fTemp = GetDistance(x, y, z, ffInfo[i][ffX], ffInfo[i][ffY], ffInfo[i][ffZ]);
		if(fTemp < fDistance)
		{
			fire = i;
			fDistance = fTemp;
		}
	}
	return fire;
}


stock AddFire(Float:x, Float:y, Float:z)
{
	new slot = -1;
	for(new i = 0; i < MAX_ACTUAL_FIRES; i++)
	{
		if(gmInfo[actFires][i] <= 0) 
		{
			slot = i;
			break;
		}		
	}
	new fireid;
	for(new i = 0; i < MAX_FIRES; i++)
	{
		if(IsValidFire(i) || ffInfo[i][ffUse])
			continue;
		fireid = i;
		break;
	}
	if(slot >= 0)
	{
		gmInfo[totalFires]++;
		gmInfo[actFires][slot] = fireid;
		ffInfo[fireid][ffHealth] = 100.0;
		ffInfo[fireid][ffUse] = true;
		ffInfo[fireid][ffX] = x;
		ffInfo[fireid][ffY] = y;
		ffInfo[fireid][ffZ] = z;
		ffInfo[fireid][ffObject] = CreateDynamicObject(18689, ffInfo[fireid][ffX], ffInfo[fireid][ffY], ffInfo[fireid][ffZ]-2.5, 0.0, 0.0, 0.0);
		ffInfo[fireid][ffSmoke] = CreateDynamicObject(18727, ffInfo[fireid][ffX], ffInfo[fireid][ffY], ffInfo[fireid][ffZ]-2.5, 0.0, 0.0, 0.0);
		ffInfo[fireid][ffText] = CreateDynamic3DTextLabel("100/100", -1, ffInfo[fireid][ffX], ffInfo[fireid][ffY], ffInfo[fireid][ffZ], 100.0);  
		printf("Stworzono po¿ar %d. Health %f", fireid, ffInfo[fireid][ffHealth]);
	}
	else
		printf("ERROR_MAX_FIRES__PLEASE_CLEAN_TABLE");
	return 1;
}

stock DestroyFire(fireid)
{
	if(IsValidFire(fireid))
	{
		new slot = -1;
		for(new i = 0; i < MAX_ACTUAL_FIRES; i++)
		{
			if(fireid == gmInfo[actFires][i])
			{
				slot = i;
				break;
			}
		}
		if(slot >= 0)
		{
			gmInfo[actFires][slot] = 0;
			DestroyDynamicObject(ffInfo[fireid][ffObject]);
			DestroyDynamicObject(ffInfo[fireid][ffSmoke]);
			DestroyDynamic3DTextLabel(ffInfo[fireid][ffText]);
			ffInfo[fireid][ffUse] = false;
			ffInfo[fireid][ffX] = 0.0;
			ffInfo[fireid][ffY] = 0.0;
			ffInfo[fireid][ffZ] = 0.0;
			gmInfo[lastFire] = fireid;
			ffInfo[fireid][ffHealth] = 0.0;
			gmInfo[totalFires]--;
		}
		else
			printf("ERROR_NOT_FOUND_FIRE_ID");
	}
	else
		printf("ERROR_UNFENED_FIRE_%d", fireid);
	return 1;
}

stock IsInWaterCar(playerid)
{
	if(GetVehicleModel(GetPlayerVehicleID(playerid)) == 407 || GetVehicleModel(GetPlayerVehicleID(playerid)) == 601) 
		return true;
	return false;
}

stock CreateRandomFire()
{
	new fireid = random(MAX_FIRES);
	if(IsValidStableFire(fireid) && fireid != gmInfo[lastFire])
	{
		new szString[64];
		AddFire(sfInfo[fireid][ffX], sfInfo[fireid][ffY], sfInfo[fireid][ffZ]);

		AddFire(sfInfo[fireid][ffX] + float(random(5)), sfInfo[fireid][ffY], sfInfo[fireid][ffZ]);
		AddFire(sfInfo[fireid][ffX] + float(random(5)), sfInfo[fireid][ffY] + float(random(5)), sfInfo[fireid][ffZ]);
		AddFire(sfInfo[fireid][ffX], sfInfo[fireid][ffY] + float(random(5)), sfInfo[fireid][ffZ]);
		format(szString, sizeof szString, "W³aœnie wybuch³ po¿ar w {b]%s{/b}! Musisz siê pospieszyæ aby go ugasiæ!", GetPointZone(ffInfo[fireid][ffX], ffInfo[fireid][ffY]));
		foreach(new i : Player) {
			if(IsWorked(i, TEAM_TYPE_MEDIC))
				Msg(i, COLOR_INFO3, szString);
		}
		format(szString, sizeof szString, "Uwaga! Wybuch³ po¿ar w %s! Zachowaj ostro¿noœæ!", GetPointZone(ffInfo[fireid][ffX], ffInfo[fireid][ffY]));
		MsgToAll(COLOR_INFO2, szString);
	}
	else 
		CreateRandomFire();

	return 1;
}

Float:DistanceCameraTargetToLocation(Float:CamX, Float:CamY, Float:CamZ,   Float:ObjX, Float:ObjY, Float:ObjZ,   Float:FrX, Float:FrY, Float:FrZ) {
 
        new Float:TGTDistance;
        TGTDistance = floatsqroot((CamX - ObjX) * (CamX - ObjX) + (CamY - ObjY) * (CamY - ObjY) + (CamZ - ObjZ) * (CamZ - ObjZ));
        new Float:tmpX, Float:tmpY, Float:tmpZ;
        tmpX = FrX * TGTDistance + CamX;
        tmpY = FrY * TGTDistance + CamY;
        tmpZ = FrZ * TGTDistance + CamZ;
        return floatsqroot((tmpX - ObjX) * (tmpX - ObjX) + (tmpY - ObjY) * (tmpY - ObjY) + (tmpZ - ObjZ) * (tmpZ - ObjZ));
}
 
stock PlayerFaces(playerid, Float:x, Float:y, Float:z, Float:radius)
{
    new Float:cx,Float:cy,Float:cz,Float:fx,Float:fy,Float:fz;
    GetPlayerCameraPos(playerid, cx, cy, cz);
    GetPlayerCameraFrontVector(playerid, fx, fy, fz);
    if(radius <= DistanceCameraTargetToLocation(cx, cy, cz, x, y, z, fx, fy, fz))
    	return false;
    return true;
}

stock IsPlayerLookingAtPoint(playerid,Float:X,Float:Y,Float:Z,Float:ViewWidth=22.5,Float:ViewHeight=10.0)
{
    new Float:cx,Float:cy,Float:cz,Float:pa1,Float:pa2,Float:ca1,Float:ca2,Float:px,Float:py,Float:pz;
    GetPlayerCameraFrontVector(playerid,cx,cy,cz);
    GetPlayerPos(playerid,px,py,pz);
    pz+=2.0;
    cx=floatadd(cx,px);
    cy=floatadd(cy,py);
    cz=floatadd(cz,pz);
    pa1=atan2(X-px,Y-py);
    if(pa1>360)pa1=floatsub(pa1,360);
    if(pa1<0)pa1=floatadd(pa1,360);
    pa2=atan2(Y-py,Z-pz);
    if(pa2>360)pa2=floatsub(pa2,360);
    if(pa2<0)pa2=floatadd(pa2,360);
    ca1=atan2(cx-px,cy-py);
    if(ca1>360)ca1=floatsub(ca1,360);
    if(ca1<0)ca1=floatadd(ca1,360);
    ca2=atan2(cy-py,cz-pz);
    if(ca2>360)ca2=floatsub(ca2,360);
    if(ca2<0)ca2=floatadd(ca2,360);
	if((ca1>(pa1-ViewWidth))&&(ca1<(pa1+ViewWidth))&&(ca2>(pa2-ViewHeight))&&(ca2<(pa2+ViewHeight)))return 1;
    if((pa1-ViewWidth)<0)
    {
        ca1-=360.0;
        if((ca1>(pa1-ViewWidth))&&(ca1<(pa1+ViewWidth))&&(ca2>(pa2-ViewHeight))&&(ca2<(pa2+ViewHeight)))return 1;
    }
    if((pa1+ViewWidth)>360)
    {
        ca1+=360.0;
        if((ca1>(pa1-ViewWidth))&&(ca1<(pa1+ViewWidth))&&(ca2>(pa2-ViewHeight))&&(ca2<(pa2+ViewHeight)))return 1;
    }
    return 0;
}

CMD:pozar(playerid)
{
	Msg(playerid, COLOR_INFO2, "Testowy po¿ar.");
	CreateRandomFire();
	return 1;
}

CMD:closefire(playerid)
{
	printf("Najbli¿szy pozar %d", GetClosestFire(playerid));
	return 1;
}
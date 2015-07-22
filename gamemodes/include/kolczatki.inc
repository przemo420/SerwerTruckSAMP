stock StripUpdate(playerid)
{
	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		for(new i = 0; i < sizeof(ssInfo); i++)
		{
			if(IsPlayerInRangeOfPoint(playerid, 3.0, ssInfo[i][sX], ssInfo[i][sY], ssInfo[i][sZ]))
			{
				if(ssInfo[i][sCreated] == 1)
				{
					ClosestTireToStrip(GetPlayerVehicleID(playerid), ssInfo[i][sX], ssInfo[i][sY], ssInfo[i][sZ]);
					break;
				}
			}
		}
	}
	return 1;
}

stock ClosestTireToStrip(vehicleid, Float:ssX, Float:ssY, Float:ssZ)
{
	new NearTires = 1, Float:pos[3], Float:distance[2], Float:fPos[3];

	GetVehiclePos(vehicleid, fPos[0], fPos[1], fPos[2]);
	
	GetVehicleModelInfo(GetVehicleModel(vehicleid), VEHICLE_MODEL_INFO_WHEELSFRONT, pos[0], pos[1], pos[2]);
	distance[0] = GetDistancePointToPoint(ssX, ssY, ssZ, (fPos[0] + pos[0]), (fPos[1] + pos[1]), (fPos[2] + pos[2]));

	GetVehicleModelInfo(GetVehicleModel(vehicleid), VEHICLE_MODEL_INFO_WHEELSREAR, pos[0], pos[1], pos[2]);
	distance[1] = GetDistancePointToPoint(ssX, ssY, ssZ, (fPos[0] + pos[0]), (fPos[1] + pos[1]), (fPos[2] + pos[2]));

	if(distance[0] < distance[1])
		NearTires = 0;

	new panels, doors, lights, tires;
	GetVehicleDamageStatus(vehicleid, panels, doors, lights, tires);

	if(tires == 0000)
		switch(NearTires)
		{
			case 0: tires = 0b1010;
			case 1: tires = 0b0101;
		}
	else
		if((tires & 1010))
		{
			if(distance[1] <= 1.0)
				tires = 0b1111;
		}
		else
		{
			if(distance[0] <= 1.0)
				tires = 0b1111;
		}
	UpdateVehicleDamageStatus(vehicleid, panels, doors, lights, tires);
	return 1;
}

stock GetDistancePointToPoint(Float:x1, Float:y1, Float:z1, Float:x2, Float:y2, Float:z2)
{
	return floatround(floatsqroot(((( x1 - x2 ) * ( x1 - x2 )) + (( y1 - y2 ) * ( y1 - y2 )) + (( z1 - z2 ) * ( z1 - z2 )))));
}

stock CreateStrip(Float:x, Float:y, Float:z, Float:Angle)
{
	for(new i = 0; i < sizeof(ssInfo); i++)
	{
		if(ssInfo[i][sCreated] == 0)
		{
			ssInfo[i][sCreated] = 1;
			ssInfo[i][sX] = x;
			ssInfo[i][sY] = y;
			ssInfo[i][sZ] = z-0.7;
			ssInfo[i][sObject] = CreateDynamicObject(2899, x, y, z-0.9, 0.0, 0.0, Angle-90);
			return 1;
		}
	}
	return 1;
}

stock DeleteAllStrip()
{
	for(new i = 0; i < sizeof(ssInfo); i++)
	{
		if(ssInfo[i][sCreated] == 1)
		{
			ssInfo[i][sCreated] = 0;
			ssInfo[i][sX] = 0.0;
			ssInfo[i][sY] = 0.0;
			ssInfo[i][sZ] = 0.0;
			DestroyDynamicObject(ssInfo[i][sObject]);
		}
	}
	return 1;
}
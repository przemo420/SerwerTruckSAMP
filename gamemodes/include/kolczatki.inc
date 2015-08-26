stock StripUpdate(playerid)
{
	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		for(new i = 0; i < MAX_SPIKESTRIPS; i++)
		{
			if(IsPlayerInRangeOfPoint(playerid, 3.0, spikeInfo[i][sX], spikeInfo[i][sY], spikeInfo[i][sZ]))
			{
				if(spikeInfo[i][sCreated] == 1)
				{
					ClosestTireToStrip(GetPlayerVehicleID(playerid), spikeInfo[i][sX], spikeInfo[i][sY], spikeInfo[i][sZ]);
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
			if(distance[1] <= 1.3)
				tires = 0b1111;
		}
		else
		{
			if(distance[0] <= 1.3)
				tires = 0b1111;
		}
	UpdateVehicleDamageStatus(vehicleid, panels, doors, lights, tires);
	return 1;
}

stock GetDistancePointToPoint(Float:x1, Float:y1, Float:z1, Float:x2, Float:y2, Float:z2)
{
	return floatround(floatsqroot(((( x1 - x2 ) * ( x1 - x2 )) + (( y1 - y2 ) * ( y1 - y2 )) + (( z1 - z2 ) * ( z1 - z2 )))));
}

stock CreateStrip(playerid, Float:x, Float:y, Float:z, Float:Angle)
{
	new count = -1;
	for(new i = 0; i < MAX_SPIKESTRIPS; i++)
	{
		if(spikeInfo[i][sCreated] == 0)
		{
			new string[50];
			format(string, sizeof(string), "{FFFFFF}UID: %d\nUtworzy³: %s", i, PlayerName(playerid));

			count = i;
			spikeInfo[i][sCreated] = 1;
			spikeInfo[i][sX] = x;
			spikeInfo[i][sY] = y;
			spikeInfo[i][sZ] = z-0.7;
			spikeInfo[i][sObject] = CreateDynamicObject(2899, x, y, z-0.9, 0.0, 0.0, Angle-90);
			spikeInfo[i][sText] = CreateDynamic3DTextLabel(string, ZIELONY6, x, y, z, 5.0);

			return 1;
		}
	}
	return count;
}

stock DeleteAllStrip()
{
	for(new i = 0; i < MAX_SPIKESTRIPS; i++)
	{
		if(spikeInfo[i][sCreated] == 1)
		{
			spikeInfo[i][sCreated] = 0;
			spikeInfo[i][sX] = 0.0;
			spikeInfo[i][sY] = 0.0;
			spikeInfo[i][sZ] = 0.0;
			DestroyDynamicObject(spikeInfo[i][sObject]);
			DestroyDynamic3DTextLabel(spikeInfo[i][sText]);
		}
	}
	return 1;
}

CMD:kolczatka(playerid)
{
	if(firmInfo[playerInfo[playerid][pFirm]][tType] != TEAM_TYPE_POLICE)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	new Float:pos[4], id;
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	GetPlayerFacingAngle(playerid, pos[3]);
	id = CreateStrip(playerid, pos[0], pos[1], pos[2], pos[3]);

	if(id < 0)
		Msg(playerid, COLOR_INFO, "Nie mo¿na rozstawiæ wiêcej kolczatek.");
	else
		Msg(playerid, COLOR_INFO, "Pomyœlnie ustawiono kolczatke");

	return 1;
}

CMD:usunkolczatke(playerid, params[])
{
	if(firmInfo[playerInfo[playerid][pFirm]][tType] != TEAM_TYPE_POLICE)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(isnull(params))
		return Msg(playerid, COLOR_ERROR, "/usunkolczatke [id]");

	new i = strval(params);

	if(i < 0 || i > MAX_SPIKESTRIPS)
		return Msg(playerid, COLOR_ERROR, "B³êdne ID kolczatki.");

	if(spikeInfo[i][sCreated] == 0)
		return Msg(playerid, COLOR_ERROR, "Kolczatka o danym ID nie istnieje.");

	spikeInfo[i][sCreated] = 0;
	spikeInfo[i][sX] = 0.0;
	spikeInfo[i][sY] = 0.0;
	spikeInfo[i][sZ] = 0.0;
	DestroyDynamicObject(spikeInfo[i][sObject]);
	DestroyDynamic3DTextLabel(spikeInfo[i][sText]);

	Msg(playerid, COLOR_INFO, "Pomyœlnie usuniêto kolczatkê.");	

	return 1;
}

CMD:usunkolczatki(playerid)
{
	if(firmInfo[playerInfo[playerid][pFirm]][tType] != TEAM_TYPE_POLICE)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(spikeInfo[0][sCreated] == 0)
		return Msg(playerid, COLOR_ERROR, "Brak kolczatek do usuniêcia.");
	else
		DeleteAllStrip();

	Msg(playerid, COLOR_INFO, "Pomyœlnie usuniêto {b}wszystkie{/b} kolczatki.");
	return 1;
}
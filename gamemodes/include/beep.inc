forward Beep_OnGameModeInit();
public Beep_OnGameModeInit()
{
	SetTimer("ReverseBeeper", 1000, true);
	return 1;
}

forward ReverseBeeper();
public ReverseBeeper()
{
	for(new i = 0; i < GetVehiclePoolSize() + 1; i++)
	{
		if(!IsValidVehicle(i) || !IsVehicleDrivingBackwards(i) || !IsVehicleTruck(GetVehicleModel(i)))
			continue;

		new Float:pos[3];
		GetVehiclePos(i, pos[0], pos[1], pos[2]);
		for(new c = 0; c < MAX_PLAYERS; c++)
		{
			if(!IsPlayerConnected(c) || !IsPlayerInRangeOfPoint(c, 5.0, pos[0], pos[1], pos[2]))
				continue;

			PlayerPlaySound(c, 1147, 0.0, 0.0, 0.0); 
		}
	}
	return 1;
}

stock IsVehicleDrivingBackwards(vehicleid)
{
	new Float:Float[3];
	if(GetVehicleVelocity(vehicleid, Float[1], Float[2], Float[0]))
	{
		GetVehicleZAngle(vehicleid, Float[0]);
		if(Float[0] < 90)
		{
			if(Float[1] > 0 && Float[2] < 0) 
				return true;
		}
		else if(Float[0] < 180)
		{
			if(Float[1] > 0 && Float[2] > 0) 
				return true;
		}
		else if(Float[0] < 270)
		{
			if(Float[1] < 0 && Float[2] > 0) 
				return true;
		}
		else if(Float[1] < 0 && Float[2] < 0) 
			return true;
	}
	return false;
}
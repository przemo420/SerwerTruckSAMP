forward Sytuacje_OneMinPlayerTimer(playerid);
public Sytuacje_OneMinPlayerTimer(playerid)
{
	if(!IsPlayerInAnyVehicle(playerid))
		return 0;

	switch(random(20))
	{
		case 0: 
		{
			if(GetDTime(playerid) < 7200)
				return 0;

			SetPlayerDrunkLevel(playerid, 5500);
			Msg(playerid, COLOR_INFO2, "Zmêczy³eœ siê, zbyt d³ugo jeŸdzisz!");
		} // gdy za d³ugo jeŸdzi - bêdzie mia³ ciemniejszy ekran + drunk

		case 1: 
		{

		} // gdy bêdzie za g³odny
		
		case 2: 
		{
			SetPVarInt(playerid, "zaslabl", 1);
			Msg(playerid, COLOR_ERROR, "Zas³ab³eœ za kierownic¹!");
			TogglePlayerControllable(playerid, false);
			SetTimerEx("Zaslabniecie", 15000, false, "d", playerid);
		} // po prostu zas³abniêcie

		default: {} // nic
	}
	return 1;
}

forward Zaslabniecie(playerid);
public Zaslabniecie(playerid)
{
	if(!IsPlayerConnected(playerid) || !GetPVarInt(playerid, "zaslabl"))
		return 0;

	TogglePlayerControllable(playerid, true);
	DeletPVar(playerid, "zaslabl");
	Msg(playerid, COLOR_INFO, "Odzyska³eœ si³y, lepiej nie wsiadaj za kó³ko.");
	return 1;
}

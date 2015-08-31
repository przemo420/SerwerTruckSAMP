forward Sytuacje_OneMinPlayerTimer(playerid);
public Sytuacje_OneMinPlayerTimer(playerid)
{
	if(!IsPlayerInAnyVehicle(playerid))
		return 0;

	switch(random(20))
	{
		case 0: {} // gdy za d³ugo jeŸdzi - bêdzie mia³ ciemniejszy ekran + drunk
		case 1: {} // gdy bêdzie za g³odny
		case 2: {} // po prostu zas³abniêcie
		default: {} // nic
	}
	return 1;
}
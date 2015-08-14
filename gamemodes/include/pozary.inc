#include "include/lib/pozary.inc"

forward Fire_OnGameModeInit();
public Fire_OnGameModeInit()
{
	FireSystem_Init();
	SetTimer("LosujPozary", 30*60*1000, true);
	return 1;
}

forward LosujPozary();
public LosujPozary()
{
	if(NaDyzurze[2])
	{
		CallLocalFunction("RandomFire", ""),
		MsgTo(COLOR_INFO, "Do wszystkich jednostek: ZG£OSZONO PO¯AR!", TEAM_TYPE_MEDIC);
	}
	
	return 1;
}

CMD:pozary(playerid, cmdtext[])
{
	AddFire(1648.4308, -1053.9159, 23.8984, 80+random(10));
	return 1;
}

CMD:nadyzurze(playerid, cmdtext[])
{
	new string[50];
	for(new i; i<=7; i++)
		format(string, sizeof(string), "%s%s (%d)\n", string, Firmy[i][tName], NaDyzurze[i]);

	return ShowInfo(playerid, string);
}

forward Fire_OnPlayerKey(playerid, newkeys, oldkeys);
public Fire_OnPlayerKey(playerid, newkeys, oldkeys)
{
	//if(PRESSED(KEY_FIRE))
	//{
		//Dialog_Show(playerid, NEVER_DIALOG, DIALOG_STYLE_MSGBOX, " ", "Odleg³oœæ od punktu: %d", "Wybierz", "Zamknij", IsPlayerLookingAtPoint(playerid, 1648.4308, -1053.9159, 23.8984));
	//}
}
#include "include/lib/pozary.inc"

forward Fire_OnGameModeInit();
public Fire_OnGameModeInit()
{
	FireSystem_Init();
	SetTimer("LosujPozary", 30*60*1000, true);
	return 1;
}

forward LosujPozary(Float:pos);
public LosujPozary(Float:pos)
{
	if(NaDyzurze[2])
	{
		CallLocalFunction("RandomFire", "f", pos),
		MsgTo(COLOR_INFO, "Do wszystkich jednostek: ZG£OSZONO PO¯AR!", TEAM_TYPE_MEDIC);
	}
	
	return 1;
}

CMD:pozar(playerid, cmdtext[])
	return LosujPozary(floatstr(cmdtext));

CMD:pozary(playerid, cmdtext[])
{
	AddFire(1648.4308, -1053.9159, 23.8984, 80+random(10));
	return 1;
}

CMD:nadyzurze(playerid, cmdtext[])
{
	new string[50];
	for(new i; i<=7; i++)
		format(string, sizeof(string), "%s%s (%d)\n", string, firmInfo[i][tName], NaDyzurze[i]);

	return ShowInfo(playerid, string);
}

forward Fire_OneSecPlayerTimer(playerid);
public Fire_OneSecPlayerTimer(playerid)
{
	new newkeys, ud, lr;
    GetPlayerKeys(playerid, newkeys, ud, lr);

	if(HOLDING(KEY_FIRE))
	{
		new fireobject = GetPlayerCameraTargetObject(playerid);
		new id = GetClosestFire(playerid);

		if(fireobject == INVALID_OBJECT_ID)
			return 1;

		if(Fire[id][object] != fireobject)
			return 1;

		if(GetPlayerWeapon(playerid) != 42)
			return Msg(playerid, COLOR_ERROR, "Tylko gaœnic¹ mo¿esz zgasiæ po¿ar.");

		if(Fire[id][Health] != 0)
		{
			Fire[id][Health]-=1;
			format(_firestr, sizeof(_firestr), TXT_TEXT);
			Update3DTextLabelText(Fire[id][info], 0xFFFFFFFFF, _firestr);
		}
		else 
		{
			DeleteFire(id);
			OnPlayerText(playerid, "#Ognisko ugaszone, over.");
		}
	}

	return 1;
}

forward OnPlayerNearFire(playerid, fireid);
forward OnFireAdd(fireid);
forward OnFireDelete(fireid);
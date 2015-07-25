CMD:przeglad(playerid, params[])
{
	new targetid, vehicleid, string[100];

	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_POMOC)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(sscanf(params, "d", targetid))
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/przeglad [id gracza]{/b}");

	if(!IsPlayerConnected(targetid))
		return Msg(playerid, COLOR_ERROR, "Gracza z tym ID nie ma online.");

	if(GetDistancePlayerToPlayer(playerid, targetid) > 20)
		return Msg(playerid, COLOR_ERROR, "Znajdujesz siê za daleko od gracza.");

	if(GetPlayerState(targetid) != PLAYER_STATE_DRIVER)
		return Msg(playerid, COLOR_ERROR, "Gracz z tym ID nie prowadzi pojazdu.");

	vehicleid = GetPlayerVehicleID(targetid);

	if(Spawned[vehicleid])
		return Msg(playerid, COLOR_ERROR, "Nie mo¿esz zrobiæ przegl¹du w tym pojeŸdzie.");

	format(string, sizeof(string), "UPDATE `Pojazdy` SET `Przeglad`= aktualny timestamp WHERE `id`='%d'", DBVehID[vehicleid]);
	mysql_query(string);

	format(string, sizeof(string), "SELECT `Przeglad` FROM `Pojazdy` WHERE `id`='%d'", DBVehID[vehicleid]);
	mysql_query(string);
	mysql_store_result();
	mysql_fetch_field("Przeglad", vehInfo[DBVehID[vehicleid]][vPrzeglad]);
	mysql_free_result();

	Msg(playerid, COLOR_INFO, "Zrobi³eœ przegl¹d w pojeŸdzie.");
	Msg(targetid, COLOR_INFO, "W Twoim pojeŸdzie zrobiono przegl¹d.");
	return 1;
}

forward Pomoc_OnPlayerEnterJob(playerid);
public Pomoc_OnPlayerEnterJob(playerid)
{
	GivePlayerWeapon(playerid, 11, 99999);
	return GivePlayerWeapon(playerid, 41, 99999);
}

forward Pomoc_OnPlayerKey(playerid, newkeys, oldkeys);
public Pomoc_OnPlayerKey(playerid, newkeys, oldkeys)
{
	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_POMOC)
		return 0;

	if(newkeys & KEY_FIRE &&(GetPlayerWeapon(playerid) == 41 && !IsPlayerInAnyVehicle(playerid)))
	{
		new nearveh = GetNearestVehicle(playerid,5);

		if(IsPlayerFacingVehicle(playerid,nearveh))
		{
			SetTimerEx("ResprayTimer",3000,false,"ii",playerid,GetNearestVehicle(playerid,5));
			Firehold[playerid] = 1;
		}
	}
	
	if(oldkeys & KEY_FIRE && (Firehold[playerid] == 1)) 
		Firehold[playerid] = 0;
	
	if(newkeys & KEY_FIRE && (GetPlayerWeapon(playerid) == 11 && !IsPlayerInAnyVehicle(playerid)))
	{
		new nearveh = GetNearestVehicle(playerid,5);

		if(IsPlayerFacingVehicle(playerid,nearveh))
		{
			if(Firehold2[playerid] == 0)
				Firehold2[playerid] = 1;
			else if(Firehold2[playerid] == 1)
				Firehold2[playerid] = 2;
			else if(Firehold2[playerid] == 2)
				Firehold2[playerid] = 3;
			else if(Firehold2[playerid] == 3)
			{
				SetTimerEx("RepairTimer",3000,false,"ii",playerid,GetNearestVehicle(playerid,5));
				Firehold2[playerid] = 4;
			}
		}
	}

	return 1;
}

forward ResprayTimer(playerid,vehicleid);
public ResprayTimer(playerid,vehicleid)
{
new kolor1 = GetPVarInt(playerid,"typ1");
new kolor2 = GetPVarInt(playerid,"typ2");

if(GetPlayerWeapon(playerid) == 41 && GetNearestVehicle(playerid,8) == vehicleid && Firehold[playerid] == 1)
{
if(IsPlayerFacingVehicle(playerid,vehicleid))
{
if(GetPVarInt(playerid,"typ1") && GetPVarInt(playerid,"typ2"))
{
ChangeVehicleColor(GetNearestVehicle(playerid,8),kolor1,kolor2);
RepairVehicle(GetNearestVehicle(playerid,8));
}
else
{
ChangeVehicleColor(GetNearestVehicle(playerid,8),random(127),random(127));
RepairVehicle(GetNearestVehicle(playerid,8));
}
new c[2];
GetVehicleColor(GetNearestVehicle(playerid,8), c[0], c[1]);

SendClientMessage(playerid, 0x0, "{C0C0C0}Lakierowanie pojazdu zakoñczone.");
DeletePVar(playerid, "typ1");
DeletePVar(playerid, "typ2");
ResetPlayerWeapons(playerid);
GivePlayerWeapon(playerid, 43, 9999);
GivePlayerWeapon(playerid, 11, 99999);
GivePlayerWeapon(playerid, 41, 99999);
}
}
return 1;
}

forward RepairTimer(playerid,vehicleid);
public RepairTimer(playerid,vehicleid)
{
if(GetPlayerWeapon(playerid) == 11 && Firehold2[playerid] == 4)
{
if(IsPlayerFacingVehicle(playerid,vehicleid))
{
RepairVehicle(GetNearestVehicle(playerid,8));
SendClientMessage(playerid, 0x0, "{C0C0C0}Naprawianie pojazdu zakoñczone.");
ResetPlayerWeapons(playerid);
GivePlayerWeapon(playerid, 43, 9999);
GivePlayerWeapon(playerid, 11, 99999);
GivePlayerWeapon(playerid, 41, 99999);
Firehold2[playerid] = 0;
}
}
return 1;
}

CMD:setplate(playerid, params[])
{
	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_POMOC)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");
	
	if(strlen(params) < 3 || strlen(params) > 32)
		return Msg(playerid, COLOR_ERROR, "Wpisz {b}/setplate [treœæ]{/b}");

	SetPVarString(playerid, "PLATE_Name", params);
	new szString[128], Float:fPos[3], count = 0;
	GetPlayerPos(playerid, fPos[0], fPos[1], fPos[2]);
	for(new i = 0; i < MAX_VEHICLES; i++)
	{
		if(IsValidVehicle(i))
		{
			new Float:fDistance;
			fDistance = GetVehicleDistanceFromPoint(i, fPos[0], fPos[1], fPos[2]);
			if(fDistance < 10.0)
			{
				new szTemp[32];
				format(szTemp, sizeof szTemp, "%s\t%d\n", GetVehicleModelName(GetVehicleModel(i)), i);
				strcat(szString, szTemp);
				count++;
			}
		}
	}
	if(count < 1)
		strcat(szString, "Nie odnaleziono pojazdów w promieniu 5m.");
	else
		strins(szString, "Nazwa\tID\n", 0);

	SetPVarInt(playerid, "PLATE_Count", count);
	Dialog_Show(playerid, DIALOG_SET_PLATE, count > 0 ? DIALOG_STYLE_TABLIST_HEADERS : DIALOG_STYLE_LIST, " ", szString, "Wybierz", "WyjdŸ");
	return 1;
}

forward SetPos(vehicleid, Float:x, Float:y, Float:z);
public SetPos(vehicleid, Float:x, Float:y, Float:z)
{
	SetVehiclePos(vehicleid, x, y, z);
	return 1;
}

CMD:napraw(playerid, params[])
{
	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_POMOC)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	Firehold2[playerid] = 0;
	Msg(playerid, COLOR_INFO3, "Mo¿esz {b}zacz¹æ{/b} naprawianie pojazdu.");
	GivePlayerWeapon(playerid, 11, 50000);
	return 1;
}

CMD:zatankuj(playerid)
{
	if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] != TEAM_TYPE_POMOC)
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ");

	if(!GetPVarInt(playerid, "Worked"))
		return Msg(playerid, COLOR_ERROR, "Musisz byæ na s³u¿bie.");

	if(!IsPlayerInAnyVehicle(playerid))
		return Msg(playerid, COLOR_ERROR, "Nie siedzisz w ¿adnym pojeŸdzie.");

	new vehicleid = GetPlayerVehicleID(playerid), string[80];

	if(Spawned[vehicleid])
		vehOtherInfo[vehicleid][vFuel] = MaxFuel(GetVehicleModel(vehicleid));
	else
		vehInfo[DBVehID[vehicleid]][vFuel] = MaxFuel(GetVehicleModel(vehicleid));

	format(string, sizeof(string), "Zatankowa³eœ pojazd {b}ID: %d{/b} do pe³na.", vehicleid);
	Msg(playerid, COLOR_INFO3, string);
	return 1;
}

Dialog:DIALOG_SET_PLATE(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;
	new playerName[32];
	GetPVarString(playerid, "PLATE_Name", playerName, sizeof playerName);
	if(GetPVarInt(playerid, "PLATE_Count") < 1)
		return cmd_setplate(playerid, playerName);

	new Float:fPos[3], count = 0;
	GetPlayerPos(playerid, fPos[0], fPos[1], fPos[2]);
	for(new i = 0; i < MAX_VEHICLES; i++)
	{
		if(IsValidVehicle(i))
		{
			new Float:fDistance;
			fDistance = GetVehicleDistanceFromPoint(i, fPos[0], fPos[1], fPos[2]);
			if(fDistance < 10.0)
			{
				count++;
				if(listitem+1 == count)
				{
					new szTemp[64];
					format(szTemp, sizeof szTemp, "Pomyœlnie %s");
					GetVehiclePos(i, fPos[0], fPos[1], fPos[2]);
					SetTimerEx("SetPos", 1500, false, "dfff", i, fPos[0], fPos[1], fPos[2]);
					SetVehicleNumberPlate(i, playerName);
					SetVehicleToRespawn(i);

					DeletePVar(playerid, "PLATE_Name");
					DeletePVar(playerid, "PLATE_Count");
					break;
				}
			}
		}
	}
	return 1;
}
// Zmienne
new CRASHON = 1;
new Float:StareHPPojazdu[MAX_VEHICLES];
new Float:NoweHPPojazdu[MAX_VEHICLES];

forward Wypadek();
public Wypadek()
{
	if(CRASHON == 1)
	{
		foreach (new playerid : Player)
		{
			if(NaDyzurze[TEAM_TYPE_MEDIC] <= 0)
			{
				if(!IsPlayerConnected(playerid) || !IsPlayerLogged(playerid))
					continue;

				if(GetPVarInt(playerid, "Wypadek") > 0 && GetPVarInt(playerid, "Wypadekzmedykiem") > 0)
				{
					Msg(playerid, COLOR_INFO, "Pracownicy pogotowia zakoñczyli dy¿ur, ockniesz siê za {b}15 sekund{/b}.");
					new t;
					t = SetTimerEx_("POwypadku", 0, 15 * 1000, 1, "i", playerid);
					SetPVarInt(playerid, "healTimer", t);
					DeletePVar(playerid, "Wypadekzmedykiem");
					FadeColorForPlayer(playerid, 181, 51, 36, 125, 204, 58, 41, 125, 20, 0);
				}
			}
		}

		new Float:health;
		foreach (new i : Vehicle)
		{
			if(!IsValidVehicle(i))
				continue;

			GetVehicleHealth(i, NoweHPPojazdu[i]);

			if((StareHPPojazdu[i] - NoweHPPojazdu[i]) >= 40.0)
			{
				new cargoVehicle = IsVehicleTruck(GetVehicleModel(i)) ? GetVehicleTrailer(i) : i;
				if(vloadInfo[cargoVehicle][vLoaded] == true)
				{
					if((StareHPPojazdu[i] - NoweHPPojazdu[i]) >= 25.0)
					{
						cargoHealth[cargoVehicle] -= (StareHPPojazdu[i] - NoweHPPojazdu[i])/1.2;
						if(cargoHealth[cargoVehicle] < 0.0)
							cargoHealth[cargoVehicle] = 0.0;
					}
				}
				foreach (new g : Player)
				{
					if(IsPlayerSpawned(g) && IsPlayerInVehicle(g, i) && !GetPVarInt(g, "Wypadek"))
					{
						new t;
						new engine, lights, alarm, doors, bonnet, boot, objective;
						GetVehicleParamsEx(i, engine, lights, alarm, doors, bonnet, boot, objective);
								 			
						GetPlayerHealth(g, health);
						TogglePlayerControllable(g, 0);
						SetVehicleParamsEx(i, VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective);

						if(GetPVarInt(g, "PASY"))
							SetPlayerHealth(g, health - random(10));
						else 
							SetPlayerHealth(g, health - random(20));
	
						GetVehicleHealth(i, NoweHPPojazdu[i]);

						if(NoweHPPojazdu[i] < 300)
						{
							SetVehicleHealth(i, 0);
							new Float:Pos[3];
							GetPlayerPos(g, Float:Pos[0], Float:Pos[1], Float:Pos[2]);
							CreateExplosion(Float:Pos[0], Float:Pos[1], Float:Pos[2], 2, 50);
						}

						if(NaDyzurze[TEAM_TYPE_MEDIC] > 0)
						{
							Msg(g, COLOR_ERROR, "Spowodowa³eœ wypadek, poczekaj na {b}przyjazd{/b} pogotowia.");
							SetPVarInt(g, "Wypadekzmedykiem", 1);
							FadeColorForPlayer(g, 181, 51, 36, 125, 204, 58, 41, 125, 20, 0);
									
							MedicInfo(g);
						}
						else
						{
							FadeColorForPlayer(g, 181, 51, 36, 125, 204, 58, 41, 125, 20, 0);
							Msg(g, COLOR_ERROR, "Spowodowa³eœ wypadek, odzyskasz przytomnoœæ za {b}30 sekund{/b}.");
							t = SetTimerEx_("POwypadku", 0, 30000, 1, "i", g);
							SetPVarInt(g, "healTimer", t);
						}

						SetPVarInt(g, "Wypadek", 1);
						
					}
				}
			}
			GetVehicleHealth(i, StareHPPojazdu[i]);
		}
	}
	return 1;
}

// Po wypadku
forward POwypadku(playerid);
public POwypadku(playerid)
{
	FadeColorForPlayer(playerid, 204, 58, 41, 125, 0, 0, 0, 0, 25, 0);
	DeletePVar(playerid, "Wypadek");
	DeletePVar(playerid, "Wypadekzmedykiem");
	DeletePVar(playerid, "healTimer");
	TogglePlayerControllable(playerid, 1);
	Msg(playerid, COLOR_INFO, "{b}Odzyska³eœ{/b} przytomnoœæ.");
	return 1;
}

forward MedicInfo(playerid);
public MedicInfo(playerid)
{
	foreach (new i : Player)
	{
		if(IsWorked(i, TEAM_TYPE_MEDIC) && !GetPVarInt(i, "AFK") && i != playerid && !GetPVarInt(i, "otherAFK"))
		{
			Msg(i, COLOR_ERROR, "Ktoœ spowodowa³ wypadek, musisz poczekaæ na zg³oszenie!");
		}
	}

	Msg(playerid, COLOR_INFO, "Musisz {b}zawiadomiæ{/b} pogotowie komend¹ {b}/telefon{/b}!");
	return 1;
}

CMD:crashveh(playerid, params[])
{
	new string[128];

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(CRASHON == 1)
	{
		CRASHON = 0;
		format(string, sizeof string, "Wypadki zosta³y {b}wy³¹czone{/b} przez administratora {b}%s{/b}.", PlayerName(playerid));
		MsgToAll(COLOR_INFO2, string);
	}
	else
	{
		CRASHON = 1;
		format(string, sizeof string, "Wypadki zosta³y {b}w³¹czone{/b} przez administratora {b}%s{/b}.", PlayerName(playerid));
		MsgToAll(COLOR_INFO2, string);
	}

	return 1;
}
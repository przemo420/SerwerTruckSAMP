// Zmienne
new CRASHON = 1;
new Float:StareHPPojazdu[MAX_VEHICLES];
new Float:NoweHPPojazdu[MAX_VEHICLES];

// Odpalanie wypadku
forward Wypadek();
public Wypadek()
{
	if(CRASHON == 1)
	{
		new LiczbaMedykow = 0;

		Loop(playerid, MAX_PLAYERS)
			if(Firmy[PlayerInfo[playerid][pFirma]][tTyp] == TEAM_TYPE_MEDIC)
				if(GetPVarInt(playerid, "Worked") && !GetPVarInt(playerid, "AFK"))
					LiczbaMedykow++;

		new Float:health;

		Loop(i, MAX_VEHICLES)
		{
			if(!IsValidVehicle(i) || !IsPlayerInAnyVehicle(i))
				continue;

			GetVehicleHealth(GetPlayerVehicleID(i), NoweHPPojazdu[GetPlayerVehicleID(i)]);

			if(IsPlayerConnected(i))
			{
				if(LiczbaMedykow <= 0 && GetPVarInt(i, "Wypadek") && GetPVarInt(i, "Wypadekzmedykiem"))
				{
					SendClientMessage(i, 0x0, "{8080C0}Pogotowie zakoñczy³o dy¿ur. Odzyskasz przytomnoœæ za {FFFFFF}15 sekund.");
					SetTimerEx("POwypadku", 15000, false, "u", i);
					DeletePVar(i, "Wypadekzmedykiem");
				}

				if((StareHPPojazdu[GetPlayerVehicleID(i)] - NoweHPPojazdu[GetPlayerVehicleID(i)]) >= 100 && IsPlayerInVehicle(i, GetPlayerVehicleID(i)) && (GetPlayerState(i) == PLAYER_STATE_DRIVER || GetPlayerState(i) == PLAYER_STATE_PASSENGER) && !GetPVarInt(i, "Wypadek"))
				{
					Loop(g, MAX_PLAYERS)
					{
						if(IsPlayerConnected(g))
						{
							if(IsPlayerInVehicle(g, GetPlayerVehicleID(i)))
							{
								new vehicleid = GetPlayerVehicleID(g);
								new engine,lights,alarm,doors,bonnet,boot,objective;
								GetVehicleParamsEx(vehicleid,engine,lights,alarm,doors,bonnet,boot,objective);
								 			
								GetPlayerHealth(g, health);
								TogglePlayerControllable(g, 0);
								SetVehicleParamsEx(vehicleid,VEHICLE_PARAMS_OFF,lights,alarm,doors,bonnet,boot,objective);

								if(GetPVarInt(g, "PASY"))
									SetPlayerHealth(g, health-10);
								else 
									SetPlayerHealth(g, health-40);
	
								GetVehicleHealth(GetPlayerVehicleID(g), NoweHPPojazdu[GetPlayerVehicleID(g)]);

								if(NoweHPPojazdu[GetPlayerVehicleID(g)] < 300) 
									SetVehicleHealth(GetPlayerVehicleID(i), 0);

								if(LiczbaMedykow > 0)
								{
									SendClientMessage(g, 0x0, "{8080C0}Spowodowa³eœ wypadek, poczekaj na przyjazd pogotowia.");
									SetPVarInt(g, "Wypadekzmedykiem", 1);
									
									MedicInfo(g);
								}
								else
								{
									SendClientMessage(g, 0x0, "{8080C0}Spowodowa³eœ wypadek, odzyskasz przytomnoœæ za {FFFFFF}30 sekund.");
									SetTimerEx("POwypadku", 30000, false, "u", g);
								}

								SetPVarInt(g, "Wypadek", 1);
								awaryjne(g);
							}
						}
					}
				}

				GetVehicleHealth(GetPlayerVehicleID(i), StareHPPojazdu[GetPlayerVehicleID(i)]);
			}
		}
	}
	return 1;
}

// Po wypadku
forward POwypadku(playerid);
public POwypadku(playerid)
{
	DeletePVar(playerid, "Wypadek");
	DeletePVar(playerid, "Wypadekzmedykiem");
	TogglePlayerControllable(playerid, 1);
	Msg(playerid, COLOR_INFO, "{b}Odzyska³eœ{/b} przytomnoœæ.");
	RemovePlayerFromVehicle(playerid);

	return 1;
}

forward MedicInfo(playerid);
public MedicInfo(playerid)
{
	new string[128], Float:Pos[3];
	GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);

	Loop(i, MAX_PLAYERS)
	{
		if(Firmy[PlayerInfo[i][pFirma]][tTyp] == TEAM_TYPE_MEDIC && !GetPVarInt(i, "AFK"))
		{
			format(string, sizeof string, "Gracz {b}%s{/b} (ID: {b}%d{/b}) spowodowa³ wypadek! Zosta³ oznaczony na minimapce.", PlayerName(playerid), playerid);
			Msg(i, COLOR_ERROR, string);

			SetPlayerMapIcon(i, playerid, Pos[0], Pos[1], Pos[2], 19, 0, MAPICON_GLOBAL_CHECKPOINT);
		}
	}

	Msg(playerid, COLOR_INFO, "Pogotowie zosta³o {b}zawiadomione{/b}.");
	return 1;
}

// Komendy
CMD:crashveh(playerid, params[])
{
	new string[128];

	if(!PlayerInfo[playerid][pAdmin])
		return SendClientMessage(playerid, LIGHTRED, "Nie masz uprawnieñ.");

	if(CRASHON == 1)
	{
		CRASHON = 0;
		format(string, sizeof string, "Wypadki zosta³y {b}wy³¹czone{/b} przez administratora {b}%s{/b}.", PlayerName(playerid));
		MsgToAll(COLOR_INFO2, string);
	}
	else
	{
		CRASHON = 1;
		format(string, sizeof string, "Wypadki zosta³y {b}za³¹czone{/b} przez administratora {b}%s{/b}.", PlayerName(playerid));
		MsgToAll(COLOR_INFO2, string);
	}

	return 1;
}
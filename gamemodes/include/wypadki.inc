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
	SendClientMessage(playerid, 0x0, "{8080C0}Odzyska³eœ przytomnoœæ.");
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
			format(string, sizeof string, "{F40000}! {8080C0}Gracz (%s, %d) spowodowa³ wypadek! Zosta³ oznaczony na minimapce.", PlayerName(playerid), playerid);
			SendClientMessage(i, 0x0, string);

			SetPlayerMapIcon(i, playerid, Pos[0], Pos[1], Pos[2], 19, 0, MAPICON_GLOBAL_CHECKPOINT);
		}
	}

	SendClientMessage(playerid, 0x0, "{8080C0}Pogotowie zosta³o {FFFFFF}powiadomione.");
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
	format(string, sizeof string, "{C0C0C0}Wypadki zostaæy {FFFFFF}wyæÄ…czone {C0C0C0}przez %s.", PlayerName(playerid));
	SendClientMessageToAll(0x0, string);
	}
	else
	{
	format(string, sizeof string, "{C0C0C0}Wypadki zostaæy {FFFFFF}wæÄ…czone {C0C0C0}przez %s.", PlayerName(playerid));
	SendClientMessageToAll(0x0, string);
	CRASHON = 1;
	}

	return 1;
}
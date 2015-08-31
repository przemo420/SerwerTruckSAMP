forward Spectactor_OnPlayerKey(playerid, newkeys, oldkeys);
public Spectactor_OnPlayerKey(playerid, newkeys, oldkeys)
{
	if(GetPVarType(playerid, "Przeglada") != PLAYER_VARTYPE_NONE)
	{
		if(newkeys & KEY_FIRE || newkeys & KEY_AIM)
		{
			if(GetPVarInt(playerid, "LeciPetla"))
				return 1;

			new aktualniePrzegladany = GetPVarInt(playerid, "Przeglada"), gdzie = (newkeys & KEY_FIRE) ? (1) : (-1), bool:obrot, bool:znalazlo;

			for(new nastepny = aktualniePrzegladany+gdzie; nastepny <= GetMaxPlayers(); nastepny+=gdzie)
			{
				SetPVarInt(playerid, "LeciPetla", 1);

				if(nastepny == GetMaxPlayers())
				{
					if(obrot && !znalazlo)
					{
						TogglePlayerSpectating(playerid, 0);
						SpectactorTextDraw(playerid, false);
						break;
					}
					nastepny = 0;
					obrot = true;
				}

				if(nastepny == (-1))
				{
					if(obrot && !znalazlo)
					{
						TogglePlayerSpectating(playerid, 0);
						SpectactorTextDraw(playerid, false);
						break;
					}

					obrot = true;
					nastepny = GetMaxPlayers()-1;
				}

				if(nastepny == playerid || GetPVarType(nastepny, "Przeglada") != PLAYER_VARTYPE_NONE || GetPVarType(nastepny, "jestPrzegladany") != PLAYER_VARTYPE_NONE || !IsPlayerLogged(nastepny))
					continue;

				if(IsPlayerConnected(nastepny))
				{
					DeletePVar(aktualniePrzegladany, "jestPrzegladany");
					SetPVarInt(nastepny, "jestPrzegladany", playerid);
					SetPVarInt(playerid, "Przeglada", nastepny);

					if(IsPlayerInAnyVehicle(nastepny))
						PlayerSpectateVehicle(playerid, GetPlayerVehicleID(nastepny));
					else
						PlayerSpectatePlayer(playerid, nastepny);

					SetPVarInt(playerid, "LeciPetla", 0);
					obrot = false;
					znalazlo = true;
					break;
				}
				else
					znalazlo = false;

				if(!IsPlayerConnected(playerid) || gmInfo[gmUsersConnected] == 1)
					break;
			}
		}
	}

	return 1;
}

forward Spectactor_OnPlayerSpawn(playerid);
public Spectactor_OnPlayerSpawn(playerid)
{
	if(GetPVarType(playerid, "Przeglada") != PLAYER_VARTYPE_NONE)
	{
		DeletePVar(GetPVarInt(playerid, "Przeglada"), "jestPrzegladany");
		DeletePVar(playerid, "Przeglada");

		SetPlayerPos(playerid, GetPVarFloat(playerid, "specPosX"), GetPVarFloat(playerid, "specPosY"), GetPVarFloat(playerid, "specPosZ"));

		SpectactorTextDraw(playerid, false);
	}

	return 1;
}

forward SpectactorUpdate(playerid);
public SpectactorUpdate(playerid)
{
	if(GetPVarType(playerid, "Przeglada") == PLAYER_VARTYPE_NONE || GetPVarType(GetPVarInt(playerid, "Przeglada"), "Przeglada") != PLAYER_VARTYPE_NONE)
		return SpectactorTextDraw(playerid, false);

	new string[256], id = GetPVarInt(playerid, "Przeglada"), Float:vehhp, vehhp2[7], Float:plhp, Float:predkosc;

	if(IsPlayerInAnyVehicle(id))
	{
		GetVehicleHealth(GetPlayerVehicleID(id), vehhp);
		format(vehhp2, sizeof(vehhp2), "%.0f", vehhp);
		GetVehicleSpeed(GetPlayerVehicleID(id), predkosc);
	}
	else
	{
		format(vehhp2, sizeof(vehhp2), "Brak");
		predkosc = float(GetPlayerSpeed(id));
	}

	GetPlayerHealth(id, plhp);
	format(string, sizeof(string), "%d~n~%s~n~%d~n~%d~n~%.0f~n~%s~n~%.0f", id, PlayerName(id), GetMoney(id), GetScore(id), plhp, vehhp2, predkosc);
	PlayerTextDrawSetString(playerid, SpectactorsTD[playerid][SPECTACTOR_TEXTDRAW_INFO2], string);

	return 1;
}

CMD:specon(playerid, params[])
{
	new forplayerid;

	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(sscanf(params, "ddd", forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Wpisz: /specon [id gracza]");

	if(!IsPlayerConnected(forplayerid))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Gracz nie jest pod³¹czony.");

	if(GetPVarInt(forplayerid, "Przeglada"))
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Gracz ju¿ kogoœ przegl¹da.");

	if(forplayerid == playerid)
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie mo¿esz przegl¹daæ siebie.");

	if(GetPVarType(playerid, "Przeglada") == PLAYER_VARTYPE_NONE)
	{
		new Float:Pos[3];
		GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);

		SetPVarFloat(playerid, "specPosX", Pos[0]);
		SetPVarFloat(playerid, "specPosY", Pos[1]);
		SetPVarFloat(playerid, "specPosZ", Pos[2]);
	}

	SpectactorTextDraw(playerid, true);
	TogglePlayerSpectating(playerid, 1);

	if(IsPlayerInAnyVehicle(forplayerid))
		PlayerSpectateVehicle(playerid, GetPlayerVehicleID(forplayerid));
	else
		PlayerSpectatePlayer(playerid, forplayerid);

	SetPVarInt(playerid, "Przeglada", forplayerid);
	SetPVarInt(forplayerid, "jestPrzegladany", playerid);

	return 1;
}

CMD:specoff(playerid, params[])
{
	if(!playerInfo[playerid][pAdmin])
		return Msg(playerid, COLOR_ERROR, "Nie masz uprawnieñ.");

	if(GetPVarType(playerid, "Przeglada") == PLAYER_VARTYPE_NONE)
		return SendClientMessage(playerid, LIGHTRED, "{CC0000}>>{FFFFFF} Nie przegl¹dasz ¿adnego gracza.");

	TogglePlayerSpectating(playerid, 0);
	SpectactorTextDraw(playerid, false);

	return 1;
}
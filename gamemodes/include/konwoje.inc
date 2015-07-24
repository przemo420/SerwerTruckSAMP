#define MAX_CONVOYS 			25
#define MAX_CONVOY_MEMBERS 		20

new pConvoy[MAX_PLAYERS];
new pConvoyInvites[MAX_PLAYERS][MAX_CONVOYS];

enum E_CONVOY
{
	cId,
	cBoss,
	cMembers[MAX_CONVOY_MEMBERS],
	cPersons
}

new cInfo[MAX_CONVOYS][E_CONVOY];

forward Konwoje_OnPlayerConnect(playerid);
public Konwoje_OnPlayerConnect(playerid)
{
	for(new i = 1; i < MAX_CONVOYS; i++)
		pConvoyInvites[playerid][i] = 0;

	pConvoy[playerid] = -1;

	return 1;
}

forward Konwoje_OnPlayerDisconnect(playerid);
public Konwoje_OnPlayerDisconnect(playerid)
{
	if(!GetPVarInt(playerid, "CONVOY_Boss"))
		return 1;

	return DIALOG_CONVOY_MAIN_DEL(playerid, true, 0, "");
}

forward Konwoje_OnPlayerText(playerid, text[]);
public Konwoje_OnPlayerText(playerid, text[])
{
	if(strcmp(text, "!", true, 1) || pConvoy[playerid] < 0)
		return 0;

	new string[200];
	text[1] = toupper(text[1]);
	format(string, sizeof(string), "{FFFF00}!Konwój >> %s(id:%d): %s", PlayerName(playerid), playerid, text[1]);
		
	Loop(playeri, MAX_PLAYERS)
		if(pConvoy[playerid] == pConvoy[playeri])
			SendClientMessage(playeri, 0x0, string);

	return 1;
}

CMD:konwoj(playerid)
{
	new szString[128], IsBoss = GetPVarInt(playerid, "CONVOY_Boss");
	if(pConvoy[playerid] > -1)
	{
		if(IsBoss)
			format(szString, sizeof szString, "Konwój (ID: %d)\n \nUczestnicy konwoju\nZaproœ graczy (%d/%d)\nWyproœ gracza\nUsuñ konwój", pConvoy[playerid], cInfo[pConvoy[playerid]][cPersons], MAX_CONVOY_MEMBERS);
		else
			format(szString, sizeof szString, "Konwój\n \nUczestnicy konwoju (%d/%d)\nOpuœæ konwój", cInfo[pConvoy[playerid]][cPersons], MAX_CONVOY_MEMBERS);
	
		Dialog_Show(playerid, DIALOG_CONVOY_MAIN, DIALOG_STYLE_LIST, " ", szString, "Wybierz", "WyjdŸ");
	}
	else
	{
		Dialog_Show(playerid, DIALOG_CONVOY_CREATE, DIALOG_STYLE_MSGBOX, " ", "Tworzenie konwoju\nStworzenie konwoju jest darmowe. Konwój mo¿e maksymalnie pomieœciæ 20 osób.\nCzy chcesz stworzyæ konwój?", "Tak", "WyjdŸ");
	}
	return 1;
}

CMD:zaproszenia(playerid)
{
	if(pConvoy[playerid] > -1)
		return Msg(playerid, COLOR_ERROR, "Jesteœ ju¿ w konwoju.");
	new szString[256], count;
	for(new i = 0; i < MAX_CONVOYS; i++)
	{
		if(pConvoyInvites[playerid][i] != 0)
		{
			new szTemp[64];
			format(szTemp, sizeof szTemp, "Konwój gracza %s\n", PlayerName(cInfo[pConvoyInvites[playerid][i]][cBoss]));
			strcat(szString, szTemp);
			count++;
		}
	}
	if(count <= 0)
		format(szString, sizeof szString, "Brak zaproszeñ.");

	Dialog_Show(playerid, DIALOG_CONVOY_INV_LIST, DIALOG_STYLE_LIST, " ", szString, "Akceptuj", "WyjdŸ");
	return 1;
}

Dialog:DIALOG_CONVOY_INV_LIST(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new count;
	for(new i = 0; i < MAX_CONVOYS; i++)
		if(pConvoyInvites[playerid][i] > 0)
			count++;

	if(count <= 0)
	{
		if(listitem == 1)
			return cmd_zaproszenia(playerid);
	}
	else
	{
		for(new c = 0; c < MAX_CONVOYS; c++)
		{
			if(listitem == c)
			{
				if(pConvoyInvites[playerid][c] < 0)
					return Msg(playerid, COLOR_ERROR, "Zaproszenie straci³o wa¿noœæ.");
				
				new szTemp[64];
				pConvoy[playerid] = pConvoyInvites[playerid][c];
				format(szTemp, sizeof szTemp, "Pomyœlnie do³¹czono do konwoju gracza {b}%s{/b}.", PlayerName(cInfo[pConvoy[playerid]][cBoss]));
				Msg(playerid, COLOR_INFO, szTemp);

				format(szTemp, sizeof szTemp, "Gracz {b}%s{/b} do³¹czy³ do twojego konwoju.", PlayerName(playerid));
				Msg(cInfo[pConvoy[playerid]][cBoss], COLOR_INFO, szTemp);

				cInfo[pConvoy[playerid]][cPersons]++;
				break;
			}
		}
	}
	return 1;
}

Dialog:DIALOG_CONVOY_CREATE(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	new free = -1;
	for(new i = 0; i < MAX_CONVOYS; i++)
	{	
		if(cInfo[i][cPersons] == 0)
		{
			free = i;
			break;
		}
	}
	if(free < 0)
		return Msg(playerid, COLOR_ERROR, "Brak wolnych miejsc na konwój.");

	cInfo[free][cId] = free;
	cInfo[free][cBoss] = playerid;
	cInfo[free][cMembers][0] = playerid;
	cInfo[free][cPersons] = 1;
	SetPVarInt(playerid, "CONVOY_Boss", 1);
	pConvoy[playerid] = free;
	Msg(playerid, COLOR_INFO, "{b}Pomyœlnie{/b} utworzono konwój. Aby kogoœ zaprosiæ, ponownie u¿yj komendy {b}/konwoj{/b}.");
	return 1;
}

Dialog:DIALOG_CONVOY_MAIN(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;

	switch(listitem)
	{
		case 0 .. 1:
			return cmd_konwoj(playerid);
		case 2:
		{
			Dialog_Show(playerid, DIALOG_CONVOY_MAIN_LIST, DIALOG_STYLE_MSGBOX, " ", GetConvoyMembers(pConvoy[playerid]), "Wróæ", #);
		}
		case 3:
		{
			if(GetPVarInt(playerid, "CONVOY_Boss"))
			{
				if(cInfo[pConvoy[playerid]][cPersons] == MAX_CONVOY_MEMBERS)
				{
					cmd_konwoj(playerid);
					return Msg(playerid, COLOR_ERROR, "Nie mo¿esz dodaæ wiêcej osób do konwoju.");
				}

				Dialog_Show(playerid, DIALOG_CONVOY_MAIN_INVIT, DIALOG_STYLE_INPUT, " ", "WprowadŸ ID lub nick gracza którego chcesz zaprosiæ:", "Zaproœ", "Wstecz");
			}
			else
			{
				cInfo[pConvoy[playerid]][cPersons]--;
				pConvoy[playerid] = -1;
				Msg(playerid, COLOR_INFO, "Opuœci³eœ konwój.");
			}
		}
		case 4:
		{
			Dialog_Show(playerid, DIALOG_CONVOY_MAIN_KICK, DIALOG_STYLE_INPUT, " ", "WprowadŸ ID lub nick gracza którego chcesz wyprosiæ:", "Wyproœ", "Wstecz");
		}
		case 5:
		{
			Dialog_Show(playerid, DIALOG_CONVOY_MAIN_DEL, DIALOG_STYLE_MSGBOX, " ", "Czy aby na pewno chcesz usun¹æ konwój?", "Tak", "Wstecz");
		}
	}
	return 1;
}

Dialog:DIALOG_CONVOY_MAIN_LIST(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 1;
	else
		return cmd_konwoj(playerid);
}

Dialog:DIALOG_CONVOY_MAIN_INVIT(playerid, response, listitem, inputtext[])
{
	if(!response)
		return cmd_konwoj(playerid);

	new targetid = GetIDByNick(inputtext);
	if(targetid < 0)
	{
		return Dialog_Show(playerid, DIALOG_CONVOY_MAIN_INVIT, DIALOG_STYLE_INPUT, " ", "WprowadŸ ID lub nick gracza którego chcesz zaprosiæ:\nNie odnaleziono takiego gracza na serwerze...", "Zaproœ", "Wstecz");
	}
	if(pConvoy[targetid] > 0)
		return Dialog_Show(playerid, DIALOG_CONVOY_MAIN_INVIT, DIALOG_STYLE_INPUT, " ", "WprowadŸ ID lub nick gracza którego chcesz zaprosiæ:\nTen gracz uczestniczy w innym konwoju...", "Zaproœ", "Wstecz");

	new szTemp[128];
	format(szTemp, sizeof szTemp, "Pomyœlnie zaproszono gracza {b}%s{/b}.", PlayerName(targetid));
	Msg(playerid, COLOR_INFO, szTemp);

	format(szTemp, sizeof szTemp, "Zosta³es zaproszony do konwoju gracza {b}%s{/b}. Aby odpowiedzieæ na zg³oszenie wpisz /zaproszenia.", PlayerName(playerid));
	Msg(targetid, COLOR_INFO, szTemp);

	for(new i = 0; i < MAX_CONVOYS; i++)
	{
		if(pConvoyInvites[targetid][i] == 0)
		{
			pConvoyInvites[targetid][i] = pConvoy[playerid];
			break;
		}
	}
	return 1;
}

Dialog:DIALOG_CONVOY_MAIN_KICK(playerid, response, listitem, inputtext[])
{
	if(!response)
		return cmd_konwoj(playerid);

	new targetid = GetIDByNick(inputtext);
	if(targetid < 0)
	{
		return Dialog_Show(playerid, DIALOG_CONVOY_MAIN_KICK, DIALOG_STYLE_INPUT, " ", "WprowadŸ ID lub nick gracza którego chcesz wyprosiæ:\nNie odnaleziono takiego gracza na serwerze", "Wyproœ", "Wstecz");
	}
	if(pConvoy[targetid] != pConvoy[playerid])
		return Dialog_Show(playerid, DIALOG_CONVOY_MAIN_KICK, DIALOG_STYLE_INPUT, " ", "WprowadŸ ID lub nick gracza którego chcesz wyprosiæ:\nTen gracz nie uczestniczy w twoim konwoju...", "Zaproœ", "Wstecz");
	if(targetid == playerid)
		return Dialog_Show(playerid, DIALOG_CONVOY_MAIN_KICK, DIALOG_STYLE_INPUT, " ", "WprowadŸ ID lub nick gracza którego chcesz wyprosiæ:\nNie mo¿esz wyprosiæ samego siebie...", "Zaproœ", "Wstecz");

	new szTemp[128];
	format(szTemp, sizeof szTemp, "Gracz {b}%s{/b} zosta³ pomyœlnie usuniêty.", PlayerName(targetid));
	Msg(playerid, COLOR_INFO, szTemp);

	format(szTemp, sizeof szTemp, "Zosta³eœ {b}usuniêty{/b} z konwoju gracza {b}%s{/b}.", PlayerName(playerid));
	Msg(targetid, COLOR_INFO, szTemp);

	pConvoy[targetid] = -1;
	return 1;
}

Dialog:DIALOG_CONVOY_MAIN_DEL(playerid, response, listitem, inputtext[])
{
	if(!response)
		return cmd_konwoj(playerid);

	new konwojid = pConvoy[playerid];
	Msg(playerid, COLOR_ERROR, "Pomyœlnie usuniêto konwój.");
	DeletePVar(playerid, "CONVOY_Boss");

	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if(pConvoy[i] != konwojid)
			continue;

		if(i != playerid)
			Msg(cInfo[pConvoy[playerid]][cMembers][i], COLOR_ERROR, "Twój konwój zosta³ {b}usuniêty{/b} przez za³o¿yciela!");

		pConvoy[i] = -1;
		cInfo[pConvoy[playerid]][cMembers][i] = 0;

		for(new c = 0; c < MAX_CONVOYS; c++)
		{
			if(pConvoyInvites[i][c] == pConvoy[playerid])
			{
				pConvoyInvites[i][c] = 0;
				new szTemp[64];
				format(szTemp, sizeof szTemp, "Zaproszenie do konwoju gracza {b}%s{/b} zosta³o cofniête.", PlayerName(playerid));
				Msg(i, COLOR_INFO, szTemp);
			}
		}
	}

	cInfo[pConvoy[playerid]][cId] = 0;
	cInfo[pConvoy[playerid]][cBoss] = 0;
	cInfo[pConvoy[playerid]][cPersons] = 0;
	return 1;
}

stock GetConvoyMembers(convoyId)
{
	new szString[540];
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if(IsPlayerConnected(i) && pConvoy[i] == convoyId)
		{
			strcat(szString, PlayerName(i));
			if(i == cInfo[convoyId][cBoss])
				strcat(szString, " (za³o¿yciel)");
			strcat(szString, "\n");
		}
	}
	return szString;
}
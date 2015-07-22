enum
{
	ADR_Level_0, // brak licencji
	ADR_Level_1, // paliwa
	ADR_Level_2, // gazy
	ADR_Level_3  // ca³a reszta
}

// Klasa licencji | Numer pytania | {Pytanie - OdpowiedŸ 1 - OdpowiedŸ 2 - OdpowiedŸ 3} | Prawid³owa odpowiedŸ

new ADR_ReqLvl[] = {
	1, 5, 10
};

new ADR_Cost[] = {
	6000, 4000, 3000
};

new ADR_QuestNumb[] = {
	5, 5, 3
};

new ADRQuestions[][][] = {
	{ADR_Level_1, 1, "{488CDB}1) W jakim miejscu nale¿y zatrzymywaæ siê z towarem ³atwopalnym w celu zrobienia przerwy?{FFFFFF}\n \na) a poboczu drogi\nb) na specjalnym parkingu\nc) gdziekolwiek", 2},
	{ADR_Level_1, 2, "{488CDB}2) Czy mo¿na zostawiaæ za³adowany pojazd bez nadzoru?{FFFFFF}\n \na) tak\nb) nie\nc) nie wiem", 2},
	{ADR_Level_1, 3, "{488CDB}3) Czym jest licencja ADR i na co pozwala?{FFFFFF}\n \na) s¹ to uprawnienia do przewo¿enia towarów ³atwopalnych etc.\nb) s¹ to uprawnienia do przewo¿enia wartoœci pieniê¿nych\nc) nie s¹ to ¿adne specjalne uprawnienia i nic one nie daj¹", 1},
	{ADR_Level_1, 4, "{488CDB}4) Towar niebezpieczny wed³ug licencji ADR to:{FFFFFF}\n \na) ka¿dy towar przewo¿ony w cysternie\nb) ka¿dy ³adunek przewo¿ony bez opakowania\nc) ka¿da substancja zakwalifikowana do którejœ z klas niebezpieczeñstwa", 3},
	{ADR_Level_1, 5, "{488CDB}5) Które z poni¿szych zagro¿eñ stwarzanych przez towary ³atwopalne uznaje siê za najgroŸniejsze?{FFFFFF}\n \na) utlenianie\nb) palnoœæ\nc) w³aœciwoœci ¿r¹ce", 2},
	{ADR_Level_2, 1, "{488CDB}1) Przed wyruszeniem w drogê z nape³nion¹ cystern¹ kierowca musi:{FFFFFF}\n \na) odpocz¹æ przez pewien czas\nb) zatankowaæ ci¹gnik do pe³na\nc) sprawdziæ czy naczepa jest bezpieczna", 3},
	{ADR_Level_2, 2, "{488CDB}2) Co grozi pojazdom obok podczas wypadku z pe³n¹ cystern¹?{FFFFFF}\n \na) potencjalne podpalenie\nb) wy³¹czenie silnika\nc) zgaszenie siê œwiate³", 1},
	{ADR_Level_2, 3, "{488CDB}3) Silne poruszanie siê substancji w zbiorniku cysterny powodowane jest:{FFFFFF}\n \na) przez du¿¹ prêdkoœæ\nb) przez ostre hamowanie\nc) przez jazdê pod górkê", 2},
	{ADR_Level_2, 4, "{488CDB}4) Co nale¿y zrobiæ po odbyciu kolizji z innym pojazdem maj¹c za³adowany towar?{FFFFFF}\n \na) wezwaæ pomoc drogow¹\nb) wezwaæ pogotowie\nc) wezwaæ policjê", 1},
	{ADR_Level_2, 5, "{488CDB}5) Dlaczego uderzenie lub tarcie jest niebezpieczne dla towarów wybuchowych?{FFFFFF}\n \na) poniewa¿ naczepa jest zbyt s³aba\nb) poniewa¿ zawsze w ich wyniku roœnie ciœnienie\nc) poniewa¿ mog¹ one zainicjowaæ ich wybuch", 3},
	{ADR_Level_3, 1, "{488CDB}1) Towarami wybuchowymi inicjuj¹cymi s¹:{FFFFFF}\n \na) towary wybuchowe, które s¹ stosowane do wytwarzania innych towarów wybuchowych\nb) towary najbardziej wra¿liwe\nc) towary zawieraj¹ce czarny proch", 2},
	{ADR_Level_3, 2, "{488CDB}2) W celu ochrony przed kradzie¿¹ pojazd przewo¿¹cy towary wybuchowe mo¿e byæ zaparkowany:{FFFFFF}\n \na) jedynie w obszarze zabudowanym\nb) powinien byæ konwojowany przez policjê\nc) powinien byæ zamykany na klucz", 3},
	{ADR_Level_3, 3, "{488CDB}3) Jakimi drogami nie powinno siê poruszaæ z pe³n¹ cystern¹?{FFFFFF}\n \na) drogami bez odpowiedniej nawierzchni\nb) autostradami\nc) drogami w mieœcie", 1}
};

CMD:adr(playerid)
{
	new class = 0, levelReq, szBuffer[256], szForm[5];
	switch(PlayerInfo[playerid][pADR])
	{
		case 0: class = 1;
		case 1: class = 2;
		case 2: class = 3;
		case 3: return Msg(playerid, COLOR_ERROR, "Posiadasz ju¿ wszystkie klasy licencji ADR.");
	}
	levelReq = ADR_ReqLvl[class - 1];
	if(GetPlayerLevel(pInfo[playerid][pExp]) < levelReq)
		return Msg(playerid, COLOR_ERROR, "Nie spe³niasz wymagañ aby móc zdawaæ testy na licencjê nastêpnej klasy.");
	if(GetPlayerMoney(playerid) < ADR_Cost[class -1])
	{
		format(szBuffer, sizeof szBuffer, "Nie posiadasz wystarczajaco du¿o gotówki. Aby móc zdawaæ testy potrzebujesz $%d.", ADR_Cost[class -1]);
		return Msg(playerid, COLOR_ERROR, szBuffer);
	}

	szForm = (class == 1) ? ("szej") : ((class == 2) ? ("giej") : ("ciej"));

	format(szBuffer, sizeof szBuffer, "{488CDB}Licencja ADR\n \n{FFFFFF}Czy jesteœ pewien, ¿e chcesz przyst¹piæ do testu na licencjê ADR klasy %d-%s?\n \nW przypadku oblania testów koszta ($%d) nie zostan¹ zwrócone\nW zale¿noœci od klasy test sk³ada siê z od 3 do 5 pytañ.\nPowodzenia. :)", class, szForm, ADR_Cost[class - 1]);
	ShowPlayerDialog(playerid, DIALOG_ADR, DIALOG_STYLE_MSGBOX, " ", szBuffer, "Rozumiem", "Anuluj");
	return 1;
}

/*
switch(dialogid)
{
	case DIALOG_ADR:
	{
		if(response)
		{
			new questNumb = PlayerInfo[playerid][pADR] * 5;

			GivePlayerMoney(playerid, -ADR_Cost[PlayerInfo[playerid][pADR]]);
			ShowPlayerDialog(playerid, DIALOG_ADR2, DIALOG_STYLE_INPUT, " ", ADRQuestions[questNumb][2], "Dalej", #);
			SetPVarInt(playerid, "ADR_Step", 0);
			SetPVarInt(playerid, "ADR_Balance", 0);
		}
		else
			return 1;
	}
	case DIALOG_ADR2:
	{
		new step = GetPVarInt(playerid, "ADR_Step"), questNumb = (PlayerInfo[playerid][pADR] * 5) + step;
		if(response)
		{
			new answer, balance;
			
			if(!isnull(inputtext) && !strcmp(inputtext[0], "a", true) || !strcmp(inputtext[0], "b", true) || !strcmp(inputtext[0], "c", true))
			{
				switch(inputtext[0])
				{
					case 'a': answer = 1;
					case 'b': answer = 2;
					case 'c': answer = 3;

					case 'A': answer = 1;
					case 'B': answer = 2;
					case 'C': answer = 3;
				}
				balance = GetPVarInt(playerid, "ADR_Balance");
				if(ADRQuestions[questNumb][3][0] == answer)
					balance++;
				SetPVarInt(playerid, "ADR_Balance", balance);
				if(ADR_QuestNumb[PlayerInfo[playerid][pADR]] == step + 1)
				{
					new szBuffer[128], szForm[5];
					szForm = (PlayerInfo[playerid][pADR]+1 == 1) ? ("szej") : ((PlayerInfo[playerid][pADR]+1 == 2) ? ("giej") : ("ciej"));
					if(GetPVarInt(playerid, "ADR_Balance") >= (ADR_QuestNumb[PlayerInfo[playerid][pADR]]-1))
					{
						PlayerInfo[playerid][pADR] += 1;
						format(szBuffer, sizeof szBuffer, "Gracz {b}%s{/b} zdoby³ licencjê ADR {b}%d{/b}-{b}%s{/b} klasy!", GetPlayerNick(playerid), PlayerInfo[playerid][pADR], szForm);
						MsgToAll(COLOR_INFO2, szBuffer);
						format(szBuffer, sizeof szBuffer, "{488CDB}Gratulacje!{FFFFFF}\n \nUda³o Ci siê zaliczyæ testy na licencje ADR klasy {488CDB}%d-%s.", PlayerInfo[playerid][pADR], szForm);
					}
					else
					{
						format(szBuffer, sizeof szBuffer, "{488CDB}Niestety...{FFFFFF}\n \nNie uda³o Ci siê zaliczyæ testów na licencje ADR klasy {488CDB}%d-%s.\n", PlayerInfo[playerid][pADR]+1, szForm);
					}

					DeletePVar(playerid, "ADR_Step");
					DeletePVar(playerid, "ADR_Balance");
					ShowPlayerDialog(playerid, DIALOG_ADR3, DIALOG_STYLE_MSGBOX, " ", szBuffer, "Rozumiem", #);
					return 1;
				}
				else
					ShowPlayerDialog(playerid, DIALOG_ADR2, DIALOG_STYLE_INPUT, " ", ADRQuestions[questNumb + 1][2], "Dalej", #),
					step++,
					SetPVarInt(playerid, "ADR_Step", step);
			}
			else
				ShowPlayerDialog(playerid, DIALOG_ADR2, DIALOG_STYLE_INPUT, " ", ADRQuestions[questNumb][2], "Dalej", #);
		}
		else
			ShowPlayerDialog(playerid, DIALOG_ADR2, DIALOG_STYLE_INPUT, " ", ADRQuestions[questNumb][2], "Dalej", #);
	}
	case DIALOG_ADR3:
	{
		return 1;
	}
}*/
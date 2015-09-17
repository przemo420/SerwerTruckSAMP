#include "a_samp"
#include "include/lib/geoip.inc"
#include "zcmd"

CMD:myloc(playerid)
{
	new szTemp[128];
	GetPlayerCountry(playerid, szTemp, sizeof szTemp);
	SendClientMessage(playerid, -1, szTemp);
	GetPlayerCity(playerid, szTemp, sizeof szTemp);
	SendClientMessage(playerid, -1, szTemp);
	return 1;
}
/*
	~~Fire System~~
		by Kaz
	Dostosowa³ GeDox
*/
#if !defined MAX_FIRE
	#define MAX_FIRE 20
#endif
#define FIRE_TIMER 500

// USTAWIENIA

#define TXT_TEXT "%d | %d/20%",id, Fire[id][Health] // Ogólne ustawienia 3d tekstu (id - id po¿aru, health - ¿ycie po¿aru) - mo¿na tutaj kolorowaæ i takie tam

enum _fire
{
	bool:use,
	object,
	Float:fX,
	Float:fY,
	Float:fZ,
	Text3D:info,
	fMapIcon,
	Health
}
new Fire[MAX_FIRE][_fire];
new _firestr[10];

new const FireProbability[] = {
	80,
	60,
	65,
	50,
	20,
	60,
	80,
	20,
	0,
	15,
	60,
	80,
	10,
	80,
	60,
	20,
	0,
	80,
	60
};

new const Float:RandomFires[][] =
{
    {2780.413330,-1220.205444,49.229999},
    {2668.413330,-828.205444,64.699996},
	{2234.413330,-856.205444,112.680000},
	{1982.413330,-954.205444,55.840000},
    {1842.413330,-926.205444,52.889999},
    {1912.413330,-1080.205444,23.219999},
    {2024.413330,-1206.205444,20.359998},
    {2304.413330,-1122.205444,31.939998},
    {2444.413330,-1136.205444,33.630001},
    {2486.413330,-1678.205444,12.329999},
    {2276.413330,-1594.205444,2.509999},
    {1842.413330,-1412.205444,12.389999},
    {2682.413330,-1846.205444,8.439999},
    {1562.413330,-1930.205444,25.309999},
    {1137.413330,-2014.205444,68.000000},
    {969.413330,-1888.205444,6.500000},
    {871.413330,-1874.205444,5.109999},
    {213.413330,-1832.205444,2.839999},
    {-108.586669,-1519.205444,1.889999},
    {-164.586669,-1351.205444,2.069999},
    {-542.586669,-1677.205444,23.829999},
    {-836.586669,-556.205444,38.239997},
    {-216.586669,27.794555,2.109999},
    {831.413330,298.794555,21.090000},
    {1154.413330,750.794555,9.819999},
    {2672.413330,2535.794433,9.840000},
    {1213.413330,2493.794433,9.359999},
    {-2735.586669,2411.794433,73.360000},
    {-2021.586669,665.794433,43.829998},
    {-2735.586669,763.794433,62.079998},
    {-2105.586669,265.794433,34.680000},
    {-2693.586669,149.794433,3.319999}
};

stock FireSystem_Init(){
	SetTimer("FireUpdate", FIRE_TIMER, true);
	return 1;
}

forward RandomFire();
public RandomFire()
{
	if(random(100) <= FireProbability[GetWeather()])
	{
        new r = random(sizeof(RandomFires));
        AddFire(RandomFires[r][0], RandomFires[r][1], RandomFires[r][2], 80+random(10));
        return 1;
    }
}

forward FireUpdate();
public FireUpdate(){
	for(new playerid; playerid < GetMaxPlayers(); playerid++){
		new id = GetClosestFire(playerid);
		if(id == -1) continue;
		if(GetPlayerWeapon(playerid) == 42 || (GetVehicleModel(GetPlayerVehicleID(playerid)) == 407 && GetPlayerVehicleSeat(playerid) == 0)){
			new newkeys,l,u;
			GetPlayerKeys(playerid, newkeys, l, u);
			if(HOLDING(KEY_FIRE)){	
				if(IsPlayerLookingAtPoint(playerid, Fire[id][fX], Fire[id][fY], Fire[id][fZ])){
					if(Fire[id][Health] != 0){
						Fire[id][Health]-=1;
						format(_firestr, sizeof(_firestr),TXT_TEXT);
						Update3DTextLabelText(Fire[id][info], 0xFFFFFFFFF, _firestr);
						CallRemoteFunction("OnPlayerPutOutFire", "ddd", playerid, id, false);
					}
					else {
						DeleteFire(id);
						CallRemoteFunction("OnPlayerPutOutFire", "ddd", playerid, id, true);
					}
				}
			}
		}
	}
	return 1;
}

stock AddFire(Float:X, Float:Y, Float:Z, wielkosc){
	new id = GetFreeFireID();
	if(id == -1) 
		return -1;
	Fire[id][use] = true;
	Fire[id][fX] = X;
	Fire[id][fY] = Y;
	Fire[id][fZ] = Z;
	Fire[id][Health] = wielkosc;
	Fire[id][object] = CreateObject(3461, X, Y, Z-2.61, 0, 0, 0.0);
	format(_firestr, sizeof(_firestr),TXT_TEXT);
	Fire[id][info] = Create3DTextLabel(_firestr, 0xFFFFFFFFF, X, Y, Z, 20, 0);
	Fire[id][fMapIcon] = CreateDynamicMapIcon(X, Y, Z, 20, 0xFFFFFFFFF, _, _, _, 6000.0, MAPICON_GLOBAL);
	CallRemoteFunction("OnFireAdd", "d", id);
	return id;
}

stock DeleteFire(id){
	CallRemoteFunction("OnFireDelete", "d", id);
	Fire[id][use] = false;
	Fire[id][fX] = 0.0;
	Fire[id][fY] = 0.0;
	Fire[id][fZ] = 0.0;
	Fire[id][Health] = 0;
	DestroyObject(Fire[id][object]);
	Fire[id][object] = -1;
	Delete3DTextLabel(Fire[id][info]);
	DestroyDynamicMapIcon(Fire[id][fMapIcon]);
	return 1;
}

stock GetClosestFire(playerid){
	for(new i; i < MAX_FIRE; i++){
		if(Fire[i][use] == true && IsPlayerInRangeOfPoint(playerid, 1, Fire[i][fX],  Fire[i][fY],  Fire[i][fZ])){
			CallRemoteFunction("OnPlayerNearFire", "dd",playerid, i);
			return i;
		}
	}
	return -1;
}		
stock GetFreeFireID(){
	for(new i; i < MAX_FIRE; i++){
		if(Fire[i][use] == false){
			return i;
		}
	}
	return -1;
}

stock IsPlayerLookingAtPoint(playerid,Float:X,Float:Y,Float:Z,Float:ViewWidth=22.5,Float:ViewHeight=10.0)
{
    new Float:cx,Float:cy,Float:cz,Float:pa1,Float:pa2,Float:ca1,Float:ca2,Float:px,Float:py,Float:pz;
    GetPlayerCameraFrontVector(playerid,cx,cy,cz);
    GetPlayerPos(playerid,px,py,pz);
    pz+=2.0;
    cx=floatadd(cx,px);
    cy=floatadd(cy,py);
    cz=floatadd(cz,pz);
    pa1=atan2(X-px,Y-py);
    if(pa1>360)pa1=floatsub(pa1,360);
    if(pa1<0)pa1=floatadd(pa1,360);
    pa2=atan2(Y-py,Z-pz);
    if(pa2>360)pa2=floatsub(pa2,360);
    if(pa2<0)pa2=floatadd(pa2,360);
    ca1=atan2(cx-px,cy-py);
    if(ca1>360)ca1=floatsub(ca1,360);
    if(ca1<0)ca1=floatadd(ca1,360);
    ca2=atan2(cy-py,cz-pz);
    if(ca2>360)ca2=floatsub(ca2,360);
    if(ca2<0)ca2=floatadd(ca2,360);
	if((ca1>(pa1-ViewWidth))&&(ca1<(pa1+ViewWidth))&&(ca2>(pa2-ViewHeight))&&(ca2<(pa2+ViewHeight)))return 1;
    if((pa1-ViewWidth)<0)
    {
        ca1-=360.0;
        if((ca1>(pa1-ViewWidth))&&(ca1<(pa1+ViewWidth))&&(ca2>(pa2-ViewHeight))&&(ca2<(pa2+ViewHeight)))return 1;
    }
    if((pa1+ViewWidth)>360)
    {
        ca1+=360.0;
        if((ca1>(pa1-ViewWidth))&&(ca1<(pa1+ViewWidth))&&(ca2>(pa2-ViewHeight))&&(ca2<(pa2+ViewHeight)))return 1;
    }
    return 0;
}
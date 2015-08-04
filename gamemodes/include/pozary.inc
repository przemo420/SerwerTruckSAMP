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

stock FireSystem_Init(){
	SetTimer("FireUpdate", FIRE_TIMER, true);
	return 1;
}

#define HOLDING(%0) \
	((newkeys & (%0)) == (%0))	
	
forward FireUpdate();
public FireUpdate(){
	for(new playerid; playerid < GetMaxPlayers(); playerid++){
		new id = GetClosestFire(playerid);
		if(id == -1) continue;
		if(GetPlayerWeapon(playerid) == 42){
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

stock AddFire(Float:X, Float:Y, Float:Z){
	new id = GetFreeFireID();
	if(id == -1) 
		return -1;
	Fire[id][use] = true;
	Fire[id][fX] = X;
	Fire[id][fY] = Y;
	Fire[id][fZ] = Z;
	Fire[id][Health] = 10+random(10);
	Fire[id][object] = CreateObject(3461, X, Y, Z-2.61, 0, 0, 0.0);
	format(_firestr, sizeof(_firestr),TXT_TEXT);
	Fire[id][info] = Create3DTextLabel(_firestr, 0xFFFFFFFFF, X, Y, Z, 20, 0);
	Fire[id][fMapIcon] = CreateDynamicMapIcon(X, Y, Z, type, 0xFFFFFFFFF, _, _, _, 6000.0, MAPICON_GLOBAL);
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
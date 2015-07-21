/*
 *  Fast Dialog Processing System
 *
 *  Version 1.5
 *
 *  By Gamer_Z
 *
 */

//#define FDLG_NO_DISABLE_CALLBACK
//#define FDLG_USE_YSI_HOOK
//#define FDLG_ALLOW_NEGATIVE_CALLBACKS

#include <a_samp>

#if defined _FAST_DIALOG_INCLUDED_
	#endinput
#else
	#define _FAST_DIALOG_INCLUDED_
#endif

#if !defined isnull
	#define isnull(%1) \
				((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif

#define FDLG_INCLUDE_VERSION (15)
 //Own Definition Dialog
//Usage:: DIALOG:dialogid(playerid,response,listitem,inputtext[])
#define DIALOG:%1(%2,%3,%4,%5) \
	forward GDialog_%1(%2,%3,%4,%5);\
	public GDialog_%1(%2,%3,%4,%5)

 //FastDialog
//Usage:: fDialog(dialogid)
#define fDialog(%1) \
	forward GDialog_%1(playerid,response,listitem,inputtext[]);\
	public GDialog_%1(playerid,response,listitem,inputtext[])
	
#if defined FDLG_ALLOW_NEGATIVE_CALLBACKS

	#define fDialog_m(%1) \
		forward GDialogm_%1(playerid,response,listitem,inputtext[]);\
		public GDialogm_%1(playerid,response,listitem,inputtext[])
		
	#define DIALOG_m:%1(%2,%3,%4,%5) \
		forward GDialogm_%1(%2,%3,%4,%5);\
		public GDialogm_%1(%2,%3,%4,%5)

#endif

new g__FDLG_PRIV_CALLBACK[24];
#if defined FDLG_USE_YSI_HOOK
	#include <YSI\y_hooks>

	#if defined FDLG_NO_DISABLE_CALLBACK
		Hook:FastDialog_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
		{
		    #if defined FDLG_ALLOW_NEGATIVE_CALLBACKS
				if(dialogid >= 0)format(g__FDLG_PRIV_CALLBACK,24,"GDialog_%d",dialogid);
				else format(g__FDLG_PRIV_CALLBACK,24,"GDialogm_%d",(dialogid*(-1)));
			#else
				format(g__FDLG_PRIV_CALLBACK,24,"GDialog_%d",dialogid);
			#endif
			if(isnull(inputtext))inputtext[0] = 1;
			if(funcidx(g__FDLG_PRIV_CALLBACK) != -1)
			{
				return CallLocalFunction(g__FDLG_PRIV_CALLBACK,"iiis",playerid,response,listitem,inputtext);
			}
			return 1;
		}
	#else
		Hook:FastDialog_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
		{
		    #if defined FDLG_ALLOW_NEGATIVE_CALLBACKS
				if(dialogid >= 0)format(g__FDLG_PRIV_CALLBACK,24,"GDialog_%d",dialogid);
				else format(g__FDLG_PRIV_CALLBACK,24,"GDialogm_%d",(dialogid*(-1)));
			#else
				format(g__FDLG_PRIV_CALLBACK,24,"GDialog_%d",dialogid);
			#endif
			if(isnull(inputtext))inputtext[0] = 1;
			CallLocalFunction(g__FDLG_PRIV_CALLBACK,"iiis",playerid,response,listitem,inputtext);
			return -1;
		}
	#endif
#else
	#if defined FDLG_NO_DISABLE_CALLBACK
		public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
		{
		    #if defined FDLG_ALLOW_NEGATIVE_CALLBACKS
				if(dialogid >= 0)format(g__FDLG_PRIV_CALLBACK,24,"GDialog_%d",dialogid);
				else format(g__FDLG_PRIV_CALLBACK,24,"GDialogm_%d",(dialogid*(-1)));
			#else
				format(g__FDLG_PRIV_CALLBACK,24,"GDialog_%d",dialogid);
			#endif
			if(isnull(inputtext))inputtext[0] = 1;
			if(funcidx(g__FDLG_PRIV_CALLBACK) != -1)
			{
				return CallLocalFunction(g__FDLG_PRIV_CALLBACK,"iiis",playerid,response,listitem,inputtext);
			}
			return CallLocalFunction("FDLG_OnDialogResponse","iiiis",playerid,dialogid,response,listitem,inputtext);
		}
	#else
		public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
		{
		    #if defined FDLG_ALLOW_NEGATIVE_CALLBACKS
				if(dialogid >= 0)format(g__FDLG_PRIV_CALLBACK,24,"GDialog_%d",dialogid);
				else format(g__FDLG_PRIV_CALLBACK,24,"GDialogm_%d",(dialogid*(-1)));
			#else
				format(g__FDLG_PRIV_CALLBACK,24,"GDialog_%d",dialogid);
			#endif
			if(isnull(inputtext))inputtext[0] = 1;
			return CallLocalFunction(g__FDLG_PRIV_CALLBACK,"iiis",playerid,response,listitem,inputtext);
		}
	#endif
	#if defined _ALS_OnDialogResponse
		#undef OnDialogResponse
	#else
		#define _ALS_OnDialogResponse
	#endif
	#define OnDialogResponse FDLG_OnDialogResponse
    forward FDLG_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
    
#include <a_http>

/* below is the plugin callback support code, you really shouldn't touch this */

#if defined FILTERSCRIPT
public OnFilterScriptInit()
{
	privGPB_AddUpdatecheck("FDLG",1,FDLG_INCLUDE_VERSION);
	return CallLocalFunction("call_fdlg_OnFilterScriptInit","");
}

#if defined _ALS_OnFilterScriptInit
	#undef OnFilterScriptInit
#else
	#define _ALS_OnFilterScriptInit
#endif

#define OnFilterScriptInit call_fdlg_OnFilterScriptInit
forward call_fdlg_OnFilterScriptInit();

#else
public OnGameModeInit()
{
    privGPB_AddUpdatecheck("FDLG",1,FDLG_INCLUDE_VERSION);
	return CallLocalFunction("call_fdlg_OnGameModeInit","");
}

#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit call_fdlg_OnGameModeInit
forward call_fdlg_OnGameModeInit();

#endif
    #include <GPBMAIN>
#endif

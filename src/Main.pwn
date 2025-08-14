#include <a_samp>
#include <sscanf2>
#include <YSI_Coding\y_timers>
#include <YSI_Coding\y_hooks>
#include <YSI_Visual\y_commands>

#include "Utils/Global"
#include "Modules/VoiceCmds"

#pragma unused GetName

@timer(100) NewIncomingConnection(playerid)
{
	SetSpawnInfo(playerid,NO_TEAM,299,1759.0189,-1898.1260,13.5622,266.4503,0,0,0,0,0,0);
	TogglePlayerSpectating(playerid,false);
	TogglePlayerControllable(playerid,true);
	CallRemoteFunction("OnPlayerLogged", "i", playerid);
    return 1;
}

@cmd() gmx(playerid, params[], help)
{
	SendRconCommand("gmx");
    return 1;
}

main() {}

public OnGameModeInit()
{
	SetGameModeText("Hello World!");
	return 1;
}

public OnPlayerRequestClass(playerid,classid)
{
    TogglePlayerSpectating(playerid,true);
    defer NewIncomingConnection(playerid);
	return 1;
}
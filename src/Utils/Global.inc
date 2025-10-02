#include <YSI_Coding\y_hooks>

#define forex(%0,%1) for(new %0 = 0; %0 < %1; %0++)
#define IsOdd(%0) ((%0)&1)
#define Pressed(%0) ((newkeys & %0) && !(oldkeys & %0))
#define Holding(%0) ((newkeys & (%0)) == (%0))
#define Pressing(%0,%1) (%0 & (%1))
#define Released(%0) (((newkeys & (%0)) != (%0)) && ((oldkeys & (%0)) == (%0)))

#define GetVehicleModelName(%0)	VehicleData[((%0)-400)][Name]
#define SendErrorMessage(%0,%1) SendClientMessage(%0,0xBFC0C200,%1)
#define SendPrivilegeMessage(%0) SendClientMessage(%0,0xBFC0C200,"ERROR: You don't have the privilege to use this command!")

new MySQL:Database;
new RaceCheck[MAX_PLAYERS];
new bool:PlayerLogged[MAX_PLAYERS];
new GMT	= 25200; // GMT+7 Indonesia WIB

ptask pTask1s[1000](playerid)
	return 1;

ptask pTask100ms[100](playerid) 
	return 1;

@timer(2000) UnlockServer()
{
	SendRconCommand("password 0");
	return 1;
}

@timer(100) OnPlayerUpdateEx(playerid)
{
	// OnPlayerUpdate 100/ms
	return 1;
}

@timer(100) KickPlayer(playerid)
{
	Kick(playerid);
	return 1;
}

GetName(playerid)
{
	new playername[20];
	GetPlayerName(playerid,playername,sizeof(playername));
	return playername;
}

GetTimeDate(timestamp)
{
    new Timestamp:ts = Timestamp:(timestamp+GMT), string[256];
    TimeFormat(ts,"%a %d %b %Y, %T",string);
    return string;
}

GetPlayerAdmin(playerid)
{
	if (PlayerAccountInfo[playerid][AdminLevel] < 1) {
		return 0;
	} else if (PlayerAccountInfo[playerid][AdminLevel] >= 1) {
		return PlayerAccountInfo[playerid][AdminLevel];
	}
	return 0;
}

IsRoleplayName(const nickname[])
{
    static Regex:regex;
    if(!regex) regex = Regex_New("[A-Z][a-z]+_[A-Z][a-z]+");
    return Regex_Check(nickname, regex);
}

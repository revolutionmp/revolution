#include <YSI_Coding\y_hooks>

@cmd(.alts = {"p"}) call(playerid, params[], help)
{
	if(sscanf(params,"i",params[0]))
		return SendClientMessage(playerid, -1,"USAGE: /call [playerid]");
    CallRemoteFunction("StartPhoneCall", "ii", playerid, params[0]);
    return 1;
}

@cmd(.alts = {"h"}) hangup(playerid, params[], help)
{
    CallRemoteFunction("EndPhoneCall", "i", playerid);
    return 1;
}

@cmd(.alts = {"tg"}) togradio(playerid, params[], help)
{
    CallRemoteFunction("SetRadioToggle", "i", playerid);
    return 1;
}

@cmd(.alts = {"fq"}) freq(playerid, params[], help)
{
    if(sscanf(params,"i",params[0]))
		return SendClientMessage(playerid, -1,"USAGE: /freq [1-9]");
    CallRemoteFunction("SetRadioFreq", "ii", playerid, params[0]);
    return 1;
}

@hook(.fallback = true) OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(Pressing(newkeys, KEY_NO))
        CallRemoteFunction("SetProximityMode", "i", playerid);
    return 1;
}
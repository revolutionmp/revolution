@cmd() savestatic(playerid, params[], help)
{
    extract params -> vehicleid; else return SendErrorMessage(playerid, "USAGE: /savestatic [vehicleid]");
    if (!IsValidVehicle(vehicleid)) return SendErrorMessage(playerid, "ERROR: invalid vehicle id");
    SaveStaticVehicle(vehicleid);
    return 1;
}

@cmd() aveh(playerid, params[], help)
{
    new ownerid, modelid, Float:pos[4];
    if (sscanf(params, "ud", ownerid, modelid)) return SendErrorMessage(playerid, "USAGE: /createpveh [owner] [model]");
    GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
    GetPlayerFacingAngle(playerid, pos[3]);
    CreatePlayerVehicle(ownerid, modelid, 0, 0, pos[0], pos[1], pos[2], pos[3], 0, 0, false);
    return 1;
}

@cmd() asveh(playerid, params[], help)
{
    new ownerid, modelid, Float:pos[4];
    if (sscanf(params, "ud", ownerid, modelid)) return SendErrorMessage(playerid, "USAGE: /createpveh [model]");
    GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
    GetPlayerFacingAngle(playerid, pos[3]);
    CreateStaticVehicle(modelid, 0, 0, pos[0], pos[1], pos[2], pos[3], 0, 0, false);
    return 1;
}

@cmd() adveh(playerid, params[], help)
{
    new ownerid, modelid, Float:pos[4];
    if (sscanf(params, "ud", ownerid, modelid)) return SendErrorMessage(playerid, "USAGE: /adveh [owner] [model]");
    GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
    GetPlayerFacingAngle(playerid, pos[3]);
    CreateOnDemandVehicle(ownerid, modelid, 0, 0, pos[0], pos[1], pos[2], pos[3], 0, 0, false);
    return 1;
}

@cmd() dspawn(playerid, params[], help)
{
    extract params -> vehicleid; else return SendErrorMessage(playerid, "USAGE: /dspawn [vid]");
    UnloadVehicle(vehicleid);
    return 1;
}

@cmd() netstats(playerid, params[], help)
{
    new server_stats[456];
    GetNetworkStats(server_stats, sizeof(server_stats));
    ShowPlayerDialog(playerid, 99999, DIALOG_STYLE_MSGBOX, "Server Network Stats", server_stats, "Close", "");
    return 1;
}

@cmd(.alts = {"cc"}) clearchat(playerid, params[], help)
{
    new value_clear;
    sscanf(params, "D(20)", value_clear);
        
    forex(i, value_clear)
        SendClientMessage(playerid, -1, " ");
        
    return 1;
}

@cmd() shownametags(playerid, params[], help)
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        ShowPlayerNameTagForPlayer(playerid, i, true);
    }
    return 1;
}

@cmd() hidenametags(playerid, params[], help)
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        ShowPlayerNameTagForPlayer(playerid, i, false);
    }
    return 1;
}

@cmd() online(playerid, params[], help)
{
    new cnt = 0,
        player = 0
    ;
    for (; player < GetMaxPlayers(); player++)
    {
        if (IsPlayerConnected(player)) {
            cnt++;
        }
    }
    if (cnt >= 1) {
        new str[4];
        format(str, sizeof(str), "Online player: %d", cnt);
        SendClientMessage(playerid, -1, str); 
    }
    return 1;
}

@cmd() time(playerid, params[], help)
{
    new str[26],
        hh,
        mm,
        _hh,
        _mm,
        _ss
    ;
    
    GetPlayerTime(playerid, hh, mm);
    format(str, 26, "Your time: %02d:%02d", hh, mm);
    SendClientMessage(playerid, -1, str);
    gettime(_hh, _mm, _ss);
    format(str, 26, "Server time: %02d:%02d:%02d", _hh, _mm, _ss);
    SendClientMessage(playerid, -1, str);

    return 1;
}

@cmd() pm(playerid, params[], help)
{
    new targetid,
        text_pm[128]
    ;
    if (sscanf(params, "us[128]", targetid, text_pm))
        return SendClientMessage(playerid, -1, "/pm <target> [text]");

    if (strlen(text_pm) > 128) {
        return SendClientMessage(playerid, -1, "{FF0000}ERROR: Your text is very long. maximum 128!");
    }
    if (!IsPlayerConnected(targetid)) {
        return SendClientMessage(playerid, -1, "{FF0000}ERROR: Player not connected!");
    }
    if (targetid == playerid) {
        return SendClientMessage(playerid, -1, "{FF0000}ERROR: You can't PM yourself!");
    }

    new str[144];
    format(str, 144, "{FFF070}PM from %s (%d): %s", GetName(playerid), playerid, text_pm);
    SendClientMessage(targetid, -1, str);
    SendClientMessage(playerid, -1, "Your message has been send..");

    return 1;
}

@cmd() b(playerid, params[], help)
{
    extract params -> new string:text_local[144]; else return SendClientMessage(playerid, -1, "/local occ [text]");
    if (strlen(text_local) > 128) {
        return SendClientMessage(playerid, -1, "{FF0000}ERROR: Your text is very long. maximum 128!");
    }

    new str[144],
        Float:posX,
        Float:posY,
        Float:posZ
    ;
    GetPlayerPos(playerid, posX, posY, posZ);

    format(str, sizeof(str), "%s : (( %s ))", GetName(playerid), text_local);
    SendClientMessage(playerid, -1, str);

    for (new i = 0; i < GetMaxPlayers(); i++) {
        if (IsPlayerConnected(i)) {
            if (i != playerid) {
                if (IsPlayerInRangeOfPoint(i, 20.0, posX, posY, posZ)) {
                    SendClientMessage(i, -1, str);
                }
            }
        }
    }

    return 1;
}

@cmd() s(playerid, params[], help)
{
    extract params -> new string:text_shout[144]; else return SendClientMessage(playerid, -1, "/s(hout) [text]");
    if (strlen(text_shout) > 128) {
        return SendClientMessage(playerid, -1, "{FF0000}ERROR: Your text is very long. maximum 128!");
    }

    new str[144],
        Float:posX,
        Float:posY,
        Float:posZ
    ;
    GetPlayerPos(playerid, posX, posY, posZ);

    format(str, sizeof(str), "%s %s", GetName(playerid), text_shout);
    SendClientMessage(playerid, 0x800080FF, str);

    for (new i = 0; i < GetMaxPlayers(); i++) {
        if (IsPlayerConnected(i)) {
            if (i != playerid) {
                if (IsPlayerInRangeOfPoint(i, 40.0, posX, posY, posZ)) {
                    SendClientMessage(i, -1, str);
                }
            }
        }
    }

    return 1;
}

@cmd() ame(playerid, params[], help)
{
    extract params -> new string:text_ame[144]; else return SendClientMessage(playerid, -1, "/ame [text]");
    if (strlen(text_ame) > 128) {
        return SendClientMessage(playerid, -1, "{FF0000}ERROR: Your text is very long. maximum 128!");
    }

    new str[144],
        Float:posX,
        Float:posY,
        Float:posZ
    ;
    GetPlayerPos(playerid, posX, posY, posZ);

    format(str, sizeof(str), "* %s %s", GetName(playerid), text_ame);
    SendClientMessage(playerid, 0x800080FF, str);

    for (new i = 0; i < GetMaxPlayers(); i++) {
        if (IsPlayerConnected(i)) {
            if (i != playerid) {
                if (IsPlayerInRangeOfPoint(i, 10.1, posX, posY, posZ)) {
                    SendClientMessage(i, -1, str);
                }
            }
        }
    }

    return 1;
}

@cmd() me(playerid, params[], help)
{
    extract params -> new string:text_me[144]; else return SendClientMessage(playerid, -1, "/me [text]");
    if (strlen(text_me) > 128) {
        return SendClientMessage(playerid, -1, "{FF0000}ERROR: Your text is very long. maximum 128!");
    }

    new str[144],
        Float:posX,
        Float:posY,
        Float:posZ
    ;
    GetPlayerPos(playerid, posX, posY, posZ);

    format(str, sizeof(str), "* %s %s", GetName(playerid), text_me);
    SendClientMessage(playerid, 0x800080FF, str);

    for (new i = 0; i < GetMaxPlayers(); i++) {
        if (IsPlayerConnected(i)) {
            if (i != playerid) {
                if (IsPlayerInRangeOfPoint(i, 10.1, posX, posY, posZ)) {
                    SendClientMessage(i, -1, str);
                }
            }
        }
    }

    return 1;
}

@cmd() do(playerid, params[], help)
{
    extract params -> new string:text_do[144]; else return SendClientMessage(playerid, -1, "/do [text]");
    if (strlen(text_do) > 128) {
        return SendClientMessage(playerid, -1, "{FF0000}ERROR: Your text is very long. maximum 128!");
    }

    new str[144],
        Float:posX,
        Float:posY,
        Float:posZ
    ;
    GetPlayerPos(playerid, posX, posY, posZ);

    format(str, sizeof(str), "* %s (( %s ))", GetName(playerid), text_do);
    SendClientMessage(playerid, 0x800080FF, str);

    for (new i = 0; i < GetMaxPlayers(); i++) {
        if (IsPlayerConnected(i)) {
            if (i != playerid) {
                if (IsPlayerInRangeOfPoint(i, 10.1, posX, posY, posZ)) {
                    SendClientMessage(i, -1, str);
                }
            }
        }
    }

    return 1;
}


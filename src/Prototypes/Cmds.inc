@cmd() savestatic(playerid, params[], help)
{
    extract params -> new vehicleid; else return SendErrorMessage(playerid, "USAGE: /savestatic [vehicleid]");
    if (!IsValidVehicle(vehicleid)) return SendErrorMessage(playerid, "ERROR: invalid vehicle id");
    SaveStaticVehicle(vehicleid);
    return 1;
}

@cmd() aveh(playerid, params[], help)
{
    if (!GetPlayerAdmin(playerid)) return 1;
    new ownerid, modelid, Float:pos[4];
    if (sscanf(params, "ud", ownerid, modelid)) return SendErrorMessage(playerid, "USAGE: /createpveh [owner] [model]");
    GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
    GetPlayerFacingAngle(playerid, pos[3]);
    CreatePlayerVehicle(ownerid, modelid, 0, 0, pos[0], pos[1], pos[2], pos[3], 0, 0, false);
    return 1;
}

@cmd() asveh(playerid, params[], help)
{
    if (!GetPlayerAdmin(playerid)) return 1;
    new ownerid, modelid, Float:pos[4];
    if (sscanf(params, "ud", ownerid, modelid)) return SendErrorMessage(playerid, "USAGE: /createpveh [model]");
    GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
    GetPlayerFacingAngle(playerid, pos[3]);
    CreateStaticVehicle(modelid, 0, 0, pos[0], pos[1], pos[2], pos[3], 0, 0, false);
    return 1;
}

@cmd() adveh(playerid, params[], help)
{
    if (!GetPlayerAdmin(playerid)) return 1;
    new ownerid, modelid, Float:pos[4];
    if (sscanf(params, "ud", ownerid, modelid)) return SendErrorMessage(playerid, "USAGE: /adveh [owner] [model]");
    GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
    GetPlayerFacingAngle(playerid, pos[3]);
    CreateOnDemandVehicle(ownerid, modelid, 0, 0, pos[0], pos[1], pos[2], pos[3], 0, 0, false);
    return 1;
}

@cmd() dspawn(playerid, params[], help)
{
    if (!GetPlayerAdmin(playerid)) return 1;
    extract params -> new vehicleid; else return SendErrorMessage(playerid, "USAGE: /dspawn [vid]");
    UnloadVehicle(vehicleid);
    return 1;
}

@cmd() netstats(playerid, params[], help)
{
    new
        server_stats[456]
    ;
    GetNetworkStats(server_stats, 456);
    ShowPlayerDialog(playerid, 9999, DIALOG_STYLE_MSGBOX, "Server Network Stats", server_stats, "Close", "");

    return 1;
}

@cmd() slap(playerid, params[], help)
{
    if (!GetPlayerAdmin(playerid)) return 1;
    new targetid;
    if (sscanf(params, "u", targetid)) return SendClientMessage(playerid, -1, "/slap <targetid/name>");
    if (!IsPlayerConnected(targetid))
        return SendClientMessage(playerid, -1, "{FF0000}Player not connected!");

    new Float:pPos[3];
    new Float:curr_health;

    GetPlayerPos(targetid, pPos[0], pPos[1], pPos[2]);
    GetPlayerHealth(targetid, curr_health);
    SetPlayerPos(targetid, pPos[0], pPos[1], pPos[2] + 5.0);
    SetPlayerHealth(targetid, curr_health);

    return 1;
}

@cmd(.alts = {"cc"}) clearchat(playerid, params[], help)
{
    if (GetPlayerAdmin(playerid))
    {
        new self_or_all, value_clear;

        if (sscanf(params, "D(0)D(20)", self_or_all, value_clear))
            return SendClientMessage(playerid, -1, "Usage: /cc [self/all <0/1>] <value>");

        if (value_clear <= 0) value_clear = 20;

        switch (self_or_all)
        {
            case 0:
            {
                forex(i, value_clear)
                    SendClientMessage(playerid, -1, " ");
            }
            case 1:
            {
                forex(i, value_clear)
                    SendClientMessageToAll(-1, " ");
            }
            default:
            {
                forex(i, value_clear)
                    SendClientMessage(playerid, -1, " ");
            }
        }
    } else {
        new value_clear;

        if (sscanf(params, "D(20)", value_clear))
            return SendClientMessage(playerid, -1, "Usage: /cc <value>");

        if (value_clear <= 0) value_clear = 20;

        forex(i, value_clear)
            SendClientMessage(playerid, -1, " ");
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
    new targetid, text_pm[128];
    if (sscanf(params, "us[128]", targetid, text_pm))
        return SendClientMessage(playerid, -1, "/pm <target> [text]");
    if (strlen(text_pm) > 128)
        return SendClientMessage(playerid, -1, "{FF0000}ERROR: Your text is very long. maximum 128!");
    if (!IsPlayerConnected(targetid))
        return SendClientMessage(playerid, -1, "{FF0000}ERROR: Player not connected!");
    if (targetid == playerid)
        return SendClientMessage(playerid, -1, "{FF0000}ERROR: You can't PM yourself!");
    SendClientMessageInRange(playerid, targetid, 9999.0, 0xFFF070FF, 0, "PM from %s (%d): %s", GetName(playerid), playerid, text_pm);
    SendClientMessage(playerid, -1, "Your message has been sent.");
    return 1;
}

@cmd() b(playerid, params[], help)
{
    extract params -> new string:text_local[128]; else
        return SendClientMessage(playerid, -1, "/b [text]");
    if (strlen(text_local) > 128)
        return SendClientMessage(playerid, -1, "{FF0000}ERROR: Your text is very long. maximum 128!");
    SendClientMessageInRange(playerid, INVALID_PLAYER_ID, 20.0, -1, 0, "%s: (( %s ))", GetName(playerid), text_local);
    return 1;
}

@cmd() s(playerid, params[], help)
{
    extract params -> new string:text_shout[128]; else
        return SendClientMessage(playerid, -1, "/s(hout) [text]");
    if (strlen(text_shout) > 128)
        return SendClientMessage(playerid, -1, "{FF0000}ERROR: Your text is very long. maximum 128!");
    SendClientMessageInRange(playerid, INVALID_PLAYER_ID, 40.0, 0x800080FF, 0, "%s %s", GetName(playerid), text_shout);
    return 1;
}

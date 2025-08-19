@cmd() savestatic(playerid, params[], help)
{
    new vehicleid;
    if (sscanf(params, "d", vehicleid)) return SendErrorMessage(playerid, "USAGE: /savestatic [vehicleid]");
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
    new vehicleid;
    if (sscanf(params, "d", vehicleid)) return SendErrorMessage(playerid, "USAGE: /dspawn [vid]");
    UnloadVehicle(vehicleid);
    return 1;
}

@cmd() cc(playerid, params[], help)
{
    forex(i,20)
        SendClientMessage(playerid, -1, " ");
    return 1;
}
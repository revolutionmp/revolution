#define VMENU_SPAWN     1
#define VMENU_DESPAWN   2
#define VMENU_DEMAND    3
#define VMENU_SHARE     4

// ID >0 VID >0 (Spawn), ID >0 VID 0 (Despawn), ID 0 VID >0 (Share), ID 0 VID >0 FID >0 (OnDemand)
static enum E_VEHICLE_MENU
{
    ID,
    VehicleID,
    ShareKeyID,
    Model
}

static VehicleMenu[MAX_PLAYERS][MAX_PLAYER_VEHICLES][E_VEHICLE_MENU];
static const ResetVehicleMenu[E_VEHICLE_MENU];
static Iterator:VehicleMenu[MAX_PLAYERS]<MAX_PLAYER_VEHICLES>;

forward MyVehicle(playerid, check);
forward DetailsMyVehicle(playerid, slot, check);
forward ListMyVehicle(playerid, check);

GetVehicleMenuType(playerid, slot)
{
    if (VehicleMenu[playerid][slot][ID] != 0 && VehicleMenu[playerid][slot][VehicleID] != 0) return VMENU_SPAWN;
    if (VehicleMenu[playerid][slot][ID] != 0 && VehicleMenu[playerid][slot][VehicleID] == 0) return VMENU_DESPAWN;
    if (VehicleMenu[playerid][slot][ID] == 0 && VehicleMenu[playerid][slot][VehicleID] != 0 && VehicleInfo[VehicleMenu[playerid][slot][VehicleID]][FactionID] != 0) return VMENU_DEMAND;
    if (VehicleMenu[playerid][slot][ID] == 0 && VehicleMenu[playerid][slot][VehicleID] != 0 && VehicleMenu[playerid][slot][ShareKeyID] != 0) return VMENU_SHARE;
    return false;
}

GetVehicleIndentification(vid, vehmodel)
{
    new string[32], modelvin[64 + 1];
    valstr(string, vid);
    valstr(modelvin, vehmodel);
    SHA256_PassHash(modelvin, string, modelvin, sizeof(modelvin));
    strdel(modelvin, 0, 53);
    format(string, sizeof(string), "%s%05d", modelvin, vid);
    return string;
}

ShowVehicleDetails(id, model, const plate[], insurance, engine, body, fuel, siren)
{
    new string[128], dialog[512];
    format(string, sizeof(string), WHITE"VIN: "CYAN"%s\n"WHITE"Model: "YELLOW"%s\n", GetVehicleIndentification(id, model), GetVehicleModelName(model));
    strcat(dialog, string);

    if (!strcmp(plate, "NULL", false, 9)) string = WHITE"Plate: "RED"NO HAVE\n";
    else format(string, sizeof(string), WHITE"Plate: "ORANGE"%s\n", plate);
    strcat(dialog, string);

    if (insurance) string = WHITE"Insurance: "RED"NO HAVE\n";
    else format(string, sizeof(string), WHITE"Insurance: "GREEN"%d year(s)\n", insurance);
    strcat(dialog, string);

    strcat(dialog, WHITE"Upgrade: \n");
    if (!engine && !body && !fuel)
        strcat(dialog, RED"NONE");
    else
    {
        if (engine)
        {
            format(string, sizeof(string), WHITE"- "GREEN"Engine "FORESTGREEN"(Level %d)\n", engine);
            strcat(dialog, string);
        }
        if (body)
        {
            format(string, sizeof(string), WHITE"- "GREEN"Body "FORESTGREEN"(Level %d)\n", body);
            strcat(dialog, string);
        }
        if (fuel)
        {
            format(string, sizeof(string), WHITE"- "GREEN"Fuel "FORESTGREEN"(Level %d)\n", fuel);
            strcat(dialog, string);
        }
    }
    if (siren)
        strcat(dialog, WHITE"- "GREEN"Alarm "FORESTGREEN"(INSTALLED)\n");
    return dialog;
}

@cmd(.alts = {"myveh", "mv"}) myvehicle(playerid, params[], help)
{
    if (!PlayerLogged[playerid]) return 1;
    new query[256];
    mysql_format(Database, query, sizeof(query), "SELECT `ID`, `ShareKeyID`, `Model`, `PosX`, `PosY`, `PosZ` FROM `vehicles` WHERE `CharacterID`='%u' AND ParkingID IS NULL AND NOT `Destroyed`='1' ORDER BY `ID` DESC;", PlayerCharInfo[playerid][ID]);
    mysql_pquery(Database, query, "MyVehicle", "dd", playerid, RaceCheck[playerid]);
    return 1;
}

public MyVehicle(playerid, check)
{
    if (check != RaceCheck[playerid])
    {
        SendErrorMessage(playerid, "ERROR: An unexpected error has occurred, please try again later!");
        defer KickPlayer(playerid);
    }
    else
    {
        new slot, caption[32], rows = cache_num_rows();
        format(caption, sizeof(caption), "My Vehicles (%d/%d):", rows, rows);
        AddDialogListitem(playerid, "#\tVehicle Model\tStatus\tDistance\n");
        if (rows)
        {
            new bool:spawn;
            forex(i, rows)
            {
                cache_get_value_name_int(i, "ID", VehicleMenu[playerid][slot][ID]);
                foreach (new j : VehicleInfo)
                {
                    if (VehicleInfo[j][ID] == VehicleMenu[playerid][slot][ID])
                    {
                        spawn = true;
                        VehicleMenu[playerid][slot][VehicleID] = j;
                        VehicleMenu[playerid][slot][ShareKeyID] = VehicleInfo[j][ShareKeyID];
                        AddDialogListitem(playerid, WHITE"V%02d:\t%s\t%s\t"YELLOW"%.1f\n", j, GetVehicleModelName(VehicleInfo[j][Model]), (VehicleInfo[j][Locked]) ? RED"Locked" : GREEN"Unlock", GetPlayerDistanceToVehicle(playerid, j));
                        break;
                    }
                }

                if (!spawn)
                {
                    new Float:pos[3], bool:sharekeyid;
                    cache_is_value_name_null(i, "ShareKeyID", sharekeyid);
                    if (!sharekeyid) cache_get_value_name_int(i, "ShareKeyID", VehicleMenu[playerid][slot][ShareKeyID]);
                    cache_get_value_name_int(i, "Model", VehicleMenu[playerid][slot][Model]);
                    cache_get_value_name_float(i, "PosX", pos[0]);
                    cache_get_value_name_float(i, "PosY", pos[1]);
                    cache_get_value_name_float(i, "PosZ", pos[2]);
                    AddDialogListitem(playerid, WHITE"D%02d:\t%s\t"ORANGE"Despawned\t"YELLOW"%.1f\n", VehicleMenu[playerid][slot][ID], GetVehicleModelName(VehicleMenu[playerid][slot][Model]), GetPlayerDistanceToPoint3D(playerid, pos[0], pos[1], pos[2]));
                }

                spawn = false;
                Iter_Add(VehicleMenu[playerid], slot++);
            }
        }

        foreach (new i : VehicleInfo)
        {
            if (VehicleInfo[i][FactionID] && ((VehicleInfo[i][ShareKeyID] == PlayerCharInfo[playerid][ID]) || (VehicleInfo[i][CharacterID] == PlayerCharInfo[playerid][ID] && VehicleInfo[i][FactionID] == PlayerCharInfo[playerid][FactionID])))
            {
                new color[9];
                VehicleMenu[playerid][slot][VehicleID] = i;
                if (VehicleInfo[i][ShareKeyID] != PlayerCharInfo[playerid][ID]) color = BLUE;
                else
                {
                    VehicleMenu[playerid][slot][ShareKeyID] = VehicleInfo[i][ShareKeyID];
                    color = CYAN;
                }
                Iter_Add(VehicleMenu[playerid], slot++);
                AddDialogListitem(playerid, WHITE"V%02d:\t%s%s\t%s\t"YELLOW"%.1f\n", i, color, GetVehicleModelName(VehicleInfo[i][Model]), (VehicleInfo[i][Locked]) ? RED"Locked" : GREEN"Unlock", GetPlayerDistanceToVehicle(playerid, i));
            }
            else if (VehicleInfo[i][ShareKeyID] == PlayerCharInfo[playerid][ID])
            {
                VehicleMenu[playerid][slot][VehicleID] = i;
                VehicleMenu[playerid][slot][ShareKeyID] = VehicleInfo[i][ShareKeyID];
                Iter_Add(VehicleMenu[playerid], slot++);
                AddDialogListitem(playerid, WHITE"V%02d:\t"ORANGE"%s\t%s\t"YELLOW"%.1f\n", i, GetVehicleModelName(VehicleInfo[i][Model]), (VehicleInfo[i][Locked]) ? RED"Locked" : GREEN"Unlock", GetPlayerDistanceToVehicle(playerid, i));
            }
        }

        ShowPlayerDialogPages(playerid, "MyVehicle", DIALOG_STYLE_TABLIST_HEADERS, caption, "Select", "Close", 10, "Next Page >", "< Previous Page");
    }
    return 1;
}

DialogPages:MyVehicle(playerid, response, listitem, inputtext[])
{
    if (!GetVehicleMenuType(playerid, listitem))
    {
        SendErrorMessage(playerid, "ERROR: An unexpected error has occurred, please try again later!");
        defer KickPlayer(playerid);
    }
    else if (!response)
    {
        foreach (new i : VehicleMenu[playerid])
            VehicleMenu[playerid][i] = ResetVehicleMenu;
        Iter_Clear(VehicleMenu[playerid]);
    }
    else
    {
        new caption[32];
        SetPVarInt(playerid, "Slot", listitem);
        switch (GetVehicleMenuType(playerid, listitem))
        {
            case VMENU_SHARE:
            {
                format(caption, sizeof(caption), "[ID:%d] Menu Vehicle", VehicleInfo[VehicleMenu[playerid][listitem][VehicleID]][ID]);
                Dialog_Show(playerid, SelectMyVehicle, DIALOG_STYLE_TABLIST, caption, "Information Details\n"RED"Revoke Key", "Select", "Back");
            }
            case VMENU_DEMAND:
            {
                new dialog[64];
                format(caption, sizeof(caption), "[FactionID:%d] Menu Vehicle", VehicleInfo[VehicleMenu[playerid][listitem][VehicleID]][FactionID]);
                if (VehicleInfo[VehicleMenu[playerid][listitem][VehicleID]][ShareKeyID]) dialog = "Information Details\n"RED"Revoke Key";
                else dialog = "Information Details\nSharing Key";
                Dialog_Show(playerid, SelectMyVehicle, DIALOG_STYLE_TABLIST, caption, dialog, "Select", "Back");
            }
            default:
            {
                new dialog[256];
                format(caption, sizeof(caption), "[ID:%d] Menu Vehicle", VehicleMenu[playerid][listitem][ID]);
                if (VehicleMenu[playerid][listitem][ShareKeyID]) strcat(dialog, "Information Details\n"RED"Revoke Key\nGive to Player\nSell to Player\nTrade to Player");
                else strcat(dialog, "Information Details\nSharing Key\nGive to Player\nSell to Player\nTrade to Player");
                if (!VehicleMenu[playerid][listitem][VehicleID]) strcat(dialog, "\n"GREEN"Spawn Vehicle");
                Dialog_Show(playerid, SelectMyVehicle, DIALOG_STYLE_TABLIST, caption, dialog, "Select", "Back");
            }
        }
    }
    return 1;
}

Dialog:SelectMyVehicle(playerid, response, listitem, inputtext[])
{
    new slot = GetPVarInt(playerid, "Slot");
    if (!GetVehicleMenuType(playerid, slot))
    {
        SendErrorMessage(playerid, "ERROR: An unexpected error has occurred, please try again later!");
        defer KickPlayer(playerid);
    }
    else if (!response)
    {
        new query[256];
        mysql_format(Database, query, sizeof(query), "SELECT `ID`, `ShareKeyID`, `Model`, `PosX`, `PosY`, `PosZ` FROM `vehicles` WHERE `CharacterID`='%u' AND ParkingID IS NULL AND NOT `Destroyed`='1' ORDER BY `ID` DESC;", PlayerCharInfo[playerid][ID]);
        mysql_pquery(Database, query, "MyVehicle", "dd", playerid, RaceCheck[playerid]);
    }
    else
    {
        switch (listitem)
        {
            case 0:
            {
                if (VehicleMenu[playerid][slot][VehicleID])
                    Dialog_Show(playerid, DetailsMyVehicle, DIALOG_STYLE_MSGBOX, "Vehicle Information Details",
                                ShowVehicleDetails(VehicleMenu[playerid][slot][ID], VehicleInfo[VehicleMenu[playerid][slot][VehicleID]][Model], VehicleInfo[VehicleMenu[playerid][slot][VehicleID]][Plate], VehicleInfo[VehicleMenu[playerid][slot][VehicleID]][Insurance], VehicleInfo[VehicleMenu[playerid][slot][VehicleID]][UpgradeEngine], VehicleInfo[VehicleMenu[playerid][slot][VehicleID]][UpgradeBody], VehicleInfo[VehicleMenu[playerid][slot][VehicleID]][UpgradeFuel], VehicleInfo[VehicleMenu[playerid][slot][VehicleID]][Siren]), "Close", "");
                else
                {
                    new query[256];
                    mysql_format(Database, query, sizeof(query), "SELECT `Model`, `Plate`, `Insurance`, `UpgradeEngine`, `UpgradeBody`, `UpgradeFuel`, `Siren` FROM `vehicles` WHERE `ID`='%u' AND ParkingID IS NULL AND `Destroyed`='0' LIMIT 1;", VehicleMenu[playerid][slot][ID]);
                    mysql_pquery(Database, query, "DetailsMyVehicle", "bdd", playerid, slot, RaceCheck[playerid]);
                }
            }
            case 1:
            {
                if (VehicleMenu[playerid][slot][ShareKeyID] < 1)
                    return Dialog_Show(playerid, ShareMyVehicle, DIALOG_STYLE_INPUT, "Share Vehicle", WHITE"Input player id or name you want to\nshare your "YELLOW"%s"WHITE" with: "GREEN"(input bellow)\n\n"RED"NOTICE: "YELLOW"The person who get access the\nvehicle from you, cannot be give or sell\nyour vehicle to someone else, he can only\nunlock it and access your vehicle trunk.", "Confirm", "Cancel", GetVehicleModelName((VehicleMenu[playerid][slot][VehicleID]) ? VehicleInfo[VehicleMenu[playerid][slot][VehicleID]][Model] : VehicleMenu[playerid][slot][Model]));
                if (VehicleMenu[playerid][slot][ID])
                {
                    new query[128];
                    mysql_format(Database, query, sizeof(query), "UPDATE `vehicles` SET ShareKeyID = NULL WHERE `ID`='%u';", VehicleMenu[playerid][slot][ID]);
                    mysql_pquery(Database, query);
                }
                if (VehicleMenu[playerid][slot][VehicleID])
                {
                    VehicleInfo[VehicleMenu[playerid][slot][VehicleID]][ShareKeyID] = 0;
                    if (VehicleInfo[VehicleMenu[playerid][slot][VehicleID]][ID])
                        orm_delvar(VehicleInfo[VehicleMenu[playerid][slot][VehicleID]][ORM_ID], "ShareKeyID");
                }
                //SendClientMessage(playerid, X11_LIGHTSTEELBLUE3, "VEHICLE: "WHITE"The owner of "GREEN"%s"WHITE"'s "YELLOW"%s"WHITE" has been "RED"revoked"WHITE" shared key.", PlayerCharInfo[playerid][Nickname], GetVehicleModelName((VehicleMenu[playerid][slot][VehicleID]) ? VehicleInfo[VehicleMenu[playerid][slot][VehicleID]][Model] : VehicleMenu[playerid][slot][Model]));
            }
            case 5:
            {
                if (!VehicleMenu[playerid][slot][VehicleID])
                {
                    new query[512];
                    mysql_format(Database, query, sizeof(query), "SELECT `ID`, `CharacterID`, `ShareKeyID`, `Model`, `Plate`, `Insurance`, `Locked`, `Handbrake`, `Destroyed`, `Health`, `MaxHealth`, `Fuel`, `MaxFuel`, `UpgradeEngine`, `UpgradeBody`, `UpgradeFuel`, `DamagePanels`, `DamageDoors`, `DamageLights`, `DamageTires`, `VirtualWorld`, `Interior`, `PosX`, `PosY`, `PosZ`, `PosA`, `Color1`, `Color2`, `Siren`, `RespawnDelay` FROM `vehicles` WHERE `ID`='%u' AND ParkingID IS NULL AND Destroyed='0' LIMIT 1;", VehicleMenu[playerid][slot][ID]);
                    mysql_pquery(Database, query, "LoadPlayerVehicle", "bd", playerid, RaceCheck[playerid]);
                }
            }
        }
    }

    foreach (new i : VehicleMenu[playerid])
        VehicleMenu[playerid][i] = ResetVehicleMenu;
    DeletePVar(playerid, "Slot");
    Iter_Clear(VehicleMenu[playerid]);
    return 1;
}

public DetailsMyVehicle(playerid, slot, check)
{
    if (check != RaceCheck[playerid] || cache_num_rows() != 1)
    {
        SendErrorMessage(playerid, "ERROR: An unexpected error has occurred, please try again later!");
        defer KickPlayer(playerid);
    }
    else
    {
        new vehicleinfo[6], plate[9];
        cache_get_value_name_int(0, "Model", vehicleinfo[0]);
        cache_get_value_name(0, "Plate", plate, sizeof(plate));
        cache_get_value_name_int(0, "Insurance", vehicleinfo[1]);
        cache_get_value_name_int(0, "UpgradeEngine", vehicleinfo[2]);
        cache_get_value_name_int(0, "UpgradeBody", vehicleinfo[3]);
        cache_get_value_name_int(0, "UpgradeFuel", vehicleinfo[4]);
        cache_get_value_name_int(0, "Siren", vehicleinfo[5]);
        Dialog_Show(playerid, DetailsMyVehicle, DIALOG_STYLE_MSGBOX, "Vehicle Information Details",
                    ShowVehicleDetails(VehicleMenu[playerid][slot][ID], vehicleinfo[0], plate, vehicleinfo[1], vehicleinfo[2], vehicleinfo[3], vehicleinfo[4], vehicleinfo[5]), "Close", "");
    }
    return 1;
}

Dialog:ShareMyVehicle(playerid, response, listitem, inputtext[])
{
    new slot = GetPVarInt(playerid, "Slot");
    if (!GetVehicleMenuType(playerid, slot))
    {
        SendErrorMessage(playerid, "ERROR: An unexpected error has occurred, please try again later!");
        defer KickPlayer(playerid);
    }
    else if (response)
    {
        new targetid;
        if (sscanf(inputtext, "r", targetid) || !IsPlayerConnected(targetid))
            return Dialog_Show(playerid, ShareMyVehicle, DIALOG_STYLE_INPUT, "Share Vehicle", WHITE"Input player id or name you want to\nshare your "YELLOW"%s"WHITE" with: "GREEN"(input bellow)\n\n"RED"NOTICE: "YELLOW"The person who get access the\nvehicle from you, cannot be give or sell\nyour vehicle to someone else, he can only\nunlock it and access your vehicle trunk.", "Confirm", "Cancel", GetVehicleModelName((VehicleMenu[playerid][slot][VehicleID]) ? VehicleInfo[VehicleMenu[playerid][slot][VehicleID]][Model] : VehicleMenu[playerid][slot][Model]));
        else if (targetid == playerid)
            SendErrorMessage(playerid, "ERROR: You can't share to yourself!");
        else if (!IsPlayerInRangeOfPlayer(playerid, targetid, 5.0))
            SendErrorMessage(playerid, "ERROR: Unable to share vehicle, you are to far away from target id!");
        else
        {
            if (VehicleMenu[playerid][slot][VehicleID])
                VehicleInfo[VehicleMenu[playerid][slot][VehicleID]][ShareKeyID] = PlayerCharInfo[targetid][ID];
            if (VehicleInfo[VehicleMenu[playerid][slot][VehicleID]][ID])
                orm_addvar_int(VehicleInfo[VehicleMenu[playerid][slot][VehicleID]][ORM_ID], VehicleInfo[VehicleMenu[playerid][slot][VehicleID]][ShareKeyID], "ShareKeyID");
            if (VehicleMenu[playerid][slot][ID])
            {
                new query[128];
                mysql_format(Database, query, sizeof(query), "UPDATE `vehicles` SET `ShareKeyID`='%u' WHERE `ID`='%u';", PlayerCharInfo[targetid][ID], VehicleMenu[playerid][slot][ID]);
                mysql_pquery(Database, query);
            }
            SendClientMessage(playerid, X11_LIGHTSTEELBLUE3, "VEHICLE: "WHITE"You have "GREEN"shared"WHITE" your "YELLOW"%s"WHITE" to "CYAN"%s"WHITE".", GetVehicleModelName((VehicleMenu[playerid][slot][VehicleID]) ? VehicleInfo[VehicleMenu[playerid][slot][VehicleID]][Model] : VehicleMenu[playerid][slot][Model]), PlayerCharInfo[targetid][Nickname]);
            SendClientMessage(targetid, X11_LIGHTSTEELBLUE3, "VEHICLE: "WHITE"You have received "GREEN"shared"WHITE" a "YELLOW"%s"WHITE" from "CYAN"%s"WHITE".", GetVehicleModelName((VehicleMenu[playerid][slot][VehicleID]) ? VehicleInfo[VehicleMenu[playerid][slot][VehicleID]][Model] : VehicleMenu[playerid][slot][Model]), PlayerCharInfo[playerid][Nickname]);
        }
    }
    foreach (new i : VehicleMenu[playerid])
        VehicleMenu[playerid][i] = ResetVehicleMenu;
    DeletePVar(playerid, "Slot");
    Iter_Clear(VehicleMenu[playerid]);
    return 1;
}

@cmd(.alts = {"lmv"}) listmyvehicle(playerid, params[], help)
{
    if (!PlayerLogged[playerid]) return 1;
    new query[256];
    mysql_format(Database, query, sizeof(query), "SELECT `ID`, `Model`, `Destroyed`, `ParkingID` FROM `vehicles` WHERE `CharacterID`='%u' ORDER BY `Destroyed`, `ParkingID` ASC, `ID` DESC;", PlayerCharInfo[playerid][ID]);
    mysql_pquery(Database, query, "ListMyVehicle", "dd", playerid, RaceCheck[playerid]);
    return 1;
}

public ListMyVehicle(playerid, check)
{
    if (check != RaceCheck[playerid])
    {
        SendErrorMessage(playerid, "ERROR: An unexpected error has occurred, please try again later!");
        defer KickPlayer(playerid);
    }
    else
    {
        new caption[32], rows = cache_num_rows();
        format(caption, sizeof(caption), "List Vehicles (%d):", rows);
        AddDialogListitem(playerid, "#\tModel\tState");
        if (rows)
        {
            new cache[4], bool:parkingid;
            forex(i, rows)
            {
                cache_get_value_name_int(i, "ID", cache[0]);
                cache_get_value_name_int(i, "Model", cache[1]);
                cache_get_value_name_int(i, "Destroyed", cache[2]);
                cache_is_value_name_null(i, "ParkingID", parkingid);
                if (cache[2])
                    AddDialogListitem(playerid, "D%03d:\t%s\t"GRAY"Destroyed", cache[0], GetVehicleModelName(cache[1]));
                else if (!parkingid)
                {
                    cache_get_value_name_int(i, "ParkingID", cache[3]);
                    AddDialogListitem(playerid, WHITE"D%03d:\t%s\t"YELLOW"Parked in [ID:%d]", cache[0], GetVehicleModelName(cache[1]), cache[3]);
                }
                else
                {
                    new vehicleid;
                    foreach (new j : VehicleInfo)
                    {
                        if (VehicleInfo[j][ID] == cache[0])
                        {
                            vehicleid = j;
                            break;
                        }
                    }
                    if (vehicleid) AddDialogListitem(playerid, "V%03d:\t%s\t"GREEN"Spawned", vehicleid, GetVehicleModelName(VehicleInfo[vehicleid][Model]));
                    else AddDialogListitem(playerid, WHITE"D%03d:\t%s\t"ORANGE"Despawned", cache[0], GetVehicleModelName(cache[1]));
                }
            }
        }
        ShowPlayerDialogPages(playerid, "ListMyVehicle", DIALOG_STYLE_TABLIST_HEADERS, caption, "Close", "", 10, "Next Page >", "< Previous Page");
    }
    return 1;
}
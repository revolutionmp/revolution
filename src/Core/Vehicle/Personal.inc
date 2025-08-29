forward LoadPlayerVehicle(playerid, check);

GetPlayerOwnVehicle(playerid)
{
    new count;
	foreach(new i : VehicleInfo)
        if(VehicleInfo[i][id_character] == PlayerCharInfo[playerid][id])
            count++;
	return count;
}

UnloadPlayerOwnVehicle(playerid)
{
    foreach (new i : VehicleInfo)
        if (IsValidVehicle(i) && VehicleInfo[i][CharacterID] == PlayerCharInfo[playerid][ID])
            UnloadVehicle(i);
    return 1;
}

SavePlayerOwnVehicle(playerid)
{
    foreach (new i : VehicleInfo)
        if (IsValidVehicle(i) && VehicleInfo[i][CharacterID] == PlayerCharInfo[playerid][ID])
            SavePlayerVehicle(i);
    return 1;
}

SavePlayerVehicle(vehicleid)
{
    if (GetVehicleInfoType(vehicleid) != VEHICLE_PLAYER) return false;

    if (!VehicleInfo[vehicleid][Handbrake])
    {
        GetVehiclePos(vehicleid, VehicleInfo[vehicleid][PosX], VehicleInfo[vehicleid][PosY], VehicleInfo[vehicleid][PosZ]);
        GetVehicleZAngle(vehicleid, VehicleInfo[vehicleid][PosA]);
    }

    if (!VehicleInfo[vehicleid][Destroyed])
    {
        GetVehicleHealth(vehicleid, VehicleInfo[vehicleid][Health]);
        GetVehicleDamageStatus(vehicleid, VehicleInfo[vehicleid][DamagePanels], VehicleInfo[vehicleid][DamageDoors], VehicleInfo[vehicleid][DamageLights], VehicleInfo[vehicleid][DamageTires]);
    }
    else
    {
        GetVehicleMaxHealth(vehicleid, VehicleInfo[vehicleid][Health]);
        GetVehicleMaxFuel(vehicleid, VehicleInfo[vehicleid][Fuel]);
        VehicleInfo[vehicleid][DamagePanels] = 0;
        VehicleInfo[vehicleid][DamageDoors] = 0;
        VehicleInfo[vehicleid][DamageLights] = 0;
        VehicleInfo[vehicleid][DamageTires] = 0;
    }

    VehicleInfo[vehicleid][VirtualWorld] = GetVehicleVirtualWorld(vehicleid);
    VehicleInfo[vehicleid][Interior] = AntiCheatGetVehicleInterior(vehicleid);
    VehicleInfo[vehicleid][Interior] = 0;
    orm_update(VehicleInfo[vehicleid][ORM_ID]);
    return true;
}

CreatePlayerVehicle(ownerid, modelid, virtualworld, interior, Float:posx, Float:posy, Float:posz, Float:posa, color1, color2, bool:siren)
{
    if (GetVehiclePoolToLimit() < 1)
    {
        new query[512];
        mysql_format(Database, query, sizeof(query), "INSERT INTO `vehicles` (`CharacterID`,`Model`,`VirtualWorld`,`Interior`,`PosX`,`PosY`,`PosZ`,`PosA`,`Color1`,`Color2`,`Siren`) VALUES ('%u','%d','%d','%d','%f','%f','%f','%f','%d','%d','%d');",
        PlayerCharInfo[ownerid][ID], modelid, virtualworld, interior, posx, posy, posz, posa, color1, color2, siren);
        mysql_pquery(Database, query);
    }
    else
    {
        new vehicleid = CreateVehicle(modelid, posx, posy, posz, posa, color1, color2, -1, siren);
        SetVehicleNumberPlate(vehicleid, " ");
        SetVehicleVirtualWorld(vehicleid, virtualworld);
        LinkVehicleToInterior(vehicleid, interior);

        VehicleInfo[vehicleid][CharacterID] = PlayerCharInfo[ownerid][ID];
        VehicleInfo[vehicleid][Model] = modelid;
        VehicleInfo[vehicleid][Health] = 1000.0;
        VehicleInfo[vehicleid][MaxHealth] = 1000.0;
        VehicleInfo[vehicleid][Fuel] = 100.0;
        VehicleInfo[vehicleid][MaxFuel] = 100.0;
        VehicleInfo[vehicleid][VirtualWorld] = virtualworld;
        VehicleInfo[vehicleid][Interior] = interior;
        VehicleInfo[vehicleid][PosX] = posx;
        VehicleInfo[vehicleid][PosY] = posy;
        VehicleInfo[vehicleid][PosZ] = posz;
        VehicleInfo[vehicleid][PosA] = posa;
        VehicleInfo[vehicleid][Color1] = color1;
        VehicleInfo[vehicleid][Color2] = color2;
        VehicleInfo[vehicleid][Siren] = siren;

        CreateVehicleORM(vehicleid);
        orm_setkey(VehicleInfo[vehicleid][ORM_ID], "ID");
        orm_insert(VehicleInfo[vehicleid][ORM_ID]);
        Iter_Add(VehicleInfo, vehicleid);
    }
    return 1;
}

public LoadPlayerVehicle(playerid, check)
{
    if (check != RaceCheck[playerid])
    {
        SendErrorMessage(playerid, "ERROR: An unexpected error has occurred, please try again later!");
        defer KickPlayer(playerid);
    }
    else
    {
        new rows = cache_num_rows();
        if (GetVehiclePoolToLimit() < rows)
        {
            new over = (rows + (Iter_Count(VehicleInfo) - (Iter_Size(VehicleInfo) - 1)));
            rows = (rows - over);
            SendErrorMessage(playerid, "SERVER: Your %d vehicle data cannot be loaded, because it exceeds the server's maximum vehicle limit!", over);
        }

        forex(i, rows)
        {
            new bool:sharekeyid, bool:plate, model, Float:pos[4], color[2], bool:siren;
            cache_is_value_name_null(i, "ShareKeyID", sharekeyid);
            cache_is_value_name_null(i, "Plate", plate);
            cache_get_value_name_int(i, "Model", model);
            cache_get_value_name_float(i, "PosX", pos[0]);
            cache_get_value_name_float(i, "PosY", pos[1]);
            cache_get_value_name_float(i, "PosZ", pos[2]);
            cache_get_value_name_float(i, "PosA", pos[3]);
            cache_get_value_name_int(i, "Color1", color[0]);
            cache_get_value_name_int(i, "Color2", color[1]);
            cache_get_value_name_bool(i, "Siren", siren);
            new vehicleid = CreateVehicle(model, pos[0], pos[1], pos[2], pos[3], color[0], color[1], -1, siren);

            CreateVehicleORM(vehicleid, true, true, sharekeyid, true, plate);
            orm_setkey(VehicleInfo[vehicleid][ORM_ID], "ID");
            orm_apply_cache(VehicleInfo[vehicleid][ORM_ID], i);

            if (IsNull(VehicleInfo[vehicleid][Plate])) SetVehicleNumberPlate(vehicleid, " ");
            else SetVehicleNumberPlate(vehicleid, VehicleInfo[vehicleid][Plate]);
            SetVehicleHealth(vehicleid, VehicleInfo[vehicleid][Health]);
            UpdateVehicleDamageStatus(vehicleid, VehicleInfo[vehicleid][DamagePanels], VehicleInfo[vehicleid][DamageDoors], VehicleInfo[vehicleid][DamageLights], VehicleInfo[vehicleid][DamageTires]);
            LinkVehicleToInterior(vehicleid, VehicleInfo[vehicleid][Interior]);
            SetVehicleVirtualWorld(vehicleid, VehicleInfo[vehicleid][VirtualWorld]);
            Iter_Add(VehicleInfo, vehicleid);
        }
    }
    return 1;
}
forward LoadStaticVehicle();

SaveStaticVehicle(vehicleid)
{
    if (GetVehicleInfoType(vehicleid) != VEHICLE_STATIC) return false;
    if (!VehicleInfo[vehicleid][Handbrake])
    {
        GetVehiclePos(vehicleid, VehicleInfo[vehicleid][PosX], VehicleInfo[vehicleid][PosY], VehicleInfo[vehicleid][PosZ]);
        GetVehicleZAngle(vehicleid, VehicleInfo[vehicleid][PosA]);
    }

    GetVehicleHealth(vehicleid, VehicleInfo[vehicleid][MaxHealth]);
    GetVehicleDamageStatus(vehicleid, VehicleInfo[vehicleid][DamagePanels], VehicleInfo[vehicleid][DamageDoors], VehicleInfo[vehicleid][DamageLights], VehicleInfo[vehicleid][DamageTires]);
    VehicleInfo[vehicleid][VirtualWorld] = GetVehicleVirtualWorld(vehicleid);
    // VehicleInfo[vehicleid][Interior] = AntiCheatGetVehicleInterior(vehicleid);
    VehicleInfo[vehicleid][Interior] = 0;
    orm_update(VehicleInfo[vehicleid][ORM_ID]);
    return true;
}

CreateStaticVehicle(modelid, virtualworld, interior, Float:posx, Float:posy, Float:posz, Float:posa, color1, color2, bool:siren)
{
    if (GetVehiclePoolToLimit() < 1) return false;
    new vehicleid = AddStaticVehicleEx(modelid, posx, posy, posz, posa, color1, color2, -1, siren);
    SetVehicleNumberPlate(vehicleid, " ");
    SetVehicleVirtualWorld(vehicleid, virtualworld);
    LinkVehicleToInterior(vehicleid, interior);

    VehicleInfo[vehicleid][Model] = modelid;
    VehicleInfo[vehicleid][MaxHealth] = 1000.0;
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

    CreateVehicleORM(vehicleid, false);
    orm_setkey(VehicleInfo[vehicleid][ORM_ID], "ID");
    orm_insert(VehicleInfo[vehicleid][ORM_ID]);
    Iter_Add(VehicleInfo, vehicleid);
    return true;
}

public LoadStaticVehicle()
{
    new rows = cache_num_rows();
    if (rows > GetVehiclePoolToLimit())
    {
        new over = (rows + (Iter_Count(VehicleInfo) - (Iter_Size(VehicleInfo) - 1)));
        rows = (rows - over);
        printf("SERVER: Static %d vehicle data cannot be loaded, because it exceeds the server's maximum vehicle limit!", over);
    }

    forex(i, rows)
    {
        new bool:factionid, bool:sharekeyid, bool:plate, model, Float:pos[4], color[2], bool:siren;
        cache_is_value_name_null(i, "FactionID", factionid);
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
        new vehicleid = AddStaticVehicleEx(model, pos[0], pos[1], pos[2], pos[3], color[0], color[1], -1, siren);

        CreateVehicleORM(vehicleid, false, factionid, sharekeyid, true, plate);
        orm_setkey(VehicleInfo[vehicleid][ORM_ID], "ID");
        orm_apply_cache(VehicleInfo[vehicleid][ORM_ID], i);
        SetVehicleHealth(vehicleid, VehicleInfo[vehicleid][MaxHealth]);
        SetVehicleNumberPlate(vehicleid, VehicleInfo[vehicleid][Plate]);
        UpdateVehicleDamageStatus(vehicleid, VehicleInfo[vehicleid][DamagePanels], VehicleInfo[vehicleid][DamageDoors], VehicleInfo[vehicleid][DamageLights], VehicleInfo[vehicleid][DamageTires]);
        LinkVehicleToInterior(vehicleid, VehicleInfo[vehicleid][Interior]);
        SetVehicleVirtualWorld(vehicleid, VehicleInfo[vehicleid][VirtualWorld]);
        Iter_Add(VehicleInfo, vehicleid);
    }
    return 1;
}
#include <YSI_Coding\y_hooks>

ORM:CreateVehicleORM(vehicleid, bool:characterid = true, bool:factionid = true, bool:sharekeyid = true, bool:parkingid = true, bool:plate=true)
{
    new ORM:ormid = VehicleInfo[vehicleid][ORM_ID] = orm_create("vehicles", Database);
    if (characterid)
    {
        orm_addvar_int(ormid, VehicleInfo[vehicleid][CharacterID], "CharacterID");
        orm_addvar_int(ormid, VehicleInfo[vehicleid][Destroyed], "Destroyed");
        orm_addvar_int(ormid, VehicleInfo[vehicleid][Insurance], "Insurance");
        orm_addvar_float(ormid, VehicleInfo[vehicleid][Health], "Health");
        orm_addvar_float(ormid, VehicleInfo[vehicleid][Fuel], "Fuel");
    }
    if (!factionid) orm_addvar_int(ormid, VehicleInfo[vehicleid][FactionID], "FactionID");
    if (!sharekeyid) orm_addvar_int(ormid, VehicleInfo[vehicleid][ShareKeyID], "ShareKeyID");
    if (!parkingid) orm_addvar_int(ormid, VehicleInfo[vehicleid][ParkingID], "ParkingID");
    if (!plate) orm_addvar_string(ormid, VehicleInfo[vehicleid][Plate], 9, "Plate");
    orm_addvar_int(ormid, VehicleInfo[vehicleid][ID], "ID");
    orm_addvar_int(ormid, VehicleInfo[vehicleid][Model], "Model");
    orm_addvar_int(ormid, VehicleInfo[vehicleid][Locked], "Locked");
    orm_addvar_int(ormid, VehicleInfo[vehicleid][Handbrake], "Handbrake");
    orm_addvar_float(ormid, VehicleInfo[vehicleid][MaxHealth], "MaxHealth");
    orm_addvar_float(ormid, VehicleInfo[vehicleid][MaxFuel], "MaxFuel");
    orm_addvar_int(ormid, VehicleInfo[vehicleid][UpgradeEngine], "UpgradeEngine");
    orm_addvar_int(ormid, VehicleInfo[vehicleid][UpgradeBody], "UpgradeBody");
    orm_addvar_int(ormid, VehicleInfo[vehicleid][UpgradeFuel], "UpgradeFuel");
    orm_addvar_int(ormid, VehicleInfo[vehicleid][DamagePanels], "DamagePanels");
    orm_addvar_int(ormid, VehicleInfo[vehicleid][DamageDoors], "DamageDoors");
    orm_addvar_int(ormid, VehicleInfo[vehicleid][DamageLights], "DamageLights");
    orm_addvar_int(ormid, VehicleInfo[vehicleid][DamageTires], "DamageTires");
    orm_addvar_int(ormid, VehicleInfo[vehicleid][VirtualWorld], "VirtualWorld");
    orm_addvar_int(ormid, VehicleInfo[vehicleid][Interior], "Interior");
    orm_addvar_float(ormid, VehicleInfo[vehicleid][PosX], "PosX");
    orm_addvar_float(ormid, VehicleInfo[vehicleid][PosY], "PosY");
    orm_addvar_float(ormid, VehicleInfo[vehicleid][PosZ], "PosZ");
    orm_addvar_float(ormid, VehicleInfo[vehicleid][PosA], "PosA");
    orm_addvar_int(ormid, VehicleInfo[vehicleid][Color1], "Color1");
    orm_addvar_int(ormid, VehicleInfo[vehicleid][Color2], "Color2");
    orm_addvar_int(ormid, VehicleInfo[vehicleid][Siren], "Siren");
    orm_addvar_int(ormid, VehicleInfo[vehicleid][RespawnDelay], "RespawnDelay");
    return ormid;
}

@hook(.fallback = true) OnGameModeInit()
{
    Iter_Init(VehicleInfo);
    return 1;
}

@hook(.fallback = true) OnGameModeExit()
{
    foreach (new i : VehicleInfo)
        UnloadVehicle(i);
    return 1;
}

@hook(.fallback = true) OnVehicleSpawn(vehicleid)
{
    if (VehicleInfo[vehicleid][Destroyed]) return UnloadVehicle(vehicleid);
    if (GetVehicleInfoType(vehicleid) == VEHICLE_STATIC)
    {
        SetVehicleHealth(vehicleid, VehicleInfo[vehicleid][Health]);
        SetVehicleNumberPlate(vehicleid, VehicleInfo[vehicleid][Plate]);
        UpdateVehicleDamageStatus(vehicleid, VehicleInfo[vehicleid][DamagePanels], VehicleInfo[vehicleid][DamageDoors], VehicleInfo[vehicleid][DamageLights], VehicleInfo[vehicleid][DamageTires]);
        SetVehiclePos(vehicleid, VehicleInfo[vehicleid][PosX], VehicleInfo[vehicleid][PosY], VehicleInfo[vehicleid][PosZ]);
        SetVehicleZAngle(vehicleid, VehicleInfo[vehicleid][PosA]);
        LinkVehicleToInterior(vehicleid, VehicleInfo[vehicleid][Interior]);
        SetVehicleVirtualWorld(vehicleid, VehicleInfo[vehicleid][VirtualWorld]);
    }
    return 1;
}

@hook(.fallback = true) OnVehicleDeath(vehicleid, killerid)
{
    VehicleInfo[vehicleid][Handbrake] = false;
    if (GetVehicleInfoType(vehicleid) != VEHICLE_STATIC)
    {
        VehicleInfo[vehicleid][Destroyed] = true;
        SavePlayerVehicle(vehicleid);
    }
    return 1;
}

GetVehiclePoolToLimit()
return ((Iter_Count(VehicleInfo) - (Iter_Size(VehicleInfo) - 1)) * -1);

GetVehicleInfoType(vehicleid)
{
    if (!IsValidVehicle(vehicleid)) return INVALID_VEHICLE_ID;
    if (VehicleInfo[vehicleid][ID] != 0 && VehicleInfo[vehicleid][CharacterID] != 0) return VEHICLE_PLAYER;
    if (VehicleInfo[vehicleid][ID] != 0 && VehicleInfo[vehicleid][CharacterID] == 0) return VEHICLE_STATIC;
    if (VehicleInfo[vehicleid][ID] == 0 && VehicleInfo[vehicleid][CharacterID] != 0) return VEHICLE_DEMAND;
    return VEHICLE_TEMP;
}

GetVehicleMaxHealth(vehicleid, &Float:maxhealth)
{
    if (!IsValidVehicle(vehicleid)) return INVALID_VEHICLE_ID;
    switch (VehicleInfo[vehicleid][UpgradeEngine])
    {
        case 1: maxhealth = (VehicleInfo[vehicleid][MaxHealth]+500.0);
        case 2: maxhealth = (VehicleInfo[vehicleid][MaxHealth]+1000.0);
        default:
            maxhealth = VehicleInfo[vehicleid][MaxHealth];
    }
    return 1;
}

GetVehicleMaxFuel(vehicleid, &Float:maxfuel)
{
    if (!IsValidVehicle(vehicleid)) return INVALID_VEHICLE_ID;
    switch (VehicleInfo[vehicleid][UpgradeBody])
    {
        case 1: maxfuel = (VehicleInfo[vehicleid][MaxFuel]+50.0);
        case 2: maxfuel = (VehicleInfo[vehicleid][MaxFuel]+100.0);
        default:
            maxfuel = VehicleInfo[vehicleid][MaxFuel];
    }
    return 1;
}

UnloadVehicle(vehicleid)
{
    if (!IsValidVehicle(vehicleid)) return INVALID_VEHICLE_ID;
    if (VehicleInfo[vehicleid][ID] != 0)
    {
        SavePlayerVehicle(vehicleid);
        orm_destroy(VehicleInfo[vehicleid][ORM_ID]);
    }
    new const ResetVehicleInfo[E_VEHICLE_DATA];
    VehicleInfo[vehicleid] = ResetVehicleInfo;
    Iter_Remove(VehicleInfo, vehicleid);
    DestroyVehicle(vehicleid);
    return 1;
}

SetToggleVehicleEngine(vehid)
{
    new engine,lights,alarm,doors,bonnet,boot,objective;
    GetVehicleParamsEx(vehid,engine,lights,alarm,doors,bonnet,boot,objective);

    if (engine == VEHICLE_PARAMS_UNSET
            || engine == VEHICLE_PARAMS_OFF) {
        SetVehicleParamsEx(
            vehid,
            VEHICLE_PARAMS_ON,
            lights,
            alarm,
            doors,
            bonnet,
            boot,
            objective);
    } else {
        SetVehicleParamsEx(
            vehid,
            VEHICLE_PARAMS_OFF,
            lights,
            alarm,
            doors,
            bonnet,
            boot,
            objective);
    }
}

SetToggleVehicleLights(vehid)
{
    new engine,lights,alarm,doors,bonnet,boot,objective;
    GetVehicleParamsEx(vehid,engine,lights,alarm,doors,bonnet,boot,objective);

    if (lights == VEHICLE_PARAMS_UNSET
            || lights == VEHICLE_PARAMS_OFF) {
        SetVehicleParamsEx(
            vehid,
            engine,
            VEHICLE_PARAMS_ON,
            alarm,
            doors,
            bonnet,
            boot,
            objective);
    } else {
        SetVehicleParamsEx(
            vehid,
            engine,
            VEHICLE_PARAMS_OFF,
            alarm,
            doors,
            bonnet,
            boot,
            objective);
    }
}

SetToggleVehicleDoorsLocked(vehid)
{
    new engine,lights,alarm,doors,bonnet,boot,objective;
    GetVehicleParamsEx(vehid,engine,lights,alarm,doors,bonnet,boot,objective);

    if (doors == VEHICLE_PARAMS_UNSET
            || doors == VEHICLE_PARAMS_OFF) {
        SetVehicleParamsEx(
            vehid,
            engine,
            lights,
            alarm,
            VEHICLE_PARAMS_ON,
            bonnet,
            boot,
            objective);
    } else {
        SetVehicleParamsEx(
            vehid,
            engine,
            lights,
            alarm,
            VEHICLE_PARAMS_OFF,
            bonnet,
            boot,
            objective);
    }
}

SetToggleVehicleHood(vehid)
{
    new engine,lights,alarm,doors,bonnet,boot,objective;
    GetVehicleParamsEx(vehid,engine,lights,alarm,doors,bonnet,boot,objective);

    if (bonnet == VEHICLE_PARAMS_UNSET
            || bonnet == VEHICLE_PARAMS_OFF) {
        SetVehicleParamsEx(
            vehid,
            engine,
            lights,
            alarm,
            doors,
            VEHICLE_PARAMS_ON,
            boot,
            objective);
    } else {
        SetVehicleParamsEx(
            vehid,
            engine,
            lights,
            alarm,
            doors,
            VEHICLE_PARAMS_OFF,
            boot,
            objective);
    }
}

SetToggleVehicleTrunk(vehid)
{
    new engine,lights,alarm,doors,bonnet,boot,objective;
    GetVehicleParamsEx(vehid,engine,lights,alarm,doors,bonnet,boot,objective);

    if (boot == VEHICLE_PARAMS_UNSET
            || boot == VEHICLE_PARAMS_OFF) {
        SetVehicleParamsEx(
            vehid,
            engine,
            lights,
            alarm,
            doors,
            bonnet,
            VEHICLE_PARAMS_ON,
            objective);
    } else {
        SetVehicleParamsEx(
            vehid,
            engine,
            lights,
            alarm,
            doors,
            bonnet,
            VEHICLE_PARAMS_OFF,
            objective);
    }
}

SetVehicleEngineState(vehid, const setstate)
{
    new engine,lights,alarm,doors,bonnet,boot,objective;
    GetVehicleParamsEx(vehid,engine,lights,alarm,doors,bonnet,boot,objective);

    if (setstate) {
        SetVehicleParamsEx(
            vehid,
            VEHICLE_PARAMS_ON,
            lights,
            alarm,
            doors,
            bonnet,
            boot,
            objective);
    } else {
        SetVehicleParamsEx(
            vehid,
            VEHICLE_PARAMS_OFF,
            lights,
            alarm,
            doors,
            bonnet,
            boot,
            objective);
    }
}

SetVehicleLightsState(vehid, const setstate)
{
    new engine,lights,alarm,doors,bonnet,boot,objective;
    GetVehicleParamsEx(vehid,engine,lights,alarm,doors,bonnet,boot,objective);

    if (setstate) {
        SetVehicleParamsEx(
            vehid,
            engine,
            VEHICLE_PARAMS_ON,
            alarm,
            doors,
            bonnet,
            boot,
            objective);
    } else {
        SetVehicleParamsEx(
            vehid,
            engine,
            VEHICLE_PARAMS_OFF,
            alarm,
            doors,
            bonnet,
            boot,
            objective);
    }
}

SetVehicleDoorsLockedState(vehid, const setstate)
{
    new engine,lights,alarm,doors,bonnet,boot,objective;
    GetVehicleParamsEx(vehid,engine,lights,alarm,doors,bonnet,boot,objective);

    if (setstate) {
        SetVehicleParamsEx(
            vehid,
            engine,
            lights,
            alarm,
            VEHICLE_PARAMS_ON,
            bonnet,
            boot,
            objective);
    } else {
        SetVehicleParamsEx(
            vehid,
            engine,
            lights,
            alarm,
            VEHICLE_PARAMS_OFF,
            bonnet,
            boot,
            objective);
    }
}

stock SetVehicleHoodState(vehid, const setstate)
{
    new engine,lights,alarm,doors,bonnet,boot,objective;
    GetVehicleParamsEx(vehid,engine,lights,alarm,doors,bonnet,boot,objective);

    if (setstate) {
        SetVehicleParamsEx(
            vehid,
            engine,
            lights,
            alarm,
            doors,
            VEHICLE_PARAMS_ON,
            boot,
            objective);
    } else {
        SetVehicleParamsEx(
            vehid,
            engine,
            lights,
            alarm,
            doors,
            VEHICLE_PARAMS_OFF,
            boot,
            objective);
    }
}

SetVehicleTrunkState(vehid, const setstate)
{
    new engine,lights,alarm,doors,bonnet,boot,objective;
    GetVehicleParamsEx(vehid,engine,lights,alarm,doors,bonnet,boot,objective);

    if (setstate) {
        SetVehicleParamsEx(
            vehid,
            engine,
            lights,
            alarm,
            doors,
            bonnet,
            VEHICLE_PARAMS_ON,
            objective);
    } else {
        SetVehicleParamsEx(
            vehid,
            engine,
            lights,
            alarm,
            doors,
            bonnet,
            VEHICLE_PARAMS_OFF,
            objective);
    }
}

CreateOnDemandVehicle(ownerid, modelid, virtualworld, interior, Float:posx, Float:posy, Float:posz, Float:posa, color1, color2, bool:siren = false, plate[] = SERVER_NAME)
{
    if (GetVehiclePoolToLimit() < 1) return false;
    new vehicleid;
    if (siren == true) {
        vehicleid = CreateVehicle(modelid, posx, posy, posz, posa, color1, color2, -1, siren);
    } else {
        vehicleid = CreateVehicle(modelid, posx, posy, posz, posa, color1, color2, -1);
    }
    
    SetVehicleNumberPlate(vehicleid, plate);
    SetVehicleVirtualWorld(vehicleid, virtualworld);
    LinkVehicleToInterior(vehicleid, interior);

    if (plate[0] != '\0')
        format(VehicleInfo[vehicleid][Plate], 9, plate);
    VehicleInfo[vehicleid][CharacterID] = PlayerCharInfo[ownerid][ID];
    VehicleInfo[vehicleid][FactionID] = PlayerCharInfo[ownerid][FactionID];
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
    Iter_Add(VehicleInfo, vehicleid);

    return true;
}

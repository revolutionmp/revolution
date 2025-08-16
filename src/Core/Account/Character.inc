#include <YSI_Coding\y_hooks>

@hook(.fallback = true) pTask1s(playerid)
{
    if (PlayerLogged[playerid])
    {
        PlayerCharInfo[playerid][TimePlayed]++;
        PlayerAccountInfo[playerid][TimePlayed]++;
    }
    return 1;
}

ORM:CreateCharORM(playerid, bool:factionid=true)
{
    new ORM:ormid = PlayerCharInfo[playerid][ORM_ID] = orm_create("characters", Database);
    if (!factionid) orm_addvar_int(ormid, PlayerCharInfo[playerid][FactionID], "FactionID");
    orm_addvar_int(ormid, PlayerCharInfo[playerid][ID], "ID");
    orm_addvar_int(ormid, PlayerCharInfo[playerid][AccountID], "AccountID");
    orm_addvar_string(ormid, PlayerCharInfo[playerid][Nickname], 20, "Nickname");
    orm_addvar_int(ormid, PlayerCharInfo[playerid][CreatedDate], "CreatedDate");
    orm_addvar_int(ormid, PlayerCharInfo[playerid][LastLogin], "LastLogin");
    orm_addvar_int(ormid, PlayerCharInfo[playerid][TimePlayed], "TimePlayed");
    orm_addvar_int(ormid, PlayerCharInfo[playerid][Level], "Level");
    orm_addvar_int(ormid, PlayerCharInfo[playerid][Gender], "Gender");
    orm_addvar_int(ormid, PlayerCharInfo[playerid][BirthDate], "BirthDate");
    orm_addvar_string(ormid, PlayerCharInfo[playerid][Origin], 20, "Origin");
    orm_addvar_int(ormid, PlayerCharInfo[playerid][Skin], "Skin");
    orm_addvar_float(ormid, PlayerCharInfo[playerid][Health], "Health");
    orm_addvar_float(ormid, PlayerCharInfo[playerid][MaxHealth], "MaxHealth");
    orm_addvar_float(ormid, PlayerCharInfo[playerid][Armour], "Armour");
    orm_addvar_int(ormid, PlayerCharInfo[playerid][Cash], "Cash");
    orm_addvar_int(ormid, PlayerCharInfo[playerid][BankBalance], "BankBalance");
    orm_addvar_int(ormid, PlayerCharInfo[playerid][InjuredStatus], "InjuredStatus");
    orm_addvar_int(ormid, PlayerCharInfo[playerid][InjuredTime], "InjuredTime");
    orm_addvar_int(ormid, PlayerCharInfo[playerid][VirtualWorld], "VirtualWorld");
    orm_addvar_int(ormid, PlayerCharInfo[playerid][Interior], "Interior");
    orm_addvar_float(ormid, PlayerCharInfo[playerid][PosX], "PosX");
    orm_addvar_float(ormid, PlayerCharInfo[playerid][PosY], "PosY");
    orm_addvar_float(ormid, PlayerCharInfo[playerid][PosZ], "PosZ");
    orm_addvar_float(ormid, PlayerCharInfo[playerid][PosA], "PosA");
    return ormid;
}

SaveAllCharactersInfo()
foreach (new i : Player) SavePlayerCharacterInfo(i);

SavePlayerCharacterInfo(playerid)
{
    if (PlayerCharInfo[playerid][ID] == 0) return false;
    GetPlayerPos(playerid, PlayerCharInfo[playerid][PosX], PlayerCharInfo[playerid][PosY], PlayerCharInfo[playerid][PosZ]);
    GetPlayerFacingAngle(playerid, PlayerCharInfo[playerid][PosA]);
    GetPlayerHealth(playerid, PlayerCharInfo[playerid][Health]);
    GetPlayerArmour(playerid, PlayerCharInfo[playerid][Armour]);
    PlayerCharInfo[playerid][VirtualWorld] = GetPlayerVirtualWorld(playerid);
    PlayerCharInfo[playerid][Interior] = GetPlayerInterior(playerid);
    orm_update(PlayerCharInfo[playerid][ORM_ID]);
    return true;
}
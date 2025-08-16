ORM:CreateAccountORM(playerid)
{
    new ORM:ormid = PlayerAccountInfo[playerid][ORM_ID] = orm_create("accounts", Database);
    orm_addvar_int(ormid, PlayerAccountInfo[playerid][ID], "ID");
    orm_addvar_string(ormid, PlayerAccountInfo[playerid][Username], 20, "Username");
    orm_addvar_int(ormid, PlayerAccountInfo[playerid][AdminLevel], "AdminLevel");
    orm_addvar_int(ormid, PlayerAccountInfo[playerid][CreatedDate], "CreatedDate");
    orm_addvar_int(ormid, PlayerAccountInfo[playerid][LastLogin], "LastLogin");
    orm_addvar_int(ormid, PlayerAccountInfo[playerid][TimePlayed], "TimePlayed");
    return ormid;
}

SaveAllAccountInfo()
{
    foreach (new i : Player)
        if (PlayerAccountInfo[i][ID] != 0)
            orm_update(PlayerAccountInfo[i][ORM_ID]);
    return 1;
}

SavePlayerAccountInfo(playerid)
{
    if (PlayerAccountInfo[playerid][ID] == 0) return false;
    orm_update(PlayerAccountInfo[playerid][ORM_ID]);
    return true;
}
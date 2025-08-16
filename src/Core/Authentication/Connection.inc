#include <YSI_Coding\y_hooks>

@timer(100) IncomingConnection(playerid)
{
    if (!PlayerLogged[playerid])
    {
        new query[256];
        SetSpawnInfo(playerid, NO_TEAM, 299, 1759.0189, -1898.1260, 13.5622, 266.4503, 0, 0, 0, 0, 0, 0);
        TogglePlayerSpectating(playerid, false);
        if (!IsRoleplayName(GetName(playerid)))
            mysql_format(Database, query, sizeof(query), "SELECT `ID`, `Username`, `Password` FROM `accounts` WHERE `Username`='%e' LIMIT 1;", GetName(playerid));
        else
            mysql_format(Database, query, sizeof(query), "SELECT accounts.ID, accounts.Username, accounts.Password FROM characters JOIN accounts ON characters.AccountID = accounts.ID WHERE `Nickname`='%e' LIMIT 1;", GetName(playerid));
        mysql_pquery(Database, query, "LoginAccount", "dd", playerid, RaceCheck[playerid]);
    }
    return 1;
}

@hook(.fallback = true) OnPlayerDisconnect(playerid, reason)
{
    if (PlayerLogged[playerid])
    {
        SavePlayerAccountInfo(playerid);
        SavePlayerCharacterInfo(playerid);
    }

    if (!GetPVarInt(playerid, "DuplicateLogin"))
    {
        if (PlayerAccountInfo[playerid][ID] != 0)
            orm_destroy(PlayerAccountInfo[playerid][ORM_ID]);
        if (PlayerCharInfo[playerid][ID] != 0)
            orm_destroy(PlayerCharInfo[playerid][ORM_ID]);
    }
    return 1;
}

@hook(.fallback = true) OnPlayerRequestClass(playerid, classid)
{
    if (!PlayerLogged[playerid])
    {
        SetPlayerVirtualWorld(playerid, 999);
        TogglePlayerSpectating(playerid, true);
        defer IncomingConnection(playerid);
    }
    return 1;
}
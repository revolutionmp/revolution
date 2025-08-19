forward LoginAccount(playerid, check);
forward LoginAttempt(playerid, bool:success);
forward LoadAccount(playerid, check);
forward MenuCharacter(playerid, check);
forward LoadCharacter(playerid, idx, check);

public LoginAccount(playerid, check)
{
    TogglePlayerSpectating(playerid, true);
    if (check != RaceCheck[playerid])
    {
        SendErrorMessage(playerid, "ERROR: An unexpected error has occurred, please try again later!");
        defer KickPlayer(playerid);
    }
    else
    {
        if (!cache_num_rows())
        {
            Dialog_Show(playerid, PlayerUnregistered, DIALOG_STYLE_MSGBOX, "System Notice", YELLOW"This account name is unregistered!\n"WHITE"Please register at: {00FFFF}https://revolution-mp.com", "Close", "");
            defer KickPlayer(playerid);
        }
        else
        {
            cache_get_value_name_int(0, "ID", PlayerAccountInfo[playerid][ID]);
            foreach (new i : Player)
            {
                if (PlayerAccountInfo[playerid][ID] == PlayerAccountInfo[i][ID] && playerid != i)
                {
                    SetPVarInt(playerid, "DuplicateLogin", 1);
                    SendErrorMessage(playerid, "ERROR: Someone has logged in or is currently logged into this account,");
                    SendErrorMessage(playerid, "please report it to the '"SERVER_NAME"' discord if this is your account!");
                    defer KickPlayer(playerid);
                    return false;
                }
            }

            new hash[BCRYPT_HASH_LENGTH];
            cache_get_value_name(0, "Username", PlayerAccountInfo[playerid][Username], 20);
            cache_get_value_name(0, "Password", hash, sizeof(hash));
            SetPVarString(playerid, "Hash", hash);
            if (!IsRoleplayName(GetName(playerid)))
                Dialog_Show(playerid, MatchingPassword, DIALOG_STYLE_PASSWORD, "Login to "SERVER_NAME, WHITE"UCP Account: "CYAN"%s\n"WHITE"Attempts: "CYAN"0/5\n"WHITE"Password: "YELLOW"(input below)", "Login", "Quit", PlayerAccountInfo[playerid][Username]);
            else
                Dialog_Show(playerid, MatchingPassword, DIALOG_STYLE_PASSWORD, "Login to "SERVER_NAME, WHITE"UCP Account: "CYAN"%s\n"WHITE"Character: "CYAN"%s\n"WHITE"Attempts: "CYAN"0/5\n"WHITE"Password: "YELLOW"(input below)", "Login", "Quit", PlayerAccountInfo[playerid][Username], GetName(playerid));
        }
    }
    return true;
}

Dialog:MatchingPassword(playerid, response, listitem, const inputtext[])
{
    if (!response)
        Kick(playerid);
    else
    {
        new hash[BCRYPT_HASH_LENGTH];
        GetPVarString(playerid, "Hash", hash, sizeof(hash));
        bcrypt_verify(playerid, "LoginAttempt", inputtext, hash);
    }
    return 1;
}

public LoginAttempt(playerid, bool:success)
{
    if (success)
    {
        new query[128];
        DeletePVar(playerid, "Attempts");
        DeletePVar(playerid, "Hash");
        mysql_format(Database, query, sizeof(query), "SELECT `ID`, `Username`, `AdminLevel`, `CreatedDate`, `LastLogin`, `TimePlayed` FROM `accounts` WHERE `ID`='%u' LIMIT 1;", PlayerAccountInfo[playerid][ID]);
        mysql_pquery(Database, query, "LoadAccount", "dd", playerid, RaceCheck[playerid]);
    }
    else
    {
        SetPVarInt(playerid, "Attempts", (GetPVarInt(playerid, "Attempts") + 1));
        if (GetPVarInt(playerid, "Attempts") > 4)
            return Kick(playerid);
        else if (!IsRoleplayName(GetName(playerid)))
            Dialog_Show(playerid, MatchingPassword, DIALOG_STYLE_PASSWORD, "Login to "SERVER_NAME, WHITE"UCP Account: "CYAN"%s\n"WHITE"Attempts: "CYAN"%d/5\n"WHITE"Password: "YELLOW"(input below)", "Login", "Quit", PlayerAccountInfo[playerid][Username], (GetPVarInt(playerid, "Attempts")));
        else
            Dialog_Show(playerid, MatchingPassword, DIALOG_STYLE_PASSWORD, "Login to "SERVER_NAME, WHITE"UCP Account: "CYAN"%s\n"WHITE"Character: "CYAN"%s\n"WHITE"Attempts: "CYAN"%d/5\n"WHITE"Password: "YELLOW"(input below)", "Login", "Quit", PlayerAccountInfo[playerid][Username], GetName(playerid), (GetPVarInt(playerid, "Attempts")));
    }
    return 1;
}

public LoadAccount(playerid, check)
{
    if (check != RaceCheck[playerid] || !cache_num_rows())
    {
        SendErrorMessage(playerid, "ERROR: An unexpected error has occurred, please try again later!");
        defer KickPlayer(playerid);
    }
    else
    {
        new query[128];
        CreateAccountORM(playerid);
        orm_setkey(PlayerAccountInfo[playerid][ORM_ID], "ID");
        orm_apply_cache(PlayerAccountInfo[playerid][ORM_ID], 0);
        PlayerAccountInfo[playerid][LastLogin] = gettime();
        if (!IsRoleplayName(GetName(playerid)))
        {
            mysql_format(Database, query, sizeof(query), "SELECT `Nickname`, `Level`, `LastLogin` FROM `characters` WHERE `AccountID`='%u';", PlayerAccountInfo[playerid][ID]);
            mysql_pquery(Database, query, "MenuCharacter", "dd", playerid, RaceCheck[playerid]);
        }
        else
        {
            mysql_format(Database, query, sizeof(query), "SELECT * FROM `characters` WHERE `Nickname`='%e' LIMIT 1;", GetName(playerid));
            mysql_pquery(Database, query, "LoadCharacter", "ddd", playerid, 0, RaceCheck[playerid]);
        }
    }
    return 1;
}

public MenuCharacter(playerid, check)
{
    if (check != RaceCheck[playerid])
    {
        SendErrorMessage(playerid, "ERROR: An unexpected error has occurred, please try again later!");
        defer KickPlayer(playerid);
    }
    else
    {
        new rows = cache_num_rows(), dialog[512], string[256];
        strcat(dialog, "Nickname\tLevel\tLast Login\n");
        if (rows)
        {
            new nickname[20], level, lastlogin;
            forex(i, rows)
            {
                cache_get_value_name(i, "Nickname", nickname, sizeof(nickname));
                cache_get_value_name_int(i, "Level", level);
                cache_get_value_name_int(i, "LastLogin", lastlogin);
                if (lastlogin == 0)
                {
                    if (!IsOdd(i)) format(string, sizeof(string), WHITE"%s\t%d\t"GREEN"New\n", nickname, level);
                    else format(string, sizeof(string), GRAY"%s\t"GRAY"%d\t"GREEN"New\n", nickname, level);
                }
                else
                {
                    if (!IsOdd(i)) format(string, sizeof(string), WHITE"%s\t%d\t%s\n", nickname, level, GetTimeDate(lastlogin));
                    else format(string, sizeof(string), GRAY"%s\t"GRAY"%d\t"GRAY"%s\n", nickname, level, GetTimeDate(lastlogin));
                }
                strcat(dialog, string);
            }
        }
        Dialog_Show(playerid, SelectCharacter, DIALOG_STYLE_TABLIST_HEADERS, "Select Character", dialog, "Select", "");
    }
    return 1;
}

Dialog:SelectCharacter(playerid, response, listitem, const inputtext[])
{
    if (!response)
        Kick(playerid);
    else
    {
        new query[128];
        mysql_format(Database, query, sizeof(query), "SELECT * FROM `characters` WHERE `AccountID`='%u';", PlayerAccountInfo[playerid][ID]);
        mysql_pquery(Database, query, "LoadCharacter", "ddd", playerid, listitem, RaceCheck[playerid]);
    }
    return 1;
}

public LoadCharacter(playerid, idx, check)
{
    if (check != RaceCheck[playerid] || !cache_num_rows())
    {
        SendErrorMessage(playerid, "ERROR: An unexpected error has occurred, please try again later!");
        defer KickPlayer(playerid);
    }
    else
    {
        new query[512], bool:factionid;
        SetPVarInt(playerid, "curtick", GetTickCount());
        cache_is_value_name_null(idx, "FactionID", factionid);
        CreateCharORM(playerid, factionid);
        orm_setkey(PlayerCharInfo[playerid][ORM_ID], "ID");
        orm_apply_cache(PlayerCharInfo[playerid][ORM_ID], idx);
        PlayerCharInfo[playerid][LastLogin] = gettime();
        SetPlayerName(playerid, PlayerCharInfo[playerid][Nickname]);
        SetPlayerScore(playerid, PlayerCharInfo[playerid][Level]);
        SetPlayerColor(playerid, X11_WHITE);
        if (!PlayerCharInfo[playerid][TimePlayed])
        {
            PlayerCharInfo[playerid][Skin] = 299;
            SetSpawnInfo(playerid, NO_TEAM, PlayerCharInfo[playerid][Skin], 1759.0189, -1898.1260, 13.5622, 266.4503, 0, 0, 0, 0, 0, 0);
            SetPlayerVirtualWorld(playerid, 0);
            SetPlayerInterior(playerid, 0);
        }
        else
        {
            SetSpawnInfo(playerid, NO_TEAM, PlayerCharInfo[playerid][Skin], PlayerCharInfo[playerid][PosX], PlayerCharInfo[playerid][PosY], PlayerCharInfo[playerid][PosZ], PlayerCharInfo[playerid][PosA], 0, 0, 0, 0, 0, 0);
            SetPlayerVirtualWorld(playerid, PlayerCharInfo[playerid][VirtualWorld]);
            SetPlayerInterior(playerid, PlayerCharInfo[playerid][Interior]);
        }
        SetCameraBehindPlayer(playerid);
        SetPlayerHealth(playerid, PlayerCharInfo[playerid][Health]);
        SetPlayerArmour(playerid, PlayerCharInfo[playerid][Armour]);

        mysql_format(Database, query, sizeof(query), "SELECT `ID`, `CharacterID`, `ShareKeyID`, `Model`, `Plate`, `Insurance`, `Locked`, `Handbrake`, `Destroyed`, `Health`, `MaxHealth`, `Fuel`, `MaxFuel`, `UpgradeEngine`, `UpgradeBody`, `UpgradeFuel`, `DamagePanels`, `DamageDoors`, `DamageLights`, `DamageTires`, `VirtualWorld`, `Interior`, `PosX`, `PosY`, `PosZ`, `PosA`, `Color1`, `Color2`, `Siren`, `RespawnDelay` FROM `vehicles` WHERE `CharacterID`='%u' AND ParkingID IS NULL AND Destroyed='0';", PlayerCharInfo[playerid][ID]);
        mysql_pquery(Database, query, "LoadPlayerVehicle", "bd", playerid, RaceCheck[playerid]);

        TogglePlayerSpectating(playerid, false);
        TogglePlayerControllable(playerid, true);
        CallRemoteFunction("OnPlayerLogged", "i", playerid);
        PlayerLogged[playerid] = true;
        SendClientMessage(playerid, X11_CYAN, "SERVER: "WHITE"Server memerlukan waktu, "YELLOW"%d milisecond "WHITE"untuk memuat data char anda!", (GetTickCount() - GetPVarInt(playerid, "curtick")));
        SendClientMessage(playerid, X11_CYAN, "NOTICE: "WHITE"This feature is under construction, "YELLOW"please wait soon after updates"WHITE"!");
        DeletePVar(playerid, "curtick");
    }
    return 1;
}
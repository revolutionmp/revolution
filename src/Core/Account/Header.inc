enum E_ACCOUNT_DATA
{
    ORM:ORM_ID,
    ID,
    Username[20],
    AdminLevel,
    CreatedDate,
    LastLogin,
    TimePlayed
}

enum E_CHARACTER_DATA
{
    ORM:ORM_ID,
    ID,
    AccountID,
    FactionID,
    Nickname[20],
    CreatedDate,
    LastLogin,
    TimePlayed,
    Level,
    bool:Gender,
    BirthDate,
    Origin[32],
    Skin,
    Float:Health,
    Float:MaxHealth,
    Float:Armour,
    Cash,
    BankBalance,
    bool:InjuredStatus,
    InjuredTime,
    VirtualWorld,
    Interior,
    Float:PosX,
    Float:PosY,
    Float:PosZ,
    Float:PosA
}

new PlayerAccountInfo[MAX_PLAYERS][E_ACCOUNT_DATA];
new PlayerCharInfo[MAX_PLAYERS][E_CHARACTER_DATA];
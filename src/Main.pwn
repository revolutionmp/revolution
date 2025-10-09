#include <a_samp>                      // This MUST be include for SA:MP.

/// This will be useless if your host resets MAX_PLAYERS to a specific player slot limit.
/// Limits: 1000
#if defined MAX_PLAYERS
    #undef MAX_PLAYERS
    #define MAX_PLAYERS        100     // 100 Max player's id
#else
    #define MAX_PLAYERS        100
#endif

/// Limits: 2000
#if defined MAX_VEHICLES
    #undef MAX_VEHICLES
    #define MAX_VEHICLES       1801    // 1800 Max vehicle's id
#else
    #define MAX_VEHICLES       1801
#endif

#define SERVER_NAME           "RevolutionMP"
#define SERVER_NAME_SHORT     "R:MP"
#define MODE_VERSION          "R:MP v0.1.0 Alpha"

#define FOREACH_NO_BOTS              // Disabled the "NPC", "Bot", and "Character" from iterators.
#define FOREACH_NO_LOCALS           // Disabled the "LocalActor" and "LocalVehicle" from iterators.
#define YSI_NO_HEAP_MALLOC          // The AMX is much larger because the allocation pool is embedded in the file.
#define easyDialog_Mobile           // Disable "HidePlayerDialog" for mobile users, that have issue with empty dialog.

#define AC_USE_CONFIG_FILES   false
#define NO_SUSPICION_LOGS

#include <sscanf2>                    // Allows us to read formatted data from a string rather than standard input.
#include <crashdetect>               // Debug runtime errors and server crashes. (Can't use with jit plugin)
#include <streamer>                  // This plugin streams objects, pickups, checkpoints, etc.
#include <YSI_Coding\y_va>           // Passes all variable parameters to another function.
#include <YSI_Coding\y_timers>       // Wraps SetTimer and SetTimerEx to give compile-time parameter type checks.
#include <YSI_Coding\y_hooks>        // Provides support for 'hooking' functions.
#include <YSI_Data\y_iterate>        // foreach with many extras for iterators and special iterators.
#include <YSI_Server\y_colours>      // Functions for manipulating colours, with many named colours.
#include <YSI_Visual\y_commands>     // Fully featured command processor for SA:MP.
#include <a_mysql>                   // Allows you to use MySQL in PAWN.
#include <samp_bcrypt>               // A bcrypt plugin for samp. written in Rust.
#include <distance>                  // Proximity/distance checks between entities.
#include <VehiclePartPosition>       // Get world position of specific vehicle parts.
#include <chrono>                    // A modern Pawn library for working with dates and times.
#include <Pawn.Regex>                // Adds regex support in Pawn.
#include <easyDialog>                // Makes dialogs easier to use.
#include <ndialog-pages>             // Create paged dialog lists with Next button support.

// Temporary Disable Anti-Cheat Nex-AC
#include <nex-ac_en.lang>
#include <nex-ac>                    // Nex Anticheat — anti-cheat and DoS protection.

#include "Utils/Global"
#include "Utils/AntiCheat"
#include "Utils/Anims"
#include "Core/Entry"
#include "Modules/VehicleCmds"
#include "Modules/VoiceCmds"
#include "Prototypes/Cmds"

// Temporary Unused Function
#pragma unused GetPlayerOwnVehicle
#pragma unused SavePlayerOwnVehicle
#pragma unused CreateOnDemandVehicle
#pragma unused CreateTemporaryVehicle

main() {}

public OnGameModeInit()
{
	new curtick = GetTickCount();
    new MySQLOpt:options = mysql_init_options();
    mysql_set_option(options, POOL_SIZE, 5);
    Database = mysql_connect_file();
    mysql_log(ALL);

    if (mysql_errno(Database) != 0)
    {
        printf("[MySQL] Couldn't connect to the database (%d).", mysql_errno(Database));
        SendRconCommand("exit");
    }
    else
    {
        new query[128];
        printf("[MySQL] Connected to the database successfully (%d).", _:Database);
        mysql_set_charset("utf8mb4", Database);
        SetGameModeText(MODE_VERSION);         // Set the name of the game mode, which appears in the server browser (client).
        DisableInteriorEnterExits();           // Disable all the interior entrances and exits in the game (the yellow arrows at doors).
        EnableVehicleFriendlyFire();           // Enable friendly fire for team vehicles. Players will be unable to damage teammates' vehicles (SetPlayerTeam must be used!).
        ShowPlayerMarkers(false);              // Change the colour of a player's nametag and radar blip for another player.
        ShowNameTags(false);                   // Toggle the drawing of nametags, health bars and armor bars above players.
        //ManualVehicleEngineAndLights();      // This prevents the game automatically turning the engine on/off when players enter/exit vehicles and headlights automatically coming on when it is dark.
        SetNameTagDrawDistance(21.0);          // Set the maximum distance to display the names of players.
        EnableStuntBonusForAll(false);         // Enables or disables stunt bonuses for all players. If enabled, players will receive monetary rewards when performing a stunt in a vehicle (e.g. a wheelie).

        Streamer_TickRate(150);
        Streamer_VisibleItems(STREAMER_TYPE_OBJECT, 300);

        mysql_format(Database, query, sizeof(query), "SELECT * FROM vehicles WHERE CharacterID IS NULL;");
        mysql_pquery(Database, query, "LoadStaticVehicle");

        printf("Server needs %d millisecond to load the gamemode!", (GetTickCount() - curtick));
        defer UnlockServer();
    }
	return 1;
}

public OnGameModeExit()
{
	SaveAllAccountInfo();
    SaveAllCharactersInfo();
	DestroyAllDynamic3DTextLabels();
    DestroyAllDynamicAreas();
    DestroyAllDynamicCPs();
    DestroyAllDynamicMapIcons();
    DestroyAllDynamicObjects();
    DestroyAllDynamicPickups();
    DestroyAllDynamicRaceCPs();
	for (new i = 0, j = GetPlayerPoolSize(); i <= j; i++) 
	{
		if (IsPlayerConnected(i))
			KickPlayer(i);
	}
    mysql_close(Database);
	return 1;
}

public OnPlayerConnect(playerid)
{
    RaceCheck[playerid]++;
    Streamer_ToggleIdleUpdate(playerid, 1);
    Streamer_ToggleCameraUpdate(playerid, 1);
    SetPlayerColor(playerid, X11_GRAY);
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    if (PlayerLogged[playerid])
        UnloadPlayerOwnVehicle(playerid);
	PreloadAnimLibs(playerid);
    static const ResetCharInfo[E_CHARACTER_DATA];
    static const ResetAccountInfo[E_ACCOUNT_DATA];
    PlayerCharInfo[playerid] = ResetCharInfo;
    PlayerAccountInfo[playerid] = ResetAccountInfo;
    PlayerLogged[playerid] = false;
    RaceCheck[playerid]++;
    return 1;
}

public OnQueryError(errorid, const error[], const callback[], const query[], MySQL:handle)
{
    switch (errorid)
    {
        case CR_SERVER_GONE_ERROR: printf("Lost connection to server");
        case ER_SYNTAX_ERROR: printf("Something is wrong in your syntax, query: %s", query);
    }
    return 1;
}

public e_COMMAND_ERRORS:OnPlayerCommandReceived(playerid, cmdtext[], e_COMMAND_ERRORS:success)
{
    if (success == COMMAND_UNDEFINED)
        SendErrorMessage(playerid, "ERROR: Unknown command, see '/help' for a few commands information.");
    return COMMAND_OK;
}

public e_COMMAND_ERRORS:OnPlayerCommandPerformed(playerid, cmdtext[], e_COMMAND_ERRORS:success)
{
    printf("[COMMAND]: playerid=%d playername=%s cmdtext=%s success=%d", playerid, GetName(playerid), cmdtext, success);
    if (!IsPlayerConnected(playerid)) return COMMAND_NO_PLAYER;
    return COMMAND_OK;
}

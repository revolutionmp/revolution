#include <a_samp> // This MUST be include for SA:MP.

#if defined MAX_PLAYERS
    #undef MAX_PLAYERS
    #define MAX_PLAYERS	100 // 100 Max player's id
#else
    #define MAX_PLAYERS    100
#endif
#if defined MAX_VEHICLES
    #undef MAX_VEHICLES
    #define MAX_VEHICLES	1801 // 1800 Max vehicle's id
#else
    #define MAX_VEHICLES    1801
#endif

#define SERVER_NAME         "RevolutionMP"
#define SERVER_NAME_SHORT   "R:MP"
#define MODE_VERSION        "R:MP v0.1.0 Alpha"

#define FOREACH_NO_BOTS // Disabled the "NPC", "Bot", and "Character" from iterators.
#define FOREACH_NO_LOCALS // Disabled the "LocalActor" and "LocalVehicle" from iterators.
#define YSI_NO_HEAP_MALLOC // The AMX is much larger because the allocation pool is embedded in the file.
#define easyDialog_Mobile // Disable "HidePlayerDialog" for mobile users, that have issue with empty dialog.

#define AC_USE_CONFIG_FILES	false
#define NO_SUSPICION_LOGS

#include <sscanf2> // Allows us to read formatted data from a string rather than standard input.
#include <crashdetect> // Debug runtime errors and server crashes. (Can't use with jit plugin)
#include <streamer> // This plugin streams objects, pickups, checkpoints, race checkpoints, map icons, 3D text labels, and actors at user-defined server ticks.
#include <YSI_Coding\y_va> // Provides ___, a companion to ..., which passes all variable parameters to another function instead of receiving them.
#include <YSI_Coding\y_timers> // Wraps SetTimer and SetTimerEx to give compile-time parameter type checks.
#include <YSI_Coding\y_hooks> // Provides language syntax and support for 'hooking' functions. Allowing you to intercept them, or use the same callback in multiple files at once.
#include <YSI_Data\y_iterate> // The latest version of foreach with many extras for iterators and special iterators.
#include <YSI_Server\y_colours> // Provides many functions for manipulating colours, as well as several thousand pre-defined named colours
#include <YSI_Visual\y_commands> // The most fully featured command processor for SA:MP.
#include <a_mysql> // This plugin allows you to use MySQL in PAWN.
#include <samp_bcrypt> // A bcrypt plugin for samp in Rust.
#include <distance> // Performing proximity/distance checks between them and finding the closest entities to other entities.
#include <VehiclePartPosition> // This function to obtain the position in the world of a certain part of the vehicle, being able to define a distance between the part of the vehicle and the position in the world
#include <chrono> // A modern Pawn library for working with dates and times.
#include <Pawn.Regex> // Plugin that adds support for regular expressions in Pawn.
#include <easyDialog> // The purpose of this include is to make dialogs easier to use in general.
#include <ndialog-pages> // Dialog Pages adds the possibility to create paged dialog lists. It will basically calculate how many items will fit into one page and generate the Next button if there are too many.

// Temporary Disable Anti-Cheat Nex-AC
#include <nex-ac_en.lang>
#include <nex-ac> // Nex Anticheat (Nex-AC) - is a comprehensive protection which combines powerful anticheat and protection against various attacks (flood, DoS).

#include "Utils/Global"
#include "Utils/AntiCheat"
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
        SetGameModeText(MODE_VERSION); // Set the name of the game mode, which appears in the server browser (client).
        DisableInteriorEnterExits(); // Disable all the interior entrances and exits in the game (the yellow arrows at doors).
        EnableVehicleFriendlyFire(); // Enable friendly fire for team vehicles. Players will be unable to damage teammates' vehicles (SetPlayerTeam must be used!).
        ShowPlayerMarkers(false); // https://open.mp/docs/scripting/functions/SetPlayerMarkerForPlayer
        ShowNameTags(false); // https://open.mp/docs/scripting/functions/ShowNameTags
        //ManualVehicleEngineAndLights(); // This prevents the game automatically turning the engine on/off when players enter/exit vehicles and headlights automatically coming on when it is dark.
        SetNameTagDrawDistance(21.0); // https://open.mp/docs/scripting/functions/SetNameTagDrawDistance
        EnableStuntBonusForAll(false); // Enables or disables stunt bonuses for all players. If enabled, players will receive monetary rewards when performing a stunt in a vehicle (e.g. a wheelie).

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
			KickEx(playerid);
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

public OnPlayerCommandPerformed(playerid, cmdtext[], success)
{
    new size_names[MAX_PLAYER_NAME]
    	;
    GetPlayerName(playerid, size_names, sizeof(size_names));
    printf("[COMMAND] %s: %s", size_names, cmdtext);
    
    /* not connected */
    if (!IsPlayerConnected(playerid)) return 0;
    return 1;
}

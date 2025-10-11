#include <YSI_Coding\y_hooks>
#include <YSI_Coding\y_va>

/*
 <summary> A shorthand loop macro for iterating from 0 up to a given limit. </summary>
 <summary> This macro replaces a standard for-loop pattern for cleaner code. </summary>
 <summary> Use this when you need a compact loop declaration with a variable index. </summary>
*/
#define forex(%0,%1) for(new %0 = 0; %0 < %1; %0++)

/*
 <summary> Checks if a number is odd using bitwise AND. </summary>
 <summary> Returns true if the least significant bit is set (odd number). </summary>
 <summary> Use this when you need a quick odd/even check without division. </summary>
*/
#define IsOdd(%0) ((%0)&1)

/*
 <summary> Detects if a specific key has been pressed (newly pressed). </summary>
 <summary> Compares newkeys and oldkeys to check for fresh input. </summary>
 <summary> Use this when you want to detect key press events only once per press. </summary>
*/
#define Pressed(%0) ((newkeys & %0) && !(oldkeys & %0))

/*
 <summary> Checks if a specific key is currently being held down. </summary>
 <summary> It evaluates whether the key is continuously active in newkeys. </summary>
 <summary> Use this when you need to track sustained input holding (e.g., movement keys). </summary>
*/
#define Holding(%0) ((newkeys & (%0)) == (%0))

/*
 <summary> Tests if a given key set matches another key mask. </summary>
 <summary> Performs a bitwise check between the pressed keys and a target mask. </summary>
 <summary> Use this when you want to validate combined key input (e.g., CTRL + another key). </summary>
*/
#define Pressing(%0,%1) (%0 & (%1))

/*
 <summary> Detects if a specific key has been released. </summary>
 <summary> It verifies that the key was pressed before but is not pressed anymore. </summary>
 <summary> Use this when you need to trigger events on key release rather than press. </summary>
*/
#define Released(%0) (((newkeys & (%0)) != (%0)) && ((oldkeys & (%0)) == (%0)))

/*
 <summary> Retrieves a vehicle’s model name based on its ID. </summary>
 <summary> This accesses the VehicleData array to return the string name. </summary>
 <summary> Use this when displaying vehicle names in chat, logs, or UI. </summary>
*/
#define GetVehicleModelName(%0)	VehicleData[((%0)-400)][Name]

/*
 <summary> Sends a red-colored error message to a player. </summary>
 <summary> This provides user feedback when something goes wrong. </summary>
 <summary> Use this for standardized error reporting to players. </summary>
*/
#define SendErrorMessage(%0,%1) SendClientMessage(%0,0xBFC0C200,%1)

/*
 <summary> Sends a privilege error message to a player. </summary>
 <summary> This tells players they lack permission to use a command. </summary>
 <summary> Use this to notify non-admins about restricted commands. </summary>
*/
#define SendPrivilegeMessage(%0) SendClientMessage(%0,0xBFC0C200,"ERROR: You don't have the privilege to use this command!")

new MySQL:Database;
new RaceCheck[MAX_PLAYERS];
new bool:PlayerLogged[MAX_PLAYERS];

/*
 <summary> Global timezone offset in seconds from UTC. </summary>
 <summary> Default is 25200 seconds (7 hours), which corresponds to GMT+7 (Jakarta/WIB). </summary>
 <summary> Use this when you need to adjust timestamps for local timezones; change the value if server runs in another region. </summary>
*/
new GMT	= 25200;

/*
 <summary> This task is a placeholder function that executes every second. </summary>
 <summary> It is used as a timed recurring callback with a 1-second interval. </summary>
 <summary> Use this when you need a periodic task that runs every second per player. </summary>
*/
ptask pTask1s[1000](playerid)
	return 1;

/*
 <summary> This task is a placeholder function that executes every 100 milliseconds. </summary>
 <summary> It is useful for rapid checks or updates that need high frequency execution. </summary>
 <summary> Use this if you require fast, sub-second interval tasks (e.g., anti-cheat, fast timers). </summary>
*/
ptask pTask100ms[100](playerid) 
	return 1;

/*
 <summary> Unlocks the server by resetting the RCON password to zero. </summary>
 <summary> This is intended for scenarios where temporary locking is applied and must be lifted. </summary>
 <summary> Use this when you want to automatically unlock the server after a delay. </summary>
*/
@timer(2000) UnlockServer()
{
	SendRconCommand("password 0");
	return 1;
}

/*
 <summary> Custom OnPlayerUpdate task that executes every 100 ms for each player. </summary>
 <summary> This is mainly for handling real-time updates or fast player state checks. </summary>
 <summary> SA-MP’s native OnPlayerUpdate callback runs ~30 times per second (≈ every 33 ms), while this custom OnPlayerUpdateEx runs only 10 times per second (every 100 ms). </summary>
 <summary> Use this if you need to track player actions at shorter intervals but with reduced frequency compared to the native callback (lighter on performance). </summary>
*/
ptask OnPlayerUpdateEx[100](playerid)
{
	// OnPlayerUpdate 100/ms
	return 1;
}

/*
 <summary> Kicks a specific player from the server. </summary>
 <summary> This ensures forced disconnection after a timed interval. </summary>
 <summary> Use this when a player must be removed (e.g., after rule violation or inactivity). </summary>
*/
@timer(100) KickPlayer(playerid)
{
	Kick(playerid);
	return 1;
}

/*
 <summary> Retrieves the name of a player based on their ID. </summary>
 <summary> This function simplifies player name fetching into a string. </summary>
 <summary> Use this when you need the player’s display name (e.g., for chat logs, UI, or admin tools). </summary>
*/
GetName(playerid)
{
	new playername[MAX_PLAYER_NAME+1];
	GetPlayerName(playerid,playername,sizeof(playername));
	return playername;
}

/*
 <summary> Converts a UNIX timestamp into a formatted date and time string. </summary>
 <summary> This helps in displaying human-readable time from server-side timestamps. </summary>
 <summary> Use this when you need to show players a readable datetime format adjusted with GMT. </summary>
*/
GetTimeDate(timestamp)
{
    new Timestamp:ts = Timestamp:(timestamp+GMT), string[256];
    TimeFormat(ts,"%a %d %b %Y, %T",string);
    return string;
}

/*
 <summary>Sends a formatted message to a player with a specified color.</summary>
 <summary>This wraps SendClientMessage with support for format strings and variable arguments (like printf).</summary>
 <summary>Use this when you want to include dynamic values (e.g., player names, numbers) in your messages.</summary>
 <param name="playerid">The target player ID to receive the message.</param>
 <param name="colour">The color of the message in ARGB format (e.g., 0xFFFFFFFF for white).</param>
 <param name="fmat">The format string (printf-style).</param>
 <param name="...">Additional arguments to be formatted into the message.</param>
 <returns>Returns 1 after sending the message.</returns>
*/
#if ! defined SendClientMessageEx
	#define SendClientMessageEx _@SendClientMessageEx
#endif
stock _@SendClientMessageEx(playerid, colour, const fmat[],  va_args<>)
{
	new
		str[145];
	va_format(str, sizeof (str), fmat, va_start<3>);
	return SendClientMessage(playerid, colour, str);
}

/*
 <summary>Sends a formatted message from one player to another or to all players within a specified range.</summary>
 <summary>Supports formatted strings (varargs) and optionally displays a chat bubble above the sender’s head.</summary>
 <summary>Use this for local chat where only nearby players receive the message.</summary>
 <param name="playerid">The player sending the message.</param>
 <param name="targetid">The target player. Use INVALID_PLAYER_ID (-1) to broadcast to all players in range.</param>
 <param name="range">Maximum distance the message can be seen (default: 1.0).</param>
 <param name="color">Message text color (default: white 0xFFFFFFFF).</param>
 <param name="bubble">Show chat bubble above sender (0 = off, 1 = on).</param>
 <param name="fmat">Format string for the message (printf-style).</param>
 <param name="...">Additional arguments for formatting (varargs).</param>
 <returns>Always 1 (success).</returns>
*/
SendClientMessageInRange(playerid, targetid, Float:range = 1.0, color = 0xFFFFFFFF, bubble = 0, const fmat[], {Float, _}:...)
{
    new
		Float:posX,
		Float:posY,
		Float:posZ
	;
    GetPlayerPos(playerid, posX, posY, posZ);

    new str[145];
    va_format(str, sizeof(str), fmat, va_start<6>);

    if (targetid == INVALID_PLAYER_ID || targetid == -1)
    {
        for (new i = 0; i < GetMaxPlayers(); i++)
        {
            if (!IsPlayerConnected(i)) continue;
            if (i == playerid) continue;
            if (IsPlayerInRangeOfPoint(i, range, posX, posY, posZ)) 
                SendClientMessage(i, color, str);
            if (bubble != 0)
                SetPlayerChatBubble(playerid, str, color, range, 10000);
        }
    }
    else
	{
        if (IsPlayerConnected(targetid) && targetid != playerid)
        {
            if (IsPlayerInRangeOfPoint(targetid, range, posX, posY, posZ))
                SendClientMessage(targetid, color, str);
            if (bubble != 0)
                SetPlayerChatBubble(playerid, str, color, range, 10000);
        }
    }
    return 1;
}
				
/*
 <summary> Retrieves the admin level of a player. </summary>
 <summary> This checks the PlayerAccountInfo and returns the privilege level. </summary>
 <summary> Use this when determining if a player has admin rights for commands or actions. </summary>
*/
#if ! defined GetPlayerAdmin
	#define GetPlayerAdmin _@GetPlayerAdmin
#endif
stock _@GetPlayerAdmin(playerid)
{
	new
		ret = PlayerAccountInfo[playerid][AdminLevel]
	;
	if (ret < 1) {
		ret = -1;
	}
	return ret;
}

/*
 <summary> Validates whether a nickname follows the roleplay naming convention. </summary>
 <summary> This uses a regular expression to enforce "Firstname_Lastname" format. </summary>
 <summary> Use this to restrict player names and ensure they fit roleplay server rules. </summary>
*/
IsRoleplayName(const nickname[])
{
    static Regex:regex;
    if(!regex) regex = Regex_New("[A-Z][a-z]+_[A-Z][a-z]+");
    return Regex_Check(nickname, regex);
}

#include <YSI_Coding\y_hooks>

@hook(.fallback = true) OnGameModeInit()
{
	EnableAntiCheat(14, false); // Anti-Money hack
	EnableAntiCheat(15, false); // Anti-Weapon hack
	EnableAntiCheat(16, false); // Anti-Ammo hack (add)
	EnableAntiCheat(17, false); // Anti-Ammo hack (infinite)
	EnableAntiCheat(19, false); // Anti-GodMode from bullets (onfoot)
	EnableAntiCheat(20, false); // Anti-GodMode from bullets (in vehicle)
	EnableAntiCheat(21, false); // Anti-Invisible hack
	EnableAntiCheat(24, false); // Anti-Parkour mod
	EnableAntiCheat(25, false); // Anti-Quick turn
	EnableAntiCheat(27, false); // Anti-FakeSpawn
	EnableAntiCheat(33, false); // Anti-UnFreeze
	EnableAntiCheat(35, false); // Anti-Full Aiming
	EnableAntiCheat(36, false); // Anti-Fake NPC
	EnableAntiCheat(37, false); // Anti-Reconnect
	EnableAntiCheat(39, false); // Anti-Dialog hack
	EnableAntiCheat(40, false); // Protection from sandbox
	// EnableAntiCheat(41, false); // Protection from invalid version
	EnableAntiCheat(47, false); // Anti-Weapon Crasher

	EnableAntiNOP(1, false); // Anti-NOP SetPlayerAmmo
	EnableAntiNOP(3, false); // Anti-NOP SetPlayerHealth
	return 1;
}
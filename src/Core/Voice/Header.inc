#define MAX_RADIO_CHANNELS      10

#define PROXIMITY_MODE_WHISPER      1
#define PROXIMITY_MODE_NORMAL       2
#define PROXIMITY_MODE_SHOUT        3

enum E_PLAYER_VOICE_DATA
{
    ProximityMode,
    RadioChannel,
    PhoneCallTarget,
    SV_DLSTREAM:ProximityStream,
    SV_GSTREAM:PhoneCallStream
}

new PlayerVoiceData[MAX_PLAYERS][E_PLAYER_VOICE_DATA];
new SV_GSTREAM:RadioStream[MAX_RADIO_CHANNELS];
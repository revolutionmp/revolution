#include <a_samp>
#include <sampvoice>

#include "Core/Voice/Header"
#include "Core/Voice/Proximity"
#include "Core/Voice/PhoneCall"
#include "Core/Voice/Radio"

forward OnPlayerLogged(playerid);

public OnPlayerLogged(playerid)
{
    if (!(PlayerVoiceData[playerid][ProximityStream] = SvCreateDLStreamAtPlayer(10.0, SV_INFINITY, playerid, 0xFFFFFFFF, "")))
        SendClientMessage(playerid, -1, "Error: Gagal membuat voice stream.");
    else
    {
        PlayerVoiceData[playerid][ProximityMode] = PROXIMITY_MODE_NORMAL;
        SendClientMessage(playerid, -1, "Press B to talk to local chat.");
        SvAddKey(playerid, 0x42);
    }
    return 1;
}

public OnFilterScriptInit()
{
    for(new i; i < MAX_RADIO_CHANNELS; ++i)
        RadioStream[i] = SvCreateGStream(0xFFFFFF00, "");
    return 1;
}

public OnFilterScriptExit()
{
    for(new i; i < MAX_RADIO_CHANNELS; ++i)
        SvDeleteStream(RadioStream[i]);
    return 1;
}

public OnPlayerConnect(playerid)
{
    if (SvGetVersion(playerid) == SV_NULL)
        SendClientMessage(playerid, -1, "Could not find plugin sampvoice.");
    else if (SvHasMicro(playerid) == SV_FALSE)
        SendClientMessage(playerid, -1, "The microphone could not be found.");
    static const ResetPlayerVoiceData[E_PLAYER_VOICE_DATA];
    PlayerVoiceData[playerid] = ResetPlayerVoiceData;
    PlayerVoiceData[playerid][PhoneCallTarget] = -1;
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    if (PlayerVoiceData[playerid][ProximityStream] != SV_NULL)
        SvDeleteStream(PlayerVoiceData[playerid][ProximityStream]);
    if (RadioStream[PlayerVoiceData[playerid][RadioChannel]] != SV_NULL && PlayerVoiceData[playerid][RadioChannel] > 0)
        SvDetachListenerFromStream(RadioStream[PlayerVoiceData[playerid][RadioChannel]], playerid);
    if (PlayerVoiceData[playerid][PhoneCallStream] != SV_NULL)
        EndPhoneCall(playerid);
    static const ResetPlayerVoiceData[E_PLAYER_VOICE_DATA];
    PlayerVoiceData[playerid] = ResetPlayerVoiceData;
    return 1;
}

public SV_VOID:OnPlayerActivationKeyPress(SV_UINT:playerid, SV_UINT:keyid) 
{
    if (keyid == 0x42)
    {
        if (PlayerVoiceData[playerid][ProximityStream] != SV_NULL)
            SvAttachSpeakerToStream(PlayerVoiceData[playerid][ProximityStream], playerid);
        if(PlayerVoiceData[playerid][PhoneCallStream] != SV_NULL && IsPlayerConnected(PlayerVoiceData[playerid][PhoneCallTarget]))
            SvAttachSpeakerToStream(PlayerVoiceData[playerid][PhoneCallStream], playerid);
        else if (RadioStream[PlayerVoiceData[playerid][RadioChannel]] != SV_NULL && PlayerVoiceData[playerid][RadioChannel] > 0)
        {
            ApplyAnimation(playerid, "ped", "phone_talk", 4.1, true, true, true, true, 1, true);
            SetPlayerAttachedObject(playerid, 9, 19942, 6, 0.078999, 0.047999, 0.023999, 0.000000, 0.000000, 179.099899, 1.000000, 1.000000, 1.000000);
            SvAttachSpeakerToStream(RadioStream[PlayerVoiceData[playerid][RadioChannel]], playerid);
        }
        
    }
}

public SV_VOID:OnPlayerActivationKeyRelease(SV_UINT:playerid, SV_UINT:keyid)
{
    if (keyid == 0x42)
    {
        if (PlayerVoiceData[playerid][ProximityStream] != SV_NULL)
            SvDetachSpeakerFromStream(PlayerVoiceData[playerid][ProximityStream], playerid);
        if (PlayerVoiceData[playerid][PhoneCallStream] != SV_NULL && IsPlayerConnected(PlayerVoiceData[playerid][PhoneCallTarget]))
            SvDetachSpeakerFromStream(PlayerVoiceData[playerid][PhoneCallStream], playerid);
        else if (RadioStream[PlayerVoiceData[playerid][RadioChannel]] != SV_NULL && PlayerVoiceData[playerid][RadioChannel] > 0)
        {
            ApplyAnimation(playerid, "CARRY", "crry_prtial", 4.1, false, false, false, false, 0, true);
            SvDetachSpeakerFromStream(RadioStream[PlayerVoiceData[playerid][RadioChannel]], playerid);
            RemovePlayerAttachedObject(playerid, 9);
        }
    }
}
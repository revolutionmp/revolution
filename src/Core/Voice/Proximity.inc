forward SetProximityMode(playerid);

public SetProximityMode(playerid)
{
    if (PlayerVoiceData[playerid][ProximityStream] == SV_NULL)
        return 1;

    switch (PlayerVoiceData[playerid][ProximityMode])
    {
        case PROXIMITY_MODE_WHISPER:
        {
            PlayerVoiceData[playerid][ProximityMode] = PROXIMITY_MODE_NORMAL;
            SvUpdateDistanceForLStream(PlayerVoiceData[playerid][ProximityStream], 10.0);
            SendClientMessage(playerid, -1, "Mode suara diubah menjadi: Normal");
        }
        case PROXIMITY_MODE_NORMAL:
        {
            PlayerVoiceData[playerid][ProximityMode] = PROXIMITY_MODE_SHOUT;
            SvUpdateDistanceForLStream(PlayerVoiceData[playerid][ProximityStream], 20.0);
            SendClientMessage(playerid, -1, "Mode suara diubah menjadi: Teriak");
        }
        case PROXIMITY_MODE_SHOUT:
        {
            PlayerVoiceData[playerid][ProximityMode] = PROXIMITY_MODE_WHISPER;
            SvUpdateDistanceForLStream(PlayerVoiceData[playerid][ProximityStream], 3.0);
            SendClientMessage(playerid, -1, "Mode suara diubah menjadi: Bisik");
        }
    }
    return 1;
}
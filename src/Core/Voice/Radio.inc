forward SetRadioToggle(playerid);
forward SetRadioFreq(playerid, freq);

public SetRadioToggle(playerid)
{
    if(PlayerVoiceData[playerid][RadioChannel] > 0)
    {
        SvDetachListenerFromStream(RadioStream[PlayerVoiceData[playerid][RadioChannel]], playerid);
        PlayerVoiceData[playerid][RadioChannel] = 0;
        SendClientMessage(playerid, -1, "Radio dimatikan");
    }
    else
    {
        PlayerVoiceData[playerid][RadioChannel] = 1;
        SvAttachListenerToStream(RadioStream[PlayerVoiceData[playerid][RadioChannel]], playerid);
        SendClientMessage(playerid, -1, "Radio Hidup");
    }
    return 1;
}

public SetRadioFreq(playerid, freq)
{
    if(PlayerVoiceData[playerid][RadioChannel] < 1)
        return SendClientMessage(playerid, -1, "Radio Mati, hidupkan radio terlebih dahulu!");
    SvDetachListenerFromStream(RadioStream[PlayerVoiceData[playerid][RadioChannel]], playerid);
    SvAttachListenerToStream(RadioStream[freq], playerid);
    PlayerVoiceData[playerid][RadioChannel] = freq;
    SendClientMessage(playerid, -1, "Radio freq diganti");
    return 1;
}
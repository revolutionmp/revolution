forward StartPhoneCall(callerid, targetid);
forward EndPhoneCall(playerid);

public StartPhoneCall(callerid, targetid)
{
    if (PlayerVoiceData[callerid][PhoneCallTarget] != -1)
        return SendClientMessage(callerid, -1, "Anda sudah dalam panggilan.");
    if (PlayerVoiceData[targetid][PhoneCallTarget] != -1)
        return SendClientMessage(callerid, -1, "Orang tersebut sedang dalam panggilan lain.");

    new SV_GSTREAM:callStream = SvCreateGStream(0x00BFFFFF, "");

    if(callStream == SV_NULL) {
        SendClientMessage(callerid, -1, "Error: Gagal membuat stream telepon.");
        return 1;
    }

    PlayerVoiceData[callerid][PhoneCallTarget] = targetid;
    PlayerVoiceData[callerid][PhoneCallStream] = callStream;

    PlayerVoiceData[targetid][PhoneCallTarget] = callerid;
    PlayerVoiceData[targetid][PhoneCallStream] = callStream;
    
    ApplyAnimation(callerid, "ped", "phone_talk", 4.1, true, true, true, true, 1, true);
    SetPlayerAttachedObject(callerid, 9, 19942, 6, 0.078999, 0.047999, 0.023999, 0.000000, 0.000000, 179.099899, 1.000000, 1.000000, 1.000000);
    ApplyAnimation(targetid, "ped", "phone_talk", 4.1, true, true, true, true, 1, true);
    SetPlayerAttachedObject(targetid, 9, 19942, 6, 0.078999, 0.047999, 0.023999, 0.000000, 0.000000, 179.099899, 1.000000, 1.000000, 1.000000);

    SvAttachListenerToStream(callStream, callerid);
    SvAttachListenerToStream(callStream, targetid);
    
    new message[128];
    format(message, sizeof(message), "Panggilan terhubung dengan %d (%d). Tekan 'B' untuk berbicara.", targetid, targetid);
    SendClientMessage(callerid, 0x33CCFFAA, message);
    
    format(message, sizeof(message), "Panggilan masuk dari %d (%d). Tekan 'B' untuk berbicara.", callerid, callerid);
    SendClientMessage(targetid, 0x33CCFFAA, message);
    return 1;
}

public EndPhoneCall(playerid)
{
    if (PlayerVoiceData[playerid][PhoneCallTarget] == -1)
        return SendClientMessage(playerid, -1, "Anda tidak sedang dalam panggilan.");

    if (PlayerVoiceData[playerid][PhoneCallStream] != SV_NULL)
    {
        SvDetachListenerFromStream(PlayerVoiceData[playerid][PhoneCallStream], playerid);
        SvDetachListenerFromStream(PlayerVoiceData[playerid][PhoneCallStream], PlayerVoiceData[playerid][PhoneCallTarget]);
        SvDeleteStream(PlayerVoiceData[playerid][PhoneCallStream]);
    }
    
    ApplyAnimation(playerid, "ped", "phone_out", 4.1, false, true, true, false, 1, true);
    RemovePlayerAttachedObject(playerid, 9);
    ApplyAnimation(PlayerVoiceData[playerid][PhoneCallTarget], "ped", "phone_out", 4.1, false, true, true, false, 1, true);
    RemovePlayerAttachedObject(PlayerVoiceData[playerid][PhoneCallTarget], 9);
    
    SendClientMessage(playerid, 0xCCCCCCAA, "Panggilan diakhiri.");
    SendClientMessage(PlayerVoiceData[playerid][PhoneCallTarget], 0xCCCCCCAA, "Panggilan diakhiri.");
    
    PlayerVoiceData[playerid][PhoneCallStream] = SV_NULL;
    PlayerVoiceData[PlayerVoiceData[playerid][PhoneCallTarget]][PhoneCallStream] = SV_NULL;
    PlayerVoiceData[PlayerVoiceData[playerid][PhoneCallTarget]][PhoneCallTarget] = -1;
    PlayerVoiceData[playerid][PhoneCallTarget] = -1;
    return 1;
}
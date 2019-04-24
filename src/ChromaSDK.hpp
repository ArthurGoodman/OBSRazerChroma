#pragma once

#include <Windows.h>

#include "chroma_sdk/RzChromaSDKDefines.h"
#include "chroma_sdk/RzChromaSDKTypes.h"
#include "chroma_sdk/RzErrors.h"

namespace OBSRazerChroma {

class CChromaSDK final
{
public:
    explicit CChromaSDK();
    ~CChromaSDK();

    RZRESULT CreateEffect(
        RZDEVICEID DeviceId,
        ChromaSDK::EFFECT_TYPE Effect,
        PRZPARAM pParam,
        RZEFFECTID *pEffectId);
    RZRESULT CreateKeyboardEffect(
        ChromaSDK::Keyboard::EFFECT_TYPE Effect,
        PRZPARAM pParam,
        RZEFFECTID *pEffectId);
    RZRESULT CreateMouseEffect(
        ChromaSDK::Mouse::EFFECT_TYPE Effect,
        PRZPARAM pParam,
        RZEFFECTID *pEffectId);
    RZRESULT CreateHeadsetEffect(
        ChromaSDK::Headset::EFFECT_TYPE Effect,
        PRZPARAM pParam,
        RZEFFECTID *pEffectId);
    RZRESULT CreateMousepadEffect(
        ChromaSDK::Mousepad::EFFECT_TYPE Effect,
        PRZPARAM pParam,
        RZEFFECTID *pEffectId);
    RZRESULT CreateKeypadEffect(
        ChromaSDK::Keypad::EFFECT_TYPE Effect,
        PRZPARAM pParam,
        RZEFFECTID *pEffectId);
    RZRESULT CreateChromaLinkEffect(
        ChromaSDK::ChromaLink::EFFECT_TYPE Effect,
        PRZPARAM pParam,
        RZEFFECTID *pEffectId);
    RZRESULT SetEffect(RZEFFECTID EffectId);
    RZRESULT DeleteEffect(RZEFFECTID EffectId);
    RZRESULT RegisterEventNotification(HWND hWnd);
    RZRESULT UnregisterEventNotification();
    RZRESULT QueryDevice(RZDEVICEID DeviceId, ChromaSDK::DEVICE_INFO_TYPE &DeviceInfo);

private:
    using InitFunc = RZRESULT (*)();
    using UnInitFunc = RZRESULT (*)();
    using CreateEffectFunc = RZRESULT (*)(
        RZDEVICEID DeviceId,
        ChromaSDK::EFFECT_TYPE Effect,
        PRZPARAM pParam,
        RZEFFECTID *pEffectId);
    using CreateKeyboardEffectFunc = RZRESULT (
            *)(ChromaSDK::Keyboard::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
    using CreateMouseEffectFunc =
        RZRESULT (*)(ChromaSDK::Mouse::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
    using CreateHeadsetEffectFunc = RZRESULT (
            *)(ChromaSDK::Headset::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
    using CreateMousepadEffectFunc = RZRESULT (
            *)(ChromaSDK::Mousepad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
    using CreateKeypadEffectFunc =
        RZRESULT (*)(ChromaSDK::Keypad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
    using CreateChromaLinkEffectFunc = RZRESULT (
            *)(ChromaSDK::ChromaLink::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
    using SetEffectFunc = RZRESULT (*)(RZEFFECTID EffectId);
    using DeleteEffectFunc = RZRESULT (*)(RZEFFECTID EffectId);
    using RegisterEventNotificationFunc = RZRESULT (*)(HWND hWnd);
    using UnregisterEventNotificationFunc = RZRESULT (*)();
    using QueryDeviceFunc =
        RZRESULT (*)(RZDEVICEID DeviceId, ChromaSDK::DEVICE_INFO_TYPE &DeviceInfo);

private:
    HMODULE m_module = nullptr;

    InitFunc m_Init = nullptr;
    UnInitFunc m_UnInit = nullptr;
    CreateEffectFunc m_CreateEffect = nullptr;
    CreateKeyboardEffectFunc m_CreateKeyboardEffect = nullptr;
    CreateMouseEffectFunc m_CreateMouseEffect = nullptr;
    CreateHeadsetEffectFunc m_CreateHeadsetEffect = nullptr;
    CreateMousepadEffectFunc m_CreateMousepadEffect = nullptr;
    CreateKeypadEffectFunc m_CreateKeypadEffect = nullptr;
    CreateChromaLinkEffectFunc m_CreateChromaLinkEffect = nullptr;
    SetEffectFunc m_SetEffect = nullptr;
    DeleteEffectFunc m_DeleteEffect = nullptr;
    RegisterEventNotificationFunc m_RegisterEventNotification = nullptr;
    UnregisterEventNotificationFunc m_UnregisterEventNotification = nullptr;
    QueryDeviceFunc m_QueryDevice = nullptr;
};

} // namespace OBSRazerChroma

#include "ChromaSDK.hpp"

#include <functional>
#include <stdexcept>

namespace OBSRazerChroma {

CChromaSDK::CChromaSDK()
{
#ifdef _WIN64
    static constexpr LPCSTR c_chroma_sdk_dll = "RzChromaSDK64.dll";
#else
    static constexpr LPCSTR c_chroma_sdk_dll = "RzChromaSDK.dll";
#endif

    m_module = ::LoadLibraryA(c_chroma_sdk_dll);
    if (m_module == nullptr)
    {
        return;
    }

    m_Init = (InitFunc)::GetProcAddress(m_module, "Init");
    if (m_Init == nullptr)
    {
        return;
    }

    RZRESULT rz_result = m_Init();
    if (rz_result != RZRESULT_SUCCESS)
    {
        return;
    }

    m_UnInit = (UnInitFunc)::GetProcAddress(m_module, "UnInit");
    m_CreateEffect = (CreateEffectFunc)::GetProcAddress(m_module, "CreateEffect");
    m_CreateKeyboardEffect =
        (CreateKeyboardEffectFunc)::GetProcAddress(m_module, "CreateKeyboardEffect");
    m_CreateMouseEffect = (CreateMouseEffectFunc)::GetProcAddress(m_module, "CreateMouseEffect");
    m_CreateHeadsetEffect =
        (CreateHeadsetEffectFunc)::GetProcAddress(m_module, "CreateHeadsetEffect");
    m_CreateMousepadEffect =
        (CreateMousepadEffectFunc)::GetProcAddress(m_module, "CreateMousepadEffect");
    m_CreateKeypadEffect = (CreateKeypadEffectFunc)::GetProcAddress(m_module, "CreateKeypadEffect");
    m_CreateChromaLinkEffect =
        (CreateChromaLinkEffectFunc)::GetProcAddress(m_module, "CreateChromaLinkEffect");
    m_SetEffect = (SetEffectFunc)::GetProcAddress(m_module, "SetEffect");
    m_DeleteEffect = (DeleteEffectFunc)::GetProcAddress(m_module, "DeleteEffect");
    m_RegisterEventNotification =
        (RegisterEventNotificationFunc)::GetProcAddress(m_module, "RegisterEventNotification");
    m_UnregisterEventNotification =
        (UnregisterEventNotificationFunc)::GetProcAddress(m_module, "UnregisterEventNotification");
    m_QueryDevice = (QueryDeviceFunc)::GetProcAddress(m_module, "QueryDevice");
}

CChromaSDK::~CChromaSDK()
{
    if (m_module)
    {
        if (m_UnInit)
        {
            m_UnInit();
        }

        ::FreeLibrary(m_module);
        m_module = nullptr;
    }
}

RZRESULT CChromaSDK::CreateEffect(
    RZDEVICEID DeviceId,
    ChromaSDK::EFFECT_TYPE Effect,
    PRZPARAM pParam,
    RZEFFECTID *pEffectId)
{
    if (m_CreateEffect)
    {
        return m_CreateEffect(DeviceId, Effect, pParam, pEffectId);
    }

    return RZRESULT_FAILED;
}

RZRESULT CChromaSDK::CreateKeyboardEffect(
    ChromaSDK::Keyboard::EFFECT_TYPE Effect,
    PRZPARAM pParam,
    RZEFFECTID *pEffectId)
{
    if (m_CreateKeyboardEffect)
    {
        return m_CreateKeyboardEffect(Effect, pParam, pEffectId);
    }

    return RZRESULT_FAILED;
}

RZRESULT CChromaSDK::CreateMouseEffect(
    ChromaSDK::Mouse::EFFECT_TYPE Effect,
    PRZPARAM pParam,
    RZEFFECTID *pEffectId)
{
    if (m_CreateMouseEffect)
    {
        return m_CreateMouseEffect(Effect, pParam, pEffectId);
    }

    return RZRESULT_FAILED;
}

RZRESULT CChromaSDK::CreateHeadsetEffect(
    ChromaSDK::Headset::EFFECT_TYPE Effect,
    PRZPARAM pParam,
    RZEFFECTID *pEffectId)
{
    if (m_CreateHeadsetEffect)
    {
        return m_CreateHeadsetEffect(Effect, pParam, pEffectId);
    }

    return RZRESULT_FAILED;
}

RZRESULT CChromaSDK::CreateMousepadEffect(
    ChromaSDK::Mousepad::EFFECT_TYPE Effect,
    PRZPARAM pParam,
    RZEFFECTID *pEffectId)
{
    if (m_CreateMousepadEffect)
    {
        return m_CreateMousepadEffect(Effect, pParam, pEffectId);
    }

    return RZRESULT_FAILED;
}

RZRESULT CChromaSDK::CreateKeypadEffect(
    ChromaSDK::Keypad::EFFECT_TYPE Effect,
    PRZPARAM pParam,
    RZEFFECTID *pEffectId)
{
    if (m_CreateKeypadEffect)
    {
        return m_CreateKeypadEffect(Effect, pParam, pEffectId);
    }

    return RZRESULT_FAILED;
}

RZRESULT CChromaSDK::CreateChromaLinkEffect(
    ChromaSDK::ChromaLink::EFFECT_TYPE Effect,
    PRZPARAM pParam,
    RZEFFECTID *pEffectId)
{
    if (m_CreateChromaLinkEffect)
    {
        return m_CreateChromaLinkEffect(Effect, pParam, pEffectId);
    }

    return RZRESULT_FAILED;
}

RZRESULT CChromaSDK::SetEffect(RZEFFECTID EffectId)
{
    if (m_SetEffect)
    {
        return m_SetEffect(EffectId);
    }

    return RZRESULT_FAILED;
}

RZRESULT CChromaSDK::DeleteEffect(RZEFFECTID EffectId)
{
    if (m_DeleteEffect)
    {
        return m_DeleteEffect(EffectId);
    }

    return RZRESULT_FAILED;
}

RZRESULT CChromaSDK::RegisterEventNotification(HWND hWnd)
{
    if (m_RegisterEventNotification)
    {
        return m_RegisterEventNotification(hWnd);
    }

    return RZRESULT_FAILED;
}

RZRESULT CChromaSDK::UnregisterEventNotification()
{
    if (m_UnregisterEventNotification)
    {
        return m_UnregisterEventNotification();
    }

    return RZRESULT_FAILED;
}

RZRESULT CChromaSDK::QueryDevice(RZDEVICEID DeviceId, ChromaSDK::DEVICE_INFO_TYPE &DeviceInfo)
{
    if (m_QueryDevice)
    {
        return m_QueryDevice(DeviceId, DeviceInfo);
    }

    return RZRESULT_FAILED;
}

} // namespace OBSRazerChroma

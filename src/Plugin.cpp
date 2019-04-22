#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <obs-frontend-api.h>
#include <obs-module.h>

#include <Windows.h>
#include <tchar.h>

#include "chroma_sdk/RzChromaSDKDefines.h"
#include "chroma_sdk/RzChromaSDKTypes.h"
#include "chroma_sdk/RzErrors.h"

#ifdef _WIN64
#define CHROMASDKDLL _T("RzChromaSDK64.dll")
#else
#define CHROMASDKDLL _T("RzChromaSDK.dll")
#endif

static const COLORREF BLACK = RGB(0, 0, 0);
static const COLORREF WHITE = RGB(255, 255, 255);
static const COLORREF RED = RGB(255, 0, 0);
static const COLORREF GREEN = RGB(0, 255, 0);
static const COLORREF BLUE = RGB(0, 0, 255);
static const COLORREF YELLOW = RGB(255, 255, 0);
static const COLORREF PURPLE = RGB(128, 0, 128);
static const COLORREF CYAN = RGB(00, 255, 255);
static const COLORREF ORANGE = RGB(255, 165, 00);
static const COLORREF PINK = RGB(255, 192, 203);
static const COLORREF GREY = RGB(125, 125, 125);

typedef RZRESULT(*INIT)(void);
typedef RZRESULT(*UNINIT)(void);
typedef RZRESULT(*CREATEEFFECT)(RZDEVICEID DeviceId, ChromaSDK::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEKEYBOARDEFFECT)(ChromaSDK::Keyboard::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEHEADSETEFFECT)(ChromaSDK::Headset::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEMOUSEPADEFFECT)(ChromaSDK::Mousepad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEMOUSEEFFECT)(ChromaSDK::Mouse::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATEKEYPADEFFECT)(ChromaSDK::Keypad::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*CREATECHROMALINKEFFECT)(ChromaSDK::ChromaLink::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID *pEffectId);
typedef RZRESULT(*SETEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*DELETEEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*REGISTEREVENTNOTIFICATION)(HWND hWnd);
typedef RZRESULT(*UNREGISTEREVENTNOTIFICATION)(void);
typedef RZRESULT(*QUERYDEVICE)(RZDEVICEID DeviceId, ChromaSDK::DEVICE_INFO_TYPE &DeviceInfo);

// TODO Make non-static

INIT Init = NULL;
UNINIT UnInit = NULL;
CREATEEFFECT CreateEffect = NULL;
CREATEKEYBOARDEFFECT CreateKeyboardEffect = NULL;
CREATEMOUSEEFFECT CreateMouseEffect = NULL;
CREATEHEADSETEFFECT CreateHeadsetEffect = NULL;
CREATEMOUSEPADEFFECT CreateMousematEffect = NULL;
CREATEKEYPADEFFECT CreateKeypadEffect = NULL;
CREATECHROMALINKEFFECT CreateChromaLinkEffect = NULL;
SETEFFECT SetEffect = NULL;
DELETEEFFECT DeleteEffect = NULL;
QUERYDEVICE QueryDevice = NULL;

class CChromaSDKImpl
{
public:
    explicit CChromaSDKImpl();
    ~CChromaSDKImpl();

public:
    void reset();
    void blink();

private:
    void animationThread();

private:
    HMODULE m_module{NULL};
    std::thread m_animation_thread;
    RZEFFECTID m_normal_id{GUID_NULL};
    RZEFFECTID m_red_id{GUID_NULL};
    std::condition_variable m_cv;
    bool m_abort_animation{false};
    std::mutex m_mutex;
};

CChromaSDKImpl::CChromaSDKImpl()
{
    m_module = ::LoadLibrary(CHROMASDKDLL);
    if (m_module != NULL)
    {
        INIT Init = (INIT)::GetProcAddress(m_module, "Init");
        if (Init != NULL)
        {
            RZRESULT rzResult = Init();
            if (rzResult == RZRESULT_SUCCESS)
            {
                CreateEffect = (CREATEEFFECT)::GetProcAddress(m_module, "CreateEffect");
                CreateKeyboardEffect = (CREATEKEYBOARDEFFECT)::GetProcAddress(m_module, "CreateKeyboardEffect");
                CreateMouseEffect = (CREATEMOUSEEFFECT)::GetProcAddress(m_module, "CreateMouseEffect");
                CreateMousematEffect = (CREATEMOUSEPADEFFECT)::GetProcAddress(m_module, "CreateMousepadEffect");
                CreateKeypadEffect = (CREATEKEYPADEFFECT)::GetProcAddress(m_module, "CreateKeypadEffect");
                CreateHeadsetEffect = (CREATEHEADSETEFFECT)::GetProcAddress(m_module, "CreateHeadsetEffect");
                CreateChromaLinkEffect = (CREATECHROMALINKEFFECT)::GetProcAddress(m_module, "CreateChromaLinkEffect");
                SetEffect = (SETEFFECT)GetProcAddress(m_module, "SetEffect");
                DeleteEffect = (DELETEEFFECT)GetProcAddress(m_module, "DeleteEffect");
            }
        }
    }

    ChromaSDK::Keyboard::CUSTOM_KEY_EFFECT_TYPE KeyEffect = {};

    KeyEffect.Key[HIBYTE(ChromaSDK::Keyboard::RZKEY_F12)][LOBYTE(ChromaSDK::Keyboard::RZKEY_F12)] = 0x1000000 | RED;

    static constexpr COLORREF c_bg_color = RGB(200, 200, 200);

    for (int x = 0; x < ChromaSDK::Keyboard::MAX_ROW; x++)
    {
        for (int y = 0; y < ChromaSDK::Keyboard::MAX_COLUMN; y++)
        {
            KeyEffect.Color[x][y] = c_bg_color;
        }
    }

    // TODO Wrap for NULL checks

    CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM_KEY, &KeyEffect, &m_red_id);

    ChromaSDK::Keyboard::STATIC_EFFECT_TYPE StaticEffect = {};
    StaticEffect.Color = c_bg_color;

    CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_STATIC, &StaticEffect, &m_normal_id);

    reset();
}

CChromaSDKImpl::~CChromaSDKImpl()
{
    if (m_module != NULL)
    {
        UNINIT UnInit = (UNINIT)::GetProcAddress(m_module, "UnInit");
        if (UnInit != NULL)
        {
            RZRESULT rzResult = UnInit();
            if (rzResult != RZRESULT_SUCCESS)
            {
                // TODO Handle error
            }
        }

        ::FreeLibrary(m_module);
        m_module = NULL;
    }
}

void CChromaSDKImpl::reset()
{
    if (m_animation_thread.joinable())
    {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_abort_animation = true;
            m_cv.notify_one();
        }

        m_animation_thread.join();
    }

    SetEffect(m_normal_id);
}

void CChromaSDKImpl::blink()
{
    m_animation_thread = std::thread{std::bind(&CChromaSDKImpl::animationThread, this)};
}

void CChromaSDKImpl::animationThread()
{
    int state = 0;
    while (true)
    {
        if (state)
        {
            SetEffect(m_normal_id);
        }
        else
        {
            SetEffect(m_red_id);
        }

        state = !state;

        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait_for(lock, std::chrono::milliseconds(750), [&]() { return m_abort_animation; });
            if (m_abort_animation)
            {
                m_abort_animation = false;
                break;
            }
        }
    }
}

OBS_DECLARE_MODULE()

OBS_MODULE_USE_DEFAULT_LOCALE("obs-razer-chroma", "en-US")

static CChromaSDKImpl *g_chroma_sdk;

void process_frontend_event(enum obs_frontend_event event, void *private_data)
{
    switch (event)
    {
    case OBS_FRONTEND_EVENT_RECORDING_STARTING:
        g_chroma_sdk->blink();
        break;

    case OBS_FRONTEND_EVENT_RECORDING_STOPPING:
        g_chroma_sdk->reset();
        break;

    default:
        break;
    }
}

bool obs_module_load()
{
    g_chroma_sdk = new CChromaSDKImpl;
    obs_frontend_add_event_callback(process_frontend_event, NULL);
    return true;
}

void obs_module_unload()
{
    delete g_chroma_sdk;
}

#include "AnimationController.hpp"

#include <map>

namespace {

using namespace OBSRazerChroma;

constexpr COLORREF rgbToBgr(ColorRGB c)
{
    return (c & 0xff) << 16 | (c & 0xff00) | (c & 0xff0000) >> 16;
}

ChromaSDK::Keyboard::RZKEY keyToRzKey(Key key)
{
    static const std::map<Key, ChromaSDK::Keyboard::RZKEY> key_to_rz_key{
#define X(key, str, rz_key) {key, ChromaSDK::Keyboard::rz_key},
#include "Keys.def"
#undef X
    };

    const auto &it = key_to_rz_key.find(key);

    if (it == key_to_rz_key.end())
    {
        return ChromaSDK::Keyboard::RZKEY_INVALID;
    }
    else
    {
        return it->second;
    }
};

} // anonymous namespace

namespace OBSRazerChroma {

CAnimationController::CAnimationController(const CConfig &config)
    : m_config{config}
{
    const COLORREF bg_color = rgbToBgr(m_config.getBgColor());
    const COLORREF fg_color = rgbToBgr(m_config.getFgColor());

    ChromaSDK::Keyboard::CUSTOM_KEY_EFFECT_TYPE key_effect{};

    const ChromaSDK::Keyboard::RZKEY rz_key = keyToRzKey(m_config.getKey());
    key_effect.Key[HIBYTE(rz_key)][LOBYTE(rz_key)] = 0x1000000 | fg_color;

    for (int x = 0; x < ChromaSDK::Keyboard::MAX_ROW; x++)
    {
        for (int y = 0; y < ChromaSDK::Keyboard::MAX_COLUMN; y++)
        {
            key_effect.Color[x][y] = bg_color;
        }
    }

    m_sdk.CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM_KEY, &key_effect, &m_active_id);

    ChromaSDK::Keyboard::STATIC_EFFECT_TYPE static_effect{};
    static_effect.Color = bg_color;

    m_sdk.CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_STATIC, &static_effect, &m_normal_id);
}

CAnimationController::~CAnimationController()
{
    m_sdk.DeleteEffect(m_active_id);
    m_sdk.DeleteEffect(m_normal_id);
}

void CAnimationController::reset()
{
    std::unique_lock<std::mutex> lock(m_access_mutex);

    if (m_animation_thread.joinable())
    {
        {
            std::unique_lock<std::mutex> lock(m_animation_mutex);
            m_abort_animation = true;
            m_cv.notify_one();
        }

        m_animation_thread.join();
    }

    m_sdk.SetEffect(m_normal_id);
}

void CAnimationController::blink()
{
    std::unique_lock<std::mutex> lock(m_access_mutex);

    if (m_config.getBlink())
    {
        m_animation_thread = std::thread{std::bind(&CAnimationController::animationThread, this)};
    }
    else
    {
        m_sdk.SetEffect(m_active_id);
    }
}

void CAnimationController::animationThread()
{
    int state = 0;
    while (true)
    {
        if (state)
        {
            m_sdk.SetEffect(m_normal_id);
        }
        else
        {
            m_sdk.SetEffect(m_active_id);
        }

        state = !state;

        {
            std::unique_lock<std::mutex> lock(m_animation_mutex);
            m_cv.wait_for(lock, std::chrono::milliseconds(m_config.getInterval()), [&]() {
                return m_abort_animation;
            });
            if (m_abort_animation)
            {
                m_abort_animation = false;
                break;
            }
        }
    }
}

} // namespace OBSRazerChroma

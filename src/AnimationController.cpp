#include "AnimationController.hpp"

namespace OBSRazerChroma {

CAnimationController::CAnimationController()
{
    static constexpr COLORREF c_bg_color = RGB(200, 200, 200);
    static constexpr COLORREF c_fg_color = RGB(255, 0, 0);

    ChromaSDK::Keyboard::CUSTOM_KEY_EFFECT_TYPE key_effect{};

    static constexpr ChromaSDK::Keyboard::RZKEY c_key = ChromaSDK::Keyboard::RZKEY_F12;
    key_effect.Key[HIBYTE(c_key)][LOBYTE(c_key)] = 0x1000000 | c_fg_color;

    for (int x = 0; x < ChromaSDK::Keyboard::MAX_ROW; x++)
    {
        for (int y = 0; y < ChromaSDK::Keyboard::MAX_COLUMN; y++)
        {
            key_effect.Color[x][y] = c_bg_color;
        }
    }

    m_sdk.CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM_KEY, &key_effect, &m_red_id);

    ChromaSDK::Keyboard::STATIC_EFFECT_TYPE static_effect{};
    static_effect.Color = c_bg_color;

    m_sdk.CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_STATIC, &static_effect, &m_normal_id);
}

CAnimationController::~CAnimationController()
{
    m_sdk.DeleteEffect(m_red_id);
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

    m_animation_thread = std::thread{std::bind(&CAnimationController::animationThread, this)};
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
            m_sdk.SetEffect(m_red_id);
        }

        state = !state;

        {
            std::unique_lock<std::mutex> lock(m_animation_mutex);
            m_cv.wait_for(
                lock, std::chrono::milliseconds(750), [&]() { return m_abort_animation; });
            if (m_abort_animation)
            {
                m_abort_animation = false;
                break;
            }
        }
    }
}

} // namespace OBSRazerChroma

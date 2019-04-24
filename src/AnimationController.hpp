#pragma once

#include <condition_variable>
#include <mutex>
#include <thread>

#include "ChromaSDK.hpp"
#include "Config.hpp"

namespace OBSRazerChroma {

class CAnimationController final
{
public:
    explicit CAnimationController(const CConfig &config);
    ~CAnimationController();

    void reset();
    void blink();

private:
    void animationThread();

private:
    const CConfig m_config;

    CChromaSDK m_sdk;

    std::thread m_animation_thread;
    std::mutex m_animation_mutex;
    std::condition_variable m_cv;
    bool m_abort_animation = false;

    RZEFFECTID m_normal_id = GUID_NULL;
    RZEFFECTID m_red_id = GUID_NULL;

    std::mutex m_access_mutex;
};

} // namespace OBSRazerChroma

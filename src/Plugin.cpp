#include <iostream>
#include <memory>
#include <thread>

#include <obs-frontend-api.h>
#include <obs-module.h>

#include "AnimationController.hpp"

OBS_DECLARE_MODULE()

OBS_MODULE_USE_DEFAULT_LOCALE("obs-razer-chroma", "en-US")

static std::shared_ptr<OBSRazerChroma::CAnimationController> g_animation_controller;

void process_frontend_event(obs_frontend_event event, void *)
{
    switch (event)
    {
    case OBS_FRONTEND_EVENT_RECORDING_STARTING:
        g_animation_controller->blink();
        break;

    case OBS_FRONTEND_EVENT_RECORDING_STOPPING:
        g_animation_controller->reset();
        break;

    case OBS_FRONTEND_EVENT_FINISHED_LOADING:
        std::thread{[]() {
            std::this_thread::sleep_for(std::chrono::milliseconds{500});
            g_animation_controller->reset();
        }}
            .detach();
        break;

    default:
        break;
    }
}

bool obs_module_load()
{
    g_animation_controller = std::make_shared<OBSRazerChroma::CAnimationController>();
    obs_frontend_add_event_callback(process_frontend_event, nullptr);
    return true;
}

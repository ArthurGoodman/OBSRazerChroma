#include "Config.hpp"

#include <cstdio>
#include <memory>
#include <string>

#include <obs-module.h>
#include <util/bmem.h>
#include <util/config-file.h>
#include <util/platform.h>

namespace {

using OBSRazerChroma::ColorBGR;

static constexpr const char *c_config_name = "config.ini";

static constexpr const char *c_main_section = "general";

static constexpr const char *c_bg_color_option = "background_color";
static constexpr const char *c_fg_color_option = "blink_color";
static constexpr const char *c_interval_option = "blink_interval_msec";

static constexpr ColorBGR c_default_bg_color = 0xc8c8c8;
static constexpr ColorBGR c_default_fg_color = 0x0000ff;
static constexpr uint32_t c_default_interval = 750;

constexpr ColorBGR swapRedAndBlue(ColorBGR c)
{
    return (c & 0xff) << 16 | (c & 0xff00) | (c & 0xff0000) >> 16;
}

std::string colorToString(ColorBGR c)
{
    char buf[8];
    std::snprintf(buf, sizeof(buf), "#%06X", swapRedAndBlue(c));
    return buf;
}

void stringToColor(const std::string &str, ColorBGR &c)
{
    if (std::sscanf(str.c_str(), "#%x", &c) > 0)
    {
        c = swapRedAndBlue(c);
    }
}

} // anonymous namespace

namespace OBSRazerChroma {

CConfig::CConfig()
    : m_bg_color{c_default_bg_color}
    , m_fg_color{c_default_fg_color}
    , m_interval{c_default_interval}
{
    char *config_dir_path = nullptr;
    char *config_path = nullptr;
    config_t *config = nullptr;

    std::shared_ptr<void> scope_guard{nullptr, [&](...) {
                                          bfree(config_dir_path);
                                          bfree(config_path);
                                          config_close(config);
                                      }};

    config_dir_path = obs_module_config_path("");
    if (!os_file_exists(config_dir_path))
    {
        os_mkdirs(config_dir_path);
    }

    config_path = obs_module_config_path(c_config_name);

    if (os_file_exists(config_path))
    {
        int res = config_open(&config, config_path, CONFIG_OPEN_EXISTING);
        if (res != CONFIG_SUCCESS)
        {
            return;
        }
    }
    else
    {
        config = config_create(config_path);
        if (!config)
        {
            return;
        }
    }

    config_set_default_string(
        config, c_main_section, c_bg_color_option, colorToString(m_bg_color).c_str());
    config_set_default_string(
        config, c_main_section, c_fg_color_option, colorToString(m_fg_color).c_str());
    config_set_default_uint(config, c_main_section, c_interval_option, m_interval);

    stringToColor(config_get_string(config, c_main_section, c_bg_color_option), m_bg_color);
    stringToColor(config_get_string(config, c_main_section, c_fg_color_option), m_fg_color);
    m_interval = config_get_uint(config, c_main_section, c_interval_option);

    config_set_string(config, c_main_section, c_bg_color_option, colorToString(m_bg_color).c_str());
    config_set_string(config, c_main_section, c_fg_color_option, colorToString(m_fg_color).c_str());
    config_set_uint(config, c_main_section, c_interval_option, m_interval);

    config_save(config);
}

ColorBGR CConfig::getBgColor() const
{
    return m_bg_color;
}

ColorBGR CConfig::getFgColor() const
{
    return m_fg_color;
}

uint32_t CConfig::getInterval() const
{
    return m_interval;
}

} // namespace OBSRazerChroma

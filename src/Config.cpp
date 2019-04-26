#include "Config.hpp"

#include <algorithm>
#include <cstdio>
#include <map>
#include <memory>
#include <string>

#include <obs-module.h>
#include <util/bmem.h>
#include <util/config-file.h>
#include <util/platform.h>

namespace {

using namespace OBSRazerChroma;

static constexpr const char *c_config_name = "config.ini";

static constexpr const char *c_main_section = "general";

static constexpr const char *c_bg_color_option = "background_color";
static constexpr const char *c_fg_color_option = "blink_color";
static constexpr const char *c_interval_option = "blink_interval_msec";
static constexpr const char *c_blink_option = "blink";
static constexpr const char *c_key_option = "key";

static constexpr uint32_t c_min_interval = 100;
static constexpr uint32_t c_max_interval = 10000;

static constexpr ColorRGB c_default_bg_color = 0xc8c8c8;
static constexpr ColorRGB c_default_fg_color = 0xff0000;
static constexpr uint32_t c_default_interval = 750;
static constexpr Key c_default_key = KeyF12;

std::string colorToString(ColorRGB c)
{
    char buf[8];
    std::snprintf(buf, sizeof(buf), "#%06X", c);
    return buf;
}

void stringToColor(const std::string &str, ColorRGB &c)
{
    std::sscanf(str.c_str(), "#%x", &c);
}

Key strToKey(const std::string &str)
{
    static const std::map<std::string, Key> str_to_key{
#define X(key, str, rz_key) {str, key},
#include "Keys.def"
#undef X
    };

    const auto &it = str_to_key.find(str);

    if (it == str_to_key.end())
    {
        return c_default_key;
    }
    else
    {
        return it->second;
    }
}

std::string keyToStr(Key key)
{
    static const std::map<Key, std::string> key_to_str{
#define X(key, str, rz_key) {key, str},
#include "Keys.def"
#undef X
    };

    const auto &it = key_to_str.find(key);

    if (it == key_to_str.end())
    {
        return "";
    }
    else
    {
        return it->second;
    }
}

} // anonymous namespace

namespace OBSRazerChroma {

CConfig::CConfig()
    : m_bg_color{c_default_bg_color}
    , m_fg_color{c_default_fg_color}
    , m_interval{c_default_interval}
    , m_blink{true}
    , m_key{c_default_key}
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
    config_set_default_bool(config, c_main_section, c_blink_option, m_blink);
    config_set_default_string(config, c_main_section, c_key_option, keyToStr(m_key).c_str());

    stringToColor(config_get_string(config, c_main_section, c_bg_color_option), m_bg_color);
    stringToColor(config_get_string(config, c_main_section, c_fg_color_option), m_fg_color);
    m_interval = static_cast<uint32_t>(config_get_uint(config, c_main_section, c_interval_option));
    m_blink = config_get_bool(config, c_main_section, c_blink_option);
    m_key = strToKey(config_get_string(config, c_main_section, c_key_option));

    m_interval = std::max(c_min_interval, std::min(m_interval, c_max_interval));

    config_set_string(config, c_main_section, c_bg_color_option, colorToString(m_bg_color).c_str());
    config_set_string(config, c_main_section, c_fg_color_option, colorToString(m_fg_color).c_str());
    config_set_uint(config, c_main_section, c_interval_option, m_interval);
    config_set_bool(config, c_main_section, c_blink_option, m_blink);
    config_set_string(config, c_main_section, c_key_option, keyToStr(m_key).c_str());

    config_save(config);
}

ColorRGB CConfig::getBgColor() const
{
    return m_bg_color;
}

ColorRGB CConfig::getFgColor() const
{
    return m_fg_color;
}

uint32_t CConfig::getInterval() const
{
    return m_interval;
}

bool CConfig::getBlink() const
{
    return m_blink;
}

Key CConfig::getKey() const
{
    return m_key;
}

} // namespace OBSRazerChroma

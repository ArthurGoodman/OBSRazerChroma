#pragma once

#include <cstdint>

namespace OBSRazerChroma {

using ColorRGB = uint32_t;

enum Key
{
#define X(key, str, rz_key) key,
#include "Keys.def"
#undef X
};

class CConfig final
{
public:
    explicit CConfig();

    ColorRGB getBgColor() const;
    ColorRGB getFgColor() const;
    uint32_t getInterval() const;
    bool getBlink() const;
    Key getKey() const;

private:
    ColorRGB m_bg_color;
    ColorRGB m_fg_color;
    uint32_t m_interval;
    bool m_blink;
    Key m_key;
};

} // namespace OBSRazerChroma

#pragma once

#include <cstdint>

namespace OBSRazerChroma {

using ColorBGR = uint32_t;

class CConfig final
{
public:
    explicit CConfig();

    ColorBGR getBgColor() const;
    ColorBGR getFgColor() const;
    uint32_t getInterval() const;

private:
    ColorBGR m_bg_color;
    ColorBGR m_fg_color;
    uint32_t m_interval;
};

} // namespace OBSRazerChroma

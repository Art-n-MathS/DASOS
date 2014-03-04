#include "Peak.h"

//-----------------------------------------------------------------------------
Peak::Peak()
{
}


//-----------------------------------------------------------------------------
Peak::Peak(const Peak &i_peak):
    m_position(i_peak.m_position),
    m_itensity(i_peak.m_itensity),
    m_leftAngle(i_peak.m_leftAngle),
    m_rightAngle(i_peak.m_rightAngle)
{
}


//-----------------------------------------------------------------------------
Peak::~Peak()
{
}

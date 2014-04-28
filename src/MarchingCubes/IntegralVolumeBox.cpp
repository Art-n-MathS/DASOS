#include "IntegralVolumeBox.h"

//-----------------------------------------------------------------------------
IntegralVolumeBox::IntegralVolumeBox(
        const IntegralVolumeBox &i_box
        ):
    m_mins(i_box.m_mins),
    m_lens(i_box.m_lens),
    m_divisibles(i_box.m_divisibles),
    m_nextSide(i_box.m_nextSide)
{

}

//-----------------------------------------------------------------------------
IntegralVolumeBox::IntegralVolumeBox(
        const unsigned short i_lenX,
        const unsigned short i_lenY,
        const unsigned short i_lenZ
        ):
    m_mins(3,0),
    m_lens(3),
    m_divisibles(7),
    m_nextSide(0)
{
   m_lens[0] = i_lenX;
   m_lens[1] = i_lenY;
   m_lens[2] = i_lenZ;
}

//-----------------------------------------------------------------------------
IntegralVolumeBox::~IntegralVolumeBox()
{}

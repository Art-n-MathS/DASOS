#include "PairDoubleInt.h"

//-----------------------------------------------------------------------------
PairDoubleInt::PairDoubleInt():
    m_intensity(0.0),
    m_noOfInt_or_posOfInt(1)
{}

//-----------------------------------------------------------------------------
PairDoubleInt::PairDoubleInt(double i_int, unsigned int i_pos):
    m_intensity(i_int),
    m_noOfInt_or_posOfInt(i_pos)
{}

//-----------------------------------------------------------------------------
PairDoubleInt::PairDoubleInt(const PairDoubleInt &i_pd):
    m_intensity(i_pd.m_intensity),
    m_noOfInt_or_posOfInt(i_pd.m_noOfInt_or_posOfInt)
{

}

//-----------------------------------------------------------------------------
PairDoubleInt & PairDoubleInt::operator=(
        const PairDoubleInt& i_v
        )
{
    m_intensity = i_v.m_intensity;
    m_noOfInt_or_posOfInt = i_v.m_noOfInt_or_posOfInt;
    return *this;
}

//-----------------------------------------------------------------------------
bool PairDoubleInt::operator ==( const PairDoubleInt &i_v)const
{
    return (m_intensity-0.00001 < i_v.m_intensity &&
            m_intensity+0.00001 > i_v.m_intensity &&
            m_noOfInt_or_posOfInt == i_v.m_noOfInt_or_posOfInt);
}

//-----------------------------------------------------------------------------
PairDoubleInt::~PairDoubleInt()
{}

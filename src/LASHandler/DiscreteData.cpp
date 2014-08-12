#include "DiscreteData.h"


//-----------------------------------------------------------------------------
DiscreteData::DiscreteData(unsigned int i_numOfRecords):m_next(0)
{
   // resize the arrays such that reallocation of memory will be avoided
   // in the future
   m_positions.resize(i_numOfRecords);
   m_intensities.resize(i_numOfRecords);
}


//-----------------------------------------------------------------------------
void DiscreteData::addPoint(unsigned short i_intensity, ngl::Vec3 i_point)
{
    m_positions[m_next] = i_point;
    m_intensities[m_next] = i_intensity;
    m_next++;
}

//-----------------------------------------------------------------------------
Object *DiscreteData::fillObject(
        Object *i_obj,
        const std::vector <double> &user_limits
        ) const
{
   std::cout << "DiscreteDAta: fillObject\n";
   for(unsigned int i=0; i<m_next;++i)
   {
      if(isInsideLimits(user_limits,m_positions[i]))
      {
         i_obj->addItensity(gmtl::Vec3f(m_positions[i].m_x,m_positions[i].m_y,m_positions[i].m_z),m_intensities[i]);
      }
   }
   i_obj->normalise();
   i_obj->insertIntoIntegralVolume();
   return i_obj;
}

//-----------------------------------------------------------------------------
bool DiscreteData::isInsideLimits(
        const std::vector<double> &i_user_limits,
        const ngl::Vec3 &i_point
        )const
{
   return i_point.m_y<i_user_limits[0] && i_point.m_y>i_user_limits[1] &&
          i_point.m_x<i_user_limits[2] && i_point.m_x>i_user_limits[3];
}

//-----------------------------------------------------------------------------
DiscreteData::~DiscreteData()
{}

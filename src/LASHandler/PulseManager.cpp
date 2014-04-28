#include "PulseManager.h"
#include <string.h>

//-----------------------------------------------------------------------------
PulseManager::PulseManager(
        const Types::Public_Header_Block &i_publicHeader,
        const Types::WF_packet_Descriptor &i_wv_info
        ):m_quadTree(0),
    m_noiseLevel(30.0)
{
   memcpy((void*) &m_public_header,(void*)&i_publicHeader,sizeof(m_public_header));
   memcpy((void*) &m_wfInfo,(void *) &i_wv_info, sizeof(m_wfInfo));

   // set the kernel for smoothing the wave while detecting the key points
   float kernel[] = {1,2,3,2,1};
   Pulse::s_kernel.assign (kernel,kernel+5);   // assigning from array.
}

//-----------------------------------------------------------------------------
PulseManager::PulseManager(
        const PulseManager &i_pulseManager
        )
{
    memcpy((void*) &m_public_header,(void*)&i_pulseManager.m_public_header,
           sizeof(m_public_header));
    memcpy((void*) &m_wfInfo,(void *) &i_pulseManager.m_wfInfo,
           sizeof(m_wfInfo));
    m_pulses.resize(i_pulseManager.m_pulses.size());
    for(unsigned int i=0; i<m_pulses.size(); ++i)
    {
        m_pulses[i] = new Pulse(*i_pulseManager.m_pulses[i]);
    }
}

//-----------------------------------------------------------------------------
Object *PulseManager::fillObject(
        Object *obj,
        const std::vector<double> &user_limits
        ) const
{
   obj->setNoiseLevel(m_noiseLevel);
   for(unsigned int i=0; i< m_pulses.size(); ++i)
   {
      if(m_pulses[i]->isInsideLimits(user_limits))
      {
         m_pulses[i]->addIntensitiesToObject(obj);
      }
   }
   obj->normalise();
   return obj;
}

//-----------------------------------------------------------------------------
void PulseManager::setNoiseLevel(double i_noiseLevel)
{
   m_noiseLevel = i_noiseLevel;
}

//-----------------------------------------------------------------------------
void PulseManager::addPoint(
        const Types::Data_Point_Record_Format_4 &i_point,
        const char *wave_data
        )
{
   float originZ = i_point.Z*m_public_header.z_scale_factor +
           (float)i_point.Z_t*i_point.return_point_wf_location;
   if(originZ < 110.f)
   {
      Pulse *pulse = new Pulse(m_public_header,m_wfInfo,i_point,wave_data);
      m_pulses.push_back(pulse);
      ngl::Vec3 &origin = m_pulses[m_pulses.size()-1]->getOrigin();
      ngl::Vec3 &offset = m_pulses[m_pulses.size()-1]->getOffset();
      ngl::Vec3 endPoint = m_wfInfo.number_of_samples * offset;
      endPoint+=origin;
      m_public_header.max_x = std::max((double)origin.m_x,m_public_header.max_x);
      m_public_header.max_x = std::max((double)endPoint.m_x,m_public_header.max_x);
      m_public_header.max_y = std::max((double)origin.m_y,m_public_header.max_y);
      m_public_header.max_y = std::max((double)endPoint.m_y,m_public_header.max_y);
      m_public_header.max_z = std::max((double)origin.m_z,m_public_header.max_z);
      m_public_header.max_z = std::max((double)endPoint.m_z,m_public_header.max_z);
   }
}


////-----------------------------------------------------------------------------
//pcl::PointCloud<pcl::PointXYZI>::Ptr PulseManager::getPeakPointsCloud(
//        const std::vector<double> &i_user_limits
//        )
//{
//   std::cout << "kernel values = " << Pulse::s_kernel[0] << " "
//             << Pulse::s_kernel[1] << " " << Pulse::s_kernel[2] << " "
//             << Pulse::s_kernel[3] << " " << Pulse::s_kernel[4] << "\n";

//   pcl::PointCloud<pcl::PointXYZI>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZI>);

//   for(unsigned int i=0; i<m_pulses.size(); ++i)
//   {
//       if(m_pulses[i]->isInsideLimits(i_user_limits))
//       {
//           /// TO DO: addPeaksTOCloud
//       }
//   }
//   return cloud;
//}

//-----------------------------------------------------------------------------
void PulseManager::setKernel(const std::vector<double> &i_kernel)
{
   Pulse::s_kernel.resize(i_kernel.size());
   for(unsigned int i=0; i<Pulse::s_kernel.size(); ++i)
   {
      Pulse::s_kernel[i] = i_kernel[i];
   }
}


////-----------------------------------------------------------------------------
//void PulseManager::clearPeaks()
//{
//   for (unsigned int i=0; i< m_pulses.size(); ++i)
//   {
//      m_pulses[i]->clearPeaks();
//   }
//}

//-----------------------------------------------------------------------------
void PulseManager::createQuadtree(unsigned int i_stopLimit)
{
    // first free memory if a quadtree has been previously created
    if(m_quadTree!=0)
    {
        delete m_quadTree;
    }

    // pack the limits of the area
    std::vector<double> m_areaLimits;
    m_areaLimits.resize(4);

    // make the limits a square to avoid streched quads
    double disX = m_public_header.max_x - m_public_header.min_x;
    double disY = m_public_header.max_y - m_public_header.min_y;

    m_areaLimits[1]=m_public_header.min_y;
    m_areaLimits[3]=m_public_header.min_x;
    if(disX>disY)
    {
        m_areaLimits[0] = m_areaLimits[1]+disX;
        m_areaLimits[2] = m_public_header.max_x;
    }
    else
    {
        m_areaLimits[0] = m_public_header.max_y;
        m_areaLimits[2] = m_areaLimits[3]+disY;
    }

    // one of the conditions of creating a Quadtree is that m_pulses should be
    // sorted in the y axis.
    sortPulseWithRespectToY();

    // finally create the quadTree
    m_quadTree = new QuadTreeNode(&m_pulses[0],m_pulses.size(),
                                  m_areaLimits,i_stopLimit);
}

//-----------------------------------------------------------------------------
void PulseManager::sortPulseWithRespectToY()
{
    // array to be sorted
    unsigned int len = m_pulses.size();
    // allocate memory for temporarly saved values
    std::vector<Pulse *> tempValues;
    tempValues.resize(len);
    // start sorting 2 elements each time, then merge them with the two next to them etc
    for(unsigned int step=2; step/2 < len; step*=2)
    {
       for(unsigned int i=0; i < len; i+=step)
       {
          unsigned int endOfT2 = i+step;
          if(i+ step/2 >= len)
          {
             continue;
          }
          else if (i+step >=len)
          {
             endOfT2 = len;
          }
          // both sets have step/2 items.
          // t1 points to the first set of values
          unsigned int t1 = i;
          // t2 points to the second set of values
          unsigned int t2 = i+step/2;
          // here we save all the values that have been overridden from the first set
          unsigned int tempIndex=0;
          while(t1 < i+step/2 && t2 < endOfT2)
          {
             if(m_pulses[t1]->m_origin.m_y>m_pulses[t2]->m_origin.m_y)
             {
                tempValues[tempIndex]=m_pulses[t1];
                t1++;
             }
             else
             {
                tempValues[tempIndex]=m_pulses[t2];
                t2++;
             }
             tempIndex++;
          }
          while(t1<i+step/2)
          {
             tempValues[tempIndex]=m_pulses[t1];
             t1++;
             tempIndex++;
          }
          // write values back to the array
          for(unsigned int t=0; t < tempIndex; ++t)
          {
              m_pulses[i+t]=tempValues[t];
          }
       }
    }
}

//-----------------------------------------------------------------------------
double PulseManager::getMaxX()const
{
   return m_public_header.max_x;
}

//-----------------------------------------------------------------------------
double PulseManager::getMaxY()const
{
   return m_public_header.max_y;
}

//-----------------------------------------------------------------------------
double PulseManager::getMaxZ()const
{
   return m_public_header.max_z;
}


//-----------------------------------------------------------------------------
double PulseManager::getMinX()const
{
    return m_public_header.min_x;
}

//-----------------------------------------------------------------------------
double PulseManager::getMinY()const
{
    return m_public_header.min_y;
}

//-----------------------------------------------------------------------------
double PulseManager::getMinZ()const
{
    return m_public_header.min_z;
}


////-----------------------------------------------------------------------------
//void PulseManager::addToCloud(
//        pcl::PointCloud<pcl::PointXYZI>::Ptr  &cloud,
//        const std::vector<double> &i_user_limits
//        )
//{
//    Pulse::count = 0;


//    std::cout << "File Limits = " << m_public_header.max_y <<" "<< m_public_header.min_y
//              <<" "<<m_public_header.max_x << " " << m_public_header.min_x << "\n";
//    std::cout << "user defined limits: " << i_user_limits[0] << " "
//              <<i_user_limits[1] <<" "<< i_user_limits[2] << " " << i_user_limits[3];

//    for(unsigned int i=0; i< m_pulses.size(); ++i)
//    {
//       if(m_pulses[i]->isInsideLimits(i_user_limits))
//       {
//          m_pulses[i]->addAllReturnsToCloud(cloud);
//       }
//    }
//    std::cout << Pulse::count << " pulses has been added to the cloud\n";
//}

//-----------------------------------------------------------------------------
PulseManager::~PulseManager()
{
   for(unsigned int i=0; i<m_pulses.size(); ++i)
   {
      delete m_pulses[i];
   }
//   if(m_quadTree!=0)
//   {
//       delete m_quadTree;
//       // no need to delete its children because every node deletes each
//       // children
//   }
}

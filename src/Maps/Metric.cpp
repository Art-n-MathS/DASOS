#include "Metric.h"

//-----------------------------------------------------------------------------
Metric::Metric(
        const std::string i_name,
        Volume *i_obj
        ):
    Map(i_name,i_obj)
{
}


//-----------------------------------------------------------------------------
void Metric::createMap()
{
   // Loop through all the voxels and generate the metric of interest
   // the variable m_noOfPixelsX, m_noOfPixelsY and m_noOfPixelsZ gives you
   // the the number of voxels in x,y,z axis respectively
   for(unsigned int x=0; x<m_noOfPixelsX; ++x)
   {
      for(unsigned int y=0; y<m_noOfPixelsY; ++y)
      {
         for(unsigned int z=0; z<m_noOfPixelsZ; ++z)
         {
            // in m_mapValues all the values of the metrics are saved
            // the following command assigns the value 0 at the (x,y) position
            // of the map
            m_mapValues[getIndex(x,y)]=-0.0f;

            // the voxelised 3D volume is the m_object variable and some
            // useful examples of using it are shown below:

            // get the length of the voxel
            float voxelLength = m_object->getVoxelLen();
            // get the intesity at voxel (x,y,z)
            float intensity = m_object->getIntensity(x,y,z);
            // check whether the value of the voxel at (x,y,z) is considered to
            // be inside or outside the scanned object. This checks whether the
            // intensity value is above the boundary threshold.
            bool isInside = m_object->isInside(x,y,z);
         }
      }
   }
}

//-----------------------------------------------------------------------------
Metric::~Metric()
{}

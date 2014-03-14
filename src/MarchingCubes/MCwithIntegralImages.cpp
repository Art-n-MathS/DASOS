#include "MCwithIntegralImages.h"

//-----------------------------------------------------------------------------
MCwithIntegralImages::MCwithIntegralImages(
        Object *i_obj,
        unsigned int i_x
        ):
    MarchingCubes(i_obj,i_x)
{
}




//-----------------------------------------------------------------------------
void MCwithIntegralImages::computeVertices(GLData *i_glData)
{
    double isolevel = m_obj->getIsolevel();
    std::vector<ngl::Vec3> points;
    points.resize(8);
    for(unsigned int x=0; x<m_numOfCubsX-1; ++x)
    {
       for(unsigned int y=0; y<m_numOfCubsY-1; ++y)
       {
          for (unsigned int z=0; z<m_numOfCubsZ-1; ++z)
          {
             points[0]= getXYZ(x  , y   ,z  );
             points[1]= getXYZ(x+1, y   ,z  );
             points[2]= getXYZ(x+1, y   ,z+1);
             points[3]= getXYZ(x  , y   ,z+1);
             points[4]= getXYZ(x  , y+1 ,z  );
             points[5]= getXYZ(x+1, y+1 ,z  );
             points[6]= getXYZ(x+1, y+1 ,z+1);
             points[7]= getXYZ(x  , y+1 ,z+1);
             Polygonise(points,isolevel,i_glData);
          }
       }
    }
}

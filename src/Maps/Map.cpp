#include "Map.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>

//-----------------------------------------------------------------------------
Map::Map(const std::string i_name,
        Volume *i_obj
        ):
    m_name(i_name),
    m_object(i_obj),
    m_noOfPixelsX(i_obj->getNoVoxelsX()),
    m_noOfPixelsY(i_obj->getNoVoxelsY()),
    m_noOfPixelsZ(i_obj->getNoVoxelsZ())
{
   m_mapValues.resize(m_noOfPixelsX*m_noOfPixelsY);
}

//-----------------------------------------------------------------------------
Map::Map(const Map &i_map):
    m_name(i_map.m_name),
    m_object(i_map.m_object),
    m_noOfPixelsX(i_map.m_noOfPixelsX),
    m_noOfPixelsY(i_map.m_noOfPixelsY),
    m_noOfPixelsZ(i_map.m_noOfPixelsZ)
{
   m_mapValues.resize(m_noOfPixelsX*m_noOfPixelsY);
   for (unsigned int i=0; i<m_mapValues.size() ; ++i)
   {
       m_mapValues[i] = i_map.m_mapValues[i];
   }
}

//-----------------------------------------------------------------------------
bool Map::isInside(unsigned int i_x, unsigned int i_y, unsigned int i_z)
{
    return m_object->isInside(i_x, i_y, i_z);
}

//-----------------------------------------------------------------------------
float Map::getIntensity(unsigned int i_x, unsigned int i_y, unsigned int i_z)
{
   return m_object->getIntensity(i_x,i_y,i_z);
}

//-----------------------------------------------------------------------------
void Map::saveAsc()
{ 
    std::string TextFileName = m_name + ".asc";
    std::ofstream myfile;
    myfile.open (TextFileName.c_str());
    myfile << "ncols "       << std::setprecision(8) << m_object->getNoVoxelsX()
           << "\nnrows "     << std::setprecision(8) << m_object->getNoVoxelsY()
           << "\nxllcorner " << m_object->getMinLimits()[0]
           << "\nyllcorner " << m_object->getMinLimits()[1]
           << "\ncellsize "  << m_object->getVoxelLen()
           << "\nNODATA_value -0\n";

    for(unsigned y=0; y<m_noOfPixelsY; ++y)
    {
       for(unsigned int x=0; x<m_noOfPixelsX-1; ++x)
       {
          myfile << m_mapValues[getIndex(x,m_noOfPixelsY-1-y)] << " ";
       }
       myfile << m_mapValues[getIndex(m_noOfPixelsX-1,m_noOfPixelsY-1-y)] << "\n";
    }
    myfile.close();

    std::cout << TextFileName << " is saved\n";
}

//-----------------------------------------------------------------------------
void Map::createAndSave(unsigned int /*i_thres*/, unsigned int /*i_sample*/)
{
    if(m_mapValues.size()==0)
    {
       std::cout << "Map has not been initialised correctly\n";
       return;
    }
    createMap();
    saveAsc();
}

//-----------------------------------------------------------------------------
Map::~Map()
{
}


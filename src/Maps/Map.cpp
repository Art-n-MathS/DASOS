#include "Map.h"
#include <iostream>
#include <QImage>

//-----------------------------------------------------------------------------
Map::Map(
        const std::string i_name,
        Object *i_obj
        ):
    m_name(i_name),
    m_object(i_obj),
    m_noOfPixelsX(i_obj->m_noOfVoxelsX),
    m_noOfPixelsY(i_obj->m_noOfVoxelsY),
    m_noOfPixelsZ(i_obj->m_noOfVoxelsZ)
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

////-----------------------------------------------------------------------------
//Map Map::operator +(const Map &i_map)const
//{
//   return i_map;
//}

//-----------------------------------------------------------------------------
Map &Map::operator =(const Map &i_map)
{
   if(this==&i_map)
   {
      return *this;
   }
   m_name = i_map.m_name;
   m_object = i_map.m_object;
   m_noOfPixelsX = i_map.m_noOfPixelsX;
   m_noOfPixelsY = i_map.m_noOfPixelsY;
   m_noOfPixelsZ = i_map.m_noOfPixelsZ;
   m_mapValues.resize(m_noOfPixelsX*m_noOfPixelsY);
   for (unsigned int i=0; i<m_mapValues.size() ; ++i)
   {
      m_mapValues[i] = i_map.m_mapValues[i];
   }
   return *this;
}

//-----------------------------------------------------------------------------
Map &Map::operator -=(const Map &i_map)
{
   if(m_object!=i_map.m_object && m_noOfPixelsX!=i_map.m_noOfPixelsX &&
     m_noOfPixelsY!= i_map.m_noOfPixelsY && m_noOfPixelsZ!=i_map.m_noOfPixelsZ)
   {
      std::cout<<"WARNING: Maps should always come from the same object and "
                <<"have the same resolution.\n First object will be retrned\n";
      return *this;
   }
   for(unsigned int i=0; i<m_mapValues.size(); ++i)
   {
      m_mapValues[i]-=i_map.m_mapValues[i];
   }
   return *this;
}
//-----------------------------------------------------------------------------
Map &Map::operator +=(const Map &i_map)
{
   if(m_object!=i_map.m_object && m_noOfPixelsX!=i_map.m_noOfPixelsX &&
     m_noOfPixelsY!= i_map.m_noOfPixelsY && m_noOfPixelsZ!=i_map.m_noOfPixelsZ)
   {
      std::cout<<"WARNING: Maps should always come from the same object and "
                <<"have the same resolution.\n First object will be retrned\n";
      return *this;
   }
   for(unsigned int i=0; i<m_mapValues.size(); ++i)
   {
      m_mapValues[i]+=i_map.m_mapValues[i];
   }
   return *this;
}

//-----------------------------------------------------------------------------
const Map Map::operator +(const Map &i_map)const
{
    return Map (*this) += i_map;
}

//-----------------------------------------------------------------------------
bool Map::isInside(unsigned int i_x, unsigned int i_y, unsigned int i_z)
{
    return (m_object->m_intensities[i_x+i_y*m_noOfPixelsX+i_z*
            m_noOfPixelsX*m_noOfPixelsY]>=m_object->m_isolevel);
}

//-----------------------------------------------------------------------------
float Map::getIntensity(unsigned int i_x, unsigned int i_y, unsigned int i_z)
{
   return m_object->m_intensities[i_x+i_y*m_noOfPixelsX+
                                  i_z*m_noOfPixelsX*m_noOfPixelsY];
}

//-----------------------------------------------------------------------------
void Map::threshold(unsigned int i_thres)
{
   for(unsigned int i=0; i<m_mapValues.size(); ++i)
   {
      if (m_mapValues[i]> 0.0001)
      {
         if (m_mapValues[i]<i_thres )
         {
            m_mapValues[i] = 1.0f;
         }
         else
         {
            m_mapValues[i] = 255.0f;
         }
      }
   }
}

//-----------------------------------------------------------------------------
void Map::sample(unsigned int i_samp)
{
   float d = 256/pow(2.0,i_samp);
   for(unsigned int i=0; i<m_mapValues.size(); ++i)
   {
       m_mapValues[i]/=d;
   }
   normalise();
}

//-----------------------------------------------------------------------------
void Map::normalise()
{
   float min = m_mapValues[0];
   float max = m_mapValues[0];
   for(unsigned int i=0; i<m_mapValues.size(); ++i)
   {
      if(min>m_mapValues[i])
      {
         min=m_mapValues[i];
      }else if (max<m_mapValues[i])
      {
         max = m_mapValues[i];
      }
   }
   std::cout << "max min = " << max << " " << min << "\n";
   for(unsigned int i=0; i<m_mapValues.size(); ++i)
   {
       m_mapValues[i] =(m_mapValues[i]-min)/(max-min)*255.0f;
   }
}

//-----------------------------------------------------------------------------
void Map::createAndSave(unsigned int i_thres, unsigned int i_sample)
{
    if(m_mapValues.size()==0)
    {
       std::cout << "Map has not been initialised correctly\n";
       return;
    }
    createMap();
    normalise();
    if(i_thres!=0)
    {
       threshold(i_thres);
    }
    else if (i_sample!=0)
    {
       sample(i_sample);
    }
    else
    {
       // keep original image
    }
    saveMapToImage();
}

//-----------------------------------------------------------------------------
void Map::saveMapToImage() const
{
    if (m_mapValues.size()!=m_noOfPixelsX*m_noOfPixelsY)
    {
       std::cout << "Length of i_mapValues is wrong! Image map not saved\n";
       return;
    }
    QImage *image =new QImage(m_noOfPixelsX, m_noOfPixelsY,QImage::Format_RGB16);
    for(unsigned int x=0; x<m_noOfPixelsX; ++x)
    {
       for(unsigned int y=0; y<m_noOfPixelsY; ++y)
       {
          int value = (m_mapValues[getIndex(x,y)]);
          image->setPixel(x,y,qRgb(value,value,value));
       }
    }
    image->save(m_name.c_str());
    delete image;
}

//-----------------------------------------------------------------------------
Map::~Map()
{
}


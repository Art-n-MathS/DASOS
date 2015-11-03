#include "Map.h"
#include <iostream>
#include <fstream>
#include <QImage>
#include <iomanip>

//-----------------------------------------------------------------------------
Map::Map(
        const std::string i_name,
        Object *i_obj
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
    return m_object->isInside(i_x, i_y, i_z);
}

//-----------------------------------------------------------------------------
float Map::getIntensity(unsigned int i_x, unsigned int i_y, unsigned int i_z)
{
   return m_object->getIntensity(i_x,i_y,i_z);
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
   unsigned int i=-1;
   while (m_mapValues[++i]<0.0001);
   float min = m_mapValues[i];
   float max = m_mapValues[i];
   for(++i;i<m_mapValues.size(); ++i)
   {
      if(min>m_mapValues[i] && m_mapValues[i]>0.0001)
      {
         min=m_mapValues[i];
      }else if (max<m_mapValues[i] && m_mapValues[i]>0.0001)
      {
         max = m_mapValues[i];
      }
   }
   if(max-min < 0.0000001)
   {
      std::cout << "WARNING: Map cannot be normalised because all values are zero\n";
      return;
   }
   std::cout << "max min = " << max << " " << min << "\n";
   for(unsigned int i=0; i<m_mapValues.size(); ++i)
   {
      if(m_mapValues[i]<0.0001)
      {
         m_mapValues[i] =0.0f;
      }
      else
      {
         m_mapValues[i] =(m_mapValues[i]-min)/(max-min)*255.0f;
      }
   }
}

//-----------------------------------------------------------------------------
void Map::saveTxt()
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

//    myfile << "ncols 184\nnrows 1424\nxllcorner 435299\nyllcorner 100985\ncellsize 3.5\nNODATA_value -0\n";

    for(unsigned y=0; y<m_noOfPixelsY; ++y)
    {
       for(unsigned int x=0; x<m_noOfPixelsX-1; ++x)
       {
          myfile << m_mapValues[getIndex(x,m_noOfPixelsY-1-y)] << " ";
       }
       myfile << m_mapValues[getIndex(m_noOfPixelsX-1,m_noOfPixelsY-1-y)] << "\n";
    }
//    for(unsigned int x=0; x<m_noOfPixelsX-1; ++x)
//    {
//       myfile << m_mapValues[getIndex(x,m_noOfPixelsY-1)] << " ";
//    }
//    myfile << m_mapValues[getIndex(m_noOfPixelsX-1,m_noOfPixelsY-1)];
    myfile.close();

    std::cout << TextFileName << " is saved\n";
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
    saveTxt();
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
    std::cout<< "Ready to save image\n";
    saveMapToImage();
}

//-----------------------------------------------------------------------------
void Map::saveMapToImage()
{
    std::cout << "hohohohohohohohojhokooooooo\n";
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
          image->setPixel(x,(m_noOfPixelsY-1-y),qRgb(value,value,value));
       }
    }
    image->save(m_name.c_str());
    delete image;
}

//-----------------------------------------------------------------------------
Map::~Map()
{
}


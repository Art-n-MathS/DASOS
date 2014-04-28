#include "Grid.h"
#include <bilLib/binfile.h>
#include <math.h>

//-----------------------------------------------------------------------------
Grid::Grid(const std::string &i_IGMfileName,
        const unsigned short i_samplingRate
           ): m_Xs(NULL), m_Ys(NULL)
{
   if(i_IGMfileName!="")
   {
      readIGMandFindMinsMaxs(i_IGMfileName);
      m_nX = (unsigned int)sqrt(double(m_nsamps*m_nsamps/i_samplingRate));
      m_nY = (unsigned int)sqrt(double(m_nlines*m_nlines/i_samplingRate));
      fillGrid();
   }
   else
   {
      // IGM file does not exist
   }
}

//-----------------------------------------------------------------------------
Grid::Grid(
        const std::string &i_IGMfileName,
        const float i_vl,
        const ngl::Vec3 i_LiDARmins
        )
{
   if(i_IGMfileName!="")
   {
      readIGMandFindMinsMaxs(i_IGMfileName);
      // calculate no of squares in x,y axes
      float n1x = (i_LiDARmins.m_x-m_min.m_x)/i_vl;
      float n1y = (i_LiDARmins.m_y-m_min.m_y)/i_vl;
      float n2x = (m_max.m_x-i_LiDARmins.m_x)/i_vl;
      float n2y = (m_max.m_y-i_LiDARmins.m_y)/i_vl;
      if(n1x<0.0f)
      {
         m_min.m_x = i_LiDARmins.m_x;
         n1x = 0.0f;
      }
      else
      {
         m_min.m_x = i_LiDARmins.m_x - ceil(n1x)*i_vl;
      }
      if(n1y<0.0f)
      {
         m_min.m_y = i_LiDARmins.m_y;
         n1y = 0.0f;
      }
      else
      {
         m_min.m_y = i_LiDARmins.m_y - ceil(n1y)*i_vl;
      }
      m_max.m_x = i_LiDARmins.m_x + ceil(n2x)*i_vl;
      m_max.m_y = i_LiDARmins.m_y + ceil(n2y)*i_vl;

      m_nX = ceil(n1x) + ceil(n2x);
      m_nY = ceil(n1y) + ceil(n2y);
      fillGrid();
   }
   else
   {
      // IGM file does not exist
   }
}

//-----------------------------------------------------------------------------
void Grid::readIGMandFindMinsMaxs(const std::string &i_IGMfileName)
{
    try
    {
       bilLib::BinFile file(i_IGMfileName);
       unsigned char dataType =
               bilLib::StringToUINT(file.FromHeader("data type"));
       if(dataType!=5)
       {
          std::cout << "Class Grid only accept bil files with data type 5\n";
          std::cout << "Please update class Grid to accept more types.\n";
          return;
       }
       // get number of samples and number of lines
       m_nsamps=bilLib::StringToUINT(file.FromHeader("samples"));
       m_nlines=bilLib::StringToUINT(file.FromHeader("lines"));
       // calculate the number of squares that corresponds to x,y axes

       unsigned int nbands=bilLib::StringToUINT(file.FromHeader("bands"));
       if(nbands<3)
       {
          std::cout << "Missing information in IGM file. Number of bands"
                    << "should be 3\n";
          return;
       }
       m_Xs = new double[m_nsamps*m_nlines];
       m_Ys = new double[m_nsamps*m_nlines];
       file.Readband((char *)m_Xs,0);
       file.Readband((char *)m_Ys,1);
       m_min.m_x = m_Xs[0];
       m_max.m_x = m_Xs[0];
       m_min.m_y = m_Ys[0];
       m_max.m_y = m_Ys[0];
       for(unsigned int i=1; i<m_nsamps*m_nlines; ++i)
       {
          if (m_min.m_x>m_Xs[i])
          {
             m_min.m_x=m_Xs[i];
          } else if (m_max.m_x<m_Xs[i])
          {
             m_max.m_x=m_Xs[i];
          }
          if (m_min.m_y>m_Ys[i])
          {
             m_min.m_y=m_Ys[i];
          } else if (m_max.m_y<m_Ys[i])
          {
             m_max.m_y=m_Ys[i];
          }
       }
       std::cout << "X: " << m_min.m_x << " " << m_max.m_x << "\n";
       std::cout << "Y: " << m_min.m_y << " " << m_max.m_y << "\n";
       file.Close();
    }
    catch(bilLib::BinaryReader::BRexception e)
    {
       std::cout<<e.what()<<std::endl;
       std::cout<<e.info<<std::endl;
    }
}

//-----------------------------------------------------------------------------
void Grid::fillGrid()
{
   // each pixel is associated with the square it lies in
   for(unsigned int y=0; y<m_nlines; ++y)
   {
      // add pixels of current line to the grid
      for(unsigned int x=0; x<m_nsamps; ++x)
      {
         const unsigned int pixelIndex = getKeyOfPixel(x,y);
         // the geographical position of the pixel
         unsigned int sqX = double((m_Xs[pixelIndex]-m_min.m_x)/
                                   (m_max.m_x-m_min.m_x))*m_nX;
         unsigned int sqY = double((m_Ys[pixelIndex]-m_min.m_y)/
                                   (m_max.m_y-m_min.m_y))*m_nY;
         m_map.emplace(getKeyOfSquare(sqX,sqY),pixelIndex);
      }
   }
}

//-----------------------------------------------------------------------------
unsigned short int Grid::pixIndicesOfSquare(
        const ngl::Vec2 &i_point,
        unsigned int **i_pixPos
        ) const
{
   if(m_Xs==NULL || m_Ys==NULL)
   {
      std::cout << "WARNING: grid haven't been initialised proberly\n";
      return 0;
   }
   const unsigned int x = float((i_point.m_x-m_min.m_x)/
                                (m_max.m_x-m_min.m_x)*(float)m_nX);
   const unsigned int y = float((i_point.m_y-m_min.m_y)/
                                (m_max.m_y-m_min.m_y)*(float)m_nY);
   unsigned short int noOfPoints = m_map.count(getKeyOfSquare(x,y));
   (*i_pixPos) = new unsigned int [noOfPoints];
   auto itsElements = m_map.equal_range(getKeyOfSquare(x,y));
   unsigned short int count = 0;
   for (auto it = itsElements.first; it != itsElements.second; ++it)
   {
      (*i_pixPos)[count] = it->second;
      count++;
   }
   return noOfPoints;
}

//-----------------------------------------------------------------------------
ngl::Vec2 Grid::getPixelPosFromKey(const unsigned int i_a)const
{
   return ngl::Vec2(i_a%m_nsamps, ((i_a-(i_a%m_nsamps))/m_nsamps));
}

//-----------------------------------------------------------------------------
unsigned int Grid::getKeyOfSquare(
        const unsigned int i_x,
        const unsigned int i_y
        )const
{
    return i_x + i_y * m_nX;
}

//-----------------------------------------------------------------------------
unsigned int Grid::getKeyOfPixel(
        const unsigned int i_x,
        const unsigned int i_y
        ) const
{
   return i_x + i_y * m_nsamps;
}


//-----------------------------------------------------------------------------
ngl::Vec2 Grid::getPixelPositionScaled0_1(
        const float i_x,
        const float i_y
        )const
{
//   if(m_nX==NULL || m_nY==NULL)
//   {
//      std::cout << "WARNING: grid haven't been initialised proberly\n";
//      return ngl::Vec2(0,0);
//   }
   const unsigned int x = float((i_x-m_min.m_x)/
                                (m_max.m_x-m_min.m_x)*(float)m_nX);
   const unsigned int y = float((i_y-m_min.m_y)/
                                (m_max.m_y-m_min.m_y)*(float)m_nY);
   const ngl::Vec2 point(i_x,i_y);
   ngl::Vec2 pixelPos(0,0);
   unsigned int currentPixelKey = 0;
   float minDis = sqrt(pow(m_Xs[0]-point.m_x,2.0)+pow(m_Ys[0]-point.m_y,2.0));
   auto itsElements = m_map.equal_range(getKeyOfSquare(x,y));
   for (auto it = itsElements.first; it != itsElements.second; ++it)
   {
      const float distance = sqrt(pow(m_Xs[it->second]-point.m_x,2.0)+
                                  pow(m_Ys[it->second]-point.m_y,2.0));
      if(distance<minDis)
      {
         currentPixelKey = it->second;
         minDis = distance;
      }
   }
   pixelPos = getPixelPosFromKey(currentPixelKey);
   pixelPos.m_x/=m_nsamps;
   pixelPos.m_y/=m_nlines;
   pixelPos.m_y = -pixelPos.m_y;
   return pixelPos;
}



//-----------------------------------------------------------------------------
Grid::~Grid()
{
   if (m_Xs!=NULL)
   {
      delete [] m_Xs;
   }
   if(m_Ys!=NULL)
   {
      delete [] m_Ys;
   }
}

#include "CreateTreeTemplates.h"

#include <QImage>
#include <QString>
#include <QColor>
#include <iostream>
#include <fstream>
#include <VolumeFactory.h>
#include <iomanip>
#include <math.h>

//-----------------------------------------------------------------------------
CreateTreeTemplates::CreateTreeTemplates(
        const std::string &i_volume
        ):
    m_obj(NULL),
    m_freeObject(true)
{
   m_obj = VolumeFactory::produceVolume(i_volume,"1D_ARRAY");
   gmtl::Vec3f lowerLimits = m_obj->getMinLimits();
   gmtl::Vec3f highLimits = m_obj->getMaxLimits();
   std::cout << "OBJECT LIMITS \n ----------------------\n";
   std::cout << highLimits[0] << " " << highLimits[1] << " " << highLimits[2] << "\n";
   std::cout << lowerLimits[0] << " " << lowerLimits[1] << " " << lowerLimits[2] << "\n";
}


////-----------------------------------------------------------------------------
//void CreateTreeTemplates::addSamples(const Int_PlotsManager *i_plots, bool alive)
//{
//   gmtl::Vec3f lowerLimits = m_obj->getMinLimits();
//   float voxelLenght= m_obj->getVoxelLen();
//   unsigned int noOfVoxelsX = m_obj->getNoVoxelsX();
//   unsigned int noOfVoxelsY = m_obj->getNoVoxelsY();

//   const Int_Plot *plotAOI = i_plots->getPlotOfAOI(lowerLimits[0],lowerLimits[1],
//           voxelLenght*float(noOfVoxelsX),float(noOfVoxelsY)*voxelLenght);
//   m_currentPlot = plotAOI->m_plotID;
//   if(plotAOI==NULL)
//   {
//      std::cout << "AOI was not found\n";
//      std::exit(EXIT_FAILURE);
//   }
//   for(unsigned int i=0; i<plotAOI->m_trees.size(); ++i)
//   {
//      if(plotAOI->m_trees[i]->isDead || alive)
//      {
//         gmtl::Vec2i treeVoxel = m_obj->getIndices(
//                  plotAOI->m_trees[i]->xGPS,plotAOI->m_trees[i]->yGPS);
//         m_trees.push_back(std::make_pair(treeVoxel[0],treeVoxel[1]));
//         m_GPStrees.push_back(std::make_pair(plotAOI->m_trees[i]->xGPS,plotAOI->m_trees[i]->yGPS));
//         m_isDead.push_back(plotAOI->m_trees[i]->isDead);
//      }
//   }
//   std::cout << "No of Trees added: " << m_trees.size() << " " << plotAOI->m_trees.size() <<"\n";
//}

////-----------------------------------------------------------------------------
//void CreateTreeTemplates::addAllPixelsAsSamples(
//        const Int_PlotsManager *i_plots,
//        const float i_offset)
//{
//    gmtl::Vec3f lowerLimits = m_obj->getMinLimits();
//    float voxelLenght= m_obj->getVoxelLen();
//    unsigned int noOfVoxelsX = m_obj->getNoVoxelsX();
//    unsigned int noOfVoxelsY = m_obj->getNoVoxelsY();

//    const Int_Plot *plotAOI = i_plots->getPlotOfAOI(lowerLimits[0],lowerLimits[1],
//            voxelLenght*float(noOfVoxelsX),float(noOfVoxelsY)*voxelLenght);
//    m_currentPlot = plotAOI->m_plotID;
//    if(plotAOI==NULL)
//    {
//       std::cout << "AOI was not found\n";
//       std::exit(EXIT_FAILURE);
//    }
//    for(unsigned int x=0; x<noOfVoxelsX; ++x)
//    {
//       for(unsigned int y=0; y<noOfVoxelsY; ++y)
//       {
//          float x_pos = float(x) * voxelLenght + lowerLimits[0] + voxelLenght/2;
//          float y_pos = float(y) * voxelLenght + lowerLimits[1] + voxelLenght/2;
//          if(plotAOI->isPointInsidePlot(x_pos,y_pos,i_offset))
//          {
//             // add column in the list
//             m_trees.push_back(std::make_pair(x,y));
//             m_GPStrees.push_back(std::make_pair(x_pos,y_pos));
//          }
//       }
//    }
//    std::cout << "All Columns added\n";
//}


//-----------------------------------------------------------------------------
CreateTreeTemplates::CreateTreeTemplates(
        const std::string &i_volume,
        const std::string &i_image
        ):
    m_obj(NULL),
    m_freeObject(true)
{
   m_obj =VolumeFactory::produceVolume(i_volume,"1D_ARRAY");
   addSamples(i_image);
}

//-----------------------------------------------------------------------------
CreateTreeTemplates::CreateTreeTemplates(
        Volume *i_obj
        ):
    m_obj(i_obj),
    m_freeObject(false)
{

}


//-----------------------------------------------------------------------------
// the .header file is important because the given boudaries of the fieldplots
// are wrong so we adjust them according to where the data lie and any tree
// far away from the data are ignore. So the .header file contains the new
// pixelwise boundaries of the fieldplot
// The format of the file is the following
//-----------------------------------------------------------------------------
// startXY <x_start> <y_start>
// endXY <x_end> <y_end>
//-----------------------------------------------------------------------------
void CreateTreeTemplates::addAllSamples(const std::string &i_image)
{
   std::cout << "start to add templates\n";
   QString fileName(i_image.c_str());
   QImage image = QImage(fileName);

   int x_end(0);
   int y_end(0);
   int x_start(image.width());
   int y_start(image.height());

   std::cout << "Initial start end values: \n";
   std::cout << "start xy: " << x_start << " " << y_start << "\n";
   std::cout << "end   xy: " << x_end   << " " << y_end   << "\n";

   // correcting boundaries
   for(int y=0; y<image.height(); ++y)
   {
      for(int x=0; x<x_start; ++x)
      {
         QColor colour(image.pixel(QPoint(x,image.height()-1-y)));
         qreal r,g,b;
         colour.getRgbF(&r,&g,&b);
         if(colour.red()>0.9999f && colour.green()<0.0001f && colour.blue()<0.0001f)
         {
            x_start = x_start<x ? x_start : x;
            continue;
         }
      }
   }
   for( int y=0; y<image.height(); ++y)
   {
      for(int x=image.width()-1; x>x_end; --x)
      {
         QColor colour(image.pixel(QPoint(x,image.height()-1-y)));
         qreal r,g,b;
         colour.getRgbF(&r,&g,&b);
         if(colour.red()>0.9999f && colour.green()<0.0001f && colour.blue()<0.0001f)
         {
            x_end = x_end>x ? x_end : x;
            continue;
         }
      }
   }

   for(int x=0; x<x_end; ++x)
   {
      for(int y=0; y<y_start; ++y)
      {
         QColor colour(image.pixel(QPoint(x,image.height()-1-y)));
         qreal r,g,b;
         colour.getRgbF(&r,&g,&b);
         if(colour.red()>0.9999f && colour.green()<0.0001f && colour.blue()<0.0001f)
         {
            y_start = y_start<y ? y_start : y;
            continue;
         }
      }
   }
   for(int x=0; x<x_end; ++x)
   {
      for(int y=image.height()-1; y>y_end; --y)
      {
         QColor colour(image.pixel(QPoint(x,image.height()-1-y)));
         qreal r,g,b;
         colour.getRgbF(&r,&g,&b);
         if(colour.red()>0.9999f && colour.green()<0.0001f && colour.blue()<0.0001f)
         {
            y_end = y_end>y ? y_end : y;
            continue;
         }
      }
   }

   std::cout << "start xy: " << x_start << " " << y_start << "\n";
   std::cout << "end   xy: " << x_end   << " " << y_end   << "\n";

   // adding templates trees to the tree and non tree list
   for(int x=x_start; x<x_end; ++x)
   {
      for(int y=y_start; y<y_end; ++y)
      {
         QColor colour(image.pixel(QPoint(x,y)));
         qreal r,g,b;

         colour.getRgbF(&r,&g,&b);
         if(colour.red()>0.9999f && colour.green()<0.0001f && colour.blue()<0.0001f)
         {
            m_trees.push_back(std::make_pair(x,y));
         }
         else
         {
            m_nonTrees.push_back(std::make_pair(x,y));
         }
      }
   }
   std::cout << "No of trees VS no of non trees : " << m_trees.size() << " " << m_nonTrees.size() << "\n";
   std::string header = i_image + ".header";
   std::ofstream myfile;
   myfile.open(header.c_str());
   if(!myfile)
   {
      std::cout << "File " << header << " not opened\n";
   }else
   {
      myfile << "startXY " << x_start << " " << y_start << "\n";
      myfile << "endXY " << x_end << " " << y_end << "\n";
      myfile.close();
      std::cout << "file : " << header << "saved\n";
   }
}

//-----------------------------------------------------------------------------
void CreateTreeTemplates::addSamples(
        const std::string &i_image,
        bool i_pos
        )
{
   QString fileName(i_image.c_str());
   QImage image = QImage(fileName);

   unsigned int width = image.width();
   unsigned int height = image.height();
   for(unsigned x=0; x<width; ++x)
   {
      for(unsigned int y=0; y<height; ++y)
      {
         QColor colour(image.pixel(QPoint(x,y)));
         qreal r,g,b;

         colour.getRgbF(&r,&g,&b);
         if(colour.red()<0.001f && colour.green()>0.9999f && colour.blue()<0.0001f)
         {
            if(i_pos)
            {
               m_trees.push_back(std::make_pair(x,y));
            }
            else
            {
               m_nonTrees.push_back(std::make_pair(x,y));
            }
         }
      }
   }
   std::cout << "Samples added\n";
}

//-----------------------------------------------------------------------------
// LibSVM training/testing files format:
//-----------------------------------------------------------------------------
// 1 1:value 2:value 3:value ... n:value
// 1 1:value 2:value 3:value ... n:value
// ...
// 1 1:value 2:value 3:value ... n:value
// 0 1:value 2:value 3:value ... n:value
// 0 1:value 2:value 3:value ... n:value
// ...
// 0 1:value 2:value 3:value ... n:value
//-----------------------------------------------------------------------------
void CreateTreeTemplates::exportTemplatesForLibSVM(
        unsigned short i_dimensionX,
        unsigned short i_dimensionY,
        unsigned short i_dimensionZ,
        const std::string &i_template
        )
{
    if(!(i_dimensionX%2 | i_dimensionY%2))
    {
       std::cout<<"Warning:Tree crowns aren't' in the centre of the templates\n";
    }
    std::ofstream fileLibSVM;
    fileLibSVM.open (i_template);

    for(unsigned int i=0; i<m_trees.size(); ++i)
    {
       unsigned int x = m_trees[i].first;
       unsigned int y = m_trees[i].second;
       unsigned int z=m_obj->getNoVoxelsZ()-1;
       // make x,y,z to be the first voxel of the template instead of the centre
       x = x-floor(double(i_dimensionX)/2.0);
       y = y-floor(double(i_dimensionY)/2.0);
       for(; z>0; --z)
       {
          if(m_obj->isInside(x,y,z))
          {
             break;
          }
       }
       if(z< m_obj->getNoVoxelsZ()-1)
       {
          fileLibSVM << "1";
          z = z-1;
          unsigned trainingIndex = 0;
          for(unsigned int dX=0; dX<i_dimensionX ; ++dX)
          {
             for(unsigned int dY=0; dY<i_dimensionY ; ++dY)
             {
                for(unsigned int dZ=0; dZ<i_dimensionZ ; ++dZ)
                {
                   trainingIndex++;
                   fileLibSVM << " "<< trainingIndex << ":" <<m_obj->getIntensity(x+dX,y+dY,z+dZ);
                }
             }
          }
          fileLibSVM << "\n";
       }
       else
       {
          std::cout << "WARNING: A template has been removed due two information missiong\nThe NoOfTemplates number will not match the actual number of templates\n";
       }

    }

    // negative training/testing samples
    for(unsigned int i=0; i<m_nonTrees.size(); ++i)
    {
       unsigned int x = m_nonTrees[i].first;
       unsigned int y = m_nonTrees[i].second;
       unsigned int z=m_obj->getNoVoxelsZ()-1;
       // make x,y,z to be the first voxel of the template instead of the centre
       x = x-floor(double(i_dimensionX)/2.0);
       y = y-floor(double(i_dimensionY)/2.0);
       for(; z>0; --z)
       {
          if(m_obj->isInside(x,y,z))
          {
             break;
          }
       }
       if(z< m_obj->getNoVoxelsZ()-1)
       {
          fileLibSVM << "0";
          z = z-1;
          unsigned trainingIndex = 0;
          for(unsigned int dX=0; dX<i_dimensionX ; ++dX)
          {
             for(unsigned int dY=0; dY<i_dimensionY ; ++dY)
             {
                for(unsigned int dZ=0; dZ<i_dimensionZ ; ++dZ)
                {
                   trainingIndex++;
                   fileLibSVM << " "<< trainingIndex << ":" <<m_obj->getIntensity(x+dX,y+dY,z+dZ) ;
                }
             }
          }
          fileLibSVM << "\n";
       }
       else
       {
          std::cout << "WARNING: A template has been removed due two information missiong\nThe NoOfTemplates number will not match the actual number of templates\n";
       }
    }

    fileLibSVM.close();
}

//-----------------------------------------------------------------------------
void CreateTreeTemplates::exportTemplatesCSV(
        const unsigned short int i_dimensionX,
        const unsigned short int i_dimensionY,
        const unsigned short int i_dimensionZ,
        const std::string &i_template,
        const bool i_AOI
        )
{
    float voxelLength = m_obj->getVoxelLen();
    gmtl::Vec3f minLimits = m_obj->getMinLimits();
    if(!(i_dimensionX%2 | i_dimensionY%2))
    {
       std::cout<<"Warning:Tree crowns aren't' in the centre of the templates\n";
    }
    std::ofstream fileWithTemplates;
    fileWithTemplates.open (i_template);
    unsigned int templatesSize =0;
    for(unsigned int i=0; i<m_trees.size(); ++i)
    {
        unsigned int x = m_trees[i].first;
        unsigned int y = m_trees[i].second;
        unsigned int z=m_obj->getNoVoxelsZ()-1;
        x = x-floor(double(i_dimensionX)/2.0);
        y = y-floor(double(i_dimensionY)/2.0);
        for(; z>0; --z)
        {
           if(m_obj->isInside(x,y,z))
           {
              break;
           }
        }
        if(z< m_obj->getNoVoxelsZ()-1)
        {
           templatesSize++;
        }
    }
    if(m_trees.size()!=m_GPStrees.size())
    {
       std::cerr << "ERROR: the number of the GPS tree positions is not equal to the number of the trees\n";
       return;
    }

    if(!i_AOI)
    {
       fileWithTemplates << "PlotID, Index, x, y, centroid_x, centroid_y, isDead";
    }
    else
    {
       fileWithTemplates << "PlotID, Index, centroid_x, centroid_y";
    }

    for(unsigned int dX=0; dX<i_dimensionX ; ++dX)
    {
       for(unsigned int dY=0; dY<i_dimensionY ; ++dY)
       {
          for(unsigned int dZ=0; dZ<i_dimensionZ ; ++dZ)
          {
             fileWithTemplates << ", V" << dX << "_" << dY << "_" << dZ  ;
          }
       }
    }
    fileWithTemplates <<"\n";
    fileWithTemplates << std::setprecision(3) << std::fixed;

    for(unsigned int i=0; i<m_trees.size(); ++i)
    {
       unsigned int x = m_trees[i].first;
       unsigned int y = m_trees[i].second;
       unsigned int z=m_obj->getNoVoxelsZ()-1;
       // make x,y,z to be the first voxel of the template instead of the centre
       x = x-floor(double(i_dimensionX)/2.0);
       y = y-floor(double(i_dimensionY)/2.0);
       for(; z>0; --z)
       {
          if(m_obj->isInside(x,y,z))
          {
             break;
          }
       }

       if(z< m_obj->getNoVoxelsZ()-1)
       {
          if(!i_AOI)
          {
             fileWithTemplates << m_currentPlot<< ", " << i << ", "<< m_GPStrees[i].first << ", "
                           << m_GPStrees[i].second <<", " <<  minLimits[0]+ float(m_trees[i].first)*voxelLength+voxelLength/2
                           << ", " << minLimits[1]+ float(m_trees[i].second)*voxelLength+voxelLength/2  << ", " << m_isDead[i] << ", ";
          }
          else
          {
              fileWithTemplates <<m_currentPlot<< ", " <<i << ", "<< m_GPStrees[i].first << ", "
                            << m_GPStrees[i].second <<", ";

          }
          z = z-1;
          for(unsigned short int dX=0; dX<i_dimensionX ; ++dX)
          {
             for(unsigned short int dY=0; dY<i_dimensionY ; ++dY)
             {

                for(unsigned short int dZ=0; dZ<i_dimensionZ ; ++dZ)
                {
                   if(!(dX==i_dimensionX-1 && dY==i_dimensionY-1 && dZ==i_dimensionZ-1))
                   {
                      fileWithTemplates << m_obj->getIntensity(x+dX,y+dY,z+dZ) << ", ";
                   }
                   // else
                   // last element and "," should not be added
                }
             }
          }
          fileWithTemplates << m_obj->getIntensity(x+i_dimensionX-1,y+i_dimensionY-1,z+i_dimensionZ-1) << "\n";
       }
       else
       {
          std::cout << "WARNING: A template has been removed due two information missiong\n";
       }
    }
    fileWithTemplates.close();
    std::cout << "Templates in .csv exported\n";
}



//-----------------------------------------------------------------------------
// Templates File Format
//----------------------//
// NoOfTemplates: <No>
// Dimentions: <ie n m k, means that the teplace is an n*m*k template >
// Intensities:
// <intensitity (1,1,1) of template 0> <intensity (2,1,1) of template 0> ... <intensity (n,1,1) of teplate 0>
// <intensitity (1,1,1) of template 1> <intensity (2,1,1) of template 1> ... <intensity (n,1,1) of teplate 1>
// ...
// <intensitity (1,m-1,k-1) of template (No-1)> <intensity (2,m-1,k-1) of template (No-1)> ... <intensity (n,m,k) of teplate (No-1)>
//-----------------------------------------------------------------------------
void CreateTreeTemplates::exportTemplates(
        unsigned short int i_dimensionX,
        unsigned short int i_dimensionY,
        unsigned short int i_dimensionZ,
        const std::string &i_template
        )
{
   if(!(i_dimensionX%2 | i_dimensionY%2))
   {
      std::cout<<"Warning:Tree crowns aren't' in the centre of the templates\n";
   }
   std::ofstream fileWithTemplates;
   fileWithTemplates.open (i_template);

   unsigned int templatesSize =0;
   for(unsigned int i=0; i<m_trees.size(); ++i)
   {
       unsigned int x = m_trees[i].first;
       unsigned int y = m_trees[i].second;
       unsigned int z=m_obj->getNoVoxelsZ()-1;
       x = x-floor(double(i_dimensionX)/2.0);
       y = y-floor(double(i_dimensionY)/2.0);
       for(; z>0; --z)
       {
          if(m_obj->isInside(x,y,z))
          {
             break;
          }
       }
       if(z< m_obj->getNoVoxelsZ()-1)
       {
          templatesSize++;
       }
   }
   fileWithTemplates << "NoOfTemplates: " << templatesSize << "\n";
   fileWithTemplates << "Dimensions: " << i_dimensionX << " "
                     << i_dimensionY << " " << i_dimensionZ << "\n";
   fileWithTemplates << "Intensities: \n";

   for(unsigned int i=0; i<m_trees.size(); ++i)
   {
      unsigned int x = m_trees[i].first;
      unsigned int y = m_trees[i].second;
      unsigned int z=m_obj->getNoVoxelsZ()-1;
      // make x,y,z to be the first voxel of the template instead of the centre
      x = x-floor(double(i_dimensionX)/2.0);
      y = y-floor(double(i_dimensionY)/2.0);
      for(; z>0; --z)
      {
         if(m_obj->isInside(x,y,z))
         {
            break;
         }
      }
      if(z< m_obj->getNoVoxelsZ()-1)
      {
         z = z-1;
         for(unsigned int dX=0; dX<i_dimensionX ; ++dX)
         {
            for(unsigned int dY=0; dY<i_dimensionY ; ++dY)
            {
               for(unsigned int dZ=0; dZ<i_dimensionZ ; ++dZ)
               {
                  fileWithTemplates << m_obj->getIntensity(x+dX,y+dY,z+dZ) << " ";
               }
            }
         }
      }
      else
      {
         std::cout << "WARNING: A template has been removed due two information missiong\nThe NoOfTemplates number will not match the actual number of templates\n";
      }
   }
   fileWithTemplates.close();
   std::cout << "Templates exported\n";
}


//-----------------------------------------------------------------------------
void CreateTreeTemplates::addTemplate(const float i_x, const float i_y, bool i_pos)
{
   gmtl::Vec2i indices = m_obj->getIndices(i_x,i_y);
   if(i_pos)
   {
      m_trees.push_back(std::make_pair(indices[0],indices[1]));
   }
   else
   {
      m_nonTrees.push_back(std::make_pair(indices[0],indices[1]));
   }
}

//-----------------------------------------------------------------------------
CreateTreeTemplates::~CreateTreeTemplates()
{
   if(m_obj!=NULL && m_freeObject)
   {
      delete m_obj;
   }
}


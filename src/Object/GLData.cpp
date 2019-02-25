#include "GLData.h"
#include <iostream>
#include <fstream>
#include "Grid.h"

#include "binfile.h"
#include  "commonfunctions.h"
#include <QtGui/QImage>
#include <math.h>



//-----------------------------------------------------------------------------
GLData::GLData()
{
}


//-----------------------------------------------------------------------------
GLData::GLData(const GLData &i_glData)
{
   setGLData(i_glData);
}

//-----------------------------------------------------------------------------
void GLData::setGLData(const GLData &i_glData)
{
   m_indices.resize(i_glData.m_indices.size());
   for(unsigned int i=0; i<i_glData.m_indices.size(); ++i)
   {
      m_indices[i] = i_glData.m_indices[i];
   }
   m_normals.resize(i_glData.m_normals.size());
   for(unsigned int i=0; i<i_glData.m_normals.size(); ++i)
   {
      m_normals[i] = i_glData.m_normals[i];
   }
   m_vertices.resize(i_glData.m_vertices.size());
   for(unsigned int i=0; i<i_glData.m_vertices.size(); ++i)
   {
      m_vertices[i] = i_glData.m_vertices[i];
   }
   m_UVs.resize(i_glData.m_UVs.size());
   for(unsigned int i=0; i<i_glData.m_UVs.size(); ++i)
   {
      m_UVs[i] = i_glData.m_UVs[i];
   }
}

//-----------------------------------------------------------------------------
void GLData::createUVsBIL(const std::string &i_bilFilename)
{
   if(i_bilFilename=="")
   {
       return;
   }
   try
   {
      bilLib::BinFile file(i_bilFilename);
      std::string map_info = file.FromHeader("map info");

      unsigned int nsamps=bilLib::StringToUINT(file.FromHeader("samples"));
      unsigned int nlines=bilLib::StringToUINT(file.FromHeader("lines"));

      gmtl::Vec2f min(atof(bilLib::GetItemFromString(map_info,3,',').c_str()),
                     atof(bilLib::GetItemFromString(map_info,4,',').c_str())-
                    nlines*atof(bilLib::GetItemFromString(map_info,6,',').c_str()));
      gmtl::Vec2f max(min[0]+nsamps*atof(bilLib::GetItemFromString(map_info,5,',').c_str()),
                    atof(bilLib::GetItemFromString(map_info,4,',').c_str()));
      std::cout << min[0] << " " << min[1] << "\n";
      std::cout << max[0] << " " << max[1] << "\n";
      file.Close();
      m_UVs.resize(m_vertices.size()/3*2+1);
      for(unsigned int i=0; i<m_vertices.size()/3; ++i)
      {
         m_UVs[i*2  ] = (m_vertices[i*3  ]-min[0])/(max[0]-min[0]);
         m_UVs[i*2+1] = (m_vertices[i*3+1]-min[1])/(max[1]-min[1]);
      }
   }
   catch(bilLib::BinaryReader::BRexception e)
   {
      std::cout<<e.what()<<std::endl;
      std::cout<<e.info<<std::endl;
   }
   std::cout << "uvs created\n";
}

//-----------------------------------------------------------------------------
void GLData::createUVsIGM(const std::string &i_igmFile)
{
   std::cout << "Creating UVs from IGM file\n";
   if(i_igmFile=="")
   {
      return;
   }
   m_UVs.resize(m_vertices.size()/3*2+1);
   Grid *grid = new Grid(i_igmFile,30);
   for(unsigned int i=0; i<m_vertices.size()/3; ++i)
   {
      const gmtl::Vec2f nextPixelPos =
               grid->getPixelPositionScaled0_1(m_vertices[i*3],m_vertices[i*3+1]);
      m_UVs[i*2  ] = nextPixelPos[0];
      m_UVs[i*2+1] = nextPixelPos[1];
   }
   delete grid;
}

//-----------------------------------------------------------------------------
unsigned int GLData::addVertex(const gmtl::Vec3f &i_vertex)
{
   m_vertices.push_back(i_vertex[0]);
   m_vertices.push_back(i_vertex[1]);
   m_vertices.push_back(i_vertex[2]);

   return (m_vertices.size()/3-1);
}

//-----------------------------------------------------------------------------
gmtl::Vec3f GLData::getVertex(unsigned int i_index)
{
    return gmtl::Vec3f(m_vertices[i_index/3  ],
                     m_vertices[i_index/3+1],
                     m_vertices[i_index/3+2]);
}

//-----------------------------------------------------------------------------
void GLData::
exportHyperToImage(
        const std::string &i_fName,
        std::string i_name,
        const std::vector<unsigned short> &i_bands
        )
{
   const std::string extension = ".png";
   if (i_name.find(extension)!=std::string::npos)
   {
     std::size_t pos = i_name.find_last_of(extension);
     if (pos!=i_name.size()-1)
     {
       i_name+=extension;
     }
   }
   else
   {
     i_name+=extension;
   }

   try
   {
      bilLib::BinFile file(i_fName);
      unsigned char dataType = bilLib::StringToUINT(
                  file.FromHeader("data type"));
      unsigned int nsamps=bilLib::StringToUINT(file.FromHeader("samples"));
      unsigned int nlines=bilLib::StringToUINT(file.FromHeader("lines"));
      unsigned int nbands=bilLib::StringToUINT(file.FromHeader("bands"));

      unsigned int gLsamples = nsamps+((4-(nsamps%4))%4);

      std::cout << "bil file lines samples " << nlines << " " << nsamps << " " << nbands <<"\n";

      if (dataType == 4)// 32-bit doubleing point
      {
         double *data0 = new double[nlines*nsamps];
         file.Readband((char *)data0,i_bands[0]);
         double *data1 = new double[nlines*nsamps];
         file.Readband((char *)data1,i_bands[1]);
         double *data2 = new double[nlines*nsamps];
         file.Readband((char *)data2,i_bands[2]);
         QImage *img = new QImage(gLsamples, nlines,QImage::Format_RGB16);

         unsigned int k=0;
         while(data1[k]==0 && k!=nsamps*nlines-1)
         {
            k++;
         }
         double max1 = data1[k];
         double min1 = data1[k];
         for(unsigned int i=0;i<nlines*nsamps;++i)
         {
            if(max1<data1[i])
            {
               max1 = data1[i];
            }else if (min1>data1[i] && data1[i]!=0)
            {
               min1 = data1[i];
            }
         }
         k=0;
         while(data2[k]==0 && k!=nsamps*nlines-1)
         {
            k++;
         }
         double max2 = data2[k];
         double min2 = data2[k];
         for(unsigned int i=0;i<nlines*nsamps;++i)
         {
            if(max2<data2[i])
            {
               max2 = data2[i];
            }else if (min2>data1[i] && data2[i]!=0)
            {
               min2 = data2[i];
            }
         }
         k=0;
         while(data0[k]==0 && k!=nsamps*nlines-1)
         {
            k++;
         }
         double max0 = data0[k];
         double min0 = data0[k];
         for(unsigned int i=0;i<nlines*nsamps;++i)
         {
            if(max0<data0[i])
            {
               max0 = data0[i];
            }else if (min1>data0[i] && data0[i]!=0)
            {
               min0 = data0[i];
            }
         }
         img->fill(qRgb(0,0,0));
         for(unsigned int x=0; x<nsamps; ++x)
         {
            for(unsigned int y=0; y<nlines; ++y)
            {
               img->setPixel(x,y,qRgb(floor(double(data0[y*nsamps+x]-min0)/(max0-min0)*255.0),
                                      floor(double(data1[y*nsamps+x]-min1)/(max1-min1)*255.0),
                                      floor(double(data2[y*nsamps+x]-min2)/(max2-min2)*255.0)));
            }
         }
         img->save(i_name.c_str());
          delete [] data0;
          delete [] data1;
          delete [] data2;
         delete img;
         std::cout << "IMG4\n";

      } else if (dataType == 12)// 16-bit unsigned integer (short)
      {
         unsigned short int *data0 = new unsigned short int[nlines*nsamps];
         file.Readband((char *)data0,i_bands[0]);
         unsigned short int *data1 = new unsigned short int[nlines*nsamps];
         file.Readband((char *)data1,i_bands[1]);
         unsigned short int *data2 = new unsigned short int[nlines*nsamps];
         file.Readband((char *)data2,i_bands[2]);
         QImage *img = new QImage(gLsamples, nlines,QImage::Format_RGB16);
         unsigned int k=0;
         while(data1[k]==0 && k!=nsamps*nlines-1)
         {
            k++;
         }
         unsigned short int max1 = data1[k];
         unsigned short int min1 = data1[k];
         for(unsigned int i=0;i<nlines*nsamps;++i)
         {
            if(max1<data1[i])
            {
               max1 = data1[i];
            }else if (min1>data1[i] && data1[i]!=0)
            {
               min1 = data1[i];
            }
         }
         k=0;
         while(data2[k]==0 && k!=nsamps*nlines-1)
         {
            k++;
         }
         unsigned short int max2 = data2[k];
         unsigned short int min2 = data2[k];
         for(unsigned int i=0;i<nlines*nsamps;++i)
         {
            if(max2<data2[i])
            {
               max2 = data2[i];
            }else if (min2>data1[i] && data2[i]!=0)
            {
               min2 = data2[i];
            }
         }
         k=0;
         while(data0[k]==0 && k!=nsamps*nlines-1)
         {
            k++;
         }
         unsigned short int max0 = data0[k];
         unsigned short int min0 = data0[k];
         for(unsigned int i=0;i<nlines*nsamps;++i)
         {
            if(max0<data0[i])
            {
               max0 = data0[i];
            }else if (min1>data0[i] && data0[i]!=0)
            {
               min0 = data0[i];
            }
         }
         img->fill(qRgb(0,0,0));
         for(unsigned int x=0; x<nsamps; ++x)
         {
            for(unsigned int y=0; y<nlines; ++y)
            {
               img->setPixel(x,y,qRgb(std::max(0.0,floor(double(data0[y*nsamps+x]-min0)/(max0-min0)*255.0)),
                                      std::max(0.0,floor(double(data1[y*nsamps+x]-min1)/(max1-min1)*255.0)),
                                      std::max(0.0,floor(double(data2[y*nsamps+x]-min2)/(max2-min2)*255.0))));
            }
         }
         img->save(i_name.c_str());
         delete img;
         delete [] data0;
         delete [] data1;
         delete [] data2;
         std::cout << "IMG12\n";
      } else
      {
         std::cout << "bil data type not accepted\n";
         return;
      }



      file.Close();
   }
   catch(bilLib::BinaryReader::BRexception e)
   {
      std::cout<<e.what()<<std::endl;
      std::cout<<e.info<<std::endl;
   }
}

//-----------------------------------------------------------------------------
void GLData::exportToObj(
        std::string i_name,
        const std::string i_igmFileName,
        const std::string i_bilFileName,
        const std::vector<unsigned short int> i_bands)
{
   std::cout << "Bil File: " << i_bilFileName << "\n";
   if(i_igmFileName!="")
   {
      createUVsIGM(i_igmFileName);
   }
   std::string mtlName("");
   std::cout << i_name << "\n";
   if(i_bilFileName!="")
   {
      std::cout << "Before exporting Texture Image\n";
      exportHyperToImage(i_bilFileName,i_name,i_bands);
      std::cout << "After exporting Texture Image\n";
      std::ofstream mtlFile;
      mtlName=i_name+".mtl";
      std::cout << mtlName << "\n";
      mtlFile.open(mtlName.c_str());
      mtlFile << "newmtl material0\nKa 1.000000 1.000000 1.000000\n"
              << "Kd 1.000000 1.000000 1.000000\nKs 0.000000 0.000000 0.000000"
              << "\nTr 1.000000\nillum 1\nNs 0.000000\nmap_Kd "
              << i_name << ".png";
      mtlFile.close();
   }
   std::cout << "No of Vertices: " << m_vertices.size()/3 << "\n";
   //  find offset that will move object to the centre
   // for animation software packages if the object is very far away from the
   // centre then it will not be visible

   gmtl::Vec3f offset;
//   if(m_vertices.size()>=3)
//   {
//      offset[0] = (m_vertices[0]+m_vertices[m_vertices.size()-3]) / 2;
//      offset[1] = (m_vertices[1]+m_vertices[m_vertices.size()-2]) / 2;
//      offset[2] = (m_vertices[2]+m_vertices[m_vertices.size()-1]) / 2;
//   }


   std::ofstream myfile;
   const std::string extension = ".obj";
   if (i_name.find(extension)!=std::string::npos)
   {
     std::size_t pos = i_name.find_last_of(extension);
     if (pos!=i_name.size()-1)
     {
       i_name+=extension;
     }
   }
   else
   {
     i_name+=extension;
   }
   myfile.open(i_name.c_str());
     if (myfile.is_open())
     {
       int vsize = m_vertices.size();
       int nsize = m_normals.size();
       int isize = m_indices.size();
       if(mtlName!="")
       {
          myfile << "mtllib " << mtlName <<"\n\n";
       }
       for(int i=0; i<vsize; i+=3)
       {
         myfile << "v "<< m_vertices[i  ] - offset[0]
                << " " << m_vertices[i+1] - offset[1]
                << " " << m_vertices[i+2] - offset[2] << "\n";
       }
       myfile <<"\n\n";

//       // m_classUVs are UVs used for validation datasets
//       if(m_vertices.size()/3*2+1 == m_classUVs.size())
//       {
//          // UVs has been calculated and better be saved into the file
//          for(unsigned int i=0; i<m_classUVs.size()-1; i+=2)
//          {
//             myfile << "vt " << m_classUVs[i] << " " << m_classUVs[i+1] << "\n";
//          }
//       }

       // uvs used for texturing
       if(m_vertices.size()/3*2+1 == m_UVs.size())
       {
         // UVs has been calculated and better be saved into the file
          for(unsigned int i=0; i<m_UVs.size()-1; i+=2)
          {
             myfile << "vt " << m_UVs[i] << " " << m_UVs[i+1] << "\n";
          }
       }
       for(int i=0; i<nsize; i+=3)
       {
         myfile <<"vn "<< m_normals[i]<<" "<< m_normals[i+1]
                << " "<< m_normals[i+2] << "\n";
       }
       myfile <<"\n\n";
       for(int i=0; i<isize; i+=3)
       {
         int x = (int)m_indices[i];
         int y = (int)m_indices[i+1];
         int z = (int)m_indices[i+2];
         if(x!=y && y!=z && x!=z)
         {
           x++;y++;z++;
           if(m_UVs.size()==0)
           {
              myfile << "f "<< y <<
                        "//" << y <<
                        " "<< x <<
                   "//" << x <<
                   " "  << z <<
                   "//" << z <<
                   "\n";
           }
           else
           {
              myfile << "f "<< y <<
                        "/"<< y <<"/" << y <<
                        " " << x <<
                   "/"<< x <<"/" << x <<
                   " " << z <<
                   "/"<< z <<"/" << z <<
                   "\n";
           }
         }
       }
       myfile.close();
     }
     std::cout << "File saved\n";
}



//-----------------------------------------------------------------------------
GLData::~GLData()
{
}

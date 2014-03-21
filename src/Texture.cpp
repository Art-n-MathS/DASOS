/*
  Copyright (C) 2009 Jon Macey

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//----------------------------------------------------------------------------------------------------------------------
/// @file Texture.cpp
/// @brief implementation files for Texture class
//----------------------------------------------------------------------------------------------------------------------
#include "Texture.h"
#include <ngl/NGLassert.h>
#include <QtGui/QImage>
#include <bilLib/binfile.h>




//----------------------------------------------------------------------------------------------------------------------
bool Texture::loadImage(
                        const std::string &i_fName,
        const std::vector<unsigned short int> &i_bands
                       )
{
   try
   {
      float *data1 = 0;
      bilLib::BinFile file(i_fName);
      unsigned int nsamps=bilLib::StringToUINT(file.FromHeader("samples"));
      unsigned int nlines=bilLib::StringToUINT(file.FromHeader("lines"));
      unsigned int nbands=bilLib::StringToUINT(file.FromHeader("bands"));

      unsigned int gLsamples = nsamps+((4-(nsamps%4))%4);
      //Print out file dimensions
      std::cout<<"Dimensions: "<<nsamps<<" "<<nlines<<" "<<nbands<<std::endl;
      std::cout<< " Opengl samples: " << nsamps+((4-(nsamps%4))%4) << "\n";
      m_bpp=3;
      m_format = GL_RGB;
      m_hasAlpha=false;
      m_height = nlines;
      m_width = gLsamples;
      if(m_data!=0)
      {
         delete m_data;
      }
      m_data = new char [nlines*gLsamples*m_bpp];
      for(unsigned int b=0; b<3; ++b)
      {
         data1 = new float[nlines*nsamps];
         file.Readband((char *)data1,i_bands[b]);
         unsigned int k=0;
         while(data1[k]==0 && k!=m_width*m_height-1)
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
         if(m_data==0)
         {
             std::cout << "Error while allocating memory!\n";
             return EXIT_FAILURE;
         }
         unsigned int index  = b;
         for (int y=nlines-1; y>=0; --y)
         {
            unsigned int x=0;
            for(; x<nsamps; ++x)
            {
               if(data1[y*nsamps+x]>=0.0000001f)
               {
                  m_data[index]=floor(double(data1[y*nsamps+x]-min1)/(max1-min1)*255.0);
                  index+=3;
               }
               else
               {
                  m_data[index]= 0;
                  index+=3;
               }
            }
            for(;x<gLsamples;++x)
            {
               m_data[index]= 0;
               index+=3;
            }
         }
         if(data1!=0)
         {
             delete []data1;
         }
      }
      file.Close();
   }
   catch(bilLib::BinaryReader::BRexception e)
   {
      std::cout<<e.what()<<std::endl;
      std::cout<<e.info<<std::endl;
   }
   return true;
}

//----------------------------------------------------------------------------------------------------------------------
Texture::Texture():m_data(0)
{
    m_width=0;
    m_height=0;
    m_data=0;
    m_multiTextureID=0;
    m_hasAlpha=false;
}

//----------------------------------------------------------------------------------------------------------------------
Texture::Texture(
                 const std::string &i_fName
                ):m_data(0)
{
    m_width=0;
    m_height=0;
    m_data=NULL;
    m_multiTextureID=0;
    std::vector<unsigned short int> bands(3);
    bands[0]=137;
    bands[1]=75;
    bands[2]=38;
    loadImage(i_fName,bands);
}


//----------------------------------------------------------------------------------------------------------------------
Texture::~Texture()
{
  if(m_data !=0)
  {
    delete [] m_data;
  }
}

//----------------------------------------------------------------------------------------------------------------------
double Texture::getFromXY(
                          const GLuint _x,
                          const GLuint _y
                         ) const
{
// make sure were in the image range
    NGL_ASSERT(_x<=m_width && _y<=m_height);
    if (m_data !=0)
    {
        return m_data[_x*4+((_y)*m_width*4)];
    }
    else
    {
        return 0;
    }
}
//----------------------------------------------------------------------------------------------------------------------

GLuint Texture::setTextureGL() const
{
  GLuint textureName;
  glGenTextures(1,&textureName);
  glActiveTexture(GL_TEXTURE0+m_multiTextureID);
  glBindTexture(GL_TEXTURE_2D,textureName);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D,0,m_format,m_width,m_height,0,m_format,GL_UNSIGNED_BYTE,m_data);
  std::cout<<"texture GL set "<<textureName<<"\n";
  glGenerateMipmap(GL_TEXTURE_2D);
  return textureName;
}
//----------------------------------------------------------------------------------------------------------------------

void Texture::setMultiTexture(
                              const GLint _id
                             )
{
 m_multiTextureID=_id;
}

//----------------------------------------------------------------------------------------------------------------------
ngl::Colour Texture::getColourFromXY(
                                    const GLuint _x,
                                    const GLuint _y
                                   ) const
{
// make sure were in the image range
    NGL_ASSERT(_x<=m_width && _y<=m_height);
    if (m_data !=0)
    {
        int Offset=_x*4+((_y)*m_width*4);
        return ngl::Colour(m_data[Offset],m_data[Offset+1],m_data[Offset+2],m_data[Offset+3]);
    }
    else
    {
        return ngl::Colour(0,0,0,0);
    }
}

//----------------------------------------------------------------------------------------------------------------------
ngl::Colour Texture::getColour(
                                const ngl::Real c_uvX,
                                const ngl::Real c_uvY
                              ) const
{

  GLuint xx = c_uvX * (m_width-1);
  GLuint yy = c_uvY * (m_height-1);

  NGL_ASSERT(xx<m_width && yy<m_height);

  if(m_data!=0)
  {
    int offset = xx * m_bpp + (yy * m_width * m_bpp );
    if(m_bpp == 4)
    {
      return ngl::Colour(m_data[offset],m_data[offset+1],m_data[offset+2],m_data[offset+3]);
    }
    else
    {
      return ngl::Colour(m_data[offset],m_data[offset+1],m_data[offset+2],1.0);
    }
  }
  else
  {
    return ngl::Colour(0,0,0,0);
  }
}



//----------------------------------------------------------------------------------------------------------------------


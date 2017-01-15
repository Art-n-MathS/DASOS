#include "SignatureDifferneceMap.h"
#include "Grid.h"
#include "binfile.h"
#include "commonfunctions.h"
#include <algorithm>
#include <math.h>

//--------------------------------------------------------------------------
SignatureDifferneceMap::SignatureDifferneceMap(const std::string &i_name,
        Volume *i_obj,
        const std::string &i_bilFileName,
        const std::string &i_IGMfileName,
        const std::string &i_fodisFileName,
        const std::string &i_signatureFileName
        , const std::string &I_signatureType):
    Map(i_name,i_obj),
    m_IGMfileName(i_IGMfileName),
    m_bilFileName(i_bilFileName),
    m_fodisFileName(i_fodisFileName),
    m_spectralSingature(i_signatureFileName,I_signatureType)
{
    std::cout << i_signatureFileName << "\n";
}


//--------------------------------------------------------------------------
void SignatureDifferneceMap::createMap()
{
    try
    {
       if(m_bilFileName=="" || m_IGMfileName=="")
       {
          std::cout<<"WARNING: hyperspectral files have not been loaded\n";
          return;
       }
       std::cout << "m_bilFileName = " << m_bilFileName << "\n";
       bilLib::BinFile file(m_bilFileName);
       unsigned int nsamps=bilLib::StringToUINT(file.FromHeader("samples"));
       unsigned int nlines=bilLib::StringToUINT(file.FromHeader("lines"));
       unsigned int nbands=bilLib::StringToUINT(file.FromHeader("bands"));


       bilLib::BinFile fileFodis(m_fodisFileName);
//       unsigned int nsampsF=bilLib::StringToUINT(fileFodis.FromHeader("samples"));
//       unsigned int nlinesF=bilLib::StringToUINT(fileFodis.FromHeader("lines"));
       unsigned int nbandsF=bilLib::StringToUINT(fileFodis.FromHeader("bands"));

       Grid *grid = new Grid(m_IGMfileName,m_object->getVoxelLen()*1.8,
                             m_object->getMinLimits());

       std::vector<float> reflectances(nbands*4);
       std::vector<float> wavelengths(nbands);
       std::string wavelengthsStr = file.FromHeader("wavelength");

       std::vector<short unsigned int *> fotides(nbandsF);
       for(unsigned int i=0; i<fotides.size(); ++i)
       {
          fotides[i]= new short unsigned int [nlines];
          fileFodis.Readband((char *)fotides[i],i);
       }

       std::string temp ("temp");
       unsigned int w=1;
       while(temp!="" && (w-1)<nbands)
       {
          temp = bilLib::GetItemFromString(wavelengthsStr,w,';');
          wavelengths[w-1] = atof(temp.c_str());
          w++;
       }




       reflectances.resize(nbands);
       std::fill(reflectances.begin(),reflectances.end(),0.0f);
       unsigned int *pixPos = NULL;
       const float vl(m_object->getVoxelLen());
       const gmtl::Vec3f &mins = m_object->getMinLimits();

       for(unsigned s=0; s<m_object->getNoVoxelsX(); ++s)
       {
          for(unsigned int l=0; l<m_object->getNoVoxelsY(); ++l)
          {
             const gmtl::Vec2f point(mins[0]+vl/2+ vl*s,mins[1]+vl/2+vl*l) ;
             const unsigned int noOfPix =
                      grid->pixIndicesOfSquare(point,&pixPos);

             for(unsigned int b=0; b<nbands; ++b)
             {
                reflectances[b]=0.0f;
                for(unsigned int i=0; i<noOfPix; ++i)
                {
                   unsigned int f = floor(pixPos[i]/nsamps);
                   reflectances[b]+=(file.ReadCell(b,floor(pixPos[i]/nsamps),pixPos[i]%nsamps)
                                     /double(fotides[b][f]));
                }
                reflectances[b]/=(double(noOfPix)/100.0f);
             }
             m_mapValues[getIndex(s,l)]=m_spectralSingature.percentageError
                     (wavelengths,reflectances);
          }
       }
       std::cout << "close file\n";
       file.Close();
       fileFodis.Close();
       delete grid;

       for(unsigned int i=0; i<fotides.size(); ++i)
       {
          delete [] fotides[i];
       }

    }
    catch(bilLib::BinaryReader::BRexception e)
    {
       std::cout<<e.what()<<std::endl;
       std::cout<<e.info<<std::endl;
    }
}


//--------------------------------------------------------------------------
SignatureDifferneceMap::~SignatureDifferneceMap()
{}


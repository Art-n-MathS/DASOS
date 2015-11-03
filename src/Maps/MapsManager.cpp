#include "MapsManager.h"
#include "DensityMap.h"
#include "NonEmptyVoxelsMap.h"
#include "ThicknessMap.h"
#include "HyperMap.h"
#include "HyperMean.h"
#include "FirstPatch.h"
#include "LastPatch.h"
#include "HyperStandardDeviation.h"
#include "NDVI.h"
#include "SignatureDifferneceMap.h"
#include "TerrainModel.h"
#include "HeightMap.h"
#include "AverageHeightDifference.h"
#include "IntensityMap.h"
#include "IntensityMax.h"
#include <map>
#include <algorithm>

//-----------------------------------------------------------------------------
MapsManager::MapsManager():m_map(0)
{
    m_types["NON-EMPTY_VOXELS"] = 1;
    m_types["DENSITY"]   = 2;
    m_types["THICKNESS"]    = 3;
    m_types["HYPERSPECTRAL"] = 4;
    m_types["HYPERSPECTRAL_MEAN"] = 5;
    m_types["LOWEST_RETURN"] = 6;
    m_types["SPECTRAL_SIGNATURE"] = 7;
//    m_types["HYPERSPECTRAL_STD"] = 8;
    m_types["FIRST_PATCH"] = 9;
    m_types["NDVI"] = 10;
    m_types["LAST_PATCH"] = 11;
    m_types["HEIGHT"] = 12;
    m_types["AVERAGE_HEIGHT_DIFFERENCE"] = 13;
    m_types["TREE_CROWNS"] = 14;
    m_types["TREE_CROWNS_2TEMPLATES"] = 15;
    m_types["INTENSITY"] = 16;
    m_types["INTENSITY_MAX"] = 17;
    m_types["PLOT_FIELD_PLOT"] = 18;

}


//-----------------------------------------------------------------------------
void MapsManager::createMap(
        mapInfo *m_infoOfMap
        )
{
   if (m_map!=0)
   {
      delete m_map;
      m_map=0;
   }
   // convert i_type to upper case letters
   std::string s(m_infoOfMap->type);
   std::transform(s.begin(), s.end(), s.begin(), toupper);
   // create a map with the given type
   switch (m_types[s])
   {
   case 1:
      std::cout << "Non empty voxels map\n";
      m_map = new NonEmptyVoxelsMap(m_infoOfMap->name,m_infoOfMap->obj);
      break;
   case 2:
      std::cout << "Density map\n";
      m_map = new DensityMap(m_infoOfMap->name,m_infoOfMap->obj);
      break;
   case 3:
      std::cout << "Thickness mapn\n";
      m_map = new ThicknessMap(m_infoOfMap->name,m_infoOfMap->obj);
      break;
   case 4:
      std::cout << "Hyperspectral Map\n";
      m_map = new HyperMap(m_infoOfMap->name,m_infoOfMap->obj,m_infoOfMap->band,
                           m_infoOfMap->bilFileName,m_infoOfMap->IGMfileName);
      break;
   case 5:
      std::cout << "Hyperspectral Mean Map\n";
      m_map = new HyperMean(m_infoOfMap->name,m_infoOfMap->obj,
                            m_infoOfMap->bilFileName,m_infoOfMap->IGMfileName);
      break;
   case 6:
      std::cout << "Terrain Model\n";
      m_map = new TerrainModel(m_infoOfMap->name,m_infoOfMap->obj);
      break;
   case 7:
       std::cout << "Spectral Signature\n";
       m_map = new SignatureDifferneceMap(
                   m_infoOfMap->name,
                   m_infoOfMap->obj,
                   m_infoOfMap->bilFileName,
                   m_infoOfMap->IGMfileName,
                   m_infoOfMap->fodisFileName,
                   m_infoOfMap->spectralSignature,
                   m_infoOfMap->spectralSignatureType);
       break;
   case 8:
//       std::cout << "Hyperspectral Standard Deviations\n";
//       m_map = new HyperStandardDeviation
//               (m_infoOfMap->name,m_infoOfMap->obj,
//                m_infoOfMap->bilFileName,m_infoOfMap->IGMfileName,
//                m_infoOfMap->fodisFileName);
       break;
   case 9:
       std::cout << "Length of first continues patch of non empty voxels\n";
       m_map = new FirstPatch(m_infoOfMap->name,m_infoOfMap->obj);
       break;
   case 10:
      std::cout << "NDVI\n";
      m_map = new NDVI(m_infoOfMap->name,m_infoOfMap->obj,
                       m_infoOfMap->bilFileName,m_infoOfMap->IGMfileName,
                       m_infoOfMap->fodisFileName);
      break;
   case 11:
       std::cout << "Length of last continues patch of non empty voxels\n";
       m_map = new LastPatch(m_infoOfMap->name,m_infoOfMap->obj);
       break;
   case 12:
       std::cout << "Height Map\n";
       m_map = new HeightMap(m_infoOfMap->name,m_infoOfMap->obj);
       break;
   case 13:
       std::cout << "Average Height Difference Map\n";
       m_map = new AverageHeightDifference(m_infoOfMap->name,m_infoOfMap->obj);
       break;
//   case 14:
//       std::cout << "Tree Crowns Detection\n";
//       m_map = new TreeCrowns(m_infoOfMap->name,
//                              m_infoOfMap->obj,
//                              m_infoOfMap->templatesPosFileName);
//       break;
//   case 15:
//       std::cout<<"Tree Crowns Detection with positive & negative templates\n";
//       m_map = new TreeCrownsWith2Templates(m_infoOfMap->name,
//                                            m_infoOfMap->obj,
//                                            m_infoOfMap->templatesPosFileName,
//                                            m_infoOfMap->templatesNegFileName);
//       break;
   case 16:
       std::cout << "Intensity Average Map\n";
       m_map = new IntensityMap(m_infoOfMap->name,m_infoOfMap->obj);
       break;
   case 17:
       std::cout << "Intensity Max Map\n";
       m_map = new IntensityMax(m_infoOfMap->name,m_infoOfMap->obj);
       break;
//   case 18:
//       std::cout << "Plotting Field plot on a Height map\n";
//       m_map = new FieldPlotOnHeightMap(m_infoOfMap->name,m_infoOfMap->obj,
//                                        m_infoOfMap->fieldPlot);
//       break;
   default:
      std::cout << std::string (s) << " is not a valid type of map";
      break;
   }
   // create and save the map
   if(m_map!=0)
   {
      m_map->createAndSave(m_infoOfMap->thres,m_infoOfMap->samp);
      delete m_map;
      m_map=0;
   }
}


//-----------------------------------------------------------------------------
MapsManager::~MapsManager()
{
   if(m_map!=0)
   {
      delete m_map;
   }
}


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
#include "TreeCrowns.h"
#include "TreeCrownsWith2Templates.h"
#include "IntensityMap.h"
#include "IntensityMax.h"
#include "FieldPlotOnHeightMap.h"
#include "HeightLevels.h"
#include <map>
#include <algorithm>

//-----------------------------------------------------------------------------
MapsManager::MapsManager():m_map(0),
    m_FWMetrics({"NON-EMPTY_VOXELS",
                 "DENSITY",
                 "THICKNESS",
                 "LOWEST_RETURN",
                 "HEIGHT",
                 "LAST_PATCH",
                 "FIRST_PATCH",
                 "AVERAGE_HEIGHT_DIFFERENCE",
                 "INTENSITY_AVG",
                 "INTENSITY_MAX",
                 "HEIGHT_LEVELS"
                })
{
   // The types should aggree with the fw metrics list
   m_types =
   {
      {"NON-EMPTY_VOXELS",1},
      {"DENSITY",2},
      {"THICKNESS",3},
      {"HYPERSPECTRAL",4},
      {"HYPERSPECTRAL_MEAN",5},
      {"LOWEST_RETURN",6},
      {"SPECTRAL_SIGNATURE",7},
//      {"HYPERSPECTRAL_STD",8},
      {"FIRST_PATCH",9},
      {"NDVI",10},
      {"LAST_PATCH",11},
      {"HEIGHT",12},
      {"AVERAGE_HEIGHT_DIFFERENCE",13},
      {"TREE_CROWNS",14},
      {"TREE_CROWNS_2TEMPLATES",15},
      {"INTENSITY_AVG",16},
      {"INTENSITY_MAX",17},
      {"FIELDPLOT",18},
      {"FIELDPLOT",18},
      {"HEIGHT_LEVELS",19}

   };
}

//-----------------------------------------------------------------------------
const std::vector<std::string> &MapsManager::getNamesOfFWMetrics()const
{
   return m_FWMetrics;
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
      std::cout << "Thickness map\n";
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
   case 14:
       std::cout << "Tree Crowns Detection\n";
       m_map = new TreeCrowns(m_infoOfMap->name,
                              m_infoOfMap->obj,
                              m_infoOfMap->templatesPosFileName);
       break;
   case 15:
       std::cout<<"Tree Crowns Detection with positive & negative templates\n";
       m_map = new TreeCrownsWith2Templates(m_infoOfMap->name,
                                            m_infoOfMap->obj,
                                            m_infoOfMap->templatesPosFileName,
                                            m_infoOfMap->templatesNegFileName);
       break;
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
//                                        m_infoOfMap->fieldplot);
//       break;
   case 19:
       std::cout << "Height Levels\n";
       m_map = new HeightLevels(m_infoOfMap->name,m_infoOfMap->obj);
       break;
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


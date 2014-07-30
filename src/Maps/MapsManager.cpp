#include "MapsManager.h"
#include "DensityMap.h"
#include "NonEmptyVoxelsMap.h"
#include "ThicknessMap.h"
#include "HyperMap.h"
#include "HyperMean.h"
#include <map>


//-----------------------------------------------------------------------------
MapsManager::MapsManager():m_map(0)
{
    m_types["NON-EMPTY VOXELS"] = 1;
    m_types["DENSITY"]   = 2;
    m_types["THICKNESS"]    = 3;
    m_types["HYPERSPECTRAL"] = 4;
    m_types["HYPERSPECTRAL MEAN"] = 5;
}


//-----------------------------------------------------------------------------
void MapsManager::createMap(
        mapInfo *m_infoOfMap
        )
{
   std::cout << m_infoOfMap->name << " " << m_infoOfMap->type << " " << m_infoOfMap->thres << "\n";
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
   default:
      std::cout << m_infoOfMap->type << " is not a valid type of map";
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


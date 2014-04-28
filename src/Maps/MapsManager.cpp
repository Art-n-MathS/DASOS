#include "MapsManager.h"
#include "DensityMap.h"
#include "NonEmptyVoxelsMap.h"
#include "ThicknessMap.h"
#include "HyperMap.h"
#include <map>


//-----------------------------------------------------------------------------
MapsManager::MapsManager():m_map(0)
{
    m_types["NON-EMPTY VOXELS"] = 1;
    m_types["DENSITY"]   = 2;
    m_types["THICKNESS"]    = 3;
    m_types["HYPERSPECTRAL"] = 4;
}


//-----------------------------------------------------------------------------
void MapsManager::createMap(
        const std::string &i_type,
        const std::string &i_name,
        Object *i_obj,
        const unsigned int &i_band,
        const std::string &i_bilFileName,
        const std::string &i_IGMfileName,
        const unsigned int i_thres,
        const unsigned int i_samp
        )
{
   if (m_map!=0)
   {
      delete m_map;
      m_map=0;
   }
   // convert i_type to upper case letters
   std::string s(i_type);
   std::transform(s.begin(), s.end(), s.begin(), toupper);
   // create a map with the given type
   switch (m_types[s])
   {
   case 1:
      std::cout << "Non empty voxels map\n";
      m_map = new NonEmptyVoxelsMap(i_name,i_obj);
      break;
   case 2:
      std::cout << "Density map\n";
      m_map = new DensityMap(i_name,i_obj);
      break;
   case 3:
      std::cout << "Thickness mapn\n";
      m_map = new ThicknessMap(i_name,i_obj);
      break;
   case 4:
      std::cout << "Hyperspectral Map\n";
      m_map = new HyperMap(i_name,i_obj,i_band,i_bilFileName,i_IGMfileName);
      break;
   default:
      std::cout << i_type << " is not a valid type of map";
      break;
   }
   // create and save the map
   if(m_map!=0)
   {
      m_map->createAndSave(i_thres,i_samp);
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


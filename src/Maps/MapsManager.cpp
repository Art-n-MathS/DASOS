#include "MapsManager.h"
#include "DensityMap.h"
#include "NonEmptyVoxelsMap.h"
#include "ThicknessMap.h"
#include <map>


//-----------------------------------------------------------------------------
MapsManager::MapsManager():m_map(0)
{
    m_types["NON-EMPTY VOXELS"] = 1;
    m_types["DENSITY"]   = 2;
    m_types["Thickness"]    = 3;
}


//-----------------------------------------------------------------------------
void MapsManager::createMap(
        const std::string &i_type,
        const std::string &i_name,
        Object *i_obj
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
      std::cout << "thickness map\n";
      m_map = new NonEmptyVoxelsMap(i_name,i_obj);
      break;
   case 2:
       std::cout << "density map\n";
      m_map = new DensityMap(i_name,i_obj);
      break;
   case 3:
       m_map = new ThicknessMap(i_name,i_obj);
      break;
   default:
       std::cout << i_type << " is not a valid type of map";
      break;
   }
   // create and save the map
   if(m_map!=0)
   {
      m_map->createAndSave();
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


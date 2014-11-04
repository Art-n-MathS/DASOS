#include "Manager.h"
#include "MarchingCubes.h"
#include "MCwithIntegralImages.h"
#include "Histogram.h"
#include <iostream>

//-----------------------------------------------------------------------------
Manager::Manager()
{

}

//-----------------------------------------------------------------------------
GLData *Manager::getPolygonisedObject(
        Object *i_obj,
        unsigned int i_x,
        bool i_useIntegralData
        )
{
   if(i_obj!=0)
   {
      if(i_useIntegralData)
      {
         std::cout << "calling Marching Cubes with Integral Volume\n";
         MCwithIntegralImages mc(i_obj,i_x);
         return mc.createPolygonisedObject();
      }
      else
      {
         std::cout << "calling Marching Cubes\n";
         MarchingCubes mc(i_obj,i_x);
         return mc.createPolygonisedObject();
      }
   }
   // else return an empty object
   return new GLData;
}

//-----------------------------------------------------------------------------
void Manager::createHistogram(
        Object *i_obj,
        const std::string &i_name,
        unsigned int i_numOfIntervals
        )
{
   if(i_obj!=0)
   {
      Histogram hist(i_obj->getIntensities(),i_numOfIntervals);
      hist.drawHistogram(i_name);
   }
   else
   {
      std::cout << "Object given is empty.\n";
   }
}

//-----------------------------------------------------------------------------
Manager::~Manager()
{

}

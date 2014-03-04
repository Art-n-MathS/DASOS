#include "Manager.h"
#include "MarchingCubes.h"
#include "Histogram.h"

//-----------------------------------------------------------------------------
Manager::Manager()
{

}

//-----------------------------------------------------------------------------
Object *Manager::createObject(
        unsigned int i_x,
        const std::vector<double> user_limits,
        PulseManager *i_pulseManager,
        double noiseLevel
        )
{
   Object *obj = new Object(i_x,user_limits);
   obj->setNoiseLevel(noiseLevel);
   obj = i_pulseManager->fillObject(obj,user_limits);
   std::cout << "Object Created\n";
   return obj;
}


//-----------------------------------------------------------------------------
GLData *Manager::getPolygonisedObject(Object *i_obj, unsigned int i_x)
{
   if(i_obj!=0)
   {
      std::cout << "calling Marching Cubes\n";
      MarchingCubes mc(i_obj,i_x);
      return mc.createPolygonisedObject();
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

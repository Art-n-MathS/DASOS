#include "Manager.h"
#include "MarchingCubes.h"
#include "MCwithIntegralImages.h"
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
   std::cout << "Start filling object - full waveform\n";
   i_pulseManager->setNoiseLevel(noiseLevel);
   obj = i_pulseManager->fillObject(obj,user_limits);
   return obj;
}

//-----------------------------------------------------------------------------
Object *Manager::createObject(
        unsigned int i_x,
        const std::vector <double> user_limits,
        DiscreteData *i_discreteData
        )
{
   Object *obj = new Object(i_x,user_limits);
   std::cout << "Start filling object - discrete\n";
   obj->setNoiseLevel(0.0f);
   obj = i_discreteData->fillObject(obj,user_limits);
   return obj;
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

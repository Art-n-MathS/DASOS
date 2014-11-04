#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

//-----------------------------------------------------------------------------
/// @file ObjectManager.h
/// @author Milto Miltiadou
/// @version 1.0
/// @date 14/10/13
/// @class Object
/// @brief this manages operations that relates to objects
//-----------------------------------------------------------------------------


#include "Object.h"
#include "DiscreteData.h"

class Manager
{
public:
    //-------------------------------------------------------------------------
    /// @brief default constructor
    //-------------------------------------------------------------------------
    Manager();
    //-------------------------------------------------------------------------
    /// @brief method that returns a polygonised object
    /// @param[in] i_obj object to be polygonisedmilmil
    /// @param[in] i_x the number of cubes on the x axis
    /// @note the method dynamic allocated memory for the object, so by calling
    /// this function you are responsible for freeing the memory afterwards.
    //-------------------------------------------------------------------------
    static GLData *getPolygonisedObject(
            Object *i_obj,
            unsigned int i_x,
            bool i_useIntegralData
            );

    //-------------------------------------------------------------------------
    /// @brief method that saves the histogram of the voxelled object
    /// @param[in] i_obj the object
    /// @param[in] i_name the name of the image to be saved
    //-------------------------------------------------------------------------
    static void createHistogram(
            Object *i_obj,
            const std::string &i_name,
            unsigned int i_numOfIntervals = 200
            );
    //-------------------------------------------------------------------------
    /// @brief default destructor
    //-------------------------------------------------------------------------
    ~Manager();


private:



};

#endif // OBJECTMANAGER_H

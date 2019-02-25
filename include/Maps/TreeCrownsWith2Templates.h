#ifndef TREECROWNSWITH2TEMPLATES_H
#define TREECROWNSWITH2TEMPLATES_H


//-------------------------------------------------------------------------
/// @file TreeCrownsWith2Templates.h
/// @author Milto Miltiadou
/// @version 1.0
/// @date 17/07/15
/// @class TreeCrowns
/// @brief Metrics that gives the percentage of a pixel to be a tree crown
/// Takes as input 2 file with positive and negative templates and uses NN
///  to caclulate the possibility of being a tree crown
//-------------------------------------------------------------------------

#include "Map.h"
#include "TreeCrownsTemplates.h"


class TreeCrownsWith2Templates : public Map
{
public:
   //--------------------------------------------------------------------------
   /// @brief default constructor
   //--------------------------------------------------------------------------
   TreeCrownsWith2Templates(const std::string i_name,
           Volume *i_obj,
           const std::string i_PositiveTemplatesFileName,
           const std::string i_NegativeTemplatesFileName
           );
   //-------------------------------------------------------------------------
   /// @brief default destructor
   //-------------------------------------------------------------------------
   ~TreeCrownsWith2Templates();

private:
   //-------------------------------------------------------------------------
   /// @brief method that creates the Map
   //-------------------------------------------------------------------------
   void createMap();

   //-------------------------------------------------------------------------
   /// @brief positive tree crowns templates
   //-------------------------------------------------------------------------
   TreeCrownsTemplates m_positiveTemplates;
   //-------------------------------------------------------------------------
   /// @brief negative tree crowns templates
   //-------------------------------------------------------------------------
   TreeCrownsTemplates m_negativeTemplates;

};

#endif // TREECROWNSWITH2TEMPLATES_H

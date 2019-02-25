#ifndef TREECROWNS_H
#define TREECROWNS_H


//-------------------------------------------------------------------------
/// @file TreeCrowns.h
/// @author Milto Miltiadou
/// @version 1.0
/// @date 17/07/15
/// @class TreeCrowns
/// @brief Metrics that gives the percentage of a pixel to be a tree crown
/// Takes as input a file with templates and uses NN to caclulate the
/// possibility of being a tree crown
//-------------------------------------------------------------------------

#include "Map.h"
#include "TreeCrownsTemplates.h"


class TreeCrowns : public Map
{
public:
   //-------------------------------------------------------------------------
   /// @brief default constructor
   //-------------------------------------------------------------------------
   TreeCrowns(
           const std::string i_name,
           Volume *i_obj,
           const std::string i_templatesFileName
           );
   //-------------------------------------------------------------------------
   /// @brief default destructor
   //-------------------------------------------------------------------------
   ~TreeCrowns();

private:
   //-------------------------------------------------------------------------
   /// @brief method that creates the Map
   //-------------------------------------------------------------------------
   void createMap();

   //-------------------------------------------------------------------------
   /// @brief positive tree crowns templates
   //-------------------------------------------------------------------------
   TreeCrownsTemplates m_positiveTemplates;

};

#endif // TREECROWNS_H

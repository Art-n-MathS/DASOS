#ifndef HYPERMEAN_H
#define HYPERMEAN_H

#include "HyperMap.h"

class HyperMean : public Map
{
public:
   //--------------------------------------------------------------------------
   /// @brief default constructor
   //--------------------------------------------------------------------------
   HyperMean(const std::string &i_name,
           Volume *i_obj,
           const std::string &i_bilFileName,
           const std::string &i_IGMfileName
           );

   //--------------------------------------------------------------------------
   /// @brief default destructor
   //--------------------------------------------------------------------------
   ~HyperMean();

private:
   //--------------------------------------------------------------------------
   /// @brief
   //--------------------------------------------------------------------------
   void createMap();
   //-------------------------------------------------------------------------
   /// @brief the name of the IGM file associated with the hypersctral data
   //-------------------------------------------------------------------------
   std::string m_IGMfileName;
   //-------------------------------------------------------------------------
   /// @brief the name of the bil file of the hyperspectral data
   //-------------------------------------------------------------------------
   std::string m_bilFileName;

};

#endif // HYPERMEAN_H

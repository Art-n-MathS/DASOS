#ifndef LAS1_3_HANDLER_H
#define LAS1_3_HANDLER_H

#include <iostream>
#include <fstream>
#include <string>

#include "Types.h"
#include "Object.h"

class Las1_3_handler
{
public:
   //-------------------------------------------------------------------------
   /// @brief default constructor
   /// @param[in] i_filename the file of the name to be read
   //-------------------------------------------------------------------------
   Las1_3_handler(const std::string i_filename);
   //-------------------------------------------------------------------------
   /// @brief method that returns the min and max values that define the
   /// boundary of the scanned area
   //-------------------------------------------------------------------------
   std::vector<double> getBoundaries();
   //-------------------------------------------------------------------------
   /// @param[in] i_x the number of voxels in the x dimension - the rest are
   /// calculated depending on the higher and lower x y z of the points
   /// @param[in] i_user_limits [maxNorthY, minNorthY, maxEastX, minEastX]
   /// @return the implicit object which is a made by voxels
   //-------------------------------------------------------------------------
   Object *readFileAndGetObject(
           float i_voxelLength,
           const std::vector<double> &user_limits,
           double noiseLevel,
           int i_type
           );
   //-------------------------------------------------------------------------
   /// @brief method that prints headdata
   //-------------------------------------------------------------------------
   void printPublicHeader()const;
   //-------------------------------------------------------------------------
   /// @brief default destructor
   //-------------------------------------------------------------------------
   ~Las1_3_handler();




protected:
   //-------------------------------------------------------------------------
   /// @brief the name of the LAS file
   //-------------------------------------------------------------------------
   std::string m_filename;
   //-------------------------------------------------------------------------
   /// @brief the publi header block of the LAS1.3 file
   //-------------------------------------------------------------------------
   Types::Public_Header_Block public_header;
   //-------------------------------------------------------------------------
   /// @brief the intensity histogram of the lasfile
   //-------------------------------------------------------------------------
   int *i_hist;
   //-------------------------------------------------------------------------
   /// @brief Leica mission information
   //-------------------------------------------------------------------------
   Types::Leica_mission_info mis_info;
   //-------------------------------------------------------------------------
   /// @brief waveform packet descriptor
   //-------------------------------------------------------------------------
   Types::WF_packet_Descriptor wv_info;

private:
   //-------------------------------------------------------------------------
   /// @brief method that reads public header block
   //-------------------------------------------------------------------------
   void read_public_header();
   //-------------------------------------------------------------------------
   /// @brief method that reads variable length records including waveform
   /// packet descriptors (up to 255)
   //-------------------------------------------------------------------------
   void read_variable_length_records();
   //-------------------------------------------------------------------------
   /// @brief saved here to avoid opening and closing the file multiple times
   //-------------------------------------------------------------------------
   std::ifstream lasfile;
   //-------------------------------------------------------------------------
   /// @brief Public Header length in bytes.
   //-------------------------------------------------------------------------
   static const unsigned int public_header_length = 235;
   //-------------------------------------------------------------------------
   /// @brief  Variable Length Record Header length in bytes
   //-------------------------------------------------------------------------
   static const unsigned int VbleRec_header_length = 54;
   //-------------------------------------------------------------------------
   /// @brief Extended Variable Lenght Record Header, in Version 1.3 the only
   /// EVLR is waveform data packets
   //-------------------------------------------------------------------------
   static const unsigned int EVLR_length = 60;
   //-------------------------------------------------------------------------
   /// @brief the length of the point data
   //-------------------------------------------------------------------------
   static const unsigned int point_data_length = 57;


};

#endif // LAS1_3_HANDLER_H

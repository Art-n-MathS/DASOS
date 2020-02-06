#ifndef LAS1_3_HANDLER_H
#define LAS1_3_HANDLER_H

#include <iostream>
#include <fstream>
#include <string>

#include "LAS1_3Types.h"
#include "Volume.h"

class Las1_3_handler
{
public:
   //-------------------------------------------------------------------------
   /// @brief default constructor
   /// @param[in] i_filename the file of the name to be read
   //-------------------------------------------------------------------------
   Las1_3_handler(
           const std::string i_filename
           );
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
   void readFileAndGetObject(Volume *i_obj,
           const std::string i_type
           , const std::string &i_dtm);

   //-------------------------------------------------------------------------
   /// @brief method that prints 10 random pulses into a .csv file
   /// @param[in] i_csvFileName the name of the csv file to be saved
   /// @param[in] i_noOfSamples the number of samples to be saved
   //-------------------------------------------------------------------------
   void saveSamples(
           const std::string &i_csvFileName,
           unsigned int i_noOfSamples);
   //-------------------------------------------------------------------------
   /// @brief method that exports the sum intensity of each pulse into .csv
   /// @param[in] i_intsFileName the name of the file that will be exported
   /// and will contain the sum of intensities per pulse - ignored if it is ""
   //-------------------------------------------------------------------------
   void saveSumIntensity(const std::string i_intsFileName);
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
   std::string m_lasFilename;
   //-------------------------------------------------------------------------
   /// @brief the name of the wdp file
   //-------------------------------------------------------------------------
   std::string m_wdpFilename;
   //-------------------------------------------------------------------------
   /// @brief the publi header block of the LAS1.3 file
   //-------------------------------------------------------------------------
   LAS1_3Types::Public_Header_Block public_header;
   //-------------------------------------------------------------------------
   /// @brief the intensity histogram of the lasfile
   //-------------------------------------------------------------------------
   int *i_hist;
   //-------------------------------------------------------------------------
   /// @brief Leica mission information
   //-------------------------------------------------------------------------
   LAS1_3Types::Leica_mission_info mis_info;
   //-------------------------------------------------------------------------
   /// @brief waveform packet descriptor
   //-------------------------------------------------------------------------
   std::vector<LAS1_3Types::WF_packet_Descriptor *> wv_info;

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
   /// @brief method that read Point Record Format 4
   //-------------------------------------------------------------------------
   void read_point_record_format_4(Volume *i_obj,
           const std::string i_type,
           const std::string &m_dtm);

   void read_point_record_format_4(
           Volume *i_obj,
           const std::string i_type
           );

   //-------------------------------------------------------------------------
   /// @brief method that read Point Record Format 0
   //-------------------------------------------------------------------------
   void read_point_record_format_0(Volume *i_obj);
   //-------------------------------------------------------------------------
   /// @brief method that read Point Record Format 1
   //-------------------------------------------------------------------------
   void read_point_record_format_1(Volume *i_obj);
   //-------------------------------------------------------------------------
   /// @brief method that read Point Record Format 2
   //-------------------------------------------------------------------------
   void read_point_record_format_2(Volume *i_obj);
   //-------------------------------------------------------------------------
   /// @brief method that read Point Record Format 3
   //-------------------------------------------------------------------------
   void read_point_record_format_3(Volume *i_obj);
   //--------------------------------------------------------------------------
   /// @brief method that checks the endianess of the computer
   //--------------------------------------------------------------------------
   bool isBigEndian();

   //-------------------------------------------------------------------------
   /// @brief length of file that contains waveforms (either .LAS or .wdp)
   //-------------------------------------------------------------------------
   unsigned int long long m_wvFileLenght;
   //-------------------------------------------------------------------------
   /// @brief saved here to avoid opening and closing the file multiple times
   //-------------------------------------------------------------------------
   std::ifstream m_lasfile;
   //-------------------------------------------------------------------------
   /// @brief wdp file, only exist if the waveforms are saved into an
   /// external file
   //-------------------------------------------------------------------------
   std::ifstream m_wdpFile;
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
   //-------------------------------------------------------------------------
   /// @brief indicates whether the waveform are saved inside the las file
   /// or outside
   //-------------------------------------------------------------------------
   bool m_areWFInternal;


};

#endif // LAS1_3_HANDLER_H

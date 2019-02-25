#ifndef PW_HANDLER_H
#define PW_HANDLER_H

#include <iostream>
#include <fstream>
#include <string>

#include "PWTypes.h"
#include "PW_descriptor.h"
#include "Volume.h"
#include "PW_scannerVLR.h"
#include "PW_lookUpTable.h"

class PW_handler
{
public:
   //-------------------------------------------------------------------------
   /// @brief default constructor
   /// @param[in] i_filename the file of the name to be read
   //-------------------------------------------------------------------------
   PW_handler(const std::string i_filename);
   //-------------------------------------------------------------------------
   /// @brief method that returns the min and max values that define the
   /// boundary of the scanned area
   //-------------------------------------------------------------------------
   std::vector<double> getBoundaries();
   //-------------------------------------------------------------------------
   /// @brief method that prints 10 random pulses into a .csv file
   /// @param[in] i_csvFileName the name of the csv file to be saved
   /// @param[in] i_noOfSamples the number of samples to be saved
   //-------------------------------------------------------------------------
   void saveSamples(
           const std::string &i_csvFileName,
           unsigned int i_noOfSamples);
   //-------------------------------------------------------------------------
   /// @param[in] i_x the number of voxels in the x dimension - the rest are
   /// calculated depending on the higher and lower x y z of the points
   /// @param[in] i_user_limits [maxNorthY, minNorthY, maxEastX, minEastX]
   /// @return the implicit object which is a made by voxels
   //-------------------------------------------------------------------------
   void readFileAndGetObject(
           Volume *i_obj,
           const std::string &i_type,
           const std::string &i_dtmFileName
           );
   //-------------------------------------------------------------------------
   /// @brief method that prints headdata
   //-------------------------------------------------------------------------
   void printPublicHeader()const;
   //-------------------------------------------------------------------------
   /// @brief default destructor
   //-------------------------------------------------------------------------
   ~PW_handler();




protected:
   //-------------------------------------------------------------------------
   /// @brief the name of the pulse waves file
   //-------------------------------------------------------------------------
   std::string m_pls_filename;
   //-------------------------------------------------------------------------
   /// @brief the name of the waves file
   //-------------------------------------------------------------------------
   std::string m_wvs_filename;
   //-------------------------------------------------------------------------
   /// @brief the name of the waveform packets file
   //-------------------------------------------------------------------------
   std::string m_fw_packets;

   //-------------------------------------------------------------------------
   /// @brief the public header block of the LAS1.3 file
   //-------------------------------------------------------------------------
   PWTypes::Header_Block public_header;
   //-------------------------------------------------------------------------
   /// @brief the
   //-------------------------------------------------------------------------
   PWTypes::WavesHeader waves_header;

   //-------------------------------------------------------------------------
   /// @brief the
   //-------------------------------------------------------------------------
//   std::vector<std::pair<PWTypes::CompositionRecord *, PWTypes::SamplingRecord **> > m_compositionRecords;
   //-------------------------------------------------------------------------
   /// @brief the
   //-------------------------------------------------------------------------

   //-------------------------------------------------------------------------
   /// @brief the
   //-------------------------------------------------------------------------


   //-------------------------------------------------------------------------
   /// @brief the intensity histogram of the lasfile
   //-------------------------------------------------------------------------
//   int *i_hist;
   //-------------------------------------------------------------------------
   /// @brief Leica mission information
   //-------------------------------------------------------------------------
//   PWTypes::Leica_mission_info mis_info;
   //-------------------------------------------------------------------------
   /// @brief waveform packet descriptor
   //-------------------------------------------------------------------------
//   PWTypes::WF_packet_Descriptor wv_info;

private:
   //-------------------------------------------------------------------------
   /// @brief methdod that prints the pulse info information
   //-------------------------------------------------------------------------
   void print_pointInfo(const PWTypes::PulseRecord &i_pulse_info)const;

   //-------------------------------------------------------------------------
   /// @brief method that reads variable length records including waveform
   /// packet descriptors (up to 255)
   //-------------------------------------------------------------------------
   void read_variable_length_records();
   //-------------------------------------------------------------------------
   /// @brief saved here to avoid opening and closing the file multiple times
   //-------------------------------------------------------------------------
   std::ifstream plsfile;
   //-------------------------------------------------------------------------
   /// @brief the binary waves file
   //-------------------------------------------------------------------------
   std::ifstream wvsfile;
   //-------------------------------------------------------------------------
   /// @brief Public Header length in bytes.
   //-------------------------------------------------------------------------
   static const unsigned int public_header_length = 352;
   //-------------------------------------------------------------------------
   /// @brief  Variable Length Record Header length in bytes
   //-------------------------------------------------------------------------
   static const unsigned int VbleRec_header_length = 96;

   //-------------------------------------------------------------------------
   /// @brief pulse wave decriptors
   //-------------------------------------------------------------------------
   std::vector<PW_descriptor *> m_pwDecriptors;
   //-------------------------------------------------------------------------
   /// @brief the scanners
   //-------------------------------------------------------------------------
   std::vector<PW_scannerVLR *> m_scanners;
   //-------------------------------------------------------------------------
   /// @brief look up tables
   //-------------------------------------------------------------------------
   std::vector<PW_lookUpTable *> m_lookUpTables;

   //-------------------------------------------------------------------------
   /// @brief Extended Variable Lenght Record Header, in Version 1.3 the only
   /// EVLR is waveform data packets
   //-------------------------------------------------------------------------
//   static const unsigned int EVLR_length = 60;
   //-------------------------------------------------------------------------
   /// @brief the length of the point data
   //-------------------------------------------------------------------------
//   static const unsigned int point_data_length = 57;

};

#endif // PW_HANDLER_H

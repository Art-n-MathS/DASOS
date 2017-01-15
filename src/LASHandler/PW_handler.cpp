#include "PW_handler.h"

#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <DtmBilReader.h>

//-----------------------------------------------------------------------------
PW_handler::PW_handler(
        const std::string i_filename
        ):
    m_pls_filename(i_filename),
    m_wvs_filename(i_filename)

{
    m_wvs_filename.replace(m_wvs_filename.end()-3,m_wvs_filename.end(),"wvs");
    std::cout << "\nReading: " << i_filename << "\n";
    plsfile.open(m_pls_filename.c_str(),std::ios::binary);
    wvsfile.open(m_wvs_filename.c_str(),std::ios::binary);

    if(!plsfile.is_open())
    {
       std::cerr << "PLS File not found \n";
       return;
    }
    if(!wvsfile.is_open())
    {
       std::cerr <<"WVS file not found \n";
       return;
    }
    plsfile.read((char *) &public_header,sizeof(public_header));
    wvsfile.read((char *) &waves_header ,sizeof(waves_header ));
//    printPublicHeader();

    plsfile.close();
    wvsfile.close();
}

//-----------------------------------------------------------------------------
void PW_handler::saveSamples(
        const std::string &i_csvFileName,
        unsigned int i_noOfSamples
        )
{
    read_variable_length_records();
    plsfile.open(m_pls_filename.c_str(),std::ios::binary);
    wvsfile.open(m_wvs_filename.c_str(),std::ios::binary);
    PWTypes::PulseRecord pulse_info;
    plsfile.seekg((int)public_header.offset_to_pulse_data);

    std::ofstream csv;
    csv.open(i_csvFileName);
    csv << "Amplitude\n";
    for(unsigned int i=0; i<i_noOfSamples && i<public_header.number_of_pulses; ++i)
    {
       plsfile.read((char *) &pulse_info, (int)public_header.pulse_size);
       PW_descriptor *currentDis = NULL;
       unsigned short int id = pulse_info.pulse_discriptor_index;
       unsigned int dIndex=0;
       for(; dIndex< m_pwDecriptors.size();++dIndex)
       {
          if(id==m_pwDecriptors[dIndex]->m_id)
          {
             break;
          }
       }
       if(id==m_pwDecriptors.size())
       {
          std::cout << "WARNING: descriptor does not exist. Pulse will be ignored\n";
          continue;
       }
       else
       {
          currentDis = m_pwDecriptors[dIndex];
          currentDis->addAmplitudes(csv,wvsfile,pulse_info.offset_to_waves);
       }
    }
    std::cout << "\n";
    plsfile.close();
    wvsfile.close();
    csv.close();
    std::cout << "CSV file " << i_csvFileName << " saved\n";
}

//-----------------------------------------------------------------------------
void PW_handler::printPublicHeader()const
{
   std::cout << std::setprecision(4) << std::fixed;
   std::cout << "\nHeader file of " << m_pls_filename << std::endl;
   std::cout << "\nHeader file of " << m_wvs_filename << std::endl;
   std::cout << "File Signature " << public_header.file_signiture << std::endl;
   std::cout << "Global Parameters " << public_header.global_parameters << std::endl;
   std::cout << "File Source ID  "<< public_header.file_source_ID << std::endl;

   std::cout << "Project ID - GUID data 1 " << public_header.project_ID_GUID_data_1 << std::endl;
   std::cout << "Project ID - GUID data 2 " << public_header.project_ID_GUID_data_2 << std::endl;
   std::cout << "Project ID - GUID data 3 " << public_header.project_ID_GUID_data_3 << std::endl;
   std::cout << "Project ID - GUID data 4 " << public_header.project_ID_GUID_data_4 << std::endl;
   std::cout << "Version Major " << (int) public_header.version_major << std::endl;
   std::cout << "Version Minor " << (int) public_header.version_minor << std::endl;
   std::cout << "System Identifier " << public_header.system_identifier << std::endl;
   std::cout << "Generating Software " << public_header.generating_software << std::endl;

   std::cout << "File Creation Day of Year  " << public_header.file_creation_day_of_year << std::endl;
   std::cout << "File Creation Year " << public_header.file_creation_year << std::endl;
   std::cout << "Header Size    "  <<  public_header.header_size << std::endl;
   std::cout << "Offset to pulse data "  <<  public_header.offset_to_pulse_data << std::endl;
   std::cout << "Number of Pulses "  <<  public_header.number_of_pulses << std::endl;
   std::cout << "Pulse format "  << (int)  public_header.pulse_format << std::endl;
   std::cout << "Pulse attributs " << (int) public_header.pules_attributes << std::endl;
   std::cout << "Pulse size " << (int) public_header.pulse_size << std::endl;
   std::cout << "Pulse compression " << (int)public_header.pulse_compression << std::endl;
   std::cout << "reserved " << (int) public_header.reserved << std::endl;

   std::cout << "Number of Variable Length records " << (int) public_header.number_of_variable_length_records << std::endl;
   std::cout << "Number of Appended variable length records " << (int) public_header.number_of_appended_variable_length_records << std::endl;

   std::cout << "t scale factor " << public_header.t_scale_factor << std::endl;
   std::cout << "t offset " << public_header.t_offset << std::endl;
   std::cout << "min t " << public_header.min_t << std::endl;
   std::cout << "max_t " << public_header.max_t << std::endl;
   std::cout << "X scale factor "  <<  public_header.x_scale_factor << std::endl;
   std::cout << "Y scale factor "  <<  public_header.y_scale_factor << std::endl;
   std::cout << "Z scale factor "  <<  public_header.z_scale_factor << std::endl;
   std::cout << "X offset  "  <<  (double) public_header.x_offset << std::endl;
   std::cout << "Y offset  " <<  (double) public_header.y_offset << std::endl;
   std::cout << "Z offset  "  <<  (double) public_header.z_offset << std::endl;
   std::cout << "Max X  "  << (double) public_header.max_x << std::endl;
   std::cout << "Min X  "  << (double)  public_header.min_x << std::endl;
   std::cout << "Max Y  " <<  (double) public_header.max_y << std::endl;
   std::cout << "Min Y  " <<  (double) public_header.min_y << std::endl;
   std::cout << "Max Z  " <<  (double) public_header.max_z << std::endl;
   std::cout << "Min Z  " <<  (double) public_header.min_z << std::endl;
}

//-----------------------------------------------------------------------------
std::vector<double> PW_handler::getBoundaries()
{
    std::vector<double> boundaries(6);
    // [MaxNorthY, MinNorthY, MaxEastX, MinEastX, MaxHeightZ, MinHeightZ]
    boundaries[0] = public_header.max_y;
    boundaries[1] = public_header.min_y;
    boundaries[2] = public_header.max_x;
    boundaries[3] = public_header.min_x;
    boundaries[4] = public_header.max_z;
    boundaries[5] = public_header.min_z;
    return boundaries;
}

//-----------------------------------------------------------------------------
void PW_handler::read_variable_length_records()
{
   plsfile.open(m_pls_filename.c_str(),std::ios::binary);
   plsfile.seekg(public_header.header_size);
   PWTypes::Variable_Length_Record headdata_rec;

   for(unsigned int i=0; i<public_header.number_of_variable_length_records;++i)
   {
      plsfile.read((char *) &headdata_rec,VbleRec_header_length);
      char skip_record [headdata_rec.record_length_after_header];
      plsfile.read((char *) &skip_record, headdata_rec.record_length_after_header);

      if(headdata_rec.record_ID < 100255 && headdata_rec.record_ID >= 100000)
      {

         m_scanners.push_back(new PW_scannerVLR(
                                  skip_record,
                                  headdata_rec.record_length_after_header,
                                  headdata_rec.record_ID-100000)
                              );
      }
      else  if(headdata_rec.record_ID < 200255 && headdata_rec.record_ID >= 200000)
      {

         m_pwDecriptors.push_back(
                     new PW_descriptor(skip_record,
                                       headdata_rec.record_length_after_header,
                                       headdata_rec.record_ID-200000)
                     );
      }
      else  if(headdata_rec.record_ID < 300255 && headdata_rec.record_ID >= 300000)
      {

         m_lookUpTables.push_back(
                     new PW_lookUpTable(skip_record,
                                        headdata_rec.record_length_after_header,
                                        headdata_rec.record_ID-300000)
                     );
      }
   }
   plsfile.close();
}

//-----------------------------------------------------------------------------
void PW_handler::readFileAndGetObject(Volume *i_obj,
        const std::string & /*i_type*/,
        const std::string &i_dtmFileName
        )
{
   DtmBilReader dtm(i_dtmFileName,i_obj);
   read_variable_length_records();
   std::cout << "Variable Length records read, tables, scanners and descriptors saved\n";

   plsfile.open(m_pls_filename.c_str(),std::ios::binary);
   wvsfile.open(m_wvs_filename.c_str(),std::ios::binary);
   PWTypes::PulseRecord pulse_info;

   unsigned int percentage = public_header.number_of_pulses / 10;
   plsfile.seekg((int)public_header.offset_to_pulse_data);
   std::cout << "Progress on reading " << public_header.number_of_pulses << " pulses:\n";
   for(unsigned int i=0; i<public_header.number_of_pulses; ++i)
   {
      if(i%percentage == 0)
      {
         std::cout << i/percentage*10 <<  "% ...";
      }
      plsfile.read((char *) &pulse_info, (int)public_header.pulse_size);
      gmtl::Vec3f anchor(
          pulse_info.anchor_x * public_header.x_scale_factor + public_header.x_offset,
          pulse_info.anchor_y * public_header.y_scale_factor + public_header.y_offset,
          pulse_info.anchor_z * public_header.z_scale_factor + public_header.z_offset);

      gmtl::Vec3f target(
          pulse_info.target_x * public_header.x_scale_factor + public_header.x_offset,
          pulse_info.target_y * public_header.y_scale_factor + public_header.y_offset,
          pulse_info.target_z * public_header.z_scale_factor + public_header.z_offset);

      gmtl::Vec3f dis(( target[0]-anchor[0])/1000.0,
                      ( target[1]-anchor[1])/1000.0,
                      ( target[2]-anchor[2])/1000.0);

      PW_descriptor *currentDis = NULL;
      unsigned short int id = pulse_info.pulse_discriptor_index;
      unsigned int dIndex=0;
      for(; dIndex< m_pwDecriptors.size();++dIndex)
      {
         if(id==m_pwDecriptors[dIndex]->m_id)
         {
            break;
         }
      }
      if(id==m_pwDecriptors.size())
      {
         std::cout << "WARNING: descriptor does not exist. Pulse will be ignored\n";
         continue;
      }
      else
      {
         currentDis = m_pwDecriptors[dIndex];
         currentDis->readWaveform(wvsfile,pulse_info.offset_to_waves,anchor,dis,i_obj,dtm);
      }
   }
   std::cout << "All waves Read\n";
   plsfile.close();
   wvsfile.close();
}


//-----------------------------------------------------------------------------
void PW_handler::print_pointInfo(const PWTypes::PulseRecord &i_pulse_info)const
{
//   std::cout << "---------------------------------\n";
   std::cout << "GPS timesta P: " << i_pulse_info.gps_timestap_t << "\n";
   std::cout << "Offset to waves: " << i_pulse_info.offset_to_waves << "\n";
   std::cout << "Achor x y z : "  << i_pulse_info.anchor_x << " " << i_pulse_info.anchor_y << " " << i_pulse_info.anchor_z<< "\n";
   std::cout << "Target x, y z: " << i_pulse_info.target_x << " " << i_pulse_info.target_y << " " << i_pulse_info.target_z << "\n";
   std::cout << "First Returning Sample: " << i_pulse_info.first_returning_sample << "\n";
   std::cout << "Last Returning Sample: " << i_pulse_info.last_returning_sample << "\n";
   std::cout << "Pulse Discriptor index: " << i_pulse_info.pulse_discriptor_index << int(i_pulse_info.pulse_discriptor_index) << "\n";
//   std::cout << "Reserved " << i_pulse_info.res4_scanLineE1_scanDir1_MirFacet_2
   std::cout << "Intensity : " << (int)i_pulse_info.intensity << "\n";
   std::cout << "Classification : " << (int)i_pulse_info.classification << "\n";

   std::cout << "---------------------------------\n";
}

//-----------------------------------------------------------------------------
PW_handler::~PW_handler()
{
   if(plsfile.is_open())
   {
      plsfile.close();
   }
   if(wvsfile.is_open())
   {
      wvsfile.close();
   }
   for(unsigned int i=0; i<m_pwDecriptors.size(); ++i)
   {
      delete m_pwDecriptors[i];
   }
   for(unsigned int i=0; i<m_scanners.size(); ++i)
   {
      delete m_scanners[i];
   }
   for(unsigned int i=0; i<m_lookUpTables.size(); ++i)
   {
      delete m_lookUpTables[i];
   }
}



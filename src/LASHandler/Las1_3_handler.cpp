#include "Las1_3_handler.h"
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <algorithm>
#include <new>
#include <iomanip>

#include <iostream>
#include <fstream>
#include <gmtl/gmtl.h>
#include <map>


//-----------------------------------------------------------------------------
Las1_3_handler::Las1_3_handler(
        const std::string i_filename
        ): m_filename(i_filename),i_hist(0)
{
   std::cout << "\n" << i_filename << "\n";
}

//-----------------------------------------------------------------------------
std::vector<double> Las1_3_handler::getBoundaries()
{
   lasfile.open(m_filename.c_str(),std::ios::binary);
   if(!lasfile.is_open())
   {
      std::cerr << "File not found \n";
   }
   read_public_header();
   std::vector<double> boundaries(6);
   // [MaxNorthY, MinNorthY, MaxEastX, MinEastX, MaxHeightZ, MinHeightZ]
   boundaries[0] = public_header.max_y;
   boundaries[1] = public_header.min_y;
   boundaries[2] = public_header.max_x;
   boundaries[3] = public_header.min_x;
   boundaries[4] = public_header.max_z;
   boundaries[5] = public_header.min_z;
   lasfile.close();
   return boundaries;
}

//-----------------------------------------------------------------------------
Object *Las1_3_handler::readFileAndGetObject(float i_voxelLength,
        const std::vector<double> &user_limits,
        double noiseLevel,
        const std::string i_type
        )
{
   lasfile.open(m_filename.c_str(),std::ios::binary);
   if(!lasfile.is_open())
   {
      std::cerr << "File not found \n";
   }
   read_public_header();
   read_variable_length_records();

   Object *obj=new Object(i_voxelLength,user_limits);
   obj->setNoiseLevel(noiseLevel);

   if(obj==0)
   {
      std::cerr << "Error: Memory could not be allocated\n";
      exit(EXIT_FAILURE);
   }
   Types::Data_Point_Record_Format_4 point_info;
   lasfile.seekg((int) public_header.offset_to_point);

   unsigned int count=0;
   // temporarly saving discrete values that are associated with a
   // waveform but the 1st return haven't been saved yet
   std::vector<gmtl::Vec3f> discretePoints;
   // the corresponding intensities of the discrete points
   std::vector<unsigned short> discreteIntensities;
   // the corresponding wave offsets of the dicrete points
   std::vector<int> discreteWaveOffsets;

   std::map<std::string, int> types;
   types["FULL-WAVEFORM"] = 1;
   types["ALL_DISCRETE"] = 2;
   types["DISCRETE_N_WAVEFORMS"] = 3;
   types["DISCRETE"] = 4;


   std::string s(i_type);
   std::transform(s.begin(), s.end(), s.begin(), toupper);

   for(unsigned int i=0; i< public_header.number_of_point_records; ++i)
   {
      lasfile.read((char *) &point_info, (int) public_header.point_data_record_length);
      int wave_offset = public_header.start_of_wf_data_Packet_record +
               point_info.byte_offset_to_wf_packet_data;

      if((int)point_info.classification!=7)
      {
         // TYPES:
         // 1. Full-waveform
         // 2. All_Discrete
         // 3. Discrete_n_Waveforms
         // 4. Discrete (associated with waveform only
         switch(types[s])
         {
         // Waveform samples only
         case 1:
         {
            if( point_info.wave_packet_descriptor_index!=0 &&
                    (unsigned int)(point_info.returnNo_noOfRe_scanDirFla_EdgeFLn&7)==1 )
            {
               count++;
               char *wave_data = new (std::nothrow) char [point_info.wf_packet_size_in_bytes];
               if(wave_data==0)
               {
                   std::cerr << "Fail assigning memory in file Las1_3_handler.cpp\n"
                             << "Program will terminate\n";
                   exit(EXIT_FAILURE);
               }
               int tmp = lasfile.tellg();
               lasfile.seekg(wave_offset);
               lasfile.read((char *) wave_data,point_info.wf_packet_size_in_bytes);

               gmtl::Vec3f origin (point_info.X*public_header.x_scale_factor+public_header.x_offset,
                                   point_info.Y*public_header.y_scale_factor+public_header.y_offset,
                                   point_info.Z*public_header.z_scale_factor+public_header.z_offset);

               float temporalSampleSpacing = ((int)wv_info.temporal_sample_spacing)/1000.0f;
               gmtl::Vec3f offset= gmtl::Vec3f(point_info.X_t, point_info.Y_t, point_info.Z_t);
               offset *= (1000.0f * temporalSampleSpacing);

               origin[0] = origin[0] + (double )point_info.X_t*
                       (double )point_info.return_point_wf_location;
               origin[1] = origin[1] + (double )point_info.Y_t*
                       (double )point_info.return_point_wf_location;
               origin[2] = origin[2] + (double )point_info.Z_t*
                       (double )point_info.return_point_wf_location;

               unsigned int noOfSamples = point_info.wf_packet_size_in_bytes;

               char *waveSamplesIntensities = new (std::nothrow) char[noOfSamples];
               if(waveSamplesIntensities==0)
               {
                   std::cerr << "Error: Memory could not be allocated in file Pulse.cpp\n";
                   exit(EXIT_FAILURE);
               }
               memcpy(waveSamplesIntensities,wave_data,noOfSamples);

               gmtl::Vec3f tempPosition = origin;
               for(unsigned int j=0; j< noOfSamples; ++j)
               {
                 obj->addItensity(tempPosition,waveSamplesIntensities[j]);
                 tempPosition+=offset;
               }
               lasfile.seekg(tmp);
               delete []waveSamplesIntensities;
               delete []wave_data;
            }
            break;
         }
         // All Discrete Points
         case 2:
            if((int)point_info.classification!=7)
            {
               obj->addItensity(gmtl::Vec3f(point_info.X*public_header.x_scale_factor,
                                         point_info.Y*public_header.y_scale_factor,
                                         point_info.Z*public_header.z_scale_factor),
                             point_info.itensity);
            }
            break;
         case 3:

            break;
         // Only Discrete Returns that have a waveform associated with the
         case 4:
            if (point_info.wave_packet_descriptor_index!=0 &&
                    (int)point_info.classification!=7)
            {
               obj->addItensity(gmtl::Vec3f(point_info.X*public_header.x_scale_factor,
                                            point_info.Y*public_header.y_scale_factor,
                                            point_info.Z*public_header.z_scale_factor),
                                point_info.itensity);
            }
            break;
         default:
             //there are no more types of objects, returns an empty volume
             std::cout <<  "WARNING: The type <" << i_type
                       << "> does not defined a valid type of data\n";
             break;
         }
      }
      else
      {
          // only noise has been recorded
      }

   }

   if(count==0)
   {
       std::cout << "no waveforms associated with that area\n";
   }
   else
   {
       std::cout << count << " waveforms found\n";
       std::cout << public_header.number_of_point_records << " discrete points found\n";
   }

   discreteIntensities.clear();
   discretePoints.clear();
   discreteWaveOffsets.clear();
//   std::cout << "There are " << i_pulseManager->getNumOfAloneDiscretePoints()
//             << " Discrete Without Waveforms\n";
   std::cout << "----------------------------------------------------------\n";
   lasfile.close();
   obj->normalise();
   return obj;
}


//-----------------------------------------------------------------------------
void Las1_3_handler::read_public_header()
{
   lasfile.read((char *) &public_header,sizeof(public_header));



   // tests if waveform data packets are saved into that file
   // if not program terminates because it cannot handle that case
   if((public_header.global_encoding & 2 ) !=2 )
   {
      std::cerr << "Waveform Data Packets are not saved in this file.\n"
                << "Program will terminate\n";
      exit(EXIT_FAILURE);
   }

   // test version of the file. version should be 1.3
   if (public_header.version_major!=1 || public_header.version_minor!=3)
   {
      std::cerr << "Incorrect version. Only 1.3 is allowed.\n"
                << "Program will terminate\n";
      exit(EXIT_FAILURE);
  }
}

//-----------------------------------------------------------------------------
void Las1_3_handler::read_variable_length_records()
{
   Types::Variable_Length_Record_Header headdata_rec;
   for(unsigned int i=0; i<public_header.number_of_variable_lenght_records;
      ++i)
   {
      lasfile.read((char *) &headdata_rec, VbleRec_header_length);
      char skip_record [headdata_rec.record_length_after_header];
      lasfile.read((char *) skip_record,headdata_rec.record_length_after_header);

      // 1001 for the intensity histogram
      /*
      #If RecordID= 1001 it is the intensity histogram
      # of 1st returns w/ intensity of 0
      # of 1st returns w/ intensity of 1
      # of 1st returns w/ intensity of 2
      #
      # of 1st returns w/ intensity of 255
      # of 2nd returns w/ intensity of 0
      # of 2nd returns w/ intensity of 1
      # of 2nd returns w/ intensity of 2
      #
      # of 2nd returns w/ intensity of 255
      # of 3rd returns w/ intensity of 0
      # of 3rd returns w/ intensity of 1
      # of 3rd returns w/ intensity of 2
      #
      # of 3rd returns w/ intensity of 255
      # of returns with AGC value of 0
      # of returns with AGC value of 1
      # of returns with AGC value of 2
      #
      # of returns with AGC value of 255
      # of all returns with intensity of 0
      # of all returns with intensity of 1
      # of all returns with intensity of 2
      #
      # of all returns with intensity of 255
      */
      if(headdata_rec.record_ID == 1001)
      {
         i_hist = new (std::nothrow) int[headdata_rec.record_length_after_header/4];
         if(i_hist==0)
         {
             std::cout << "Memory assignment failed in Las1_3_handler\n"
                       << "Program will terminate\n";
             exit(EXIT_FAILURE);
         }
         memcpy((void *)i_hist,(void *)skip_record,headdata_rec.record_length_after_header);
///         for(unsigned int i=0; i<headdata_rec.record_length_after_header/4; ++i)
///         {
///             std::cout << i_hist[i] << " ";
///         }
///         std::cout << std::endl;
      }

      // 1002 for Leica mission info containing
      /*
      #If RecordID= 1002 it is Leica mission info containing
      #0_ Laser Pulserate: 1 Hz
      #1_ Field Of View: 0.1 degrees
      #2_ Scanner Offset: 1 ticks
      #3_ Scan Rate:  0.1 Hz
      #4_ Flying Altitude (AGL): meters
      #5_ GPS Week Number at start of line:   week
      #6_ GPS Seconds of Week at start of line: seconds
      #7_ Reserved
      #NOTE Leica definition says this record contains 26 bytes but it actually contains only 22 so not sure what is what...
      # By comparisson with FlightLineLog fields 0,3,4,5 and 6 are ok
      */
      else if (headdata_rec.record_ID == 1002)
      {
         memcpy((void *) &mis_info,(void *)skip_record,sizeof(mis_info));
///         std::cout << "miss_info = " << mis_info.laster_pulse_rate << " "
///                   << mis_info.field_of_view << " " << mis_info.scanner_offset << " "
///                   << mis_info.scan_rate << " " << mis_info.fly_altitude << "\n";
      }
      else if (headdata_rec.record_ID == 1003)
      {

      }
      else if (headdata_rec.record_ID>=100 && headdata_rec.record_ID<356)
      {
         memcpy((void *) &wv_info, (void *) skip_record, sizeof(wv_info));
//         std::cout <<  "//--------------------------------------------------\n"
//                   <<"bits per sample = " << (int) wv_info.bits_per_sample << "\n"
//                   << "waveform compression type = " << (int)  wv_info.wf_compression_type << "\n"
//                   << "number of samples = " << (int)  wv_info.number_of_samples << "\n"
//                   << "temporal sample spacing = " << (int)  wv_info.temporal_sample_spacing << "\n"
//                   << "Digitizer Gain = " <<(int)  wv_info.digitizer_gain<< "\n"
//                   << "Digitizer Offset = " << (int) wv_info.digitizer_offset << "\n";
      }
   }
}

//-----------------------------------------------------------------------------
void Las1_3_handler::printPublicHeader()const
{
   std::cout << std::setprecision(2) << std::fixed;
   std::cout << "\nHeader file of " << m_filename << std::endl;
   std::cout << "File Signature " << public_header.file_signiture << std::endl;
   std::cout << "File Source ID  "<< public_header.file_source_ID << std::endl;
   std::cout << "Global Encoding "<< public_header.global_encoding << std::endl;
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
   std::cout << "Offset to point data "  <<  public_header.offset_to_point << std::endl;
   std::cout << "Number of Variable Length Records "  <<  public_header.number_of_variable_lenght_records << std::endl;
   std::cout << "Point Data Format ID (0-99 for spec) "  << (int)  public_header.point_data_format_ID << std::endl;
   std::cout << "Point Data Record Length "  <<  public_header.point_data_record_length << std::endl;
   std::cout << "Number of point records " <<  public_header.number_of_point_records << std::endl;
   std::cout << "Number of points by return  " <<  public_header.number_of_points_by_return[0] << " "
             <<  public_header.number_of_points_by_return[1]<< " "
             <<  public_header.number_of_points_by_return[2]<< " "
             <<  public_header.number_of_points_by_return[3]<< " "
             <<  public_header.number_of_points_by_return[4]  <<  std::endl;
   std::cout << "X scale factor "  <<  public_header.x_scale_factor << std::endl;
   std::cout << "Y scale factor  "  <<  public_header.y_scale_factor << std::endl;
   std::cout << "Z scale factor "  <<  public_header.z_scale_factor << std::endl;
   std::cout << "X offset  "  <<  (double) public_header.x_offset << std::endl;
   std::cout << "Y offset  " <<  (double) public_header.y_offset << std::endl;
   std::cout << "Z offset "  <<  (double) public_header.z_offset << std::endl;
   std::cout << "Max X  "  << (double) public_header.max_x << std::endl;
   std::cout << "Min X  "  << (double)  public_header.min_x << std::endl;
   std::cout << "Max Y  " <<  (double) public_header.max_y << std::endl;
   std::cout << "Min Y " <<  (double) public_header.min_y << std::endl;
   std::cout << "Max Z  " <<  (double) public_header.max_z << std::endl;
   std::cout << "Min Z  " <<  (double) public_header.min_z << std::endl;
   std::cout << "Start of Waveform Data Packet Record " <<  public_header.start_of_wf_data_Packet_record << std::endl;
}

//-----------------------------------------------------------------------------
Las1_3_handler::~Las1_3_handler()
{
    if (i_hist!=0)
    {
        delete []i_hist;
    }
}


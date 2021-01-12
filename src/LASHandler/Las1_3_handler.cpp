#include "Las1_3_handler.h"
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <algorithm>
#include <new>
#include <iomanip>
#include <iterator>
#include <iostream>
#include <fstream>
#include <gmtl/gmtl.h>
#include <map>
#include "DtmBilReader.h"

//-----------------------------------------------------------------------------
Las1_3_handler::Las1_3_handler(
        const std::string i_filename
        ):
    m_lasFilename(i_filename),
    i_hist(0),
    m_areWFInternal(true)
{
   std::cout << "\n" << i_filename << "\n";
   m_lasfile.open(m_lasFilename.c_str(),std::ios::binary);
   if(!m_lasfile.is_open())
   {
      std::cerr << "File not found \n";
      std::exit(EXIT_FAILURE);
      return;
   }
   read_public_header();
   read_variable_length_records();
   if(m_areWFInternal)
   {
     m_lasfile.seekg(0,std::ios::end);
     m_wvFileLenght = m_lasfile.tellg();
   }
   else
   {
      m_wdpFile.open(m_wdpFilename.c_str(),std::ios::binary);
      m_wdpFile.seekg(0,std::ios::end);
      m_wvFileLenght = m_wdpFile.tellg();
      m_wdpFile.close();
   }
   m_lasfile.close();
}

////-----------------------------------------------------------------------------
//void Las1_3_handler::saveSumIntensity(
//        const std::string i_intsFilename,
//        double i_noiseLevel,
//        const std::vector<double> &userLimits
//        )
//{
//   m_lasfile.open(m_lasFilename.c_str(),std::ios::binary);
//   if(!m_lasfile.is_open())
//   {
//      return;
//   }
//   if((unsigned short int)(public_header.point_data_format_ID)!=4)
//   {
//      std::cerr << " ++ ERROR: Not supported LAS file format\n";
//      m_lasfile.close();
//      return;
//   }
//   LAS1_3Types::Data_Point_Record_Format_4 point_info;
//   m_lasfile.seekg((int) public_header.offset_to_point);
//   if(!m_areWFInternal)
//   {
//      m_wdpFile.open(m_wdpFilename.c_str(),std::ios::binary);
//      if(!m_wdpFile)
//      {
//         std::cerr << "ERROR: Failed to open "<< m_wdpFilename << "\n" ;
//      }
//   }
//   std::ofstream csv060,csv120;
//   std::string strCSV120=i_intsFilename+"120.csv";
//   std::string strCSV060=i_intsFilename+"060.csv";
//   csv060.open(strCSV060.c_str());
//   csv120.open(strCSV120.c_str());
//   std::cout << public_header.number_of_point_records << "\n";
//   unsigned int countIntsSums(0);
//   unsigned count(0);
//   for(unsigned int i=0; i<public_header.number_of_point_records; ++i)
//   {
//      if(i%(int(public_header.number_of_point_records/100))==1)
//      {
//          std::cout << ".";
//      }
//      m_lasfile.read((char *) &point_info, (int) public_header.point_data_record_length);
//      int wave_offset = public_header.start_of_wf_data_Packet_record +
//               point_info.byte_offset_to_wf_packet_data;
//      if((int)point_info.classification!=7)
//      {
//         if(wv_info.size()==0)
//         {
//            std::cout << "ERROR: no wf descriptor exist in the vlr\n";
//            return;
//         }

//         LAS1_3Types::WF_packet_Descriptor *currentDescpriptor = NULL;
//         if(wv_info.size()==1)
//         {
//            currentDescpriptor = wv_info[0];
//         }
//         else
//         {
//            for(unsigned int i=0; i< wv_info.size();++i)
//            {
//               if(point_info.wave_packet_descriptor_index == wv_info[i]->id)
//               {
//                  currentDescpriptor = wv_info[i];
//               }
//            }
//         }

//         if(currentDescpriptor==NULL || point_info.wave_packet_descriptor_index==0)
//         {
//             continue;
//         }

//         if((unsigned int)(point_info.returnNo_noOfRe_scanDirFla_EdgeFLn&7)==1  &&
//             point_info.wf_packet_size_in_bytes == currentDescpriptor->bits_per_sample*currentDescpriptor->number_of_samples/8 &&
//             wave_offset+point_info.wf_packet_size_in_bytes<=m_wvFileLenght)

//         {
//             std::cout << point_info.X<< " > " <<userLimits[3]-0.0001  << " && "
//                       << point_info.X<< " < " << userLimits[2]+0.0001 << " && "
//                       << point_info.Y<< " > " << userLimits[1]-0.0001 << " && "
//                       << point_info.Y<< " < " << userLimits[0]-0.0001 << " " << (point_info.X > userLimits[3]-0.0001 && point_info.X < userLimits[2]+0.0001
//                     && point_info.Y > userLimits[1]-0.0001 && point_info.Y < userLimits[0]-0.0001) << "\n";

//             if (point_info.X > userLimits[3]-0.0001 && point_info.X < userLimits[2]+0.0001
//                     && point_info.Y > userLimits[1]-0.0001 && point_info.Y < userLimits[0]-0.0001)
//             {
//           count++;
//             }
//           char *wave_data = new (std::nothrow) char [point_info.wf_packet_size_in_bytes];
//            if(wave_data==0)
//            {
//                std::cerr << "Fail assigning memory in file Las1_3_handler.cpp\n"
//                          << "Program will terminate\n";
//                exit(EXIT_FAILURE);
//            }
//            int tmp(0);
//            if(m_areWFInternal)
//            {
//               tmp = m_lasfile.tellg();
//               m_lasfile.seekg(wave_offset);
//               m_lasfile.read((char *) wave_data,point_info.wf_packet_size_in_bytes);
//            }
//            else
//            {
//               m_wdpFile.seekg(wave_offset);
//               m_wdpFile.read((char *) wave_data,point_info.wf_packet_size_in_bytes);
//            }

//            unsigned int noOfSamples = currentDescpriptor->number_of_samples;
//            if(currentDescpriptor->bits_per_sample==8)
//            {
//               unsigned char *waveSamplesIntensities = new (std::nothrow) unsigned char
//                        [noOfSamples*currentDescpriptor->bits_per_sample/8];
//               if(waveSamplesIntensities==0)
//               {
//                   std::cerr << "Error: Memory could not be allocated in file Pulse.cpp\n";
//                  exit(EXIT_FAILURE);
//               }
//               memcpy(waveSamplesIntensities,wave_data,currentDescpriptor->number_of_samples);
//               long int sumOfInts(0);
//               if (count%1000==0 && point_info.X > userLimits[3]-0.0001 && point_info.X < userLimits[2]+0.0001
//                       && point_info.Y > userLimits[1]-0.0001 && point_info.Y < userLimits[0]-0.0001)
//               {
//                  for(unsigned int j=0; j< noOfSamples; ++j)
//                  {
//                     // save samples to csv
//                     sumOfInts+=(int(waveSamplesIntensities[j]));
//                  }
//              // std::cout << noOfSamples << " : " << sumOfInts <<" " << int(waveSamplesIntensities[30])<< "\n";
//                   csv060 << sumOfInts <<",";
//                   countIntsSums++;
//               }
//               delete []waveSamplesIntensities;


//            }
//            else if(currentDescpriptor->bits_per_sample==16)
//            {
//                unsigned short int *waveSamplesIntensities = new (std::nothrow) unsigned short int
//                         [noOfSamples];
//                if(waveSamplesIntensities==0)
//                {
//                    std::cerr << "Error: Memory could not be allocated in file Pulse.cpp\n";
//                   exit(EXIT_FAILURE);
//                }
//                memcpy(waveSamplesIntensities,wave_data,currentDescpriptor->number_of_samples*
//                        currentDescpriptor->bits_per_sample/8);
//                unsigned long int sumOfInts(0);
//                unsigned long int sumOfIntsNL(0);
//                unsigned int sumOfOverNL(0);
//                if (count%1000==0 && point_info.X > userLimits[3]-0.0001 && point_info.X < userLimits[2]+0.0001
//                        && point_info.Y > userLimits[1]-0.0001 && point_info.Y < userLimits[0]-0.0001)
//                {
//                   for(unsigned int j=0; j< noOfSamples; ++j)
//                   {
//                      // save samples to csv
//                      if (int(waveSamplesIntensities[j])>i_noiseLevel)
//                      {
//                         sumOfOverNL++;
//                         sumOfIntsNL+=waveSamplesIntensities[j];
//                      }
////                      if (currentDescpriptor->number_of_samples==60)
////                      {
////                        csv060 << int(waveSamplesIntensities[j]) << ",";
////                      }
////                      else
////                      {
////                          csv120 << int(waveSamplesIntensities[j]) << ",";
////                      }
//                      sumOfInts+=(int(waveSamplesIntensities[j]));
//                   }
//                   if (currentDescpriptor->number_of_samples==60)
//                   {
//                       csv060 << sumOfInts <<"," << sumOfOverNL << "," << sumOfIntsNL <<","
//                           << (double(sumOfInts)/double(sumOfOverNL)) <<","
//                           << (double(sumOfIntsNL)/double(sumOfOverNL)) << "\n";
//                   }
//                   else
//                   {
//                    csv120 << sumOfInts <<"," << sumOfOverNL << "," << sumOfIntsNL <<","
//                        << (double(sumOfInts)/double(sumOfOverNL)) <<","
//                        << (double(sumOfIntsNL)/double(sumOfOverNL)) << "\n";
//                   }

//                   if (currentDescpriptor->number_of_samples==60)
//                   {
//                    csv060 << "\n";
//                   }
//                   else
//                   {
//                       csv120 << "\n";
//                   }

//                    countIntsSums++;
//                }
//                delete []waveSamplesIntensities;
//            }
//            if(m_areWFInternal)
//            {
//               m_lasfile.seekg(tmp);
//            }
//            delete []wave_data;
//         }
//      }
//      else
//      {
//         --i;
//      }
//   }
//   csv060.close();
//   csv120.close();

//   m_lasfile.close();
//   std::cout << "CSV " << i_intsFilename << " file saved\n";
//}

//-----------------------------------------------------------------------------
void Las1_3_handler::saveSumIntensity(
        const std::string i_intsFilename,
        double i_noiseLevel,
        const std::vector<double> &userLimits
        )
{
   m_lasfile.open(m_lasFilename.c_str(),std::ios::binary);
   if(!m_lasfile.is_open())
   {
      return;
   }
   if((unsigned short int)(public_header.point_data_format_ID)!=4)
   {
      std::cerr << " ++ ERROR: Not supported LAS file format\n";
      m_lasfile.close();
      return;
   }
   LAS1_3Types::Data_Point_Record_Format_4 point_info;
   m_lasfile.seekg((int) public_header.offset_to_point);
   if(!m_areWFInternal)
   {
      m_wdpFile.open(m_wdpFilename.c_str(),std::ios::binary);
      if(!m_wdpFile)
      {
         std::cerr << "ERROR: Failed to open "<< m_wdpFilename << "\n" ;
      }
   }
   std::vector<std::ofstream *> csvs,csvsRaw;
   std::vector<unsigned int> noSamples;
   std::cout << public_header.number_of_point_records << "\n";
   unsigned int countIntsSums(0);
   for(unsigned int i=0; i<public_header.number_of_point_records; ++i)
   {
      if(i%(int(public_header.number_of_point_records/100))==1)
      {
          std::cout << ".";
      }
      m_lasfile.read((char *) &point_info, (int) public_header.point_data_record_length);
      int wave_offset = public_header.start_of_wf_data_Packet_record +
               point_info.byte_offset_to_wf_packet_data;
      if((int)point_info.classification!=7)
      {
         if(wv_info.size()==0)
         {
            std::cout << "ERROR: no wf descriptor exist in the vlr\n";
            return;
         }

         LAS1_3Types::WF_packet_Descriptor *currentDescpriptor = NULL;
         if(wv_info.size()==1)
         {
            currentDescpriptor = wv_info[0];
         }
         else
         {
            for(unsigned int i=0; i< wv_info.size();++i)
            {
               if(point_info.wave_packet_descriptor_index == wv_info[i]->id)
               {
                  currentDescpriptor = wv_info[i];
               }
            }
         }
         if(currentDescpriptor==NULL || point_info.wave_packet_descriptor_index==0)
         {
             continue;
         }
//         csv060.open(i_intsFilename+"120.csv");
         unsigned int cNS=0;
         for(;cNS<noSamples.size();++cNS)
         {
            if (noSamples[cNS]==currentDescpriptor->number_of_samples)
            {
               break;
            }
         }
         assert(cNS<=noSamples.size() && noSamples.size() == csvs.size() && csvs.size()==csvsRaw.size());
         if (cNS==noSamples.size())
         {
            noSamples.push_back(currentDescpriptor->number_of_samples);
            csvs.push_back(new std::ofstream);
            csvsRaw.push_back(new std::ofstream);
            std::string tmpFileName(i_intsFilename+std::to_string(noSamples[cNS])+".csv");
            csvs[cNS]->open(tmpFileName);
            tmpFileName = i_intsFilename+std::to_string(noSamples[cNS])+"_raw.csv";
            csvsRaw[cNS]->open(tmpFileName);
            *csvs[cNS] << "SumofInts,NoOfInts,SumofIntsAboveNL,NofOfIntsAboveNL,SumofInts/NoOfInts,SumofIntsAboveNL/NofOfIntsAboveNL\n";
            std::cout << "New File " << tmpFileName << " created\n";
            std::cout << "currentDescpriptor->number_of_samples currentDescpriptor->bits_per_sample: \n - - - ";
            std::cout << currentDescpriptor->number_of_samples << " " << int(currentDescpriptor->bits_per_sample) <<"\n";

         }

         // 104313 104155 437805 437652 ground
         // 103176 102965 437551 437332 trees
         if((unsigned int)(point_info.returnNo_noOfRe_scanDirFla_EdgeFLn&7)==1  &&
             point_info.wf_packet_size_in_bytes == currentDescpriptor->bits_per_sample*currentDescpriptor->number_of_samples/8 &&
             wave_offset+point_info.wf_packet_size_in_bytes<=m_wvFileLenght


              &&  (point_info.X*public_header.x_scale_factor+public_header.x_offset > userLimits[3]-0.0001
                   && point_info.X*public_header.x_scale_factor+public_header.x_offset < userLimits[2]+0.0001
                  && point_info.Y*public_header.y_scale_factor+public_header.y_offset > userLimits[1]-0.0001
                   && point_info.Y*public_header.y_scale_factor+public_header.y_offset < userLimits[0]-0.0001)
//             //  sample
//          &&  (point_info.X > userLimits[3]-0.0001 && point_info.X < userLimits[2]+0.0001
//              && point_info.Y > userLimits[1]-0.0001 && point_info.Y < userLimits[0]-0.0001)
                 )
         {
           char *wave_data = new (std::nothrow) char [point_info.wf_packet_size_in_bytes];
            if(wave_data==0)
            {
                std::cerr << "Fail assigning memory in file Las1_3_handler.cpp\n"
                          << "Program will terminate\n";
                exit(EXIT_FAILURE);
            }
            int tmp(0);
            if(m_areWFInternal)
            {
               tmp = m_lasfile.tellg();
               m_lasfile.seekg(wave_offset);
               m_lasfile.read((char *) wave_data,point_info.wf_packet_size_in_bytes);
            }
            else
            {
               m_wdpFile.seekg(wave_offset);
               m_wdpFile.read((char *) wave_data,point_info.wf_packet_size_in_bytes);
            }

            unsigned int noOfSamples = currentDescpriptor->number_of_samples;
            if(currentDescpriptor->bits_per_sample==8)
            {
               unsigned char *waveSamplesIntensities = new (std::nothrow) unsigned char
                        [noOfSamples*currentDescpriptor->bits_per_sample/8];
               if(waveSamplesIntensities==0)
               {
                   std::cerr << "Error: Memory could not be allocated in file Pulse.cpp\n";
                  exit(EXIT_FAILURE);
               }
               memcpy(waveSamplesIntensities,wave_data,currentDescpriptor->number_of_samples*
                       currentDescpriptor->bits_per_sample/8);
               unsigned long int sumOfInts(0);
               unsigned long int sumOfIntsNL(0);
               unsigned int sumOfOverNL(0);
//               if (countIntsSums%50==0)
               {
                  for(unsigned int j=0; j< noOfSamples; ++j)
                  {
                     // save samples to csv
                     if (int(waveSamplesIntensities[j])>i_noiseLevel)
                     {
                        sumOfOverNL++;
                        sumOfIntsNL+=waveSamplesIntensities[j];
                     }
                     *csvsRaw[cNS] <<  int(waveSamplesIntensities[j]) << ",";
                     sumOfInts+=(int(waveSamplesIntensities[j]));
                  }
                  *csvs[cNS] << /* sumOfInts <<"," <<*/ currentDescpriptor->number_of_samples<< ","<< sumOfIntsNL <<","<<sumOfOverNL << ","
                          << (double(sumOfInts)/double(currentDescpriptor->number_of_samples)) <<","
                          << (double(sumOfIntsNL)/double(sumOfOverNL)) << "\n";
                  *csvsRaw[cNS] << "\n";
                   countIntsSums++;
               }
               delete []waveSamplesIntensities;


            }
            else if(currentDescpriptor->bits_per_sample==16)
            {
                unsigned short int *waveSamplesIntensities = new (std::nothrow) unsigned short int
                         [noOfSamples];
                if(waveSamplesIntensities==0)
                {
                    std::cerr << "Error: Memory could not be allocated in file Pulse.cpp\n";
                   exit(EXIT_FAILURE);
                }
                memcpy(waveSamplesIntensities,wave_data,currentDescpriptor->number_of_samples*
                        currentDescpriptor->bits_per_sample/8);
                unsigned long int sumOfInts(0);
                unsigned long int sumOfIntsNL(0);
                unsigned int sumOfOverNL(0);
//                if (countIntsSums%50==0)
                {
                   for(unsigned int j=0; j< noOfSamples; ++j)
                   {
                      // save samples to csv
                      if (int(waveSamplesIntensities[j])>i_noiseLevel)
                      {
                         sumOfOverNL++;
                         sumOfIntsNL+=waveSamplesIntensities[j];
                      }
                      *csvsRaw[cNS] <<  int(waveSamplesIntensities[j]) << ",";
                      sumOfInts+=(int(waveSamplesIntensities[j]));
                   }

                   *csvs[cNS] << sumOfInts <<"," ;/*<< currentDescpriptor->number_of_samples<< ","<< sumOfIntsNL <<","<<sumOfOverNL << ","
                           << (double(sumOfInts)/double(currentDescpriptor->number_of_samples)) <<","
                           << (double(sumOfIntsNL)/double(sumOfOverNL)) << "\n";*/
                   *csvsRaw[cNS] << "\n";
                    countIntsSums++;
                }
                delete []waveSamplesIntensities;
            }
            if(m_areWFInternal)
            {
               m_lasfile.seekg(tmp);
            }
            delete []wave_data;
         }
      }
      else
      {
         --i;
      }

   }

   assert(csvs.size()==csvsRaw.size());
   for(unsigned int ci=0; ci<csvs.size();++ci)
   {
      csvs[ci]->close();
      csvsRaw[ci]->close();
      delete csvs[ci];
      delete csvsRaw[ci];
   }

   m_lasfile.close();
   std::cout << "CSV " << i_intsFilename << " file saved\n";
}



//-----------------------------------------------------------------------------
void Las1_3_handler::saveSamples(
        const std::string &i_csvFileName,
        unsigned int i_noOfSamples
        )
{
   m_lasfile.open(m_lasFilename.c_str(),std::ios::binary);
   if(!m_lasfile.is_open())
   {
      return;
   }
   if((unsigned short int)(public_header.point_data_format_ID)!=4)
   {
      std::cerr << "ERROR: Not supported LAS file format\n";
      m_lasfile.close();
      return;
   }
   LAS1_3Types::Data_Point_Record_Format_4 point_info;
   m_lasfile.seekg((int) public_header.offset_to_point);
   if(!m_areWFInternal)
   {
      m_wdpFile.open(m_wdpFilename.c_str(),std::ios::binary);
      if(!m_wdpFile)
      {
         std::cerr << "ERROR: Failed to open "<< m_wdpFilename << "\n" ;
      }
   }
   std::ofstream csv;
   csv.open(i_csvFileName.c_str());
   csv << "Amplitude\n";
   for(unsigned int i=0; i<i_noOfSamples &&
       i<public_header.number_of_point_records; ++i)
   {
      m_lasfile.read((char *) &point_info, (int) public_header.point_data_record_length);
      int wave_offset = public_header.start_of_wf_data_Packet_record +
               point_info.byte_offset_to_wf_packet_data;
      if((int)point_info.classification!=7)
      {
         if(wv_info.size()==0)
         {
            std::cout << "ERROR: no wf descriptor exist in the vlr\n";
            return;
         }

         LAS1_3Types::WF_packet_Descriptor *currentDescpriptor = NULL;
         if(wv_info.size()==1)
         {
            currentDescpriptor = wv_info[0];
         }
         else
         {
            for(unsigned int i=0; i< wv_info.size();++i)
            {
               if(point_info.wave_packet_descriptor_index == wv_info[i]->id)
               {
                  currentDescpriptor = wv_info[i];
               }
            }
         }

         if(currentDescpriptor==NULL || point_info.wave_packet_descriptor_index==0)
         {
            break;
         }

         if((unsigned int)(point_info.returnNo_noOfRe_scanDirFla_EdgeFLn&7)==1  &&
             point_info.wf_packet_size_in_bytes == currentDescpriptor->bits_per_sample*currentDescpriptor->number_of_samples/8 &&
             wave_offset+point_info.wf_packet_size_in_bytes<=m_wvFileLenght)
         {

            char *wave_data = new (std::nothrow) char [point_info.wf_packet_size_in_bytes];
            if(wave_data==0)
            {
                std::cerr << "Fail assigning memory in file Las1_3_handler.cpp\n"
                          << "Program will terminate\n";
                exit(EXIT_FAILURE);
            }
            int tmp(0);
            if(m_areWFInternal)
            {
               tmp = m_lasfile.tellg();
               m_lasfile.seekg(wave_offset);
               m_lasfile.read((char *) wave_data,point_info.wf_packet_size_in_bytes);
            }
            else
            {
               m_wdpFile.seekg(wave_offset);
               m_wdpFile.read((char *) wave_data,point_info.wf_packet_size_in_bytes);
            }

            unsigned int noOfSamples = currentDescpriptor->number_of_samples;
            if(currentDescpriptor->bits_per_sample==8)
            {
               unsigned char *waveSamplesIntensities = new (std::nothrow) unsigned char
                        [noOfSamples*currentDescpriptor->bits_per_sample/8];
               if(waveSamplesIntensities==0)
               {
                   std::cerr << "Error: Memory could not be allocated in file Pulse.cpp\n";
                  exit(EXIT_FAILURE);
               }
               memcpy(waveSamplesIntensities,wave_data,currentDescpriptor->number_of_samples);
               for(unsigned int j=0; j< noOfSamples; ++j)
               {
                  // save samples to csv
                  csv << int(waveSamplesIntensities[j]) <<"\n";
               }
               delete []waveSamplesIntensities;

            }
            else if(currentDescpriptor->bits_per_sample==16)
            {
                unsigned short int *waveSamplesIntensities = new (std::nothrow) unsigned short int
                         [noOfSamples];
                if(waveSamplesIntensities==0)
                {
                    std::cerr << "Error: Memory could not be allocated in file Pulse.cpp\n";
                   exit(EXIT_FAILURE);
                }
                memcpy(waveSamplesIntensities,wave_data,currentDescpriptor->number_of_samples*
                        currentDescpriptor->bits_per_sample/8);
                for(unsigned int j=0; j< noOfSamples; ++j)
                {
                   // save samples to .csv
                   csv << int(waveSamplesIntensities[j]) <<"\n";
                }
                delete []waveSamplesIntensities;
            }
            if(m_areWFInternal)
            {
               m_lasfile.seekg(tmp);
            }
            delete []wave_data;
         }
      }
      else
      {
         --i;
      }
   }
   csv.close();

   m_lasfile.close();
   std::cout << "CSV " << i_csvFileName << " file saved\n";
}
//-----------------------------------------------------------------------------
std::vector<double> Las1_3_handler::calBoundaries()
{
   m_lasfile.open(m_lasFilename.c_str(),std::ios::binary);
   if(!m_lasfile.is_open())
   {
      std::cerr << "File not found \n";
      std::vector<double> empty(8,0);
      return empty;
   }
   read_public_header();
   std::vector<double> boundaries(6);
   double minx(0.0),maxx(0.0),miny(0.0),maxy(0.0),minz(0.0),maxz(0.0);

   if((unsigned short int)(public_header.point_data_format_ID)!=2)
   {
      std::cout << "calBoundaries only supported for LAS1.2 \n";
      LAS1_3Types::Data_Point_Record_Format_2 point_info;
      m_lasfile.seekg((int) public_header.offset_to_point);
      if (public_header.number_of_point_records>0)
      {
         m_lasfile.read((char *) &point_info, (int) public_header.point_data_record_length);
         if((int)point_info.classification!=7)
         {
            // All Discrete Points
             if((int)point_info.classification!=7)
             {
                gmtl::Vec3f xyz(point_info.X*public_header.x_scale_factor,
                                           point_info.Y*public_header.y_scale_factor,
                                           point_info.Z*public_header.z_scale_factor);
                std::cout << point_info.X << " " <<public_header.x_scale_factor<< "\n";
                std::cout << xyz[0] << " " << xyz[1] << " " << xyz[2] << "\n";
                minx=xyz[0]; maxx=xyz[0];
                miny=xyz[1]; maxy=xyz[1];
                minz=xyz[2]; maxz=xyz[2];

             }
         }
      }
      for(unsigned int i=1; i< public_header.number_of_point_records; ++i)
      {
         m_lasfile.read((char *) &point_info, (int) public_header.point_data_record_length);
         if((int)point_info.classification!=7)
         {

            // All Discrete Points
             if((int)point_info.classification!=7)
             {
                 gmtl::Vec3f xyz(point_info.X*public_header.x_scale_factor,
                                            point_info.Y*public_header.y_scale_factor,
                                            point_info.Z*public_header.z_scale_factor);
                 minx=(minx>xyz[0])?xyz[0]:minx; maxx=(maxx<xyz[0])?xyz[0]:maxx;
                 miny=(miny>xyz[1])?xyz[1]:miny; maxy=(maxy<xyz[1])?xyz[1]:maxy;
                 minz=(minz>xyz[2])?xyz[2]:minz; maxz=(maxz<xyz[2])?xyz[2]:maxz;
             }
         }
         else
         {
             // only noise has been recorded
         }
      }
   }


   // [MaxNorthY, MinNorthY, MaxEastX, MinEastX, MaxHeightZ, MinHeightZ]
   boundaries[0] = maxy;
   boundaries[1] = miny;
   boundaries[2] = maxx;
   boundaries[3] = minx;
   boundaries[4] = maxz;
   boundaries[5] = minz;
   m_lasfile.close();
   return boundaries;
}

//-----------------------------------------------------------------------------
std::vector<double> Las1_3_handler::getBoundaries()
{
   m_lasfile.open(m_lasFilename.c_str(),std::ios::binary);
   if(!m_lasfile.is_open())
   {
      std::cerr << "File not found \n";
      std::vector<double> empty(8,0);
      return empty;
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
   m_lasfile.close();
   return boundaries;
}


bool Las1_3_handler::isBigEndian()
{
    union {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1;
}


//-----------------------------------------------------------------------------
void Las1_3_handler::read_point_record_format_4(Volume *i_obj,
        const std::string i_type,
        const std::string &i_dtm
        )
{
   DtmBilReader dtm(i_dtm,i_obj);
   LAS1_3Types::Data_Point_Record_Format_4 point_info;
   m_lasfile.seekg((int) public_header.offset_to_point);

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
   types["AGC"] = 5;

   if(!m_areWFInternal)
   {
      m_wdpFile.open(m_wdpFilename.c_str(),std::ios::binary);
      if(!m_wdpFile)
      {
         std::cerr << "ERROR: Failed to open "<< m_wdpFilename << "\n" ;
      }
   }

   std::string s(i_type);
   std::transform(s.begin(), s.end(), s.begin(), toupper);

   unsigned int percentage = public_header.number_of_point_records / 10;
   std::cout << "Progress on reading " << public_header.number_of_point_records << " pulses:\n";

   for(unsigned int i=0; i< public_header.number_of_point_records; ++i)
   {
      if(i%percentage == 0)
      {
         std::cout << i/percentage*10 <<  "% ...";
      }
      m_lasfile.read((char *) &point_info, (int) public_header.point_data_record_length);
      int wave_offset = public_header.start_of_wf_data_Packet_record +
               point_info.byte_offset_to_wf_packet_data;

      if((int)point_info.classification!=7)
      {
         // TYPES:
         // 1. Full-waveform
         // 2. All_Discrete
         // 3. Discrete_n_Waveforms
         // 4. Discrete (associated with waveform only
         // 5. AGC values, which are saved in the point_ID

         // find the corresponding discription
         if(wv_info.size()==0)
         {
            std::cout << "ERROR: no wf descriptor exist in the vlr\n";
            return;
         }

         switch(types[s])
         {
         // Waveform samples only
         case 1:
         {
             LAS1_3Types::WF_packet_Descriptor *currentDescpriptor = NULL;
             if(wv_info.size()==1)
             {
                currentDescpriptor = wv_info[0];
             }
             else
             {
                for(unsigned int i=0; i< wv_info.size();++i)
                {
                   if(point_info.wave_packet_descriptor_index == wv_info[i]->id)
                   {
                      currentDescpriptor = wv_info[i];
                   }
                }
             }
             if(currentDescpriptor==NULL || point_info.wave_packet_descriptor_index==0)
             {
                break;
             }

            if((unsigned int)(point_info.returnNo_noOfRe_scanDirFla_EdgeFLn&7)==1  &&
                    point_info.wf_packet_size_in_bytes == currentDescpriptor->bits_per_sample*currentDescpriptor->number_of_samples/8 &&
                    wave_offset+point_info.wf_packet_size_in_bytes<=m_wvFileLenght)
            {
               count++;
               char *wave_data = new (std::nothrow) char [point_info.wf_packet_size_in_bytes];
               if(wave_data==0)
               {
                   std::cerr << "Fail assigning memory in file Las1_3_handler.cpp\n"
                             << "Program will terminate\n";
                   exit(EXIT_FAILURE);
               }
               unsigned long long int tmp(0);
               if(m_areWFInternal)
               {
                  tmp = m_lasfile.tellg();
                  m_lasfile.seekg(wave_offset);
                  m_lasfile.read((char *) wave_data,point_info.wf_packet_size_in_bytes);
               }
               else
               {
                  m_wdpFile.seekg(wave_offset);
                  m_wdpFile.read((char *) wave_data,point_info.wf_packet_size_in_bytes);
               }

               gmtl::Vec3f origin (point_info.X*public_header.x_scale_factor+public_header.x_offset,
                                   point_info.Y*public_header.y_scale_factor+public_header.y_offset,
                                   point_info.Z*public_header.z_scale_factor+public_header.z_offset);

               float temporalSampleSpacing = ((int)currentDescpriptor->temporal_sample_spacing)/1000.0f;
               gmtl::Vec3f offset= gmtl::Vec3f(point_info.X_t, point_info.Y_t, point_info.Z_t);
               offset *= (1000.0f * temporalSampleSpacing);

               origin[0] = origin[0] + (double )point_info.X_t*
                       (double )point_info.return_point_wf_location;
               origin[1] = origin[1] + (double )point_info.Y_t*
                       (double )point_info.return_point_wf_location;
               origin[2] = origin[2] + (double )point_info.Z_t*
                       (double )point_info.return_point_wf_location;

               unsigned int noOfSamples = currentDescpriptor->number_of_samples;



               gmtl::Vec3f *tempPosition=new gmtl::Vec3f;
               (*tempPosition)[0]=origin[0]; (*tempPosition)[1]=origin[1]; (*tempPosition)[2]=origin[2];

               if(currentDescpriptor->bits_per_sample==8)
               {
                  unsigned char *waveSamplesIntensities = new (std::nothrow) unsigned char
                           [noOfSamples*currentDescpriptor->bits_per_sample/8];
                  if(waveSamplesIntensities==0)
                  {
                      std::cerr << "Error: Memory could not be allocated in file Pulse.cpp\n";
                     exit(EXIT_FAILURE);
                  }
                  memcpy(waveSamplesIntensities,wave_data,currentDescpriptor->number_of_samples);
                  for(unsigned int j=0; j< noOfSamples; ++j)
                  {
                    gmtl::Vec3f *point=new gmtl::Vec3f;
                    (*point)[0]=(*tempPosition)[0]; (*point)[1]=(*tempPosition)[1]; (*point)[2]=(*tempPosition)[2];
                    std::cout << point[2] ;
                    (*point)[2]-=dtm.getHeightOf((*tempPosition)[0],(*tempPosition)[1]);
                    std::cout << " - " << tempPosition[2] << " = " << point[2] << "\n";
                    i_obj->addItensity((*point),waveSamplesIntensities[j]);
                    (*tempPosition)+=offset;
                    delete point;
                  }
                  delete []waveSamplesIntensities;

               }
               else if(currentDescpriptor->bits_per_sample==16)
               {
                   unsigned short int *waveSamplesIntensities = new (std::nothrow) unsigned short int
                            [noOfSamples];
                   if(waveSamplesIntensities==0)
                   {
                       std::cerr << "Error: Memory could not be allocated in file Pulse.cpp\n";
                      exit(EXIT_FAILURE);
                   }
                   memcpy(waveSamplesIntensities,wave_data,currentDescpriptor->number_of_samples*
                           currentDescpriptor->bits_per_sample/8);
                   for(unsigned int j=0; j< noOfSamples; ++j)
                   {
                     gmtl::Vec3f *point=new gmtl::Vec3f;
                     (*point)[0]=(*tempPosition)[0]; (*point)[1]=(*tempPosition)[1]; (*point)[2]=(*tempPosition)[2];
//                     std::cout << "**************************\n";
//                     std::cout << (*point)[0] << " " << (*point)[1] << " " << (*point)[2] << "\n";

                     double xtemp=dtm.getHeightOf((*point)[0],(*point)[1]);
                     if (xtemp>0.01)
                     {
                        (*point)[2]-=xtemp;//dtm.getHeightOf(tempPosition[0],tempPosition[1]);
//                        std::cout <<  " - " << xtemp << " = " << (*point)[2] << "\n";
//                        std::cout <<"* "<< (*point)[2] << " *\n";
                     }
//                     else
//                     {
//                      std::cout << (*point)[2] << " * " << xtemp << " * ";
//                     }

                     i_obj->addItensity((*point),waveSamplesIntensities[j]);
                     (*tempPosition)+=offset;
                     delete  point;
                   }
                   delete []waveSamplesIntensities;
               }
               delete  tempPosition;
               if(m_areWFInternal)
               {
                  m_lasfile.seekg(tmp);
               }
               delete []wave_data;
            }
            break;
         }
         // All Discrete Points
         case 2:
            if((int)point_info.classification!=7)
            {
               i_obj->addItensity(gmtl::Vec3f(point_info.X*public_header.x_scale_factor,
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
               i_obj->addItensity(gmtl::Vec3f(point_info.X*public_header.x_scale_factor,
                                            point_info.Y*public_header.y_scale_factor,
                                            point_info.Z*public_header.z_scale_factor),
                                point_info.itensity);
            }
            break;
         case 5:
            if((int)point_info.classification!=7)
            {
               i_obj->addItensity(gmtl::Vec3f(point_info.X*public_header.x_scale_factor,
                                         point_info.Y*public_header.y_scale_factor,
                                         point_info.Z*public_header.z_scale_factor),
                             point_info.gain);
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
   std::cout << "\n";

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
   if(!m_areWFInternal)
   {
      m_wdpFile.close();
   }

}

//-----------------------------------------------------------------------------
void Las1_3_handler::read_point_record_format_4(Volume *i_obj,
        const std::string i_type
        )
{

   LAS1_3Types::Data_Point_Record_Format_4 point_info;
   m_lasfile.seekg((int) public_header.offset_to_point);

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
   types["AGC"] = 5;

   if(!m_areWFInternal)
   {
      m_wdpFile.open(m_wdpFilename.c_str(),std::ios::binary);
      if(!m_wdpFile)
      {
         std::cerr << "ERROR: Failed to open "<< m_wdpFilename << "\n" ;
      }
   }

   std::string s(i_type);
   std::transform(s.begin(), s.end(), s.begin(), toupper);

   unsigned int percentage = public_header.number_of_point_records / 10;
   std::cout << "Progress on reading " << public_header.number_of_point_records << " pulses:\n";

   for(unsigned int i=0; i< public_header.number_of_point_records; ++i)
   {
      if(i%percentage == 0)
      {
         std::cout << i/percentage*10 <<  "% ...";
      }
      m_lasfile.read((char *) &point_info, (int) public_header.point_data_record_length);
      int wave_offset = public_header.start_of_wf_data_Packet_record +
               point_info.byte_offset_to_wf_packet_data;

      if((int)point_info.classification!=7)
      {
         // TYPES:
         // 1. Full-waveform
         // 2. All_Discrete
         // 3. Discrete_n_Waveforms
         // 4. Discrete (associated with waveform only
         // 5. AGC values, which are saved in the point_ID

         // find the corresponding discription
         if(wv_info.size()==0)
         {
            std::cout << "ERROR: no wf descriptor exist in the vlr\n";
            return;
         }
         switch(types[s])
         {
         // Waveform samples only
         case 1:
         {
            LAS1_3Types::WF_packet_Descriptor *currentDescpriptor = NULL;
            if(wv_info.size()==1)
            {
               currentDescpriptor = wv_info[0];
            }
            else
            {
               for(unsigned int i=0; i< wv_info.size();++i)
               {
                  if(point_info.wave_packet_descriptor_index == wv_info[i]->id)
                  {
                     currentDescpriptor = wv_info[i];
                  }
               }
            }

            if(currentDescpriptor==NULL || point_info.wave_packet_descriptor_index==0)
            {
               break;
            }

            if((unsigned int)(point_info.returnNo_noOfRe_scanDirFla_EdgeFLn&7)==1  &&
                point_info.wf_packet_size_in_bytes == currentDescpriptor->bits_per_sample*currentDescpriptor->number_of_samples/8 &&
                wave_offset+point_info.wf_packet_size_in_bytes<=m_wvFileLenght)
            {

               count++;
               char *wave_data = new (std::nothrow) char [point_info.wf_packet_size_in_bytes];
               if(wave_data==0)
               {
                   std::cerr << "Fail assigning memory in file Las1_3_handler.cpp\n"
                             << "Program will terminate\n";
                   exit(EXIT_FAILURE);
               }
               long long int tmp(0);
               if(m_areWFInternal)
               {
                  tmp  = m_lasfile.tellg();
                  m_lasfile.seekg(wave_offset);
                  m_lasfile.read((char *) wave_data,point_info.wf_packet_size_in_bytes);
               }
               else
               {
                  m_wdpFile.seekg(wave_offset);
                  m_wdpFile.read((char *) wave_data,point_info.wf_packet_size_in_bytes);
               }

               gmtl::Vec3f origin (point_info.X*public_header.x_scale_factor+public_header.x_offset,
                                   point_info.Y*public_header.y_scale_factor+public_header.y_offset,
                                   point_info.Z*public_header.z_scale_factor+public_header.z_offset);

               float temporalSampleSpacing = ((int)currentDescpriptor->temporal_sample_spacing)/1000.0f;
               gmtl::Vec3f offset= gmtl::Vec3f(point_info.X_t, point_info.Y_t, point_info.Z_t);
               offset *= (1000.0f * temporalSampleSpacing);

               origin[0] = origin[0] + (double )point_info.X_t*
                       (double )point_info.return_point_wf_location;
               origin[1] = origin[1] + (double )point_info.Y_t*
                       (double )point_info.return_point_wf_location;
               origin[2] = origin[2] + (double )point_info.Z_t*
                       (double )point_info.return_point_wf_location;

               unsigned int noOfSamples = currentDescpriptor->number_of_samples;

               double aveIntOfWaveform(0);
               gmtl::Vec3f tempPosition = origin;
               if(currentDescpriptor->bits_per_sample==8)
               {
                  unsigned char *waveSamplesIntensities = new (std::nothrow) unsigned char
                           [noOfSamples*currentDescpriptor->bits_per_sample/8];
                  if(waveSamplesIntensities==0)
                  {
                      std::cerr << "Error: Memory could not be allocated in file Pulse.cpp\n";
                     exit(EXIT_FAILURE);
                  }
                  memcpy(waveSamplesIntensities,wave_data,currentDescpriptor->number_of_samples);
                  assert(noOfSamples!=0);

//                  aveIntOfWaveform=waveSamplesIntensities[0];
//                  for(unsigned int wi=1; wi<noOfSamples; ++wi)
//                  {
//                     aveIntOfWaveform = double(wi)/(double(wi)+1.0)*aveIntOfWaveform
//                             + 1.0/(double(wi)+1.0)*(waveSamplesIntensities[wi]);
//                  }
//                  double constAve(aveIntOfWaveform);
//                  for(unsigned int j=0; j< noOfSamples; ++j)
//                  {
//                    i_obj->addItensity(tempPosition,waveSamplesIntensities[j]
//                                       /aveIntOfWaveform*double(noOfSamples)*constAve);
//                    aveIntOfWaveform=-(1.0/double(noOfSamples))*waveSamplesIntensities[j]
//                           +((double(noOfSamples)-1.0)/double(noOfSamples))*aveIntOfWaveform;
//                    tempPosition+=offset;
//                  }

                  for(unsigned int j=0; j< noOfSamples; ++j)
                  {
                    i_obj->addItensity(tempPosition,waveSamplesIntensities[j]);
                    tempPosition+=offset;
                  }

                  delete []waveSamplesIntensities;

               }
               else if(currentDescpriptor->bits_per_sample==16)
               {
                   unsigned short int *waveSamplesIntensities = new (std::nothrow) unsigned short int
                            [noOfSamples];
                   if(waveSamplesIntensities==0)
                   {
                       std::cerr << "Error: Memory could not be allocated in file Pulse.cpp\n";
                      exit(EXIT_FAILURE);
                   }
                   memcpy(waveSamplesIntensities,wave_data,currentDescpriptor->number_of_samples*
                           currentDescpriptor->bits_per_sample/8);

//                   aveIntOfWaveform=waveSamplesIntensities[0];
//                   for(unsigned int wi=1; wi<noOfSamples; ++wi)
//                   {
//                      aveIntOfWaveform = double(wi)/(double(wi)+1.0)*aveIntOfWaveform
//                              + 1.0/(double(wi)+1.0)*(waveSamplesIntensities[wi]);
//                   }
//                   for(unsigned int j=0; j< noOfSamples; ++j)
//                   {
//                     i_obj->addItensity(tempPosition,waveSamplesIntensities[j]
//                                        /aveIntOfWaveform*noOfSamples);
//                     aveIntOfWaveform=-(1.0/double(noOfSamples))*waveSamplesIntensities[j]
//                            +((double(noOfSamples)-1.0)/double(noOfSamples))*aveIntOfWaveform;
//                     tempPosition+=offset;
//                   }


                   for(unsigned int j=0; j< noOfSamples; ++j)
                   {
                     i_obj->addItensity(tempPosition,waveSamplesIntensities[j]);
                     tempPosition+=offset;
                   }


                   delete []waveSamplesIntensities;
               }
               if(m_areWFInternal)
               {
                 m_lasfile.seekg(tmp);
               }
               delete []wave_data;
            }
            break;
         }
         // All Discrete Points
         case 2:
            if((int)point_info.classification!=7)
            {
               i_obj->addItensity(gmtl::Vec3f(point_info.X*public_header.x_scale_factor,
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
               i_obj->addItensity(gmtl::Vec3f(point_info.X*public_header.x_scale_factor,
                                            point_info.Y*public_header.y_scale_factor,
                                            point_info.Z*public_header.z_scale_factor),
                                point_info.itensity);
            }
            break;
         case 5:
            if((int)point_info.classification!=7)
            {
               i_obj->addItensity(gmtl::Vec3f(point_info.X*public_header.x_scale_factor,
                                         point_info.Y*public_header.y_scale_factor,
                                         point_info.Z*public_header.z_scale_factor),
                             point_info.gain);
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
   std::cout << "\n";

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
   if(!m_areWFInternal)
   {
      m_wdpFile.close();
   }

}


//-----------------------------------------------------------------------------
void Las1_3_handler::read_point_record_format_0(Volume *i_obj
        )
{
    LAS1_3Types::Data_Point_Record_Format_0 point_info;
    m_lasfile.seekg((int) public_header.offset_to_point);
    for(unsigned int i=0; i< public_header.number_of_point_records; ++i)
    {
       m_lasfile.read((char *) &point_info, (int) public_header.point_data_record_length);
       if((int)point_info.classification!=7)
       {

          // All Discrete Points
           if((int)point_info.classification!=7)
           {
              i_obj->addItensity(gmtl::Vec3f(point_info.X*public_header.x_scale_factor,
                                        point_info.Y*public_header.y_scale_factor,
                                        point_info.Z*public_header.z_scale_factor),
                            point_info.itensity);
           }
       }
       else
       {
           // only noise has been recorded
       }

    }
}


//-----------------------------------------------------------------------------
void Las1_3_handler::read_point_record_format_1(Volume *i_obj
        )
{
    LAS1_3Types::Data_Point_Record_Format_1 point_info;
    m_lasfile.seekg((int) public_header.offset_to_point);
    for(unsigned int i=0; i< public_header.number_of_point_records; ++i)
    {
       m_lasfile.read((char *) &point_info, (int) public_header.point_data_record_length);
       if((int)point_info.classification!=7)
       {

          // All Discrete Points
           if((int)point_info.classification!=7)
           {
              i_obj->addItensity(gmtl::Vec3f(point_info.X*public_header.x_scale_factor+public_header.x_offset,
                                        point_info.Y*public_header.y_scale_factor+public_header.y_offset,
                                        point_info.Z*public_header.z_scale_factor+public_header.z_offset),
                            point_info.itensity);
           }
       }
       else
       {
           // only noise has been recorded
       }

    }
}

//-----------------------------------------------------------------------------
void Las1_3_handler::read_point_record_format_2(Volume *i_obj
        )
{
    std::cout << "Reading point data record format 2\n";
    LAS1_3Types::Data_Point_Record_Format_2 point_info;
    m_lasfile.seekg((int) public_header.offset_to_point);
    for(unsigned int i=0; i< public_header.number_of_point_records; ++i)
    {
       m_lasfile.read((char *) &point_info, (int) public_header.point_data_record_length);
       if((int)point_info.classification!=7)
       {
          // All Discrete Points
           if((int)point_info.classification!=7)
           {
              i_obj->addItensity(gmtl::Vec3f(point_info.X*public_header.x_scale_factor,
                                        point_info.Y*public_header.y_scale_factor,
                                        point_info.Z*public_header.z_scale_factor),
                            point_info.itensity);

           }
       }
       else
       {
           // only noise has been recorded
       }

    }
}


//-----------------------------------------------------------------------------
void Las1_3_handler::read_point_record_format_3(Volume *i_obj
        )
{
    std::cout << "Reading point data record format 3\n";
    LAS1_3Types::Data_Point_Record_Format_3 point_info;
    m_lasfile.seekg((int) public_header.offset_to_point);
    for(unsigned int i=0; i< public_header.number_of_point_records; ++i)
    {
       m_lasfile.read((char *) &point_info, (int) public_header.point_data_record_length);
       if((int)point_info.classification!=7)
       {
          // All Discrete Points
           if((int)point_info.classification!=7)
           {
              i_obj->addItensity(gmtl::Vec3f(point_info.X*public_header.x_scale_factor,
                                        point_info.Y*public_header.y_scale_factor,
                                        point_info.Z*public_header.z_scale_factor),
                            point_info.itensity);
           }
       }
       else
       {
           // only noise has been recorded
       }

    }
}


//-----------------------------------------------------------------------------
void Las1_3_handler::readFileAndGetObject(
        Volume *i_obj,
        const std::string i_type,
        const std::string &i_dtm
        )
{
   m_lasfile.open(m_lasFilename.c_str(),std::ios::binary);
   if(!m_lasfile.is_open())
   {
      std::cerr << "File not found \n";
      exit(EXIT_FAILURE);
   }
   read_public_header();
   read_variable_length_records();

   if(i_obj==0)
   {
      std::cerr << "Error: Memory could not be allocated\n";
      exit(EXIT_FAILURE);
   }

   if((unsigned short int)(public_header.point_data_format_ID)==4)
   {
      if(i_dtm=="")
      {
         read_point_record_format_4(i_obj,i_type);
      }
      else
      {
         read_point_record_format_4(i_obj,i_type,i_dtm);
      }
   }
   else if((unsigned short int)(public_header.point_data_format_ID)==0)
   {
      std::cout << "Warning:: Type ingored only discrete data exist in the LAS1.0 file \n";
      read_point_record_format_0(i_obj);

   }
   else if((unsigned short int)(public_header.point_data_format_ID)==1)
   {
      std::cout << "Warning: Type ingored only discrete data exist in the LAS1.1 file\n";
      read_point_record_format_1(i_obj);
   }
   else if((unsigned short int)(public_header.point_data_format_ID)==2)
   {
      std::cout << "Warning: Type ingored only discrete data exist in the LAS1.2 file\n";
      read_point_record_format_2(i_obj);
   }
   else if((unsigned short int)(public_header.point_data_format_ID)==3)
   {
      std::cout << "Warning: Type ingored only discrete data exist in the LAS1.3 file\n";
      read_point_record_format_3(i_obj);
   }
   m_lasfile.close();
}


//-----------------------------------------------------------------------------
void Las1_3_handler::read_public_header()
{
   m_lasfile.read((char *) &public_header,sizeof(public_header));

   // tests if waveform data packets are saved into that file
   // if not program terminates because it cannot handle that case
   if((public_header.global_encoding & 2 ) ==2 )
   {
      m_areWFInternal = true;
   }
   else if((public_header.global_encoding & 4) == 4)
   {
      m_areWFInternal = false;
      // check if .wdp exist
      m_wdpFilename = m_lasFilename;
      m_wdpFilename.replace(m_wdpFilename.end()-3,m_wdpFilename.end(),"wdp");
      m_wdpFile.open(m_wdpFilename.c_str());
      if(!m_wdpFile)
      {
         std::cout << "Error: waveforms are externally saved but .wdp file "
                   << "doesn't exist\n";
         exit(EXIT_FAILURE);
      }
      else
      {
        m_wdpFile.close();
      }
   }

   // test version of the file. version should be 1.3
   if (!(public_header.version_major==1 && (public_header.version_minor==0
          || public_header.version_minor==1 || public_header.version_minor==2
          || public_header.version_minor==3)))
   {
      std::cerr << "Incorrect version. Only 1.3 are allowed.\n";

  }
   if(public_header.x_scale_factor<0.00001 && public_header.x_scale_factor>-0.00001)
   {
       public_header.x_scale_factor=1.0;
   }

   if(public_header.y_scale_factor<0.00001 && public_header.y_scale_factor>-0.00001)
   {
       public_header.y_scale_factor=1.0;
   }

   if(public_header.z_scale_factor<0.00001 && public_header.z_scale_factor>-0.00001)
   {
       public_header.z_scale_factor=1.0;
   }
}

//-----------------------------------------------------------------------------
void Las1_3_handler::read_variable_length_records()
{
   for(unsigned int i=0; i<wv_info.size();++i)
   {
      if(wv_info[i]!=NULL)
      {
         delete wv_info[i];
      }
   }
   wv_info.clear();
   LAS1_3Types::Variable_Length_Record_Header headdata_rec;
   for(unsigned int i=0; i<public_header.number_of_variable_lenght_records;
      ++i)
   {
      m_lasfile.read((char *) &headdata_rec, VbleRec_header_length);
      char skip_record [headdata_rec.record_length_after_header];
      m_lasfile.read((char *) skip_record,headdata_rec.record_length_after_header);


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
      }
      else if (headdata_rec.record_ID == 1003)
      {

      }
      else if (headdata_rec.record_ID>=100 && headdata_rec.record_ID<356)
      {
         LAS1_3Types::WF_packet_Descriptor *waveInfo = new LAS1_3Types::WF_packet_Descriptor;
         // -1 exist for the id that is added manually here and it's not read
         memcpy((void *) waveInfo, (void *) skip_record, sizeof(LAS1_3Types::WF_packet_Descriptor)-1);
         if(!waveInfo)
         {
            std::cerr << "ERROR: LAS1_3_handler: failed to allocate memory\n";
            continue;
         }
         unsigned int id = int(headdata_rec.record_ID)-99;
         waveInfo->id = id;
//         std::cout << "    ***   " << int(headdata_rec.record_ID) << "   ***\n";
//         std::cout << "waveInfo->bits_per_sample : " << int(waveInfo->bits_per_sample) << "\n";
//         std::cout << "waveInfo->number_of_samples : " << int(waveInfo->number_of_samples)  << "\n";
//         std::cout << "waveInfo->digitizer_gain : " << int(waveInfo->digitizer_gain) << "\n";
//         std::cout << "waveInfo->digitizer_offset  : " << int(waveInfo->digitizer_offset) << "\n";
//         std::cout << "waveInfo->temporal_sample_spacing : " << int(waveInfo->temporal_sample_spacing) << "\n";
//         std::cout << "waveInfo->id : " <<  int(waveInfo->id) << "\n";
//         std::cout << "waveInfo->wf_compression_type : " << int(waveInfo->wf_compression_type) << "\n";

         wv_info.push_back(waveInfo);
      }
   }
}

//-----------------------------------------------------------------------------
void Las1_3_handler::printPublicHeader()const
{
   std::cout << std::setprecision(2) << std::fixed;
   std::cout << "\nHeader file of " << m_lasFilename << std::endl;
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
    for(unsigned int i=0; i<wv_info.size();++i)
    {
       if(wv_info[i]!=NULL)
       {
          delete wv_info[i];
       }
    }
}

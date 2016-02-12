#ifndef PWTYPES_H
#define PWTYPES_H

// the comments in quotes are taken from the Pulse Waves file specifications
// available at:
// https://github.com/PulseWaves/Specification/blob/master/specification.rst


class PWTypes
{
public:

#pragma pack(push)
#pragma pack(1)
   typedef struct Header_Block                           //352 bytes
   {
       // ---------------------------------------------------------------
       // file signiture is always "PulseWavesPulse"
       // ---------------------------------------------------------------
       char file_signiture[16];                           // 16 bytes   0
       // ---------------------------------------------------------------
       // "This is a bit field used to specify global properties about the file"
       // ---------------------------------------------------------------
       unsigned int global_parameters;                    //  4 bytes   1
       // ---------------------------------------------------------------
       // should be between 1-65535 and it could be the flight line number
       // 0 indicates that it haven't been specified
       // ---------------------------------------------------------------
       unsigned int file_source_ID;                       //  4 bytes   2
       // ---------------------------------------------------------------
       // optional
       // ---------------------------------------------------------------
       unsigned int project_ID_GUID_data_1;               //  4 bytes   3
       unsigned short project_ID_GUID_data_2;             //  2 bytes   4
       unsigned short project_ID_GUID_data_3;             //  2 bytes   5
       unsigned char project_ID_GUID_data_4 [8];          //  8 bytes   6
       // ---------------------------------------------------------------
       // "This info is ASCII data descriping the hardware sensor that
       // collected or the process that generated the pulse records in
       // the file."
       // ---------------------------------------------------------------
       char system_identifier[64];                        // 64 bytes   7
       char generating_software[64];                      // 64 bytes   8
       // ---------------------------------------------------------------
       // compute at the Greenwich Mean Time (GMT) day
       // with Jan 1 considered to be day 1
       // ---------------------------------------------------------------
       unsigned short file_creation_day_of_year;          //  2 bytes   9
       unsigned short file_creation_year;                 //  2 bytes  10
       // ---------------------------------------------------------------
       unsigned char version_major;                       //  1 bytes  11
       unsigned char version_minor;                       //  1 bytes  12
       // ---------------------------------------------------------------

       // always 235 unless more data are added to the end of the header
       // ---------------------------------------------------------------
       unsigned short header_size;                        //  2 bytes  13
       // ---------------------------------------------------------------
       // number of bytes from the beginning of the file
       // to the first field of the first point record data field
       // ---------------------------------------------------------------
       unsigned long long int offset_to_pulse_data;       //  8 bytes  14
       unsigned long long int number_of_pulses;           //  8 bytes  15
       unsigned int pulse_format;                         //  4 bytes  16
       unsigned int pules_attributes;                     //  4 bytes  17
       // ---------------------------------------------------------------
       // the size of point data record in bytes
       // ---------------------------------------------------------------
       unsigned int pulse_size;                           //  4 bytes  18
       unsigned int pulse_compression;                    //  4 bytes  19
       long long int reserved;                            //  8 bytes  20
       unsigned int number_of_variable_length_records;    //  4 bytes  21
       int number_of_appended_variable_length_records;    //  4 bytes  22
       // ---------------------------------------------------------------
       double t_scale_factor;                             //  8 bytes  23
       double t_offset;                                   //  8 bytes  24
       long long int min_t;                               //  8 bytes  25
       long long int max_t;                               //  8 bytes  26

       // ---------------------------------------------------------------
       // these scale factors must be applied to each point x,y,z
       // ---------------------------------------------------------------
       double x_scale_factor;                             //  8 bytes  27
       double y_scale_factor;                             //  8 bytes  28
       double z_scale_factor;                             //  8 bytes  29
       // these offset must be added to each point x,y,z
       double x_offset;                                   //  8 bytes  30
       double y_offset;                                   //  8 bytes  31
       double z_offset;                                   //  8 bytes  32
       // max and min data fields
       double min_x;                                      //  8 bytes  33
       double max_x;                                      //  8 bytes  34
       double min_y;                                      //  8 bytes  35
       double max_y;                                      //  8 bytes  36
       double min_z;                                      //  8 bytes  37
       double max_z;                                      //  8 bytes  38
   }Header_Block;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
   typedef struct Variable_Length_Record                 // 96 bytes
   {
     char user_ID [16];                                  // 16 bytes   0
     unsigned int record_ID;                             //  4 bytes   1
     unsigned int reserved;                              //  4 bytes   2
     unsigned long long int record_length_after_header;  //  8 bytes   3
     char description [64];                              // 64 bytes   4
    }Variable_Length_Record;
#pragma pack(pop)

// maybe ingnored! could have just been another variable length record
// interpretated differently
#pragma pack(push)
#pragma pack(1)
   typedef struct Appended_Variable_Length_Record        // 96 bytes
   {
     char user_ID [16];                                  // 16 bytes   0
     unsigned int record_ID;                             //  4 bytes   1
     unsigned int reserved;                              //  4 bytes   2
     unsigned long long int record_length_before_footer; //  8 bytes   3
     char description [64];                              // 64 bytes   4
    }Appended_Variable_Length_Record;
#pragma pack(pop)

    // point data format
#pragma pack(push)
#pragma pack(1)
   typedef struct PulseRecord                            // 48 bytes
   {
      long long int gps_timestap_t;                      //  8 bytes   0
      long long int offset_to_waves;                     //  8 bytes   1
      int anchor_x;                                      //  4 bytes   2
      int anchor_y;                                      //  4 bytes   3
      int anchor_z;                                      //  4 bytes   4
      int target_x;                                      //  4 bytes   5
      int target_y;                                      //  4 bytes   6
      int target_z;                                      //  4 bytes   7
      short int first_returning_sample;                  //  2 bytes   8
      short int last_returning_sample;                   //  2 bytes   9
      unsigned char pulse_discriptor_index;              //  1 byte   10
      unsigned char res4_scanLineE1_scanDir1_MirFacet_2; //  1 byte   11:14
      unsigned char intensity;                           //  1 byte   15
      unsigned char classification;                      //  1 byte   16

   }PulseRecord;
#pragma pack(pop)


#pragma pack(push)
#pragma pack(1)
   typedef struct WavesHeader                            // 96 bytes
   {
      char file_signiture[16];                           // 16 bytes   0
      unsigned int compression;                          //  4 bytes   1
      unsigned char reserved[40];                        // 40 bytes   2
   } WavesHeader;
#pragma pack(pop)










};

#endif // TYPESOFPULSEWAVES_H

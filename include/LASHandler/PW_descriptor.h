#ifndef PW_DESCRIPTORS_H
#define PW_DESCRIPTORS_H

#include <vector>
#include <iostream>
#include <fstream>
#include <gmtl/Vec.h>
#include "Volume.h"
#include <assert.h>
#include <math.h>
#include "DtmBilReader.h"


class PW_descriptor
{
public:
   #pragma pack(push)
   #pragma pack(1)
   typedef struct CompositionRecord                      // 28 bytes
   {
      unsigned int size;                                 //  4 bytes    0
      unsigned int reserved;                             //  4 bytes    1
      int optical_centre_to_anchor_point;                //  4 bytes    2
      unsigned short int number_of_extra_wave_bytes;     //  2 bytes    3
      unsigned short int number_of_samplings;            //  2 bytes    4
      float sample_units;                                //  4 bytes    5
      unsigned int compression;                          //  4 bytes    6
      unsigned int scanner_index;                        //  4 bytes    7
      // ...
      // char description [64] 64  bytes
   } CompositionRecord;
   #pragma pack(pop)


   #pragma pack(push)
   #pragma pack(1)
   typedef struct SamplingRecord                         // 28 bytes
   {
      unsigned int size;                                 //  4 bytes    0
      unsigned int reserved;                             //  4 bytes    1
      unsigned char type;                                //  1 byte     2
      unsigned char channel;                             //  1 byte     3
      unsigned char unused;                              //  1 byte     4
      unsigned char bits_for_duration_from_anchor;       //  1 byte     5
      float scale_for_duration_from_anchor;              //  4 bytes    6
      float offset_for_duration_from_anchor;             //  4 bytes    7
      unsigned char bits_for_number_of_segments;         //  1 byte     8
      unsigned char bits_for_number_of_samples;          //  1 byte     9
      unsigned short int number_of_segments;             //  2 bytes   10
      unsigned int number_of_samples;                    //  4 bytes   11
      unsigned short int bits_per_sample;                //  2 bytes   12
      unsigned short int look_up_table_index;            //  2 bytes   13
      float sample_units;                                //  4 bytes   14
      unsigned int compression;                          //  4 bytes   15
      // ...
      // char description [64] 64  bytes
   } SamplingRecord;
   #pragma pack(pop)


  //-------------------------------------------------------------------------
   /// @brief default contrtuctor
   //-------------------------------------------------------------------------
   PW_descriptor(
           char *skip_record,
           unsigned int record_length,
           unsigned short i_id
           );
   //-------------------------------------------------------------------------
   /// @brief method that read waveforms from a given file pointing at the
   /// starting of the waveform
   //-------------------------------------------------------------------------
   void readWaveform(std::ifstream &i_wvsfile,
           long long i_offset,
           const gmtl::Vec3f &i_anchor,
           const gmtl::Vec3f &i_dis,
           Volume *i_obj,
           DtmBilReader &i_dtm);
   //-------------------------------------------------------------------------
   /// @brief method that prints the descriptor
   //-------------------------------------------------------------------------
   void print();
   //-------------------------------------------------------------------------
   /// @brief default destructor
   //-------------------------------------------------------------------------
   ~PW_descriptor();


//private:
   //-------------------------------------------------------------------------
   /// @brief Composition Record
   //-------------------------------------------------------------------------
   CompositionRecord m_compositionRecord;
   //-------------------------------------------------------------------------
   /// @brief descriptor id, which is a unique number
   //-------------------------------------------------------------------------
   unsigned short int m_id;

   //-------------------------------------------------------------------------
   /// @brief method that returns the a number from the requested
   /// number of bits 8, 16, or 32
   /// @param[in] i_noOfbits : number of bits to be returned
   //-------------------------------------------------------------------------
   inline unsigned int getNumber(
           const unsigned short int i_noOfbits,
           std::ifstream &i_wvsfile
           )const
   {
      unsigned int value = 0;
      i_wvsfile.read((char *) &value,i_noOfbits/8);
      assert(value<=pow(2,i_noOfbits));
      return value;
   }

   //-------------------------------------------------------------------------
   /// @brief current dtm Index
   //-------------------------------------------------------------------------
   unsigned int m_dtmIndex;
   //-------------------------------------------------------------------------
   /// @brief current dtm height
   //-------------------------------------------------------------------------
   float m_dtmHeight;


   //-------------------------------------------------------------------------

   //-------------------------------------------------------------------------
   std::vector<SamplingRecord *> m_samplingRecords;

};

#endif // PW_DESCRIPTORS_H

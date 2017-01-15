#include "PW_descriptor.h"
#include <stdio.h>
#include <string.h>
#include <iostream>

//-----------------------------------------------------------------------------
PW_descriptor::PW_descriptor(
        char *skip_record,
        unsigned int record_length,
        unsigned short int i_id
        ):m_id(i_id),m_dtmIndex(0), m_dtmHeight(0.0f)
{
   unsigned int offsetToStartOfRecord =0;
   char *tempSkipRecord = skip_record;
   unsigned int size;
   memcpy((void *)&size,(void *)skip_record,sizeof(unsigned int));
   if(size+offsetToStartOfRecord>record_length)
   {
      std::cout << "Warning: size of scanner VLR bigger than allocated\n";
   }
   memcpy((void *) &m_compositionRecord,skip_record,sizeof(CompositionRecord));

   offsetToStartOfRecord+=m_compositionRecord.size;
   m_samplingRecords.resize(m_compositionRecord.number_of_samplings);
   for(unsigned int i=0; i<m_samplingRecords.size(); ++i)
   {
      tempSkipRecord=skip_record+offsetToStartOfRecord;
      memcpy((void *)&size,(void *)tempSkipRecord,sizeof(unsigned int));
      if(size+offsetToStartOfRecord>record_length)
      {
         std::cout << "Warning: size of scanner VLR bigger than allocated\n";
      }
      m_samplingRecords[i] = new SamplingRecord;
      memcpy((void*)m_samplingRecords[i],tempSkipRecord,sizeof(SamplingRecord));
      offsetToStartOfRecord+=m_samplingRecords[i]->size;
   }
}

//-----------------------------------------------------------------------------
void PW_descriptor::addAmplitudes(
        std::ostream &i_csv,
        std::ifstream &i_wvsfile,
        long long int i_offset
        )
{
    i_wvsfile.seekg(i_offset+m_compositionRecord.number_of_extra_wave_bytes);
    unsigned int number_of_segments(0);
    unsigned int number_of_samples(0);
    for(unsigned int sampling=0;
        sampling<m_compositionRecord.number_of_samplings; ++sampling)
    {
        if(m_samplingRecords[sampling]->type == 1) // outgoing waveform
        {
        }
        else if (m_samplingRecords[sampling]->type == 2) // returning waveform
        {
            // get number of segments
            unsigned short int bits_for_number_of_segments =
                    m_samplingRecords[sampling]->bits_for_number_of_segments;
           number_of_segments=bits_for_number_of_segments==0?
                       m_samplingRecords[sampling]->number_of_segments:
                       getNumber(bits_for_number_of_segments,i_wvsfile);

           for(unsigned s=0; s<number_of_segments; ++s)
           {
              // number of samples
              unsigned short int bits_for_number_of_samples=
                      m_samplingRecords[sampling]->bits_for_number_of_samples;
              number_of_samples=bits_for_number_of_samples!=0?
                          getNumber(bits_for_number_of_samples,i_wvsfile):
                          m_samplingRecords[sampling]->number_of_samples;


              unsigned short int bits_per_sample = m_samplingRecords[sampling]->bits_per_sample;
              for(unsigned i=0;i<number_of_samples;++i)
              {

                 i_csv << getNumber(bits_per_sample,i_wvsfile) << "\n";
              }
           }
        }
        else
        {
           std::cout << "WARNING: unrecognised sampling type of waveform";
        }
     }
}

//-----------------------------------------------------------------------------
void PW_descriptor::readWaveform(
        std::ifstream &i_wvsfile,
        long long int i_offset,
        const gmtl::Vec3f &i_anchor,
        const gmtl::Vec3f &i_dis,
        Volume *i_obj,
        DtmBilReader &i_dtm
        )
{
   i_wvsfile.seekg(i_offset+m_compositionRecord.number_of_extra_wave_bytes);

   unsigned int number_of_segments(0);
   unsigned int number_of_samples(0);
   float duration_from_the_anchor(0.0f);
   float sample_units(0.0f);

   for(unsigned int sampling=0;
       sampling<m_compositionRecord.number_of_samplings; ++sampling)
   {
       if(m_samplingRecords[sampling]->type == 1) // outgoing waveform
       {
       }
       else if (m_samplingRecords[sampling]->type == 2) // returning waveform
       {
           // get number of segments
           unsigned short int bits_for_number_of_segments =
                   m_samplingRecords[sampling]->bits_for_number_of_segments;
          number_of_segments=bits_for_number_of_segments==0?
                      m_samplingRecords[sampling]->number_of_segments:
                      getNumber(bits_for_number_of_segments,i_wvsfile);


          // read each one of the segments
          unsigned short int bits_for_duration_from_anchor =
                  m_samplingRecords[sampling]->bits_for_duration_from_anchor;
          for(unsigned s=0; s<number_of_segments; ++s)
          {
             // get duration from the anchor
             duration_from_the_anchor=(bits_for_duration_from_anchor==0)?
                         duration_from_the_anchor == 0.0f:
                        ((float)getNumber(bits_for_duration_from_anchor,i_wvsfile)*
                        m_samplingRecords[sample_units]->scale_for_duration_from_anchor+
                        m_samplingRecords[sample_units]->offset_for_duration_from_anchor);

             // number of samples
             unsigned short int bits_for_number_of_samples=
                     m_samplingRecords[sampling]->bits_for_number_of_samples;
             number_of_samples=bits_for_number_of_samples!=0?
                         getNumber(bits_for_number_of_samples,i_wvsfile):
                         m_samplingRecords[sampling]->number_of_samples;


             unsigned short int bits_per_sample = m_samplingRecords[sampling]->bits_per_sample;
             for(unsigned i=0;i<number_of_samples;++i)
             {
                gmtl::Vec3f point(i_anchor);
                point[0]+=(duration_from_the_anchor * i_dis[0]);
                point[1]+=(duration_from_the_anchor * i_dis[1]);
                point[2]+=(duration_from_the_anchor * i_dis[2]);
                point[2]-=i_dtm.getHeightOf(point[0],point[1]);
                unsigned int intensity = getNumber(bits_per_sample,i_wvsfile);
                i_obj->addItensity(point,intensity);
             }
          }
       }
       else
       {
          std::cout << "WARNING: unrecognised sampling type of waveform";
       }
    }
}

//-----------------------------------------------------------------------------
void PW_descriptor::print()
{
   std::cout << "Composition Record: " << m_id << "\n--------------------------\n";
   std::cout << "Size = " << m_compositionRecord.size << "\n";
   std::cout << "reserved = " << m_compositionRecord.reserved << "\n";
   std::cout << "optical centre to anchor point = " << m_compositionRecord.optical_centre_to_anchor_point << "\n";
   std::cout << "mumber of extra wave bytes = " << m_compositionRecord.number_of_extra_wave_bytes << "\n";
   std::cout << "mumber of samplings = " << m_compositionRecord.number_of_samplings << "\n";
   std::cout << "smaple units = " << m_compositionRecord.sample_units << "\n";
   std::cout << "compression = " << m_compositionRecord.compression << "\n";
   std::cout << "scanner index = " << m_compositionRecord.scanner_index << "\n";
   std::cout << "Sampling records are the follwoing:\n";

   for(unsigned int i=0; i<m_samplingRecords.size() ; ++i)
   {
       std::cout << "Record : " << i << " **\n";
       std::cout << "Size = " << (int) m_samplingRecords[i]->size << "\n";
       std::cout << "type = " << (int) m_samplingRecords[i]->type << "\n";
       std::cout << "channel = "<< (int)m_samplingRecords[i]->channel << "\n";
       std::cout << "unused = "<<(int) m_samplingRecords[i]->unused << "\n";
       std::cout << "bits_for_duration_from_anchor = "<< (int)m_samplingRecords[i]->bits_for_duration_from_anchor << "\n";
       std::cout << "scale_for_duration_from_anchor = "<< m_samplingRecords[i]->scale_for_duration_from_anchor << "\n";
       std::cout << "offset_for_duration_from_anchor = "<< m_samplingRecords[i]->offset_for_duration_from_anchor << "\n";
       std::cout << "bits_for_number_of_segments = "<<(int) m_samplingRecords[i]->bits_for_number_of_segments << "\n";
       std::cout << "bits_for_number_of_samples = " << (int)m_samplingRecords[i]->bits_for_number_of_samples << "\n";
       std::cout << "number_of_segments = " << (int)m_samplingRecords[i]->number_of_segments << "\n";
       std::cout << "number_of_samples = " << (int)m_samplingRecords[i]->number_of_samples << "\n";
       std::cout << "bits_per_sample = " << (int)m_samplingRecords[i]->bits_per_sample << "\n";
       std::cout << "look_up_table_index = " << (int)m_samplingRecords[i]->look_up_table_index << "\n";
       std::cout << "sample_units = " << m_samplingRecords[i]->sample_units << "\n";
       std::cout << "compression = " << (int)m_samplingRecords[i]->compression << "\n";
   }
}

//-----------------------------------------------------------------------------
PW_descriptor::~PW_descriptor()
{
   for(unsigned int i=0; i< m_samplingRecords.size();++i)
   {
      if(m_samplingRecords[i]!=NULL)
      {
         delete m_samplingRecords[i];
      }
   }
}


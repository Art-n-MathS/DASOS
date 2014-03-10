#include "Pulse.h"
#include <iostream>

int Pulse::count = 0;
std::vector<float> Pulse::s_kernel;

//-----------------------------------------------------------------------------
Pulse::Pulse(
        const Types::Public_Header_Block &i_publicHeader,
        const Types::WF_packet_Descriptor &i_wv_info,
        const Types::Data_Point_Record_Format_4 &i_point_info,
        const char *wave_data
        ):m_returns(0)
{
   // sampling frequency in nanoseconds

   ngl::Vec3 point_scale_factors(i_publicHeader.x_scale_factor,
                                i_publicHeader.y_scale_factor,
                                i_publicHeader.z_scale_factor);
   ngl::Vec3 point_offsets(i_publicHeader.x_offset,
                          i_publicHeader.y_offset,
                          i_publicHeader.z_offset);
   m_origin = ngl::Vec3(i_point_info.X*i_publicHeader.x_scale_factor,
                     i_point_info.Y*i_publicHeader.y_scale_factor,
                     i_point_info.Z*i_publicHeader.z_scale_factor);
   m_origin+=point_offsets;
   m_point = ngl::Vec3(i_point_info.X,i_point_info.Y,i_point_info.Z)
           *point_scale_factors;
   m_numberOfReturnsForThisPulse =(int)
           (i_point_info.returnNo_noOfRe_scanDirFla_EdgeFLn&7);
   m_time = i_point_info.GBS_time;
   m_scanAngle = i_point_info.scan_angle_rank;
   m_classification = i_point_info.classification;
   m_temporalSampleSpacing = ((int)i_wv_info.temporal_sample_spacing)/1000.0f;
   m_AGCgain = i_point_info.gain;
   m_returnNumber = (int) (i_point_info.returnNo_noOfRe_scanDirFla_EdgeFLn&7);

   m_digitiserGain = i_wv_info.digitizer_gain;
   m_digitiserOffset = i_wv_info.digitizer_offset;
   m_sampleLength = m_temporalSampleSpacing*c_light_speed/2;

   m_noOfSamples = i_point_info.wf_packet_size_in_bytes;
   m_returnPointLocation = i_point_info.return_point_wf_location/1000;
   m_pointInWaveform = i_point_info.return_point_wf_location
           *c_light_speed/2/1000;

   m_offset= ngl::Vec3(i_point_info.X_t, i_point_info.Y_t, i_point_info.Z_t);
   m_offset *= (1000 * m_temporalSampleSpacing);

   m_origin.m_x = m_origin.m_x + (double )i_point_info.X_t*
           (double )i_point_info.return_point_wf_location;
   m_origin.m_y = m_origin.m_y + (double )i_point_info.Y_t*
           (double )i_point_info.return_point_wf_location;
   m_origin.m_z = m_origin.m_z + (double )i_point_info.Z_t*
           (double )i_point_info.return_point_wf_location;
   m_returns = new (std::nothrow) char[m_noOfSamples];
   if(m_returns==0)
   {
       std::cerr << "Error: Memory could not be allocated in file Pulse.cpp\n";
       exit(EXIT_FAILURE);
   }
   memcpy(m_returns,wave_data,m_noOfSamples);

}

//-----------------------------------------------------------------------------
Pulse::Pulse(
        const Pulse &i_pulse
        ):
    m_point(i_pulse.m_point),
    m_returnNumber(i_pulse.m_returnNumber),
    m_numberOfReturnsForThisPulse(i_pulse.m_numberOfReturnsForThisPulse),
    m_time(i_pulse.m_time),
    m_scanAngle(i_pulse.m_scanAngle),
    m_classification(i_pulse.m_classification),
    m_temporalSampleSpacing(i_pulse.m_temporalSampleSpacing),
    m_AGCgain(i_pulse.m_AGCgain),
    m_digitiserGain(i_pulse.m_digitiserGain),
    m_digitiserOffset(i_pulse.m_digitiserOffset),
    m_noOfSamples(i_pulse.m_noOfSamples),
    m_sampleLength(i_pulse.m_sampleLength),
    m_returnPointLocation(i_pulse.m_returnPointLocation),
    m_pointInWaveform(i_pulse.m_pointInWaveform),
    m_offset(i_pulse.m_offset),
    m_origin(i_pulse.m_origin)
{
   m_returns = new (std::nothrow) char[m_noOfSamples];
   if(m_returns==0)
   {
      std::cerr << "Error: Memory could not be allocated in file Pulse.cpp\n";
      exit(EXIT_FAILURE);
   }
   memcpy(m_returns,i_pulse.m_returns,m_noOfSamples);
//   m_peakPoints.resize(i_pulse.m_peakPoints.size());
//   for(unsigned int i=0; i<i_pulse.m_peakPoints.size();++i)
//   {
//      m_peakPoints[i] = i_pulse.m_peakPoints[i];
//   }
}

////-----------------------------------------------------------------------------
//void Pulse::clearPeaks()
//{
//   m_peakPoints.clear();
//}

////-----------------------------------------------------------------------------
//const std::vector<Peak *> &Pulse::getPeakPoints(

//        )
//{
//   // check if points has already been detected
//   if(m_peakPoints.size()!=0)
//   {
//      return m_peakPoints;
//   }
//   else
//   {
//      detectPeaks();
//      return m_peakPoints;
//   }
//}

////-----------------------------------------------------------------------------
//void Pulse::detectPeaks()
//{
//   // first smooth the wave
//   float kernelNormalisedFactor = 0;
//   for(unsigned int i=0; i<s_kernel.size(); ++i)
//   {
//       kernelNormalisedFactor+=s_kernel[i];
//   }
//   // then use gradient to find the peak points
//}


//-----------------------------------------------------------------------------
void Pulse::print()const
{
   std::cout << "Point                            " << m_point.m_x << " " << m_point.m_y << " " << m_point.m_z << "\n";
   std::cout << "Return Number                    " << m_returnNumber<< "\n";
   std::cout << "Number of returns for this pulse " << m_numberOfReturnsForThisPulse<< "\n";
   std::cout << "Time                             " << m_time<< "\n";
   std::cout << "Scan Angle                       " << m_scanAngle << "\n";
   std::cout << "Classification                   " << m_classification << "\n";
   std::cout << "Temporal Sample Spacing          " << m_temporalSampleSpacing << "\n";
   std::cout << "AGC gain                         " << m_AGCgain << "\n";
   std::cout << "Digitiser Gain                   " << m_digitiserGain << "\n";
   std::cout << "Digitiser Offset                 " << m_digitiserOffset  << "\n";
   std::cout << "No. of Samples                   " << m_noOfSamples << "\n";
   std::cout << "Sample Length                    " << m_sampleLength << "\n";
   std::cout << "Return Point Location            " << m_returnPointLocation << "\n";
   std::cout << "Point in Waveform                " << m_pointInWaveform << "\n";
   std::cout << "Offset                           " << m_offset.m_x << " " << m_offset.m_y << " " << m_offset.m_z << "\n";
   std::cout << "Origin                           " << m_origin.m_x << " " << m_origin.m_y << " " << m_origin.m_z << "\n";
   std::cout << "Intensities\n";
   if(m_returns!=0)
   {
      for(unsigned short int i=0; i< m_noOfSamples; ++i)
      {
         std::cout << (int) m_returns[i] << " ";
      }
      std::cout << "\n";
   }
}

//-----------------------------------------------------------------------------
bool Pulse::isInsideLimits(const std::vector<double> &i_user_limits)const
{
   return m_point.m_y<i_user_limits[0] && m_point.m_y>i_user_limits[1] &&
          m_point.m_x<i_user_limits[2] && m_point.m_x>i_user_limits[3];
}

//-----------------------------------------------------------------------------
void Pulse::addIntensitiesToObject(
        Object *obj
        )
{
   ngl::Vec3 tempPosition = m_origin;
   for(unsigned int i=0; i< m_noOfSamples; ++i)
   {
     obj->addItensity(tempPosition,m_returns[i]);
     tempPosition+=m_offset;
   }
}

//-----------------------------------------------------------------------------
Pulse::~Pulse()
{
   if (m_returns!=0)
   {
       delete []m_returns;
   }
//   for(unsigned int i=0; i<m_peakPoints.size(); ++i)
//   {
//       delete m_peakPoints[i];
//   }
}

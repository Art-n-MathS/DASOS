#ifndef PULSE_H
#define PULSE_H
#include <ngl/Vec3.h>
#include "Types.h"
#include "Object.h"
#include "Peak.h"

class Pulse
{
    friend class PulseManager;

public:
   //--------------------------------------------------------------------------
   /// @brief default constructor
   //--------------------------------------------------------------------------
   Pulse(
       const Types::Public_Header_Block &i_publicHeader,
       const Types::WF_packet_Descriptor &i_wv_info,
       const Types::Data_Point_Record_Format_4 &i_point_info,
       const char *wave_data
           );
   //--------------------------------------------------------------------------
   /// @brief copy constructor
   //--------------------------------------------------------------------------
   Pulse(const Pulse &i_pulse);
   //--------------------------------------------------------------------------
   /// @brief method that returns true if the pulse is inside the user limits
   /// @param[in] i_limits the limits of area of our interest
   /// @returns boolean whether the pulse is insise the given limits or not
   //--------------------------------------------------------------------------
   bool isInsideLimits(const std::vector<double> &i_user_limits)const;

   //--------------------------------------------------------------------------
   /// @brief method that prints all the attributes associated with this pulse
   //--------------------------------------------------------------------------
   void print()const;
   //--------------------------------------------------------------------------
   /// @brief method that returns the origin of the point
   //--------------------------------------------------------------------------
   ngl::Vec3 &getOrigin(){return m_origin;}
   //--------------------------------------------------------------------------
   /// @brief method that returns the offset of the point
   //--------------------------------------------------------------------------
   ngl::Vec3 &getOffset(){return m_offset;}
   //--------------------------------------------------------------------------
   /// @brief method that add the intensities return to a given object
   /// @param[in] i_obj the input obj of time LASVoxels
   //--------------------------------------------------------------------------
   void addIntensitiesToObject(
           Object *obj
           );
   //-------------------------------------------------------------------------
   /// @brief the kernel used to smooth the wave
   /// @note normalised factor does not have to be included
   //-------------------------------------------------------------------------
   static std::vector<float> s_kernel;


   //--------------------------------------------------------------------------
   /// @brief default destructor
   //--------------------------------------------------------------------------
   ~Pulse();

   static int count;

private:

   //--------------------------------------------------------------------------
   /// @brief method that detects the peak points
   //--------------------------------------------------------------------------
   void detectPeaks();
   //--------------------------------------------------------------------------
   /// @brief
   //--------------------------------------------------------------------------
   char *m_returns;

   ngl::Vec3 m_point;

   char m_returnNumber;

   char m_numberOfReturnsForThisPulse;

   float m_time;

   char m_scanAngle;
   // ---------------------------------------------------------------
   //  0 created, never classified
   //  1 Unclassified
   //  2 Ground
   //  3 Low Vegetation
   //  4 Medium Vegetation
   //  5 High Vegetation
   //  6 Building
   //  7 Low Point (noise)
   //  8 model Key-point (mass point)
   //  9 water
   // 10- 11 reserved for ASPRS definition
   // 12 overlap points
   // 13-31 reserved for ASPRS definition
   // ---------------------------------------------------------------
   char m_classification;

   float m_temporalSampleSpacing;

   unsigned char m_AGCgain;

   float m_digitiserGain;

   float m_digitiserOffset;

   unsigned int m_noOfSamples;

   float m_sampleLength;

   float m_returnPointLocation;

   float m_pointInWaveform;

   ngl::Vec3 m_offset;

   ngl::Vec3 m_origin;
   //-------------------------------------------------------------------------
   /// @brief meters per nanosecond
   //-------------------------------------------------------------------------
   static constexpr float c_light_speed = 0.299792458;



   //-------------------------------------------------------------------------
   /// @brief all the peak points of the pulse
   //-------------------------------------------------------------------------
//   std::vector<Peak *> m_peakPoints;

};

#endif // PULSE_H

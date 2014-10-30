


#ifndef PULSEMANAGER_H
#define PULSEMANAGER_H

#include "Pulse.h"
#include "Types.h"
#include "Object.h"

#include <unordered_map>
#include <vector>
#include <gmtl/Vec.h>

class PulseManager
{
    unsigned int assDis =0;
    unsigned int noAssDis = 0;
    unsigned int assDis2 =0;
    unsigned int noAssDis2 = 0;
public:
   //-------------------------------------------------------------------------
   /// @brief default constructor
   /// @param[in] i_wv_info
   //-------------------------------------------------------------------------
   PulseManager(
           const Types::Public_Header_Block &i_publicHeader,
           const Types::WF_packet_Descriptor &i_wv_info
           );
   //-------------------------------------------------------------------------
   /// @brief copy constructor
   /// @param[in] i_pulseManager object to be copied
   //-------------------------------------------------------------------------
   PulseManager(const PulseManager &i_pulseManager);
   //-------------------------------------------------------------------------
   /// @brief method that adds a fw pulse to the manager
   /// @param[in] i_point
   /// @param[in] wave_data  information of the wave
   /// @param[in] wave_offset the offset of the wave in the binary file
   //-------------------------------------------------------------------------
   void addPoint(
           const Types::Data_Point_Record_Format_4 &i_point,
           const char *wave_data,
           int wave_offset
                 );
   //-------------------------------------------------------------------------
   /// @brief method that finds the pulse associate with the given discrete
   /// point and saves it
   /// this method is called when we want to save 2nd, 3rd or 4th returns
   /// @param[in] i_point the discrete info about the point
   /// @param[in] wave_offset the offset of the waveform associated for that
   /// point
   //-------------------------------------------------------------------------
   void addPoint(
           const Types::Data_Point_Record_Format_4 &i_point,
           int wave_offset
           );
   //-------------------------------------------------------------------------
   /// @brief method that returns the max x coordinates as specified in
   /// the header file
   /// @returns the max x coordinates of the las file
   //-------------------------------------------------------------------------
   double getMaxX()const;
   //-------------------------------------------------------------------------
   /// @brief method that returns the min x coordinates as specified in
   /// the header file
   /// @returns the min x coordinates of the las file
   //-------------------------------------------------------------------------
   double getMinX()const;
   //-------------------------------------------------------------------------
   /// @brief method that returns the max y coordinates as specified in
   /// the header file
   /// @returns the max y coordinates of the las file
   //-------------------------------------------------------------------------
   double getMaxY()const;
   //-------------------------------------------------------------------------
   /// @brief method that returns the min y coordinates as specified in
   /// the header file
   /// @returns the min y coordinates of the las file
   //-------------------------------------------------------------------------
   double getMinY()const;
   //-------------------------------------------------------------------------
   /// @brief method that returns the max y coordinates as specified in
   /// the header file
   /// @returns the max y coordinates of the las file
   //-------------------------------------------------------------------------
   double getMaxZ()const;
   //-------------------------------------------------------------------------
   /// @brief method that returns the min z coordinates as specified in
   /// the header file
   /// @returns the min z coordinates of the las file
   //-------------------------------------------------------------------------
   double getMinZ()const;
   //-------------------------------------------------------------------------
   /// @brief method that sorts the discrete points into the pulses
   //-------------------------------------------------------------------------
   void sortDiscretePoints(
           const std::vector<gmtl::Vec3f> &m_discretePoints,
           const std::vector<unsigned short> &m_discreteIntensities,
           const std::vector<int> &m_discreteWaveOffsets
           );
   //-------------------------------------------------------------------------
   /// @brief method that returns the number of discrete points that are not
   /// associated with any waveform
   //-------------------------------------------------------------------------
   unsigned int getNumOfAloneDiscretePoints(){return m_discretePoints.size();}
   //-------------------------------------------------------------------------
   /// @brief method tha inserts the waveforms into a 3D density volume and
   /// returns the volume as an implicit object
   /// @warning the object is dynamically allocated and this class is not
   /// resposible for freeing that memory, so by calling this method, you are
   ///  resposible for deleting that object afterwards
   /// @param[in] i_x the number of voxels in the x dimension - the rest are
   /// calculated depending on the higher and lower x y z of the points
   /// @param[in] i_user_limits [maxNorthY, minNorthY, maxEastX, minEastX]
   /// @return the implicit object which is a made by voxels
   //-------------------------------------------------------------------------
   void fillObject(
           Object *obj,
           const std::vector<double> &user_limits,
           int i_type
           );
   //-------------------------------------------------------------------------
   /// @brief method that insert the discrete values into the 3D density
   /// volume
   ///
   //-------------------------------------------------------------------------

   //-------------------------------------------------------------------------
   /// @brief method that adds a discrete point that is not associated with
   /// any waveform
   /// @param[in] i_position the position of the discrete point
   /// @param[in] i_intensity the
   //-------------------------------------------------------------------------
   void addUnAssociatedDiscretePoint(
           const Types::Data_Point_Record_Format_4 &i_point_info
           );
   //-------------------------------------------------------------------------
   /// @brief method that sets the noise level
   /// @param[in] i_noiseLevel the new noise level
   //-------------------------------------------------------------------------
   void setNoiseLevel(double i_noiseLevel);
   //-------------------------------------------------------------------------
   /// @brief default destructor
   //-------------------------------------------------------------------------
   ~PulseManager();

private:
   //-------------------------------------------------------------------------
   /// @brief method that sorts the pulses with respect to the y position of
   /// their origins
   //-------------------------------------------------------------------------
   void sortPulseWithRespectToY();
   //-------------------------------------------------------------------------
   /// @brief method that fills the object with fw samples
   /// @param[in/out] i_obj: object to be filled
   /// @param[in] i_user_limits [maxNorthY, minNorthY, maxEastX, minEastX]
   //-------------------------------------------------------------------------
   void fillWithFWSamples(
           Object *i_obj,
           const std::vector<double> &user_limits
           );
   //-------------------------------------------------------------------------
   /// @brief method that fills the object with all the discrete returns
   /// @param[in/out] i_obj: object to be filled
   /// @param[in] i_user_limits [maxNorthY, minNorthY, maxEastX, minEastX]
   //-------------------------------------------------------------------------
   void fillWithAllDiscreteReturns(
           Object *i_obj,
           const std::vector<double> &user_limits
           );
   //-------------------------------------------------------------------------
   /// @brief method that fills the object with a combination of fw waves and
   /// the discrete returns that have not waveform associated with it
   /// @param[in/out] i_obj: object to be filled
   /// @param[in] i_user_limits [maxNorthY, minNorthY, maxEastX, minEastX]
   //-------------------------------------------------------------------------
   void fillWithFWnDiscrete(
           Object *i_obj,
           const std::vector<double> &user_limits
           );
   //-------------------------------------------------------------------------
   /// @brief method that fills the object with only the discrete returns that
   /// have a waveform associated with them
   /// @param[in/out] i_obj: object to be filled
   /// @param[in] i_user_limits [maxNorthY, minNorthY, maxEastX, minEastX]
   //-------------------------------------------------------------------------
   void fillWithDiscreteAssociatedWithFWonly(
           Object *i_obj,
           const std::vector<double> &user_limits
           );
   //-------------------------------------------------------------------------
   /// @brief public header block
   //-------------------------------------------------------------------------
   Types::Public_Header_Block m_public_header;
   //-------------------------------------------------------------------------
   /// @brief waveform packet descriptor
   //-------------------------------------------------------------------------
   Types::WF_packet_Descriptor m_wfInfo;
   //-------------------------------------------------------------------------
   /// @brief sort the pulses according to there wave offset for easy search
   /// while trying to associate the discrete points with the pulses
   //-------------------------------------------------------------------------
   std::unordered_map <  int , unsigned int> myMap;

   //-------------------------------------------------------------------------
   std::vector<Pulse *> m_pulses;
   //-------------------------------------------------------------------------
   /// @brief temporarly saving discrete values that are associated with a
   /// waveform but the 1st return haven't been saved yet
   //-------------------------------------------------------------------------
   std::vector<gmtl::Vec3f> m_discretePoints;
   //-------------------------------------------------------------------------
   /// @brief the corresponding intensities of the discrete points
   //-------------------------------------------------------------------------
   std::vector<unsigned short> m_discreteIntensities;
   //-------------------------------------------------------------------------
   /// @brief the corresponding wave offsets of the dicrete points
   //-------------------------------------------------------------------------
   std::vector<int> m_discreteWaveOffsets;
   //-------------------------------------------------------------------------
   /// @brief the noise Level
   //-------------------------------------------------------------------------
   double m_noiseLevel;






};

#endif // PULSEMANAGER_H

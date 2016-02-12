#ifndef PW_SCANNERVLR_H
#define PW_SCANNERVLR_H


class PW_scannerVLR
{
public:
   #pragma pack(push)
   #pragma pack(1)
   typedef struct ScannerVLR                             //_ bytes
   {
      unsigned int size;                                 //  4 bytes    0
      unsigned int reserved;                             //  4 bytes    1
      char instrument[64];                               // 64 bytes    2
      char serial[64];                                   // 64 bytes    3
      float wavelength;                                  //  4 bytes    4
      float outgoing_pulse_width;                        //  4 bytes    5
      unsigned int scan_pattern;                         //  4 bytes    6
      unsigned int number_of_mirror_facets;              //  4 bytes    7
      float scan_frequency;                              //  4 bytes    8
      float scan_angle_min;                              //  4 bytes    9
      float scan_angle_max;                              //  4 bytes   10
      float pulse_frequency;                             //  4 bytes   11
      float beam_diameter_at_exit_aperture;              //  4 bytes   12
      float beam_divergence;                             //  4 bytes   13
      float minimal_range;                               //  4 bytes   14
      float maximal_range;                               //  4 bytes   15
      // ...
      // char discription [64] 64  bytes
   } ScannerVLR;
   #pragma pack(pop)

   //--------------------------------------------------------------------------
   /// @brief default contructor
   //--------------------------------------------------------------------------
   PW_scannerVLR(
           char *skip_record,
           unsigned int record_length,
           unsigned short i_id
           );
   //--------------------------------------------------------------------------
   /// @brief method that prints all the information about the scanner
   //--------------------------------------------------------------------------
   void print()const;
   //--------------------------------------------------------------------------
   /// @brief defautlt destructor
   //--------------------------------------------------------------------------
   ~PW_scannerVLR();

private:
   //--------------------------------------------------------------------------
   /// @brief the information about the scanner
   //--------------------------------------------------------------------------
   ScannerVLR m_scannerVLR;
   //--------------------------------------------------------------------------
   /// @brief scanner id
   //--------------------------------------------------------------------------
   unsigned short int m_id;

};

#endif // PW_SCANNERVLR_H

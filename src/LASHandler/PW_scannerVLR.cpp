#include "PW_scannerVLR.h"
#include <string>
#include <cstring>
#include <stdio.h>
#include <iostream>

//-----------------------------------------------------------------------------
PW_scannerVLR::PW_scannerVLR(
        char *skip_record,
        unsigned int record_length,
        unsigned short i_id
        ):m_id(i_id)
{
    unsigned int size;
    memcpy((void *)&size,(void *)skip_record,sizeof(unsigned int));
    if(size>record_length)
    {
       std::cout << "Warning: size of scanner VLR bigger than allocated\n";
    }
    memcpy((void *) &m_scannerVLR,skip_record,sizeof(ScannerVLR));
}

//-----------------------------------------------------------------------------
void PW_scannerVLR::print()const
{
    std::cout << "size: " << m_scannerVLR.size << "\n";
    std::cout << "reserved: " << m_scannerVLR.reserved << "\n";
    std::cout << "instrument: " << m_scannerVLR.instrument << "\n";
    std::cout << "serial: " << m_scannerVLR.serial << "\n";
    std::cout << "wavelength: " << m_scannerVLR.wavelength << "\n";
    std::cout << "outgoing pulse width: " << m_scannerVLR.outgoing_pulse_width << "\n";
    std::cout << "scan pattern: " << m_scannerVLR.scan_pattern << "\n";
    std::cout << "number of mirror facets: " << m_scannerVLR.number_of_mirror_facets << "\n";
    std::cout << "scan fequency: " << m_scannerVLR.scan_frequency << "\n";
    std::cout << "scan angle min: " << m_scannerVLR.scan_angle_min << "\n";
    std::cout << "scan angle max: " << m_scannerVLR.scan_angle_max << "\n";
    std::cout << "pulse frequency: " << m_scannerVLR.pulse_frequency << "\n";
    std::cout << "beam diameter at exit aperture: " << m_scannerVLR.beam_diameter_at_exit_aperture << "\n";
    std::cout << "beam divergence: " << m_scannerVLR.beam_divergence << "\n";
    std::cout << "minimal range: " << m_scannerVLR.minimal_range << "\n";
    std::cout << "maximal range: " << m_scannerVLR.maximal_range << "\n";

}

//-----------------------------------------------------------------------------
PW_scannerVLR::~PW_scannerVLR()
{}

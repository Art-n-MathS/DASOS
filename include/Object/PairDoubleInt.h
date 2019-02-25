#ifndef PAIRDOUBLEINT_H
#define PAIRDOUBLEINT_H


class PairDoubleInt
{
public:
    //--------------------------------------------------------------------------
    /// @brief default constructor
    //--------------------------------------------------------------------------
    PairDoubleInt();
    PairDoubleInt(double i_int,unsigned int i_pos);
    //--------------------------------------------------------------------------
    /// @brief copy constructor
    //--------------------------------------------------------------------------
    PairDoubleInt(const PairDoubleInt &i_pd);
    //--------------------------------------------------------------------------
    /// @brief overwritting operator equal to correct it
    //--------------------------------------------------------------------------
    PairDoubleInt &operator=(const PairDoubleInt& _v);
    bool operator==(
                    const PairDoubleInt &_v
                   )const;
    //--------------------------------------------------------------------------
    /// @brief the intensity of the corresponding voxel
    //--------------------------------------------------------------------------
    double m_intensity;
    //--------------------------------------------------------------------------
    /// @brief before normalisation it's the number of samples inside a voxel
    /// while after the normalisation it's the
    //--------------------------------------------------------------------------
    unsigned int m_noOfInt_or_posOfInt;
    //--------------------------------------------------------------------------
    /// @brief default destructor
    //--------------------------------------------------------------------------
    ~PairDoubleInt();

};

#endif // PAIRDOUBLEINT_H

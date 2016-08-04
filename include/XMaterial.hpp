/**
 *  @file   XCoilHandle.hpp
 *  @author Y.Yang (Kyushu University)
 *  @date   4 Aug 2016
 */

#ifndef XMaterial_HH
#define XMaterial_HH

/// @enum material
enum Material
{
  kAluminium,
  kCopper,
  kNbTi,
  kKapton
};


/// base class to handle the material
//
class XMaterial
{
  public:
    /*! deconstructor */
    virtual ~XMaterial() {}

    /*! @brief setup material property */
    void SetMaterialProperty(const double T, const double RRR, const double B);

    /*! @brief setup temperature */
    void SetTemperature(const double T) { fTemp = T; }

    /*! @brief setup residual resistance ratio */
    void SetRRR(const double RRR) { fRRR = RRR; }

    /*! @brief setup magnetic field */
    void SetField(const double B) { fFld = B; }

    /*! @brief return material density */
    virtual double GetDensity() const = 0;

    /*! @brief return material capacity */
    virtual double GetCapacity() const = 0;

    /*! @brief return material resistivity */
    virtual double GetResistivity() const = 0;

    /*! @brief return material thermal conductivity */
    virtual double GetConductivity() const = 0;

    /*! @brief print out the information */
    void Print();

  protected:
    double fTemp;
    double fRRR;
    double fFld;

};

#endif

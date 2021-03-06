/**
 *  @file   XMatAluminium.hpp
 *  @author Y.Yang (Kyushu University)
 *  @date   5 Aug 2016
 */

#ifndef XMatAluminium_HH
#define XMatAluminium_HH

#include "XMaterial.hpp"

class XMatAluminium : public XMaterial
{
  public:
    /*! constructor */
    XMatAluminium();

    /*! deconstructor */
    virtual ~XMatAluminium();

    /*! @brief return material density */
    virtual double GetDensity() const { return 2700.; }

    /*! @brief return material thermal conductivity */
    virtual double GetConductivity();

    /*! @brief return material electric resistivity */
    virtual double GetResistivity();

    /*! @brief return material heat capacity */
    virtual double GetCapacity();


  protected:
    /*! @brief calculate electric resistivity */
    double calresist() const;

    /*! @brief calculate resistivity with input parameters */
    double evalresist(double* par) const;

    /*! @brief calculate magnetoresistance */
    double calmagres(double res) const;

    /*! @brief calculate heat capacity */
    double calcapacity(const double T) const;


  private:
    double fRhoRT;

};

#endif

/**
 *  @file   XCoilStrip.hpp
 *  @author Y.Yang (Kyushu University)
 *  @date   15 Aug 2016
 */

#ifndef XCoilStrip_HH
#define XCoilStrip_HH

#include "XCoilHandle.hpp"

namespace Quench
{ class XCoilStrip; }

/// abstract class to define the quench propagator strip parameters */
//
class Quench::XCoilStrip : public Quench::XCoilHandle
{
  public:
    /*! constructor */
    XCoilStrip();

    /*! deconstructor */
    virtual ~XCoilStrip();

    /*! @brief setup dimension of strip only */
    virtual void SetDimension(const double lz, const double lr);

    /*! @brief returns the array of dimension of strip only */
    virtual double* GetDimension() { return fSize; }

    /*! @brief setup the insulation thickness */
    virtual void SetInsSize(const double lz, const double lr);

    /*! @brief calculate the total area of strip */
    virtual double GetTotalArea() const;


  private:
    double* fSize;
    double* fIns;
};

#endif

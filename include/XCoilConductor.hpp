/**
 *  @file   XCoilConductor.hpp
 *  @author Y.Yang (Kyushu University)
 *  @date   14 Aug 2016
 */

#ifndef XCoilConductor_HH
#define XCoilConductor_HH

#include "XCoilHandle.hpp"

namespace Quench
{ class XCoilConductor; }

/// abstract class to define the conductor parameters
//
class Quench::XCoilConductor : public Quench::XCoilHandle
{
  public:
    /*! constructor */
    XCoilConductor();

    /*! deconstructor */
    virtual ~XCoilConductor();

    /*! @brief setup the dimension of conductor.
     *         In this case, the dimension of conductor is the dimension of
     *         the stabilizer
     */
    virtual void SetDimension(const double lz, const double lr);

    /*! @brief returns the array of dimension of conductor (stabilizer) */
    virtual double* GetDimension() { return fSize; }

    /*! @brief setup the insulation size along the z and r direction respectively */
    virtual void SetInsSize(const double lz, const double lr);

    /*! @brief return the array of insulation size.
     *         the first array is Lz
     *         the second array is Lr
     */
    virtual double* GetInsSize() { return fTape; }

    /*! @brief calculate the total area of conductor including the stabilizer
     *         and insulation
     */
    virtual double GetTotalArea() const;

    /*! @brief calculate this conductor area.
     *         noting that the insulation is not included
     */
    virtual double GetArea() const;

    /*! @brief calculate the area of only for the insulation */
    virtual double GetInsArea() const;


  private:
    double* fSize;
    double* fTape;
};

#endif

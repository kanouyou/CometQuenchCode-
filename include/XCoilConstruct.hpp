/**
 *  @file   XCoilConstruct.hpp
 *  @author Y.Yang (Kyushu University)
 *  @date   2 Aug 2016
 */

#ifndef XCoilConstruct_HH
#define XCoilConstruct_HH

#include <string>

#ifndef IFdmUnits_HH
#include "IFdmUnits.hpp"
#endif

namespace Quench
{ class XCoilConstruct; }

/// class to handle the information of coil
//
class Quench::XCoilConstruct
{
  friend class XPreProcess;

  public:
    /*! constuctor */
    XCoilConstruct();

    /*! @brief deconstructor */
    ~XCoilConstruct();

    /*! @brief setup stabilizer size */
    void SetStabilizer(const double lz, const double lr);

    /*! @brief setup tape size */
    void SetTape(const double lz, const double lr);

    /*! @brief setup conductor material ratio */
    void SetMaterialRatio(const double Al, const double Cu, const double SC);

    /*! @brief setup coil size */
    void SetCoil(const double lz, const double lp, const double lr);

    /*! @brief return stabilizer size */
    double* GetStabilizer() const { return fStable; }

    /*! @brief return stabilizer size */
    double GetStabilizer(const std::string &name) const;

    /*! @brief return tape size */
    double* GetTape() const { return fTape; }

    /*! @brief return tape size */
    double GetTape(const std::string &name) const;

    /*! @brief return material ratio */
    double* GetMaterialRatio() const { return fRatio; }

    /*! @brief return material ratio */
    double GetMaterialRatio(const std::string &name) const;

    /*! @brief return coil size */
    double* GetCoil() const { return fCoil; }

    /*! @brief return coil size */
    double GetCoil(const std::string &name) const;

    /*! @brief return conductor size */
    double* GetConductorSize() const;

    /*! @brief return conductor size */
    double GetConductorSize(const std::string& name) const;

    /*! @brief return area */
    double GetArea(const std::string& name) const;
    

  private:
    double* fStable;
    double* fTape;
    double* fRatio;
    double* fCoil;
};

#endif

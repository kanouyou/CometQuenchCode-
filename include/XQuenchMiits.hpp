/**
 *  @file   XQuenchMiits.hpp
 *  @author Y.Yang (Kyushu University)
 *  @date   13 Sep 2016
 **/

#ifndef XQuenchMiits_HH
#define XQuenchMiits_HH

#include <map>
#include "XCoilHandle.hpp"

class XQuenchMiits
{
  public:
    /// @brief constructor
    XQuenchMiits();

    /// @brief deconstructor
    ~XQuenchMiits();

    /// @brief setup dump resistor resistance
    void SetDumpResistor(const double R);

    /// @brief returns the resistance of dump resistor
    double GetDumpResistor() const { return fResist; }

    /// @brief setup magnet total inductance
    void SetInductance(const double L);

    /// @brief returns the total inductance
    double GetInductance() const { return fIndct; }

    /// @brief  returns the time constance
    /// @detail time constance: \f$ \tau = \frac{L}{R} f$
    double GetTimeConstance() const;

    /// @brief setup the initial current
    void SetCurrent(const double I0);

    /// @brief setup material RRR
    void SetRRR(const double RRR);

    /// @brief setup magnetic field
    void SetField(const double B);

    /// @brief setup initial temperature
    void SetInitialTemp(const double T0);

    /// @brief setup coil handler
    void SetCoilHandler(Quench::XCoilHandle* coil) { fCoil = coil; }

    /// @brief  calculate miits from current
    /// @detail calculate miits by integrating the current \f$ \int_0^{\infty} I(t)^2 dt \f$,
    ///         assuming the current is decaied following the dump resistor and inductance,
    ///         \f$ Miits = \frac{L}{2R} I_0^2 \f$
    double GetMiits() const;

    /// @brief  calculate miits from current integration
    /// @detail miits: \f$ \int_0^{\infty} J_0 exp(-2\frac{R}{L}t) dt  f$
    double GetMiits(const double tf) const;

    /// @brief calculate miits from material property
    std::map<double, double> Eval(const double T0, const double Tf);

    /// @brief interpolate and calculate the maximum temperature from the given Miits
    double GetMaxTemperature(const double miits);

    /// @brief  return the minimal propagation zone
    /// @detail MPZ: \f$ l = \sqrt{\frac{2k(T_c - T_0)}{J_c^2 \rho}} \f$
    double GetMPZ() const;
    

  private:
    double fResist;
    double fIndct;
    double fCurr;
    double fField;
    double fRRR;
    double fTemp0;
    Quench::XCoilHandle* fCoil;
};

#endif

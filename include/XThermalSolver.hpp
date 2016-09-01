/**
 *  @file   XThermalSolver.hpp
 *  @author Y.Yang (Kyushu University)
 *  @date   31 Aug 2016
 **/

#ifndef XThermalSolver_HH
#define XThermalSolver_HH

#include "XProcessManager.hpp"

enum Cooling
{
  kLeft,
  kRight,
  kSide
};

/// class to solve the thermal transfer equation
//
class XThermalSolver
{
  public:
    /// @brief constructor
    XThermalSolver();

    /// @brief deconstructor
    ~XThermalSolver();

    /// @brief setup process manager
    void SetProcessHandle(Quench::XProcessManager* hand);

    /// @brief setup time interval
    void SetTimeInterval(const double dt) { fdt = dt; }

    /// @brief setup accelerating factor
    void SetAccelerateFactor(const double acc=1.);
    
    /// @brief initialization
    void Initial();

    /// @brief solve the thermal problem
    void Solve();

    /// @brief setup boundary
    void SetBoundary();

    /// @brief connect cell to cell
    void Connect(const int from_z, const int from_p, const int from_r,
                 const int to_z  , const int to_p  , const int to_r);
    void Connect(const int from_id, const int to_id);

    /// @brief setup cooling to given temperature
    /// @param r the quench code set the default cooling path is along r diection.
    ///          and default cooling path is at strip. if the r!=kStrip, then warning
    ///          will be output.
    /// @param opt option for selecting the way to cool down the magnet, kSide means
    ///            the both side cooling.
    /// @detail this function must be used after SetBoundary()
    void SetCoolingPath(const int r, const double T=4.5*K, const Cooling opt=kSide);

    /// @brief return process handler
    Quench::XProcessManager* GetProcess() { return fProcess; }

    /// @brief print info
    void Print();


  protected:
    /// @brief in the 3d loop
    void InTheLoop(const int i, const int j, const int k);


  private:
    Quench::XProcessManager* fProcess;
    int fMshZ;
    int fMshP;
    int fMshR;
    double fAcce;
    double fdt;
};

#endif
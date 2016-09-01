/**
 *  @file   XTransientLoop.hpp
 *  @author Y.Yang (Kyushu University)
 *  @date   1 Sep 2016
 **/

#ifndef XTransientLoop_HH
#define XTransientLoop_HH

class XThermalSolver;

/// base time loop class -> QuenchLoop, ThermalLoop
class XTransientLoop
{
  public:
    /// @brief constructor
    XTransientLoop();

    /// @brief deconstructor
    ~XTransientLoop();

    /// @brief setup time mesh
    void SetTime(const double t0, const double tf, const double dt);

    /// @brief begin of run
    void Begin();

    /// @brief setup process manager
    void SetProcess(Quench::XProcessManager* hand);

    /// @brief run
    void Run();


  private:
    XThermalSolver* fSolver;
    double fTime0;
    double fTimef;
    double fdt;
};

#endif

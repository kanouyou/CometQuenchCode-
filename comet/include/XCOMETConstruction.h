/**
 * @file   XCOMETConstruction.h
 * @author Y.Yang (Kyushu University)
 * @date   7 Oct 2016
 **/

#ifndef XCOMETConstruction_HH

#include "XThermalSolver.hpp"
#include "XFieldHandle.hpp"
#include "XCoilHandle.hpp"
#include "XQuenchTransient.hpp"

using namespace Quench;

class XCOMETConstruction : public XQuenchTransient
{
  public:
    /// constructor
    XCOMETConstruction();

    /// deconstructor
    ~XCOMETConstruction();

    /// @brief calculate the coil total resistance
    double GetCoilResistance(XThermalSolver* solver);

    /// @brief construct cs0 coil
    void ConstructCS0();

    /// @brief construct cs1 coil
    void ConstructCS1();

    /// @brief update the resistance of conductor
    /// @detail please use this function after SetMaterial() and the current decay calculation
    void UpdateQuench(XThermalSolver* solve);

    /// @brief begin of run
    virtual void Begin();

    /// @brief run
    virtual void Run();

    /// @brief end of run
    virtual void End();

    /// @brief connect two magnets
    void ConnectMagnet(XThermalSolver* mag1, XThermalSolver* mag2);


  protected:
    /// @brief setup conductor size
    void SetConductor(XCoilHandle* hand);

    /// @brief setup strip size
    void SetStrip(XCoilHandle* hand);

    /// @brief setup shell size
    void SetShell(XCoilHandle* hand);

    /// @brief construct magnetic field
    void ConstructField(XFieldHandle* fld);
    

  private:
    XFieldHandle* fFld;
    XThermalSolver* fCS0;
    XThermalSolver* fCS1;
};

#endif

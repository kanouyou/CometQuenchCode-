#include <iostream>
#include "XQuenchLogger.hpp"
#include "XQuenchExcept.hpp"
#include "XThermalSolver.hpp"
#include "XTransientLoop.hpp"

using Quench::XQuenchLogger;

XTransientLoop :: XTransientLoop()
    : fSolver(NULL),
      fTime0(0.*sec),
      fTimef(60.*sec),
      fdt(1.*msec),
      fDisplay(500)
{
  if (!fSolver) fSolver = new XThermalSolver();
}

XTransientLoop :: ~XTransientLoop()
{
  if (fSolver) delete fSolver;
}

void XTransientLoop :: SetTime(const double t0, const double tf, const double dt)
{
  fTime0 = t0;
  fTimef = tf;
  fdt    = dt;

  QuenchError( XQuenchLogger::INFO, "setup time -> t0:" << fTime0 << "sec, tf:"
                                    << fTimef << "sec, dt:" << fdt << "sec" );
}

void XTransientLoop :: Begin()
{
  std::cout << "running the thermal transient loop..." << std::endl;
}

void XTransientLoop :: SetProcess(Quench::XProcessManager* hand)
{
  fSolver->SetProcessHandle(hand);
}

void XTransientLoop :: Run()
{
  double dt = fdt;
  fSolver->SetTimeInterval(dt);

  double time = fTime0;
  const double Tcool = 4.5*K;
  int cnt = 0;

  while (time<fTimef) {

    //fSolver->GetProcess()->SetMaterial();

    fSolver->Solve(dt);

    fSolver->SetBoundary();

    // setup cooling path
    // two side configuration
    fSolver->SetCoolingPath(1, Tcool, kSide);
    fSolver->SetCoolingPath(3, Tcool, kSide);
    fSolver->SetCoolingPath(5, Tcool, kSide);
    fSolver->SetCoolingPath(7, Tcool, kSide);
    fSolver->SetCoolingPath(9, Tcool, kSide);
    fSolver->SetCoolingPath(11, Tcool, kSide);
    fSolver->SetCoolingPath(13, Tcool, kSide);
    fSolver->SetCoolingPath(15, Tcool, kSide);
    fSolver->SetCoolingPath(17, Tcool, kSide);

    // original configuration
    /*
    fSolver->SetCoolingPath(1, Tcool, kSide);
    fSolver->SetCoolingPath(3, Tcool, kLeft);
    fSolver->SetCoolingPath(5, Tcool, kRight);
    fSolver->SetCoolingPath(7, Tcool, kLeft);
    fSolver->SetCoolingPath(9, Tcool, kRight);
    fSolver->SetCoolingPath(11, Tcool, kLeft);
    fSolver->SetCoolingPath(13, Tcool, kRight);
    fSolver->SetCoolingPath(15, Tcool, kLeft);
    fSolver->SetCoolingPath(17, Tcool, kRight);
    */

    // setup cooling point on shell
    for (unsigned int i=0; i<fSolver->GetProcess()->GetMesh(iZ); i++) {
      fSolver->SetLastCoolingPoint(i, Tcool);
    }

    if (cnt%fDisplay==0) {
      std::cout << "time: " << time << " [sec], step: " << dt << " [sec] ";
      fSolver->Print();
    }

    dt = fSolver->FindTimeStep();

    time += dt;
    cnt ++;
  }

}

void XTransientLoop :: End()
{
  for (int i=0; i<fSolver->GetProcess()->GetMaterialEntries(); i++) {
    std::cout << fSolver->GetProcess()->GetDimensionEntry(i)->GetId(iZ) << "  "
              << fSolver->GetProcess()->GetDimensionEntry(i)->GetId(iPhi) << "  "
              << fSolver->GetProcess()->GetDimensionEntry(i)->GetId(iR) << "  "
              << fSolver->GetProcess()->GetMaterialEntry(i)->GetTemperature() << std::endl;
  }
}

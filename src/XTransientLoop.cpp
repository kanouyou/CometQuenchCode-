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
      fdt(1.*msec)
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
}

void XTransientLoop :: SetProcess(Quench::XProcessManager* hand)
{
  fSolver->SetProcessHandle(hand);
}

void XTransientLoop :: Run()
{
  fSolver->SetTimeInterval(fdt);

  double time = fTime0;
  const double Tcool = 4.5*K;
  int cnt = 0;

  while (time<fTimef) {

    fSolver->Initial();

    fSolver->Solve();

    fSolver->SetBoundary();

    fSolver->SetCoolingPath(1, Tcool, kSide);
    fSolver->SetCoolingPath(3, Tcool, kLeft);
    fSolver->SetCoolingPath(5, Tcool, kRight);

    if (cnt%1000==0) {
      std::cout << time << " ";
      fSolver->Print();
    }

    time += fdt;
    cnt ++;
  }

  for (int i=0; i<fSolver->GetProcess()->GetMaterialEntries(); i++) {
    std::cout << fSolver->GetProcess()->GetDimensionEntry(i)->GetId(iZ) << "  "
              << fSolver->GetProcess()->GetDimensionEntry(i)->GetId(iPhi) << "  "
              << fSolver->GetProcess()->GetDimensionEntry(i)->GetId(iR) << "  "
              << fSolver->GetProcess()->GetMaterialEntry(i)->GetTemperature() << std::endl;
  }
}

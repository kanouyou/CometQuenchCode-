#include <iostream>
#include "XQuenchLogger.hpp"
#include "XQuenchExcept.hpp"
#include "XTransientLoop.hpp"
#include "XQuenchTransient.hpp"

using Quench::XQuenchLogger;

XQuenchTransient :: XQuenchTransient()
    : fDumpRes(0.125),
      fInduct(12.69),
      fCurr(2700.),
      XTransientLoop()
{}


XQuenchTransient :: ~XQuenchTransient()
{}


void XQuenchTransient :: SetDumpResistor(const double R)
{
  fDumpRes = R;
  QuenchInfo(" set dump resistor resistance: " << fDumpRes << " [Ohm]");
}


void XQuenchTransient :: SetInductance(const double L)
{
  fInduct = L;
  QuenchInfo(" set coil inductance: " << fInduct << " [H]");
}


void XQuenchTransient :: SetCurrent(const double I)
{
  fCurr = I;
  QuenchInfo(" set operating current: " << fCurr << " [A]");
}


void XQuenchTransient :: SetVoltage(const double V)
{
  fDumpRes = V / fCurr;
  QuenchInfo(" calculating the dump resistance: " << fDumpRes << " [Ohm]"
             << " (I=" << fCurr << "[A], V=" << V << "[V]");
}


void XQuenchTransient :: Begin()
{
  std::cout << "running the quench transient loop ... " << std::endl;
}


void XQuenchTransient :: Run()
{

}


void XQuenchTransient :: End()
{

}

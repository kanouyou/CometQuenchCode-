#include <iostream>
#include <TSpline.h>
#include "XQuenchExcept.hpp"
#include "XQuenchLogger.hpp"
#include "XMatAluminium.hpp"
#include "XMatCopper.hpp"
#include "XMatNbTi.hpp"
#include "XQuenchMiits.hpp"

using Quench::XQuenchLogger;

XQuenchMiits :: XQuenchMiits()
    : fResist(0.182),
      fIndct(12.69),
      fCurr(2700.),
      fField(5.5),
      fRRR(100.),
      fTemp0(4.5),
      fCoil(NULL)
{}


XQuenchMiits :: ~XQuenchMiits()
{
  if (fCoil) delete fCoil;
}

void XQuenchMiits :: SetDumpResistor(const double R)
{
  fResist = R;

  QuenchInfo("resistance of dump resistor: " << fResist);
}


void XQuenchMiits :: SetInductance(const double L)
{
  fIndct = L;

  QuenchInfo("magnet total inductance: " << fIndct);
}


double XQuenchMiits :: GetTimeConstance() const
{
  const double tau = fIndct / fResist;
  QuenchInfo("time constance: " << tau << " sec");

  return tau;
}


void XQuenchMiits :: SetCurrent(const double I0)
{
  fCurr = I0;

  QuenchInfo("intial current: " << fCurr << " A");
}


void XQuenchMiits :: SetField(const double B)
{
  fField = B;

  QuenchInfo("(MIITs) setting field: " << fField << " Tesla");
}


void XQuenchMiits :: SetRRR(const double RRR)
{
  fRRR = RRR;

  QuenchInfo("(MIITs) setting RRR: " << fRRR);
}


void XQuenchMiits :: SetInitialTemp(const double T0)
{
  fTemp0 = T0;

  QuenchInfo("initial temperature: " << fTemp0 << " K");
}


double XQuenchMiits :: GetMiits() const
{
  const double Atot = fCoil->GetCoilParts(kConductor)->GetArea();
  const double J0 = fCurr / Atot;
  const double miits = fIndct * pow(J0, 2) / fResist / 2.;

  QuenchInfo("initial current density: " << J0 << " A/m2");
  QuenchInfo("Miits: " << miits);

  return miits;
}


double XQuenchMiits :: GetMiits(const double tf) const
{
  const int nt = 1000;
  const double dt = (tf - fTemp0) / nt;

  const double Atot = fCoil->GetCoilParts(kConductor)->GetArea();
  const double J0 = fCurr / Atot;

  double t = fTemp0;
  double miits = 0.;

  while (t<=tf) {
    miits += J0 * exp(-2 * fResist * t / fIndct) * dt;
    t += dt;
  }

  return miits;
}


std::map<double, double> XQuenchMiits :: Eval(const double T0, const double Tf)
{
  const int    nT = 1500;
  const double dT = (Tf - T0) / nT;

  double T = T0;
  std::map<double, double> miits;

  const double r_Al = fCoil->GetMaterialRatio(iAluminium);
  const double r_Cu = fCoil->GetMaterialRatio(iCopper);
  const double r_SC = fCoil->GetMaterialRatio(iNbTi);

  const double Atot = fCoil->GetCoilParts(kConductor)->GetArea();
  const double A_Al = r_Al * Atot;
  const double A_Cu = r_Cu * Atot;

  XMatCopper    cu;
  XMatAluminium al;
  XMatNbTi      sc;

  const double rho_Cu = cu.GetDensity();
  const double rho_Al = al.GetDensity();
  const double rho_SC = sc.GetDensity();
  const double rho_avg = 4000.;

  double C_Al = 0.;
  double C_Cu = 0.;
  double C_SC = 0.;
  double C_avg = 0.;
  double R_Al = 0.;
  double R_Cu = 0.;
  double R_avg = 0.;
  double res = 0.;

  double mii = 0.;

  const double RRR_Cu = 50.;
  const double l = 1.;

  while (T<=Tf) {
    cu.SetMaterialProperty(T, RRR_Cu, fField);
    al.SetMaterialProperty(T, fRRR, fField);
    sc.SetMaterialProperty(T, fRRR, fField);

    C_Al = r_Al * rho_Al * al.GetCapacity() / rho_avg;
    C_Cu = r_Cu * rho_Cu * cu.GetCapacity() / rho_avg;
    C_SC = r_SC * rho_SC * sc.GetCapacity() / rho_avg;
    C_avg = C_Al + C_Cu + C_SC;

    R_Al = al.GetResistivity() * l / A_Al;
    R_Cu = cu.GetResistivity() * l / A_Cu;
    R_avg = pow( (1./R_Al + 1./R_Cu), -1. );
    res = Atot * R_avg / l;

    mii += rho_avg * C_avg * dT / res;

    miits.insert( std::map<double, double>::value_type(T,mii) );

    T += dT;
  }

  return miits;
}


double XQuenchMiits :: GetMaxTemperature(const double miits)
{
  const double T0 = 4.5;
  const double Tf = 500.;

  std::map<double, double> miit = Eval(T0, Tf); 
  int cnt = 0;

  const int n = miit.size();
  double T[n];
  double M[n];

  for (std::map<double, double>::iterator it=miit.begin(); it!=miit.end(); it++) {
    T[cnt] = it->second;
    M[cnt] = it->first;
    cnt++;
  }

  TSpline3 sp("miits", T, M, n);
  double maxtemp = sp.Eval(miits);

  return maxtemp;
}


double XQuenchMiits :: GetMPZ() const
{
  XMatNbTi nbti;
  XMatAluminium al;
  //XMatCopper cu;

  nbti.SetField(fField);
  const double Tc = nbti.GetCriticalT();
  const double Atot = fCoil->GetCoilParts(kConductor)->GetArea();
  // take the average temperature for setting the temperature property for resistivity etc.
  const double Tavg = (Tc + fTemp0) / 2.;

  //cu.SetMaterialProperty(Tavg, fRRR, fField);
  al.SetMaterialProperty(Tavg, fRRR, fField);
  nbti.SetTemperature(Tavg);

  const double k = al.GetConductivity();
  const double rho = al.GetResistivity();
  const double Jc = nbti.GetCriticalI() / Atot;

  const double mpz = sqrt(2. * k * (Tc - fTemp0) / pow(Jc,2) / rho);

  QuenchInfo("Jc: " << Jc << " A/m2, Tc: " << Tc << " K, " <<
             "Tavg: " << Tavg << " K, k:" << k << " W/m/K, "
             "MPZ: " << mpz << " m");

  return mpz;
}

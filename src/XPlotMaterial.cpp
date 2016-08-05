#include <iostream>
#include <TMultiGraph.h>
#include <TCanvas.h>
#include <TGraph.h>
#include "XQuenchLogger.hpp"
#include "XPlotMaterial.hpp"

using Quench::XQuenchLogger;

XPlotMaterial :: XPlotMaterial()
    : fOpt("R"), fRange(NULL),
      fMat(NULL), fMg(new TMultiGraph())
{}


XPlotMaterial :: ~XPlotMaterial()
{
  if (fRange)  delete [] fRange;
  if (fMat)    delete fMat;
  if (fMg)     delete fMg;
}


void XPlotMaterial :: SetRange(const double min, const double max)
{
  if (!fRange)  fRange = new double[2];

  fRange[0] = min;
  fRange[1] = max;
}


void XPlotMaterial :: Add(const std::string& opt, const double var)
{
  if (opt=="RRR")
    fMat->SetRRR(var);
  else if (opt=="B" || opt=="field")
    fMat->SetField(var);
  else 
    QuenchError( XQuenchLogger::WARNING, "this option " << opt << " did not exist." );

  TGraph* gr = new TGraph();

  const double T0 = fRange[0];
  const double Tf = fRange[1];
  const int    nT = 200;
  const double dT = (Tf - T0) / nT;

  double T = T0;
  double par;

  for (int i=0; i<nT; i++) {
    fMat->SetTemperature(T);
    if (fOpt=="R" || fOpt=="resist")
      par = fMat->GetResistivity();
    else if (fOpt=="k" || fOpt=="conduct")
      par = fMat->GetConductivity();
    gr->SetPoint(i, T, par);
    T += dT;
  }

  fMg->Add(gr, "l");
}


void XPlotMaterial :: Plot()
{
  if (fOpt=="R" || fOpt=="resist")
    fMg->SetTitle("; Temperature [K]; Resistivity [#Omega m]");
  else if (fOpt=="k" || fOpt=="conduct")
    fMg->SetTitle("; Temperature [K]; Thermal Conductivity [W/m/K]");

  TCanvas* c0 = new TCanvas("mat", "mat", 650, 480);
  c0->SetTicks(1,1);
  c0->SetLogy();

  fMg->Draw("a");
}


#include <iostream>
#include <TCanvas.h>
#include <TGraph.h>
#include "XQuenchInfo.hpp"
#include "XQuenchExcept.hpp"
#include "XQuenchLogger.hpp"
#include "XPostInfoPlot.hpp"

using Quench::XDimensionInfo;
using Quench::XQuenchLogger;

XPostInfoPlot :: ~XPostInfoPlot()
{
  delete fProcess;
}


void XPostInfoPlot :: SetProcessManager(Quench::XProcessManager* pro)
{
  if (!pro) {
    QuenchError( XQuenchLogger::WARNING, "XProcessManager is null." );
  }

  fProcess = pro;
}


void XPostInfoPlot :: PlotNode(const bool save)
{
  TCanvas* c0 = new TCanvas("node", "node", 640, 450);
  c0->SetTicks(1,1);

  int idOfPhi = 1;
  GetNodeGraph(idOfPhi)->Draw("ap");

  if (save==true)
    c0->Print("NodePosInfo.pdf");
}


TGraph* XPostInfoPlot :: GetNodeGraph(const int phi)
{
  if (!fProcess) {
    QuenchError( XQuenchLogger::ERROR, "XProcessManager is not set." );
    XQuenchExcept except("please set the XProcessManager first." );
    throw except;
  }

  std::vector<XDimensionInfo*> dim = fProcess->GetDimensionContainer();

  TGraph* gr = new TGraph();
  int cnt = 0;

  for (std::vector<int>::size_type i=0; i<dim.size(); i++) {
    if ( dim.at(i)->GetId(iPhi)==phi ) {
      gr->SetPoint( cnt, dim.at(i)->GetPosition(iZ), dim.at(i)->GetPosition(iR) );
      cnt ++;
    }
  }

  gr->SetMarkerStyle(20);
  gr->SetMarkerColor(kRed);
  gr->SetTitle( Form("#phi = %i; Z [m]; R [m]", phi) );

  return gr;
}

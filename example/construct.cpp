#include <iostream>
#include <vector>
#include <TApplication.h>
#include "XCoilHandle.hpp"
#include "XQuenchLogger.hpp"
#include "XQuenchExcept.hpp"
#include "XCoilConductor.hpp"
#include "XCoilStrip.hpp"
#include "XCoilShell.hpp"
#include "XProcessManager.hpp"
#include "XFieldHandle.hpp"
#include "XPostInfoPlot.hpp"
#include "XQuenchOutput.hpp"
#include "XRadiationHandle.hpp"
#include "XTransientLoop.hpp"
#include "IFdmUnits.hpp"

using namespace Quench;

void ConstructField(XFieldHandle* fld)
{
  fld->SetCurrent(2700.);
  fld->AddCoil("CS0", 857.88*mm, 1038.12*mm, 672.*mm, 823.65*mm);
  fld->SetMesh("CS0", 35, 19);

  fld->AddCoil("CS1", -595.25*mm, 795.25*mm, 672.*mm, 823.65*mm);
  fld->SetMesh("CS1", 270, 19);

  fld->SetTarget("CS1");
  fld->Run();
}

void SetConductor(XCoilHandle* hand)
{
  XCoilConductor* cdt = new XCoilConductor();
  cdt->SetDimension(4.73*mm, 15.*mm);
  //cdt->SetInsSize(0.15*mm, 0.15*mm);
  cdt->SetInsSize(0.8*mm, 0.4*mm);
  hand->SetConductor( dynamic_cast<XCoilBase*>(cdt) );
}

void SetStrip(XCoilHandle* hand)
{
  XCoilStrip* strip = new XCoilStrip();
  strip->SetDimension(4.73*mm+0.15*2*mm, 1.*mm);
  //strip->SetInsSize(0.0, 0.15*mm);
  strip->SetInsSize(0.0, 0.4*mm);
  hand->SetStrip( dynamic_cast<XCoilBase*>(strip) );
}

void SetShell(XCoilHandle* hand)
{
  XCoilShell* shell = new XCoilShell();
  shell->SetDimension(4.73*mm+0.15*2*mm, 80.*mm);
  //shell->SetInsSize(0.0, 3*mm);
  shell->SetInsSize(0.0, 1*mm);
  hand->SetShell( dynamic_cast<XCoilBase*>(shell) );
}

int main(int argc, char** argv)
{

  XQuenchLogger* log = XQuenchLogger::GetInstance();
  log->Start(XQuenchLogger::DEBUG, "thermal.log");

  XCoilHandle* hand = new XCoilHandle();
  XProcessManager* man = new XProcessManager();
  XPostInfoPlot* plt = new XPostInfoPlot;
  XFieldHandle* fld = new XFieldHandle();

  XRadiationHandle* rad = new XRadiationHandle(argv[1]);

  XQuenchOutput* in  = new XQuenchOutput("input.dat", iOfstream);
  XQuenchOutput* out = new XQuenchOutput("output.dat", iOfstream);
  XQuenchOutput* geo = new XQuenchOutput("geometry.dat", iOfstream);

  try {
    std::cout << "initialization ..." << std::endl;

    const double r0 = 0.673 * m;

    hand->SetName("CS1");
    hand->SetCoilSize(0., 2.*r0*M_PI, 0.);
    hand->SetMesh(90, 4, 19);
    hand->SetCoilLayers(9);
    hand->SetCoilTurns(270);
    hand->SetMaterialRatio(7.3, 1., 0.9);
    SetConductor(hand);
    SetStrip(hand);
    SetShell(hand);
    hand->AddLayer( 1,     kStrip);
    hand->AddLayer( 2, kConductor);
    hand->AddLayer( 3,     kStrip);
    hand->AddLayer( 4, kConductor);
    hand->AddLayer( 5,     kStrip);
    hand->AddLayer( 6, kConductor);
    hand->AddLayer( 7,     kStrip);
    hand->AddLayer( 8, kConductor);
    hand->AddLayer( 9,     kStrip);
    hand->AddLayer(10, kConductor);
    hand->AddLayer(11,     kStrip);
    hand->AddLayer(12, kConductor);
    hand->AddLayer(13,     kStrip);
    hand->AddLayer(14, kConductor);
    hand->AddLayer(15,     kStrip);
    hand->AddLayer(16, kConductor);
    hand->AddLayer(17,     kStrip);
    hand->AddLayer(18, kConductor);
    hand->AddLayer(19,     kShell);

    rad->SetIrrTime(60.*day);
    man->SetCoilHandler(hand);
    man->Initialize();
    // field and RRR map
    //ConstructField(fld);
    //man->SetFieldHandler(fld);
    //man->SetRadiationHandler(rad);
    
    // uniform RRR and field
    man->SetUniformField(5.5);
    man->SetUniformRRR(kConductor, 50);
    man->SetUniformRRR(kStrip, 100);
    man->SetUniformRRR(kShell, 10);
    man->SetUniformHeatGen(0.03);
    man->SetMaterial();

    geo->WriteGeometry(man);
    geo->Close();

    in->Write(man);
    in->Close();

    std::cout << "starting the thermal calculation ..." << std::endl;

    XTransientLoop* trans = new XTransientLoop();
    trans->SetTime(0., 90., 4.e-3*msec);
    trans->SetProcess(man);
    trans->Run();
    
    out->Write(man);
    out->Close();

    std::cout << "finished" << std::endl;

  }
  catch (XQuenchExcept except) {
    delete hand;
    delete man;
    delete plt;
    delete fld;
    delete geo;
    delete out;
    delete rad;
  }

  return 0;
}

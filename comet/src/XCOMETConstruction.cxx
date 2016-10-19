#include <iostream>
#include <cmath>
#include <TString.h>
#include "XMatCopper.hpp"
#include "XMatAluminium.hpp"
#include "XMatNbTi.hpp"

#include "XCoilConductor.hpp"
#include "XCoilStrip.hpp"
#include "XCoilShell.hpp"

#include "XRootOutput.hpp"
#include "XQuenchExcept.hpp"
#include "XQuenchLogger.hpp"
#include "XCOMETConstruction.h"


XCOMETConstruction :: XCOMETConstruction()
    : fFld(NULL),
      fCS0(NULL),
      XQuenchTransient()
{
  if (!fFld)  fFld = new XFieldHandle();
  ConstructField(fFld);
}


XCOMETConstruction :: ~XCOMETConstruction()
{
  if (fFld)  delete fFld;
  if (fCS0)  delete fCS0;
}


void XCOMETConstruction ::ConstructCS0()
{
  const double r = (672.+823.65)/2.*mm;
  const std::string name = "CS0";
  
  XCoilHandle* coil = new XCoilHandle();
  coil->SetName(name);
  coil->SetCoilSize(0., 2.*M_PI*r, 0.);
  coil->SetMesh(35, 4, 19);
  coil->SetCoilLayers(9);
  coil->SetCoilTurns(35);
  coil->SetMaterialRatio(7.3, 1, 0.9);

  // set coil parts
  SetConductor(coil);
  SetStrip(coil);
  SetShell(coil);

  // set coil structure
  coil->AddLayer(1, kStrip);
  coil->AddLayer(2, kConductor);
  coil->AddLayer(3, kStrip);
  coil->AddLayer(4, kConductor);
  coil->AddLayer(5, kStrip);
  coil->AddLayer(6, kConductor);
  coil->AddLayer(7, kStrip);
  coil->AddLayer(8, kConductor);
  coil->AddLayer(9, kStrip);
  coil->AddLayer(10, kConductor);
  coil->AddLayer(11, kStrip);
  coil->AddLayer(12, kConductor);
  coil->AddLayer(13, kStrip);
  coil->AddLayer(14, kConductor);
  coil->AddLayer(15, kStrip);
  coil->AddLayer(16, kConductor);
  coil->AddLayer(17, kStrip);
  coil->AddLayer(18, kConductor);
  coil->AddLayer(19, kShell);

  // set processor
  XProcessManager* pro = new XProcessManager();
  pro->SetCoilHandler(coil);
  pro->Initialize();
  
  // get magnetic field map
  //fFld->SetTarget(name);
  //fFld->Run();
  //coil->SetFieldHandler(fFld);
  
  // uniform RRR and magnetic field
  pro->SetUniformField(5.0);
  pro->SetUniformRRR(kConductor, 100.);
  pro->SetUniformRRR(kStrip, 100.);
  pro->SetUniformRRR(kShell, 10.);

  // fill the materail info into the coil solver
  if (!fCS0) fCS0 = new XThermalSolver();
  fCS0->SetProcessHandle(pro);
}


void XCOMETConstruction :: ConstructField(XFieldHandle* fld)
{
  fld->SetCurrent(2700.*Amp);
  fld->AddCoil( "CS0", 857.88*mm, 1038.12*mm, 672.*mm, 823.65*mm );
  fld->SetMesh( "CS0", 35, 19 );

  fld->AddCoil( "CS1", -595.25*mm, 795.25*mm, 672.*mm, 823.65*mm );
  fld->SetMesh( "CS1", 270, 19 );
}


void XCOMETConstruction :: SetConductor(XCoilHandle* hand)
{
  XCoilConductor* cdt = new XCoilConductor();
  cdt->SetDimension( 4.73*mm, 15.*mm );
  cdt->SetInsSize( 0.15*mm, 0.15*mm );
  hand->SetConductor( dynamic_cast<XCoilBase*>(cdt) );
}


void XCOMETConstruction :: SetStrip(XCoilHandle* hand)
{
  XCoilStrip* strip = new XCoilStrip();
  strip->SetDimension( 4.73*mm+0.15*2*mm, 1.*mm );
  strip->SetInsSize( 0., 0.25*mm );
  hand->SetStrip( dynamic_cast<XCoilBase*>(strip) );
}


void XCOMETConstruction :: SetShell(XCoilHandle* hand)
{
  XCoilShell * shell = new XCoilShell();
  shell->SetDimension( 4.73*mm+0.15*2*mm, 80.*mm );
  shell->SetInsSize( 0., 3.*mm );
  hand->SetShell( dynamic_cast<XCoilBase*>(shell) );
}


double XCOMETConstruction :: GetCoilResistance(XThermalSolver* solve)
{
  double res = 0.;
  
  for (unsigned int i=0; i<solve->GetProcess()->GetMaterialEntries(); i++) {
    if (solve->GetProcess()->GetDimensionEntry(i)->GetGeometry()==kConductor)
      res += solve->GetProcess()->GetMaterialEntry(i)->GetResistance();
  }

  return res;
}


void XCOMETConstruction :: UpdateQuench(XThermalSolver* solve)
{
  XMatCopper    cu;
  XMatAluminium al;
  XMatNbTi      sc;

  double T     = 0.;
  double RRR   = 0.;
  double B     = 0.;
  double R_Al  = 0.;
  double R_Cu  = 0.;
  double R_avg = 0.;
  double Tcs   = 0.;
  double Tc    = 0.;
  double Rcs   = 0.;

  const double factor = solve->GetProcess()->GetCoilHandler()->GetApproachZ();

  const double l_Phi = solve->GetProcess()->GetCoilHandler()->GetCoilSize(iPhi) / solve->GetProcess()->GetMesh(iPhi);
  const double r_Cu  = solve->GetProcess()->GetCoilHandler()->GetMaterialRatio(iCopper);
  const double r_Al  = solve->GetProcess()->GetCoilHandler()->GetMaterialRatio(iAluminium);
  const double A_cdt = solve->GetProcess()->GetCoilHandler()->GetCoilParts(kConductor)->GetArea();

  const double A_Cu  = A_cdt * r_Cu;
  const double A_Al  = A_cdt * r_Al;

  double Volume = solve->GetProcess()->GetCoilHandler()->GetCoilParts(kConductor)->GetTotalArea() * l_Phi;

  for (unsigned int i=0; i<solve->GetProcess()->GetMaterialEntries(); i++) {
    if (solve->GetProcess()->GetDimensionEntry(i)->GetGeometry()==kConductor) {
      T   = solve->GetProcess()->GetMaterialEntry(i)->GetTemperature();
      RRR = solve->GetProcess()->GetMaterialEntry(i)->GetRRR();
      B   = solve->GetProcess()->GetMaterialEntry(i)->GetField();

      // update Temperature, RRR and Field
      cu.SetMaterialProperty(T, 50., B);
      al.SetMaterialProperty(T, RRR, B);
      sc.SetMaterialProperty(T, RRR, B);

      // calculate Tcs and Tc
      Tcs = sc.GetSharingT( fCurr );
      Tc  = sc.GetCriticalT();

      // calculate resistance
      R_Al  = al.GetResistivity() * l_Phi / A_Al;
      R_Cu  = cu.GetResistivity() * l_Phi / A_Cu;
      R_avg = pow(1./R_Al + 1./R_Cu, -1);
      
      // setup quench status
      if ( T<Tcs ) {
        solve->GetProcess()->GetMaterialEntry(i)->SetStatus(kSuperconduct);
        solve->GetProcess()->GetMaterialEntry(i)->SetResistance(0.);
        solve->GetProcess()->GetMaterialEntry(i)->SetVoltage(0.);
        solve->GetProcess()->GetMaterialEntry(i)->SetHeat(0.);
      }
      else if ( T>=Tcs && T<Tc ) {
        Rcs = R_avg * (T-Tcs) / (Tc-Tcs);
        solve->GetProcess()->GetMaterialEntry(i)->SetStatus(kTransition);
        solve->GetProcess()->GetMaterialEntry(i)->SetResistance( Rcs );
        solve->GetProcess()->GetMaterialEntry(i)->SetVoltage( Rcs*fCurr );
        solve->GetProcess()->GetMaterialEntry(i)->SetHeat( pow(fCurr,2)*Rcs/Volume );
      }
      else if ( T>=Tc ) {
        solve->GetProcess()->GetMaterialEntry(i)->SetStatus(kNormal);
        solve->GetProcess()->GetMaterialEntry(i)->SetResistance(R_avg);
        solve->GetProcess()->GetMaterialEntry(i)->SetVoltage( R_avg*fCurr );
        solve->GetProcess()->GetMaterialEntry(i)->SetHeat( pow(fCurr,2)*R_avg/Volume );
      }

    }
    else {
      solve->GetProcess()->GetMaterialEntry(i)->SetResistance(0.);
      solve->GetProcess()->GetMaterialEntry(i)->SetVoltage(0.);
      solve->GetProcess()->GetMaterialEntry(i)->SetHeat(0.);
    }
  }
}


void XCOMETConstruction :: Begin()
{
  std::cout << "start the quench simulation for COMET. " << std::endl;
  std::cout << "....................................................." << std::endl;
  std::cout << " Running" << std::endl;
  std::cout << "....................................................." << std::endl;
}


void XCOMETConstruction :: Run()
{
  const double Tcool = 4.5*K;

  double dt = fdt;
  fCS0->SetTimeInterval(dt);

  double time = fTime0;
  int cnt = 0;
  int ocnt = 0;
  double CoilRes = 0.;
  double qchtime = fTimef;
  bool   quenched = false;
  bool   preqch   = false;

  while (time<fTimef) {
    
    // 1. update material thermal conductivity, capacity
    fCS0->GetProcess()->SetMaterial();

    // 2. update material resistivity, voltage
    UpdateQuench(fCS0); 

    // 3. calculate coil resistance
    CoilRes = 0.;
    CoilRes += GetCoilResistance(fCS0);

    preqch = quenched;

    if ( fVth<CoilRes*fCurr )
      quenched = true;
    else
      quenched = false;

    if ( preqch==false && quenched==true )
      qchtime = time + fDetTime;

    // 4. calculate the current decay
    if ( quenched==true && time>qchtime ) {
      fCurr = CalCurrentDecay(fPreI, CoilRes, dt);
      fPreI = fCurr;

    // 5. calculate the field decay
      CalFieldDecay(fCS0);
    }

    // set heat generation before quench
    if ( quenched==false )
      fCS0->GetProcess()->GetMaterialEntry(fCS0->GetProcess()->Id(3,2,8))->SetHeat(5000.*15);

    // 6. solve the thermal equation
    fCS0->Solve(dt);
    
    fCS0->SetBoundary();

    if (cnt%fDisplay==0) {
      std::cout << "time: " << time << " [sec], step: " << dt << " [sec], Rtot: "
                << CoilRes  << " [Ohm], Vtot: " << CoilRes*fCurr << " [V], I: "
                << fCurr << " [A]";
      fCS0->Print();
    }

    // fill data into file
    if (cnt%(fDisplay*5)==0) {
      XRootOutput output( Form("./output/qchout%i.root",ocnt) );
      output.SetSubDirectory("CS0");
      output.SetHeader(cnt, time, fCurr, CoilRes, CoilRes*fCurr);
      output.Fill("CS0", fCS0->GetProcess());
      output.Close();

      ocnt ++;
    }

    dt = fCS0->FindTimeStep();
    
    time += dt;
    cnt ++;
  }

}


void XCOMETConstruction :: End()
{
  std::cout << "....................................................." << std::endl;
  std::cout << " Finished" << std::endl;
  std::cout << "....................................................." << std::endl;
}



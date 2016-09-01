#include <iostream>
#include <cmath>
#include "XQuenchLogger.hpp"
#include "XQuenchExcept.hpp"
#include "XThermalSolver.hpp"

using Quench::XQuenchLogger;

XThermalSolver :: XThermalSolver()
    : fProcess(NULL),
      fMshZ(0),
      fMshP(0),
      fMshR(0),
      fAcce(1.),
      fdt(0.1*sec)
{}

XThermalSolver :: ~XThermalSolver()
{
  if (fProcess)  delete fProcess;
}

void XThermalSolver :: SetProcessHandle(Quench::XProcessManager* hand)
{
  if (!hand) {
    QuenchError( XQuenchLogger::ERROR, "process handler is null." );
    XQuenchExcept except("process handler is null.");
    throw except;
  }

  fProcess = hand;

  fMshZ = fProcess->GetMesh(iZ);
  fMshP = fProcess->GetMesh(iPhi);
  fMshR = fProcess->GetMesh(iR);
}

void XThermalSolver :: Initial()
{
  int id = 0;
  double T = 4.5;

  for (int k=0; k<fMshR+2; k++) {
    for (int j=0; j<fMshP+2; j++) {
      for (int i=0; i<fMshZ+2; i++) {
        id = fProcess->Id(i,j,k);
        T = fProcess->GetMaterialEntry(id)->GetTemperature();
        fProcess->GetMaterialEntry(id)->SetPreTemp(T);
      }
    }
  }
}

void XThermalSolver :: Solve()
{
  for (int k=1; k<fMshR+1; k++) {
    for (int j=1; j<fMshP+1; j++) {
      for (int i=1; i<fMshZ+1; i++) {
        InTheLoop(i, j, k);
      }
    }
  }
}

void XThermalSolver :: InTheLoop(const int i, const int j, const int k)
{
  // calculate id
  const int zpr   = fProcess->Id(i, j, k);
  const int preZ  = fProcess->Id(i-1, j, k);
  const int postZ = fProcess->Id(i+1, j, k);
  const int preP  = fProcess->Id(i, j-1, k);
  const int postP = fProcess->Id(i, j+1, k);
  const int preR  = fProcess->Id(i, j, k-1);
  const int postR = fProcess->Id(i, j, k+1);

  // get pre-temperature
  double T      = fProcess->GetMaterialEntry(zpr)->GetPreTemp();
  double preTz  = fProcess->GetMaterialEntry(preZ)->GetPreTemp();
  double postTz = fProcess->GetMaterialEntry(postZ)->GetPreTemp();
  double preTp  = fProcess->GetMaterialEntry(preP)->GetPreTemp();
  double postTp = fProcess->GetMaterialEntry(postP)->GetPreTemp();
  double preTr  = fProcess->GetMaterialEntry(preR)->GetPreTemp();
  double postTr = fProcess->GetMaterialEntry(postR)->GetPreTemp();

  // get distance
  double dpreZ  = fProcess->GetDimensionEntry(zpr)->GetPosition(iZ) -
                  fProcess->GetDimensionEntry(zpr)->GetPrePosition(iZ);
  double dpostZ = fProcess->GetDimensionEntry(zpr)->GetPostPosition(iZ) - 
                  fProcess->GetDimensionEntry(zpr)->GetPosition(iZ);

  double dpreP  = fProcess->GetDimensionEntry(zpr)->GetPosition(iPhi) -
                  fProcess->GetDimensionEntry(zpr)->GetPrePosition(iPhi);
  double dpostP = fProcess->GetDimensionEntry(zpr)->GetPostPosition(iPhi) - 
                  fProcess->GetDimensionEntry(zpr)->GetPosition(iPhi);

  double dpreR  = fProcess->GetDimensionEntry(zpr)->GetPosition(iR) -
                  fProcess->GetDimensionEntry(zpr)->GetPrePosition(iR);
  double dpostR = fProcess->GetDimensionEntry(zpr)->GetPostPosition(iR) - 
                  fProcess->GetDimensionEntry(zpr)->GetPosition(iR);

  // get cell size
  const double lz = fProcess->GetDimensionEntry(zpr)->GetCellSize(iZ);
  const double lp = fProcess->GetDimensionEntry(zpr)->GetCellSize(iPhi);
  const double lr = fProcess->GetDimensionEntry(zpr)->GetCellSize(iR);

  // get density
  const double rho = fProcess->GetMaterialEntry(zpr)->GetDensity();

  // get heat capacity
  const double C = fProcess->GetMaterialEntry(zpr)->GetCapacity();

  // get thermal conductivity
  const double kz = fProcess->GetMaterialEntry(zpr)->GetConductivity(iZ);
  const double kp = fProcess->GetMaterialEntry(zpr)->GetConductivity(iPhi);
  const double kr = fProcess->GetMaterialEntry(zpr)->GetConductivity(iR);

  // get heat generation 
  const double gen = fProcess->GetMaterialEntry(zpr)->GetDeposit() * rho;

  // calculate heat flux grad [W/m2]
  double qz = kz * ( (postTz-T)/dpostZ/lz - (T-preTz)/dpreZ/lz );
  double qp = kp * ( (postTp-T)/dpostP/lp - (T-preTp)/dpreP/lp );
  double qr = kr * ( (postTr-T)/dpostR/lr - (T-preTr)/dpreR/lr );

  // set heat flux
  fProcess->GetMaterialEntry(zpr)->SetHeatFlux( qz*fdt, qp*fdt, qr*fdt );

  double Q = (qz + qp + qr + gen) / rho / C;
  double Temp = T + fdt*Q;

  fProcess->GetMaterialEntry(zpr)->SetTemperature(Temp);
}

void XThermalSolver :: SetBoundary()
{
  ///////////////////////////////////////
  // R DIRECTION
  ///////////////////////////////////////
  int id_bdy  = 0;
  int id_edge = 0;
  double T = 0.;

  // adiabatic boundary
  for (int j=0; j<fMshP+2; j++) {
    for (int i=0; i<fMshZ+2; i++) {
      id_bdy  = fProcess->Id(i,j,0);
      id_edge = fProcess->Id(i,j,1);
      T = fProcess->GetMaterialEntry(id_edge)->GetTemperature();
      fProcess->GetMaterialEntry(id_bdy)->SetTemperature(T);

      id_bdy  = fProcess->Id(i,j,fMshR+1);
      id_edge = fProcess->Id(i,j,fMshR);
      T = fProcess->GetMaterialEntry(id_edge)->GetTemperature();
      fProcess->GetMaterialEntry(id_bdy)->SetTemperature(T);
    }
  }

  ///////////////////////////////////////
  // Z DIRECTION
  ///////////////////////////////////////
  // abiabatic boundary
  for (int k=0; k<fMshR+2; k++) {
    for (int j=0; j<fMshP+2; j++) {
      id_bdy  = fProcess->Id(0,j,k);
      id_edge = fProcess->Id(1,j,k);
      T = fProcess->GetMaterialEntry(id_edge)->GetTemperature();
      fProcess->GetMaterialEntry(id_bdy)->SetTemperature(T);

      id_bdy  = fProcess->Id(fMshZ+1,j,k);
      id_edge = fProcess->Id(fMshZ,j,k);
      T = fProcess->GetMaterialEntry(id_edge)->GetTemperature();
      fProcess->GetMaterialEntry(id_bdy)->SetTemperature(T);
    }
  }

  ///////////////////////////////////////
  // CONDUCTOR DIRECTION
  ///////////////////////////////////////
  // connection
  for (int k=0; k<fMshR+2; k++) {
    for (int i=0; i<fMshZ+2; i++) {
      id_bdy  = fProcess->Id(i,0,k);
      id_edge = fProcess->Id(i,fMshP,k);
      Connect(id_edge, id_bdy);

      id_bdy  = fProcess->Id(i,fMshP+1,k);
      id_edge = fProcess->Id(i,1,k);
      Connect(id_edge, id_bdy);
      // leave the first and last turn
      if ( i>1 && i<fMshZ) {
        id_bdy  = fProcess->Id(i,0,k);
        id_edge = fProcess->Id(i-1,fMshP,k);    // privous turn
        T = fProcess->GetMaterialEntry(id_edge)->GetTemperature();
        fProcess->GetMaterialEntry(id_bdy)->SetTemperature(T);

        id_bdy  = fProcess->Id(i,fMshP+1,k);
        id_edge = fProcess->Id(i+1,1,k);
        Connect(id_edge, id_bdy);
      }
    }
  }
 
}

void XThermalSolver :: Connect(const int from_z, const int from_p, const int from_r,
                               const int to_z,   const int to_p,   const int to_r)
{
  int from_id = fProcess->Id(from_z, from_p, from_r);
  int to_id   = fProcess->Id(to_z, to_p, to_r);
  
  double T = fProcess->GetMaterialEntry(from_id)->GetTemperature();
  fProcess->GetMaterialEntry(to_id)->SetTemperature(T);
}

void XThermalSolver :: Connect(const int from_id, const int to_id)
{
  double T = fProcess->GetMaterialEntry(from_id)->GetTemperature();
  fProcess->GetMaterialEntry(to_id)->SetTemperature(T);
}

void XThermalSolver :: SetCoolingPath(const int r, const double T, const Cooling opt)
{
  int bdy = fProcess->Id(1,1,r);

  if (fProcess->GetDimensionEntry(bdy)->GetGeometry()!=kStrip) {
    QuenchError( XQuenchLogger::WARNING, "layer: " << r << " is not strip.");
  }

  // set cooling path
  for (int j=0; j<fMshP+2; j++) {
    if (opt==kLeft) {
      bdy  = fProcess->Id(0,j,r);
      fProcess->GetMaterialEntry(bdy)->SetTemperature(T);
    }
    else if (opt==kRight) {
      bdy = fProcess->Id(fMshZ+1,j,r);
      fProcess->GetMaterialEntry(bdy)->SetTemperature(T);
    } 
    else if (opt==kSide) {
      bdy  = fProcess->Id(0,j,r);
      fProcess->GetMaterialEntry(bdy)->SetTemperature(T);

      bdy = fProcess->Id(fMshZ+1,j,r);
      fProcess->GetMaterialEntry(bdy)->SetTemperature(T);
    }
  }
}

void XThermalSolver :: Print()
{
  int id = fProcess->Id(fMshZ/2, fMshP/2, fMshR/2);
  std::cout << fProcess->GetMaterialEntry(id)->GetTemperature() << std::endl;
}

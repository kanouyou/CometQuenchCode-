#include <iostream>
#include <cmath>
#include <map>

#include "XMatAluminium.hpp"
#include "XMatCopper.hpp"
#include "XMatNbTi.hpp"
#include "XMatKapton.hpp"

#include "XQuenchExcept.hpp"
#include "XQuenchLogger.hpp"
#include "XProcessManager.hpp"

using Quench::XQuenchLogger;
using Quench::XProcessManager;


XProcessManager :: XProcessManager()
    : XMeshLoop(), 
      fCoil(NULL),
      fName("")
{}


XProcessManager :: ~XProcessManager()
{
  if (fCoil)  delete fCoil;
}


void XProcessManager :: SetCoilHandler(XCoilHandle* handler)
{
  fCoil = handler;
  // update mesh
  const int z = fCoil->GetMesh(iZ);
  const int p = fCoil->GetMesh(iPhi);
  const int r = fCoil->GetMesh(iR);
  SetMesh(z, p, r);

  fName = fCoil->GetName();
}


void XProcessManager :: SetUniformField(const double fld)
{
  for (int k=1; k<fMshR+1; k++) {
    for (int j=1; j<fMshP+1; j++) {
      for (int i=1; i<fMshZ+1; i++)
        fMC.at( Id(i,j,k) )->SetField(fld);
    }
  }

  QuenchInfo( "set the uniform magnetic field: " << fld << " Tesla." );
}


void XProcessManager :: SetFieldHandler(XFieldHandle* hand)
{
  if (!hand) {
    QuenchError( XQuenchLogger::ERROR, "null field handler." );
    XQuenchExcept except("null field handler.");
    throw except;
  }
  
  if (!fCoil) {
    QuenchError( XQuenchLogger::ERROR, "no coil handler set." );
    XQuenchExcept except("please set the coil handler first.");
    throw except;
  }

  double Bz, Br, Btot;

  // fill the magnetic field into the container
  for (int k=1; k<fMshR+1; k++) {
    for (int j=1; j<fMshP+1; j++) {
      for (int i=1; i<fMshZ+1; i++) {
        Bz   = hand->GetFieldEntry(i-1, k-1)->GetField().at(0);
        Br   = hand->GetFieldEntry(i-1, k-1)->GetField().at(1);
        Btot = sqrt( pow(Bz,2) + pow(Br,2) );
        fMC.at( Id(i,j,k) )->SetField( Btot );
      }
    }
  }

  QuenchInfo( "set the calculated magnetic field." );
}


void XProcessManager :: SetUniformRRR(const Geometry part, const double RRR)
{
  std::vector<int> id = fCoil->GetLayerId(part);

  if ( id.size()==0 ) {
    QuenchError( XQuenchLogger::WARNING, "layer id size is zero." );
    throw;
  }

  for (std::vector<int>::size_type k=0; k<id.size(); k++) {
    for (int j=0; j<fMshR+2; j++) {
      for (int i=0; i<fMshZ+2; i++) 
        fMC.at( Id(i,j,k) )->SetRRR( RRR );
    }
  }

  QuenchInfo( "set the uniform RRR value: " << RRR << " for " << fCoil->GetGeometryName(part) );
}


void XProcessManager :: Initialize()
{
  if (!fCoil) {
    QuenchError( XQuenchLogger::ERROR, "coil handler is not set." );
    XQuenchExcept except("please set the coil handler first.");
    throw except;
  }

  const double T = 4.5;

  // allocate the data saving place
  init();
  InitTemp(T);
  InitPosition();
}


void XProcessManager :: Initialize(XCoilHandle* coil, XFieldHandle* fld)
{
  const double T = 4.5;

  SetCoilHandler(coil);
  init();
  InitTemp(T);
  InitPosition();
  SetFieldHandler(fld);
}


void XProcessManager :: SetMaterial()
{
  if (fMC.size()<3) {
    QuenchError( XQuenchLogger::ERROR, "container is not set." );
    XQuenchExcept except("please check the handler setup.");
    throw except;
  }

  /*
  int i = 0;  int j = 0;  int k = 0;
  double T   = fMC.at( Id(i,j,k) )->GetTemperature();
  double RRR = fMC.at( Id(i,j,k) )->GetRRR();
  double B   = fMC.at( Id(i,j,k) )->GetField();
  Geometry geo = fDC.at( Id(i,j,k) )->GetGeometry();

  while ( k<fMshR+2 ) {
    switch (geo) {
      case kConductor: 
        SetConductorMat( Id(i,j,k), T, RRR, B ); 
        break;
      case kStrip: 
        SetStripMat( Id(i,j,k), T, RRR, B ); 
        break;
      case kShell: 
        SetShellMat( Id(i,j,k), T, RRR, B ); 
        break;
      default:
        QuenchError( XQuenchLogger::WARNING, "geometry " << fCoil->GetGeometryName(geo) << " did not exist." );
        break;
    }
    j = 0;
    while ( j<fMshP+2 ) {
      i = 0;
      while ( i<fMshZ+2 ) {
        std::cout << Id(i,j,k) << " " << i << " " << fDC.at( Id(i,j,k) )->GetId(iZ)
                               << " " << j << " " << fDC.at( Id(i,j,k) )->GetId(iPhi)
                               << " " << k << " " << fDC.at( Id(i,j,k) )->GetId(iR) << std::endl;

        T   = fMC.at( Id(i,j,k) )->GetTemperature();
        RRR = fMC.at( Id(i,j,k) )->GetRRR();
        B   = fMC.at( Id(i,j,k) )->GetField();
        geo = fDC.at( Id(i,j,k) )->GetGeometry();
        i++;
      }
      j++;
    }
    k++;
  }
  */

  double T, RRR, B;
  Geometry geo;

  for (int k=0; k<fMshR+2; k++) {
    for (int j=0; j<fMshP+2; j++) {
      for (int i=0; i<fMshZ+2; i++) {
        T   = fMC.at( Id(i,j,k) )->GetTemperature();
        RRR = fMC.at( Id(i,j,k) )->GetRRR();
        B   = fMC.at( Id(i,j,k) )->GetField();
        geo = fDC.at( Id(i,j,k) )->GetGeometry();
        
        switch (geo) {
          case kConductor: 
            SetConductorMat( Id(i,j,k), T, RRR, B ); 
            break;
          case kStrip: 
            SetStripMat( Id(i,j,k), T, RRR, B ); 
            break;
          case kShell: 
            SetShellMat( Id(i,j,k), T, RRR, B ); 
            break;
          default:
            QuenchError( XQuenchLogger::WARNING, "geometry " << fCoil->GetGeometryName(geo) << " did not exist." );
            break;
        }

      }
    }
  }

}


size_t XProcessManager :: GetEntries() const
{
  if (fMC.size()!=fDC.size()) {
    QuenchError( XQuenchLogger::ERROR, "material container size is equal to the size of dimension container." );
    XQuenchExcept except("material container size is equal to the size of dimension container.");
    throw except;
  }

  return fMC.size();
}


void XProcessManager :: init()
{
  // initialize the container vector
  for (int k=0; k<fMshR+2; k++) {
    for (int j=0; j<fMshP+2; j++) {
      for (int i=0; i<fMshZ+2; i++) {
        fMC.push_back( new XMaterialInfo() );
        fDC.push_back( new XDimensionInfo );
      }
    }
  }

  // initialize id number
  for (int k=0; k<fMshR+2; k++) {
    for (int j=0; j<fMshP+2; j++) {
      for (int i=0; i<fMshZ+2; i++) {
        fDC.at( Id(i,j,k) )->SetId(i, j, k);
        fDC.at( Id(i,j,k) )->SetNodeId( Id(i,j,k) );
        fDC.at( Id(i,j,k) )->SetPosition(0., 0., 0.);
        fDC.at( Id(i,j,k) )->SetPrePosition(0., 0., 0.);
        fDC.at( Id(i,j,k) )->SetPostPosition(0., 0., 0.);
        fDC.at( Id(i,j,k) )->SetGeometry(kConductor);
      }
    }
  }

  // initialize geometry
  std::map<const int, const Geometry> coil = fCoil->GetCoilLayout();
  int k;  Geometry geo;

  for ( std::map<const int, const Geometry>::const_iterator it=coil.begin(); it!=coil.end(); ++it ) {
    k   = it->first;
    geo = it->second;
    for (int j=0; j<fMshP+2; j++) {
      for (int i=0; i<fMshZ+2; i++) {
        fDC.at( Id(i,j,k) )->SetGeometry(geo);
      }
    }
  }

}


void XProcessManager :: InitTemp(const double T)
{
  for (int k=0; k<fMshR+2; k++) {
    for (int j=0; j<fMshP+2; j++) {
      for (int i=0; i<fMshZ+2; i++) 
        fMC.at( Id(i,j,k) )->SetTemperature(T);
    }
  }
}


void XProcessManager :: InitPosition()
{
  if ( fMshR!=fCoil->GetLayoutEntries() ) {
    QuenchError( XQuenchLogger::ERROR, "total layout: " << fCoil->GetLayoutEntries()
                                       << ", layout " << fMshR << " is needed." );
    XQuenchExcept except("layout is not enough.");
    throw except;
  }

  double preZ  = 0.;  double preP  = 0.;  double preR  = 0.;        // pre-position
  double postZ = 0.;  double postP = 0.;  double postR = 0.;        // post-position

  double z  = 0.;  double p  = 0.;  double r  = 0.;        // position
  double lz = 0.;  double lp = 0.;  double lr = 0.;        // length
  double dz = 0.;  double dp = 0.;  double dr = 0.;        // distance = position - pre-position

  lz = fCoil->GetCoilLayout(2)->GetTotalLength(iZ);
  lp = fCoil->GetCoilSize(iPhi) / fMshP;

  for (int k=1; k<fMshR+1; k++) {
    lr = fCoil->GetCoilLayout(k)->GetTotalLength(iR);
    r += lr/2.;
    dr = r - preR;
    //if (k==fMshR)
    //  postR = r + lr/2.;
    //else
    //  postR = r + lr/2. + fCoil->GetCoilLayout(k+1)->GetTotalLength(iR)/2.;
    postR = k==fMshR ? r+lr/2. : r+lr/2.+fCoil->GetCoilLayout(k+1)->GetTotalLength(iR)/2.;

    p  = 0.;
    preP = 0.;
    for (int j=1; j<fMshP+1; j++) {
      p += lp/2.;
      dp = p - preP;
      postP = j==fMshR ? p+lp/2. : p+lp;

      z  = 0.;
      preZ = 0.;
      for (int i=1; i<fMshZ+1; i++) {
        z += lz/2.;
        dz = z - preZ;
        postZ = i==fMshZ ? z+lz/2. : z+lz;

        fDC.at( Id(i,j,k) )->SetPostPosition(postZ, postP, postR);
        fDC.at( Id(i,j,k) )->SetPrePosition(preZ, preP, preR);
        fDC.at( Id(i,j,k) )->SetPosition(z, p, r);
        fDC.at( Id(i,j,k) )->SetCellSize(lz, lp, lr);

        preZ = z;
        z += lz/2.;
      }
      preP = p;
      p += lp/2.;
    }
    preR = r;
    r += lr/2.;
  }

}


void XProcessManager :: SetConductorMat(const int id, const double T, const double RRR, const double B)
{
  XMatCopper    cu;
  XMatAluminium al;
  XMatNbTi      sc;
  XMatKapton    kap;

  cu.SetMaterialProperty(T, 50., B);     // copper RRR: 50
  al.SetMaterialProperty(T, RRR, B);
  sc.SetMaterialProperty(T, RRR, B);
  kap.SetMaterialProperty(T, RRR, B);

  // get density
  const double rho_Cu = cu.GetDensity();
  const double rho_Al = al.GetDensity();
  const double rho_Sc = sc.GetDensity();
  const double rho_avg = 4000.;

  fMC.at(id)->SetDensity( rho_avg );

  // get material ratio
  const double ratio_Al = fCoil->GetMaterialRatio(iAluminium);
  const double ratio_Cu = fCoil->GetMaterialRatio(iCopper);
  const double ratio_Sc = fCoil->GetMaterialRatio(iNbTi);
  
  // calculate average capacity for conductor
  const double C_Al = ratio_Al * rho_Al * al.GetCapacity() / rho_avg;
  const double C_Cu = ratio_Cu * rho_Cu * cu.GetCapacity() / rho_avg;
  const double C_Sc = ratio_Sc * rho_Sc * sc.GetCapacity() / rho_avg;
  const double C_avg = C_Al + C_Cu + C_Sc;

  fMC.at(id)->SetCapacity( C_avg );

  // calculate average thermal conductivity
  const double k_Al = al.GetConductivity();
  const double k_ins = kap.GetConductivity();

  const double lz_ins = 2. * fCoil->GetCoilParts(kConductor)->GetInsSize(iZ);
  const double lr_ins = 2. * fCoil->GetCoilParts(kConductor)->GetInsSize(iR);
  const double lz_cdt = fCoil->GetCoilParts(kConductor)->GetDimension(iZ);
  const double lr_cdt = fCoil->GetCoilParts(kConductor)->GetDimension(iR);
  const double A_cdt  = fCoil->GetCoilParts(kConductor)->GetArea();
  const double A_ins  = fCoil->GetCoilParts(kConductor)->GetInsArea();
  
  const double kz = al.GetSerialk( lz_ins, k_ins, lz_cdt, k_Al );
  const double kr = al.GetSerialk( lr_ins, k_ins, lr_cdt, k_Al );
  const double kp = al.GetParallelk( A_ins, k_ins, A_cdt, k_Al );

  fMC.at(id)->SetConductivity( kz, kp, kr );

  // calculate average resistance
  const double dl_phi = fCoil->GetCoilSize(iPhi) / fMshP;
  const double A_Cu = A_cdt * ratio_Cu;
  const double A_Al = A_cdt * ratio_Al;

  const double R_Al = al.GetResistivity() * dl_phi / A_Al;
  const double R_Cu = cu.GetResistivity() * dl_phi / A_Cu;
  const double R_avg = pow( (1./R_Al + 1./R_Cu), -1. );
  
  if ( fMC.at(id)->GetStatus()!=kSuperconduct )
    fMC.at(id)->SetResistance( R_avg );
}


void XProcessManager :: SetStripMat(const int id, const double T, const double RRR, const double B)
{
  XMatAluminium al;
  XMatKapton    kap;

  al.SetMaterialProperty(T, RRR, B);
  kap.SetMaterialProperty(T, RRR, B);

  // setup density
  fMC.at(id)->SetDensity( al.GetDensity() );

  // setup heat capacity
  fMC.at(id)->SetCapacity( al.GetCapacity() );

  // setup thermal conductivity
  const double k_Al  = al.GetConductivity();
  const double k_ins = kap.GetConductivity();

  const double lr_ins = 2. * fCoil->GetCoilParts(kStrip)->GetInsSize(iR);
  const double lr_Al  = fCoil->GetCoilParts(kStrip)->GetDimension(iR);

  const double kz = k_Al;
  const double kr = al.GetSerialk( lr_ins, k_ins, lr_Al, k_Al );
  const double kp = k_ins;

  fMC.at(id)->SetConductivity( kz, kp, kr );
  fMC.at(id)->SetResistance(0.);
}


void XProcessManager :: SetShellMat(const int id, const double T, const double RRR, const double B)
{
  XMatAluminium al;
  XMatKapton    kap;

  al.SetMaterialProperty(T, RRR, B);
  kap.SetMaterialProperty(T, RRR, B);

  // setup density
  fMC.at(id)->SetDensity( al.GetDensity() );

  // setup heat capacity
  fMC.at(id)->SetCapacity( al.GetCapacity() );

  // setup thermal conductivity
  const double k_Al  = al.GetConductivity();
  const double k_ins = kap.GetConductivity();

  const double lr_ins = fCoil->GetCoilParts(kShell)->GetInsSize(iR);
  const double lr_Al  = fCoil->GetCoilParts(kShell)->GetDimension(iR);

  const double kz = k_Al;
  const double kr = al.GetSerialk( lr_ins, k_ins, lr_Al, k_Al );
  const double kp = k_Al;

  fMC.at(id)->SetConductivity( kz, kp, kr );
  fMC.at(id)->SetResistance(0.);
}

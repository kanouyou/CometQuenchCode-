#include <iostream>
#include "XQuenchExcept.hpp"
#include "XQuenchLogger.hpp"
#include "XProcessManager.hpp"

using Quench::XQuenchLogger;
using Quench::XProcessManager;


XProcessManager :: XProcessManager()
    : XMeshLoop(), 
      fCoil(NULL)
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


void XProcessManager :: init()
{
  //XDimensionInfo* dim = NULL;

  for (int k=0; k<fMshR+2; k++) {
    for (int j=0; j<fMshP+2; j++) {
      for (int i=0; i<fMshZ+2; i++) {
        fMC.push_back( new XMaterialInfo() );
        fDC.push_back( new XDimensionInfo );
      }
    }
  }
}


void XProcessManager :: InitTemp(const double T)
{
  for (int k=0; k<fMshR+2; k++) {
    for (int j=0; j<fMshP+2; j++) {
      for (int i=0; i<fMshZ+2; i++) {
        fDC.at( Id(i,j,k) )->SetId(i, j, k);
        fDC.at( Id(i,j,k) )->SetNodeId( Id(i,j,k) );
        fDC.at( Id(i,j,k) )->SetPosition(0., 0., 0.);
        fDC.at( Id(i,j,k) )->SetPrePosition(0., 0., 0.);
        fDC.at( Id(i,j,k) )->SetPostPosition(0., 0., 0.);
        fDC.at( Id(i,j,k) )->SetDistance(0., 0., 0.);
        fMC.at( Id(i,j,k) )->SetTemperature(T);
      }
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
    if (k==fMshR)
      postR = r + lr/2.;
    else
      postR = r + lr/2. + fCoil->GetCoilLayout(k+1)->GetTotalLength(iR)/2.;

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
        fDC.at( Id(i,j,k) )->SetDistance(dz, dp, dr);

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



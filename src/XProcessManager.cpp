#include <iostream>
#include "XQuenchExcept.hpp"
#include "XQuenchLogger.hpp"
#include "XCoilHandle.hpp"
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
  //XDimensionContainer* dim = NULL;

  for (int k=0; k<fMshR+2; k++) {
    for (int j=0; j<fMshP+2; j++) {
      for (int i=0; i<fMshZ+2; i++) {
        fMC.push_back( new XMaterialContainer() );
        //std::cout << i << " " << j << " " << k << " " << Id(i,j,k) << std::endl;

        XDimensionContainer* dim = new XDimensionContainer();
        dim->SetId(i, j, k);
        dim->SetNodeId( Id(i,j,k) );
        fDC.push_back(dim);
        delete dim;
      }
    }
  }
}


void XProcessManager :: InitTemp(const double T)
{
  for (int k=0; k<fMshR+2; k++) {
    for (int j=0; j<fMshP+2; j++) {
      for (int i=0; i<fMshZ+2; i++) {
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

  double bz = 0.;  double bp = 0.;  double br = 0.;        // boundary
  double z  = 0.;  double p  = 0.;  double r  = 0.;        // position
  double lz = 0.;  double lp = 0.;  double lr = 0.;        // length
  double dz = 0.;  double dp = 0.;  double dr = 0.;        // distance

  for (int k=1; k<fMshR+1; k++) {
    lr  = fCoil->GetCoilLayout(k)->GetTotalLength(iR);
    br += lr;
    dr  = br - lr/2. - r;
    r   = br - lr/2.;
    for (int j=1; j<fMshP+1; j++) {
      lp  = fCoil->GetCoilSize(iPhi) / fMshP;
      bp += lp;
      dp  = bp - lp/2. - p;
      p   = bp - lp/2.;
      for (int i=1; i<fMshZ+1; i++) {
        lz  = fCoil->GetCoilLayout(2)->GetTotalLength(iZ);
        bz += lz;
        dz  = bz - lz/2. - z;
        z   = bz - lz/2.;
        fDC.at( Id(i,j,k) )->SetPosition(z, p, r);
        fDC.at( Id(i,j,k) )->SetCellSize(lz, lp, lr);
        fDC.at( Id(i,j,k) )->SetDistance(dz, lp, lr);
      }
    }
  }

}



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


void XProcessManager :: SetCoilHandler(const XCoilHandle* handler)
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

  // allocate the data saving place
  init();
}


void XProcessManager :: init()
{
  XDimensionContainer* dim = NULL;

  for (int k=0; k<fMshR+2; k++) {
    for (int j=0; j<fMshP+2; j++) {
      for (int i=0; i<fMshZ+2; i++) {
        fMC.push_back( new XMaterialContainer() );

        if (!dim)  dim = new XDimensionContainer();
        dim->SetId(i, j, k);
        dim->SetNodeId( Id(i,j,k) );
        fDC.push_back(dim);
        if (dim)  delete dim;
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

  double z = 0.;
  double p = 0.;
  double r = 0.;

  for (int k=1; k<fMshR+1; k++) {
    r += fCoil->GetCoilLayout(k)->GetDimension(iR);
    for (int j=1; j<fMshP+1; j++) {
      p += fCoil->GetCoilSize(iPhi) / fMshP;
      for (int i=1; i<fMshZ+1; i++) {
        z += fCoil->GetCoilLayout(k)->GetDimension(iZ);
      }
    }
  }

}



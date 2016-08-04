#include <iostream>
#include "XQuenchExcept.hpp"
#include "XMaterial.hpp"
#include "XPreProcess.hpp"

#ifndef XQuenchLogger_HH
#include "XQuenchLogger.hpp"
#endif

using Quench::XPreProcess;

XPreProcess :: XPreProcess()
    : fName(""), 
      fMshZ(-1),
      fMshP(-1),
      fMshR(-1),
      fCoil(NULL)
{} 

XPreProcess :: XPreProcess(const std::string& name)
    : fName(name), 
      fMshZ(-1),
      fMshP(-1),
      fMshR(-1),
      fCoil(NULL) 
{}


XPreProcess :: XPreProcess(const std::string& name, const XCoilHandle* coil)
    : fName(name), 
      fMshZ(-1),
      fMshP(-1),
      fMshR(-1),
      fCoil(coil)
{}


XPreProcess :: ~XPreProcess()
{
  if (fCoil)  delete fCoil;
}


void XPreProcess :: Initialize()
{
  if (!fCoil) {
    // check handler
    QuenchError( XQuenchLogger::ERROR, "coil handler is not set." );
    XQuenchExcept except("please set coil handler first.");
    throw except;
  }
  else {
    QuenchError( XQuenchLogger::INFO, "initialzed preprocesser." );
    init(fMshZ, fMshP, fMshR);
  }
}


void XPreProcess :: init(const int mz, const int mp, const int mr)
{
  XMaterialContainer* mat = NULL;
  XDimensionContainer* dim = NULL;

  /// initial temperature:
  const double temp = 4.5;

  for (int i=0; i<mr+2; i++) {
    for (int j=0; j<mp+2; j++) {
      for (int k=0; k<mz+2; k++) {
        mat = new XMaterialContainer();
        mat->SetTemperature( temp );
        fMatCollect.push_back(mat);

        dim = new XDimensionContainer();
        dim->SetId(&k, &j, &i);
        dim->SetNodeId( id(k,j,i) );
        
        fDimCollect.push_back(dim);
      }
    }
  }

}


void XPreProcess :: SetCoilHandler(const XCoilHandle* coil)
{
  if (!fCoil) {
    fCoil = coil;
    fMshZ = fCoil->GetMesh()[0];
    fMshP = fCoil->GetMesh()[1];
    fMshR = fCoil->GetMesh()[2];
    
    QuenchError( XQuenchLogger::INFO, "setting " << fName << " coil handler." );
  }

  QuenchError( XQuenchLogger::WARNING, "coil handler has been set already." );
}


// i: z, j: phi, k: r
const int XPreProcess :: id(const int i, const int j, const int k)
{
  const int ID = k*(fMshZ+2)*(fMshP+2) + j*(fMshZ+2) + i;
  return ID;
}


void XPreProcess :: SetFieldHandler(XFieldHandle* fld)
{
  double* Bi = new double[2];
  double B;

  if (fld) {
    // calculate magnetic field
    fld->SetTarget( fName );
    fld->SetMesh( fName, fMshZ, fMshR );
    fld->Run();

    // set magnetic field into container
    for (int i=1; i<fMshR+1; i++) {
      for (int j=1; j<fMshP+1; j++) {
        for (int k=1; k<fMshZ+1; k++) {
          Bi = fld->GetFieldEntry(k-1, i-1)->GetField();
          B  = sqrt( pow(Bi[0],2) + pow(Bi[1],2) );
          fMatCollect.at( id(k,j,i) )->SetField(B);
        }
      }
    }
    
  }
  else {
    QuenchError( XQuenchLogger::ERROR, "field handler is null." );
    XQuenchExcept except("field handler is null.");
    throw except;
  }
}


void XPreProcess :: setstrip(const int layer)
{
  int n = 0;

  for (int j=0; j<fMshP+2; j++) {
    for (int k=0; k<fMshZ+2; k++) {
      n = id(k, j, layer);
      fMatCollect[n]->SetDensity(2700.);
    }
  }
}

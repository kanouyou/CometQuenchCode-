#include <iostream>
#include "XQuenchExcept.hpp"
#include "XPreProcess.hpp"

//#ifndef XQuenchContainer_HH
//#include "XQuenchContainer.hpp"
//#endif

#ifndef XQuenchLogger_HH
#include "XQuenchLogger.hpp"
#endif

using Quench::XPreProcess;
//using Quench::XMaterialContainer;

XPreProcess :: XPreProcess(const std::string& name)
    : fName(name), fCoilPar(NULL), fMsh(NULL) 
{}


XPreProcess :: XPreProcess(const std::string& name, const XCoilConstruct* coil)
    : fName(name), fCoilPar(coil), fMsh(NULL)
{}


XPreProcess :: ~XPreProcess()
{
  if (fCoilPar)  delete fCoilPar;
  if (fMsh)      delete [] fMsh;
}


void XPreProcess :: Initialize()
{
  if (!fMsh) {
    QuenchError( XQuenchLogger::ERROR, "mesh is not set." );
    XQuenchExcept except("mesh is not set.");
    throw except;
  }
  else {
    QuenchError( XQuenchLogger::INFO, "initialzed preprocesser." );
    init(fMsh[0], fMsh[1], fMsh[2]);
  }
}


void XPreProcess :: init(const int mz, const int mp, const int mr)
{
  XMaterialContainer* mat = NULL;
  XDimensionContainer* dim = NULL;

  for (int i=0; i<mr+2; i++) {
    for (int j=0; j<mp+2; j++) {
      for (int k=0; k<mz+2; k++) {
        mat = new XMaterialContainer();
        fMatCollect.push_back(mat);

        dim = new XDimensionContainer();
        dim->SetId(&i, &j, &k);
        dim->SetNodeId( id(i,j,k) );
        fDimCollect.push_back(dim);
      }
    }
  }

}


void XPreProcess :: SetCoilHandler(const XCoilConstruct* coil)
{
  if (!fCoilPar) fCoilPar = coil;
  QuenchError( XQuenchLogger::CONFIG, "setup coil handler." );
}


void XPreProcess :: SetMesh(const int mz, const int mp, const int mr)
{
  if (!fMsh)  fMsh = new int[3];

  fMsh[0] = mz;
  fMsh[1] = mp;
  fMsh[2] = mr;

  QuenchError( XQuenchLogger::INFO, "mesh: {" << mz << ", " << mp << ", " << mr << "} " );
}


const int XPreProcess :: id(const int i, const int j, const int k)
{
  /// loop sequence: R -> Phi -> Z
  const int ID = i*(fMsh[0]+2)*(fMsh[1]+2) + j*(fMsh[0]+2) + k;
  return ID;
}


void XPreProcess :: SetFieldHandler(XFieldHandle* fld)
{
  std::vector<XFieldContainer*> handler;

  if (fld && fMsh) {
    // calculate magnetic field
    fld->SetMesh( fName, fMsh[0], fMsh[2] );
    handler = fld->GetFieldCollection( fName );

  }
  else {
    QuenchError( XQuenchLogger::ERROR, "field handler is null." );
    XQuenchExcept except("field handler is null.");
    throw except;
  }
}


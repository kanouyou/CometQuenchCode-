#include <iostream>
#include "XPreProcess.hpp"

#ifndef XQuenchLogger_HH
#include "XQuenchLogger.hpp"
#endif

using Quench::XPreProcess;

XPreProcess :: XPreProcess()
    : fCoilPar(NULL), fMsh(NULL) 
{}

XPreProcess :: XPreProcess(Quench::XCoilConstruct* coil)
    : fCoilPar(coil), fMsh(NULL)
{}

XPreProcess :: ~XPreProcess()
{
  if (fCoilPar)  delete fCoilPar;
  if (fMsh)      delete [] fMsh;
}

void XPreProcess :: SetMesh(const int mz, const int mp, const int mr)
{
  if (!fMsh)  fMsh = new int[3];

  fMsh[0] = mz;
  fMsh[1] = mp;
  fMsh[2] = mr;

  QuenchError( XQuenchLogger::INFO, "mesh: {" << mz << ", " << mp << ", " << mr << "} " );
}

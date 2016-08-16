#include <iostream>
#include "XQuenchExcept.hpp"
#include "XQuenchLogger.hpp"
#include "XCoilConductor.hpp"

using Quench::XQuenchLogger;
using Quench::XCoilConductor;

XCoilConductor :: XCoilConductor()
    : fSize(new double[2]),
      fTape(new double[2])
{
  // initialize the size
  if (fSize) {
    fSize[0] = -1.;
    fSize[1] = -1.;
  }

  if (fTape) {
    fTape[0] = -1.;
    fTape[1] = -1.;
  }
}


XCoilConductor :: ~XCoilConductor()
{
  if (fSize) delete [] fSize;
  if (fTape) delete [] fTape;
}


void XCoilConductor :: SetDimension(const double lz, const double lr)
{
  if (!fSize) fSize = new double[2];
  fSize[0] = lz;
  fSize[1] = lr;

  QuenchError( XQuenchLogger::INFO, "set conductor dimension -> lz: " << lz << ", lr: " << lr );
}


void XCoilConductor :: SetInsSize(const double lz, const double lr)
{
  if (!fTape) fTape = new double[2];
  fTape[0] = lz;
  fTape[1] = lr;

  QuenchError( XQuenchLogger::INFO, "set insulation dimension -> lz: " << lz << ", lr: " << lr );
}


double XCoilConductor :: GetTotalArea() const
{
  double lz = fSize[0] + 2*fTape[0];
  double lr = fSize[1] + 2*fTape[1];

  return lz * lr;
}


double XCoilConductor :: GetArea() const
{
  if (fSize)
    return fSize[0] * fSize[1];
  else {
    QuenchError( XQuenchLogger::ERROR, "please set the dimension first!" );
    XQuenchExcept except("please set the dimension first!");
    throw except;
    return 0;
  }
}


double XCoilConductor :: GetInsArea() const 
{
  double total = GetTotalArea();
  double cdt   = GetArea();
  return total - cdt;
}

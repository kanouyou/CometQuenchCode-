#include <iostream>
#include "XQuenchExcept.hpp"
#include "XQuenchLogger.hpp"
#include "XCoilStrip.hpp"

using Quench::XCoilStrip;


XCoilStrip :: XCoilStrip()
    : fSize(new double[2]),
      fIns(new double[2])
{}


XCoilStrip :: ~XCoilStrip()
{
  if (fSize)  delete [] fSize;
  if (fIns)   delete [] fIns;
}


void XCoilStrip :: SetDimension(const double lz, const double lr)
{
  if (!fSize)  fSize = new double[2];

  fSize[0] = lz;
  fSize[1] = lr;
}


void XCoilStrip :: SetInsSize(const double lz, const double lr)
{
  if (!fIns)  fIns = new double[2];

  fIns[0] = lz;
  fIns[1] = lr;
}


double XCoilStrip :: GetTotalArea() const
{
  if (!fIns || !fSize) {
    XQuenchExcept except("insulation and strip size is not set.");
    throw except;
  }

  double lz = fSize[0];
  double lr = fSize[1] + 2*fIns[1];
  return lz * lr;
}

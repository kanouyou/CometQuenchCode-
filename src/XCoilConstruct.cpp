#include <iostream>
#include "XQuenchLogger.hpp"
#include "XQuenchExcept.hpp"
#include "XCoilConstruct.hpp"

using Quench::XQuenchLogger;
using Quench::XCoilConstruct;

XCoilConstruct :: XCoilConstruct()
    : fStable(NULL), fTape(NULL),
      fRatio(NULL), fCoil(NULL)
{
  //if (!fStable)  fStable = new double[2];
  //if (!fTape)    fTape   = new double[2];
  //if (!fRatio)   fRatio  = new double[3];
  //if (!fCoil)    fCoil   = new double[3];
}

XCoilConstruct :: ~XCoilConstruct()
{
  if (fStable)  delete [] fStable;
  if (fTape)    delete [] fTape;
  if (fRatio)   delete [] fRatio;
  if (fCoil)    delete [] fCoil;
}

void XCoilConstruct :: SetStabilizer(const double lz, const double lr)
{
  if (!fStable) fStable = new double[2];

  fStable[0] = lz;
  fStable[1] = lr;

  QuenchError( XQuenchLogger::CONFIG, "stabilizer size: {" << fStable[0]
                                                           << ", " << fStable[1] << "}" );
}

void XCoilConstruct :: SetTape(const double lz, const double lr)
{
  if (!fTape)  fTape = new double[2];

  fTape[0] = lz;
  fTape[1] = lr;

  QuenchError( XQuenchLogger::CONFIG, "tape size: {" << fTape[0] << ", " << fTape[1] << "}" );
}

void XCoilConstruct :: SetMaterialRatio(const double Al, const double Cu, const double SC)
{
  if (!fRatio)   fRatio  = new double[3];

  fRatio[0] = Al;
  fRatio[1] = Cu;
  fRatio[2] = SC;

  QuenchError( XQuenchLogger::CONFIG, "material ratio: (Al:" << fRatio[0] << ", Cu:" << fRatio[1]
                                                  << ", SC:" << fRatio[2] << ")" );
}

void XCoilConstruct :: SetCoil(const double lz, const double lp, const double lr)
{
  if (!fCoil)    fCoil   = new double[3];

  fCoil[0] = lz;
  fCoil[1] = lp;
  fCoil[2] = lr;

  QuenchError( XQuenchLogger::CONFIG, "lz:" << fCoil[0] << ", lp:" << fCoil[1]
                                            << ", lr:" << fCoil[2] );
}

double XCoilConstruct :: GetStabilizer(const std::string &name) const
{
  if (name=="z" || name=="Z")
    return fStable[0];
  else if (name=="r" || name=="R")
    return fStable[1];
  else {
    QuenchError( XQuenchLogger::ERROR, "there is no such material: " << name );
    XQuenchExcept except("there is no such material!");
    throw except;
  }
}

double XCoilConstruct :: GetTape(const std::string &name) const
{
  if (name=="z" || name=="Z")
    return fTape[0];
  else if (name=="r" || name=="R")
    return fTape[1];
  else {
    QuenchError( XQuenchLogger::ERROR, "there is no such material: " << name );
    XQuenchExcept except("there is no such material!");
    throw except;
  }

}

double XCoilConstruct :: GetMaterialRatio(const std::string &name) const
{
  if (name=="Al" || name=="al")
    return fRatio[0];
  else if (name=="Cu" || name=="cu")
    return fRatio[1];
  else if (name=="SC" || name=="sc")
    return fRatio[2];
  else {
    QuenchError( XQuenchLogger::ERROR, "there is no such material: " << name );
    XQuenchExcept except("there is no such material!");
    throw except;
  }
}

double XCoilConstruct :: GetCoil(const std::string &name) const
{
  if (name=="z" || name=="Z")
    return fCoil[0];
  else if (name=="phi" || name=="Phi")
    return fCoil[1];
  else if (name=="r" || name=="R")
    return fCoil[2];
  else {
    QuenchError( XQuenchLogger::ERROR, "there is no such coordinate: " << name );
    XQuenchExcept except("there is no such coordinate!");
    throw except;
  }

}

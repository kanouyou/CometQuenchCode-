#include <iostream>
#include "XQuenchLogger.hpp"
#include "XQuenchExcept.hpp"
#include "XCoilHandle.hpp"

using Quench::XQuenchLogger;
using Quench::XCoilHandle;

XCoilHandle :: XCoilHandle()
    : fMsh(NULL), fLayer(NULL), 
      fStable(NULL), fTape(NULL),
      fRatio(NULL), fCoil(NULL),
      fStrip(1.*mm), fShell(10.*cm)
{}

XCoilHandle :: ~XCoilHandle()
{
  if (fMsh)     delete [] fMsh;
  if (fLayer)   delete [] fLayer;
  if (fStable)  delete [] fStable;
  if (fTape)    delete [] fTape;
  if (fRatio)   delete [] fRatio;
  if (fCoil)    delete [] fCoil;
}

void XCoilHandle :: SetStabilizer(const double lz, const double lr)
{
  if (!fStable) fStable = new double[2];

  fStable[0] = lz;
  fStable[1] = lr;

  QuenchError( XQuenchLogger::CONFIG, "stabilizer size: {" << fStable[0]
                                                           << ", " << fStable[1] << "}" );
}

void XCoilHandle :: SetTape(const double lz, const double lr)
{
  if (!fTape)  fTape = new double[2];

  fTape[0] = lz;
  fTape[1] = lr;

  QuenchError( XQuenchLogger::CONFIG, "tape size: {" << fTape[0] << ", " << fTape[1] << "}" );
}

void XCoilHandle :: SetMaterialRatio(const double Al, const double Cu, const double SC)
{
  if (!fRatio)   fRatio  = new double[3];

  double tot = Al + Cu + SC;

  fRatio[0] = Al / tot;
  fRatio[1] = Cu / tot;
  fRatio[2] = SC / tot;

  QuenchError( XQuenchLogger::CONFIG, "material ratio: (Al:" << fRatio[0] << ", Cu:" << fRatio[1]
                                                  << ", SC:" << fRatio[2] << ")" );
}

void XCoilHandle :: SetCoil(const double lz, const double lp, const double lr)
{
  if (!fCoil)    fCoil   = new double[3];

  fCoil[0] = lz;
  fCoil[1] = lp;
  fCoil[2] = lr;

  QuenchError( XQuenchLogger::CONFIG, "lz:" << fCoil[0] << ", lp:" << fCoil[1]
                                            << ", lr:" << fCoil[2] );
}

void XCoilHandle :: SetMesh(const int mz, const int mp, const int mr)
{
  if (!fMsh) fMsh = new int[3];

  fMsh[0] = mz;
  fMsh[1] = mp;
  fMsh[2] = mr;

  if (fLayer) fLayer = new int[mr];
}

void XCoilHandle :: AddLayer(const int layer, const Geometry geo)
{
  if (!fMsh) {
    QuenchError( XQuenchLogger::ERROR, "please set the mesh first!" );
    XQuenchExcept except("please set the mesh first!");
    throw except;
  }

  if (!fLayer) fLayer = new int[fMsh[2]];

  fLayer[layer] = geo;
}

double XCoilHandle :: GetStabilizer(const std::string &name) const
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

double XCoilHandle :: GetTape(const std::string &name) const
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

double XCoilHandle :: GetMaterialRatio(const std::string &name) const
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

double XCoilHandle :: GetCoil(const std::string &name) const
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

double* XCoilHandle :: GetConductorSize() const
{
  double* cdtsize = new double[2];
  cdtsize[0] = 2*fTape[0] + fStable[0];
  cdtsize[1] = 2*fTape[1] + fStable[1];

  return cdtsize;
}

double XCoilHandle :: GetConductorSize(const std::string &name) const
{
  double* size = GetConductorSize();

  if (name=="z" || name=="Z")
    return size[0];
  else if (name=="r" || name=="R")
    return size[1];
  else {
    QuenchError( XQuenchLogger::ERROR, "there is no such coordinate: " << name );
    XQuenchExcept except("there is no such coordinate");
    throw except;
  }
}

double XCoilHandle :: GetArea(const std::string& name) const
{
  double area;

  if (name=="Al" || name=="al")
    area = fRatio[0] * fStable[0] * fStable[1];
  else if (name=="Cu" || name=="cu")
    area = fRatio[1] * fStable[0] * fStable[1];
  else if (name=="SC" || name=="sc")
    area = fRatio[2] * fStable[0] * fStable[1];
  else if (name=="Cdt" || name=="cdt")
    area = (2*fTape[0] + fStable[0]) * (2*fTape[1] + fStable[1]);
  else if (name=="Stb" || name=="stb")
    area = fStable[0] * fStable[1];
  else if (name=="Tape" || name=="tape")
    area = (2*fTape[0] + fStable[0]) * (2*fTape[1] + fStable[1]) - fStable[0] * fStable[1];
  else {
    QuenchError( XQuenchLogger::ERROR, "did not find this material: " << name );
    XQuenchExcept except("did not find this material");
    throw except;
  }

  return area;
}

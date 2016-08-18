#include <iostream>
#include "XQuenchLogger.hpp"
#include "XQuenchExcept.hpp"
#include "XCoilHandle.hpp"

using Quench::XQuenchLogger;
using Quench::XCoilHandle;

XCoilHandle :: XCoilHandle()
    : fName(""),
      fCoil(NULL),
      fMsh(NULL),
      fLayer(0),
      fTurn(0)
{
  // initialize the material ratio
  fRatio.insert( std::map<const Material, double>::value_type(iAluminium, 7.) );
  fRatio.insert( std::map<const Material, double>::value_type(   iCopper, 1.) );
  fRatio.insert( std::map<const Material, double>::value_type(     iNbTi, 1.) );
}

XCoilHandle :: ~XCoilHandle()
{
  if (fCoil) delete [] fCoil;
  if (fMsh ) delete [] fMsh;
}


void XCoilHandle :: SetMaterialRatio(const double Al, const double Cu, const double SC)
{
  double tot = Al + Cu + SC;
  fRatio[iAluminium] = Al / tot;
  fRatio[   iCopper] = Cu / tot;
  fRatio[     iNbTi] = SC / tot;

  QuenchError( XQuenchLogger::INFO, "material ratio :: Al:Cu:SC = " << fRatio[iAluminium] 
                                                             << ":" << fRatio[iCopper]
                                                             << ":" << fRatio[iNbTi] );
}


void XCoilHandle :: SetAlPercent(const double perc)
{
  fRatio[iAluminium] = perc;
  QuenchError( XQuenchLogger::INFO, "material ratio :: Al = " << fRatio[iAluminium] );
}


void XCoilHandle :: SetCuPercent(const double perc)
{
  fRatio[iCopper] = perc;
  QuenchError( XQuenchLogger::INFO, "material ratio :: Cu = " << fRatio[iCopper] );
}


void XCoilHandle :: SetScPercent(const double perc)
{
  fRatio[iNbTi] = perc;
  QuenchError( XQuenchLogger::INFO, "material ratio :: SC = " << fRatio[iNbTi] );
}


double XCoilHandle :: GetMaterialRatio(const Material mat) const
{
  if (fRatio.find(mat)==fRatio.end()) {
    QuenchError( XQuenchLogger::ERROR, "not found material: " << mat );
    XQuenchExcept except("XCoilHandle: not found this material.");
    throw except;
  }
  else {
    return fRatio.find(mat)->second;
  }
}


void XCoilHandle :: SetCoilSize(const double lz, const double lp, const double lr)
{
  if (!fCoil) fCoil = new double[3];
  fCoil[  iZ] = lz;
  fCoil[iPhi] = lp;
  fCoil[  iR] = lr;

  QuenchError( XQuenchLogger::INFO, "coil size :: lz:" << fCoil[iZ] << ", lp:" << fCoil[iPhi]
                                            << ", lr:" << fCoil[iR] );
}


double XCoilHandle :: GetCoilSize(const Coil dim) const
{
  if (dim>=3) {
    QuenchError( XQuenchLogger::ERROR, "the dimension" << dim << " is not correct." );
    XQuenchExcept except("XCoilHandle: the coil dimension is not correct.");
    throw except;
  }
  else {
    return fCoil[dim];
  }
}


void XCoilHandle :: SetMesh(const int mz, const int mp, const int mr)
{
  if (!fMsh) fMsh = new int[3];
  fMsh[  iZ] = mz;
  fMsh[iPhi] = mp;
  fMsh[  iR] = mr;

  QuenchError( XQuenchLogger::INFO, "coil mesh :: z:" << fMsh[iZ] << ", phi:" << fMsh[iPhi]
                                            << ", r:" << fMsh[iR] );
}


int XCoilHandle :: GetMesh(const Coil dim) const
{
  if (dim>=3) {
    QuenchError( XQuenchLogger::ERROR, "mesh dimension " << dim << " is wrong." );
    XQuenchExcept except("XCoilHandle: the mesh dimension is not right.");
    throw except;
  }
  else {
    return fMsh[dim];
  }
}


void XCoilHandle :: SetCoilLayout(const int turn, const int layer)
{
  SetCoilTurns(turn);
  SetCoilLayers(layer);
}


void XCoilHandle :: SetCoilLayers(const int layer)
{
  fLayer = layer;
  QuenchError( XQuenchLogger::INFO, "coil layer: " << fLayer );
}


void XCoilHandle :: SetCoilTurns(const int turn)
{
  fTurn = turn;
  QuenchError( XQuenchLogger::INFO, "coil turns: " << fTurn );
}


void XCoilHandle :: AddLayer(const int layer, const Geometry geo)
{
  fLayerGeo.insert( std::map<const int, const Geometry>::value_type(layer, geo) );
  QuenchError( XQuenchLogger::INFO, "coil layer = " << layer << ", geometry = " << GetGeometryName(geo) );
}


std::string XCoilHandle :: GetGeometryName(const Geometry geo)
{
  std::string name = "";
  switch (geo) {
    case kConductor: name = "Conductor"; break;
    case     kStrip: name = "Strip";     break;
    case     kShell: name = "Shell";     break;
    default: break;
  }

  return name;
}


std::string XCoilHandle :: GetMaterialName(const Material mat)
{
  std::string name = "";
  switch (mat) {
    case iAluminium: name = "Aluminium"; break;
    case    iCopper: name = "Copper";    break;
    case      iNbTi: name = "NbTi";      break;
    default: break;
  }

  return name;
}

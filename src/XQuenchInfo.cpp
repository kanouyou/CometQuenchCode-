#include <iostream>
#include "XQuenchExcept.hpp"
#include "XQuenchLogger.hpp"
#include "XQuenchInfo.hpp"

using Quench::XMaterialInfo;
using Quench::XDimensionInfo;
using Quench::XQuenchLogger;

XMaterialInfo :: XMaterialInfo()
    : fField(0.), fTemp(0.), fCapacity(1.),
      fHeat(NULL), fk(NULL), fRho(1.), fRRR(),
      fStatus(kSuperconduct), fGen(0.),
      fDose(0.)
{
  fHeat = new double[3];
  fk    = new double[3];
}

XMaterialInfo :: ~XMaterialInfo()
{
  if (fHeat)  delete [] fHeat;
  if (fk)     delete [] fk;
}

void XMaterialInfo :: SetField(const double &fld)
{
  if (fld<0) {
    QuenchError( XQuenchLogger::WARNING, "field is " << fld );
    XQuenchExcept except("field is negative!");
    throw except;
  }

  fField = fld;
}

void XMaterialInfo :: SetTemperature(const double temp)
{
  fTemp = temp;
}

void XMaterialInfo :: SetCapacity(const double C)
{
  // check input capacity, if the capacity is negative, then throw it to exception
  if (C<0) {
    QuenchError( XQuenchLogger::ERROR, "heat capacity is {" << C );
    XQuenchExcept except("heat capacity is negative!");
    throw except;
  }

  fCapacity = C;
}

void XMaterialInfo :: SetHeatFlux(const double* Q)
{
  if (!fHeat) fHeat = new double[3];

  fHeat[0] = Q[0];
  fHeat[1] = Q[1];
  fHeat[2] = Q[2];
}

void XMaterialInfo :: SetHeatFlux(const double* Qx, const double* Qy, const double* Qz)
{
  if (!fHeat)  fHeat = new double[3];

  fHeat[0] = *Qx;
  fHeat[1] = *Qy;
  fHeat[2] = *Qz;
}

void XMaterialInfo :: SetConductivity(const double* k)
{
  if (k[0]<0 || k[1]<0 || k[2]<0) {
    QuenchError(XQuenchLogger::ERROR, "thermal conductivity is {" << k[0] 
                << ", " << k[1] << ", " << k[2] << "}" );
    XQuenchExcept except("thermal conductivity is negative!");
    throw except;
  }

  if (!fk) fk = new double[3];

  fk[0] = k[0];
  fk[1] = k[1];
  fk[2] = k[2];
}

void XMaterialInfo :: SetConductivity(const double* kx, const double* ky, const double* kz)
{
  if (*kx<0 || *ky<0 || *kz<0) {
    QuenchError(XQuenchLogger::ERROR, "thermal conductivity is {" << *kx
                << ", " << *ky << ", " << *kz << "}" );
    XQuenchExcept except("thermal conductivity is negative!");
    throw except;
  }

  if (!fk)  fk= new double[3];

  fk[0] = *kx;
  fk[1] = *ky;
  fk[2] = *kz;
}

/*****************************************************************************************************/
void XDimensionInfo :: SetId(const int i, const int j, const int k)
{
  if (i<0 || j<0 || k<0) {
    QuenchError(XQuenchLogger::ERROR, "cell id is {" << i << ", " << j << ", " << k << "}" );
    XQuenchExcept except("cell id is negative!");
    throw except;
  }

  fId.at(0) = i;
  fId.at(1) = j;
  fId.at(2) = k;
}


void XDimensionInfo :: SetPrePosition(const double x, const double y, const double z)
{
  fPrePos.at(0) = x;
  fPrePos.at(1) = y;
  fPrePos.at(2) = z;
}


void XDimensionInfo :: SetPosition(const double x, const double y, const double z)
{
  fPos.at(0) = x;
  fPos.at(1) = y;
  fPos.at(2) = z;
}


void XDimensionInfo :: SetPostPosition(const double x, const double y, const double z)
{
  fPostPos.at(0) = x;
  fPostPos.at(1) = y;
  fPostPos.at(2) = z;
}


void XDimensionInfo :: SetNodeId(const int node)
{
  if (node<0) {
    QuenchError(XQuenchLogger::ERROR, "node id is " << node);
    XQuenchExcept except("node id is negative!");
    throw except;
  }

  fNode = node;
}

void XDimensionInfo :: SetCellSize(const double lx, const double ly, const double lz)
{
  fCell.at(0) = lx;
  fCell.at(1) = ly;
  fCell.at(2) = lz;
}

void XDimensionInfo :: SetDistance(const double dx, const double dy, const double dz)
{
  fDistance.at(0) = dx;
  fDistance.at(1) = dy;
  fDistance.at(2) = dz;
}

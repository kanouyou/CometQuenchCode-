#include <iostream>
#include "XQuenchExcept.hpp"
#include "XQuenchLogger.hpp"
#include "XQuenchContainer.hpp"

using Quench::XTimeDependContainer;
using Quench::XQuenchLogger;

XTimeDependContainer :: XTimeDependContainer()
    : fField(0.), fTemp(0.), fCapacity(1.),
      fHeat(NULL), fk(NULL)
{
  fHeat = new double[3];
  fk    = new double[3];
}

XTimeDependContainer :: ~XTimeDependContainer()
{
  if (fHeat)  delete [] fHeat;
  if (fk)     delete [] fk;
}

void XTimeDependContainer :: SetField(const double &fld)
{
  if (fld<0) {
    QuenchError( XQuenchLogger::WARNING, "field is " << fld );
    XQuenchExcept except("field is negative!");
    throw except;
  }

  fField = fld;
}

void XTimeDependContainer :: SetTemperature(const double &temp)
{
  fTemp = temp;
}

void XTimeDependContainer :: SetCapacity(const double &C)
{
  // check input capacity, if the capacity is negative, then throw it to exception
  if (C<0) {
    QuenchError( XQuenchLogger::ERROR, "heat capacity is {" << C );
    XQuenchExcept except("heat capacity is negative!");
    throw except;
  }

  fCapacity = C;
}

void XTimeDependContainer :: SetGeneration(const double* Q)
{
  if (!fHeat) fHeat = new double[3];

  fHeat[0] = Q[0];
  fHeat[1] = Q[1];
  fHeat[2] = Q[2];
}

void XTimeDependContainer :: SetGeneration(const double* Qx, const double* Qy, const double* Qz)
{
  if (!fHeat)  fHeat = new double[3];

  fHeat[0] = *Qx;
  fHeat[1] = *Qy;
  fHeat[2] = *Qz;
}

void XTimeDependContainer :: SetConductivity(const double* k)
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

void XTimeDependContainer :: SetConductivity(const double* kx, const double* ky, const double* kz)
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

/**************************************************/

XDimensionContainer :: XDimensionContainer()
    : fId(NULL), fPos(NULL), fNode(-1)
{
  fId  = new int[3];
  fPos = new double[3];
}

XDimensionContainer :: ~XDimensionContainer()
{
  if (fId)  delete [] fId;
  if (fPos) delete [] fPos;
}

void XDimensionContainer :: SetId(const int* id)
{
  if (id[0]<0 || id[1]<0 || id[2]<0) {
    QuenchError(XQuenchLogger::ERROR, "cell id is {" << id[0] << ", " << id[1] << ", " << id[2] << "}" );
    XQuenchExcept except("cell id is negative!");
    throw except;
  }

  if (!fId) fId = new int[3];

  fId[0] = id[0];
  fId[1] = id[1];
  fId[2] = id[2];
}

void XDimensionContainer :: SetId(const int* i, const int* j, const int* k)
{
  if (*i<0 || *j<0 || *k<0) {
    QuenchError(XQuenchLogger::ERROR, "cell id is {" << *i << ", " << *j << ", " << *k << "}" );
    XQuenchExcept except("cell id is negative!");
    throw except;
  }

  if (!fId)  fId = new int[3];

  fId[0] = *i;
  fId[1] = *j;
  fId[2] = *k;
}

void XDimensionContainer :: SetPosition(const double* pos)
{
  if (!fPos)  fPos = new double[3];

  fPos[0] = pos[0];
  fPos[1] = pos[1];
  fPos[2] = pos[2];
}

void XDimensionContainer :: SetPosition(const double* x, const double* y, const double* z)
{
  if (!fPos)  fPos = new double[3];

  fPos[0] = *x;
  fPos[1] = *y;
  fPos[2] = *z;
}

void XDimensionContainer :: SetNodeId(const int node)
{
  if (node<0) {
    QuenchError(XQuenchLogger::ERROR, "node id is " << node);
    XQuenchExcept except("node id is negative!");
    throw except;
  }

  fNode = node;
}

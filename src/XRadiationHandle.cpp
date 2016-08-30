#include <fstream>
#include "XQuenchLogger.hpp"
#include "XQuenchExcept.hpp"
#include "XRadiationHandle.hpp"

using Quench::XQuenchLogger;

void XRadiationContainer :: SetId(const int z, const int p, const int r)
{
  fId.at(iZ)   = z;
  fId.at(iPhi) = p;
  fId.at(iR)   = r;
}


void XRadiationContainer :: SetPosition(const double z, const double p, const double r)
{
  fPos.at(iZ)   = z;
  fPos.at(iPhi) = p;
  fPos.at(iR)   = r;
}


XRadiationHandle :: XRadiationHandle()
    : fName("")
{}

XRadiationHandle :: XRadiationHandle(const std::string& filename)
    : fName("")
{
  Load(filename);
}

void XRadiationHandle :: Load(const std::string& filename)
{
  std::ifstream file(filename);

  if (!file) {
    QuenchError( XQuenchLogger::ERROR, "file: " << filename << " did not exist." );
    XQuenchExcept except("file did not exist.");
    throw except;
  }

  int    ibuff[3];
  double dbuff[2];
    
  XRadiationContainer* rad = NULL;
  QuenchError( XQuenchLogger::INFO, "loading file: " << filename );

  while (true) {
    if (!file) break;
    file >> ibuff[0] >> ibuff[1] >> ibuff[2] >> dbuff[0] >> dbuff[1];
    rad = new XRadiationContainer;
    rad->SetId( ibuff[0], ibuff[1], ibuff[2] );
    rad->SetNeutron( dbuff[0] );
    rad->SetDose( dbuff[1] );
    fRC.push_back( rad );
  }
}


const int XRadiationHandle :: GetMesh(const Coil dim)
{
  const int msh = findmax(dim) + 1;
  return msh;
}


int XRadiationHandle :: findmax(const Coil dim) const
{
  int max = 0;
  int id;

  for (std::vector<int>::size_type i=0; i<fRC.size(); i++) {
    id = fRC.at(i)->GetId(dim);
    max = id > max ? id : max;
  }

  return max;
}

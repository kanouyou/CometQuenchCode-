#include "XCoilBase.hpp"

using Quench::XCoilBase;

std::string XCoilBase :: GetTypeName()
{
  std::string name = "";
  const Geometry mat = this->GetType();

  switch (mat) {
    case kConductor: name = "Conductor"; break;
    case     kStrip: name = "Strip";     break;
    case     kShell: name = "Shell";     break;
    default: break;
  }

  return name;
}


#include <iostream>
#include "XQuenchExcept.hpp"
#include "XQuenchLogger.hpp"
#include "XMaterial.hpp"

//using Quench::XQuenchExcept;
using Quench::XQuenchLogger;

void XMaterial :: SetMaterialProperty(const double T, const double RRR, const double B)
{
  fTemp = T;
  fRRR  = RRR;
  fFld  = B;
}


void XMaterial :: Print()
{
  QuenchError( XQuenchLogger::INFO, "Temperature:" << fTemp << " , RRR: " << fRRR
                                    << " , Field: " << fFld );
  QuenchError( XQuenchLogger::INFO, "Density: " << this->GetDensity() );
  QuenchError( XQuenchLogger::INFO, "Electric Resistivity: " << this->GetResistivity() );
  QuenchError( XQuenchLogger::INFO, "Thermal Conductivity: " << this->GetConductivity() );
  QuenchError( XQuenchLogger::INFO, "Heat Capacity: " << this->GetCapacity() );
}

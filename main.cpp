#include <iostream>
#include "XQuenchLogger.hpp"
#include "XQuenchExcept.hpp"
#include "XQuenchContainer.hpp"
#include "XMagneticField.hpp"
#include "IFdmUnits.hpp"

using namespace Quench;

void Test() {
  XQuenchLogger* log = XQuenchLogger::GetInstance();
  log->Start(XQuenchLogger::DEBUG, "quench.log");

  XBiotSavart* biot = new XBiotSavart();
  try {
    biot->SetMapRange(-79.525*cm, 59.525*cm, 672.*mm, 823.65*mm);
    biot->SetSolenoid(-79.525*cm, 59.525*cm, 672.*mm, 823.65*mm);
    biot->SetMapMesh(270, 9);
    biot->SetCurrent(2700.);
    std::cout << biot->GetFieldEntry(0,0)->GetField()[0] << std::endl;
    std::cout << biot->GetFieldEntry(1,1)->GetField()[0] << std::endl;
  }
  catch (XQuenchExcept except) {
    std::cerr << " ERROR: " << except.what() << std::endl;
  }

  log->Stop();
}

int main(int argc, char** argv)
{
  Test();

  return 0;
}

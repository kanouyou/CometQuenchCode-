#include <iostream>
#include "XQuenchLogger.hpp"
#include "XQuenchExcept.hpp"
//#include "XQuenchContainer.hpp"
#include "XFieldHandle.hpp"
#include "XPreProcess.hpp"
#include "IFdmUnits.hpp"

using namespace Quench;

void Test() {
  XQuenchLogger* log = XQuenchLogger::GetInstance();
  log->Start(XQuenchLogger::DEBUG, "quench.log");

  XPreProcess* pre = new XPreProcess("CS1");
  XFieldHandle* fld = new XFieldHandle();
  try {
    fld->AddCoil("CS1", -79.525*cm, 59.525*cm, 672.*mm, 823.65*mm);
    fld->SetCurrent(2700.);

    pre->SetMesh(270, 2, 9);
    pre->Initialize();
    pre->SetFieldHandler(fld);
  }
  catch (XQuenchExcept except) {
    delete fld;
    std::cerr << " ERROR: " << except.what() << std::endl;
  }

  log->Stop();
}

int main(int argc, char** argv)
{
  Test();

  return 0;
}

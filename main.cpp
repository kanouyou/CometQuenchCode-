#include <iostream>
#include <TApplication.h>
#include "XQuenchLogger.hpp"
#include "XQuenchExcept.hpp"
//#include "XQuenchContainer.hpp"
#include "XFieldHandle.hpp"
#include "XPostField.hpp"
#include "XCoilHandle.hpp"
#include "XPreProcess.hpp"
#include "IFdmUnits.hpp"

using namespace Quench;

void Test() {
  XQuenchLogger* log = XQuenchLogger::GetInstance();
  log->Start(XQuenchLogger::DEBUG, "quench.log");

  XPostField* post = new XPostField();
  XPreProcess* pre  = new XPreProcess("CS1");

  XFieldHandle* fld = new XFieldHandle();
  XCoilHandle* coil = new XCoilHandle();
  coil->SetMesh(270, 2, 9);

  try {
    fld->AddCoil("CS1", -79.525*cm, 59.525*cm, 672.*mm, 823.65*mm);
    fld->SetCurrent(2700.);

    pre->SetCoilHandler( coil );
    pre->Initialize();
    pre->SetFieldHandler(fld);
    post->SetFieldHandler(fld);
    post->Plot();
  }
  catch (XQuenchExcept except) {
    delete fld;
    delete pre;
    delete coil;
    std::cerr << " ERROR: " << except.what() << std::endl;
  }

  log->Stop();
}

int main(int argc, char** argv)
{
  TApplication* app = new TApplication("app", &argc, argv);

  Test();

  app->Run();

  return 0;
}

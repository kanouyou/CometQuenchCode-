#include <iostream>
#include "XQuenchExcept.hpp"
#include "XQuenchLogger.hpp"
#include "XCOMETConstruction.h"

using namespace Quench;

int main(int argc, char** argv)
{
 
  XQuenchLogger* log = XQuenchLogger::GetInstance();
  log->Start(XQuenchLogger::DEBUG, "quenchlogger.log");
  //

  XCOMETConstruction* comet = new XCOMETConstruction();

  try {
    comet->SetTime(0.*sec, 250.*sec, 4.e-3*msec);
    //comet->SetDisplayStep(10);
    comet->SetCurrent(2700.*Amp);
    comet->SetDumpResistor(0.185*Ohm);
    comet->SetInductance(12.69);
    comet->SetDiode(0.7);
    comet->SetThreshold(0.1);
    comet->SetDetectTime(0.5*sec);

    comet->ConstructCS0();
    comet->Begin();
    comet->Run();
    comet->End();
  }
  catch (XQuenchExcept except) {
    std::cerr << "Error: " << except.what() << std::endl;
    delete comet;
  }

  return 0;
}

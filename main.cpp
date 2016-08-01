#include <iostream>
#include "XQuenchLogger.hpp"

using Quench::XQuenchLogger;

int main(int argc, char** argv)
{
  XQuenchLogger* log = XQuenchLogger::GetInstance();
  log->Start(XQuenchLogger::Level::DEBUG, "quench.log");
  QuenchError(XQuenchLogger::Level::DEBUG, "debug!");
  log->Stop();

  return 0;
}

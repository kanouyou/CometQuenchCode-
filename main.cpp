#include <iostream>
#include "XQuenchLogger.hpp"
#include "XQuenchExcept.hpp"
#include "XQuenchContainer.hpp"

using Quench::XQuenchLogger;

void Test() {
  XQuenchLogger* log = XQuenchLogger::GetInstance();
  log->Start(XQuenchLogger::DEBUG, "quench.log");

  XDimensionContainer* dim = new XDimensionContainer();
  try {
    int id[3] = { -1, 4, 8 };
    dim->SetId( &id[0], &id[1], &id[2] );
  }
  catch (XQuenchExcept except) {
    delete dim;
    std::cerr << " ERROR: " << except.what() << std::endl;
  }

  log->Stop();
}

int main(int argc, char** argv)
{
  Test();

  return 0;
}

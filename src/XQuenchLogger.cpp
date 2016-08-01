#include <iostream>
#include "XQuenchLogger.hpp"

using Quench::XQuenchLogger;

XQuenchLogger* XQuenchLogger::instance = NULL;

XQuenchLogger::XQuenchLogger()
    : fActive(false)
{}

XQuenchLogger* XQuenchLogger :: GetInstance()
{
  if (instance==NULL)
    instance = new XQuenchLogger();

  return instance;
}

void XQuenchLogger :: Start(Level priority, const std::string& filename)
{
  instance->fActive = true;
  instance->fPriority = priority;
}

void XQuenchLogger :: Stop()
{
  std::cout << instance->fActive << std::endl;
}

void XQuenchLogger :: Write(Level priority, const std::string& message)
{

}

const std::string XQuenchLogger :: GetPriorityName(Level level)
{
  const std::string priority[5] = {
      "DEBUG",
      "CONFIG",
      "INFO",
      "WARNING",
      "ERROR"
  };

  switch (level) {
    case   DEBUG: return priority[0]; break;
    case  CONFIG: return priority[1]; break;
    case    INFO: return priority[2]; break;
    case WARNING: return priority[3]; break;
    case   ERROR: return priority[4]; break;
    default: return ""; break;
  }
}

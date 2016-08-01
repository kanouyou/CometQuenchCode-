#ifndef XQuenchLogger_HH
#define XQuenchLogger_HH

#include <string>
#include <fstream>

namespace Quench
{ class XQuenchLogger; }

/// class description:
/// logger class is a singleton class to log the error and warning
/// 
class Quench::XQuenchLogger
{
  public:
    /*! logging level / priority */
    enum Level {
      DEBUG,
      CONFIG,
      INFO,
      WARNING,
      ERROR
    };

    /*! get instance */
    static XQuenchLogger* GetInstance();

    /*! start loging and write message to the file */
    static void Start(Level priority, const std::string& filename);

    /*! stop the logging */
    static void Stop();

    /*! write message */
    static void Write(Level priority, const std::string& message);


  private:
    /*! constructor */
    XQuenchLogger();

    /*! constructor */
    XQuenchLogger(const XQuenchLogger& log) {}

    /*! = operator */
    //XQuenchLogger& operator = (const XQuenchLogger& log) {}

    /*! static instance of this class */
    static XQuenchLogger* instance;

    /*! get the priority names */
    static const std::string GetPriorityName(Level level);

  private:
    bool fActive;
    std::ofstream fFile;
    Level fPriority;

};

#endif

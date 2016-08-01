/**
 *  @file   XQuenchContainer.hpp
 *  @author Y.Yang (Kyushu University)
 *  @date   1st. Aug. 2016
 */

#ifndef XQuenchContainer_HH
#define XQuenchContainer_HH

enum QuenchStatus {
  SUPERCONDUCT,
  TRANSITION,
  NORMAL
};


namespace Quench
{ class XTimeDependContainer; }


/// class description:
/// class to store the time dependent parameter
//
class Quench::XTimeDependContainer
{
  public:
    /*! constructor */
    XTimeDependContainer() {}

    /*! deconstructor */
    ~XTimeDependContainer() {}

    /*! setup magnetic field */

};


#endif

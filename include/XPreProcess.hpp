/**
 *  @file   XCoilConstruct.hpp
 *  @author Y.Yang (Kyushu University)
 *  @date   2 Aug 2016
 */

#ifndef XPreProcess_HH
#define XPreProcess_HH

#ifndef XCoilConstruct_HH
#include "XCoilConstruct.hpp"
#endif

namespace Quench
{ class XPreProcess; }

/// class to push the data into container
//
class Quench::XPreProcess
{
  public:
    /*! constructor */
    XPreProcess();

    /*! constructor */
    XPreProcess(Quench::XCoilConstruct* coil);

    /*! deconstructor */
    ~XPreProcess();

    /*! @brief setup 3 dimensional mesh */
    void SetMesh(const int mz, const int mp, const int mr);

    /*! @brief return the mesh */
    int* GetMesh() const { return fMsh; }

  private:
    Quench::XCoilConstruct* fCoilPar;
    int* fMsh;
};

#endif

/**
 *  @file   XCoilHandle.hpp
 *  @author Y.Yang (Kyushu University)
 *  @date   2 Aug 2016
 */

#ifndef XPreProcess_HH
#define XPreProcess_HH

#include <vector>
#include <string>
#include "XQuenchContainer.hpp"

#ifndef XCoilHandle_HH
#include "XCoilHandle.hpp"
#endif

#ifndef XFieldHandle_HH
#include "XFieldHandle.hpp"
#endif

//class XMaterialContainer;
//class XDimensionContainer;

namespace Quench
{ class XPreProcess; }

/// class to push the data into container
//
class Quench::XPreProcess
{
  public:
    /*! constructor */
    XPreProcess(const std::string& name);

    /*! constructor */
    XPreProcess(const std::string& name, const XCoilHandle* coil);

    /*! deconstructor */
    ~XPreProcess();

    /*! @brief initialization */
    void Initialize();

    /*! @brief set dimension handler */
    void SetCoilHandler(const XCoilHandle* coil);

    /*! @brief setup name */
    void SetName(const std::string& name) { fName = name; }

    /*! @brief return name */
    std::string GetName() const { return fName; }
        
    /*! @brief return the id number of cell */
    const int id(const int i, const int j, const int k);

    /*! @brief setup 3 dimensional mesh */
    void SetMesh(const int mz, const int mp, const int mr);

    /*! @brief return the mesh */
    int* GetMesh() const { return fMsh; }

    /*! @brief setup magnetic field handler */
    void SetFieldHandler(XFieldHandle* fld);

    /*! @brief setup material for each layer */
    void SetLayerInfo(const int layer, const Geometry geo);


  protected:
    /*! @brief initialize */
    void init(const int mz, const int mp, const int mr);

    /*! @brief set strip property and size */
    void setstrip(const int layer);

  private:
    std::string fName;
    const Quench::XCoilHandle* fCoilPar;
    int* fMsh;
    std::vector<XMaterialContainer*>  fMatCollect;
    std::vector<XDimensionContainer*> fDimCollect;
};

#endif

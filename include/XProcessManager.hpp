/**
 *  @file   XProcessManager.hpp
 *  @author Y.Yang (Kyushu University)
 *  @date   18 Aug 2016
 **/

#ifndef XProcessManager_HH
#define XProcessManager_HH

#include <vector>
#include "XQuenchContainer.hpp"
#include "XMeshLoop.hpp"

class XFieldHandle;
class XCoilHandle;

namespace Quench
{ class XProcessManager; }

/// class to manage the data initialization and update
//
class Quench::XProcessManager : public XMeshLoop
{
  public:
    /// @brief constructor
    XProcessManager();

    /// @brief deconstructor
    ~XProcessManager();

    /// @brief setup the class contained coil information
    void SetCoilHandler(XCoilHandle* handler);

    /// @brief setup the field handler
    void SetFieldHandler(XFieldHandle* hand) {}

    /// @brief return the coil handler
    const XCoilHandle* GetCoilHandler() { return fCoil; }

    /// @brief initialization
    void Initialize();

    /// @brief returns the dimensional container
    std::vector<XDimensionContainer*> GetDimensionContainer() { return fDC; }


  protected:
    /// @brief initialize the container vector
    void init();

    /// @brief initialize the temperature
    void InitTemp(const double T);

    /// @brief calculate the position and fill it into container
    void InitPosition();


  private:
    XCoilHandle* fCoil;
    std::vector<XDimensionContainer*> fDC;
    std::vector<XMaterialContainer*> fMC;
};

#endif

/**
 *  @file   XProcessManager.hpp
 *  @author Y.Yang (Kyushu University)
 *  @date   18 Aug 2016
 **/

#ifndef XProcessManager_HH
#define XProcessManager_HH

#include <vector>
#include "XQuenchInfo.hpp"
#include "XMeshLoop.hpp"
#include "XCoilHandle.hpp"

class XFieldHandle;

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
    std::vector<XDimensionInfo*> GetDimensionContainer() { return fDC; }

    /// @brief returns the size of dimensional container
    size_t GetDimensionEntries() const { return fDC.size(); }

    /// @brief returns the dimensional container's entry
    XDimensionInfo* GetDimensionEntry(const int entry) { return fDC.at(entry); }


  protected:
    /// @brief initialize the container vector
    void init();

    /// @brief initialize the temperature
    void InitTemp(const double T);

    /// @brief calculate the position and fill it into container
    void InitPosition();


  private:
    XCoilHandle* fCoil;
    std::vector<XDimensionInfo*> fDC;
    std::vector<XMaterialInfo*> fMC;
};

#endif

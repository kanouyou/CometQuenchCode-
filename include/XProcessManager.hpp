/**
 *  @file   XProcessManager.hpp
 *  @author Y.Yang (Kyushu University)
 *  @date   18 Aug 2016
 **/

#ifndef XProcessManager_HH
#define XProcessManager_HH

#include <vector>
#include <string>
#include "XQuenchInfo.hpp"
#include "XMeshLoop.hpp"
#include "XCoilHandle.hpp"
#include "XFieldHandle.hpp"

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
    void SetFieldHandler(XFieldHandle* hand);

    /// @brief return the coil handler
    const XCoilHandle* GetCoilHandler() { return fCoil; }

    /// @brief setup uniform RRR for strip
    /// @param part enumeration of geometry: kShell/kConductor/kStrip
    void SetUniformRRR(const Geometry part, const double RRR);

    /// @brief setup the uniform field
    void SetUniformField(const double fld);

    /// @brief initialization
    void Initialize();

    /// @brief initialization
    void Initialize(XCoilHandle* coil, XFieldHandle* fld);

    /// @brief initialize the material information
    void SetMaterial();

    /// @brief returns the dimensional container
    std::vector<XDimensionInfo*> GetDimensionContainer() { return fDC; }

    /// @brief returns the size of dimensional container
    size_t GetDimensionEntries() const { return fDC.size(); }

    /// @brief returns the dimensional container's entry
    XDimensionInfo* GetDimensionEntry(const int entry) { return fDC.at(entry); }

    /// @brief returns the material container
    std::vector<XMaterialInfo*> GetMaterialContainer() { return fMC; }

    /// @brief returns the size of material container
    size_t GetMaterialEntries() const { return fMC.size(); }

    /// @brief return this material container
    XMaterialInfo* GetMaterialEntry(const int entry) { return fMC.at(entry); }


  protected:
    /// @brief initialize the container vector
    void init();

    /// @brief initialize the temperature
    void InitTemp(const double T);

    /// @brief calculate the position and fill it into container
    void InitPosition();


  private:
    XCoilHandle* fCoil;
    std::string fName;
    std::vector<XDimensionInfo*> fDC;
    std::vector<XMaterialInfo*> fMC;
};

#endif
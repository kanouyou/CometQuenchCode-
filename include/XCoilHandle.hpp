/**
 *  @file   XCoilHandle.hpp
 *  @author Y.Yang (Kyushu University)
 *  @date   2 Aug 2016
 **/

#ifndef XCoilHandle_HH
#define XCoilHandle_HH

#include <map>
#include <string>
#include "XMaterial.hpp"
#include "XCoilBase.hpp"

#ifndef IFdmUnits_HH
#include "IFdmUnits.hpp"
#endif


namespace Quench
{ class XCoilHandle; }

/// class to handle the information of coil
//
class Quench::XCoilHandle
{
  public:
    /// @brief constuctor
    XCoilHandle();

    /// @brief deconstructor
    virtual ~XCoilHandle();

    /// @brief setup the name of this magnet
    /// @param name magnet name
    void SetName(const std::string& name) { fName = name; }

    /// @brief return the name of the magnet
    std::string GetName() const { return fName; }

    /// @brief  setup material ratio in the conductor
    /// @detail the default ratio is \f$ Al:Cu:SC = 7.3:1:0.9 \f$
    void SetMaterialRatio(const double Al=7.3, const double Cu=1., const double SC=0.9);

    /// @brief  setup conductor material percent ratio
    /// @detail noting that the ratio inputed in here must be \f$ \frac{r_{mat}}{r_{tot}} \f$
    void SetAlPercent(const double perc);
    void SetCuPercent(const double perc);
    void SetScPercent(const double perc);

    /// @brief  return material ratio
    /// @param  mat please using the enumeration: Material::iAluminium/iCopper/iNbTi
    /// @return percent value of the inputed material in this conductor
    double  GetMaterialRatio(const Material mat) const;
    std::map<const Material, double> GetMaterialRatio() const { return fRatio; }

    /// @brief  setup coil's size
    /// @detail length along the axis z and r will not be used in calculation.
    ///         thus it can be set to a arbitrary value
    void SetCoilSize(const double lz, const double lp, const double lr);
    
    /// @brief return coil size
    /// @param dim enumeration value: Coil::iZ/iPhi/iR
    double  GetCoilSize(const Coil dim) const;
    double* GetCoilSize() const { return fCoil; }

    /// @brief setup coil mesh
    void SetMesh(const int mz, const int mp, const int mr);

    /// @brief return coil mesh
    /// @param dim enumeration value: Coil::iZ/iPhi/iR
    int  GetMesh(const Coil dim) const;
    int* GetMesh() const { return fMsh; }

    /// @brief setup coil total layers and turns
    /// @param layer layer of conductor, not included the shell and strip 
    void SetCoilParameters(const int turn, const int layer);
    void SetCoilLayers(const int layer);
    void SetCoilTurns(const int turn);

    /// @brief setup the conductor/strip/shell
    void SetConductor(const XCoilBase* cdt) { fCdt = cdt; }
    void SetStrip(const XCoilBase* strip) { fStrip = strip; }
    void SetShell(const XCoilBase* shell) { fShell = shell; }

    /// @brief return the coil class, which consists of conductor/strip/shell
    const Quench::XCoilBase* GetCoilParts(const Geometry geo);

    /// @brief returns the coil layer which not includes the strip and shell
    int GetCoilLayers() const { return fLayer; }

    /// @brief returns the coil turns
    int GetCoilTurns() const { return fTurn; }

    /// @brief add the layer's information
    void AddLayer(const int layer, const Geometry geo);

    /// @brief return the coil layout
    std::map<const int, const Geometry> GetCoilLayout() { return fLayerGeo; }
    const Quench::XCoilBase* GetCoilLayout(const int layer);

    /// @brief check the layer is out of mesh or not
    /// @detail if the set layer is out of range, then return true
    ///         otherwise, return false
    bool is_out_range() const;
    bool IsOutRange(const int layer) const;

    /// @brief check the layer is exist or not
    bool is_exist(const int layer) const;


  protected:
    /// @brief returns the name of geometry
    std::string GetGeometryName(const Geometry geo);

    /// @brief returns the name of the material
    std::string GetMaterialName(const Material mat);


  private:
    std::string fName;
    double* fCoil;
    int*    fMsh;
    int     fLayer;
    int     fTurn;
    const XCoilBase* fCdt;
    const XCoilBase* fStrip;
    const XCoilBase* fShell;
    std::map<const int, const Geometry> fLayerGeo;
    std::map<const Material, double> fRatio;
};


#endif

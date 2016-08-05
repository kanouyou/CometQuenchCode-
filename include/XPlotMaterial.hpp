/**
 *  @file   XPlotMaterial.hpp
 *  @author Y.Yang (Kyushu University)
 *  @date   5 Aug 2016
 */

#ifndef XPlotMaterial_HH
#define XPlotMaterial_HH

#include <string>
#include "XMaterial.hpp"

class TMultiGraph;

/// class to plot the material property
//
class XPlotMaterial
{
  public:
    /*! construct */
    XPlotMaterial();

    /*! deconstructor */
    ~XPlotMaterial();

    /*! @brief setup option */
    void SetOption(const std::string& opt) { fOpt = opt; }

    /*! @brief setup range */
    void SetRange(const double min, const double max);

    /*! @brief setup material */
    void SetMaterial(XMaterial* mat) { fMat = mat; }

    /*! @brief add graph */
    void Add(const std::string& opt, const double var);

    /*! @brief plot */
    void Plot();

  protected:
    /*! @brief plot resistivity */
    //void plotresist(const char* opt);


  private:
    std::string fOpt;
    double* fRange;
    XMaterial* fMat;
    TMultiGraph* fMg;
};

#endif

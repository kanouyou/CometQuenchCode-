/**
 *  @file   XPostInfoPlot.hpp
 *  @author Y.Yang (Kyushu University)
 *  @date   24 Aug 2016
 **/

#ifndef XPostInfoPlot_HH
#define XPostInfoPlot_HH

#include <vector>
#include "XProcessManager.hpp"

class TGraph;

/// class to plot the information from quench container
//
class XPostInfoPlot
{
  public:
    /// @brief deconstructor
    ~XPostInfoPlot();

    /// @brief pass the XProcessManager class
    void SetProcessManager(Quench::XProcessManager* pro);

    /// @brief plot the position of each node
    void PlotNode(const bool save=false);


  protected:
    /// @brief return the graph of node position
    /// @param phi id number along the phi direction
    TGraph* GetNodeGraph(const int phi);

  private:
    Quench::XProcessManager* fProcess;
};

#endif

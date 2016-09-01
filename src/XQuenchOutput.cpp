#include <iostream>
#include <TFile.h>

#include "XQuenchExcept.hpp"
#include "XQuenchLogger.hpp"
#include "XQuenchOutput.hpp"

using Quench::XQuenchLogger;
using Quench::XQuenchOutput;


XQuenchOutput :: XQuenchOutput(const std::string &filename, const FileOutput opt)
    : fFilename(filename)
{
  init(opt);
}


XQuenchOutput :: ~XQuenchOutput()
{
  if (fRootfile) fRootfile->Close();
  if (fNormfile.is_open()) fNormfile.close();
}


void XQuenchOutput :: init(const FileOutput opt)
{
  switch (opt) {
    case iROOT: 
      fRootfile = new TFile(fFilename.c_str(), "recreate");
      QuenchError( XQuenchLogger::CONFIG, "write data to the ROOT file." );
      break;
    case iOfstream:
      fNormfile.open( fFilename, std::ios::out );
      QuenchError( XQuenchLogger::CONFIG, "write data to the stream file.");
      break;
    default:
      QuenchError( XQuenchLogger::ERROR, "option " << opt << " is not available." );
      XQuenchExcept except("output option is not available.");
      throw except;
  }
}

void XQuenchOutput :: Write(XProcessManager* man)
{
  if (!man) {
    QuenchError( XQuenchLogger::ERROR, "processing manager is null.");
  }

  for (int i=0; i<man->GetEntries(); i++) {
    fNormfile << man->GetDimensionEntry(i)->GetId(iZ)   << " "
              << man->GetDimensionEntry(i)->GetId(iPhi) << " "
              << man->GetDimensionEntry(i)->GetId(iR)   << " "
              << man->GetDimensionEntry(i)->GetPosition(iZ)   << " "
              << man->GetDimensionEntry(i)->GetPosition(iPhi) << " "
              << man->GetDimensionEntry(i)->GetPosition(iR)   << " "
              << man->GetMaterialEntry(i)->GetTemperature() << " "
              << man->GetMaterialEntry(i)->GetRRR() << " "
              << man->GetMaterialEntry(i)->GetField() << " "
              << man->GetMaterialEntry(i)->GetCapacity() << " "
              << man->GetMaterialEntry(i)->GetConductivity(iZ) << " "
              << man->GetMaterialEntry(i)->GetConductivity(iPhi) << " "
              << man->GetMaterialEntry(i)->GetConductivity(iR) << " "
              << man->GetMaterialEntry(i)->GetDeposit() << " "
              <<"\n";
  }
}

void XQuenchOutput :: Close()
{
  if (fRootfile) fRootfile->Close();
  if (fNormfile.is_open()) fNormfile.close();
}

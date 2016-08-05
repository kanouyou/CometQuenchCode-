#include <iostream>
#include "XPostField.hpp"

using Quench::XFieldHandle;

XPostField :: XPostField()
    : fMagnet(""), fInfo(NULL)
{}

XPostField :: XPostField(XFieldHandle* fld)
    : fMagnet(""), fInfo(NULL)
{
  SetFieldHandler(fld);
}

XPostField :: ~XPostField()
{
  if (fInfo) delete fInfo;
}

void XPostField :: SetFieldHandler(XFieldHandle* fld)
{
  fMagnet  = fld->GetTarget();
  fInfo    = fld->GetInfoEntry(fMagnet);
  fCollect = fld->GetFieldCollection();
}

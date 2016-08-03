#include "XQuenchLogger.hpp"
#include "XFieldHandle.hpp"

using Quench::XQuenchLogger;
using Quench::XFieldHandle;
using Quench::XMagnetInfoContainer;
using Quench::XFieldContainer;

XFieldHandle :: XFieldHandle()
    : fCurrent(2700.)
{}

XFieldHandle :: ~XFieldHandle()
{}

bool XFieldHandle :: is_exist(const std::string& name) const
{
  bool exist = false;

  for (std::vector<int>::size_type i=0; i<fHC.size(); i++) {
    if ( fHC[i]->GetName() == name )  {
      exist = true;
      break;
    }
  }

  return exist;
}

void XFieldHandle :: AddCoil(const std::string& name, const double z0, const double z1,
                                                      const double r0, const double r1)
{
  if (!is_exist(name)) {
    XMagnetInfoContainer* mag = new XMagnetInfoContainer();
    mag->SetName( name );
    mag->SetDimension( z0, z1, r0, r1 );
    fHC.push_back(mag);
  }
  else {
    QuenchError( XQuenchLogger::WARNING, "magnet: " << name << " is already defined." );
  }
}

void XFieldHandle :: AddCoil(XMagnetInfoContainer* mag)
{
  if ( !is_exist(mag->GetName()) )
    fHC.push_back(mag);
}

void XFieldHandle :: SetMesh(const std::string& name, const int mz, const int mr)
{
  if (is_exist(name)) {
    for (std::vector<int>::size_type i=0; i<fHC.size(); i++) {
      if (fHC[i]->GetName() == name) {
        fHC[i]->SetMesh(mz, mr);
        break;
      }
    }
  }
  else {
    QuenchError( XQuenchLogger::WARNING, "magnet: " << name << " did not exist." );
  }
} 

XMagnetInfoContainer* XFieldHandle :: GetContainerEntry(const std::string& name)
{
  for (std::vector<int>::size_type i=0; i<fHC.size(); i++) {
    if ( fHC[i]->GetName()==name )
      return fHC[i];
  }
  
  QuenchError( XQuenchLogger::WARNING, "magnet: " << name << " did not exist." );

  return NULL;
}

void XFieldHandle :: calfield(const std::string& name)
{
  XMagneticField* fld = new XBiotSavart();  

  /// find this magnet
  for (std::vector<int>::size_type i=0; i<fHC.size(); i++) {
    if ( fHC[i]->GetName()==name ) {
      fld->SetMapRange( fHC[i]->GetDimension()[0], fHC[i]->GetDimension()[1],
                        fHC[i]->GetDimension()[2], fHC[i]->GetDimension()[3] );
      fld->SetMapMesh( fHC[i]->GetMesh()[0], fHC[i]->GetMesh()[1] );
      break;
    }
  }

  /// calculate self field and mutal field
  double B[2];

  for (std::vector<int>::size_type i=0; i<fHC.size(); i++) {
    fld->SetSolenoid( fHC[i]->GetDimension()[0], fHC[i]->GetDimension()[1],
                      fHC[i]->GetDimension()[2], fHC[i]->GetDimension()[3] );
    fld->SetCurrent( fCurrent );
    fld->Run();

    if (i==0)
      /// self field
      fCollect = fld->GetFieldContainer();
    else {
      /// mutal field
      for (std::vector<int>::size_type i=0; i<fCollect.size(); i++) {
        B[0] = fCollect[i]->GetField()[0] + fld->GetFieldContainer()[i]->GetField()[0];
        B[1] = fCollect[i]->GetField()[1] + fld->GetFieldContainer()[i]->GetField()[1];
        fCollect[i]->SetField( B[0], B[1] );
      }
    }

  }
}


std::vector<Quench::XFieldContainer*> XFieldHandle :: GetFieldCollection(const std::string& name)
{
  if ( is_exist(name) )
    calfield(name);

  return fCollect;
}


Quench::XFieldContainer* XFieldHandle :: GetFieldContainer(const std::string& name, const int id)
{
  if ( is_exist(name) )
    calfield(name);

  return fCollect[id];
}

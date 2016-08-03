/**
 *  @file   XCoilConstruct.hpp
 *  @author Y.Yang (Kyushu University)
 *  @date   3 Aug 2016
 */

#ifndef XMagneticField_HH
#define XMagneticField_HH

#include <vector>

#ifndef IFdmUnits_HH
#include "IFdmUnits.hpp"
#endif

namespace Quench
{
  class XFieldContainer;
  class XMagneticField;
  class XBiotSavart;
}

/// class to contain the magnetic field data
//
class Quench::XFieldContainer
{
  public:
    /*! constructor */
    XFieldContainer() : fPos(NULL), fField(NULL), fId(NULL) {}

    /*! deconstructor */
    ~XFieldContainer() {
      if (fPos)   delete [] fPos;
      if (fField) delete [] fField;
      if (fId)    delete [] fId;
    }

    /*! @brief setup id number */
    void SetId(const int idz, const int idr) {
      if (!fId)  fId = new int[2];
      fId[0] = idz;
      fId[1] = idr;
    }

    /*! @brief setup cell position */
    void SetPosition(const double z, const double r) {
      if (!fPos)  fPos = new double[2];
      fPos[0] = z;
      fPos[1] = r;
    }

    /*! @brief setup magnetic field */
    void SetField(const double Bz, const double Br) {
      if (!fField)  fField = new double[2];
      fField[0] = Bz;
      fField[1] = Br;
    }

    /*! @brief setup vector potential */
    void SetPotential(const double vecA) { fVector = vecA; }

    /*! @brief return id number */
    int* GetId() const { return fId; }

    /*! @brief return cell position */
    double* GetPosition() const { return fPos; }

    /*! @brief return magnetic field array */
    double* GetField() const { return fField; }

    /*! @brief return magnetic vector potential */
    double GetPotential() const { return fVector; }

    
  private:
    double* fPos;
    double* fField;
    int*    fId;
    double  fVector;
};


/// base class to handle magnetic field
//
class Quench::XMagneticField
{
  public:
    /*! deconstructor */
    virtual ~XMagneticField() {}

    /*! @brief setup calculation region */
    virtual void SetMapRange(const double z0, const double z1, const double r0, const double r1) = 0;

    /*! @brief setup regin mesh */
    virtual void SetMapMesh(const int mz, const int mr, const int mp) = 0;

    /*! @brief setup current */
    virtual void SetCurrent(const double I) = 0;

    /*! @brief update the magnetic field */
    virtual XFieldContainer* GetField(const int iz, const int jr) const = 0;
};


/// class to calculate the magentic field
//
class Quench::XBiotSavart : public Quench::XMagneticField
{
  public:
    /*! constructor */
    XBiotSavart();

    /*! deconstructor */
    virtual ~XBiotSavart();

    /*! @brief setup calculation region */
    virtual void SetMapRange(const double z0, const double z1, const double r0, const double r1);

    /*! @brief setup regin mesh */
    virtual void SetMapMesh(const int mz, const int mr, const int mp=50);

    /*! @brief setup current */
    virtual void SetCurrent(const double I) { fCurrent = I; }

    /*! @brief return field container */
    virtual Quench::XFieldContainer* GetField(const int iz, const int jr) const;


  protected:
    /*! @brief calculate magnetic field */
    void calfield();

    /*! @brief calculate vector potential at this position */
    double calpotential(const double z, const double r) const;

  private:
    int*    fMapMesh;
    double* fMapRange;
    double  fCurrent;
    std::vector <Quench::XFieldContainer*> fHC;
};

#endif

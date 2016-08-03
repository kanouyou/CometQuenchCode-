/**
 *  @file   XCoilConstruct.hpp
 *  @author Y.Yang (Kyushu University)
 *  @date   3 Aug 2016
 */

#ifndef XMagneticField_HH
#define XMagneticField_HH

namespace Quench
{
  class XFieldContainer;
  class XMagneticField;
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


/// class to calculate the magentic field
//
class Quench::XMagneticField
{

};

#endif

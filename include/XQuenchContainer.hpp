/**
 *  @file   XQuenchContainer.hpp
 *  @author Y.Yang (Kyushu University)
 *  @date   1st. Aug. 2016
 */

#ifndef XQuenchContainer_HH
#define XQuenchContainer_HH

/** @enum
 *  Quench status
 */
enum QuenchStatus {
  //! superconducting status: operating
  kSuperconduct,
  //! transition status: start current sharing
  kTransition,
  //! normal: start quenching
  kNormal
};


namespace Quench
{ 
  class XMaterialContainer; 
  class XDimensionContainer;
}


/// class to store the material parameter
//
class Quench::XMaterialContainer
{
  public:
    /*! constructor */
    XMaterialContainer();

    /*! deconstructor */
    ~XMaterialContainer();

    /*! setup magnetic field */
    void SetField(const double &fld);

    /*! return the magnetic field */
    double GetField() const { return fField; }

    /*! setup temperature */
    void SetTemperature(const double &temp);

    /*! return temperature */
    double GetTemperature() const { return fTemp; }

    /*! setup heat capacity */
    void SetCapacity(const double &C);

    /*! return heat capacity */
    double GetCapacity() const { return fCapacity; }

    /*! setup heat generation */
    void SetGeneration(const double* Q); 

    /*! setup heat generation */
    void SetGeneration(const double* Qx, const double* Qy, const double* Qz);

    /*! return heat generation */
    double* GetGeneration() const { return fHeat; }

    /*! setup thermal conductivity */
    void SetConductivity(const double* k);

    /*! setup thermal conductivity */
    void SetConductivity(const double* kx, const double* ky, const double* kz);

    /*! return thermal conductivity */
    double* GetConductivity() const { return fk; }

    /*! setup local resistance */
    void SetResistance(const double R) { fR = R; }

    /*! return local resistance */
    double GetResistance() const { return fR; }

    /*! @brief setup material density */
    void SetDensity(const double rho) { fRho = rho; }

    /*! @brief return material density */
    double GetDensity() const { return fRho; }

    /*! @brief setup residual resistance ratio */
    void SetRRR(const double RRR) { fRRR = RRR; }

    /*! @brief return residual resistance ratio */
    double GetRRR() const { return fRRR; }

    /*! @brief setup the superconducting status */
    void SetStatus(const QuenchStatus status) { fStatus = status; }

    /*! @brief return the superconducting status */
    QuenchStatus GetStatus() const { return fStatus; }


  private:
    double  fField;
    double  fTemp;
    double  fCapacity;
    double* fHeat;
    double* fk;
    double  fR;
    double  fRho;
    double  fRRR;
    QuenchStatus fStatus;
};


/// class discription:
/// class to handle the dimensional container
//
class Quench::XDimensionContainer
{
  public:
    /*! constructor */
    XDimensionContainer();

    /*! deconstructor */
    ~XDimensionContainer();

    /*! setup cell id */
    void SetId(const int* id);

    /*! setup cell id */
    void SetId(const int* i, const int* j, const int* k);

    /*! return cell id */
    int* GetId() const { return fId; }

    /*! setup cell position */
    void SetPosition(const double* pos);

    /*! setup cell position */
    void SetPosition(const double* x, const double* y, const double* z);

    /*! return cell position */
    double* GetPosition() const { return fPos; }

    /*! setup node id */
    void SetNodeId(const int node);

  private:
    int* fId;
    double* fPos;
    double fNode;
};


// class description:
// class to store the time independ parameters
//
//class XTimeIndependContainer
//{
//  public:
    /*! constructor */
//    XTimeIndependContainer() : fRRR(0.), fRho(1.) {}

    /*! deconstructor */
//    ~XTimeIndependContainer() {}

    /*! @brief setup density */
//    void SetDensity(const double rho) { fRho = rho; }

    /*! @brief setup residual resistance ratio */
//    void SetRRR(const double RRR) { fRRR = RRR; }

    /*! @brief return the material density */
//    double GetDensity() const { return fRho; }

    /*! @brief return residual resistance ratio */
//    double GetRRR() const { return fRRR; }

//  private:
//    double fRRR;
//    double fRho;
//};

#endif

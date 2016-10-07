#ifndef XQuenchTransient_HH

class XTransientLoop;

class XQuenchTransient : public XTransientLoop
{
  public:
    /// constructor
    XQuenchTransient();

    /// deconstructor
    ~XQuenchTransient();

    /// @brief setup dump resistor resistance
    void SetDumpResistor(const double R);

    /// @brief returns the dump resistor resistance
    double GetDumpResistor() const { return fDumpRes; }

    /// @brief setup the coil inductance
    void SetInductance(const double L);

    /// @brief returns the coil inductance
    double GetInductance() const { return fInduct; }

    /// @brief setup operating current
    void SetCurrent(const double I);

    /// @brief return the operating current
    double GetCurrent() const { return fCurr; }

    /// @brief  setup allowable voltage
    /// @detail calculate the dump resistance from allowable voltage.
    ///         to use this function, the current must be set before this function.
    void SetVoltage(const double V);

    /// @brief returns the decay time
    inline double GetDecaytime() { return fInduct/fDumpRes; }

    /// @brief set beginning of the run
    virtual void Begin();

    /// @brief run transient loop
    virtual void Run();

    /// @brief end of transient loop
    virtual void End();


  private:
    double fDumpRes;
    double fInduct;
    double fCurr;
};

#endif

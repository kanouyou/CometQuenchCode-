# Quench Code with Propagator

---

# Author

* Ye YANG (Kyushu University)
* Email: kanouyou@kune2a.nucl.kyushu-u.ac.jp

# Overview
QuenchCode is a Toolkit for calculation of magnet quench with quench propagator.
The code is wriiten in C++ and depends on the libraries:

- ROOT
- Boost


# Code Sample for calculation of the magnetic field
Magnetic field of coil is calculated by using the Biot-Savart Law for solenoids.
The simple way to obtain the magnetic field for coil is to use the `XFieldHandle` class.

```cpp
std::vector<Quench::XFieldContainer*> cs1;
XFieldHandle* fld = new XFieldHandle();

// construct magnet
fld->AddCoil("CS0", 857.88*mm, 1038.12*mm, 672.*mm, 823.65*mm);
fld->SetMesh("CS0", 35, 18);

fld->AddCoil("CS1", -595.25*mm, 795.25*mm, 672.*mm, 823.65*mm);
fld->SetMesh("CS1", 270, 18);

fld->AddCoil("MS1", -2121.375*mm, -653.625*mm, 672.*mm, 756.25*mm);
fld->SetMesh("MS1", 285, 10);

fld->SetTarget("CS1");
cs1 = fld->GetFieldCollection();
```

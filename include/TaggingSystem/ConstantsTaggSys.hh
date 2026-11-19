//
// Created by yurchenko on 19.11.2026.
//

#ifndef COSMIC_CONSTANTSTAGGSYS_HH
#define COSMIC_CONSTANTSTAGGSYS_HH

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

constexpr G4int fNof_GEM = 3;

// геометрия системы мечания

constexpr G4double alpha0_GEM = 37.0 * mrad; // mrad
constexpr G4double theta0_GEM = 47.0 * mrad;
constexpr G4double D_GEM = 320.0 * mm;
constexpr G4double L0_GEM =
    310.0 * mm + 30.0 * mm; // расстояние от чентра мишени по Мальцеву 310 мм, по чертежу 350?? мм
constexpr G4double Z1_GEM = 130.0 * mm;
constexpr G4double L1_GEM = 83.0 * mm;
constexpr G4double L2_GEM = 83.0 * mm;
constexpr G4double L_GEM = 166.0 * mm;
constexpr G4double X00_GEM = 2.0 * mm; // добавка к вертикальной координате
constexpr G4double X10_GEM = 32.0 * mm + X00_GEM;
constexpr G4double X20_GEM = 38.0 * mm + X00_GEM;
constexpr G4double X30_GEM = 63.0 * mm + X00_GEM;

constexpr G4double DENSITY_LO = 1.032; // scintill. density - for light output calculation

#endif // COSMIC_CONSTANTSTAGGSYS_HH

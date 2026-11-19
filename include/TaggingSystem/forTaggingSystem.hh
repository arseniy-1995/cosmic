//
// Created by Арсений Юрченко on 12.11.2026.
//

#ifndef COSMIC_FORTAGGINGSYSTEM_HH
#define COSMIC_FORTAGGINGSYSTEM_HH

#include "Constants.hh"
#include "DetectorConstruction.hh"
#include "Randomize.hh"

//#include "ChamberSD.hh"
#include "DetectorConstruction.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
//#include "HadronCalorimeterSD.hh"
//#include "PlasticSD.hh"

#include <G4EllipticalTube.hh>
#include <G4Trap.hh>
#include <G4Trd.hh>
#include <TString.h>
#include "G4AutoDelete.hh"
#include "G4Box.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"

#include "G4MultiUnion.hh"
#include "G4Transform3D.hh"


#include "G4SDManager.hh"

#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
// #include <TString.h>


class DetectorConstruction;

namespace Cosmic
{

    // #include "DetectorConstruction.hh"

    class BetheHeitlerGenerator
    {
    public:
        BetheHeitlerGenerator() {}
        ~BetheHeitlerGenerator() {}

        G4double photon_energy = 0.; // Энергия излученного фотона (МэВ)
        G4double Eelectron_final = 0.; // Оставшаяся энергия электрона (МэВ)

        G4double Epositron_final = 0.; // позитрон для генератора пары

        G4double photon_theta = 0.; // Полярный угол вылета фотона (рад)
        G4double photon_phi = 0.; // // Азимутальный угол вылета фотона (рад)


        // 1. Генерация энергии фотона при тормозном излучении (Bremsstrahlung)
        // E0 - начальная энергия электрона (МэВ)
        // E_min - минимальный порог энергии фотона
        // void BremsstrahlungGenerator(G4double electron_energy0, G4double e_min_cut, G4double e_max_cut, G4double
        // &photon_energy, G4double &photon_theta, G4double &photon_phi); explicit BremsstrahlungGenerator();
        void sampleBremsstrahlungGenerator(G4double electron_energy0, G4double e_min_cut, G4double e_max_cut);

        // 2. Генерация энергии электрона при рождении пар (Pair Production)
        // E_gamma - энергия налетающего фотона (МэВ)
        void samplePairProduction(G4double E_gamma)
        {

            if (E_gamma <= 2.0 * (Me * MeV))
            {
                //  throw std::invalid_argument("Энергия фотона ниже порога рождения пар (1.022 МэВ)");
                G4cerr << "Energy Electron is small" << G4endl;
            }

            y_min = (Me * MeV) / E_gamma;
            y_max = 1.0 - y_min;

            // Максимум функции f(y) = 1 - 4/3 * y * (1 - y) равен 1.0 (при y = 0 или y = 1)
            const G4double f_max = 1.0;
            G4double y = 0.;
            while (true)
            {
                // Равномерно разыгрываем долю энергии y в допустимых кинематических пределах
                y = y_min + G4UniformRand() * (y_max - y_min);

                G4double f_y = 1.0 - (4.0 / 3.0) * y * (1.0 - y);
                G4double u = G4UniformRand();

                if (u * f_max <= f_y)
                {
                    // return x * E_gamma; // Энергия электрона (позитрон получит E_gamma - E_e)
                    break;
                }
            }

            Eelectron_final = y * E_gamma;
            Epositron_final = E_gamma - Eelectron_final;
        }

    private:
        G4double y_min = 0.; // Минимальная доля энергии фотона (k_min / E_0)
        G4double y_max = 1.; // Максимальная доля энергии фотона (обычно 1.0 - m_e/E_0)

        // Вычисление ненормированной плотности вероятности g(y) после замены y = exp(u)
        G4double evaluate_pdf_br(G4double y, G4double E0_norm) const
        {
            if (y <= 0.0 || y >= 1.0 - 1.0 / E0_norm)
            {
                return 0.0;
            }
            G4double E_norm = E0_norm * (1.0 - y);
            G4double log_term = std::log(2.0 * E0_norm * E_norm / (E0_norm * y)) - 0.5;
            if (log_term <= 0.0)
            {
                return 0.0;
            }
            G4double poly_term = (4.0 / 3.0) * (1.0 - y) + y * y;
            return poly_term * log_term;
        }
    };

    class MainVolTaggingSystem : public G4PVPlacement
    {
    public:
        MainVolTaggingSystem(G4RotationMatrix *pRot, const G4ThreeVector &tlate, G4LogicalVolume *pMotherLogical,
                             G4bool pMany, G4int pCopyNo, DetectorConstruction *c);


        ////

        void ConstructTarget();
        void ConstructMagnetCell();

        void ConstructMagnetD2();

        void ConstructMagnetD1();
        void ConstructMagnetD3();

        void ConstructMagnetQF();
        void ConstructMagnetQD();

        void ConstructGEM();

        //////////

        void SetLogVol() { SetLogicalVolume(fHousing_log); }

        G4LogicalVolume *GetHousingVolume() { return fHousing_log; }
        auto GetHousingPhys() { return fHousing_phys; }

        G4LogicalVolume *GetLogD2gap() { return fLogicD2gap; }
        G4LogicalVolume *GetLogD2gap_add1() { return fLogicD2gap_add1; }
        G4LogicalVolume *GetLogD2gap_add2() { return fLogicD2gap_add2; }

        G4LogicalVolume *GetLogD1gap() { return fLogicD1gap; }
        G4LogicalVolume *GetLogD3gap() { return fLogicD3gap; }

        G4LogicalVolume *GetLogQFgap() { return fLogicQFgap; }
        G4LogicalVolume *GetLogQDgap() { return fLogicQDgap; }

        G4ThreeVector *GetPosQF() { return new G4ThreeVector(x_pos_qf, y_pos_qf, z_pos_qf); }
        G4ThreeVector *GetPosQD() { return new G4ThreeVector(x_pos_qd, y_pos_qd, z_pos_qd); }

        G4LogicalVolume *GetLogGEMbox() { return box_GEM_log; }


    private:
        // Basic Volumes
        //

        G4Box *fHousing_box = nullptr;
        G4LogicalVolume *fHousing_log = nullptr;
        G4VPhysicalVolume *fHousing_phys = nullptr;

        G4LogicalVolume *fWorldLV = nullptr;

        G4LogicalVolume *fWorldLV_rot = nullptr; // для повернутой мишени

        // для магнита D2 GEM

        G4LogicalVolume *fLogicD2gap = nullptr;
        G4LogicalVolume *fLogicD2gap_add1 = nullptr; // дополнительный объем после магнита для вываливания поля
        G4LogicalVolume *fLogicD2gap_add2 = nullptr;

        G4LogicalVolume *fLogicD1gap = nullptr;
        G4LogicalVolume *fLogicD3gap = nullptr;

        G4LogicalVolume *fLogicQFgap = nullptr;
        G4LogicalVolume *fLogicQDgap = nullptr;

        G4double x_pos_qf = 0., y_pos_qf = 0., z_pos_qf = 0.;
        G4double x_pos_qd = 0., y_pos_qd = 0., z_pos_qd = 0.;

        G4LogicalVolume *box_GEM_log = nullptr;

        void DefineRotationMatrices();

        void VisAttributes();

        void SurfaceProperties();

        ////

        G4Material *Vacuum = nullptr;
        G4Material *Iron = nullptr;
        G4Material *Steel = nullptr;
        G4Material *Beryllium = nullptr;

        ////

        G4VisAttributes *Alum_VisAtt = nullptr;
        G4VisAttributes *Foil_VisAtt = nullptr;
        G4VisAttributes *Mag_VisAtt = nullptr;
        G4VisAttributes *Steel_VisAtt = nullptr;
        G4VisAttributes *Shield_VisAtt = nullptr;
        G4VisAttributes *Gas_VisAtt = nullptr;


        ///
        G4RotationMatrix Rotate180X;
        G4RotationMatrix Rotate180Y;
        G4RotationMatrix Rotate180Z;
        G4RotationMatrix Rotate270X;
        G4RotationMatrix Rotate90X;
        G4RotationMatrix Rotate9X;
        G4RotationMatrix Rotate99X;
        G4RotationMatrix Rotate45X;
        G4RotationMatrix Rotate45X180Z;
        G4RotationMatrix Rotate10X;
        G4RotationMatrix Rotate25X;
        G4RotationMatrix Rotate65X;
        G4RotationMatrix Rotate90Y;
        G4RotationMatrix Rotate270Y;
        G4RotationMatrix RotateNull;
        G4RotationMatrix Rotate90X180Z;
        G4RotationMatrix Rotate90Y180Z;
        G4RotationMatrix Rotate90Z;
        G4RotationMatrix Rotate90Y180X;
        G4RotationMatrix Rotate270Z;
        G4RotationMatrix Rotate270Y180X;

        ///

        DetectorConstruction *fConstructor = nullptr;

        G4double housing_x = 0.;
        G4double housing_y = 0.;
        G4double housing_z = 0.;

        G4bool fCheckOverlaps = true; // option to activate checking of volumes overlaps
    };

    // class DetectorConstruction;

    // inline void DetectorConstruction::


} // namespace Cosmic


#endif // COSMIC_FORTAGGINGSYSTEM_HH

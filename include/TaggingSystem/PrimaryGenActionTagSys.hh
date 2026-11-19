//
// Created by Арсений Юрченко on 12.11.2026.
//

#ifndef PRIMARYGENACTIONTAGSYS_HH
#define PRIMARYGENACTIONTAGSYS_HH

#include "Constants.hh"

#include "Randomize.hh"


#include "G4NistManager.hh"


#include "G4Event.hh"
#include "G4ParticleGun.hh"

#include "G4SDManager.hh"


#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>

#include "PrimaryGeneratorAction.hh"
// #include <TString.h>

class PrimaryGeneratorAction;

namespace Cosmic
{


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

    class ClassGeneratorActionTaggSys
    {
    public:
        ClassGeneratorActionTaggSys()
        {
            event_br = new BetheHeitlerGenerator();

            G4int nofParticles = 1;
            fParticleGun = new G4ParticleGun(nofParticles);
        }
        ~ClassGeneratorActionTaggSys() {}

        void GenerateElectronGEM(G4Event *event);

        G4ParticleGun *GetParticleGun() { return fParticleGun; }

        G4double virtual_photon_random(G4double Egamma_min = 10., G4double Egamma_max = 700.);

        G4ThreeVector GenVertex(G4double, G4double, G4double,
                                G4double); //  метод генерации точки вылета из пучка по гауссу

    private:
        G4double virtual_photon_distrib(G4double);


        BetheHeitlerGenerator *event_br = nullptr;
        G4ParticleGun *fParticleGun = nullptr;
    };


} // namespace Cosmic


#endif // PRIMARYGENACTIONTAGSYS_HH

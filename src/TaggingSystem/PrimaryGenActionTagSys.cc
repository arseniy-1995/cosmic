//
// Created by Арсений Юрченко on 12.11.2026.
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "../TaggingSystem/PrimaryGenActionTagSys.hh"

#include "Constants.hh"


#include "../TaggingSystem/ConstantsTaggSys.hh"


#include <iostream>
// #include <TString.h>


#include "PrimaryGeneratorAction.hh"

#include <TMath.h>

#include "PrimaryGeneratorMessenger.hh"

#include "Constants.hh"
#include "EventInfo.hh"
#include "G4BetheHeitler5DModel.hh"
#include "G4BetheHeitlerModel.hh"
#include "G4Event.hh"
#include "G4OpticalPhoton.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4eBremsstrahlungRelModel.hh"
#include "Randomize.hh"

#include "G4GenericMessenger.hh"
#include "G4ParticleMomentum.hh"
#include "G4ParticleTypes.hh"
#include "G4ThreeVector.hh"


#include "../EcoMug/EcoMug.h"

// #pragma once


namespace Cosmic
{

    // class BetheHeitlerGenerator;


    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    void ClassGeneratorActionTaggSys::GenerateElectronGEM(G4Event *event)
    {

        G4double xx_cell = 0.0, yy_cell = 0.0, zz_cell = 0.0;

        G4ThreeVector initial_pos(0.0, 0.0, 0.0);
        G4ThreeVector initial_momentum_direction(0.0, 0.0, 0.0);

        // initial_pos = G4ThreeVector(0.0, 20.0 * cm, 0.0);
        initial_momentum_direction = G4ThreeVector(0.0, 0.0, 1.0);


        G4double alpha_GEM = alpha0_GEM;
        G4double theta_GEM = 0.0;


        //

        G4double kinetic_energy = 100. * MeV;


        G4double Egamma = 400. * MeV;


        // Egamma = virtual_photon_random(360., 650.) * MeV;
        Egamma = virtual_photon_random(400., 640.) * MeV;
        //  Egamma = virtual_photon_random(150., 780.) * MeV;

        //  Egamma = 0.;
        //  Egamma = virtual_photon_random(350., 680.) * MeV;

        kinetic_energy = Ebeam * MeV - Egamma;

        if (1)
        {
            if (G4UniformRand() > 0.5)
            {
                //  kinetic_energy = (G4UniformRand() > 0.5) ? 160.0 * MeV : 400.0 * MeV;
                kinetic_energy = Ebeam * MeV - Egamma;
            }
            else
            {
                kinetic_energy = Ebeam * MeV;
            }
        }

        //   kinetic_energy = Ebeam * MeV;

        // kinetic_energy = 350.0 * MeV;
        //  kinetic_energy = 200.0 * MeV;

        // небольшая размазка, которая зависит от энергии пучка
        // alpha_GEM = G4RandGauss::shoot(alpha0_GEM, 2.0* (1. - kinetic_energy/ (Ebeam * MeV)) * mrad);
        // если E=Ebeam, alpha_GEM=alpha0_GEM
        // если E=0.5*Ebeam, alpha_GEM=alpha0_GEM +- 1.0 mrad
        // если E=0.2*Ebeam, alpha_GEM=alpha0_GEM +- 0.8 mrad

        // или по формуле для тормозного излучения
        // G4double sigma_alpha_GEM = ((Me * MeV) / kinetic_energy) * rad;
        //  if (kinetic_energy == Ebeam * MeV) sigma_alpha_GEM = 0.;
        //  alpha_GEM = G4RandGauss::shoot(alpha0_GEM,  sigma_alpha_GEM);

        // генерируем вектор тормозного фотона

        G4double energy_Br = 0.0;
        G4double theta_BR = 0.0;
        G4double phi_BR = 0.0;

        G4double e_min_cut = 10 * keV;
        // e_min_cut = 150.0 * MeV;
        G4double e_max_cut = 100 * keV;
        // e_max_cut = -1; // без ката
        // e_max_cut = 700.0 * MeV;
        e_max_cut = 0.2 * kinetic_energy;


        //  BetheHeitlerGenerator event_br;

        event_br->sampleBremsstrahlungGenerator(kinetic_energy, e_min_cut, e_max_cut);
        // BremsstrahlungGenerator(kinetic_energy,e_min_cut ,e_max_cut, energy_Br, theta_BR, phi_BR);
        energy_Br = event_br->photon_energy;
        theta_BR = event_br->photon_theta;
        phi_BR = event_br->photon_phi;


        G4ThreeVector dir_BR(0.0, 0.0, 1.0);
        dir_BR.setTheta(theta_BR);
        // dir_BR.setTheta(alpha0_GEM+theta_BR);
        dir_BR.setPhi(phi_BR);
        // dir_BR = G4ThreeVector(sin(theta_BR)*cos(phi_BR), sin(theta_BR)*sin(phi_BR), cos(alpha_GEM));

        // для генерации за магнитом D2
        alpha_GEM = alpha0_GEM;
        // theta_GEM = asin(((Ebeam*MeV /kinetic_energy))*(sin(alpha0_GEM)+sin(theta0_GEM)) - sin(alpha_GEM)) * rad;

        initial_momentum_direction = G4ThreeVector(-sin(alpha_GEM), 0.0, cos(alpha_GEM));
        // initial_momentum_direction +=dir_BR;  // размазка тормозным излучением
        // initial_momentum_direction.rotate(initial_momentum_direction, )
        auto initial_momentum_direction_old = G4ThreeVector(initial_momentum_direction);
        initial_momentum_direction.rotateY(theta_BR);
        initial_momentum_direction.rotate(phi_BR, initial_momentum_direction_old);
        alpha_GEM = initial_momentum_direction.getTheta();

        theta_GEM = asin(((Ebeam * MeV / kinetic_energy)) * (sin(alpha0_GEM) + sin(theta0_GEM)) - sin(alpha_GEM)) * rad;
        initial_momentum_direction = G4ThreeVector(sin(theta_GEM), 0.0, cos(theta_GEM));


        G4double R_GEM = D_GEM / (sin(alpha_GEM) + sin(theta_GEM));
        G4double R0_GEM = D_GEM / (sin(alpha0_GEM) + sin(theta0_GEM));

        G4double X0_GEM =
            R0_GEM * (cos(alpha0_GEM) - cos(theta0_GEM)); // X на вызоде из магнита D2 для невозмущенной траектории
        G4double X_GEM = R_GEM * (cos(alpha_GEM) - cos(theta_GEM)); // X на вызоде из магнита D2

        //  X_GEM = R_GEM - R_GEM * cos(asin(D_GEM/R_GEM));


        initial_pos = G4ThreeVector((1.0 * mm + (X0_GEM + X_GEM)), 0.0,
                                    L0_GEM + D_GEM + 0 * (Z1_GEM + L_GEM + 20. / 2. * mm + 2. * mm));


        // генерация из мишени для расчета в поле D2, перед магнитом D2

        //  xx_cell = G4RandGauss::shoot(meanX_beam, Xsigma_beam) * mm; // в см
        // yy_cell = G4RandGauss::shoot(meanY_beam, Ysigma_beam) * mm;

        //  G4double r2 = G4UniformRand();
        // zz_cell = -cell_z_size_2 + r2 * l_zz_cell; // равномерно

        // kinetic_energy = 800.0 * MeV;
        // kinetic_energy = 300.0 * MeV;
        // initial_pos = G4ThreeVector(0.0, 0.0, 0.0);

        G4double Xbeam = 0., Ybeam = 0.;
        // initial_pos = G4ThreeVector(0.0, 0.0, 0. * cm);
        //  initial_pos = G4ThreeVector(xx_cell, yy_cell, zz_cell);
        auto initial_pos_in_cell = GenVertex(Xbeam, Ybeam, Xsigma_beam, Ysigma_beam);
        initial_pos_in_cell.rotateY(-alpha0_GEM); // поворот ячейки

        initial_pos = initial_pos_in_cell;


        alpha_GEM = alpha0_GEM;
        initial_momentum_direction = G4ThreeVector(-sin(alpha_GEM), 0.0, cos(alpha_GEM));

        G4LorentzVector Electron_4vec_initial(0, 0, 0, 0); // электрон до тормозного изулучения
        G4ThreeVector Electron_3vec_initial(0.0, 0.0, 1.0); // импульс

        G4LorentzVector Electron_4vec_final(0, 0, 0, 0); // электрон после тормозного изулучения
        G4ThreeVector Electron_3vec_final(0.0, 0.0, 1.0); // импульс

        G4LorentzVector PhotonBr_4vec(0, 0, 0, 0); // фотон тормозного излучения
        G4ThreeVector PhotonBr_3vec(0.0, 0.0, 1.0); // импульс


        // электрон до тормозного излучения
        G4double Pe_initial = sqrt(kinetic_energy * (kinetic_energy + 2. * Me * MeV)); // модуль импульс электрона
        Electron_3vec_initial.setTheta(initial_momentum_direction.getTheta());
        Electron_3vec_initial.setPhi(initial_momentum_direction.getPhi());
        Electron_3vec_initial.setMag(Pe_initial);
        // Electron_4vec_initial.setVect(Pe_vec_initial);
        // Electron_4vec_initial.setE(Eelectron_initial + Me); // полная энергия
        Electron_4vec_initial.setVectMag(Electron_3vec_initial, Me * MeV);

        // фотон тормозного излучения
        G4double Pgamma_br = energy_Br;

        // PhotonBr_3vec.setTheta(theta_BR);
        // PhotonBr_3vec.setPhi(phi_BR);
        PhotonBr_3vec = dir_BR;
        PhotonBr_3vec.setMag(Pgamma_br);
        // PhotonBr_3vec = G4ThreeVector(0.,0.,0.); // зануление
        PhotonBr_4vec.setVectMag(PhotonBr_3vec, 0.);

        // PhotonBr_4vec.setVectMag(G4ThreeVector(0.,0.,0.),0.); // зануление

        // поворачиваем угол фотона по ходу движения начального электрона

        // G4ThreeVector v1(0.0, 0.0, 0.0); // исходный вектор
        // G4ThreeVector v2(0.0, 0.0, 0.0); // целевой вектор
        //
        // v1 = G4ThreeVector(dir_BR);
        // v2 = G4ThreeVector(initial_momentum_direction);
        //
        // // 1. Нормализуем оба вектора
        // G4ThreeVector u1 = v1.unit();
        // G4ThreeVector u2 = v2.unit();
        //
        // // 2. Находим ось вращения через векторное произведение (cross product)
        // G4ThreeVector axis = u1.cross(u2);
        // G4double axisMag = axis.mag();
        //
        // G4RotationMatrix rotMat;
        //
        // if (axisMag > 1e-6) {
        //     axis /= axisMag; // нормализуем ось
        //     // 3. Угол между векторами через скалярное произведение (dot product)
        //     G4double cosAngle = u1.dot(u2);
        //     // Защита от выхода за пределы [-1, 1] из-за погрешностей округления
        //     if (cosAngle > 1.0) cosAngle = 1.0;
        //     if (cosAngle < -1.0) cosAngle = -1.0;
        //     G4double angle = std::acos(cosAngle);
        //
        //     // 4. Заполняем матрицу поворота
        //     rotMat.rotate(angle, axis);
        // } else if (u1.dot(u2) < 0) {
        //     // Если векторы направлены строго в противоположные стороны,
        //     // выбираем любую перпендикулярную ось
        //     G4ThreeVector perpAxis = u1.orthogonal();
        //     rotMat.rotate(M_PI, perpAxis);
        // }
        //
        // // Теперь v1, повернутый с помощью rotMat, будет сонаправлен с v2:
        // G4ThreeVector rotatedV1 = rotMat * v1;

        // обратно перенормируем
        // rotatedV1.setMag(PhotonBr_3vec.mag());

        // PhotonBr_3vec = G4ThreeVector(rotatedV1);
        // PhotonBr_3vec.setMag(Pgamma_br);

        // PhotonBr_4vec.setVect(Pe_vec_initial);
        // PhotonBr_4vec.setE(Eelectron_initial + Me); // полная энергия
        // PhotonBr_4vec.setVectMag(PhotonBr_3vec, 0.0);

        // поворачиваем угол фотона по ходу движения начального электрона

        // PhotonBr_4vec.rotateUz(initial_momentum_direction.unit());
        PhotonBr_4vec = G4LorentzVector(PhotonBr_4vec.rotateUz(initial_momentum_direction.unit()));

        initial_momentum_direction_old = G4ThreeVector(initial_momentum_direction);
        Electron_4vec_final = Electron_4vec_initial - PhotonBr_4vec; // вычетание четырех векторов
        Electron_3vec_final = Electron_4vec_final.getV();
        initial_momentum_direction = Electron_3vec_final.unit(); // нормализуем
        G4double kinetic_energy_initial = kinetic_energy;
        kinetic_energy = Electron_4vec_final.getT() - Me * MeV;
        // kinetic_energy = sqrt(pow(Electron_4vec_final.getV().mag(),2.) + pow(Me * MeV,2.))  - Me * MeV;

        if (kinetic_energy_initial == (Ebeam * MeV)) // для энергии пучка не делаем размазку тормозным излучением
        {
            initial_momentum_direction = initial_momentum_direction_old;
            kinetic_energy = kinetic_energy_initial;
        }


        if (true or (kinetic_energy_initial != (Ebeam * MeV)))
        {
            G4cerr << "initial_kinetic_energy = " << kinetic_energy_initial
                   << " final_kinetic_energy = " << kinetic_energy
                   << " initial_dir = " << initial_momentum_direction_old.getTheta() / mrad << " "
                   << initial_momentum_direction_old.getPhi() / mrad
                   << " final_dir = " << initial_momentum_direction.getTheta() / mrad << " "
                   << initial_momentum_direction.getPhi() / mrad << " BR_dir = " << dir_BR.getTheta() / mrad << " "
                   << dir_BR.getPhi() / mrad << " BR_dir_rot = " << PhotonBr_4vec.getV().getTheta() / mrad << " "
                   << PhotonBr_4vec.getV().getPhi() / mrad << G4endl

                   << G4endl;
        }

        // G4cerr << "initial_kinetic_energy = " << kinetic_energy_initial / MeV << " final_kinetic_energy = " <<
        // kinetic_energy / MeV << " " <<  (Electron_4vec_final.getT() - Me * MeV) / MeV<< G4endl;

        // initial_momentum_direction +=dir_BR;  // размазка тормозным излучением
        // initial_momentum_direction.rotateY(theta_BR);
        initial_momentum_direction_old = G4ThreeVector(initial_momentum_direction);
        // initial_momentum_direction.rotateY(theta_BR);
        //   initial_momentum_direction.rotate(phi_BR, initial_momentum_direction_old);
        // initial_momentum_direction.rotateUz(dir_BR);
        alpha_GEM = initial_momentum_direction.getTheta();

        //  G4cerr << "!!! = " << kinetic_energy / MeV << " " << 1000. * theta_BR  << " " << 1000.*(dir_BR.getTheta())
        //  << " "  << 1000.* alpha0_GEM << " "  <<  1000.*alpha_GEM<< G4endl;
        // G4cerr << "!!! = "  << 1000.*theta0_BR << " " << 1000.* sigma_theta_BR << " " << 1000.*theta_BR << " "
        // <<landau_value << G4endl;

        // для генерации за магнитом D1
        //  if (0)
        if (kinetic_energy == Ebeam * MeV)
        {
            //  kinetic_energy = 150 * MeV;
            kinetic_energy = 800 * MeV;
            initial_pos = G4ThreeVector(2.6 * cm + G4RandGauss::shoot(0., Xsigma_beam),
                                        G4RandGauss::shoot(0., Ysigma_beam), -80. * cm);
            // initial_pos = G4ThreeVector(2.5 * cm, 0.0 * cm, -80. * cm);
            initial_momentum_direction = G4ThreeVector(0.0, 0.0, 1.0);
            // initial_momentum_direction +=dir_BR;  // размазка тормозным излучением
            //  auto initial_momentum_direction_old =  G4ThreeVector(initial_momentum_direction);
            // initial_momentum_direction.rotateY(theta_BR);
            // initial_momentum_direction.rotate(phi_BR, initial_momentum_direction_old);
        }

        // для генерации за магнитом D3
        if (0)
        {
            // kinetic_energy = 150 * MeV;
            kinetic_energy = 800 * MeV;
            // initial_pos = G4ThreeVector(1.6 * cm + G4RandGauss::shoot(0.,Xsigma_beam ),
            // G4RandGauss::shoot(0.,Ysigma_beam ), 120. * cm);
            initial_pos = G4ThreeVector(1.6 * cm, 0.0 * cm, 120. * cm);

            // initial_momentum_direction = G4ThreeVector(0.0, 0.0, 1.0);
            //   G4double sigma_alpha_GEM = ((Me * MeV) / kinetic_energy) * rad;
            //  if (kinetic_energy == Ebeam * MeV) sigma_alpha_GEM = 0.;
            //  alpha_GEM = G4RandGauss::shoot(alpha0_GEM,  sigma_alpha_GEM);
            alpha_GEM = alpha0_GEM;
            theta_GEM =
                asin(((Ebeam * MeV / kinetic_energy)) * (sin(alpha0_GEM) + sin(theta0_GEM)) - sin(alpha_GEM)) * rad;

            initial_momentum_direction = G4ThreeVector(-sin(alpha_GEM), 0.0, cos(alpha_GEM));
            // initial_momentum_direction +=dir_BR;  // размазка тормозным излучение
            auto initial_momentum_direction_old = G4ThreeVector(initial_momentum_direction);
            initial_momentum_direction.rotateY(theta_BR);
            initial_momentum_direction.rotate(phi_BR, initial_momentum_direction_old);
            alpha_GEM = initial_momentum_direction.getTheta();
            theta_GEM =
                asin(((Ebeam * MeV / kinetic_energy)) * (sin(alpha0_GEM) + sin(theta0_GEM)) - sin(alpha_GEM)) * rad;
            initial_momentum_direction = G4ThreeVector(-sin(theta_GEM), 0.0, cos(theta_GEM));
        }
        //  G4cerr << "!!! = " << (X0_GEM + X_GEM ) / mm << G4endl;

        // G4cerr << "!!! = " << kinetic_energy / MeV << " " << X_GEM / mm << " " << theta_GEM << G4endl;

        fParticleGun->SetParticleEnergy(kinetic_energy);

        G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
        G4ParticleDefinition *particle = particleTable->FindParticle("e-");
        fParticleGun->SetParticleDefinition(particle);

        fParticleGun->SetParticleMomentumDirection(initial_momentum_direction);
        fParticleGun->SetParticlePosition(initial_pos);

        fParticleGun->GeneratePrimaryVertex(event);

        // отдельно гененируем тормозной фотон


        G4ParticleDefinition *gamma_particle = particleTable->FindParticle("gamma");
        fParticleGun->SetParticleDefinition(gamma_particle);
        fParticleGun->SetParticleEnergy(energy_Br);
        fParticleGun->SetParticlePosition(initial_pos);
        fParticleGun->SetParticleMomentumDirection((PhotonBr_4vec.getV()).unit());

        if (kinetic_energy_initial == (Ebeam * MeV)) // рисуем тормозные фотоны только от пучка
        {
            fParticleGun->SetParticlePosition(initial_pos_in_cell);
            auto rot_vec = G4ThreeVector(0.0, 0.0, 1.0);
            rot_vec.rotateY(-alpha0_GEM);
            //  rot_vec.rotateY(-5 * degree);
            auto PhotonBr_4vec_from_Ebeam = G4LorentzVector();
            PhotonBr_4vec_from_Ebeam.setVectMag(PhotonBr_3vec, 0.);
            PhotonBr_4vec_from_Ebeam.rotateUz(rot_vec.unit());
            fParticleGun->SetParticleMomentumDirection((PhotonBr_4vec_from_Ebeam.getV()).unit());
            //  fParticleGun->SetParticleMomentumDirection(rot_vec);
            fParticleGun->GeneratePrimaryVertex(event);
        }
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


    // void PrimaryGeneratorAction::BremsstrahlungGenerator(G4double electron_energy0, G4double e_min_cut, G4double
    // e_max_cut, G4double &photon_energy, G4double &photon_theta, G4double &photon_phi)
    // void PrimaryGeneratorAction::BremsstrahlungGenerator()
    void BetheHeitlerGenerator::sampleBremsstrahlungGenerator(G4double electron_energy0, G4double e_min_cut,
                                                              G4double e_max_cut)

    {

        // формула Бете — Гайтлера для тормозного излучения релятивистких эектронов

        // electron_energy Полная энергия налетающего электрона (МэВ)
        // e_cut           Инфракрасный порог обрезания энергии фотона

        if (electron_energy0 <= ((Me * MeV) + e_min_cut))
        {
            G4cerr << "Energy Electron is small" << G4endl;
        }

        // Кинетическая энергия и границы доли энергии y = E_gamma / E_e
        G4double E0_norm = electron_energy0 / (Me * MeV);
        y_min = e_min_cut / electron_energy0;
        y_max = e_max_cut / electron_energy0;

        if (e_max_cut == -1 or y_max >= (1.0 - 1.0 / E0_norm))
        {
            //  y_max = 1.0 - ((Me * MeV) / electron_energy);
            y_max = 1.0 - 1.0 / E0_norm;
            //  y_max = 1.0 - y_min;
        }


        G4double u_min = TMath::Log(y_min);
        G4double u_max = TMath::Log(y_max);

        //  G4cerr << "u_min = " << u_min << " u_max" << u_max << G4endl;

        // Оценка мажоранты (максимума функции g(y) на интервале [y_min, y_max])
        // Максимум достигается на левой границе y = y_min
        G4double f_max = evaluate_pdf_br(y_min, E0_norm) * 1.05;
        // f_max = 4.0 / 3.0;


        // G4cerr <<"y_min ... y_max = " << y_min << "..." << y_max << G4endl;

        if (y_min >= y_max)
        {
            G4cerr << "Incorrect e_cut" << G4endl;
        }

        // 1. Розыгрыш энергии фотона (метод отбора с мажорантой 1/y)
        G4double y = 0.0;
        G4double u = 0.0;
        G4double ln_ratio = TMath::Log(y_max / y_min);

        while (true)
        {
            G4double r1 = G4UniformRand();
            G4double r2 = G4UniformRand();

            // Генерация y согласно плотности p(y) ~ 1/y
            // y = y_min * TMath::Exp(r1 * ln_ratio);

            u = u_min + r1 * (u_max - u_min);
            y = TMath::Exp(u);

            // Функция отклонения g(y) = 1 - y + 0.75 * y^2 (максимум <= 1 на)
            G4double g_y = 1.0 - y + 0.75 * y * y;

            // if (r2 <= g_y) {
            //     break;
            // }

            if (r2 * f_max <= evaluate_pdf_br(y, E0_norm))
            {
                break;
            }

            // G4cerr << "!!!" << G4endl;
        }

        photon_energy = y * electron_energy0;
        Eelectron_final = electron_energy0 - photon_energy;

        // G4cerr <<"photon_energy (MeV) = " << photon_energy / MeV << " electron_energy (MeV) = " << electron_energy /
        // MeV << G4endl;

        // 2. Розыгрыш угла вылета фотона (приближение Цая)
        // Лоренц-фактор электрона
        G4double gamma = electron_energy0 / (Me * MeV);
        gamma = E0_norm;

        // Обратная функция распределения для f(u) ~ u / (1 + u^2)^2:
        // CDF(u) = u^2 / (1 + u^2) = r3  =>  u = sqrt(r3 / (1 - r3))
        G4double r3 = G4UniformRand();
        G4double r4 = G4UniformRand();
        // Защита от деления на ноль при r3 -> 1.0
        if (r3 > 0.999999999)
            r3 = 0.999999999;

        // Упрощенное угловое распределение dN/d(theta) ~ theta / (theta^2 + 1/gamma^2)^2
        G4double u2 = TMath::Sqrt(r3 / (1.0 - r3));
        photon_theta = (1.0 / gamma) * u2;

        // Ограничение угла физическим пределом [0, PI]
        if (photon_theta > M_PI)
            photon_theta = M_PI;

        // Азимутальный угол изотропен
        photon_phi = 2.0 * M_PI * r4;

        photon_theta *= rad;
        photon_phi *= rad;

        /// мой изначальный способ

        // G4double theta0_BR = ((Me * MeV) / kinetic_energy) * rad; // пик распределения
        // theta0_BR /= sqrt(7.0); // пишут, что нужно делить на корень из 7
        // // G4double sigma_theta_BR = TMath::Sqrt(theta0_BR / rad) * rad; // ширину оцениваем как корень
        // //G4double sigma_theta_BR = 1.0 * mrad;
        // G4double sigma_theta_BR = 0.1 *theta0_BR;
        // // Стандартное распределение Ландау в CLHEP генерируется вокруг наиболее вероятного значения (MPV)
        // // ≈ -0.222782 и со стандартным параметром масштаба (ширины) ξ = 1.
        // G4double landau_value = CLHEP::RandLandau::shoot(G4Random::getTheEngine()) + 0.222782;
        // landau_value = fabs(landau_value);
        // //G4double landau_value = G4RandLandau::shoot();
        // // G4double theta_BR = theta0_BR + sigma_theta_BR * landau_value;  // ассиметричное распределение, подгоняем
        // распределением Ландау G4double theta_BR = fabs(G4RandGauss::shoot(theta0_BR / rad,  sigma_theta_BR)) * rad;
        // // theta_BR = 0.;
        //
        // // theta_BR = -1* deg;
        // // G4cerr << "!!! = " << 1000.* (alpha_GEM - alpha0_GEM) << G4endl;
        // G4double phi_BR = 2.0*M_PI * G4UniformRand();

        // через встроенный в GEANT класс

        auto br_model = new G4eBremsstrahlungRelModel();
        // G4BetheHeitlerModel() G4BetheHeitler5DModel
        auto photon_dp = new G4DynamicParticle();
        auto photon_cc = new G4MaterialCutsCouple();
        //  br_model->SampleSecondaries(photon_dp, photon_cc);
    }


    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    G4ThreeVector ClassGeneratorActionTaggSys::GenVertex(G4double X, G4double Y, G4double sigmaX, G4double sigmaY)
    {
        static G4ThreeVector position;
        G4double pos_x, pos_y, pos_z, pos_t;
#ifdef XY_RAND
        pos_x = G4RandGauss::shoot(X, sigmaX);
        pos_y = G4RandGauss::shoot(Y, sigmaY);
#else
        pos_x = X;
        pos_y = Y;
        sigmaX = 0;
        sigmaY = 0;
#endif
        do
        {
            pos_z = G4UniformRand();
            pos_t = G4UniformRand();
        }
        while (pos_t > (1.0 - pos_z));
        pos_z *= (G4UniformRand() > 0.5) ? cell_z_size_2 : -cell_z_size_2;
        // pos_z *= cm;

        position = G4ThreeVector(pos_x, pos_y, pos_z);
        return position;
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    G4double ClassGeneratorActionTaggSys::virtual_photon_distrib(G4double Egamma)
    {
        G4double Eelectron = 2000.0;
        G4double fl1, fl2, f1, f2, sl;
        G4double x = Egamma / Eelectron;
        G4double Egamma_distr_temp;

        // Это код из Томска
        //  fl1 = 2. * Eelectron * (Eelectron - Egamma) / Me / (2. * Eelectron - Egamma);
        //  fl2 = (2. * Eelectron - Egamma) / Egamma;
        //  f1 = 2. * (1. - x + pow(x, 2.0) / 2.);
        //  f2 = pow(x, 2.0) / 2.;
        //  sl = x - 1.;
        //  Egamma_distr_temp = (alpha_em * (f1 * log10(fabs(fl1)) + f2 * log10(fabs(fl2)) + sl) / M_PI);

        // G4double theta_e_max = 5.*M_PI/180.;
        G4double theta_e_max = 84.0 * pow(10.0, -3); // 84 mrad
        Eelectron = 800.0;
        Egamma_distr_temp = (2.0 * alpha_em / M_PI) *
            ((1. - x + pow(x, 2.0) / 2.) * log(fabs((Eelectron - Egamma) * theta_e_max / (Me * x))) - (1. - x));

        return Egamma_distr_temp;
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    G4double ClassGeneratorActionTaggSys::virtual_photon_random(G4double Egamma_min, G4double Egamma_max)
    {
        G4double r1, r2, Egamma, y;

        // G4double Egamma_min = 10., Egamma_max = 700.;
        do
        {
            // r1 = rand() / 2147483647.;
            // r2 = rand() / 2147483647.;
            r1 = G4UniformRand();
            r2 = G4UniformRand();
            //  Egamma = 145. * exp(r1 * log(10.)); // Энергия виртуального фотона в диапозоне 145...1450 МэВ
            //  y = 0.033 * r2;

            Egamma = Egamma_min *
                exp(r1 *
                    log(Egamma_max / Egamma_min)); // Энергия виртуального фотона в диапозоне Egamma_min...Egamma_max
            y = 0.065 * r2; // максимум 0,065
        }
        while (y > virtual_photon_distrib(Egamma));
        // cerr<<y<<"\t"<<dal(om)<<endl;
        return Egamma;
    }

} // namespace Cosmic

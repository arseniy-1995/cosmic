//
// Created by yurchenko on 06.11.2026.
//

#include "PhysicsList.hh"
#include "G4SystemOfUnits.hh"

#include "G4ProcessManager.hh"
#include "G4VModularPhysicsList.hh"
#include "G4VUserPhysicsList.hh"

#include "G4EmStandardPhysics.hh"

// Процессы для электронов/позитронов
#include <G4MuonMinus.hh>
#include <G4MuonPlus.hh>
#include <G4OpticalPhoton.hh>

#include "G4DecayPhysics.hh"
#include "G4MuIonisation.hh"
#include "G4eBremsstrahlung.hh" // Класс тормозного излучения
#include "G4eIonisation.hh"
#include "G4eMultipleScattering.hh"
#include "G4eplusAnnihilation.hh"


// Для синхротронного излучения
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4MuMultipleScattering.hh"
#include "G4MuPairProduction.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleTypes.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessTable.hh"
#include "G4RayleighScattering.hh"
#include "G4StepLimiter.hh"
#include "G4SynchrotronRadiation.hh"
#include "G4SynchrotronRadiationInMat.hh"
#include "G4SystemOfUnits.hh"
#include "G4XrayReflection.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eIonisation.hh"
#include "G4eMultipleScattering.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hIonisation.hh"
#include "G4hMultipleScattering.hh"
#include "G4hPairProduction.hh"


#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4Positron.hh"


#include "FTFP_BERT.hh"
#include "G4OpticalPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

// class PhysicsListMessenger;
class G4DecayPhysics;

namespace Cosmic
{
    MyPhysicsList::MyPhysicsList() : G4VUserPhysicsList(), fMess(nullptr)
    {
        SetDefaultCutValue(1. * km);
        SetVerboseLevel(1);

        fSR_on_off = false;
        fSR_on_off = true;

        fSRType = true;
        // fSRType = false;

        fMess = new PhysicsListMessenger(this);

        // Добавляем стандартную электромагнитную физику
        RegisterPhysics(new G4EmStandardPhysics());

        // Добавляем процессы распада частиц
        fDecayPhysics = new G4DecayPhysics();
        RegisterPhysics(fDecayPhysics);


        // Добавляем регистрацию оптических процессов сверху базового списка
        fOpticalPhysics = new G4OpticalPhysics();
        //  RegisterPhysics(fOpticalPhysics);


        // Добавляем радиоактивный распад
        RegisterPhysics(new G4RadioactiveDecayPhysics());

        fSync = new G4SynchrotronRadiation();
        // RegisterPhysics(fSync);
        // Подключаем для электронов
        //  G4ProcessManager* pManager = G4Electron::ElectronDefinition()->GetProcessManager();
        //  pManager->AddDiscreteProcess(fSync);

        // При необходимости — для позитронов (и других заряженных частиц)
        // pManager = G4Positron::PositronDefinition()->GetProcessManager();
        // pManager->AddDiscreteProcess(fSync);

        // G4ProcessManager* pManager = G4Electron::ElectronDefinition()->GetProcessManager();
        // Добавление процесса тормозного излучения
        // G4eBremsstrahlung* brems = new G4eBremsstrahlung();
        // pManager->AddDiscreteProcess(brems);
    }
    // 1. Регистрируем частицы
    void MyPhysicsList::ConstructParticle()
    {

        G4Gamma::GammaDefinition();
        G4Electron::ElectronDefinition();
        G4Positron::PositronDefinition();
        G4MuonMinus::MuonMinusDefinition();
        G4MuonPlus::MuonPlusDefinition();
        G4OpticalPhoton::OpticalPhotonDefinition();

        fDecayPhysics->ConstructParticle();
    }
    // 2. Назначаем процессы
    void MyPhysicsList::ConstructProcess()
    {
        // Обязательный процесс для перемещения всех частиц в геометрии
        // Добавление базового транспорта (обязательно)
        AddTransportation();

        fDecayPhysics->ConstructProcess();

        // Пример ручного добавления EM-процессов для Гамма-квантов

        ConstructEM();
    }

    // 3. Задаем пространственные пороги (Cuts)
    // Установка порогов рождения вторичных частиц
    void MyPhysicsList::SetCuts()
    {
        // Порог в 1 мм означает: фотоны с пробегом < 1 мм не рождаются как отдельные треки,
        // а их энергия учитывается локально как потери электрона (Continuous energy loss).
        // Запретить рождение вторичных частиц, если их пробег меньше 1 мм
        // SetCutsWithDefault();
        // Или вручную: SetCutValue(1.0*mm, "gamma");

        // Задается в единицах длины, Geant4 сам пересчитает это в энергию для каждого материала
        SetCutsWithDefault();
        SetCutValue(1.0 * mm,
                    "gamma"); // Фотоны тормозного излучения с пробегом < 1 мм не будут рождаться как отдельные частицы
        SetCutValue(1.0 * mm, "e-");
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    void MyPhysicsList::ConstructEM()
    {
        // Получаем указатель на частицы через итератор
        auto particleIterator = GetParticleIterator();
        particleIterator->reset();

        auto fSync = new G4SynchrotronRadiation();

        while ((*particleIterator)())
        {
            G4ParticleDefinition *particle = particleIterator->value();
            G4ProcessManager *pmanager = particle->GetProcessManager();
            G4String particleName = particle->GetParticleName();

            if (particleName == "gamma")
            {
                pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
                pmanager->AddDiscreteProcess(new G4ComptonScattering);

                // для синхротронного излучения
                if (fSR_on_off)
                {
                    pmanager->AddDiscreteProcess(new G4GammaConversion);
                    pmanager->AddDiscreteProcess(new G4RayleighScattering);
                    pmanager->AddDiscreteProcess(new G4XrayReflection);
                }

                // Добавляем наш собственный процесс
                // pmanager->AddDiscreteProcess(new MyCustomInteraction());
            }

            // Примечание: Числа -1, 3, 3 задают порядок вызова процессов на стадиях AtRestDoIt, AlongStepDoIt и
            // PostStepDoIt соответственно.

            if (particleName == "e-")
            {
                // Электрон: множественное рассеяние, ионизация и тормозное излучение
                pmanager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
                pmanager->AddProcess(new G4eIonisation, -1, 2, 2);
                pmanager->AddProcess(new G4eBremsstrahlung, -1, 3, 3); // Подключение тормозного излучения

                // для синхротронного излучения
                if (fSR_on_off)
                {
                    if (fSRType)
                    {
                        pmanager->AddProcess(fSync, -1, -1, 3);
                    }
                    else
                    {
                        pmanager->AddProcess(new G4SynchrotronRadiationInMat, -1, -1, 3);
                    }
                    pmanager->AddProcess(new G4StepLimiter, -1, -1, 4);
                }
            }
            else if (particleName == "e+")
            {
                // Позитрон: то же самое + аннигиляция
                pmanager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
                pmanager->AddProcess(new G4eIonisation, -1, 2, 2);
                pmanager->AddProcess(new G4eBremsstrahlung, -1, 3, 3); // Подключение тормозного излучения
                pmanager->AddProcess(new G4eplusAnnihilation, 0, -1, 4);

                // для синхротронного излучения
                if (fSR_on_off)
                {
                    if (fSRType)
                    {
                        pmanager->AddProcess(fSync, -1, -1, 4);
                    }
                    else
                    {
                        pmanager->AddProcess(new G4SynchrotronRadiationInMat, -1, -1, 4);
                    }
                    pmanager->AddProcess(new G4StepLimiter, -1, -1, 5);
                }
            }
            else if (particleName == "mu+" || particleName == "mu-")
            {
                // muon
                pmanager->AddProcess(new G4MuMultipleScattering, -1, 1, -1);
                pmanager->AddProcess(new G4MuIonisation, -1, 2, 1);
                pmanager->AddProcess(new G4MuBremsstrahlung, -1, 3, 2);
                pmanager->AddProcess(new G4MuPairProduction, -1, 4, 3);
                if (fSR_on_off)
                {
                    pmanager->AddProcess(fSync, -1, -1, 4);
                    pmanager->AddProcess(new G4StepLimiter, -1, -1, 5);
                }
            }
            else if (particleName == "proton")
            {
                // proton
                pmanager->AddProcess(new G4hMultipleScattering, -1, 1, -1);
                pmanager->AddProcess(new G4hIonisation, -1, 2, 1);
                pmanager->AddProcess(new G4hBremsstrahlung, -1, 3, 2);
                pmanager->AddProcess(new G4hPairProduction, -1, 4, 3);
                if (fSR_on_off)
                {
                    pmanager->AddProcess(fSync, -1, -1, 4);
                    pmanager->AddProcess(new G4StepLimiter, -1, -1, 5);
                }
            }
            else if (particle->GetPDGCharge() != 0.0 && !particle->IsShortLived())
            {
                if (fSR_on_off)
                {
                    pmanager->AddProcess(fSync, -1, -1, 1);
                }
            }
        }
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    void MyPhysicsList::SetXrayReflectionRoughness(G4double SurfaceRoughness)
    {
        G4ProcessTable *theProcessTable = G4ProcessTable::GetProcessTable();
        G4XrayReflection *XrayReflectionProcess =
            (G4XrayReflection *)theProcessTable->FindProcess("XrayReflection", "gamma");
        if (XrayReflectionProcess)
            XrayReflectionProcess->SetSurfaceRoughness(SurfaceRoughness);
        else
            G4cout << "Warning. No process XrayReflection found, "
                      "SetXrayReflectionRoughness ignored"
                   << G4endl;
    }


} // namespace Cosmic

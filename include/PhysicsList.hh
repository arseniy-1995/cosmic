//
// Created by yurchenko on 06.11.2026.
//

#ifndef PHYSICSLIST_HH
#define PHYSICSLIST_HH

// #include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"

#include <G4DecayPhysics.hh>
#include <G4SynchrotronRadiation.hh>

#include "G4VModularPhysicsList.hh"
#include "G4VUserPhysicsList.hh"

#include "FTFP_BERT.hh"
#include "G4OpticalPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4SystemOfUnits.hh"

#include "G4VDiscreteProcess.hh"


namespace Cosmic
{

    class PhysicsListMessenger;

    class MyPhysicsList : public G4VModularPhysicsList
    {
        // class MyPhysicsList: public FTFP_BERT {
        // class MyPhysicsList: public G4VUserPhysicsList {
    public:
        // MyPhysicsList() : G4VUserPhysicsList() {};
        // MyPhysicsList();
        MyPhysicsList();
        virtual ~MyPhysicsList() {};
        // ~MyUserPhysicsList();


    protected:
        void SetCuts() override;

        // Construct particles
        void ConstructProcess() override;

        // Construct processes and register them
        void ConstructParticle() override;

        inline void SetAnalyticSR(G4bool val) { fSRType = val; };
        void SetXrayReflectionRoughness(G4double SurfaceRoughness);

    private:
        void ConstructEM();

        G4bool fSRType = false;
        G4bool fSR_on_off = false;

        // PhysicsListMessenger* fMessenger;
        PhysicsListMessenger *fMess = nullptr;

        G4OpticalPhysics *fOpticalPhysics = nullptr;

        G4DecayPhysics *fDecayPhysics = nullptr;
        G4SynchrotronRadiation *fSync = nullptr;
        // PhysicsListMessenger* fMess;
    };


    class MyCustomInteraction : public G4VDiscreteProcess
    {
    public:
        MyCustomInteraction(const G4String &name = "MyCustomProcess") : G4VDiscreteProcess(name) {}

        // Возвращает длину свободного пробега частицы
        virtual G4double GetMeanFreePath(const G4Track &, G4double, G4ForceCondition *) override
        {
            return 10.0 * cm; // Или расчет на основе сечения взаимодействия
        }

        // Описывает, что происходит в точке взаимодействия (финальное состояние)
        virtual G4VParticleChange *PostStepDoIt(const G4Track &track, const G4Step &step) override
        {
            aParticleChange.Initialize(track);

            // Пример: уничтожаем первичную частицу
            aParticleChange.ProposeTrackStatus(fStopAndKill);

            return &aParticleChange;
        }
    };
} // namespace Cosmic

#endif // START_TIME_DETECTOR_PHYSICSLIST_HH

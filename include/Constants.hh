//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file Constants.hh
/// \brief Definition of B5 example constants.

#ifndef CosmicConstants_h
#define CosmicConstants_h 1

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

// #include <iostream>
#define isGenElectronGEM

namespace StartTimeDetector
{


    // #define isFEU187 // ФЭУ 187 диаметр 20 мм + световод c двух/одной стороны
    // #define isFEU187_quadro // ФЭУ 4 шт, по 2 на каждую сторону, сцинтиллятор шириной 40 мм
#define isFEU187_mirror // ФЭУ 187 диаметр 20 мм + световод c одной стороны + зеркало
    // #define isFEU187_two_mirror // дваФЭУ 187 диаметр 20 мм + световод c одной стороны + зеркало
    // #define isFEU187_black_mirror // вместо зеркала черное зеркало
    // #define isFEU187_two_black_mirror // вместо зеркала черное зеркало
    // #define isFEU187_all_black_mirror // вместо зеркала черное зеркало со всех сторон
    // #define isFEU187_two_all_black_mirror // вместо зеркала черное зеркало со всех сторон
    // #define isFEU187_three_svetovod // ФЭУ 187 диаметр 20 мм + три световода с двух стороны изогнутой формы обход
    // камеры #define isFEU187_quadro_svetovod // ФЭУ 187 диаметр 20 мм + четыре световода с двух стороны, изогнутой
    // формы c одной стороны обход камеры

#define isFEU_PMT // режим фотокатода диаметром 18 мм

#if defined(isFEU187_quadro) or defined(isFEU187_quadro_svetovod)
#define isFEU187
#endif

#if defined(isFEU187_black_mirror) || defined(isFEU187_two_black_mirror) || defined(isFEU187_all_black_mirror) ||      \
    defined(isFEU187_two_all_black_mirror)
#define isFEU187_mirror
#endif

#if defined(isFEU187_three_svetovod) || defined(isFEU187_mirror) || defined(isFEU187_two_mirror)
#define isFEU187
#endif


    constexpr G4double ScintilStartTimeDetector_x_const = 40.0 * mm / 2.;
    constexpr G4double ScintilStartTimeDetector_y_const = 20.0 * mm / 2.;
    constexpr G4double ScintilStartTimeDetector_z_const = 22.0 * cm / 2.;


    constexpr G4double SiPMHalfLength = 3. * mm / 2.;
    constexpr G4double SiPMHalfHeight = 1.45 * mm / 2.;

    //  constexpr G4int fNof_plastic_TRIG = 2;

    // пороги записи в файл
    constexpr G4double plastic_TRIG_threshold = 0.1 * MeV;
    constexpr G4double plastic_SCINT_threshold = 0.1 * MeV;
    constexpr G4double PMT_threshold = 1; // число фотонов
    constexpr G4double WLSsipm_threshold = 1; // число фотонов
    constexpr G4double SiPM_threshold = 1; // число фотонов

    constexpr G4double DENSITY_LO = 1.032; // scintill. density - for light output calculation


#if not(defined(isScinFiveParts) or defined(isScinFivePartsVer2))
    constexpr G4int fNof_plastic_SCINT_START_TIME_DETECTOR = 1;
#endif

#if not defined(isFEU187)
    constexpr G4int fNof_PMT_START_TIME_DETECTOR = 5; // число ФЭУ // 5
#endif

#if defined(isFEU187)

#if not defined(isFEU187_three_svetovod) and not defined(isFEU187_mirror)
#if not(defined(isFEU187_quadro) or defined(isFEU187_quadro_svetovod) or defined(isFEU187_two_mirror) or               \
        defined(isFEU187_two_black_mirror) or defined(isFEU187_two_all_black_mirror))
    constexpr G4int fNof_PMT_START_TIME_DETECTOR = 2;
#endif
#endif
#if defined(isFEU187_mirror) and                                                                                       \
    not(defined(isFEU187_two_mirror) or defined(isFEU187_two_black_mirror) or defined(isFEU187_two_all_black_mirror))
    constexpr G4int fNof_PMT_START_TIME_DETECTOR = 1;
#endif
#if defined(isFEU187_two_mirror) or defined(isFEU187_two_black_mirror) or defined(isFEU187_two_all_black_mirror)
    constexpr G4int fNof_PMT_START_TIME_DETECTOR = 2;
#endif
#if defined(isFEU187_three_svetovod)
    constexpr G4int fNof_PMT_START_TIME_DETECTOR = 3;
#endif

#if defined(isFEU187_quadro) or defined(isFEU187_quadro_svetovod)
    constexpr G4int fNof_PMT_START_TIME_DETECTOR = 4;
#endif

#endif


    constexpr G4int fNof_WLSsipm_START_TIME_DETECTOR = 4; // число WLS SiPM // 5

#if defined(isScinFiveParts) or defined(isScinFivePartsVer2)
    constexpr G4int fNof_plastic_SCINT_START_TIME_DETECTOR = fNof_PMT;
#endif

#if not(defined(isRUN21) || defined(isRUN23) || defined(isRUN21SvetovodSiPM) || defined(isWLSFiber))
    constexpr G4int fNof_SiPM_START_TIME_DETECTOR = 8;
    constexpr G4int fNof_SiPMgroup_START_TIME_DETECTOR = 2; // 2
#endif

#ifdef isWLSFiber
    constexpr G4int fNof_SiPM_START_TIME_DETECTOR = fNof_WLSsipm; // число SiPM // 5
    constexpr G4int fNof_SiPMgroup_START_TIME_DETECTOR = 2; // 2
#endif

#if defined(isRUN21) || defined(isRUN21SvetovodSiPM)
    constexpr G4int fNof_SiPM_START_TIME_DETECTOR = 8;
    constexpr G4int fNof_SiPMgroup_START_TIME_DETECTOR = 2; // 2
#endif
#ifdef isRUN23
    constexpr G4int fNof_SiPM_START_TIME_DETECTOR = 24 + 0; // 8
    constexpr G4int fNof_SiPMgroup_START_TIME_DETECTOR = 6; // 8
#endif


} // namespace StartTimeDetector


namespace Cosmic {

    // #define isGenCosmic

    // #define isGenLQ
    // #define isTrigLQ

    // #define isGenPNpair
    // #define isGenPPpair

#define isGenElectronGEM

#define isTrigPN

// #define isTrig_accept_all

    constexpr G4int T2M_zanulenie[3] = {1, 1, 1}; // если 1, то зануления нет, 0 - зануление есть T20, T21, T22

#define SW(N) " " << std::setw(N)

    //#define RUN21
#define RUN23
#define LOWQ_CONVERTOR

#ifdef isGenCosmic
#define CONCRETE // Бетон
#endif

#define PF1_FAT // Толстые пластики
#define PF2_FAT

#define PF1_THIN // Тонкие пластики
#define PF2_THIN

#define HADCAL1 // Адронный калориметр
#define HADCAL2

    //#define MRPC1 // MRPC-детекторы
    //#define MRPC2

#define DCARM1 // Дрейфовая камера
#define DCARM2
#define VCARM1 // Вершинная камера
#define VCARM2

#define TARGET // Мишень+фольга
#define MAGNET_CELL // Магнит мишени
#define CUBIK
#define CELL
#define INJECT_TUBE
#define SHIELD // Фольга (экран) вокруг кубика
#define FOIL
#define VACUUM_TUBE

#define TAGG_SYS

#define MAGNET_D2
#define GEM
#define START_TIME_DETECTOR

    // дипольные магниты
#define MAGNET_D1
#define MAGNET_D3

    // квадрупольные магниты
#define MAGNET_QF
#define MAGNET_QD

#define WINDOW_E // Берилиевое окно для выхода рассеяного электрона
#define UPPUMP
#define DOWNPUMP
#define PUMP
    // #define E_PLASTIC // пластик электронного плеча
    // #define GAMMA_PLASTIC // гамма пластик электронного плеча
#define GAMMA_PLASTIC2 // гамма пластик электронного плеча (другая версия)

#define LOWQ1 // электронные плечи ЛО-КУ поляриметра
#define LOWQ2


#define NX_BARS 22
#define NZ_BARS 22
#define N_UNITS (NX_BARS / 2) /* assuming square calorimetr */
#define N_LAYERS 10
#define NVC_WRS 42
#define NW1_WRS 18
#define NW2_WRS 18
#define NW3_WRS 27
#define NCX_BRS 7
#define N_LQ 2

#define AC_IND 0
#define N_AC 2
#define DE_IND (AC_IND + N_AC)
#define N_DE 8
#define HCX_IND (DE_IND + N_DE) /* X bars */
#define N_HCX NX_BARS *N_LAYERS
#define HCZ_IND (HCX_IND + N_HCX) /* Z bars */
#define N_HCZ NZ_BARS *N_LAYERS

#define N_AC 1
#define TOF_IND (AC_IND+N_AC)        /* MRPC */
#define N_TOF 1

    // #define VC_IND (HCZ_IND + N_HCZ)    /* Vertex Ch */
//#define WC1_IND (VC_IND + NVC_WRS)  /* Drift Chambers */
//#define WC2_IND (WC1_IND + NW1_WRS) /* Drift Chambers */
//#define WC3_IND (WC2_IND + NW2_WRS) /* Drift Chambers */

#define LQ_IND (WC3_IND + NW3_WRS)  /* LQ layers */
#define ARM1_IND 0
#define ARM2_IND (LQ_IND + N_LQ)


#define VC_IND 0    /* Vertex Ch */
#define WC1_IND 0 /* Drift Chambers */
#define WC2_IND (WC1_IND + NW1_WRS) /* Drift Chambers */
#define WC3_IND (WC2_IND + NW2_WRS) /* Drift Chambers */



#define NSENS ARM2_IND * 2

// detector element type
#define DT_WIRE 0
#define DT_SCINT 1
#define DT_EMCAL 2

#define DT_SCINT_THIN 11
#define DT_SCINT_FAT 22
#define DT_SCINT_LQ 33

#define DC_RES (0.3*mm)
#define VC_RES (0.6*mm)


    constexpr G4int fNofLayers_plastic_fat_nsys1 = 6; // число толстых пластиков
    constexpr G4int fNofLayers_plastic_fat_nsys2 = 8;

    constexpr G4int fNofLayers_plastic_thin_nsys1 = 2; // число тонких пластиков
    constexpr G4int fNofLayers_plastic_thin_nsys2 = 2;



#ifdef RUN21
    constexpr G4int fNofLayers_plastic_LQ_nsys1 = 2; // число пластиков LQ электронного плеча
    constexpr G4int fNofLayers_plastic_LQ_nsys2 = 2;
#endif // RUN21

#ifdef RUN23
    constexpr G4int fNofLayers_plastic_LQ_nsys1 = 1; // число пластиков LQ электронного плеча
    constexpr G4int fNofLayers_plastic_LQ_nsys2 = 1;
#endif // RUN23

    constexpr G4int fNofLayers_HadrtonCalorimeter_nsys1 =
        N_HCX + N_HCZ + 0; // число чувствительных элементов в калориметре
    constexpr G4int fNofLayers_HadrtonCalorimeter_nsys2 = N_HCX + N_HCZ + 0;

    constexpr G4int fNofLayers_W_Chamber_nsys1 = NW1_WRS + NW2_WRS + NW3_WRS + NVC_WRS + 100;
    constexpr G4int fNofLayers_W_Chamber_nsys2 = NW1_WRS + NW2_WRS + NW3_WRS + NVC_WRS + 100;
    constexpr G4int fNofLayers_V_Chamber_nsys1 = NVC_WRS + 100;
    constexpr G4int fNofLayers_V_Chamber_nsys2 = NVC_WRS + 100;

    // пороги записи в файл (по тригеру)
    constexpr G4double plastic_fat_threshold = 0.5 * MeV;
    constexpr G4double plastic_thin_threshold = 0.1 * MeV;
    constexpr G4double plastic_LQ_threshold = 0.1 * MeV;
    constexpr G4double HadronCalorimeter_threshold = 1.0 * MeV;
    constexpr G4double GEM_threshold_TDC = 0.1 * keV;
    //   constexpr G4double WChamber_threshold = 0.5 * keV;
    //   constexpr G4double VChamber_threshold = 0.5 * keV;

    constexpr G4double WChamber_threshold = 0.01 * keV;
    constexpr G4double VChamber_threshold = 0.01 * keV;


    // пороги по ВЦП
    constexpr G4double plastic_fat_threshold_TDC = 15. * MeV;
    constexpr G4double plastic_thin_threshold_TDC = 0.5 * MeV;
    constexpr G4double plastic_LQ_threshold_TDC = 0.5 * MeV;
    constexpr G4double HadronCalorimeter_threshold_TDC = 0.5 * MeV;


    constexpr G4double WChamber_threshold_TDC = 0.01 * keV;
    constexpr G4double VChamber_threshold_TDC = 0.01 * keV;


    constexpr G4double DENSITY_LO = 1.032; // scintill. density - for light output calculation


    // FOR GENBOS

#define GENBOS

#define PHIC    90.*0.0174532925199
#define DPHI    35.*0.0174532925199
#define THDN 40. * 0.0174532925199
#define XY_RAND
#define VMAX 8
    constexpr G4double cell_z_size_2 = 20.0 * cm;
    constexpr G4double meanX_beam = 0., meanY_beam = 0.;
    // constexpr G4double Xsigma_beam = 0.7 * mm, Ysigma_beam = 0.3 * mm; //for 2GeV
    constexpr G4double Xsigma_beam = 0.47 * mm, Ysigma_beam = 0.18 * mm; // for 800Mev из базы данных захода
    // constexpr G4double Xsigma_beam = 0.0 * mm, Ysigma_beam = 0.0 * mm;

    // FOR LQ-GENERATOR

#define Md 1875.61279347// ????? ????????
#define Mp 938.2720881629 // ????? ???????
const G4double mu_proton = 2.79285;
#define Mn 939.5654205254 // ????? ????????=
#define Me 0.51099895069 // Масса электрона МэВ

#define Mup 2.7928 // ????????? ?????? ???????
#define Mun -1.9130 // ????????? ?????? ????????
#define Mud 0.8574 // ????????? ?????? ????????

#define Degree .01745329252      /* degree->radians*/
#define Fm 197.32705359 // hc = ???*?? ?????????? ??????????
#define alpha_em 1./137.035989561 // ?????????? ?????? ?????????
#define charge_e 1.6021773349 // ????? ?????????, ????????? e-19 ??????
    constexpr G4double l_zz_cell = 40.0; // длина (полная) накопительной ячейки, см
    constexpr G4double Ebeam = 800.0;  // Энергия электронов, МэВ

#ifdef RUN21
    constexpr G4double delta_add_counter = 5.0; //добавляем несколько см, чтобы захватить большую площадь счетчика
#endif

#ifdef RUN23
    constexpr G4double delta_add_counter = 10.0; //добавляем несколько см, чтобы захватить большую площадь счетчика
#endif

    constexpr G4double l_theta_counter = 12.5 + delta_add_counter; // половина размера счетчика в тета направлении, см 15
    constexpr G4double l_phi_counter = 25.0 + delta_add_counter; // половина размера счетчика в тета направлении, см 25

#ifdef RUN21
    constexpr G4double R0_counter = 83.0; // радиус от центра мишени до середины счетчика, см
    constexpr G4double theta0_counter = 30.0 * Degree; // угол центра счетчика 30 градусов -> радианы
#endif
#ifdef RUN23
    constexpr G4double R0_counter = 80.0; // радиус от центра мишени до середины счетчика, см
    constexpr G4double theta0_counter = 33.0 * Degree; // угол центра счетчика 30 градусов -> радианы
#endif


    // constexpr G4double Pzz1 = 1.0, r = -2.0;
    // constexpr G4double Pzz2 = r * Pzz1;


        constexpr G4double Pzz1 = 0.39, r = -1.7;
    constexpr G4double Pzz2 = r * Pzz1;
}

#endif

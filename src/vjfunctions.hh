//  vjfunctions.hh
//  Veronica Verkest August 2018

#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/Selector.hh"
#include "fastjet/tools/Filter.hh"
#include "fastjet/tools/Pruner.hh"
#include "fastjet/contrib/SoftDrop.hh"

#include "TROOT.h"
#include "TRandom.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TF1.h"
#include "TH2.h"
#include "TH3.h"
#include "TClonesArray.h"
#include "TLatex.h"
#include "TMathText.h"
#include "TProfile.h"

// TStarJetPico
#include "TStarJetPicoReader.h"
#include "TStarJetPicoEvent.h"
#include "TStarJetPicoEventHeader.h"
#include "TStarJetPicoEventCuts.h"
#include "TStarJetPicoPrimaryTrack.h"
#include "TStarJetPicoTower.h"
#include "TStarJetPicoTrackCuts.h"
#include "TStarJetPicoTowerCuts.h"
#include "TStarJetVectorContainer.h"
#include "TStarJetVector.h"
#include "TStarJetPicoTriggerInfo.h"
#include "TStarJetPicoUtils.h"

#include <string>
#include <iostream>
#include <sstream>
#include <sstream>
#include <iomanip>
#include <cmath>

#ifndef vjfunctions_hh
#define vjfunctions_hh

namespace vjet {

  const int d_=3;    const int g_=2;

  const double beta = 0.0;
  const double z_cut = 0.1;
  
  const double numEvents = -1;       //  NUMBER OF EVENTS  (-1 runs all)
  const double pi = 3.14159265;
  const double R = 0.4;
  const double absMaxVz = 30.0;   // |Vz|<=30 cm
  const double etaCut = 1.0;
  const double partMinPt = 0.2;      //  particle Pt >= 0.2 GeV
  const double jetMinPt = 2.0;      //  Jet Pt >= 2.0 GeV
  const double jetMaxPt = 60.0;      //  Jet Pt <= 60.0 GeV

  const std::string y6PPTowerList = "src/Combined_y7_PP_Nick.txt";
  const TString chainName[d_] = { "JetTree", "JetTreeMc", "JetTree" };
  const TString dataName[d_] = { "pp High Tower", "Pythia 6 MC", "Pythia 6 + GEANT" };  
  const TString dataString[d_] = { "ppHT", "P6MC", "P6Ge" };  
  const TString DstFile[d_] = { "ppHT/picoDst*", "AddedGeantPythia/picoDst*", "AddedGeantPythia/picoDst*" };

  const fastjet::Selector etaSelector = fastjet::SelectorAbsEtaMax( 1.0-R );
  const fastjet::Selector ptMinSelector = fastjet::SelectorPtMin( jetMinPt );
  const fastjet::Selector ptMaxSelector = fastjet::SelectorPtMax( jetMaxPt );
  const fastjet::Selector etaPtSelector = etaSelector && ptMinSelector && ptMaxSelector;       //   JET SELECTOR
  
  std::vector<fastjet::PseudoJet> GatherParticles ( TStarJetVectorContainer<TStarJetVector> * container , double etaCutVal, double partMinPtVal, std::vector<fastjet::PseudoJet> & rawParticles );

  //  jetPt, jetEta, jetPhi, jetE, jetM, Rg, Zg, wt, int EventID, nCons;
  
  void FillJetInfo ( std::vector<fastjet::PseudoJet> &rawJets, TTree* Tree, int event, double weight, double &jPt, double &jEta, double &jPhi, double &jE, double &jM, double &jRg, double &jZg, double &jwt, int &jEvent, int &jncons);
  
  bool Vz_candidate( TStarJetPicoEventHeader* header, double VzCut );

  void PrintJet( fastjet::PseudoJet jet );

  double LookupXsec(TString::TString & currentfile );

  void InitReaderPythia( TStarJetPicoReader & reader, TChain* chain, int nEvents );

  void InitReader( TStarJetPicoReader & reader, TChain* chain, int nEvents );
  
}

#endif

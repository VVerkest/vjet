//  V JET FINDER
//  Veronica Verkest    August 15, 2018
//  Script to find jets and explore their observables and correlations

/*  GOALS:
    [  ] Read in ppHT, Pythia MC, or Pythia+GEANT files
    [  ] Find Jets & store information
    [  ] Perform soft-drop
    [  ] Perform jet matching

    [d]  indicates data/MC type
    [0]: ppHT
    [1]: PyMC
    [2]: PyGE

    [g]  indicates soft-drop grooming status
    [0]: ungroomed
    [1]: groomed
*/


#include "vjfunctions.hh"

using namespace fastjet;
using namespace std;
using namespace vjet;

int main () {

  TH1::SetDefaultSumw2( );  // Histograms will calculate gaussian errors
  TH2::SetDefaultSumw2( );
  TH3::SetDefaultSumw2( );
  
  JetDefinition jet_def(antikt_algorithm, R);     //  JET DEFINITION

  TString histoName, histoTitle, canvasName, canvasTitle, NAME, TITLE;       TString fileName;  //  For x-sec

  vector<PseudoJet> rawParticles[d_][g_], Jets[d_][g_], groomedJets[d_][g_];
  double weight, Rval, Zval;

  TFile * outFile = new TFile ("out/vjet.root","RECREATE");



  for ( int d=0; d<d_; ++d ) {

    TChain* Chain[d] = new TChain( chainName[d].c_str );     Chain[d]->Add( "AddedGeantPythia/picoDst*" );
    TStarJetPicoReader Reader[d];                               InitReaderPythia( Reader[d], Chain[d], numEvents );
    TStarJetPicoEventHeader* header[d];    TStarJetPicoEvent* event[d];    TStarJetVector* sv[d];
    TStarJetVectorContainer<TStarJetVector> * container[d];
  
    for ( int g=0; g<g_; ++g ) {
      

      TTree * vjTree[d][g];
      double jetPt[d][g], jetEta[d][g], jetPhi[d][g], jetE[d][g], sdPt[d][g], sdEta[d][g], sdPhi[d][g];
      double sdE[d][g], wt[d][g], Rg[d][g], Zg[d][g];  int EventID[d][g], nJetCons[d][g], nSDCons[d][g];

      NAME = dataString[d] + "JetTree";
      TITLE = dataName[d] + " Jet Tree";
      vjTree[d][g] = new TTree( NAME, TITLE );

    }
    
  }



  
  outFile->Close();
  
  return 0;
}

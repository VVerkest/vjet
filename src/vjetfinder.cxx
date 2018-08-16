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
  
  double weight, Rval, Zval;

  TFile * outFile = new TFile ("out/vjet.root","RECREATE");



  for ( int i=0; i<d; ++i ) {

    TChain* Chain[d] = new TChain( chainName[d].c_str );     Chain[d]->Add( "AddedGeantPythia/picoDst*" );
    TStarJetPicoReader Reader[d];                               InitReaderPythia( Reader[d], Chain[d], numEvents );
    TStarJetPicoEventHeader* header[d];    TStarJetPicoEvent* event[d];    TStarJetVector* sv[d];
    TStarJetVectorContainer<TStarJetVector> * container[d];
  
    for ( int j=0; j<1; ++j ) {
      vector<PseudoJet> rawParticles[d][g], Jets[d][g], groomedJets[d][g];
    }
    
  }

  TTree * sdTree[betaVals][zVals];
  double jetPt[betaVals][zVals], jetEta[betaVals][zVals], jetPhi[betaVals][zVals], jetE[betaVals][zVals], sdPt[betaVals][zVals], sdEta[betaVals][zVals], sdPhi[betaVals][zVals];
  double sdE[betaVals][zVals], wt[betaVals][zVals], Rg[betaVals][zVals], Zg[betaVals][zVals];  int EventID[betaVals][zVals], nJetCons[betaVals][zVals], nSDCons[betaVals][zVals];

  
  outFile->Close();
  
  return 0;
}

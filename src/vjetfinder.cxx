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
  TStarJetPicoReader Reader[d_];  TChain* Chain[d_];
  TStarJetPicoEventHeader* header[d_];    TStarJetPicoEvent* event[d_];
  TStarJetVector* sv[d_];  TStarJetVectorContainer<TStarJetVector> * container[d_];
  TTree * vjTree[d_][g_];  double jetPt[d_][g_], jetEta[d_][g_], jetPhi[d_][g_], jetE[d_][g_], jetM[d_][g_], Rg[d_][g_], Zg[d_][g_], wt[d_][g_];  int EventID[d_][g_], nCons[d_][g_];
  double weight, Rval, Zval;  int ID;
  TFile * outFile = new TFile ("out/vjet.root","RECREATE");



  for ( int d=0; d<d_; ++d ) {
    
    Chain[d] = new TChain( chainName[d] );     Chain[d]->Add( DstFile[d] );
    InitReaderPythia( Reader[d], Chain[d], numEvents );

  
    for ( int g=0; g<g_; ++g ) {

      NAME = dataString[d] + "JetTree";      TITLE = dataName[d] + " Jet Tree";
      vjTree[d][g] = new TTree( NAME, TITLE );
      vjTree[d][g]->Branch("jetPt", &jetPt[d][g]);       vjTree[d][g]->Branch("jetEta", &jetEta[d][g]);       vjTree[d][g]->Branch("jetPhi", &jetPhi[d][g]);       vjTree[d][g]->Branch("jetE", &jetE[d][g]);
      vjTree[d][g]->Branch("jetM", &jetM[d][g]);       vjTree[d][g]->Branch("Rg", &Rg[d][g]);       vjTree[d][g]->Branch("Zg", &Zg[d][g]);       vjTree[d][g]->Branch("wt", &wt[d][g]);
      vjTree[d][g]->Branch("EventID", &EventID[d][g]);       vjTree[d][g]->Branch("nCons", &nCons[d][g]);
    }
    
  }



  // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  BEGIN EVENT LOOP!  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  while ( Reader[0].NextEvent() ) {

    Reader[0].PrintStatus(20); 

    rawParticles[0][0].clear();  Jets[0][0].clear();  //  clear containers

    event[0] = Reader[0].GetEvent();    header[0] = event[0]->GetHeader();

    container[0] = Reader[0].GetOutputContainer();

    if ( Vz_candidate( header[0], absMaxVz ) == false ) { continue; }
    
    ID = Reader[0].GetNOfCurrentEvent();
    fileName =  Reader[0].GetInputChain()->GetCurrentFile()->GetName();
    weight = LookupXsec( fileName );
    
    GatherParticles ( container[0], etaCut, partMinPt, rawParticles[0][0]);

    
    wt[0][0] = weight;
    EventID[0][0] = ID;
    vjTree[0][0]->Fill();
    
  }
  // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  END EVENT LOOP!  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

  

  vjTree[0][0]->Write();
  outFile->Close();
  
  return 0;
}

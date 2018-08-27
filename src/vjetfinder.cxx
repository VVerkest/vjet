//  V JET FINDER
//  Veronica Verkest    August 15, 2018
//  Script to find jets and explore their observables and correlations

/*  GOALS:
    [X] Read in ppHT, Pythia MC, or Pythia+GEANT files
    [  ] Find Jets & store information
    [  ] Perform soft-drop
    [  ] Perform jet matching

    [d]  indicates data/MC type
    [0]: ppHT        383285 events
    [1]: PyMC      4142460 events
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

  vector<PseudoJet> rawParticles[d_], Jets[d_], groomedJets[d_];
  TStarJetPicoReader Reader[d_];  TChain* Chain[d_];
  TStarJetPicoEventHeader* header[d_];    TStarJetPicoEvent* event[d_];
  TStarJetVector* sv[d_];  TStarJetVectorContainer<TStarJetVector> * container[d_];
  TTree * vjTree[d_][g_];  double jetPt[d_][g_], jetEta[d_][g_], jetPhi[d_][g_], jetE[d_][g_], jetM[d_][g_], Rg[d_][g_], Zg[d_][g_], wt[d_][g_];  int EventID[d_][g_], nCons[d_][g_];
  double wtVal, Rval, Zval, Mval;  int ID;
  TFile * outFile = new TFile ("out/vjet.root","RECREATE");

  // jetPt, jetEta, jetPhi, jetE, jetM, Rg, Zg, wt, int EventID, nCons;

  for ( int d=0; d<d_; ++d ) {
    
    Chain[d] = new TChain( chainName[d] );     Chain[d]->Add( DstFile[d] );

    if ( dataString[d] == "ppHT" ) { InitReader( Reader[d], Chain[d], numEvents ); }
    else { InitReaderPythia( Reader[d], Chain[d], numEvents ); }

  
    for ( int g=0; g<g_; ++g ) {

      NAME = dataString[d] + "JetTree";      TITLE = dataName[d] + " Jet Tree";
      vjTree[d][g] = new TTree( NAME, TITLE );
      vjTree[d][g]->Branch("jetPt", &jetPt[d][g]);       vjTree[d][g]->Branch("jetEta", &jetEta[d][g]);       vjTree[d][g]->Branch("jetPhi", &jetPhi[d][g]);       vjTree[d][g]->Branch("jetE", &jetE[d][g]);
      vjTree[d][g]->Branch("jetM", &jetM[d][g]);       vjTree[d][g]->Branch("Rg", &Rg[d][g]);       vjTree[d][g]->Branch("Zg", &Zg[d][g]);       vjTree[d][g]->Branch("wt", &wt[d][g]);
      vjTree[d][g]->Branch("EventID", &EventID[d][g]);       vjTree[d][g]->Branch("nCons", &nCons[d][g]);
    }
    
  }


  // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ppHT EVENT LOOP!  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  while ( Reader[0].NextEvent() ) {
    for ( int g=0; g<g_; ++g ) {
      std::cout << "g: " << g << std::endl;
      Reader[0].PrintStatus(12); 
      // rawParticles[0].clear();  Jets[0].clear();  groomedJets[0].clear();     //  clear containers
      
      event[0] = Reader[0].GetEvent();    header[0] = event[0]->GetHeader();
      container[0] = Reader[0].GetOutputContainer();
      if ( Vz_candidate( header[0], absMaxVz ) == false ) { continue; }
    
      fileName =  Reader[0].GetInputChain()->GetCurrentFile()->GetName();
      ID = Reader[0].GetNOfCurrentEvent();
      wtVal = LookupXsec( fileName );
    
      GatherParticles ( container[0], etaCut, partMinPt, rawParticles[0] );
      ClusterSequence jetCluster( rawParticles[0], jet_def );           //  CLUSTER ALL JETS
      Jets[0] = sorted_by_pt( etaPtSelector( jetCluster.inclusive_jets() ) );     // EXTRACT SELECTED JETS

      //FillJetInfo ( Jets[0], vjTree[0][g], ID, wtVal, jetPt[0][g], jetEta[0][g], jetPhi[0][g], jetE[0][g], jetM[0][g], Rg[0][g], Zg[0][g], wt[0][g], EventID[0][g], nCons[0][g] );

      std::cout << "got here once" << std::endl;
      if ( g == 1 ) {    // Groom Jets
      	std::cout << "g == 1" << std::endl;
      	contrib::SoftDrop sd( beta, z_cut, R );
      	std::cout << "about to drop" << std::endl;
      	groomedJets[0] = sd( Jets[0] );
      	std::cout << "about to fill :)" << std::endl;
      	FillJetInfo ( groomedJets[0], vjTree[0][g], ID, wtVal, jetPt[0][g], jetEta[0][g], jetPhi[0][g], jetE[0][g], jetM[0][g], Rg[0][g], Zg[0][g], wt[0][g], EventID[0][g], nCons[0][g] );
      	std::cout <<"complete" << std::endl;
      }
      // else {
      // 	std::cout << "g == 0" << std::endl;
      // 	FillJetInfo ( Jets[0], vjTree[0][g], ID, wtVal, jetPt[0][g], jetEta[0][g], jetPhi[0][g], jetE[0][g], jetM[0][g], Rg[0][g], Zg[0][g], wt[0][g], EventID[0][g], nCons[0][g] );
      // 	std::cout << "g == 0 complete" << std::endl;
      // }

    }
    
  }
  // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  END ppHT LOOP!  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~



  // // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  MC EVENT LOOP!  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  // while ( Reader[0].NextEvent() ) {
  //   for ( int g=0; g<g_; ++g ) {
  //     Reader[0].PrintStatus(12); 
  //     rawParticles[0].clear();  Jets[0].clear();  groomedJets[0].clear();     //  clear containers

  //     event[0] = Reader[0].GetEvent();    header[0] = event[0]->GetHeader();
  //     container[0] = Reader[0].GetOutputContainer();
  //     if ( Vz_candidate( header[0], absMaxVz ) == false ) { continue; }
    
  //     fileName =  Reader[0].GetInputChain()->GetCurrentFile()->GetName();
  //     ID = Reader[0].GetNOfCurrentEvent();
  //     wtVal = LookupXsec( fileName );
    
  //     GatherParticles ( container[0], etaCut, partMinPt, rawParticles[0] );
  //     ClusterSequence jetCluster( rawParticles[0], jet_def );           //  CLUSTER ALL JETS
  //     vector<PseudoJet> Jets[0] = sorted_by_pt( etaPtSelector( jetCluster.inclusive_jets() ) );     // EXTRACT SELECTED JETS

  //     if ( g == 1 ) {    // Groom Jets
  // 	contrib::SoftDrop sd( beta, z_cut, R );
  // 	groomedJets[0].push_back( sd( Jets[0] ) );
  // 	FillJetInfo ( groomedJets[0], vjTree[0][g], ID, wtVal, jetPt[0][g], jetEta[0][g], jetPhi[0][g], jetE[0][g], jetM[0][g], Rg[0][g], Zg[0][g], wt[0][g], EventID[0][g], nCons[0][g] );
  //     }
  //     else { FillJetInfo ( Jets[0], vjTree[0][g], ID, wtVal, jetPt[0][g], jetEta[0][g], jetPhi[0][g], jetE[0][g], jetM[0][g], Rg[0][g], Zg[0][g], wt[0][g], EventID[0][g], nCons[0][g] ); }

  //     vjTree[0][g]->Fill();
  //   }
  // }
  // // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  END MC LOOP!  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

  
  for ( int d=0; d<d_; ++d ) {
    for ( int g=0; g<g_; ++g ) {
      vjTree[d][g]->Write();
    }
  }
    
  outFile->Close();
  
  return 0;
}

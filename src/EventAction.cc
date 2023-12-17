#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "G4AnalysisManager.hh"

#include "G4UnitsTable.hh"



namespace B1
{


EventAction::EventAction(RunAction* runAction): fRunAction(runAction){}


void EventAction::BeginOfEventAction(const G4Event*){
  	fEdep = 0.;
}


void EventAction::EndOfEventAction(const G4Event*){
  	// fill fEdep
  	//fRunAction->AddEdep(fEdep);
  	// get analysis manager
  	auto analysisManager = G4AnalysisManager::Instance();

  	// fill histograms
  	analysisManager->FillH1(0, fEdep);
  	analysisManager->FillH1(1, fEdep);
  	//analysisManager->FillH1(2, absoHit->GetTrackLength());
  	//analysisManager->FillH1(3, gapHit->GetTrackLength());

  	// fill ntuple
  	analysisManager->FillNtupleDColumn(0, fEdep);
  	analysisManager->FillNtupleDColumn(1, fEdep);
  	//analysisManager->FillNtupleDColumn(2, absoHit->GetTrackLength());
  	//analysisManager->FillNtupleDColumn(3, gapHit->GetTrackLength());
  	analysisManager->AddNtupleRow();
}


}

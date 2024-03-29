#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
// #include "Run.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4AnalysisManager.hh"




namespace B1
{


RunAction::RunAction(){
  	const G4double milligray = 1.e-3*gray;
  	const G4double microgray = 1.e-6*gray;
  	const G4double nanogray  = 1.e-9*gray;
  	const G4double picogray  = 1.e-12*gray;
  	new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
  	new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
  	new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
  	new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray);

  	// Register accumulable to the accumulable manager
  	G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  	accumulableManager->RegisterAccumulable(fEdep);
  	accumulableManager->RegisterAccumulable(fEdep2);

  	// inform the runManager to save random number seed
  	G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  	auto analysisManager = G4AnalysisManager::Instance();

  	analysisManager->SetHistoDirectoryName("histograms");
  	analysisManager->SetNtupleDirectoryName("ntuple");
  	analysisManager->SetVerboseLevel(1);
  	analysisManager->SetNtupleMerging(true);
    	// Note: merging ntuples is available only with Root output

  	analysisManager->CreateH1("Eabs","Edep in absorber", 100, 0., 5*MeV);
  	analysisManager->CreateH1("Egap","Edep in gap", 100, 0., 5*MeV);
}


void RunAction::BeginOfRunAction(const G4Run*){
  	auto analysisManager = G4AnalysisManager::Instance();

  	//G4String fileName = "B4.root";
  	G4String fileName = "B4.csv";

  	analysisManager->OpenFile(fileName);
  	G4cout << "Using " << analysisManager->GetType() << G4endl;
}


void RunAction::EndOfRunAction(const G4Run* run){
  	G4int nofEvents = run->GetNumberOfEvent();
  	if (nofEvents == 0) return;

  	// Merge accumulables
  	G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  	accumulableManager->Merge();

  	// Compute dose = total energy deposit in a run and its variance
  	G4double edep  = fEdep.GetValue();
  	G4double edep2 = fEdep2.GetValue();

  	G4double rms = edep2 - edep*edep/nofEvents;
  	if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;

  	const auto detConstruction = static_cast<const DetectorConstruction*>(
    	G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  	G4double mass = detConstruction->GetScoringVolume()->GetMass();
  	G4double dose = edep/mass;
  	G4double rmsDose = rms/mass;

  	// Run conditions
 	//  note: There is no primary generator action object for "master"
  	//        run manager for multi-threaded mode.
  	const auto generatorAction = static_cast<const PrimaryGeneratorAction*>(
    	G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  	G4String runCondition;
  	if (generatorAction){
	    	const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
	    	runCondition += particleGun->GetParticleDefinition()->GetParticleName();
	    	runCondition += " of ";
	    	G4double particleEnergy = particleGun->GetParticleEnergy();
	    	runCondition += G4BestUnit(particleEnergy,"Energy");
  	}


  	if (IsMaster()) {
	    	G4cout
	     		<< G4endl
	     		<< "--------------------End of Global Run-----------------------";
  	}
  	else {
	   	G4cout
	     		<< G4endl
	     		<< "--------------------End of Local Run------------------------";
  	}



  	G4cout
	     	<< G4endl
	     	<< " The run consists of " << nofEvents << " "<< runCondition
	     	<< G4endl
	     	<< " Cumulated dose per run, in scoring volume : "
	     	<< G4BestUnit(dose,"Dose") << " rms = " << G4BestUnit(rmsDose,"Dose")
	     	<< G4endl
	     	<< "------------------------------------------------------------"
	     	<< G4endl
	     	<< G4endl;
  	auto analysisManager = G4AnalysisManager::Instance();

  	analysisManager->Write();
  	analysisManager->CloseFile();
}




void RunAction::AddEdep(G4double edep){
  	fEdep  += edep;
  	fEdep2 += edep*edep;
}


}

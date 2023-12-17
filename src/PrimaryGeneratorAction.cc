#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include <cmath>
#include "G4RandomDirection.hh"

namespace B1
{


PrimaryGeneratorAction::PrimaryGeneratorAction(){
  	G4int n_particle = 1;
  	fParticleGun  = new G4ParticleGun(n_particle);

  	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  	G4String particleName;
  	G4ParticleDefinition* particle
    		= particleTable->FindParticle(particleName="gamma");
  	fParticleGun->SetParticleDefinition(particle);
  	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,-1.));
  	fParticleGun->SetParticleEnergy(2.62*MeV);//Tl-206
}


PrimaryGeneratorAction::~PrimaryGeneratorAction(){
  	delete fParticleGun;
}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
  	//this function is called at the begining of ecah event
  	//

  	// In order to avoid dependence of PrimaryGeneratorAction
  	// on DetectorConstruction class we get Envelope volume
  	// from G4LogicalVolumeStore.

  	G4double envSizeXY = 0;
  	G4double envSizeZ = 0;

  	if (!fEnvelopeBox){
    		G4LogicalVolume* envLV
      			= G4LogicalVolumeStore::GetInstance()->GetVolume("Envelope");
    	if ( envLV ) fEnvelopeBox = dynamic_cast<G4Box*>(envLV->GetSolid());
  	}

  	if ( fEnvelopeBox ) {
    		envSizeXY = fEnvelopeBox->GetXHalfLength()*2.;
    		envSizeZ = fEnvelopeBox->GetZHalfLength()*2.;
  	}
  	else  {
    		G4ExceptionDescription msg;
    		msg << "Envelope volume of box shape not found.\n";
    		msg << "Perhaps you have changed geometry.\n";
    		msg << "The gun will be place at the center.";
    		G4Exception("PrimaryGeneratorAction::GeneratePrimaries()",
     			"MyCode0002",JustWarning,msg);
  	}

    	G4ParticleMomentum    fDirection;
    	fDirection = G4RandomDirection();

  	G4double x0 = 0;
  	G4double y0 = 0;
 	G4double z0 = 2.5*mm;

  	fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  	fParticleGun->SetParticleMomentumDirection(fDirection);
  	fParticleGun->GeneratePrimaryVertex(anEvent);
}


}

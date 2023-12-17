#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

namespace B1{


G4VPhysicalVolume* DetectorConstruction::Construct(){
	G4NistManager* nist = G4NistManager::Instance();

	G4double env_sizeXY = 100*cm, env_sizeZ = 100*cm;
 	G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");

  	G4double world_sizeXY = 1.2*env_sizeXY;
  	G4double world_sizeZ  = 1.2*env_sizeZ;
  	G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  	G4Box* solidWorld = new G4Box("World",                               // its name
    		0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);  // its size
  	G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,        // its solid
    		world_mat,                                                   // its material
    		"World");                                                    // its name
  	G4PVPlacement* physWorld = new G4PVPlacement(nullptr,  // no rotation
    		G4ThreeVector(),                               // at (0,0,0)
    		logicWorld,                                    // its logical volume
    		"World",                                       // its name
    		nullptr,                                       // its mother  volume
    		false,                                         // no boolean operation
    		0,                                             // copy number
    		true);                                         // overlaps checking

	// Envelope
	G4Box* solidEnv = new G4Box("Envelope",                        // its name
    		0.5 * env_sizeXY, 0.5 * env_sizeXY, 0.5 * env_sizeZ);  // its size
	G4LogicalVolume* logicEnv = new G4LogicalVolume(solidEnv,  // its solid
		env_mat,                                           // its material
		"Envelope");                                       // its name

  	new G4PVPlacement(nullptr,  // no rotation
   		G4ThreeVector(),    // at (0,0,0)
    		logicEnv,           // its logical volume
    		"Envelope",         // its name
    		logicWorld,         // its mother  volume
    		false,              // no boolean operation
    		0,      	    // copy number
    		true);              // overlaps checking

	// shape1 Ge-detector
  	G4Material* shape1_mat = nist->FindOrBuildMaterial("G4_Ge");
  	G4double RadiusGe = 66.3/2*mm;
  	G4double HalfZGe = 51.3/2*mm;

  	G4ThreeVector pos1 = G4ThreeVector(0, 0, -HalfZGe);

  	G4Tubs* solidShape1 =  new G4Tubs(
  		"Hole", 0., RadiusGe, HalfZGe, 0. * deg, 360. * deg);


  	G4LogicalVolume* logicShape1 = new G4LogicalVolume(solidShape1,  // its solid
    		shape1_mat,                                              // its material
    		"Shape1");                                               // its name

  	new G4PVPlacement(nullptr,  // no rotation
	    	pos1,               // at position
	    	logicShape1,        // its logical volume
	    	"Shape1",           // its name
	    	logicEnv,           // its mother  volume
	    	false,              // no boolean operation
	    	0,                  // copy number
	    	true);              // overlaps checking

  	// Al-container
  	G4Material* Al_mat = nist->FindOrBuildMaterial("G4_Al");
  

  	G4double RadiusAl = (66.3+4)/2*mm;
  	G4double HalfZAl = (51.3+4)/2*mm;

	G4ThreeVector posAl = G4ThreeVector(0, 0*cm, -HalfZGe);

  	G4Tubs* solidAl1 =  new G4Tubs(
     		"Hole", RadiusAl, RadiusAl+1*mm, HalfZAl, 0. * deg, 360. * deg);

  	G4LogicalVolume* logicAl1 = new G4LogicalVolume(solidAl1,  // its solid
	    	Al_mat,                                            // its material
	    	"Shape1");                                         // its name

  	new G4PVPlacement(nullptr,  // no rotation
	    	posAl,              // at position //pos1
	    	logicAl1,           // its logical volume
	    	"Al1",              // its name
	    	logicEnv,           // its mother  volume
	    	false,              // no boolean operation
	    	0,                  // copy number
	    	true);              // overlaps checking

    	G4VisAttributes *Al1VisAtt = new G4VisAttributes(G4Colour(1. / 255, 192. / 255., 192. / 255.));
    	Al1VisAtt->SetForceWireframe(true);
    	Al1VisAtt->SetForceSolid(false);
    	logicAl1->SetVisAttributes(Al1VisAtt);

// 	Two Aluminium disks
  	G4Tubs* solidAlTop =  new G4Tubs(
     		"Hole", 0, RadiusAl, 0.5*mm, 0. * deg, 360. * deg);

  	G4LogicalVolume* logicAlTop = new G4LogicalVolume(solidAlTop,  // its solid
	    	Al_mat,                                                // its material
	    	"Shape1");                                             // its name

	G4double zAlTop =(2+0.5)*mm ;

	G4ThreeVector posAlTop = G4ThreeVector(0, 0*cm, zAlTop);

  	new G4PVPlacement(nullptr,  // no rotation
	    	posAlTop,           // at position //pos1
	    	logicAlTop,         // its logical volume
	    	"Al1",              // its name
	    	logicEnv,           // its mother  volume
	    	false,              // no boolean operation
	    	0,                  // copy number
	    	true);              // overlaps checking


  	G4Tubs* solidAlBottom =  new G4Tubs(
     		"Hole", 0, RadiusAl, 0.5*mm, 0. * deg, 360. * deg);

  	G4LogicalVolume* logicAlBottom = new G4LogicalVolume(solidAlBottom,  // its solid
    		Al_mat,                                                      // its material
    		"Shape1");                                                   // its name

	G4double zAlBottom =-2*HalfZGe -2.5*mm;

	G4ThreeVector posAlBottom = G4ThreeVector(0, 0*cm, zAlBottom);

  	new G4PVPlacement(nullptr,  // no rotation
	    	posAlBottom,        // at position //pos1
	    	logicAlBottom,      // its logical volume
	    	"Al1",              // its name
	    	logicEnv,           // its mother  volume
	    	false,              // no boolean operation
	    	0,                  // copy number
	    	true);              // overlaps checking


  	// Shape 2
  	G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_Al");
  	G4ThreeVector pos2 = G4ThreeVector(0, 0*cm, -50*cm);

  	// Trapezoid shape
  	G4double shape2_dxa = 100*cm, shape2_dxb = 100*cm;
  	G4double shape2_dya = 100*cm, shape2_dyb = 100*cm;
  	G4double shape2_dz  = 5*cm;
  	G4Trd* solidShape2 = new G4Trd("Shape2",  // its name
	    	0.5 * shape2_dxa, 0.5 * shape2_dxb, 0.5 * shape2_dya, 0.5 * shape2_dyb,
	    	0.5 * shape2_dz);  // its size

	G4LogicalVolume* logicShape2 = new G4LogicalVolume(solidShape2,  // its solid
	    	shape2_mat,                                              // its material
	    	"Shape2");                                               // its name

  	new G4PVPlacement(nullptr,  // no rotation
	    	pos2,               // at position
	    	logicShape2,        // its logical volume
	    	"Shape2",           // its name
	    	logicEnv,           // its mother  volume
	    	false,              // no boolean operation
	    	0,                  // copy number
	    	true);              // overlaps checking

  	// Set Shape2 as scoring volume
  	fScoringVolume = logicShape1;
  	
 	return physWorld;
}


}

// MYEXAMPLE1 - NEW GEOMETRY
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
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "B2TrackerSD.hh"
#include "G4SDManager.hh"

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <random>

#include <iostream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;


  ////////// WORLD //////////////
  // Dimensions 100x100x200cm made of air
  G4double world_sizeX = 20*cm;
  G4double world_sizeY = 20*cm;
  G4double world_sizeZ = 250*cm;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Box* solidWorld =
    new G4Box("World",                       //its name
       0.5*world_sizeX, 0.5*world_sizeY, 0.5*world_sizeZ);     //its size
  // WORLD LOGICAL
  G4LogicalVolume* logicWorld =
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
  // WORLD PHYSICAL
  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0,0,0),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
  /////////////////////////////////


  ///////////////// Elements //////////////////////
  // For the CZT detector
  G4Element* elCd =
    new G4Element("Cadmium","Cd",48.0,112.4*g/mole);
  G4Element* elZn =
    new G4Element("Zinc","Zn",30.0,65.4*g/mole);
  G4Element* elTe =
    new G4Element("Tellurium","Te",52.0,127.6*g/mole);
  ////////////////////////////////////////////////////


  ///////////// DEFINE THE MATERIALS /////////////////
  // CZT materal  
  G4double CZT_density = 5.8*g/cm3;
  G4Material* matCZT =
    new G4Material("CZT",CZT_density,3);
  matCZT->AddElement(elCd, 43.0*perCent);
  matCZT->AddElement(elZn, 2.77*perCent);
  matCZT->AddElement(elTe, 54.23*perCent);
  ////////////////////////////////////////////////////


  //////// HEXITEC DEFINITION AND PLACEMENT //////////

  // HEXITEC solid volume
  G4double HEX_side_length = 20*mm;
  G4double HEX_thickness = 2*mm;
  G4Box* HEXITEC_solid = 
    new G4Box("HEXITEC",
        0.5*HEX_side_length, 0.5*HEX_side_length, 0.5*HEX_thickness);
  
  // HEXITEC logical volume
  G4LogicalVolume* HEXITEC = 
    new G4LogicalVolume(HEXITEC_solid,         //its solid
                        matCZT,          //its material==
                        "HEXITEC");           //its name

  // Place HEXITEC (always at same position - (0,0,z=+1mm)
  // This places the very front edge of HEXITEC at z=0
  new G4PVPlacement(0,
                G4ThreeVector(0*mm,0*mm,1*mm),
                HEXITEC,
                "HEXITEC",
                logicWorld,
                false,
                0,
                false);
  //////////////////////////////////////////////////

  // Set HEXITEC as a scoring volume
  fScoringVolume = HEXITEC;

  //always return the physical World
  return physWorld;
}

void B1DetectorConstruction::ConstructSDandField()
{
  //Signposting generated detectors as sensitive detectors
  //SD Manager allows hit collection of each detector to be obtained individually
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  B2TrackerSD* aTrackerSD = new B2TrackerSD("tracker1","hitscollection1");
  SetSensitiveDetector("HEXITEC", aTrackerSD, true);
  SDman->AddNewDetector(aTrackerSD);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

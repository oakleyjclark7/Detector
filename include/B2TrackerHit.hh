//
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
// $Id: B2TrackerHit.hh 69706 2013-05-13 09:12:40Z gcosmo $
//
/// \file B2TrackerHit.hh
/// \brief Definition of the B2TrackerHit class

#ifndef B2TrackerHit_h
#define B2TrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"
#include "G4ParticleDefinition.hh"

/// Tracker hit class
///
/// It defines data members to store the trackID, chamberNb, energy deposit,
/// and position of charged particles in a selected volume:
/// - fTrackID, fChamberNB, fEdep, fPos

class B2TrackerHit : public G4VHit
{
  public:
    B2TrackerHit();
    B2TrackerHit(const B2TrackerHit&);
    virtual ~B2TrackerHit();

    // operators
    const B2TrackerHit& operator=(const B2TrackerHit&);
    G4int operator==(const B2TrackerHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual void Print();

    // Set methods
    void SetTrackID  (G4int track)      { fTrackID = track; };
    void SetChamberNb(G4int chamb)      { fChamberNb = chamb; };
    void SetEdep     (G4double de)      { fEdep = de; };
    void SetPos      (G4ThreeVector xyz){ fPos = xyz; };
    void SetTime       (G4double time)    { fGlobalTime = time;};
    void SetEventID  (G4int event)     {fEventID = event;};
    void SetType     (G4String type)    {fType = type;};
    void SetParID    (G4int parID)      {fParID = parID;};
    void SetProcName (G4String procName) {fProcName = procName;};
    //void SetPDGEncoding (G4int PDGEncoding) {fPDGEncoding = PDGEncoding;};

    // Get methods
    G4int GetTrackID() const     { return fTrackID; };
    G4int GetChamberNb() const   { return fChamberNb; };
    G4double GetEdep() const     { return fEdep; };
    G4ThreeVector GetPos() const { return fPos; };
    G4double GetTime() const {return fGlobalTime; };
    G4double GetEventID() const {return fEventID;  };
    G4String GetType() const {return fType; };
    G4int GetParID()  const {return fParID; };
    G4String GetProcName() const {return fProcName; };
    G4int GetPDGEncoding()  const {return fPDGEncoding; };
  private:

      G4int         fTrackID;
      G4int         fChamberNb;
      G4double      fEdep;
      G4ThreeVector fPos;
      G4double      fGlobalTime;
      G4int fEventID;
      G4String fType;
      G4int fParID;
      G4String fProcName;
      G4int fPDGEncoding;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<B2TrackerHit> B2TrackerHitsCollection;

extern G4ThreadLocal G4Allocator<B2TrackerHit>* B2TrackerHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* B2TrackerHit::operator new(size_t)
{
  if(!B2TrackerHitAllocator)
      B2TrackerHitAllocator = new G4Allocator<B2TrackerHit>;
  return (void *) B2TrackerHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void B2TrackerHit::operator delete(void *hit)
{
  B2TrackerHitAllocator->FreeSingle((B2TrackerHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

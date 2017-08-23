#include <TString.h>
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "G_Jet.h"

using namespace std;

Int_t          nPho_;
vector<float>  phoE_;
vector<float>  phoEt_;
vector<float>  phoEta_;
vector<float> phoPhi_;

void G_Jet::branchesPhotons(TTree* tree) {
  
  tree->Branch("nPho",   &nPho_);
  tree->Branch("phoE",   &phoE_);
  tree->Branch("phoEt",  &phoEt_);
  tree->Branch("phoEta", &phoEta_);
  tree->Branch("phoPhi", &phoPhi_);
}


void G_Jet::fillPhotons(const edm::Event& e, const edm::EventSetup& es) {
  phoE_.clear();
  phoEt_.clear();
  phoEta_.clear();
  phoPhi_.clear();
  nPho_ = 0;
  edm::Handle<edm::View<pat::Photon> > photonHandle;
  e.getByToken(photonSrcToken_, photonHandle);
  for (edm::View<pat::Photon>::const_iterator iPho = photonHandle->begin(); iPho != photonHandle->end(); ++iPho) {
    if (iPho->et()<50) continue;
    phoE_             .push_back(iPho->energy());
    phoEt_            .push_back(iPho->et());
    phoEta_           .push_back(iPho->eta());
    phoPhi_           .push_back(iPho->phi());
    nPho_++; 
  }
}
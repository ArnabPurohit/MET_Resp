#include <TString.h>
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "G_Jet.h"
#include "DataFormats/Common/interface/ValueMap.h"
using namespace std;

Int_t          nPho_;
vector<float>  phoE_;
vector<float>  phoEt_;
vector<float>  phoEta_;
vector<float> phoPhi_;
vector<UShort_t> phoIDbit_;

void G_Jet::branchesPhotons(TTree* tree) {
  
  tree->Branch("nPho",   &nPho_);
  tree->Branch("phoE",   &phoE_);
  tree->Branch("phoEt",  &phoEt_);
  tree->Branch("phoEta", &phoEta_);
  tree->Branch("phoPhi", &phoPhi_);
  tree->Branch("phoIDbit", &phoIDbit_);
}


void G_Jet::fillPhotons(const edm::Event& e, const edm::EventSetup& es) {
  phoE_.clear();
  phoEt_.clear();
  phoEta_.clear();
  phoPhi_.clear();
  phoIDbit_.clear();
  nPho_ = 0;
  edm::Handle<edm::View<pat::Photon> > photonHandle;
  e.getByToken(photonSrcToken_, photonHandle);
  edm::Handle<edm::ValueMap<bool> >  loose_id_decisions;
  edm::Handle<edm::ValueMap<bool> >  medium_id_decisions;
  edm::Handle<edm::ValueMap<bool> > tight_id_decisions;

  e.getByToken(phoLooseIdMapToken_ ,  loose_id_decisions);
  e.getByToken(phoMediumIdMapToken_,  medium_id_decisions);
  e.getByToken(phoTightIdMapToken_ , tight_id_decisions);


  for (edm::View<pat::Photon>::const_iterator iPho = photonHandle->begin(); iPho != photonHandle->end(); ++iPho) {
    if (iPho->et()<50) continue;
    const auto pho = photonHandle->ptrAt(nPho_);
    UShort_t tmpphoIDbit = 0;
    phoE_             .push_back(iPho->energy());
    phoEt_            .push_back(iPho->et());
    phoEta_           .push_back(iPho->eta());
    phoPhi_           .push_back(iPho->phi());
    bool isPassLoose  = (*loose_id_decisions)[pho];
    if(isPassLoose) setbit(tmpphoIDbit, 0);
    
    bool isPassMedium = (*medium_id_decisions)[pho];
    if(isPassMedium) setbit(tmpphoIDbit, 1);
    
    bool isPassTight  = (*tight_id_decisions)[pho];
    if(isPassTight) setbit(tmpphoIDbit, 2);
    phoIDbit_.push_back(tmpphoIDbit); 

    nPho_++; 
  }
}

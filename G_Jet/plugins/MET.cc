#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "G_Jet.h"

using namespace std;
float pfMET_;
float pfMETPhi_;

void G_Jet::branchesMET(TTree* tree) {
  tree->Branch("pfMET", &pfMET_);
  tree->Branch("pfMETPhi", &pfMETPhi_);
}

void G_Jet::fillMET(const edm::Event& e, const edm::EventSetup& es) {
  edm::Handle<edm::View<pat::MET> > pfMETHandle;
  e.getByToken(metSrcToken_, pfMETHandle);
  pfMET_       = -99;
  pfMETPhi_ = -99;
  if (pfMETHandle.isValid()) {
    const pat::MET *pfMET = 0;
    pfMET = &(pfMETHandle->front());

    pfMET_       = pfMET->et();
    pfMETPhi_ = pfMET->phi();
  }
}

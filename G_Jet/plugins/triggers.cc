#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
//#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "G_Jet.h"

using namespace std;

// (local) variables associated with tree branches
Int_t       run_;
Long64_t    event_;
Int_t       lumis_;
Bool_t      isData_;
Int_t       nVtx_;
Int_t       nGoodVtx_;
Int_t       nTrksPV_;
Bool_t      isPVGood_;
float       vtx_;
float       vty_;
float       vtz_;
float       rho_;
float       rhoCentral_;
//ULong64_t   HLTPho_;
//ULong64_t   HLTJet_;
//ULong64_t   HLTPhoIsPrescaled_;
//ULong64_t   HLTJetIsPrescaled_;
//vector<int> phoPrescale_;
Bool_t HLT_Photon30_R9Id90_HE10_IsoM_v_;
Bool_t HLT_Photon50_R9Id90_HE10_IsoM_v_;
Bool_t HLT_Photon75_R9Id90_HE10_IsoM_v_;
Bool_t HLT_Photon90_R9Id90_HE10_IsoM_v_;
Bool_t HLT_Photon120_R9Id90_HE10_IsoM_v_;
Bool_t HLT_Photon165_HE10_v_;
int prescale_HLT_Photon30_R9Id90_HE10_IsoM_v_;
int prescale_HLT_Photon50_R9Id90_HE10_IsoM_v_;
int prescale_HLT_Photon75_R9Id90_HE10_IsoM_v_;
int prescale_HLT_Photon90_R9Id90_HE10_IsoM_v_;
int prescale_HLT_Photon120_R9Id90_HE10_IsoM_v_;
int prescale_HLT_Photon165_HE10_v_;


void G_Jet::branchesGlobalEvent(TTree* tree) {

  tree->Branch("run",     &run_);
  tree->Branch("event",   &event_);
  tree->Branch("lumis",   &lumis_);
  tree->Branch("isData",  &isData_);
  tree->Branch("nVtx",    &nVtx_);
  tree->Branch("nGoodVtx",&nGoodVtx_);
  tree->Branch("nTrksPV", &nTrksPV_);
  tree->Branch("isPVGood",&isPVGood_);
  tree->Branch("vtx",     &vtx_); 
  tree->Branch("vty",     &vty_); 
  tree->Branch("vtz",     &vtz_); 
  tree->Branch("rho",     &rho_);
  tree->Branch("rhoCentral", &rhoCentral_);
  tree->Branch("HLT_Photon30_R9Id90_HE10_IsoM_v", &HLT_Photon30_R9Id90_HE10_IsoM_v_);
  tree->Branch("HLT_Photon50_R9Id90_HE10_IsoM_v", &HLT_Photon50_R9Id90_HE10_IsoM_v_ );
  tree->Branch("HLT_Photon75_R9Id90_HE10_IsoM_v", &HLT_Photon75_R9Id90_HE10_IsoM_v_ );
  tree->Branch("HLT_Photon90_R9Id90_HE10_IsoM_v", &HLT_Photon90_R9Id90_HE10_IsoM_v_ );
  tree->Branch("HLT_Photon120_R9Id90_HE10_IsoM_v", &HLT_Photon120_R9Id90_HE10_IsoM_v_ );
  tree->Branch("HLT_Photon165_HE10_v", &HLT_Photon165_HE10_v_);
  tree->Branch("prescale_HLT_Photon30_R9Id90_HE10_IsoM_v", &prescale_HLT_Photon30_R9Id90_HE10_IsoM_v_);
  tree->Branch("prescale_HLT_Photon50_R9Id90_HE10_IsoM_v", &prescale_HLT_Photon50_R9Id90_HE10_IsoM_v_ );
  tree->Branch("prescale_HLT_Photon75_R9Id90_HE10_IsoM_v", &prescale_HLT_Photon75_R9Id90_HE10_IsoM_v_ );
  tree->Branch("prescale_HLT_Photon90_R9Id90_HE10_IsoM_v", &prescale_HLT_Photon90_R9Id90_HE10_IsoM_v_ );
  tree->Branch("prescale_HLT_Photon120_R9Id90_HE10_IsoM_v", &prescale_HLT_Photon120_R9Id90_HE10_IsoM_v_ );
  tree->Branch("prescale_HLT_Photon165_HE10_v", &prescale_HLT_Photon165_HE10_v_);
  //  tree->Branch("phoPrescale", &phoPrescale_);
std::cout<<"Trigger Branches are made"<<std::endl;
}

void G_Jet::fillGlobalEvent(const edm::Event& e, const edm::EventSetup& es) {

  //  if (!doGenParticles_) {
  //phoPrescale_.clear();
  //phoPrescale_.reserve(9);
    //  }

  edm::Handle<double> rhoHandle;
  e.getByToken(rhoLabel_, rhoHandle);

  edm::Handle<double> rhoCentralHandle;
  e.getByToken(rhoCentralLabel_, rhoCentralHandle);

  run_    = e.id().run();
  event_  = e.id().event();
  lumis_  = e.luminosityBlock();
  isData_ = e.isRealData();
  rho_    = *(rhoHandle.product());
  if (rhoCentralHandle.isValid()) rhoCentral_ = *(rhoCentralHandle.product());
  else rhoCentral_ = -99.;

  edm::Handle<reco::VertexCollection> vtxHandle;
  e.getByToken(vtxLabel_, vtxHandle);
  
  nVtx_     = -1;
  nGoodVtx_ = -1;
  if (vtxHandle.isValid()) {
    nVtx_     = 0;
    nGoodVtx_ = 0;    

    for (vector<reco::Vertex>::const_iterator v = vtxHandle->begin(); v != vtxHandle->end(); ++v) {

      if (nVtx_ == 0) {
	nTrksPV_ = v->nTracks();
	vtx_     = v->x();
	vty_     = v->y();
	vtz_     = v->z();

	isPVGood_ = false;
	if (!v->isFake() && v->ndof() > 4. && fabs(v->z()) <= 24. && fabs(v->position().rho()) <= 2.) isPVGood_ = true;
      }

      if (!v->isFake() && v->ndof() > 4. && fabs(v->z()) <= 24. && fabs(v->position().rho()) <= 2.) nGoodVtx_++;
      nVtx_++;

    }
  }

  HLT_Photon30_R9Id90_HE10_IsoM_v_ = false;
  HLT_Photon50_R9Id90_HE10_IsoM_v_ = false;
  HLT_Photon75_R9Id90_HE10_IsoM_v_ = false;
  HLT_Photon90_R9Id90_HE10_IsoM_v_ = false;
  HLT_Photon120_R9Id90_HE10_IsoM_v_ = false;
  HLT_Photon165_HE10_v_ = false;
  prescale_HLT_Photon30_R9Id90_HE10_IsoM_v_ = 1;
  prescale_HLT_Photon50_R9Id90_HE10_IsoM_v_ = 1;
  prescale_HLT_Photon75_R9Id90_HE10_IsoM_v_ = 1;
  prescale_HLT_Photon90_R9Id90_HE10_IsoM_v_ = 1;
  prescale_HLT_Photon120_R9Id90_HE10_IsoM_v_ = 1;
  prescale_HLT_Photon165_HE10_v_ = 1;
  std::cout<<"Trigger bools initialized"<<std::endl;


  //bool cfg_changed = true;
  HLTConfigProvider const& hltCfg = hltPrescaleProvider_->hltConfigProvider();
  std::cout<<"Trigger ConfigProvider stored"<<std::endl;
  //hltPrescaleProvider_->init(e.getRun(), es, trgResultsProcess_, cfg_changed);
  //int pset = hltCfg.prescaleSet(e,es);
  std::cout<<"Trigger ConfigProvider changed"<<std::endl;


  edm::Handle<edm::TriggerResults> trgResultsHandle;
  e.getByToken(trgResultsLabel_, trgResultsHandle);
  //  HLTConfigProvider hltCfg;
  std::cout<<"Trigger handle grabbed"<<std::endl;
  ////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  const edm::TriggerNames &trgNames = e.triggerNames(*trgResultsHandle);
  for (size_t i = 0; i < trgNames.size(); ++i) {
    const string &name = trgNames.triggerName(i);
    std::cout<<"Trigger module loop"<<std::endl;
    if (name.find("HLT_Photon30_R9Id90_HE10_IsoM_v")!= string::npos && trgResultsHandle->accept(i)){
      HLT_Photon30_R9Id90_HE10_IsoM_v_= true;
      //      prescale_HLT_Photon30_R9Id90_HE10_IsoM_v_= hltCfg.prescaleValue(e, es, name);
    std::cout<<"Trigger module 1st loop"<<std::endl;
      prescale_HLT_Photon30_R9Id90_HE10_IsoM_v_( hltPrescaleProvider_->prescaleValue(e, es, name));
    std::cout<<"Trigger module 1st loop end "<<std::endl;   
    }
    else if (name.find("HLT_Photon50_R9Id90_HE10_IsoM_v")!= string::npos  && trgResultsHandle->accept(i)){
      HLT_Photon50_R9Id90_HE10_IsoM_v_= true;
      prescale_HLT_Photon50_R9Id90_HE10_IsoM_v_= hltPrescaleProvider_->prescaleValue(e, es, name);
    }
    else if (name.find("HLT_Photon75_R9Id90_HE10_IsoM_v")!= string::npos && trgResultsHandle->accept(i)){
      HLT_Photon75_R9Id90_HE10_IsoM_v_= true;
      prescale_HLT_Photon75_R9Id90_HE10_IsoM_v_=  hltPrescaleProvider_->prescaleValue(e, es, name);
    }
    else if (name.find("HLT_Photon90_R9Id90_HE10_IsoM_v")!= string::npos && trgResultsHandle->accept(i)){
      HLT_Photon90_R9Id90_HE10_IsoM_v_= true;
      prescale_HLT_Photon90_R9Id90_HE10_IsoM_v_=  hltPrescaleProvider_->prescaleValue(e, es, name);
    }
    else if (name.find("HLT_Photon120_R9Id90_HE10_IsoM_v")!= string::npos && trgResultsHandle->accept(i)){
      HLT_Photon120_R9Id90_HE10_IsoM_v_= true;
      prescale_HLT_Photon120_R9Id90_HE10_IsoM_v_= hltPrescaleProvider_->prescaleValue(e, es, name);
    }
    else if (name.find("HLT_Photon165_HE10_v")!= string::npos && trgResultsHandle->accept(i)){
      HLT_Photon165_HE10_v_= true;
      prescale_HLT_Photon165_HE10_v_= hltPrescaleProvider_->prescaleValue(e, es, name);
    }

  }
}

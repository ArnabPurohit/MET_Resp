#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "G_Jet.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Random/RandGauss.h"

using namespace std;
typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;

// ak4 jets
Int_t nJet_;
/*vector<float>  jetPt_;
vector<float>  jetEn_;
vector<float>  jetEta_;
vector<float> jetPhi_;
*/
float  jet1Pt_;                                                                                                              
float  jet1En_;                                                                                                               
float  jet1Eta_;                                                                                                              
float  jet1Phi_;       
float  jet2Pt_;                                                                                                              
float  jet2En_;                                                                                                               
float  jet2Eta_;                                                                                                              
float  jet2Phi_;       


void G_Jet::branchesJets(TTree* tree) {
  tree->Branch("nJet", &nJet_);
  tree->Branch("jet1Pt", &jet1Pt_);
  tree->Branch("jet1En", &jet1En_);
  tree->Branch("jet1Eta",&jet1Eta_);
  tree->Branch("jet1Phi", &jet1Phi_);
  tree->Branch("jet2Pt", &jet2Pt_);
  tree->Branch("jet2En", &jet2En_);
  tree->Branch("jet2Eta",&jet2Eta_);
  tree->Branch("jet2Phi", &jet2Phi_);
}
void G_Jet::fillJets(const edm::Event& e, const edm::EventSetup& es) {
  nJet_ = 0;
  /*  jetPt_                                  .clear();
  jetEn_                                  .clear();
  jetEta_                                 .clear();
  jetPhi_ .clear();
  */
  jet1Pt_ =0.0;
  jet1En_=0.0;
  jet1Eta_ =0.0;
  jet1Phi_ =0.0;
  jet2Pt_ =0.0;
  jet2En_=0.0;
  jet2Eta_ =0.0;
  jet2Phi_ =0.0;
  edm::Handle<edm::View<pat::Jet> > jetHandle;
  e.getByToken(jetSrcToken_, jetHandle);
  int jet_cntr=0;
  for (edm::View<pat::Jet>::const_iterator iJet = jetHandle->begin(); iJet != jetHandle->end(); ++iJet) {
    if (iJet->pt() < 30) continue;
    /*    jetPt_.push_back(    iJet->pt());
    jetEn_.push_back(    iJet->energy());
    jetEta_.push_back(   iJet->eta());
    jetPhi_.push_back( iJet->phi());
    */
    ++jet_cntr;
    if(jet_cntr==1){
      jet1Pt_  = iJet->pt();
      jet1En_ = iJet->energy();
      jet1Eta_ = iJet->eta();
      jet1Phi_ = iJet->phi();
    }
    else if(jet_cntr==2) {
      jet2Pt_  = iJet->pt();
      jet2En_ = iJet->energy();
      jet2Eta_ = iJet->eta();
      jet2Phi_ = iJet->phi();    
    }
    nJet_++;
  }
  
}

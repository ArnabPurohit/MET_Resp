#ifndef G_Jet_h
#define G_Jet_h

// system include files
#include <memory>
#include "TH1.h"//New
#include <string>
#include "TTree.h"
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"
#include "DataFormats/Common/interface/ValueMap.h"
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.
using namespace std;
using namespace edm;

void setbit(UShort_t& x, UShort_t bit);
class G_Jet : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit G_Jet(const edm::ParameterSet&);
      ~G_Jet();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
  std::map<std::string,TH1F*> histContainer_;
  TH1F* jetTowers_;
  /*  edm::EDGetTokenT<reco::VertexCollection> vtxToken_;             //Working
  edm::EDGetTokenT<pat::PhotonCollection> photonToken_;
  edm::EDGetTokenT<pat::JetCollection> jetToken_;
  edm::EDGetTokenT<pat::METCollection> metToken_;
  */
  edm::EDGetTokenT<edm::View<pat::Photon> > photonSrcToken_;
  edm::EDGetTokenT<edm::View<pat::Jet> > jetSrcToken_;
  edm::EDGetTokenT<edm::View<pat::MET> > metSrcToken_;
  edm::EDGetTokenT<reco::VertexCollection> vtxLabel_;
  edm::EDGetTokenT<double>                         rhoLabel_;
  edm::EDGetTokenT<double> rhoCentralLabel_;

  edm::EDGetTokenT<edm::ValueMap<bool> >  phoLooseIdMapToken_;
  edm::EDGetTokenT<edm::ValueMap<bool> >  phoMediumIdMapToken_;
  edm::EDGetTokenT<edm::ValueMap<bool> > phoTightIdMapToken_;

  //  edm::EDGetTokenT<edm::TriggerResults> trgResultsLabel_;
  string trgResultsProcess_;
  std::unique_ptr<HLTPrescaleProvider> hltPrescaleProvider_;
  edm::EDGetTokenT<edm::TriggerResults> triggerBits_;
  edm::EDGetTokenT<edm::View<pat::TriggerObjectStandAlone>> triggerObjects_;
  edm::EDGetTokenT<pat::PackedTriggerPrescales> triggerPrescales_;
  TTree *tree_;
  void branchesJets (TTree*);
  void branchesMET (TTree*);
  void branchesPhotons (TTree*);
  void branchesGlobalEvent (TTree*);


  void fillJets (const edm::Event&, const edm::EventSetup&);
  void fillMET        (const edm::Event&, const edm::EventSetup&);
  void fillPhotons (const edm::Event&, const edm::EventSetup&);
  void fillGlobalEvent (const edm::Event&, const edm::EventSetup&);
};

#endif

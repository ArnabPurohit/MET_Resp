// -*- C++ -*-
//
// Package:    MET_Resp/G_Jet
// Class:      G_Jet
// 
/**\class G_Jet G_Jet.cc MET_Resp/G_Jet/plugins/G_Jet.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Arnab Purohit
//         Created:  Mon, 14 Aug 2017 01:05:39 GMT
//
//

#include "G_Jet.h"

//
// constants, enums and typedefs
//

//
// static data member definitions
//
using namespace std;
using namespace edm;
//
// constructors and destructor
//
void setbit(UShort_t& x, UShort_t bit) {
  UShort_t a = 1;
  x |= (a << bit);
}

G_Jet::G_Jet(const edm::ParameterSet& iConfig):
  histContainer_(),
  /* vtxToken_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertices"))),     //Working
  photonToken_(consumes<pat::PhotonCollection>(iConfig.getParameter<edm::InputTag>("photons"))),
  jetToken_(consumes<pat::JetCollection>(iConfig.getParameter<edm::InputTag>("jets"))),
  metToken_(consumes<pat::METCollection>(iConfig.getParameter<edm::InputTag>("mets")))
  */
  photonSrcToken_(consumes<edm::View<pat::Photon> >(iConfig.getUntrackedParameter<edm::InputTag>("photonSrc"))),
  jetSrcToken_(consumes<edm::View<pat::Jet> >(iConfig.getUntrackedParameter<edm::InputTag>("jetSrc"))),
  metSrcToken_(consumes<edm::View<pat::MET> >(iConfig.getUntrackedParameter<edm::InputTag>("metSrc")))
{
  //now do what ever initialization is needed
  vtxLabel_ = consumes<reco::VertexCollection> (iConfig.getParameter<InputTag>("VtxLabel"));
  rhoLabel_                  = consumes<double>(iConfig.getParameter<InputTag>("rhoLabel"));
  rhoCentralLabel_ = consumes<double> (iConfig.getParameter<InputTag>("rhoCentralLabel"));
  //  trgResultsLabel_           = consumes<edm::TriggerResults>           (iConfig.getParameter<InputTag>("triggerResults"));
  triggerBits_= consumes<edm::TriggerResults>(iConfig.getParameter<InputTag>("triggerResults"));
  triggerObjects_ = consumes<edm::View<pat::TriggerObjectStandAlone>>(iConfig.getParameter<InputTag>("objects"));
  triggerPrescales_ = consumes<pat::PackedTriggerPrescales>(iConfig.getParameter<InputTag>("prescales"));

  phoLooseIdMapToken_ = consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("phoLooseIdMap"));
  phoMediumIdMapToken_ = consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("phoMediumIdMap"));
  phoTightIdMapToken_ = consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("phoTightIdMap"));



  usesResource("TFileService");
  edm::Service<TFileService> fs;
  tree_ = fs->make<TTree>("GJet_Tree", "Event data");
  histContainer_["photons"]=fs->make<TH1F>("photons", "photon multiplicity", 10, 0, 10);
  histContainer_["jets" ]=fs->make<TH1F>("jets", "jet multiplicity", 10, 0, 10);
  histContainer_["met" ]=fs->make<TH1F>("met", "missing E_{T}", 20, 0, 100);
  //hEvents_ = fs->make<TH1F>("hEvents", "total processed and skimmed events", 2, 0, 2);    


  branchesGlobalEvent(tree_);
  branchesJets(tree_);
  branchesMET(tree_);
  branchesPhotons(tree_);
}


G_Jet::~G_Jet()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
G_Jet::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   /*                                             ////Working
   edm::Handle<reco::VertexCollection> vertices;
   iEvent.getByToken(vtxToken_, vertices);
   if (vertices->empty()) return; // skip the event if no PV found
   const reco::Vertex &PV = vertices->front();


   edm::Handle<pat::PhotonCollection> photons;
   iEvent.getByToken(photonToken_, photons);
   for (const pat::Photon &pho : *photons) {
     if (pho.pt() < 20 or pho.chargedHadronIso()/pho.pt() > 0.3) continue;
     printf("phot with pt %4.1f, supercluster eta %+5.3f, sigmaIetaIeta %.3f (%.3f with full5x5 shower shapes)\n",
	    pho.pt(), pho.superCluster()->eta(), pho.sigmaIetaIeta(), pho.full5x5_sigmaIetaIeta());
   }

   edm::Handle<pat::JetCollection> jets;
   iEvent.getByToken(jetToken_, jets);
   int ijet = 0;
   for (const pat::Jet &j : *jets) {
     if (j.pt() < 20) continue;
     printf("jet  with pt %5.1f (raw pt %5.1f), eta %+4.2f, btag CSV %.3f, CISV %.3f, pileup mva disc %+.2f\n",
            j.pt(), j.pt()*j.jecFactor("Uncorrected"), j.eta(), std::max(0.f,j.bDiscriminator("combinedSecondaryVertexBJetTags")), std::max(0.f,j.bDiscriminator("combinedInclusiveSecondaryVertexBJetTags")), j.userFloat("pileupJetId:fullDiscriminant"));
     if ((++ijet) == 1) { // for the first jet, let's print the leading constituents
       std::vector<reco::CandidatePtr> daus(j.daughterPtrVector());
       std::sort(daus.begin(), daus.end(), [](const reco::CandidatePtr &p1, const reco::CandidatePtr &p2) { return p1->pt() > p2->pt(); }); // the joys of C++11
       for (unsigned int i2 = 0, n = daus.size(); i2 < n && i2 <= 3; ++i2) {
	 const pat::PackedCandidate &cand = dynamic_cast<const pat::PackedCandidate &>(*daus[i2]);
	 printf("         constituent %3d: pt %6.2f, dz(pv) %+.3f, pdgId %+3d\n", i2,cand.pt(),cand.dz(PV.position()),cand.pdgId());
       }
     }
   }

   */
   edm::Handle<reco::VertexCollection> vtxHandle;
   iEvent.getByToken(vtxLabel_, vtxHandle);

   reco::Vertex vtx;

   // best-known primary vertex coordinates
   math::XYZPoint pv(0, 0, 0);
   for (vector<reco::Vertex>::const_iterator v = vtxHandle->begin(); v != vtxHandle->end(); ++v) {
     // replace isFake() for miniAOD since it requires tracks while miniAOD vertices don't have tracks:
     // Vertex.h: bool isFake() const {return (chi2_==0 && ndof_==0 && tracks_.empty());}
     bool isFake = (v->chi2() == 0 && v->ndof() == 0);

     if (!isFake) {
       pv.SetXYZ(v->x(), v->y(), v->z());
       vtx = *v;
       break;
     }
   }

   //initTriggerFilters(e);

   fillGlobalEvent(iEvent, iSetup);


   edm::Handle<edm::View<pat::Jet> > jets;
   iEvent.getByToken(jetSrcToken_,jets);

   // get met collection
   edm::Handle<edm::View<pat::MET> > mets;
   iEvent.getByToken(metSrcToken_,mets);

   // get photon collection
   edm::Handle<edm::View<pat::Photon> > photons;
   iEvent.getByToken(photonSrcToken_,photons);

   size_t nJets=0;
   for(edm::View<pat::Jet>::const_iterator jet=jets->begin(); jet!=jets->end(); ++jet){
     if(jet->pt()>50){
       ++nJets;
     }
   }
   
   
   fillJets(iEvent,iSetup);
   fillMET(iEvent,iSetup);
   fillPhotons(iEvent,iSetup);


   histContainer_["jets"]->Fill(nJets);
   histContainer_["photons"]->Fill(photons->size() );
   histContainer_["met" ]->Fill(mets->empty() ? 0 : (*mets)[0].et());
   tree_->Fill();
   //edm::Handle<pat::METCollection> mets;   //Working
   //iEvent.getByToken(metToken_, mets);     //Working
   /*   const pat::MET &met = mets->front();
   printf("MET: pt %5.1f, phi %+4.2f, sumEt (%.1f). genMET %.1f. MET with JES up/down: %.1f/%.1f\n",
	  met.pt(), met.phi(), met.sumEt(),
	  met.genMET()->pt(),
	  met.shiftedPt(pat::MET::JetEnUp), met.shiftedPt(pat::MET::JetEnDown));
   */
   printf("\n");




#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
G_Jet::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
G_Jet::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
G_Jet::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(G_Jet);

import FWCore.ParameterSet.Config as cms

gjet = cms.EDAnalyzer('G_Jet',
                      photonSrc = cms.untracked.InputTag("slimmedPhotons"),
                      jetSrc = cms.untracked.InputTag("slimmedJets"),
                      #jetSrc = cms.untracked.InputTag("selectedUpdatedPatJetsUpdatedJEC"),
                      #jetSrc = cms.untracked.InputTag("selectedPatJetsAK4PFCHS"),
                      #jetSrc = cms.untracked.InputTag("selectedPatJets"),
                      metSrc = cms.untracked.InputTag("slimmedMETs"),
                      #phoLooseIdMap = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-loose"),
                      #phoMediumIdMap = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-medium"),
                      #phoTightIdMap = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-tight")
                      phoLooseIdMap = cms.InputTag('reducedEgammaBeforeGSFix','PhotonCutBasedIDLoose'),
                      phoTightIdMap = cms.InputTag('reducedEgammaBeforeGSFix','PhotonCutBasedIDTight'),
                      phoMediumIdMap = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-medium")
)

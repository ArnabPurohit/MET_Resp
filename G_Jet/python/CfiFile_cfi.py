import FWCore.ParameterSet.Config as cms

gjet = cms.EDAnalyzer('G_Jet',
                      photonSrc = cms.untracked.InputTag("slimmedPhotons"),
                      jetSrc = cms.untracked.InputTag("slimmedJets"),
                      #jetSrc = cms.untracked.InputTag("selectedUpdatedPatJetsUpdatedJEC"),
                      #jetSrc = cms.untracked.InputTag("selectedPatJetsAK4PFCHS"),
                      #jetSrc = cms.untracked.InputTag("selectedPatJets"),
                      metSrc = cms.untracked.InputTag("slimmedMETs")
)

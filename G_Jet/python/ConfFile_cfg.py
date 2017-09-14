import FWCore.ParameterSet.Config as cms

process = cms.Process("gjet")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
#process.load("Configuration.Geometry.GeometryIdeal_cff" )
process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff" )
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_2016SeptRepro_v7')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        #'file:pickevents.root'
        '/store/data/Run2016C/SinglePhoton/MINIAOD/23Sep2016-v1/70000/0E03274A-248A-E611-96AF-0025905A613C.root'
        #'/store/data/Run2016B/SingleElectron/MINIAOD/23Sep2016-v2/80000/5A4402F5-638C-E611-A471-0025905A60AA.root'
    )
)
#####VID framework####################
from RecoEgamma.PhotonIdentification.egmPhotonIDs_cfi import *
from PhysicsTools.SelectorUtils.centralIDRegistry import central_id_registry

# Load the producer module to build full 5x5 cluster shapes and whatever 
# else is needed for IDs
from RecoEgamma.PhotonIdentification.PhotonIDValueMapProducer_cfi import *

from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
dataFormat = DataFormat.MiniAOD
#switchOnVIDElectronIdProducer(process, dataFormat)
switchOnVIDPhotonIdProducer(process, dataFormat)
my_phoid_modules = ['RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Spring16_V2p2_cff']#,
                    #'RecoEgamma.PhotonIdentification.Identification.mvaPhotonID_Spring16_nonTrig_V1_cff']
process.photonIDValueMapProducer.srcMiniAOD = cms.InputTag('slimmedPhotons')
#process.photonMVAValueMapProducer.srcMiniAOD = cms.InputTag('slimmedPhotons')
for idmod in my_phoid_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDPhotonSelection)

process.gjet = cms.EDAnalyzer('G_Jet',
                              #vertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
                              photonSrc = cms.untracked.InputTag("slimmedPhotons"),
                              jetSrc = cms.untracked.InputTag("slimmedJets"),
                              #jetSrc = cms.untracked.InputTag("selectedUpdatedPatJetsUpdatedJEC"),
                              metSrc = cms.untracked.InputTag("slimmedMETs"),
                              VtxLabel             = cms.InputTag("offlineSlimmedPrimaryVertices"),
                              rhoLabel             = cms.InputTag("fixedGridRhoFastjetAll"),
                              rhoCentralLabel = cms.InputTag("fixedGridRhoFastjetCentralNeutral"),
                              #triggerResults       = cms.InputTag("TriggerResults", "", "HLT"),
                              triggerResults = cms.InputTag("TriggerResults","","HLT"),
                              prescales = cms.InputTag("patTrigger","","PAT"), #PAT
                              objects = cms.InputTag("selectedPatTrigger"),
                              #phoLooseIdMap=cms.InputTag("reducedEgammaBeforeGSFix","PhotonCutBasedIDLoose"),
                              phoLooseIdMap=cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-loose"),
                              #phoTightIdMap = cms.InputTag("reducedEgammaBeforeGSFix","PhotonCutBasedIDTight"),
                              phoTightIdMap = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-tight"),
                              phoMediumIdMap = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-medium")
                              )
process.TFileService = cms.Service("TFileService",
  fileName = cms.string('G_Jet.root')
)


process.p = cms.Path(
    #process.photonIDValueMapProducer *
    process.egmPhotonIDSequence*
    process.gjet)

import FWCore.ParameterSet.Config as cms

process = cms.Process("electronTreeProducer")

# import of standard configurations

process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.StandardSequences.Services_cff")
process.load('Configuration.Geometry.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Geometry.CaloEventSetup.EcalTrigTowerConstituents_cfi")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load("Geometry.CaloEventSetup.EcalTrigTowerConstituents_cfi")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = '74X_dataRun2_Prompt_v2'
process.prefer("GlobalTag")
 
HLT_name = 'HLT'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1)
                                        #SkipEvent = cms.untracked.vstring('ProductNotFound')
                                        )

# ---------------------------------------------------------------------
# Input Files
# ---------------------------------------------------------------------
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(

                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015C/DoubleEG/RAW-RECO/ZElectron-PromptReco-v1/000/254/833/00000/E85E7F06-474B-E511-9B83-02163E012204.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015C/DoubleEG/RAW-RECO/ZElectron-PromptReco-v1/000/254/833/00000/40AB1AF5-094B-E511-A4B9-02163E01472E.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015C/DoubleEG/RAW-RECO/ZElectron-PromptReco-v1/000/254/833/00000/C2C26B00-0A4B-E511-B2F3-02163E011EE9.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015C/DoubleEG/RAW-RECO/ZElectron-PromptReco-v1/000/254/833/00000/F6168DFC-094B-E511-8420-02163E0129B0.root"
                            ),                         
                        )

process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

# ---------------------------------------------------------------------
# Ouptut File
# ---------------------------------------------------------------------
process.TFileService = cms.Service ("TFileService", 
                                    fileName = cms.string ("tree.root")
                                    )
process.Out = cms.OutputModule("PoolOutputModule",
     outputCommands = cms.untracked.vstring('keep *_*_*_skimAllPathsFilter'),
     fileName = cms.untracked.string('turnonoutput.root')
)
from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *

process.e = cms.EndPath(process.Out)                                       

# ---------------------------------------------------------------------
# JETS
# ---------------------------------------------------------------------
# JPT
#process.load('RecoJets.Configuration.RecoJPTJets_cff')
process.load('RecoJets.Configuration.RecoPFJets_cff')
process.load('RecoJets.Configuration.RecoPFJets_cff')

# ---------------------------------------------------------------------
# Fast Jet Rho Correction
# ---------------------------------------------------------------------
process.load('RecoJets.JetProducers.kt4PFJets_cfi')
process.kt6PFJets = process.kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
process.kt6PFJets.Rho_EtaMax = cms.double(2.5)

from RecoVertex.Configuration.RecoVertex_cff import *
# ---------------------------------------------------------------------
# Set up electron ID (VID framework)
# ---------------------------------------------------------------------
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
# turn on VID producer, indicate data format  to be
# DataFormat.AOD or DataFormat.MiniAOD, as appropriate 
dataFormat = DataFormat.AOD
switchOnVIDElectronIdProducer(process, dataFormat)
# define which IDs we want to produce
my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Spring15_50ns_V2_cff',
                 'RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Spring15_25ns_V1_cff']
#add them to the VID producer
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

# ---------------------------------------------------------------------
# Unpack Ecal Digis
# ---------------------------------------------------------------------
process.load("EventFilter.EcalRawToDigi.EcalUnpackerMapping_cfi");
process.load("EventFilter.EcalRawToDigi.EcalUnpackerData_cfi");
process.ecalEBunpacker.InputLabel = cms.InputTag('rawDataCollector');

# ---------------------------------------------------------------------
# Simulate Ecal Trigger Primitives
# ---------------------------------------------------------------------

# Config from file
process.load('SimCalorimetry.EcalTrigPrimProducers.ecalTrigPrimESProducer_cff')
#--------------------------------------I changed the one to default--------------------
#process.EcalTrigPrimESProducer.DatabaseFile = 'TPG_beamv6_notrans_spikekill_sfgvb24.tar' 
 
# take EBDigis from saved RAW-RECO collection
process.load("SimCalorimetry.EcalTrigPrimProducers.ecalTriggerPrimitiveDigis_cff")
# process.simEcalTriggerPrimitiveDigis.Label = 'ecalDigis'
process.simEcalTriggerPrimitiveDigis.Label = 'ecalEBunpacker'
# process.simEcalTriggerPrimitiveDigis.Label = 'selectDigi'
# process.simEcalTriggerPrimitiveDigis.InstanceEB =  'selectedEcalEBDigiCollection'
# process.simEcalTriggerPrimitiveDigis.InstanceEE =  'selectedEcalEEDigiCollection'
process.simEcalTriggerPrimitiveDigis.InstanceEB =  'ebDigis'
process.simEcalTriggerPrimitiveDigis.InstanceEE =  'eeDigis'
# process.simEcalTriggerPrimitiveDigis.BarrelOnly = True
process.simEcalTriggerPrimitiveDigis.BarrelOnly =False 

# ---------------------------------------------------------------------
# Simulate Ecal Trigger Primitives
# ---------------------------------------------------------------------
process.load('Configuration.StandardSequences.SimL1Emulator_cff') 
process.load('L1Trigger.Configuration.L1Trigger_EventContent_cff')

# emulator trigger
process.simRctDigis.ecalDigis = cms.VInputTag(cms.InputTag("simEcalTriggerPrimitiveDigis"))
process.simRctDigis.hcalDigis = cms.VInputTag(cms.InputTag("hcalDigis"))
process.simGctDigis.inputLabel = cms.InputTag("simRctDigis")

# L1 extra for the re-simulated candidates
process.l1extraParticles = cms.EDProducer("L1ExtraParticlesProd",
                                          muonSource = cms.InputTag("gtDigis"),
                                          etTotalSource = cms.InputTag("simGctDigis"),
                                          nonIsolatedEmSource = cms.InputTag("simGctDigis","nonIsoEm"),
                                          etMissSource = cms.InputTag("simGctDigis"),
                                          htMissSource = cms.InputTag("simGctDigis"),
                                          produceMuonParticles = cms.bool(False),
                                          forwardJetSource = cms.InputTag("simGctDigis","forJets"),
                                          centralJetSource = cms.InputTag("simGctDigis","cenJets"),
                                          produceCaloParticles = cms.bool(True),
                                          tauJetSource = cms.InputTag("simGctDigis","tauJets"),
                                          isoTauJetSource = cms.InputTag("simGctDigis","isoTauJets"),
                                          isolatedEmSource = cms.InputTag("simGctDigis","isoEm"),
                                          etHadSource = cms.InputTag("simGctDigis"),
                                          hfRingEtSumsSource = cms.InputTag("simGctDigis"),
                                          hfRingBitCountsSource = cms.InputTag("simGctDigis"),
                                          centralBxOnly = cms.bool(True),
                                          ignoreHtMiss = cms.bool(False)
                                          )


# L1 extra for the online candidates
process.l1extraParticlesOnline = cms.EDProducer("L1ExtraParticlesProd",
                                                muonSource = cms.InputTag("gtDigis"),
                                                etTotalSource = cms.InputTag("gctDigis"),
                                                nonIsolatedEmSource = cms.InputTag("gctDigis","nonIsoEm"),
                                                etMissSource = cms.InputTag("gctDigis"),
                                                htMissSource = cms.InputTag("gctDigis"),
                                                produceMuonParticles = cms.bool(False),
                                                forwardJetSource = cms.InputTag("gctDigis","forJets"),
                                                centralJetSource = cms.InputTag("gctDigis","cenJets"),
                                                produceCaloParticles = cms.bool(True),
                                                tauJetSource = cms.InputTag("gctDigis","tauJets"),
                                                isoTauJetSource = cms.InputTag("gctDigis","isoTauJets"),
                                                isolatedEmSource = cms.InputTag("gctDigis","isoEm"),
                                                etHadSource = cms.InputTag("gctDigis"),
                                                hfRingEtSumsSource = cms.InputTag("gctDigis"),
                                                hfRingBitCountsSource = cms.InputTag("gctDigis"),
                                                centralBxOnly = cms.bool(True),
                                                ignoreHtMiss = cms.bool(False)
                                                )



# ---------------------------------------------------------------------
# Produce Ntuple Module
# ---------------------------------------------------------------------

process.load("EGamma.ECGelec.NtupleProducer_custom_cfi")
from EGamma.ECGelec.NtupleProducer_custom_cfi import *
process.produceNtuple = produceNtupleCustom.clone()


process.produceNtuple.NadL1M = cms.untracked.bool(True)
process.produceNtuple.NadTP = cms.untracked.bool(True)
process.produceNtuple.NadTPemul = cms.untracked.bool(True) # Need to put True when running Emulator !!
process.produceNtuple.NadTPmodif = cms.untracked.bool(False)
process.produceNtuple.PrintDebug = cms.untracked.bool(False)
#process.produceNtuple.PrintDebug = cms.untracked.bool(True)

process.produceNtuple.type = 'DATA'
process.produceNtuple.AOD = cms.untracked.bool(False)
process.produceNtuple.FillSC = cms.untracked.bool(True)
process.produceNtuple.functionName = cms.string("EcalClusterEnergyUncertainty")
# Trigger Stuff
process.produceNtuple.HLTTag          = 'TriggerResults::' + HLT_name
process.produceNtuple.TriggerEventTag = 'hltTriggerSummaryAOD::' + HLT_name
process.produceNtuple.HLTElePaths = cms.vstring(
   # 'HLT_Ele12_CaloIdL_TrackIdL_IsoVL_v',

'HLT_Ele17_CaloIdL_TrackIdL_IsoVL_v1',
'HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v2',
'HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_v2'
        )
process.produceNtuple.HLTMuonPaths    = cms.vstring('HLT_Mu9')
process.produceNtuple.HLTFilters      = cms.VInputTag('hltL1NonIsoHLTNonIsoSingleElectronEt17TighterEleIdIsolTrackIsolFilter::'+HLT_name,
                                                      'hltL1NonIsoHLTNonIsoDoubleElectronEt17PixelMatchFilter::'+HLT_name,
                                                      #'hltL1NonIsoHLTNonIsoSingleElectronEt17TightCaloEleIdEle8HEPixelMatchFilter::'+HLT_name,
                                                      'hltL1NonIsoHLTNonIsoSingleElectronEt17TighterEleIdIsolPixelMatchFilter::'+HLT_name,
                                                      'hltL1NonIsoHLTNonIsoSingleElectronEt17TightCaloEleIdEle8HEDoublePixelMatchFilter::'+HLT_name,
                                                      # Muon Trigger
                                                      'hltSingleMu9L3Filtered9')

process.produceNtuple.eleVetoIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V2-standalone-veto")
process.produceNtuple.eleLooseIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V2-standalone-loose")
process.produceNtuple.eleMediumIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V2-standalone-medium")
process.produceNtuple.eleTightIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V2-standalone-tight")

# ---------------------------------------------------------------------
# Sequence PATH
# ---------------------------------------------------------------------
process.goodElectrons = cms.EDFilter("GsfElectronRefSelector",
                             src = cms.InputTag("gedGsfElectrons"),
                             cut = cms.string(    "pt > 10"),
                              )
process.diElectrons = cms.EDProducer("CandViewShallowCloneCombiner",
                                     decay       = cms.string("goodElectrons goodElectrons"),
                                     checkCharge = cms.bool(False),
                                     cut         = cms.string("mass > 65 && mass < 115"),
                                     )

process.diElectronsFilter = cms.EDFilter("CandViewCountFilter",
                                    src = cms.InputTag("diElectrons"),
                                    minNumber = cms.uint32(1)
 )

process.diElectronSequence = cms.Sequence( process.goodElectrons * process.diElectrons * process.diElectronsFilter )
process.diElectronFilter = cms.Path(process.diElectronSequence)
process.p = cms.Path (
    process.kt6PFJets + 
    process.ecalEBunpacker +
    process.simEcalTriggerPrimitiveDigis +
    process.simRctDigis +
    process.simGctDigis +
    process.simGtDigis +
    process.l1extraParticles +
    process.l1extraParticlesOnline +
    process.egmGsfElectronIDSequence +
    process.produceNtuple
    
    )
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.schedule = cms.Schedule(process.diElectronFilter,process.p)

import FWCore.ParameterSet.Config as cms

process = cms.Process("electronTreeProducer")

# import of standard configurations

process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.StandardSequences.Services_cff")
#process.load("Configuration.StandardSequences.MixingNoPileUp_cff")
#process.load("Configuration.Geometry.GeometryPilot2_cff")
process.load('Configuration.Geometry.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
#process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")
process.load("Geometry.CaloEventSetup.EcalTrigTowerConstituents_cfi")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load("Geometry.CaloEventSetup.EcalTrigTowerConstituents_cfi")
#process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
#process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
#process.GlobalTag.globaltag = '74X_dataRun2_Prompt_v1'
#process.GlobalTag.globaltag = '74X_dataRun2_Express_v1'
process.GlobalTag.globaltag = '74X_dataRun2_Prompt_v1'
process.prefer("GlobalTag")
# 4_4_0
#process.GlobalTag.globaltag = 'GR_R_44_V1::All'

# Florian FastSim 441
#process.GlobalTag.globaltag = 'START44_V6::All'
#process.GlobalTag.globaltag = 'MC_53_V15A::All'
#process.GlobalTag.globaltag = '74X_dataRun2_Prompt_v1'#GR_P_V42_AN4
#process.GlobalTag.globaltag = '74X_dataRun2_Express_v1'#GR_P_V42_AN4
#process.GlobalTag.toGet = cms.VPSet( 
#        cms.PSet(record = cms.string("EcalTPGLinearizationConstRcd"),
            # tag = cms.string("EcalTPGLinearizationConst_weekly_EBEE_Run1_hlt"),             
            # tag = cms.string("EcalTPGLinearizationConst_weekly_EBEE_hlt"),
#             tag = cms.string("EcalTPGLinearizationConst_weekly_EBEE_withalpha_dbv1"),
#             connect =cms.untracked.string('frontier://FrontierPrep/CMS_COND_ECAL')
           #  connect =cms.untracked.string('frontier://FrontierPrep/CMS_CONDITIONS')             )
#                 )
#       )
 
HLT_name = 'HLT'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10)
                                        #SkipEvent = cms.untracked.vstring('ProductNotFound')
                                        )

# ---------------------------------------------------------------------
# Input Files
# ---------------------------------------------------------------------
process.source = cms.Source("PoolSource",
                            #debugFlag = cms.untracked.bool(True),
                            #debugVebosity = cms.untracked.uint32(10),
                            fileNames = cms.untracked.vstring(
#                              "root://cms-xrd-global.cern.ch//store/data/Run2012A/DoubleElectron/RAW-RECO/ZElectron-22Jan2013-v1/20000/02A1EC8D-8C67-E211-9729-002618943864.root"                  
#                               "root://eoscms.cern.ch//eos/cms/store/user/fmeng/0008202C-E78F-E211-AADB-0026189437FD.root"
                               # 'root://cms-xrd-global.cern.ch//store/data/Run2015C/DoubleEG/RAW-RECO/ZElectron-PromptReco-v1/000/254/094/00000/D41B4DB2-CA45-E511-844E-02163E013805.root'
                             "root://eoscms.cern.ch//eos/cms/store/user/fmeng/256676_raw_reco.root" 
                          #   '/store/data/Run2012A/DoubleElectron/RAW-RECO/ZElectron-13Jul2012-v1/00000/00663DF3-28DA-E111-966A-848F69FD4508.root'
#                              'root://cms-xrd-global.cern.ch//store/data/Run2012A/DoubleElectron/RAW-RECO/ZElectron-13Jul2012-v1/00000/C87DD084-4CDA-E111-B86C-00266CF25C20.root'     
                            ),                         
                        )

process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

# ---------------------------------------------------------------------
# Ouptut File
# ---------------------------------------------------------------------
process.TFileService = cms.Service ("TFileService", 
                                    fileName = cms.string ("tree_testFastSim_TPG_checknow.root")
                                    )
process.Out = cms.OutputModule("PoolOutputModule",
    # outputCommands = cms.untracked.vstring('keep *_*SuperCluster*_*_*'),
     outputCommands = cms.untracked.vstring('keep *_*_*_skimAllPathsFilter'),
   #  outputCommands = cms.untracked.vstring('keep *'),
     fileName = cms.untracked.string('turnonoutput.root')
)
from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *

process.e = cms.EndPath(process.Out)                                       
process.runSelection = cms.EDFilter("RunSelect",
    requireNoTimeScan = cms.untracked.bool(True) ,
    requireCollidingBX = cms.untracked.bool(False),
    requireNoLumiScan = cms.untracked.bool(False),
    debug = cms.untracked.bool(False)
    )

# ---------------------------------------------------------------------
# Skim ALL Path Filter
# ---------------------------------------------------------------------
#load the EDfilter to select just skim data
process.load("EGamma.LLRSkim.skimAllPathsFilter_cfi")
from EGamma.LLRSkim.skimAllPathsFilter_cfi import *
process.skimAllPathsFilter = skimAllPathsFilter.clone()

# Nadir TagAndProbe : at least 2 ele with eT>5 and at least 1 ele passing eleID==VBTF95
process.skimAllPathsFilter.mode = "TP_nadir"
process.skimAllPathsFilter.eleID= "VBTF95"


# ---------------------------------------------------------------------
# JETS
# ---------------------------------------------------------------------
# JPT
#process.load('RecoJets.Configuration.RecoJPTJets_cff')
process.load('RecoJets.Configuration.RecoPFJets_cff')
process.load('RecoJets.Configuration.RecoPFJets_cff')
#JEC Corrections... to come !
# for 360: create colection of L2L3 corrected JPT jets: ak5JPTJetsL2L3  
# one need set of tags will be provided be JES
# process.p1 = cms.Path(process.ak5JPTJetsL2L3*process.dump)

# ---------------------------------------------------------------------
# Fast Jet Rho Correction
# ---------------------------------------------------------------------
process.load('RecoJets.JetProducers.kt4PFJets_cfi')
process.kt6PFJets = process.kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
process.kt6PFJets.Rho_EtaMax = cms.double(2.5)

# ---------------------------------------------------------------------
# PF Isolation
# ---------------------------------------------------------------------
#from CommonTools.ParticleFlow.Tools.pfIsolation import setupPFElectronIso, setupPFMuonIso
#process.eleIsoSequence = setupPFElectronIso(process, 'gsfElectrons')
#process.pfiso = cms.Sequence(process.pfParticleSelectionSequence + process.eleIsoSequence)

# ---------------------------------------------------------------------
# Vertexing DA
# ---------------------------------------------------------------------
process.load("RecoVertex.Configuration.RecoVertex_cff")
process.load("RecoTracker.IterativeTracking.InitialStep_cff")
#from RecoVertex.Configuration.RecoVertex_cff import *
#process.vertexreco = cms.Sequence(offlinePrimaryVertices*offlinePrimaryVerticesWithBS)

from RecoVertex.Configuration.RecoVertex_cff import *
#process.vertexreco = cms.Sequence(offlinePrimaryVertices*offlinePrimaryVerticesWithBS)

# ---------------------------------------------------------------------
# Produce eID infos
# ---------------------------------------------------------------------
###process.load("RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentification_cfi")
###New optimization

#-------------------------------------********************------------------electron selection
#process.load("RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Spring15_25ns_V1_cff")

#my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Spring15_25ns_V1_cff']







process.load("RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Spring15_25ns_V1_cff")
#process.load("EgammaAnalysis/ElectronTools/Validation/DYJetsToLL_Sc2_AODSIM")
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
dataFormat = DataFormat.AOD
# turn on VID producer
switchOnVIDElectronIdProducer(process,dataFormat)
# define which IDs we want to produce
#my_id_modules = ['EgammaAnalysis.ElectronTools.Identification.cutBasedElectronID_CSA14_50ns_V1_cff',
#                 'EgammaAnalysis.ElectronTools.Identification.cutBasedElectronID_CSA14_PU20bx25_V0_cff',
#                 'EgammaAnalysis.ElectronTools.Identification.heepElectronID_HEEPV50_CSA14_25ns_cff',
#                 'EgammaAnalysis.ElectronTools.Identification.heepElectronID_HEEPV50_CSA14_startup_cff']
my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Spring15_25ns_V1_cff']
#add them to the VID producer
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

process.electronIDValueMapProducer.ebReducedRecHitCollection = cms.InputTag('reducedEcalRecHitsEB')
process.electronIDValueMapProducer.eeReducedRecHitCollection = cms.InputTag('reducedEcalRecHitsEE')
process.electronIDValueMapProducer.esReducedRecHitCollection = cms.InputTag('reducedEcalRecHitsES')

process.wp1 = cms.EDAnalyzer('ElectronIDValidationAnalyzer',
                             electrons = cms.InputTag("gedGsfElectrons"),
                             electronIDs = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-veto"),
                             vertices = cms.InputTag("offlinePrimaryVertices"),
                             genparticles = cms.InputTag("genParticles"),
                             convcollection = cms.InputTag("conversions"),
                             beamspot = cms.InputTag("offlineBeamSpot"),
                             full5x5SigmaIEtaIEtaMap = cms.InputTag("electronIDValueMapProducer:eleFull5x5SigmaIEtaIEta"),
                             )

process.wp2 = cms.EDAnalyzer('ElectronIDValidationAnalyzer',
                             electrons = cms.InputTag("gedGsfElectrons"),
                             electronIDs = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-loose"),
                             vertices = cms.InputTag("offlinePrimaryVertices"),
                             genparticles = cms.InputTag("genParticles"),
                             convcollection = cms.InputTag("conversions"),
                             beamspot = cms.InputTag("offlineBeamSpot"),
                             full5x5SigmaIEtaIEtaMap = cms.InputTag("electronIDValueMapProducer:eleFull5x5SigmaIEtaIEta"),
                             )

process.wp3 = cms.EDAnalyzer('ElectronIDValidationAnalyzer',
                             electrons = cms.InputTag("gedGsfElectrons"),
                             electronIDs = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-medium"),
                             vertices = cms.InputTag("offlinePrimaryVertices"),
                             genparticles = cms.InputTag("genParticles"),
                             convcollection = cms.InputTag("conversions"),
                             beamspot = cms.InputTag("offlineBeamSpot"),
                             full5x5SigmaIEtaIEtaMap = cms.InputTag("electronIDValueMapProducer:eleFull5x5SigmaIEtaIEta"),
                             )

process.wp4 = cms.EDAnalyzer('ElectronIDValidationAnalyzer',
                             electrons = cms.InputTag("gedGsfElectrons"),
                             electronIDs = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-tight"),
                             vertices = cms.InputTag("offlinePrimaryVertices"),
                             genparticles = cms.InputTag("genParticles"),
                             convcollection = cms.InputTag("conversions"),
                             beamspot = cms.InputTag("offlineBeamSpot"),
                             full5x5SigmaIEtaIEtaMap = cms.InputTag("electronIDValueMapProducer:eleFull5x5SigmaIEtaIEta"),
                             )









#------------------------------------*********************-----------------------
#process.load("RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_DataTuning_cfi")
# ---------------------------------------------------------------------
# Produce eIso infos from HZZ official package
# ---------------------------------------------------------------------
process.load("EGamma.ECGelec.HzzIsolationSequences_cff")
# ---------------------------------------------------------------------
# Produce muIso infos from HZZ official package
# ---------------------------------------------------------------------
#process.load("EGamma.ECGelec.muonHzzIsolationSequences_cff")

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

#process.load("EGamma.ECGelec.NtupleProducer_cfi")
#from EGamma.ECGelec.NtupleProducer_cfi import *
#process.produceNtuple = produceNtuple.clone()

#-------------process.load("EGamma.ECGelec.NtupleProducer_custom_cfi")
#-------------from EGamma.ECGelec.NtupleProducer_custom_cfi import *
#-------------process.produceNtuple = produceNtupleCustom.clone()
process.load("EGamma.ECGelec.NtupleProducer_EleL1Study_cfi")
process.load("RecoEgamma.ElectronIdentification.egmGsfElectronIDs_cff")
from EGamma.ECGelec.NtupleProducer_EleL1Study_cfi import *

process.produceNtuple = produceNtupleL1Study.clone()
process.produceNtuple.GetL1 = cms.untracked.bool(True)
process.produceNtuple.GetL1M = cms.untracked.bool(False)
process.produceNtuple.GetL1_SLHC = cms.untracked.bool(True)

## Get Trigger Primitives
process.produceNtuple.GetTP      = cms.untracked.bool(True)
process.produceNtuple.GetTPmodif = cms.untracked.bool(False)
process.produceNtuple.GetTPemul  = cms.untracked.bool(False)
process.produceNtuple.GetHcalTP  = cms.untracked.bool(True)
process.produceNtuple.GetStripMask = cms.untracked.bool(True)
process.produceNtuple.GetXtalMask  = cms.untracked.bool(True)
process.produceNtuple.GetVertices  = cms.untracked.bool(True)
## Printout
process.produceNtuple.PrintDebug = cms.untracked.bool(True)#False) #modif-Alex 
process.produceNtuple.PrintDebug_HLT = cms.untracked.bool(False) #modif-alex

## Data to analyze
process.produceNtuple.type = 'DATA'
process.produceNtuple.AOD  = cms.untracked.bool(True)
process.produceNtuple.DoFillTrigger = cms.untracked.bool(True)
process.produceNtuple.DoFillEle     = cms.untracked.bool(True)
process.produceNtuple.DoFillSC      = cms.untracked.bool(False)
process.produceNtuple.DoFillSpikes  = cms.untracked.bool(True)

HLT_name = 'HLT'
process.produceNtuple.HLTTag = 'TriggerResults::' + HLT_name
process.produceNtuple.HLTElePaths = cms.vstring(
    'HLT_Ele12_CaloIdL_TrackIdL_IsoVL_v',
        )


process.produceNtuple.HLT_paths = cms.vstring(
    'HLT_Ele12_CaloIdL_TrackIdL_IsoVL_v',
    )
## Nadir's parameters
#process.produceNtuple.NadL1M = cms.untracked.bool(True)
#process.produceNtuple.NadTP = cms.untracked.bool(True)
#process.produceNtuple.NadTPemul = cms.untracked.bool(True) # Need to put True when running Emulator !!
#process.produceNtuple.NadTPmodif = cms.untracked.bool(False)
#process.produceNtuple.PrintDebug = cms.untracked.bool(False)
##process.produceNtuple.PrintDebug_HLT = cms.untracked.bool(False)
#
### standard parameters
##process.produceNtuple.type = 'MC'
#process.produceNtuple.type = 'DATA'
#process.produceNtuple.AOD = cms.untracked.bool(False)
#process.produceNtuple.FillSC = cms.untracked.bool(True)
#process.produceNtuple.functionName = cms.string("EcalClusterEnergyUncertainty")
## Trigger Stuff
#process.produceNtuple.HLTTag          = 'TriggerResults::' + HLT_name
#process.produceNtuple.TriggerEventTag = 'hltTriggerSummaryAOD::' + HLT_name
#process.produceNtuple.HLTElePaths     = cms.vstring(
#    'HLT_Ele17_SW_TighterEleIdIsol_L1R_v3', 'HLT_Ele17_SW_TighterEleIdIsol_L1R_v2', 'HLT_Ele17_SW_TighterEleIdIsol_L1R_v1',
#    'HLT_Ele17_SW_TightEleIdIsol_L1R', 'HLT_DoubleEle17_SW_L1R_v1', 'HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v2',
#    'HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v1')
#process.produceNtuple.HLTMuonPaths    = cms.vstring('HLT_Mu9')
#process.produceNtuple.HLTFilters      = cms.VInputTag('hltL1NonIsoHLTNonIsoSingleElectronEt17TighterEleIdIsolTrackIsolFilter::'+HLT_name,
#                                                      'hltL1NonIsoHLTNonIsoDoubleElectronEt17PixelMatchFilter::'+HLT_name,
#                                                      'hltL1NonIsoHLTNonIsoSingleElectronEt17TightCaloEleIdEle8HEPixelMatchFilter::'+HLT_name,
#                                                      'hltL1NonIsoHLTNonIsoSingleElectronEt17TightCaloEleIdEle8HEDoublePixelMatchFilter::'+HLT_name,
#                                                      # Muon Trigger
#                                                      'hltSingleMu9L3Filtered9')
#
#hltL1NonIsoHLTNonIsoSinglePhotonEt15HcalIsolFilter::'+HLT_name)
#should add one for the Cleaned trigger?!

## HLT Filter from S. Beauceron
#----------------
#import HLTrigger.HLTfilters.hltHighLevel_cfi
#
#process.MyHLTSelection = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone(
#    # SingleElectron paths
#    #HLTPaths = [ 'HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v3',
#    #             'HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2',
#    #             'HLT_Ele45_CaloIdVT_TrkIdT_v3'
#    #             ]
#    # DoubleElectron paths
###     HLTPaths = [ 'HLT_Ele17_CaloIdL_CaloIsoVL_v3',
###                  'HLT_Ele8_CaloIdL_CaloIsoVL_v3',
###                  'HLT_Ele8_CaloIdL_TrkIdVL_v3',
###                  'HLT_Ele8_v3'
###                  'HLT_Ele17_CaloIdL_CaloIsoVL_v2',
###                  'HLT_Ele8_CaloIdL_CaloIsoVL_v2',
###                  'HLT_Ele8_CaloIdL_TrkIdVL_v2',
###                  'HLT_Ele8_v2'
###                  'HLT_Ele17_CaloIdL_CaloIsoVL_v1',
###                  'HLT_Ele8_CaloIdL_CaloIsoVL_v1',
###                  'HLT_Ele8_CaloIdL_TrkIdVL_v1',
###                  'HLT_Ele8_v1'
###                  ],
#
#    # to get the spikes
#    #HLTPaths = [ 'HLT_Activity_Ecal_SC*_*' ],
#    #HLTPaths = [ 'HLT_Activity_Ecal_SC*_*',
#    #             'HLT_L1SingleEG5_*' ],
#    HLTPaths = [ 'HLT_L1SingleEG*' ],
#    
#    throw = False
#    #dont throw except on unknown path name
#    )
#process.HLTfilter = cms.Path( process.MyHLTSelection )

# ---------------------------------------------------------------------
# Save all event content in separate file (for debug purposes)
# ---------------------------------------------------------------------
# Output definition
#process.SpecialEventContent = cms.PSet(
#         outputCommands = cms.untracked.vstring('drop *'),
#        outputCommands = cms.untracked.vstring('keep *'),
#        splitLevel = cms.untracked.int32(0)
#       )

#process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
#                                              splitLevel = cms.untracked.int32(0),
                                              #outputCommands = cms.untracked.vstring('keep *'),
                                              #outputCommands = process.RECOEventContent.outputCommands,
#                                              outputCommands = process.SpecialEventContent.outputCommands,
#                                              fileName = cms.untracked.string('tree_testFastSim_EventContent.root'),
#                                              #SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring ('filter') ),
#                                              dataset = cms.untracked.PSet(
        #filterName = cms.untracked.string('EmulSpikesFilter'),
#        dataTier = cms.untracked.string('DIGI-RECO')
#        )
#)

#process.output_step = cms.EndPath(process.FEVTDEBUGHLToutput)

# ---------------------------------------------------------------------
# Sequence PATH
# ---------------------------------------------------------------------
process.p = cms.Path (
    #process.MyHLTSelection +
#    process.InitialStep+
#    process.vertexreco# + 
    process.skimAllPathsFilter +   
    process.kt6PFJets + 
    
    process.runSelection +
    process.wp1+
    process.wp2+
    process.wp3+
    process.wp4+
    #produce the eID CiC value maps
#    process.eidVeryLoose#+
   # process.eidLoose+
   # process.eidMedium+
   # process.eidTight+
   # process.eidSuperTight+
   # process.eidHyperTight1#+
    #process.eidHyperTight2+
    #process.eidHyperTight3+
    #process.eidHyperTight4+
    process.HzzIsolationSequence# +
    #process.MuonHZZIsolationSequence +
    
#    process.ecalEBunpacker +
#    process.simEcalTriggerPrimitiveDigis +
#    process.simRctDigis +
#    process.simGctDigis +
#    process.simGtDigis +
#    process.l1extraParticles +
#    process.l1extraParticlesOnline# +
    
#    process.produceNtuple
    
    )

#process.schedule = cms.Schedule( process.p, process.output_step)
process.schedule = cms.Schedule( process.p)
#process.schedule = cms.Schedule( process.p,process.e)
#pythonDump = open("dump_cfg.py", "write")
#print >> pythonDump, process.dumpPython()

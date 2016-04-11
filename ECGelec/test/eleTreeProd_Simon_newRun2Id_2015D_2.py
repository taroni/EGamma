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
process.GlobalTag.globaltag = '74X_dataRun2_Express_v2'
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

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1)
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
                              # "root://eoscms.cern.ch//eos/cms/store/data/Run2015C/DoubleEG/RAW-RECO/ZElectron-PromptReco-v1/000/254/905/00000/222A7973-B24B-E511-8446-02163E0136CF.root"
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/843/00000/08E826D8-4F61-E511-AFAB-02163E013764.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/843/00000/72413EBB-D360-E511-B634-02163E0118B0.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/843/00000/C0A850B8-CD60-E511-9F05-02163E012022.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/843/00000/248DF2D9-D060-E511-9419-02163E01437A.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/843/00000/A8F3F914-D660-E511-8AC9-02163E0138BA.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/843/00000/C69AC11A-D060-E511-A87A-02163E01419D.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/843/00000/2C6A4E28-D860-E511-A291-02163E01244E.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/843/00000/AA33890E-E360-E511-9FB5-02163E0136FE.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/843/00000/DAA93923-C360-E511-8996-02163E011DA5.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/843/00000/2ED422FE-CB60-E511-80FD-02163E011FC3.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/843/00000/B264D643-C860-E511-809D-02163E01429D.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/843/00000/F25D6C63-C560-E511-8071-02163E0136BA.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/843/00000/420805BE-CD60-E511-B402-02163E01431E.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/843/00000/B2BD25E0-C960-E511-8D5C-02163E0145CC.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/843/00000/4AB340CC-D060-E511-8F99-02163E0119DE.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/843/00000/B40AFF05-CC60-E511-865D-02163E012A11.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/865/00000/16DBF115-F160-E511-AC7D-02163E01350E.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/866/00000/00B1E3F3-FC60-E511-A119-02163E01410B.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/867/00000/28B63F16-0B61-E511-BA01-02163E014409.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/867/00000/5283FAC9-2C61-E511-B28C-02163E011AC8.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/867/00000/72065756-1461-E511-A5D1-02163E0137CA.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/868/00000/1A7003E4-6361-E511-98E2-02163E012222.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/868/00000/922A5469-7961-E511-81B6-02163E0134A7.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/868/00000/C04453B3-7461-E511-88E7-02163E0142CB.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/868/00000/56331E1B-9061-E511-B541-02163E014660.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/868/00000/965A1FE0-6C61-E511-A3E1-02163E012B2B.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/868/00000/C4CA63A2-7261-E511-85BE-02163E0135FA.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/868/00000/84E2D885-6F61-E511-B760-02163E01204B.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/868/00000/982183E5-7661-E511-A1AB-02163E0142BD.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/868/00000/DC2E2C55-6761-E511-A397-02163E014267.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/868/00000/8ED3869D-8161-E511-AD2F-02163E01461B.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/868/00000/AED3B9FB-7061-E511-B2B6-02163E012811.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/869/00000/280A4091-1D61-E511-B2DD-02163E0133F8.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/924/00000/C2B2B504-6061-E511-87CA-02163E0142FF.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/925/00000/F2EBDD8D-6861-E511-B19E-02163E014162.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/926/00000/96C386CB-7367-E511-A69D-02163E01439E.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/935/00000/98077753-9D61-E511-AEC7-02163E013436.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/936/00000/021B117F-E661-E511-8E0B-02163E01458F.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/936/00000/9C94657A-EC61-E511-AEAB-02163E014104.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/936/00000/DE0D3D0E-0762-E511-BFA8-02163E01414F.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/936/00000/1451560C-EA61-E511-BBCA-02163E014309.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/936/00000/AA640762-D561-E511-8C85-02163E0142D6.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/936/00000/E05F89F0-D961-E511-B147-02163E01383C.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/936/00000/4E99AD32-EA61-E511-B945-02163E01427D.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/936/00000/BC5C54B8-E161-E511-B804-02163E0138BA.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/936/00000/EABAA79A-F261-E511-B8B4-02163E013450.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/936/00000/58F937C8-E261-E511-A4AC-02163E011A9B.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/936/00000/D076D362-E461-E511-86B7-02163E013512.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/936/00000/F0AB8215-D861-E511-9D63-02163E0145E8.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/936/00000/76BD5CCA-FC61-E511-B060-02163E014160.root",
                               "root://eoscms.cern.ch//eos/cms/store/data/Run2015D/DoubleEG/RAW-RECO/ZElectron-PromptReco-v3/000/256/936/00000/D6D3CA73-DD61-E511-B6AA-02163E01471E.root",
#                               "root://eoscms.cern.ch//eos/cms/store/user/fmeng/0008202C-E78F-E211-AADB-0026189437FD.root"
                               # 'root://cms-xrd-global.cern.ch//store/data/Run2015C/DoubleEG/RAW-RECO/ZElectron-PromptReco-v1/000/254/094/00000/D41B4DB2-CA45-E511-844E-02163E013805.root'
                         #    "root://eoscms.cern.ch//eos/cms/store/user/fmeng/256676_raw_reco.root" 
                          #   '/store/data/Run2012A/DoubleElectron/RAW-RECO/ZElectron-13Jul2012-v1/00000/00663DF3-28DA-E111-966A-848F69FD4508.root'
#                              'root://cms-xrd-global.cern.ch//store/data/Run2012A/DoubleElectron/RAW-RECO/ZElectron-13Jul2012-v1/00000/C87DD084-4CDA-E111-B86C-00266CF25C20.root'     
                            ), 
        )                        
process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

# ---------------------------------------------------------------------
# Ouptut File
# ---------------------------------------------------------------------
process.TFileService = cms.Service ("TFileService", 
                                    fileName = cms.string ("tree_testFastSim_TPG_checknow_2015D_14_10_2.root")
                                    )
process.Out = cms.OutputModule("PoolOutputModule",
    # outputCommands = cms.untracked.vstring('keep *_*SuperCluster*_*_*'),
     outputCommands = cms.untracked.vstring('keep *_*_*_skimAllPathsFilter'),
   #  outputCommands = cms.untracked.vstring('keep *'),
     fileName = cms.untracked.string('turnonoutput.root')
)
from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *

process.e = cms.EndPath(process.Out)                                       
#process.runSelection = cms.EDFilter("RunSelect",
#    requireNoTimeScan = cms.untracked.bool(True) ,
#    requireCollidingBX = cms.untracked.bool(False),
  #  requireNoLumiScan = cms.untracked.bool(False),
#    requireNoLumiScan = cms.untracked.bool(True),
#    debug = cms.untracked.bool(False)
#    )

## # ---------------------------------------------------------------------
## # Skim ALL Path Filter
## # ---------------------------------------------------------------------
## #load the EDfilter to select just skim data
## process.load("EGamma.LLRSkim.skimAllPathsFilter_cfi")
## from EGamma.LLRSkim.skimAllPathsFilter_cfi import *
## process.skimAllPathsFilter = skimAllPathsFilter.clone()

## # Nadir TagAndProbe : at least 2 ele with eT>5 and at least 1 ele passing eleID==VBTF95
## process.skimAllPathsFilter.mode = "TP_nadir"
## process.skimAllPathsFilter.eleID= "VBTF95"


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
#-----process.load("RecoVertex.Configuration.RecoVertex_cff")
#-----process.load("RecoTracker.IterativeTracking.InitialStep_cff")
#from RecoVertex.Configuration.RecoVertex_cff import *
#process.vertexreco = cms.Sequence(offlinePrimaryVertices*offlinePrimaryVerticesWithBS)

from RecoVertex.Configuration.RecoVertex_cff import *
#process.vertexreco = cms.Sequence(offlinePrimaryVertices*offlinePrimaryVerticesWithBS)

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

#process.load("EGamma.ECGelec.NtupleProducer_cfi")
#from EGamma.ECGelec.NtupleProducer_cfi import *
#process.produceNtuple = produceNtuple.clone()

#-------------process.load("EGamma.ECGelec.NtupleProducer_custom_cfi")
#-------------from EGamma.ECGelec.NtupleProducer_custom_cfi import *
#-------------process.produceNtuple = produceNtupleCustom.clone()
#process.load("EGamma.ECGelec.NtupleProducer_EleL1Study_cfi")
#from EGamma.ECGelec.NtupleProducer_EleL1Study_cfi import *

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

#process.produceNtuple.eleVetoIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V2-standalone-veto")
#process.produceNtuple.eleLooseIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V2-standalone-loose")
#process.produceNtuple.eleMediumIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V2-standalone-medium")
#process.produceNtuple.eleTightIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-50ns-V2-standalone-tight")
process.produceNtuple.eleVetoIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-veto")
process.produceNtuple.eleLooseIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-loose")
process.produceNtuple.eleMediumIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-medium")
process.produceNtuple.eleTightIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-tight")


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
#    process.skimAllPathsFilter +   
    process.kt6PFJets + 
    
#    process.runSelection +
    
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
#process.schedule = cms.Schedule( process.p, process.output_step)
process.schedule = cms.Schedule( process.p)
#process.schedule = cms.Schedule( process.p,process.e)
#pythonDump = open("dump_cfg.py", "write")
#print >> pythonDump, process.dumpPython()

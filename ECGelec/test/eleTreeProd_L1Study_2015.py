import FWCore.ParameterSet.Config as cms
process = cms.Process("electronTreeProducer")
# Messages
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

###Define all the input options here!
try:
    IsMC
except NameError:
    IsMC = False  #default







# ---------------------------------------------------------------------
# Standard configuration
# ---------------------------------------------------------------------
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load("Configuration.StandardSequences.Services_cff")
#process.load("Configuration.StandardSequences.GeometryPilot2_cff")
#process.load("Configuration.Geometry.GeometryPilot2_cff")
process.load('Configuration.Geometry.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
#process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")
process.load("Geometry.CaloEventSetup.EcalTrigTowerConstituents_cfi")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load("Geometry.CaloEventSetup.EcalTrigTowerConstituents_cfi")
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
#process.load("TrackingTools.TrackAssociator.default_cfi") #ROKO: For new TT matching
#process.load("TrackingTools.TrackAssociator.DetIdAssociatorESProducer_cff")  #ROKO: For new TT matching


# ---------------------------------------------------------------------
# Global Tag
# ---------------------------------------------------------------------

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = '74X_dataRun2_Prompt_v1'
#if bool(options.isMC) and options.globaltag=='NOT_PROVIDED':
  #my_global_tag='XXXXXXXXXXXXXX'
#elif options.globaltag=='NOT_PROVIDED':
  #print "Not MC and no global tag provided. Using default"
  #my_global_tag='GR_P_V42_AN2'
#else:
  #my_global_tag=options.globaltag
  
#print options.globaltag
#process.GlobalTag.globaltag = my_global_tag+'::All'




# ---------------------------------------------------------------------
# Input File
# ---------------------------------------------------------------------
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100))
#SkipEvent = cms.untracked.vstring('ProductNotFound')

# Make the job crash in case of missing product
process.options = cms.untracked.PSet( Rethrow = cms.untracked.vstring('ProductNotFound') )

process.source = cms.Source("PoolSource",
                            #eventsToProcess = cms.untracked.VEventRange(
                            #'172163:379096687','172163:379447575'),
                            #debugFlag = cms.untracked.bool(True),
                            #debugVebosity = cms.untracked.uint32(10),
                            #fileNames = cms.untracked.vstring(
    ##DATA#
    ##'rfio:/dpm/in2p3.fr/home/cms/trivcat/store/data/Run2012A/DoubleElectron/AOD/PromptReco-v1/000/191/856/8AF8C900-D98C-E111-9B33-003048D2C1C4.root'
    ##'/store/data/Run2012A/DoubleElectron/AOD/PromptReco-v1/000/191/856/8AF8C900-D98C-E111-9B33-003048D2C1C4.root'
    #'file://test_RAW-RECO.root'
    ##"file:///home/llr/cms/plestina/cmssw/slhcOutputFromData.root"
    #),
     fileNames = cms.untracked.vstring(
                                       
                                       #"/store/data/Run2012D/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/000/203/773/00000/803C0546-3A0C-E211-A580-002354EF3BDF.root"
                                       #"file:/data_CMS/cms/plestina/TempSamples/test_RAW-RECO.root"
                                       #"/store/data/Run2012D/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/000/203/773/00000/803C0546-3A0C-E211-A580-002354EF3BDF.root"
 #                                      "/store/data/Run2012D/DoubleElectron/RAW-RECO/ZElectron-PromptSkim-v1/000/206/745/00000/4E471D19-992A-E211-A960-003048678D52.root"
#    "/store/data/Run2012B/DoubleElectron/RAW-RECO/ZElectron-22Jan2013-v1/20000/001CE462-D169-E211-B1C6-002618943939.root"

#        'root://cms-xrd-global.cern.ch//store/data/Run2015C/DoubleEG/RAW-RECO/ZElectron-PromptReco-v1/000/254/094/00000/D41B4DB2-CA45-E511-844E-02163E013805.root',
                          "root://eoscms.cern.ch//eos/cms/store/user/fmeng/256676_raw_reco.root"
#           "root://eoscms.cern.ch//eos/cms/store/user/fmeng/254879_collison.root"
#        'root://cms-xrd-global.cern.ch//store/data/Run2015C/DoubleEG_0T/RAW-RECO/ZElectron-PromptReco-v3/000/256/268/00000/AA9A01ED-6859-E511-BADF-02163E014670.root',
                                       ),
    		        lumisToProcess = cms.untracked.VLuminosityBlockRange()

                            )


# ---------------------------------------------------------------------
# Ouptut File
# ---------------------------------------------------------------------
process.TFileService = cms.Service ("TFileService", 
                                    fileName = cms.string ("tree_L1_new.root"),
			      closeFileFast = cms.untracked.bool(True)

               
               )
               
                                    

from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *


# ---------------------------------------------------------------------
# Skim
# ---------------------------------------------------------------------
process.load("EGamma.LLRSkim.skimAllPathsFilter_cfi")
from EGamma.LLRSkim.skimAllPathsFilter_cfi import *
process.skimAllPathsFilter = skimAllPathsFilter.clone()

# TagAndProbe : >1 electrons (eT>5) && >0 electron passing VBTF95 Id+Iso cuts
process.skimAllPathsFilter.mode = "TP_nadir"
process.skimAllPathsFilter.eleID= "VBTF95"


# ---------------------------------------------------------------------
# HLT Filter
# ---------------------------------------------------------------------
import HLTrigger.HLTfilters.hltHighLevel_cfi
process.MyHLTSelection = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone(
    ## SingleElectron paths
    #HLTPaths = [ 'HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v3',
                 #'HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2',
                 #'HLT_Ele45_CaloIdVT_TrkIdT_v3'
                 #],
    ## DoubleElectron paths
    HLTPaths = cms.vstring(
    'HLT_Ele12_CaloIdL_TrackIdL_IsoVL_v',
##         'HLT_Ele17_CaloIdL_CaloIsoVL_v*',
##         'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Jet30_v*',
##         'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*',
##         'HLT_Ele23_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_HFT30_v*', 
##         'HLT_Ele27_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele15_CaloIdT_CaloIsoVL_trackless_v*', 
##         'HLT_Ele27_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_HFT15_v*', 
##         'HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_SC17_Mass50_v*',
##         'HLT_Ele8_CaloIdL_CaloIsoVL_v*',
##         'HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Jet30_v*',
##         'HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*',
##         'HLT_Ele8_CaloIdT_TrkIdVL_EG7_v*',
##         'HLT_Ele8_CaloIdT_TrkIdVL_Jet30_v*',
##         'HLT_Ele8_CaloIdT_TrkIdVL_v*',
##         'HLT_Ele20_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC4_Mass50_v*',
##         'HLT_L1SingleEG*',
##         'HLT_Activity_Ecal_SC*'
        ),
    
    #HLTPaths = [ 'HLT_Ele17_CaloIdL_CaloIsoVL_v*',
    #             'HLT_Ele8_CaloIdL_CaloIsoVL_v*',
    #             'HLT_Ele8_CaloIdL_TrkIdVL_v*',
    #             'HLT_Ele8_v*'
    #             ],

    ## ZeroBias
    #HLTPaths = [ 'HLT_ZeroBias*'] ,
    #'HLT_L1SingleEG5_*' ],
    
    throw = False
    #dont throw except on unknown path name
    )
        



# ---------------------------------------------------------------------
# Produce Ntuple Module
# ---------------------------------------------------------------------
process.load("EGamma.ECGelec.NtupleProducer_EleL1Study_cfi")
from EGamma.ECGelec.NtupleProducer_EleL1Study_cfi import *
process.produceNtuple = produceNtupleL1Study.clone()

## Get L1 candidates
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

#from TrackingTools.TrackAssociator.default_cfi import TrackAssociatorParameters
#process.produceNtuple.TrackAssociatorParameters = TrackAssociatorParameters #ROKO: added for matching 



## HLT tag and paths to check
HLT_name = 'HLT'
process.produceNtuple.HLTTag = 'TriggerResults::' + HLT_name
process.produceNtuple.HLTElePaths = cms.vstring(
    'HLT_Ele12_CaloIdL_TrackIdL_IsoVL_v',
##         'HLT_Ele17_CaloIdL_CaloIsoVL_v',
##         'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Jet30_v',
##         'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v',
##         'HLT_Ele23_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_HFT30_v', 
##         'HLT_Ele27_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele15_CaloIdT_CaloIsoVL_trackless_v', 
##         'HLT_Ele27_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_HFT15_v', 
##         'HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_SC17_Mass50_v',
##         'HLT_Ele8_CaloIdL_CaloIsoVL_v',
##         'HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Jet30_v',
##         'HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v',
##         'HLT_Ele8_CaloIdT_TrkIdVL_EG7_v',
##         'HLT_Ele8_CaloIdT_TrkIdVL_Jet30_v',
##         'HLT_Ele8_CaloIdT_TrkIdVL_v',
##         'HLT_Ele20_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC4_Mass50_v' 
##     #'HLT_DoubleEle17_SW_L1R_v',
##     #'HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v'
    )
process.produceNtuple.HLT_paths = cms.vstring(
    'HLT_Ele12_CaloIdL_TrackIdL_IsoVL_v',
##     'HLT_Ele17_CaloIdL_CaloIsoVL_v',
##     'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Jet30_v',
##     'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v',
##     'HLT_Ele23_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_HFT30_v', 
##     'HLT_Ele27_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele15_CaloIdT_CaloIsoVL_trackless_v', 
##     'HLT_Ele27_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_HFT15_v', 
##     'HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_SC17_Mass50_v',
##     'HLT_Ele8_CaloIdL_CaloIsoVL_v',
##     'HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Jet30_v',
##     'HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v',
##     'HLT_Ele8_CaloIdT_TrkIdVL_EG7_v',
##     'HLT_Ele8_CaloIdT_TrkIdVL_Jet30_v',
##     'HLT_Ele8_CaloIdT_TrkIdVL_v',
##     'HLT_Ele20_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC4_Mass50_v',
##     'HLT_Activity_Ecal_SC',
##     'HLT_L1SingleEG5'
##     #'HLT_L1SingleEG12',
##     #'HLT_DoubleEle'
    )


# ---------------------------------------------------------------------
# Vertexing DA
# ---------------------------------------------------------------------
#process.load("RecoVertex.Configuration.RecoVertex_cff")
from RecoVertex.Configuration.RecoVertex_cff import *
#process.vertexreco = cms.Sequence(offlinePrimaryVertices*offlinePrimaryVerticesWithBS)


# ---------------------------------------------------------------------
# Run selection
# ---------------------------------------------------------------------
process.runSelection = cms.EDFilter("RunSelect",
    requireNoTimeScan = cms.untracked.bool(True) ,
    requireCollidingBX = cms.untracked.bool(False),
    requireNoLumiScan = cms.untracked.bool(False),
    debug = cms.untracked.bool(False)
    )




##unpacking
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
#process.load("SLHCUpgradeSimulations.L1CaloTrigger.SLHCCaloTrigger_cff")
process.load("L1Trigger.L1ExtraFromDigis.l1extraParticles_cfi")

##CALO TRIGGER CONFIGURATION OVERRIDE
#process.load("L1TriggerConfig.RCTConfigProducers.L1RCTConfig_cff")
#process.RCTConfigProducers.eMaxForHoECut = cms.double(60.0)
#process.RCTConfigProducers.hOeCut = cms.double(0.05)
#process.RCTConfigProducers.eGammaECalScaleFactors = cms.vdouble(1.0, 1.01, 1.02, 1.02, 1.02,
                                                      #1.06, 1.04, 1.04, 1.05, 1.09,
                                                      #1.1, 1.1, 1.15, 1.2, 1.27,
                                                      #1.29, 1.32, 1.52, 1.52, 1.48,
                                                      #1.4, 1.32, 1.26, 1.21, 1.17,
                                                      #1.15, 1.15, 1.15)
#process.RCTConfigProducers.eMinForHoECut = cms.double(3.0)
#process.RCTConfigProducers.hActivityCut = cms.double(4.0)
#process.RCTConfigProducers.eActivityCut = cms.double(4.0)
#process.RCTConfigProducers.jetMETHCalScaleFactors = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0,
                                                                #1.0, 1.0, 1.0, 1.0, 1.0,
                                                                #1.0, 1.0, 1.0, 1.0, 1.0,
                                                                #1.0, 1.0, 1.0, 1.0, 1.0,
                                                                #1.0, 1.0, 1.0, 1.0, 1.0,
                                                                #1.0, 1.0, 1.0)
#process.RCTConfigProducers.eicIsolationThreshold = cms.uint32(6)
#process.RCTConfigProducers.etMETLSB = cms.double(0.25)
#process.RCTConfigProducers.jetMETECalScaleFactors = cms.vdouble(1.0, 1.0, 1.0, 1.0, 1.0,
                                                                #1.0, 1.0, 1.0, 1.0, 1.0,
                                                                #1.0, 1.0, 1.0, 1.0, 1.0,
                                                                #1.0, 1.0, 1.0, 1.0, 1.0,
                                                                #1.0, 1.0, 1.0, 1.0, 1.0,
                                                                #1.0, 1.0, 1.0)
#process.RCTConfigProducers.eMinForFGCut = cms.double(100.0)
#process.RCTConfigProducers.eGammaLSB = cms.double(0.25)



# ---------------------------------------------------------------------
# Sequence PATH
# ---------------------------------------------------------------------
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))
process.p = cms.Path (
#    process.RawToDigi+
###    process.SLHCCaloTrigger+
#    process.l1extraParticles+
#    process.MyHLTSelection +
 #   process.vertexreco + # ---> to recompute Vertex 
    process.skimAllPathsFilter +   ###---> skim on electrons 
#    process.runSelection +
    process.produceNtuple 
    )


process.produceNtuple.GetL1_SLHC = cms.untracked.bool(False)

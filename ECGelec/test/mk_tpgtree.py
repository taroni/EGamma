import FWCore.ParameterSet.Config as cms

process = cms.Process("electronTreeProducer")

# import of standard configurations


process.load("SimCalorimetry.EcalTrigPrimProducers.ecalTriggerPrimitiveDigis_readDBOffline_cff")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.StandardSequences.Services_cff")
process.load('Configuration.Geometry.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

process.GlobalTag.globaltag = '%globaltag%'
#process.prefer("GlobalTag")

HLT_name = 'HLT'




# ---------------------------------------------------------------------
# Unpack Ecal Digis
# ---------------------------------------------------------------------
process.load("EventFilter.EcalRawToDigi.EcalUnpackerMapping_cfi");
process.load("EventFilter.EcalRawToDigi.EcalUnpackerData_cfi");
#process.ecalEBunpacker.InputLabel = cms.InputTag('rawDataCollector');

# ECAL TPG Producer                                                                                                                                                                
process.load("Geometry.EcalMapping.EcalMapping_cfi")
process.load("Geometry.EcalMapping.EcalMappingRecord_cfi")
#process.load("CalibCalorimetry.Configuration.Ecal_FakeConditions_cff")                                                                                                            

# ECAL TPG Analyzer                                                                                                                                                                
#process.load("Geometry.CaloEventSetup.CaloGeometry_cfi")
process.load("Geometry.CaloEventSetup.EcalTrigTowerConstituents_cfi")
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")

process.ecalTriggerPrimitiveDigis = cms.EDProducer("EcalTrigPrimProducer",
   InstanceEB = cms.string('ebDigis'),
   InstanceEE = cms.string('eeDigis'),
   Label = cms.string('ecalDigis'),

   BarrelOnly = cms.bool(False),
   Famos = cms.bool(False),
   TcpOutput = cms.bool(False),

   Debug = cms.bool(False),

   binOfMaximum = cms.int32(6), ## optional from release 200 on, from 1-10                                                                   

)



# ---------------------------------------------------------------------
# Simulate Ecal Trigger Primitives
# ---------------------------------------------------------------------
# Config from file
#process.load('SimCalorimetry.EcalTrigPrimProducers.ecalTrigPrimESProducer_cff')
process.load('SimCalorimetry.EcalTrigPrimProducers.ecalTrigPrimESProducer_cff')
#process.EcalTrigPrimESProducer.DatabaseFile = 'TPG_beamv6_trans_spikekill_255999_2015_18_22.txt.gz'
process.EcalTrigPrimESProducer.DatabaseFile = 'TPG_beamv6_trans_255999_spikekill_2015.txt.gz'

process.load("SimCalorimetry.EcalTrigPrimProducers.ecalTriggerPrimitiveDigis_cff")
process.simEcalTriggerPrimitiveDigis.Label = 'ecalDigis'
#process.simEcalTriggerPrimitiveDigis.Label = 'ecalEBunpacker'
process.simEcalTriggerPrimitiveDigis.InstanceEB =  'ebDigis'
process.simEcalTriggerPrimitiveDigis.InstanceEE =  'eeDigis'
process.simEcalTriggerPrimitiveDigis.BarrelOnly = False






# ----------------------------------------------------------------------
# ECAL rechits and co                                                   
# ----------------------------------------------------------------------                                                                                                           
#process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load("Configuration/StandardSequences/Reconstruction_cff")
import RecoLocalCalo.EcalRecProducers.ecalGlobalUncalibRecHit_cfi
process.ecalUncalibHit = RecoLocalCalo.EcalRecProducers.ecalGlobalUncalibRecHit_cfi.ecalGlobalUncalibRecHit.clone()
process.load("RecoLocalCalo.EcalRecProducers.ecalRecHit_cfi")
#process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load("RecoLocalCalo.EcalRecProducers.ecalDetIdToBeRecovered_cfi")
process.ecalRecHit.EBuncalibRecHitCollection = 'ecalUncalibHit:EcalUncalibRecHitsEB'
process.ecalRecHit.EEuncalibRecHitCollection = 'ecalUncalibHit:EcalUncalibRecHitsEE'


# ---------------------------------------------------------------------
# Input Files
# ---------------------------------------------------------------------
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring('file:%filename%'
        #'root://xrootd-cms.infn.it//store/data/Run2015D/ZeroBias/RAW/v1/000/258/175/00000/269ED683-2B6A-E511-B17B-02163E013663.root'
                            ),                         
                        )

process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

# ---------------------------------------------------------------------
# ouptut File
# ---------------------------------------------------------------------
process.TFileService = cms.Service ("TFileService", 
                                    fileName = cms.string ("tree.root")
                                    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(%numevents%)
                                        #SkipEvent = cms.untracked.vstring('ProductNotFound')
                                        )


from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *

                                       
process.runSelection = cms.EDFilter("RunSelect",
    requireNoTimeScan = cms.untracked.bool(True) ,
    requireCollidingBX = cms.untracked.bool(False),
    requireNoLumiScan = cms.untracked.bool(False),
    debug = cms.untracked.bool(False)
    )



process.load("EGamma.ECGelec.NtupleProducer_SpikeStudy_cfi")
from EGamma.ECGelec.NtupleProducer_SpikeStudy_cfi import *
process.produceNtuple = produceNtupleSpikeStudy.clone()



process.produceNtuple.NadL1M = cms.untracked.bool(True)
process.produceNtuple.NadTP = cms.untracked.bool(True)
process.produceNtuple.NadTPemul = cms.untracked.bool(True) # Need to put True when running Emulator !!
process.produceNtuple.NadTPmodif = cms.untracked.bool(False)
process.produceNtuple.PrintDebug = cms.untracked.bool(False)
process.produceNtuple.type = 'DATA'
process.produceNtuple.AOD = cms.untracked.bool(False)
process.produceNtuple.FillSC = cms.untracked.bool(True)
process.produceNtuple.functionName = cms.string("EcalClusterEnergyUncertainty")



process.load("Configuration.StandardSequences.RawToDigi_Data_cff")

# ---------------------------------------------------------------------
# Simulate Ecal Trigger Primitives
# ---------------------------------------------------------------------
process.load('Configuration.StandardSequences.SimL1Emulator_cff') 
process.load('L1Trigger.Configuration.L1Trigger_EventContent_cff')

# emulator trigger
process.simRctDigis.ecalDigis = cms.VInputTag(cms.InputTag("simEcalTriggerPrimitiveDigis"))
process.simRctDigis.hcalDigis = cms.VInputTag(cms.InputTag("hcalDigis"))
process.simGctDigis.inputLabel = cms.InputTag("simRctDigis")


#process.load('Configuration.StandardSequences.L1Reco_cff')

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
# Save all event content in separate file (for debug purposes)
# ---------------------------------------------------------------------
# Output definition

process.SpecialEventContent = cms.PSet(
#         outputCommands = cms.untracked.vstring('drop *'),
        outputCommands = cms.untracked.vstring('keep *'),
        splitLevel = cms.untracked.int32(0)
       )


#
process.p = cms.Path (
#    process.ecalEBunpacker +
    process.ecalDigis +
    process.simEcalTriggerPrimitiveDigis +
    process.simRctDigis +
    process.simGctDigis +
    process.simGtDigis +
    process.l1extraParticles +
    process.l1extraParticlesOnline +  
    process.ecalUncalibHit+
    process.ecalDetIdToBeRecovered+
    process.ecalRecHit+
    process.produceNtuple
    )
process.schedule = cms.Schedule(#process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,
    process.p)#, process.output_step)


# customisation of the process.

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.postLS1Customs
from SLHCUpgradeSimulations.Configuration.postLS1Customs import customisePostLS1_lowPU 

#call to customisation function customisePostLS1 imported from SLHCUpgradeSimulations.Configuration.postLS1Customs
process = customisePostLS1_lowPU(process)

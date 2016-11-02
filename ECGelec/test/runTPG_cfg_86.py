import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
from Configuration.StandardSequences.Eras import eras

process = cms.Process("electronTreeProducer",eras.Run2_2016)

# import of standard configurations


process.load("SimCalorimetry.EcalTrigPrimProducers.ecalTriggerPrimitiveDigis_readDBOffline_cff")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.StandardSequences.Services_cff")
process.load('Configuration.Geometry.GeometryExtended2016Reco_cff')
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")


# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')


#process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )


HLT_name = 'HLT'

process.GlobalTag.toGet = cms.VPSet( #sFGVB thresholds.
           cms.PSet(record = cms.string("EcalTPGFineGrainStripEERcd"),
           tag = cms.string("EcalTPGFineGrainStrip_16"),
           connect =cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
            ),
          cms.PSet(record = cms.string("EcalTPGSpikeRcd"),
          tag = cms.string("EcalTPGSpike_22"),
         connect =cms.string('frontier://FrontierPrep/CMS_CONDITIONS')
                   )
           )


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
process.load('SimCalorimetry.EcalTrigPrimProducers.ecalTrigPrimESProducer_cff')
process.EcalTrigPrimESProducer.DatabaseFile = 'TPG_beamv6_trans_spikekill_255999_2015_18_22.txt.gz'
process.load("SimCalorimetry.EcalTrigPrimProducers.ecalTriggerPrimitiveDigis_cff")
process.simEcalTriggerPrimitiveDigis.Label = 'ecalDigis'
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
                           fileNames = cms.untracked.vstring('file:/afs/cern.ch/work/n/nancy/private/EcalL1/CMSSW_8_0_3/src/MyRECOSplahes_reducedEVTCONT.root'
                           # fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/n/ndev/public/for_nancy/MyRECO_inRECO.root'
                            ),                         
                        )

process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

# ---------------------------------------------------------------------
# ouptut File
# ---------------------------------------------------------------------
process.TFileService = cms.Service ("TFileService", 
                                    fileName = cms.string ("tree.root")
                                    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1)
                                        #SkipEvent = cms.untracked.vstring('ProductNotFound')
                                        )


from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *

                                       
process.runSelection = cms.EDFilter("RunSelect",
    requireNoTimeScan = cms.untracked.bool(True) ,
    requireCollidingBX = cms.untracked.bool(False),
    requireNoLumiScan = cms.untracked.bool(False),
    debug = cms.untracked.bool(False)
    )


#from EventFilter.L1TRawToDigi.caloStage2Digis_cfi import caloStage2Digis
#process.rawCaloStage2Digis = caloStage2Digis.clone()
#process.rawCaloStage2Digis.InputLabel = cms.InputTag('rawDataCollector')



process.load("EGamma.ECGelec.NtupleProducer_custom_cfi")
from EGamma.ECGelec.NtupleProducer_custom_cfi import *
process.produceNtuple = produceNtupleCustom.clone()




process.produceNtuple.NadL1M = cms.untracked.bool(True)
process.produceNtuple.NadTP = cms.untracked.bool(True)
process.produceNtuple.NadTPemul = cms.untracked.bool(True) # Need to put True when running Emulator !!
process.produceNtuple.NadTPmodif = cms.untracked.bool(False)
process.produceNtuple.PrintDebug = cms.untracked.bool(True)
process.produceNtuple.type_data = cms.untracked.bool(True)
process.produceNtuple.AOD = cms.untracked.bool(False)
process.produceNtuple.FillSC = cms.untracked.bool(True)
process.produceNtuple.functionName = cms.string("EcalClusterEnergyUncertainty")





# ---------------------------------------------------------------------
# Simulate Ecal Trigger Primitives
# ---------------------------------------------------------------------
process.load('Configuration.StandardSequences.SimL1Emulator_cff') 
process.load('L1Trigger.Configuration.L1Trigger_EventContent_cff')

# emulator trigger
#process.simRctDigis.ecalDigis = cms.VInputTag(cms.InputTag("simEcalTriggerPrimitiveDigis"))
#process.simRctDigis.hcalDigis = cms.VInputTag(cms.InputTag("hcalDigis"))
#process.simGctDigis.inputLabel = cms.InputTag("simRctDigis")


#process.load('Configuration.StandardSequences.L1Reco_cff')

# L1 extra for the re-simulated candidates
#process.l1extraParticles = cms.EDProducer("L1ExtraParticlesProd",
#                                          muonSource = cms.InputTag("gtDigis"),
#                                          etTotalSource = cms.InputTag("simGctDigis"),
#                                          nonIsolatedEmSource = cms.InputTag("simGctDigis","nonIsoEm"),
#                                          etMissSource = cms.InputTag("simGctDigis"),
#                                          htMissSource = cms.InputTag("simGctDigis"),
#                                          produceMuonParticles = cms.bool(False),
#                                          forwardJetSource = cms.InputTag("simGctDigis","forJets"),
#                                          centralJetSource = cms.InputTag("simGctDigis","cenJets"),
#                                          produceCaloParticles = cms.bool(True),
#                                          tauJetSource = cms.InputTag("simGctDigis","tauJets"),
#                                          isoTauJetSource = cms.InputTag("simGctDigis","isoTauJets"),
#                                          isolatedEmSource = cms.InputTag("simGctDigis","isoEm"),
#                                          etHadSource = cms.InputTag("simGctDigis"),
#                                          hfRingEtSumsSource = cms.InputTag("simGctDigis"),
#                                          hfRingBitCountsSource = cms.InputTag("simGctDigis"),
#                                          centralBxOnly = cms.bool(True),
#                                          ignoreHtMiss = cms.bool(False)
#                                          )


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
# JETS
# ---------------------------------------------------------------------
# JPT
#process.load('RecoJets.Configuration.RecoJPTJets_cff')
process.load('RecoJets.Configuration.RecoPFJets_cff')

# ---------------------------------------------------------------------
# Fast Jet Rho Correction
# ---------------------------------------------------------------------
process.load('RecoJets.JetProducers.kt4PFJets_cfi')
process.kt6PFJets = process.kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
process.kt6PFJets.Rho_EtaMax = cms.double(2.5)
# ---------------------------------------------------------------------
# Produce Ntuple Module
# ---------------------------------------------------------------------



# ---------------------------------------------------------------------
# Save all event content in separate file (for debug purposes)
# ---------------------------------------------------------------------
# Output definition

process.SpecialEventContent = cms.PSet(
#         outputCommands = cms.untracked.vstring('drop *'),
        outputCommands = cms.untracked.vstring('keep *'),
        splitLevel = cms.untracked.int32(0)
       )

# Trigger Stuff
process.produceNtuple.HLTTag          = 'TriggerResults::' + HLT_name
process.produceNtuple.TriggerEventTag = 'hltTriggerSummaryAOD::' + HLT_name
process.produceNtuple.HLTElePaths = cms.vstring(
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

process.produceNtuple.eleVetoIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-veto")
process.produceNtuple.eleLooseIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-loose")
process.produceNtuple.eleMediumIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-medium")
process.produceNtuple.eleTightIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-tight")


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


process.raw2digi_step = cms.Path(process.RawToDigi)
process.schedule = cms.Schedule(process.raw2digi_step)


# Automatic addition of the customisation function from L1Trigger.Configuration.customiseReEmul
from L1Trigger.Configuration.customiseReEmul import L1TReEmulFromRAW
process = L1TReEmulFromRAW(process)



process.p = cms.Path (

    process.ecalEBunpacker +
    process.ecalDigis + 
    process.simEcalTriggerPrimitiveDigis +
#    process.rawCaloStage2Digis   +
#    process.ecalTriggerPrimitiveDigis + This is in the input file 
### these are no longer needed in stage 2 
#    process.simRctDigis +
#    process.simGctDigis +
#    process.simGtDigis +
#    process.l1extraParticles +
    process.l1extraParticlesOnline +  
#    process.ecalUncalibHit+   This is in the input file
#    process.ecalDetIdToBeRecovered+
#     process.ecalRecHit+ + This is in the input file 
    process.egmGsfElectronIDSequence +
#    process.kt6PFJets +
    process.produceNtuple
    )




process.schedule.append(process.p)





#process.schedule = cms.Schedule(#process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,
#    process.p)#, process.output_step)


# customisation of the process.


# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.postLS1Customs
from SLHCUpgradeSimulations.Configuration.postLS1Customs import customisePostLS1_lowPU 

#call to customisation function customisePostLS1 imported from SLHCUpgradeSimulations.Configuration.postLS1Customs
process = customisePostLS1_lowPU(process)


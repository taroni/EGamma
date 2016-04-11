import FWCore.ParameterSet.Config as cms

produceNtupleNab = cms.EDAnalyzer("SpikeStudy",
                                     ## Custom Nab

                                     # boolean parameters
                                     NadL1M = cms.untracked.bool(False),
                                     PrintDebug = cms.untracked.bool(False),

                                     # to get the trigger primitives
                                     TPCollectionNormal = cms.InputTag("ecalDigis","EcalTriggerPrimitives"),
#                                     TPCollectionNormal = cms.InputTag("ecalEBunpacker","EcalTriggerPrimitives"),
                                     TPCollectionModif  = cms.InputTag("zeroedEcalTrigPrimDigis"),
                                     TPEmulatorCollection  = cms.InputTag("simEcalTriggerPrimitiveDigis"),


                                     #rechit
                                     EcalRecHitCollectionEB = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
                                     EcalRecHitCollectionEE = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
                                     uncalibratedRecHitCollectionEB = cms.InputTag("ecalMaxSampleUncalibRecHit","EcalUncalibRecHitsEB"),
                                     uncalibratedRecHitCollectionEE = cms.InputTag("ecalMaxSampleUncalibRecHit","EcalUncalibRecHitsEE"),


                                     
                                     # to get the L1 algorithms bits
                                     GTRecordCollection = cms.string('gtDigis'),
                                     #				 VerticesTag = cms.InputTag("offlinePrimaryVertices"),
                                     VerticesTag = cms.InputTag("offlinePrimaryVerticesWithBS"),

                                     # Trigger Stuff
                                     HLTTag          = cms.InputTag("TriggerResults","","HLT"),
                                     TriggerEventTag = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
                                     HLTElePaths     = cms.vstring("HLT_Photon15_L1R"),
                                     HLTMuonPaths    = cms.vstring("HLT_Mu9"),
                                     HLTFilters      = cms.VInputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt10HcalIsolFilter::HLT"),
                                     # 
                                     dcsTag = cms.untracked.InputTag("scalersRawToDigi"),                                 
                                     type = cms.string("DATA"),
                                     AOD = cms.untracked.bool(False),
                                     Pileupsrc = cms.InputTag("addPileupInfo"),
                                     simulation = cms.untracked.bool(False),
                                     FillSC = cms.untracked.bool(True),
                                     hcalTowers = cms.InputTag("towerMaker"),
                                     hOverEPtMin = cms.double(0.), 
                                     hOverEConeSize = cms.double(0.15),
                                     useBeamSpot = cms.bool(False)                        
       )


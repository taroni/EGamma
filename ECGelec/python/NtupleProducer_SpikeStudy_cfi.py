import FWCore.ParameterSet.Config as cms

produceNtupleSpikeStudy = cms.EDAnalyzer("SpikeStudy",
                                     ## Custom Nadir

                                     # boolean parameters
                                     L1M = cms.untracked.bool(True),
                                     TP = cms.untracked.bool(True),
                                     TPmodif = cms.untracked.bool(False),
                                     TPemul = cms.untracked.bool(True),
                                     PrintDebug = cms.untracked.bool(True),


                                     # to get the trigger primitives
                                     TPCollectionNormal = cms.InputTag("ecalDigis","EcalTriggerPrimitives"),
                                     #  TPCollectionNormal = cms.InputTag("ecalEBunpacker","EcalTriggerPrimitives"),
                                     
                                     TPEmulatorCollection  = cms.InputTag("simEcalTriggerPrimitiveDigis"),
                                     
                                     GTRecordCollection = cms.string('gtDigis'),                                   

                                     #rechit
                                     EcalRecHitCollectionEB = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
                                     EcalRecHitCollectionEE = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
                                     uncalibratedRecHitCollectionEB = cms.InputTag("ecalMaxSampleUncalibRecHit","EcalUncalibRecHitsEB"),
                                     uncalibratedRecHitCollectionEE = cms.InputTag("ecalMaxSampleUncalibRecHit","EcalUncalibRecHitsEE"),

                                     TPCollectionModif  = cms.InputTag("zeroedEcalTrigPrimDigis"),
                                     VerticesTag = cms.InputTag("offlinePrimaryVerticesWithBS"),
                                     MCTag = cms.InputTag("generator"),

                                     ## Trigger Stuff
                                     # 
                                     dcsTag = cms.untracked.InputTag("scalersRawToDigi"),                                 
                                     type = cms.string("DATA"),
                                     AOD = cms.untracked.bool(False),
                                     #Pileupsrc = cms.InputTag("addPileupInfo"),
                                     simulation = cms.untracked.bool(True),
                                     FillSC = cms.untracked.bool(True),
                                     hcalTowers = cms.InputTag("towerMaker"),
                                     hOverEPtMin = cms.double(0.), 
                                     hOverEConeSize = cms.double(0.15),
                                     useBeamSpot = cms.bool(False)                        
       )


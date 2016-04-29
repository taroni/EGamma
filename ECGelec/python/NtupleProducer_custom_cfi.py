import FWCore.ParameterSet.Config as cms

produceNtupleCustom = cms.EDAnalyzer("SimpleNtpleCustom",
                                     ## Custom Nadir

                                     # boolean parameters
                                     NadL1M = cms.untracked.bool(True),
                                     NadTP = cms.untracked.bool(True),
                                     NadTPmodif = cms.untracked.bool(False),
                                     NadTPemul = cms.untracked.bool(True),
                                     PrintDebug = cms.untracked.bool(True),

                                     # to get the RecHits --> spikes
                                     #EcalRecHitCollectionEB = cms.InputTag("ecalRecHit","EcalRecHitsEB"),

                                     # to get the trigger primitives
                                     TPCollectionNormal = cms.InputTag("ecalDigis","EcalTriggerPrimitives"),
                                     #  TPCollectionNormal = cms.InputTag("ecalEBunpacker","EcalTriggerPrimitives"),
                                     
                                     TPEmulatorCollection  = cms.InputTag("simEcalTriggerPrimitiveDigis"),
                                     #TPEmulatorCollection  = cms.InputTag("ecalTriggerPrimitiveDigis"),
                                     
                                     GTRecordCollection = cms.string('gtDigis'),                                   

                                     #rechit
                                     EcalRecHitCollectionEB = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
                                     EcalRecHitCollectionEE = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
                                     uncalibratedRecHitCollectionEB = cms.InputTag("ecalMaxSampleUncalibRecHit","EcalUncalibRecHitsEB"),
                                     uncalibratedRecHitCollectionEE = cms.InputTag("ecalMaxSampleUncalibRecHit","EcalUncalibRecHitsEE"),

                                     TPCollectionModif  = cms.InputTag("zeroedEcalTrigPrimDigis"),
                                     
                                     emNonisolCollToken = cms.InputTag("l1extraParticlesOnline","NonIsolated"),
                                     emIsolCollToken = cms.InputTag("l1extraParticlesOnline","Isolated"),
                                     
#                                     l1extraIsol = cms.InputTag("l1extraParticles","Isolated"),
#                                     l1extraonlineIsol=cms.InputTag("l1extraParticlesOnline","Isolated"),



                                     #HCAL TPG

                                     #TP_HCAL = cms.InputTag("hcalDigis","HcalTriggerPrimitives"),
                                     
                                     # to get the L1 algorithms bits
                                     # GTRecordCollectionM = cms.string('simGtDigis'),                                     
                                     

                                     
                                     #GTRecordCollectionM = cms.string('gtDigis'),
                                      
                                     eleIso_TdrHzzTkMapTag = cms.InputTag("TdrHzzIsolationProducer:Tk"),
                                     eleIso_TdrHzzHcalMapTag= cms.InputTag("TdrHzzIsolationProducer:Hcal"),
                                     eleIso_Eg4HzzTkMapTag   = cms.InputTag("eleIsoFromDepsTkOptimized"),
                                     eleIso_Eg4HzzEcalMapTag = cms.InputTag("eleIsoFromDepsEcalFromHitsByCrystalOptimized"),
                                     eleIso_Eg4HzzHcalMapTag = cms.InputTag("eleIsoFromDepsHcalFromTowersOptimized"),

                                     eleVetoIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-veto"),
                                     eleLooseIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-loose"),
                                     eleMediumIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-medium"),
                                     eleTightIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-tight"),
                                     
                                     EleTag = cms.InputTag("gedGsfElectrons"),
                                     # EleTag = cms.InputTag("gsfElectrons"),
                                     MuonTag = cms.InputTag("muons"),
                                     MuonIso_HzzMapTag = cms.InputTag("MuonHzzIsolationProducer"),
                                     MuonIsoTk_HzzMapTag = cms.InputTag("MuonHzzIsolationProducer:Tk"),
                                     MuonIsoEcal_HzzMapTag = cms.InputTag("MuonHzzIsolationProducer:Ecal"),
                                     MuonIsoHcal_HzzMapTag = cms.InputTag("MuonHzzIsolationProducer:Hcal"),
                                     CaloJetTag = cms.InputTag("ak5CaloJets"),
                                     JPTJetTag = cms.InputTag("JetPlusTrackZSPCorJetAntiKt5"),
                                     PFJetTag = cms.InputTag("ak5PFJets"),
                                     #				 VerticesTag = cms.InputTag("offlinePrimaryVertices"),
                                     VerticesTag = cms.InputTag("offlinePrimaryVerticesWithBS"),
                                     SeedTag = cms.InputTag("electronMergedSeeds"),
                                     MCTag = cms.InputTag("generator"),
                                     TkPTag = cms.InputTag("mergedtruth:MergedTrackTruth:readseeds"),
                                     ##
                                     kt6=cms.InputTag("kt6PFJets:rho"),
                                     kt6sigma=cms.InputTag("kt6PFJets:sigma"),
                                     Pileupsrc = cms.InputTag("addPileupInfo"),
                                     ## Trigger Stuff
                                     keep_trigger = cms.bool(False),                        
                                     HLTTag          = cms.InputTag("TriggerResults","","HLT"),
                                     TriggerEventTag = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
                                     HLTElePaths     = cms.vstring("HLT_Photon15_L1R"),
                                     HLTMuonPaths    = cms.vstring("HLT_Mu9"),
                                     HLTFilters      = cms.VInputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt10HcalIsolFilter::HLT"),
                                     hltTriggerSummaryAOD = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
                                     # 
                                     dcsTag = cms.untracked.InputTag("scalersRawToDigi"),                                 
                                     type = cms.string("DATA"),
                                     AOD = cms.untracked.bool(False),
                                     simulation = cms.untracked.bool(True),
                                     FillSC = cms.untracked.bool(True),
                                     hcalTowers = cms.InputTag("towerMaker"),
                                     hOverEPtMin = cms.double(0.), 
                                     hOverEConeSize = cms.double(0.15),
                                     useBeamSpot = cms.bool(False),
                                     towerToken = cms.InputTag("caloStage2Digis","CaloTower"),
                                     mpEGToken = cms.InputTag("caloStage2Digis", "MP"),
                                     egToken = cms.InputTag("caloStage2Digis","EGamma"),
#                                     towerToken = cms.InputTag("rawCaloStage2Digis"),
#                                     mpEGToken = cms.InputTag("rawCaloStage2Digis", "MP"),
#                                     egToken = cms.InputTag("rawCaloStage2Digis","EGamma"),
                                     towerToken_emul = cms.InputTag("simCaloStage2Layer1Digis"),
                                     clusterToken_emul = cms.InputTag("simCaloStage2Digis", "MP"),
				     mpEGToken_emul = cms.InputTag("simCaloStage2Digis","MP"),
				     egToken_emul = cms.InputTag("simCaloStage2Digis"),
                                     trigger_stage2_emul = cms.bool(True)
                        
       )


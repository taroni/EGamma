import FWCore.ParameterSet.Config as cms

produceNtupleCustom = cms.EDAnalyzer("SimpleNtpleCustom",
                                     ## Custom Nadir

                                     # boolean parameters
                                     NadL1M = cms.untracked.bool(False),
                                     NadTP = cms.untracked.bool(False),
                                     NadTPmodif = cms.untracked.bool(False),
                                     NadTPemul = cms.untracked.bool(False),
                                     PrintDebug = cms.untracked.bool(False),

                                     # to get the RecHits --> spikes
                                     #EcalRecHitCollectionEB = cms.InputTag("ecalRecHit","EcalRecHitsEB"),

                                     # to get the trigger primitives
                                     TPCollectionNormal = cms.InputTag("ecalDigis","EcalTriggerPrimitives"),
                                     TPCollectionModif  = cms.InputTag("zeroedEcalTrigPrimDigis"),
                                     TPEmulatorCollection  = cms.InputTag("simEcalTriggerPrimitiveDigis"),

                                     # HCAL TPG
                                     #TP_HCAL = cms.InputTag("hcalDigis","HcalTriggerPrimitives"),
                                     
                                     # to get the L1 algorithms bits
                                     GTRecordCollection = cms.string('gtDigis'),
                                     #GTRecordCollectionM = cms.string('simGtDigis'),
                                     #GTRecordCollectionM = cms.string('gtDigis'),
                                      
                                     eleID_VeryLooseTag = cms.InputTag("eidVeryLoose"),
                                     eleID_LooseTag = cms.InputTag("eidLoose"),
                                     eleID_MediumTag = cms.InputTag("eidMedium"),
                                     eleID_TightTag = cms.InputTag("eidTight"),
                                     eleID_SuperTightTag = cms.InputTag("eidSuperTight"),
                                     eleID_HyperTight1Tag = cms.InputTag("eidHyperTight1"),
                                     #in the new optimization there is just 2 values of hypertighness
                                     eleID_HyperTight2Tag = cms.InputTag("eidHyperTight1"),	
                                     eleID_HyperTight3Tag = cms.InputTag("eidHyperTight1"),
                                     eleID_HyperTight4Tag = cms.InputTag("eidHyperTight1"),
                                     eleIso_TdrHzzTkMapTag = cms.InputTag("TdrHzzIsolationProducer:Tk"),
                                     eleIso_TdrHzzHcalMapTag= cms.InputTag("TdrHzzIsolationProducer:Hcal"),
                                     eleIso_Eg4HzzTkMapTag   = cms.InputTag("eleIsoFromDepsTkOptimized"),
                                     eleIso_Eg4HzzEcalMapTag = cms.InputTag("eleIsoFromDepsEcalFromHitsByCrystalOptimized"),
                                     eleIso_Eg4HzzHcalMapTag = cms.InputTag("eleIsoFromDepsHcalFromTowersOptimized"),  
                                     EleTag = cms.InputTag("gsfElectrons"),
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
                                     # Trigger Stuff
                                     HLTTag          = cms.InputTag("TriggerResults","","HLT"),
                                     TriggerEventTag = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
                                     HLTElePaths     = cms.vstring("HLT_Photon15_L1R"),
                                     HLTMuonPaths    = cms.vstring("HLT_Mu9"),
                                     HLTFilters      = cms.VInputTag("hltL1NonIsoHLTNonIsoSinglePhotonEt10HcalIsolFilter::HLT"),
                                     # 
                                     dcsTag = cms.untracked.InputTag("scalersRawToDigi"),                                 
                                     type = cms.string("signalType"),
                                     AOD = cms.untracked.bool(False),
                                     #Pileupsrc = cms.InputTag("addPileupInfo"),
                                     simulation = cms.untracked.bool(False),
                                     FillSC = cms.untracked.bool(False),
                                     hcalTowers = cms.InputTag("towerMaker"),
                                     hOverEPtMin = cms.double(0.), 
                                     hOverEConeSize = cms.double(0.15),
                                     useBeamSpot = cms.bool(False)                        
       )


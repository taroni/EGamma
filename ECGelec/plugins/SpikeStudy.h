

// C++
#include <memory>
#include <iostream>

// ROOT
#include "TTree.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "TParticle.h"
#include "TVector3.h"

// CMSSW
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "CondFormats/EcalObjects/interface/EcalChannelStatus.h"

#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"

#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/Records/interface/CaloTopologyRecord.h"
#include "Geometry/CaloTopology/interface/EcalTrigTowerConstituentsMap.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"


#include "RecoEgamma/EgammaElectronAlgos/interface/ElectronHcalHelper.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterFunctionBaseClass.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"

#include "TrackingTools/GsfTools/interface/MultiTrajectoryStateTransform.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"

// Pile UP
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h" 
// Vertices
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "RecoVertex/PrimaryVertexProducer/interface/PrimaryVertexSorter.h"
// Trigger
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
// L1 Trigger
#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
#include "DataFormats/L1Trigger/interface/L1ParticleMapFwd.h"
#include "DataFormats/L1Trigger/interface/L1ParticleMap.h"
#include "L1Trigger/L1ExtraFromDigis/interface/L1ExtraParticleMapProd.h"
//rechit
#include "DataFormats/EcalRecHit/interface/EcalUncalibratedRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

// RCT
#include "CondFormats/L1TObjects/interface/L1RCTChannelMask.h"
#include "CondFormats/DataRecord/interface/L1RCTChannelMaskRcd.h"
// TPG
#include "CondFormats/DataRecord/interface/EcalTPGTowerStatusRcd.h"
#include "CondFormats/EcalObjects/interface/EcalTPGTowerStatus.h"
#include "CondFormats/DataRecord/interface/EcalTPGCrystalStatusRcd.h"
#include "CondFormats/EcalObjects/interface/EcalTPGCrystalStatus.h"
// TPG (Nadir study)
#include "DataFormats/EcalDigi/interface/EcalTriggerPrimitiveDigi.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
// Electron/SuperCluster
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidateFwd.h"
#include "DataFormats/EgammaReco/interface/ElectronSeed.h"
#include "DataFormats/EgammaReco/interface/ElectronSeedFwd.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h"
// PF electron
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
// For Photon Iso
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "RecoEgamma/PhotonIdentification/interface/PhotonIsolationCalculator.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/PhotonTkIsolation.h"
// Muons
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
// Calo Jets
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/JetCollection.h"
#include "DataFormats/JetReco/interface/Jet.h"
// JPT Jets
#include "DataFormats/JetReco/interface/JPTJet.h"
// PF Jets
#include "DataFormats/JetReco/interface/PFJet.h"
// MET
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETFwd.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METFwd.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"

// TrackingParticles
#include "FWCore/Framework/interface/EventSetupRecordImplementation.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
//#include "SimTracker/TrackAssociation/interface/TrackAssociatorBase.h"
#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"
//#include "SimTracker/TrackAssociation/interface/TrackAssociatorByChi2.h"
//#include "SimTracker/TrackAssociation/interface/TrackAssociatorByHits.h"
#include "DataFormats/RecoCandidate/interface/TrackAssociation.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"
#include <vector>

//Clusters 
#include "DataFormats/EgammaReco/interface/ElectronSeedFwd.h"
#include "DataFormats/EgammaReco/interface/ElectronSeed.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/CaloRecHit/interface/CaloClusterFwd.h"
#include "DataFormats/CaloRecHit/interface/CaloCluster.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgoRcd.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterFunctionFactory.h" 
// For H/E - Iso on SC
#include "RecoEgamma/EgammaIsolationAlgos/interface/EgammaHcalIsolation.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/EgammaTowerIsolation.h"
#include "RecoEgamma/EgammaElectronAlgos/interface/ElectronHcalHelper.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/EgammaRecHitIsolation.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/ElectronTkIsolation.h"
// For Skim
#include "EGamma/ECGelec/interface/AnalysisUtils.h"
//
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "RecoEgamma/EgammaTools/interface/ConversionFinder.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "CLHEP/Units/GlobalPhysicalConstants.h"

#include "TLorentzVector.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

#include "TrackingTools/GsfTools/interface/MultiTrajectoryStateMode.h"
// Transient tracks
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "RecoTracker/Record/interface/TrackerRecoGeometryRecord.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

#include "TrackingTools/GeomPropagators/interface/AnalyticalPropagator.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/PatternTools/interface/TransverseImpactPointExtrapolator.h"
#include "TrackingTools/PatternTools/interface/TSCPBuilderNoMaterial.h"
#include "TrackingTools/GsfTools/interface/GSUtilities.h"
#include "TrackingTools/GsfTools/interface/GsfPropagatorAdapter.h"
#include "TrackingTools/GsfTools/interface/GaussianSumUtilities1D.h"
#include "TrackingTools/GsfTools/interface/MultiTrajectoryStateTransform.h"
#include "TrackingTools/GsfTools/interface/MultiGaussianStateTransform.h"
#include "TrackingTools/GsfTools/interface/MultiGaussianState1D.h"
//
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/MaterialEffects/interface/PropagatorWithMaterial.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"

// Other specific
#include "RecoVertex/PrimaryVertexProducer/interface/PrimaryVertexSorter.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/Measurement1D.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
//nab

#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

class CaloSubdetectorGeometry ;
/// among the includes ///
class MultiTrajectoryStateMode ;
// for H/E
class EgammaTowerIsolation ;
// Auxiliary class                                                                                                                                                                   
class towerEner {
 public:
  float eRec_ ;
  float maxRechit_;
  int crystNb_ ;
  int tpgEmul_[5] ;
  int tpgEmulFlag_[5] ;
  int tpgEmulsFGVB_[5] ;
  int tpgADC_;
  int iphi_, ieta_, nbXtal_, spike_ ;
  int twrADC_, sFGVB_, sevlv_, ttFlag_, sevlv2_,rechit_cleaning_cut_;
  towerEner()
    : eRec_(0), maxRechit_(0), crystNb_(0), tpgADC_(0),
    iphi_(-999), ieta_(-999), nbXtal_(0), spike_(0), twrADC_(0), sFGVB_(999), sevlv_(0) , ttFlag_(0),sevlv2_(0),rechit_cleaning_cut_(0)
    {
      for (int i=0 ; i<5 ; i ++) {
	tpgEmul_[i] = 0 ;
	tpgEmulFlag_[i]=0;
	tpgEmulsFGVB_[i]=0;
      }
    }
};

//
// class declaration
//

class SpikeStudy : public edm::EDAnalyzer {
 public:
  explicit SpikeStudy(const edm::ParameterSet&);
  ~SpikeStudy();
	
  typedef math::XYZTLorentzVector LorentzVector ;
  typedef edm::View<reco::Track> trackCollection ;
	
 private:
  virtual void beginJob(const edm::ParameterSet& conf) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
	
  void Init();
	
  void FillEvent (const edm::Event&, const edm::EventSetup&);
  void FillTpData (const edm::Event&, const edm::EventSetup&);
  void FillSuperClusters(const edm::Event&, const edm::EventSetup&);
		
  void setMomentum (TLorentzVector &myvector, const LorentzVector & mom) ;
  bool IsConv (const reco::GsfElectron & eleRef);
  // For Trigger
  int getGCTRegionPhi(int ttphi) ; //modif-alex
  int getGCTRegionEta(int tteta) ; //modif-alex
  edm::ESHandle<CaloGeometry> theCaloGeom_;
  // e2overe9 new anti-spike variable steph
  float E2overE9( const DetId , const EcalRecHitCollection & , float , float ,  bool , bool);
  float recHitE( const DetId , const EcalRecHitCollection & );
  float recHitE( const DetId , const EcalRecHitCollection & ,  int , int );
  float recHitApproxEt( const DetId , const EcalRecHitCollection &);
  // end e2overe9

  // ----------member data ---------------------------
  ElectronHcalHelper * hcalhelper_;
	
  TTree *mytree_;
	
  int nEvent, nRun, nLumi;

  //nab
  // tower variables                                                                                                                                                             
  uint _nbOfTowers ; //max 4032 EB+EE                                                                                                                                             
  int _ieta[4032] ;
  int _iphi[4032] ;
  int _nbOfXtals[4032] ;
  int _rawTPData[4032] ;
  int _rawTPEmul1[4032] ;
  int _rawTPEmul2[4032] ;
  int _rawTPEmul3[4032] ;
  int _rawTPEmul4[4032] ;
  int _rawTPEmul5[4032] ;
  int _rawTPEmulttFlag1[4032] ;
  int _rawTPEmulttFlag2[4032] ;
  int _rawTPEmulttFlag3[4032] ;
  int _rawTPEmulttFlag4[4032] ;
  int _rawTPEmulttFlag5[4032] ;
  int _rawTPEmulsFGVB1[4032] ;
  int _rawTPEmulsFGVB2[4032] ;
  int _rawTPEmulsFGVB3[4032] ;
  int _rawTPEmulsFGVB4[4032] ;
  int _rawTPEmulsFGVB5[4032] ;
  float _eRec[4032] ;
  float _maxRechit[4032];
  int _crystNb[4032];
  int _sevlv[4032];
  int _sevlv2[4032];
  int _spike[4032] ;
  int _ttFlag[4032];
  int _sFGVB[4032];
  int _twrADC[4032];
  int _rechit_cleaning_cut[4032];
  // Vertices
  int _vtx_N;
  double _vtx_x[50], _vtx_y[50], _vtx_z[50];
  double _vtx_normalizedChi2[50], _vtx_ndof[50], _vtx_nTracks[50], _vtx_d0[50];

  //Pile-up
  int _PU_N;
  double _PU_rho, _PU_sigma;  //corrections from FastJets



  //rechits with bad crystals (sevlv 3 or 4 crystals)
  int _n_bad_crystals,_erec_Et_sevlv3_4[4032];
  double _erec_eta_sevlv3_4[4032],_erec_phi_sevlv3_4[4032],_erec_theta_sevlv3_4[4032];
 
 //all rechits
  float  _all_rechits_time[8064]; 
  int  _num_all_rechits,_all_rechits_Et[8064];
  double _all_rechits_eta[8064],_all_rechits_phi[8064],_all_rechits_theta[8064];

   //all intime rechits abs(time)<15
  int  _num_intime_rechits,_intime_rechits_Et[4032];
  double _intime_rechits_eta[4032],_intime_rechits_phi[4032],_intime_rechits_theta[4032];

  //all intime rechits abs(time)<15 with severity level 3 or 4
  int  _num_intime_rechits_sevlv3_4,_intime_rechits_sevlv3_4_Et[4032];
  double _intime_rechits_sevlv3_4_eta[4032],_intime_rechits_sevlv3_4_phi[4032],_intime_rechits_sevlv3_4_theta[4032];

  // L1
  int _trig_L1emIso_N; 
  int _trig_L1emNonIso_N;
  int _trig_L1emIso_ieta[4], _trig_L1emIso_iphi[4], _trig_L1emIso_rank[4]; 
  double _trig_L1emIso_eta[4], _trig_L1emIso_phi[4],_trig_L1emIso_energy[4],_trig_L1emIso_et[4]; 
  int _trig_L1emNonIso_ieta[4], _trig_L1emNonIso_iphi[4],_trig_L1emNonIso_rank[4];
  double _trig_L1emNonIso_eta[4], _trig_L1emNonIso_phi[4], _trig_L1emNonIso_energy[4],_trig_L1emNonIso_et[4];

  // L1 modif
  int _trig_L1emIso_N_M; 
  int _trig_L1emNonIso_N_M;
  int _trig_L1emIso_ieta_M[4], _trig_L1emIso_iphi_M[4], _trig_L1emIso_rank_M[4]; 
  double _trig_L1emIso_eta_M[4], _trig_L1emIso_phi_M[4],_trig_L1emIso_energy_M[4],_trig_L1emIso_et_M[4]; 
  int _trig_L1emNonIso_ieta_M[4], _trig_L1emNonIso_iphi_M[4],_trig_L1emNonIso_rank_M[4];
  double _trig_L1emNonIso_eta_M[4], _trig_L1emNonIso_phi_M[4], _trig_L1emNonIso_energy_M[4],_trig_L1emNonIso_et_M[4];

  // L1 prefiring
  int _trig_preL1emIso_N; 
  int _trig_preL1emNonIso_N;
  int _trig_preL1emIso_ieta[4], _trig_preL1emIso_iphi[4], _trig_preL1emIso_rank[4]; 
  int _trig_preL1emNonIso_ieta[4], _trig_preL1emNonIso_iphi[4],_trig_preL1emNonIso_rank[4];
  // L1 postfiring
  int _trig_postL1emIso_N; 
  int _trig_postL1emNonIso_N;
  int _trig_postL1emIso_ieta[4], _trig_postL1emIso_iphi[4], _trig_postL1emIso_rank[4]; 
  int _trig_postL1emNonIso_ieta[4], _trig_postL1emNonIso_iphi[4],_trig_postL1emNonIso_rank[4];
	
  int _trig_nMaskedRCT, _trig_nMaskedCh;
  int _trig_iMaskedRCTeta[100], _trig_iMaskedRCTphi[100], _trig_iMaskedRCTcrate[100], _trig_iMaskedTTeta[100], _trig_iMaskedTTphi[100];

	
  // Beam Spot
  double BS_x, BS_y, BS_z, BS_dydz, BS_dxdz, BS_dz, BS_bw_x, BS_bw_y;
  GlobalPoint vertexPosition;

  // MC Properties
  double _MC_pthat;
  int _MC_flavor[2];


  const MultiTrajectoryStateTransform *mtsTransform_;

  edm::ESHandle<MagneticField> theMagField;
  edm::ESHandle<TrackerGeometry> trackerHandle_;

  unsigned long long cacheIDTDGeom_;
  unsigned long long cacheIDMagField_;


  // SuperClusters
  //converter::SuperClusterToCandidate * sc_struct;
  // SC EB
  int _sc_hybrid_N; 
  double _sc_hybrid_E[25], _sc_hybrid_Et[25], _sc_hybrid_Eta[25], _sc_hybrid_Phi[25]; 
  int _sc_hybrid_outOfTimeSeed[25],_sc_hybrid_severityLevelSeed[25];
  double _sc_hybrid_e1[25], _sc_hybrid_e33[25];
  double _sc_hybrid_he[25], _sc_hybrid_sigmaietaieta[25];
  double _sc_hybrid_hcalDepth1TowerSumEt_dr03[25], _sc_hybrid_hcalDepth2TowerSumEt_dr03[25];
  double _sc_hybrid_ecalRecHitSumEt_dr03[25];
  double _sc_hybrid_trkiso_dr03[25];

  int _sc_hybrid_RCTeta[25];
  int _sc_hybrid_RCTphi[25];
  int _sc_hybrid_RCTL1iso[25];
  int _sc_hybrid_RCTL1noniso[25];
  int _sc_hybrid_TTetaVect[25][50], _sc_hybrid_TTphiVect[25][50];
  double _sc_hybrid_TTetVect[25][50];
  int _sc_hybrid_RCTetaVect[25][10], _sc_hybrid_RCTphiVect[25][10], _sc_hybrid_RCTL1isoVect[25][10], _sc_hybrid_RCTL1nonisoVect[25][10];
  double _sc_hybrid_RCTetVect[25][10];

  // SC EE
  int _sc_multi55_N; 
  double _sc_multi55_E[25], _sc_multi55_Et[25], _sc_multi55_Eta[25], _sc_multi55_Phi[25];
  double _sc_multi55_he[25], _sc_multi55_sigmaietaieta[25]; 
  double _sc_multi55_hcalDepth1TowerSumEt_dr03[25], _sc_multi55_hcalDepth2TowerSumEt_dr03[25];
  double _sc_multi55_ecalRecHitSumEt_dr03[25];
  double _sc_multi55_trkiso_dr03[25];

  int _sc_multi55_RCTeta[25];
  int _sc_multi55_RCTphi[25];
  int _sc_multi55_RCTL1iso[25];
  int _sc_multi55_RCTL1noniso[25];
  int _sc_multi55_TTetaVect[25][50], _sc_multi55_TTphiVect[25][50];
  double _sc_multi55_TTetVect[25][50];
  int _sc_multi55_RCTetaVect[25][10], _sc_multi55_RCTphiVect[25][10], _sc_multi55_RCTL1isoVect[25][10], _sc_multi55_RCTL1nonisoVect[25][10];
  double _sc_multi55_RCTetVect[25][10];

  const CaloSubdetectorGeometry * theEndcapGeometry_ ;
  const CaloSubdetectorGeometry * theBarrelGeometry_ ;
  edm::ESHandle<EcalTrigTowerConstituentsMap> eTTmap_;


  // for H/E
  edm::Handle<CaloTowerCollection> * towersH_ ;
  edm::InputTag hcalTowers_ ;
  EgammaTowerIsolation * towerIso1_ ;
  EgammaTowerIsolation * towerIso2_ ;
  double hOverEConeSize_ ;
  double hOverEPtMin_ ;            

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////    NADIR STUFF ////////
  ////////////////////////////////

  // Booleans
  bool GetL1M_ ; // true => get two collections of L1 candidates (standard / "cleaned") ; false => get only the standard one
  bool GetTP_ ; // true => get the standard trigger primitives
  bool GetTP_Modif_ ; // true => get the modified collection of trigger primitives (zeroing by hand)
  bool GetTP_Emul_ ; // true => get the emulated collection of trigger primitives (Jackson-Zabi's sFGVB+zeroing emulator)
  bool PrintDebug_ ;
  bool keeptrigger_;

  // tags
  edm::InputTag tpCollectionNormal_ ;
  edm::InputTag tpCollectionModif_ ;
  edm::InputTag tpEmulatorCollection_ ;

  edm::InputTag EcalRecHitCollectionEB_ ;
  edm::InputTag EcalRecHitCollectionEE_ ;
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  edm::InputTag VerticesTag_;
  edm::InputTag dcsTag_;
	

  //Pile-up
  edm::InputTag PileupSrc_;


  std::string type_;	
  bool aod_;	
  bool simulation_;
  bool fillsc_;

  const EcalRecHit getRecHit(DetId id, const EcalRecHitCollection *recHits);
	
  std::string gtRecordCollectionTag_ ;
  EcalClusterFunctionBaseClass* funcbase_;
  std::string funcname_;
  bool useBeamSpot_ ;
};

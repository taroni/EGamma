#include "EGamma/ECGelec/plugins/SpikeStudy.h"

using namespace std;
using namespace reco;
using namespace edm;
using namespace IPTools;
//using namespace math;
class CaloSubdetectorGeometry;
// ====================================================================================
SpikeStudy::SpikeStudy(const edm::ParameterSet& iConfig) :


  hcalTowers_ (iConfig.getParameter<edm::InputTag>("hcalTowers")),
  PrintDebug_ (iConfig.getUntrackedParameter<bool>("PrintDebug")),
  tpCollectionNormal_ (iConfig.getParameter<edm::InputTag> ("TPCollectionNormal") ),
  tpCollectionModif_ (iConfig.getParameter<edm::InputTag> ("TPCollectionModif") ),
  tpEmulatorCollection_ (iConfig.getParameter<edm::InputTag> ("TPEmulatorCollection") ),

  EcalRecHitCollectionEB_ (iConfig.getParameter<edm::InputTag>("EcalRecHitCollectionEB") ) ,
  EcalRecHitCollectionEE_ (iConfig.getParameter<edm::InputTag>("EcalRecHitCollectionEE")) ,

  VerticesTag_(iConfig.getParameter<edm::InputTag> ("VerticesTag")),
  dcsTag_ (iConfig.getUntrackedParameter<edm::InputTag>("dcsTag")),
  GetL1M_ (iConfig.getUntrackedParameter<bool>("L1M")),
  // Trigger Stuff
  PileupSrc_ ("addPileupInfo"),
  type_ (iConfig.getParameter<std::string>("type")),
  aod_ (iConfig.getUntrackedParameter<bool>("AOD")),
  funcname_  (iConfig.getParameter<std::string>("functionName")),
  useBeamSpot_ (iConfig.getParameter<bool>("useBeamSpot"))
				    //
				    //,"addPileupInfo::REDIGI311X"))
				    //
				    // ====================================================================================
{



  //now do what ever initialization is needed
  funcbase_ = EcalClusterFunctionFactory::get()->create( funcname_, iConfig ); 
  gtRecordCollectionTag_ = iConfig.getParameter<std::string>("GTRecordCollection") ;
	
	
  simulation_ = iConfig.getUntrackedParameter<bool>("simulation", true);
  fillsc_     = iConfig.getUntrackedParameter<bool>("FillSC", false);
  //std::cout << "Filling super cluster quantities? " << fillsc_<< std::endl;
  if(PrintDebug_) std::cout << "Creating TFileService..." << std::endl;

  edm::Service<TFileService> fs ;
  mytree_  = fs->make <TTree>("SimpleTree","SimpleTree"); 
	
  // Global
  mytree_->Branch("nEvent",&nEvent,"nEvent/I");
  mytree_->Branch("nRun",&nRun,"nRun/I");
  mytree_->Branch("nLumi",&nLumi,"nLumi/I");
	
  // Pile UP
  mytree_->Branch("PU_N",&_PU_N,"PU_N/I");
  mytree_->Branch("PU_rhoCorr",&_PU_rho,"PU_rhoCorr/D");
  mytree_->Branch("PU_sigmaCorr",&_PU_sigma,"PU_sigmaCorr/D");

  // Vertices
  mytree_->Branch("vtx_N",&_vtx_N,"vtx_N/I");
  mytree_->Branch("vtx_normalizedChi2",&_vtx_normalizedChi2,"vtx_normalizedChi2[35]/D");
  mytree_->Branch("vtx_ndof",&_vtx_ndof,"vtx_ndof[35]/D");
  mytree_->Branch("vtx_nTracks",&_vtx_nTracks,"vtx_nTracks[35]/D");
  mytree_->Branch("vtx_d0",&_vtx_d0,"vtx_d0[35]/D");
  mytree_->Branch("vtx_x",&_vtx_x,"vtx_x[35]/D");
  mytree_->Branch("vtx_y",&_vtx_y,"vtx_y[35]/D");
  mytree_->Branch("vtx_z",&_vtx_z,"vtx_z[35]/D");
	
		
  //rechits with bad (sev_level=3,4) crystals
  mytree_->Branch("n_bad_crystals", &_n_bad_crystals, "n_bad_crystals/I");
  mytree_->Branch("erec_eta_sevlv3_4", &_erec_eta_sevlv3_4, "erec_eta_sevlv3_4[n_bad_crystals]/D");
  mytree_->Branch("erec_Et_sevlv3_4", &_erec_Et_sevlv3_4, "erec_Et_sevlv3_4[n_bad_crystals]/I");
  mytree_->Branch("erec_phi_sevlv3_4", &_erec_phi_sevlv3_4, "erec_phi_sevlv3_4[n_bad_crystals]/D");
  mytree_->Branch("erec_theta_sevlv3_4", &_erec_theta_sevlv3_4, "erec_theta_sevlv3_4[n_bad_crystals]/D");


  //all rechits

  mytree_->Branch("num_all_rechits", &_num_all_rechits, "num_all_rechits/I");
  mytree_->Branch("all_rechits_time", &_all_rechits_time, "all_rechits_time[num_all_rechits]/F");
  mytree_->Branch("all_rechits_eta", &_all_rechits_eta, "all_rechits_eta[num_all_rechits]/D");
  mytree_->Branch("all_rechits_Et", &_all_rechits_Et, "all_rechits_Et[num_all_rechits]/I");
  mytree_->Branch("all_rechits_theta", &_all_rechits_theta, "all_rechits_theta[num_all_rechits]/D");
  mytree_->Branch("all_rechits_phi", &_all_rechits_phi, "all_rechits_phi[num_all_rechits]/D");


  //intime rechits: abs(time)<15
  mytree_->Branch("num_intime_rechits", &_num_intime_rechits, "num_intime_rechits/I");
  mytree_->Branch("intime_rechits_eta", &_intime_rechits_eta, "intime_rechits_eta[num_intime_rechits]/D");
  mytree_->Branch("intime_rechits_Et", &_intime_rechits_Et, "intime_rechits_Et[num_intime_rechits]/I");
  mytree_->Branch("intime_rechits_theta", &_intime_rechits_theta, "intime_rechits_theta[num_intime_rechits]/D");
  mytree_->Branch("intime_rechits_phi", &_intime_rechits_phi, "intime_rechits_phi[num_intime_rechits]/D");


  //intime rechits: abs(time)<15 with severity level 3 or 4
  mytree_->Branch("num_intime_rechits_sevlv3_4", &_num_intime_rechits_sevlv3_4, "num_intime_rechits_sevlv3_4/I");
  mytree_->Branch("intime_rechits_sevlv3_4_eta", &_intime_rechits_sevlv3_4_eta, "intime_rechits_sevlv3_4_eta[num_intime_rechits_sevlv3_4]/D");
  mytree_->Branch("intime_rechits_sevlv3_4_Et", &_intime_rechits_sevlv3_4_Et, "intime_rechits_sevlv3_4_Et[num_intime_rechits_sevlv3_4]/I");
  mytree_->Branch("intime_rechits_sevlv3_4_theta", &_intime_rechits_sevlv3_4_theta, "intime_rechits_sevlv3_4_theta[num_intime_rechits_sevlv3_4]/D");
  mytree_->Branch("intime_rechits_sevlv3_4_phi", &_intime_rechits_sevlv3_4_phi, "intime_rechits_sevlv3_4_phi[num_intime_rechits_sevlv3_4]/D");
 



  //nab

  mytree_->Branch("nbOfTowers",&_nbOfTowers,"nbOfTowers/i");


  mytree_->Branch("ieta", &_ieta,"ieta[nbOfTowers]/I");
  mytree_->Branch("iphi", &_iphi,"iphi[nbOfTowers]/I");
  mytree_->Branch("nbOfXtals", &_nbOfXtals,"nbOfXtals[nbOfTowers]/I");
  mytree_->Branch("rawTPData", &_rawTPData,"rawTPData[nbOfTowers]/I");
  mytree_->Branch("rawTPEmul1", &_rawTPEmul1,"rawTPEmul1[nbOfTowers]/I");
  mytree_->Branch("rawTPEmul2", &_rawTPEmul2,"rawTPEmul2[nbOfTowers]/I");
  mytree_->Branch("rawTPEmul3", &_rawTPEmul3,"rawTPEmul3[nbOfTowers]/I");
  mytree_->Branch("rawTPEmul4", &_rawTPEmul4,"rawTPEmul4[nbOfTowers]/I");
  mytree_->Branch("rawTPEmul5", &_rawTPEmul5,"rawTPEmul5[nbOfTowers]/I");
  mytree_->Branch("crystNb", &_crystNb,"crystNb[nbOfTowers]/I");
  mytree_->Branch("maxRechit", &_maxRechit,"maxRechit[nbOfTowers]/F");
  mytree_->Branch("eRec", &_eRec,"eRec[nbOfTowers]/F");
  mytree_->Branch("ttFlag", &_ttFlag,"ttFlag[nbOfTowers]/I");
  mytree_->Branch("sevlv", &_sevlv,"sevlv[nbOfTowers]/I");
  mytree_->Branch("sevlv2", &_sevlv2,"sevlv2[nbOfTowers]/I");
  mytree_->Branch("rechit_cleaning_cut", &_rechit_cleaning_cut,"rechit_cleaning_cut[nbOfTowers]/I");
  mytree_->Branch("twrADC", &_twrADC,"twrADC[nbOfTowers]/I");
  mytree_->Branch("spike", &_spike,"spike[nbOfTowers]/I");
  mytree_->Branch("sFVGB", &_sFGVB,"sFGVB[nbOfTowers]/I");
  mytree_->Branch("rawTPEmulsFGVB1", &_rawTPEmulsFGVB1,"rawTPEmulsFGVB1[nbOfTowers]/I");
  mytree_->Branch("rawTPEmulsFGVB2", &_rawTPEmulsFGVB2,"rawTPEmulsFGVB2[nbOfTowers]/I");
  mytree_->Branch("rawTPEmulsFGVB3", &_rawTPEmulsFGVB3,"rawTPEmulsFGVB3[nbOfTowers]/I");
  mytree_->Branch("rawTPEmulsFGVB4", &_rawTPEmulsFGVB4,"rawTPEmulsFGVB4[nbOfTowers]/I");
  mytree_->Branch("rawTPEmulsFGVB5", &_rawTPEmulsFGVB5,"rawTPEmulsFGVB5[nbOfTowers]/I");
  mytree_->Branch("rawTPEmulttFlag1", &_rawTPEmulttFlag1,"rawTPEmulttFlag1[nbOfTowers]/I");
  mytree_->Branch("rawTPEmulttFlag2", &_rawTPEmulttFlag2,"rawTPEmulttFlag2[nbOfTowers]/I");
  mytree_->Branch("rawTPEmulttFlag3", &_rawTPEmulttFlag3,"rawTPEmulttFlag3[nbOfTowers]/I");
  mytree_->Branch("rawTPEmulttFlag4", &_rawTPEmulttFlag4,"rawTPEmulttFlag4[nbOfTowers]/I");
  mytree_->Branch("rawTPEmulttFlag5", &_rawTPEmulttFlag5,"rawTPEmulttFlag5[nbOfTowers]/I");

  //
  mytree_->Branch("trig_L1emIso_N",     &_trig_L1emIso_N,     "trig_L1emIso_N/I");
  mytree_->Branch("trig_L1emIso_ieta",  &_trig_L1emIso_ieta,  "trig_L1emIso_ieta[4]/I");
  mytree_->Branch("trig_L1emIso_iphi",  &_trig_L1emIso_iphi,  "trig_L1emIso_iphi[4]/I");
  mytree_->Branch("trig_L1emIso_rank",  &_trig_L1emIso_rank,  "trig_L1emIso_rank[4]/I");
  mytree_->Branch("trig_L1emIso_eta",   &_trig_L1emIso_eta,   "trig_L1emIso_eta[4]/D");
  mytree_->Branch("trig_L1emIso_phi",   &_trig_L1emIso_phi,   "trig_L1emIso_phi[4]/D");
  mytree_->Branch("trig_L1emIso_energy",&_trig_L1emIso_energy,"trig_L1emIso_energy[4]/D");
  mytree_->Branch("trig_L1emIso_et",    &_trig_L1emIso_et,    "trig_L1emIso_et[4]/D");
  //
  mytree_->Branch("trig_L1emNonIso_N",     &_trig_L1emNonIso_N,     "trig_L1emNonIso_N/I");
  mytree_->Branch("trig_L1emNonIso_ieta",  &_trig_L1emNonIso_ieta,  "trig_L1emNonIso_ieta[4]/I");
  mytree_->Branch("trig_L1emNonIso_iphi",  &_trig_L1emNonIso_iphi,  "trig_L1emNonIso_iphi[4]/I");
  mytree_->Branch("trig_L1emNonIso_rank",  &_trig_L1emNonIso_rank,  "trig_L1emNonIso_rank[4]/I");
  mytree_->Branch("trig_L1emNonIso_eta",   &_trig_L1emNonIso_eta,   "trig_L1emNonIso_eta[4]/D");
  mytree_->Branch("trig_L1emNonIso_phi",   &_trig_L1emNonIso_phi,   "trig_L1emNonIso_phi[4]/D");
  mytree_->Branch("trig_L1emNonIso_energy",&_trig_L1emNonIso_energy,"trig_L1emNonIso_energy[4]/D");
  mytree_->Branch("trig_L1emNonIso_et",    &_trig_L1emNonIso_et,    "trig_L1emNonIso_et[4]/D");
  
  // L1 candidates : modified collection
  mytree_->Branch("trig_L1emIso_N_M",     &_trig_L1emIso_N_M,     "trig_L1emIso_N_M/I");
  mytree_->Branch("trig_L1emIso_ieta_M",  &_trig_L1emIso_ieta_M,  "trig_L1emIso_ieta_M[4]/I");
  mytree_->Branch("trig_L1emIso_iphi_M",  &_trig_L1emIso_iphi_M,  "trig_L1emIso_iphi_M[4]/I");
  mytree_->Branch("trig_L1emIso_rank_M",  &_trig_L1emIso_rank_M,  "trig_L1emIso_rank_M[4]/I");
  mytree_->Branch("trig_L1emIso_eta_M",   &_trig_L1emIso_eta_M,   "trig_L1emIso_eta_M[4]/D");
  mytree_->Branch("trig_L1emIso_phi_M",   &_trig_L1emIso_phi_M,   "trig_L1emIso_phi_M[4]/D");
  mytree_->Branch("trig_L1emIso_energy_M",&_trig_L1emIso_energy_M,"trig_L1emIso_energy_M[4]/D");
  mytree_->Branch("trig_L1emIso_et_M",    &_trig_L1emIso_et_M,    "trig_L1emIso_et_M[4]/D");
  //
  mytree_->Branch("trig_L1emNonIso_N_M",     &_trig_L1emNonIso_N_M,     "trig_L1emNonIso_N/I");
  mytree_->Branch("trig_L1emNonIso_ieta_M",  &_trig_L1emNonIso_ieta_M,  "trig_L1emNonIso_ieta_M[4]/I");
  mytree_->Branch("trig_L1emNonIso_iphi_M",  &_trig_L1emNonIso_iphi_M,  "trig_L1emNonIso_iphi_M[4]/I");
  mytree_->Branch("trig_L1emNonIso_rank_M",  &_trig_L1emNonIso_rank_M,  "trig_L1emNonIso_rank_M[4]/I");
  mytree_->Branch("trig_L1emNonIso_eta_M",   &_trig_L1emNonIso_eta_M,   "trig_L1emNonIso_eta_M[4]/D");
  mytree_->Branch("trig_L1emNonIso_phi_M",   &_trig_L1emNonIso_phi_M,   "trig_L1emNonIso_phi_M[4]/D");
  mytree_->Branch("trig_L1emNonIso_energy_M",&_trig_L1emNonIso_energy_M,"trig_L1emNonIso_energy_M[4]/D");
  mytree_->Branch("trig_L1emNonIso_et_M",    &_trig_L1emNonIso_et_M,    "trig_L1emNonIso_et_M[4]/D");

  // pre/post - firing
  mytree_->Branch("trig_preL1emIso_N",     &_trig_preL1emIso_N,     "trig_preL1emIso_N/I");
  mytree_->Branch("trig_preL1emIso_ieta",  &_trig_preL1emIso_ieta,  "trig_preL1emIso_ieta[4]/I");
  mytree_->Branch("trig_preL1emIso_iphi",  &_trig_preL1emIso_iphi,  "trig_preL1emIso_iphi[4]/I");
  mytree_->Branch("trig_preL1emIso_rank",  &_trig_preL1emIso_rank,  "trig_preL1emIso_rank[4]/I");
  //
  mytree_->Branch("trig_preL1emNonIso_N",     &_trig_preL1emNonIso_N,     "trig_preL1emNonIso_N/I");
  mytree_->Branch("trig_preL1emNonIso_ieta",  &_trig_preL1emNonIso_ieta,  "trig_preL1emNonIso_ieta[4]/I");
  mytree_->Branch("trig_preL1emNonIso_iphi",  &_trig_preL1emNonIso_iphi,  "trig_preL1emNonIso_iphi[4]/I");
  mytree_->Branch("trig_preL1emNonIso_rank",  &_trig_preL1emNonIso_rank,  "trig_preL1emNonIso_rank[4]/I");
  //
  mytree_->Branch("trig_postL1emIso_N",     &_trig_postL1emIso_N,     "trig_postL1emIso_N/I");
  mytree_->Branch("trig_postL1emIso_ieta",  &_trig_postL1emIso_ieta,  "trig_postL1emIso_ieta[4]/I");
  mytree_->Branch("trig_postL1emIso_iphi",  &_trig_postL1emIso_iphi,  "trig_postL1emIso_iphi[4]/I");
  mytree_->Branch("trig_postL1emIso_rank",  &_trig_postL1emIso_rank,  "trig_postL1emIso_rank[4]/I");
  //
  mytree_->Branch("trig_postL1emNonIso_N",     &_trig_postL1emNonIso_N,     "trig_postL1emNonIso_N/I");
  mytree_->Branch("trig_postL1emNonIso_ieta",  &_trig_postL1emNonIso_ieta,  "trig_postL1emNonIso_ieta[4]/I");
  mytree_->Branch("trig_postL1emNonIso_iphi",  &_trig_postL1emNonIso_iphi,  "trig_postL1emNonIso_iphi[4]/I");
  mytree_->Branch("trig_postL1emNonIso_rank",  &_trig_postL1emNonIso_rank,  "trig_postL1emNonIso_rank[4]/I");
  //
  mytree_->Branch("trig_nMaskedRCT",      &_trig_nMaskedRCT,     "trig_nMaskedRCT/I");      
  mytree_->Branch("trig_iMaskedRCTeta",   &_trig_iMaskedRCTeta,  "trig_iMaskedRCTeta[trig_nMaskedRCT]/I");                                          
  mytree_->Branch("trig_iMaskedRCTcrate", &_trig_iMaskedRCTcrate,"trig_iMaskedRCTcrate[trig_nMaskedRCT]/I");                                                    
  mytree_->Branch("trig_iMaskedRCTphi",   &_trig_iMaskedRCTphi,  "trig_iMaskedRCTphi[trig_nMaskedRCT]/I");
  mytree_->Branch("trig_nMaskedCh",       &_trig_nMaskedCh,      "trig_nMaskedCh/I");    
  mytree_->Branch("trig_iMaskedTTeta",    &_trig_iMaskedTTeta,   "trig_iMaskedTTeta[trig_nMaskedCh]/I");   
  mytree_->Branch("trig_iMaskedTTphi",    &_trig_iMaskedTTphi,   "trig_iMaskedTTphi[trig_nMaskedCh]/I");      	
	
  // Beam Spot
  mytree_->Branch("BS_x",&BS_x,"BS_x/D");
  mytree_->Branch("BS_y",&BS_y,"BS_y/D");
  mytree_->Branch("BS_z",&BS_z,"BS_z/D");
  mytree_->Branch("BS_dz",&BS_dz,"BS_dz/D");
  mytree_->Branch("BS_dxdz",&BS_dxdz,"BS_dxdz/D");
  mytree_->Branch("BS_dydz",&BS_dydz,"BS_dydz/D");
  mytree_->Branch("BS_bw_x",&BS_bw_x,"BS_bw_x/D");
  mytree_->Branch("BS_bw_y",&BS_bw_y,"BS_bw_y/D");
	
  // SuperClusters
  // SC EB
  mytree_->Branch("sc_hybrid_N",   &_sc_hybrid_N,   "sc_hybrid_N/I");
  mytree_->Branch("sc_hybrid_E",   &_sc_hybrid_E,   "sc_hybrid_E[25]/D");
  mytree_->Branch("sc_hybrid_Et",  &_sc_hybrid_Et,  "sc_hybrid_Et[25]/D");
  mytree_->Branch("sc_hybrid_Eta", &_sc_hybrid_Eta, "sc_hybrid_Eta[25]/D");
  mytree_->Branch("sc_hybrid_Phi", &_sc_hybrid_Phi, "sc_hybrid_Phi[25]/D");
  mytree_->Branch("sc_hybrid_outOfTimeSeed",     &_sc_hybrid_outOfTimeSeed,     "sc_hybrid_outOfTimeSeed[25]/I");
  mytree_->Branch("sc_hybrid_severityLevelSeed", &_sc_hybrid_severityLevelSeed, "sc_hybrid_severityLevelSeed[25]/I");
  mytree_->Branch("sc_hybrid_e1", &_sc_hybrid_e1, "sc_hybrid_e1[25]/D");
  mytree_->Branch("sc_hybrid_e33", &_sc_hybrid_e33, "sc_hybrid_e33[25]/D");
  mytree_->Branch("sc_hybrid_he",&_sc_hybrid_he, "sc_hybrid_he[25]/D");
  mytree_->Branch("sc_hybrid_sigmaietaieta",&_sc_hybrid_sigmaietaieta, "sc_hybrid_sigmaietaieta[25]/D");
  mytree_->Branch("sc_hybrid_hcalDepth1TowerSumEt_dr03", &_sc_hybrid_hcalDepth1TowerSumEt_dr03, "sc_hybrid_hcalDepth1TowerSumEt_dr03[25]/D");
  mytree_->Branch("sc_hybrid_hcalDepth2TowerSumEt_dr03", &_sc_hybrid_hcalDepth2TowerSumEt_dr03, "sc_hybrid_hcalDepth2TowerSumEt_dr03[25]/D");
  mytree_->Branch("sc_hybrid_ecalRecHitSumEt_dr03",      &_sc_hybrid_ecalRecHitSumEt_dr03,      "sc_hybrid_ecalRecHitSumEt_dr03[25]/D");
  mytree_->Branch("sc_hybrid_trkiso_dr03",               &_sc_hybrid_trkiso_dr03,               "sc_hybrid_trkiso_dr03[25]/D");
  // SC EB : L1 stuff
  mytree_->Branch("sc_hybrid_TTetaVect", &_sc_hybrid_TTetaVect, "_sc_hybrid_TTetaVect[25][50]/I");
  mytree_->Branch("sc_hybrid_TTphiVect", &_sc_hybrid_TTphiVect, "_sc_hybrid_TTphiVect[25][50]/I");
  mytree_->Branch("sc_hybrid_TTetVect", &_sc_hybrid_TTetVect, "_sc_hybrid_TTetVect[25][50]/D");
  mytree_->Branch("sc_hybrid_RCTetaVect", &_sc_hybrid_RCTetaVect, "_sc_hybrid_RCTetaVect[25][10]/I");
  mytree_->Branch("sc_hybrid_RCTphiVect", &_sc_hybrid_RCTphiVect, "_sc_hybrid_RCTphiVect[25][10]/I");
  mytree_->Branch("sc_hybrid_RCTetVect", &_sc_hybrid_RCTetVect, "_sc_hybrid_RCTetVect[25][10]/D");
  mytree_->Branch("sc_hybrid_RCTL1isoVect", &_sc_hybrid_RCTL1isoVect, "_sc_hybrid_RCTL1isoVect[25][10]/I");
  mytree_->Branch("sc_hybrid_RCTL1nonisoVect", &_sc_hybrid_RCTL1nonisoVect, "_sc_hybrid_RCTL1nonisoVect[25][10]/I");
  //
  mytree_->Branch("sc_hybrid_RCTeta", &_sc_hybrid_RCTeta, "_sc_hybrid_RCTeta[25]/I");
  mytree_->Branch("sc_hybrid_RCTphi", &_sc_hybrid_RCTphi, "_sc_hybrid_RCTphi[25]/I");
  mytree_->Branch("sc_hybrid_RCTL1iso", &_sc_hybrid_RCTL1iso, "_sc_hybrid_RCTL1iso[25]/I");
  mytree_->Branch("sc_hybrid_RCTL1noniso", &_sc_hybrid_RCTL1noniso, "_sc_hybrid_RCTL1noniso[25]/I");
  // SC EE 
  mytree_->Branch("sc_multi55_N",   &_sc_multi55_N,   "sc_multi55_N/I");
  mytree_->Branch("sc_multi55_E",   &_sc_multi55_E,   "sc_multi55_E[25]/D");
  mytree_->Branch("sc_multi55_Et",  &_sc_multi55_Et,  "sc_multi55_Et[25]/D");
  mytree_->Branch("sc_multi55_Eta", &_sc_multi55_Eta, "sc_multi55_Eta[25]/D");
  mytree_->Branch("sc_multi55_Phi", &_sc_multi55_Phi, "sc_multi55_Phi[25]/D");
  mytree_->Branch("sc_multi55_he",  &_sc_multi55_he,  "sc_multi55_he[25]/D");
  mytree_->Branch("sc_multi55_sigmaietaieta",&_sc_multi55_sigmaietaieta, "sc_multi55_sigmaietaieta[25]/D");
  mytree_->Branch("sc_multi55_hcalDepth1TowerSumEt_dr03", &_sc_multi55_hcalDepth1TowerSumEt_dr03, "sc_multi55_hcalDepth1TowerSumEt_dr03[25]/D");
  mytree_->Branch("sc_multi55_hcalDepth2TowerSumEt_dr03", &_sc_multi55_hcalDepth2TowerSumEt_dr03, "sc_multi55_hcalDepth2TowerSumEt_dr03[25]/D");
  mytree_->Branch("sc_multi55_ecalRecHitSumEt_dr03",      &_sc_multi55_ecalRecHitSumEt_dr03,      "sc_multi55_ecalRecHitSumEt_dr03[25]/D");
  mytree_->Branch("sc_multi55_trkiso_dr03",               &_sc_multi55_trkiso_dr03,               "sc_multi55_trkiso_dr03[25]/D");
  // SC EE : L1 stuff
  mytree_->Branch("sc_multi55_TTetaVect", &_sc_multi55_TTetaVect, "_sc_multi55_TTetaVect[25][50]/I");
  mytree_->Branch("sc_multi55_TTphiVect", &_sc_multi55_TTphiVect, "_sc_multi55_TTphiVect[25][50]/I");
  mytree_->Branch("sc_multi55_TTetVect", &_sc_multi55_TTetVect, "_sc_multi55_TTetVect[25][50]/D");
  mytree_->Branch("sc_multi55_RCTetaVect", &_sc_multi55_RCTetaVect, "_sc_multi55_RCTetaVect[25][10]/I");
  mytree_->Branch("sc_multi55_RCTphiVect", &_sc_multi55_RCTphiVect, "_sc_multi55_RCTphiVect[25][10]/I");
  mytree_->Branch("sc_multi55_RCTetVect", &_sc_multi55_RCTetVect, "_sc_multi55_RCTetVect[25][10]/D");
  mytree_->Branch("sc_multi55_RCTL1isoVect", &_sc_multi55_RCTL1isoVect, "_sc_multi55_RCTL1isoVect[25][10]/I");
  mytree_->Branch("sc_multi55_RCTL1nonisoVect", &_sc_multi55_RCTL1nonisoVect, "_sc_multi55_RCTL1nonisoVect[25][10]/I");
  //
  mytree_->Branch("sc_multi55_RCTeta", &_sc_multi55_RCTeta, "_sc_multi55_RCTeta[25]/I");
  mytree_->Branch("sc_multi55_RCTphi", &_sc_multi55_RCTphi, "_sc_multi55_RCTphi[25]/I");
  mytree_->Branch("sc_multi55_RCTL1iso", &_sc_multi55_RCTL1iso, "_sc_multi55_RCTL1iso[25]/I");
  mytree_->Branch("sc_multi55_RCTL1noniso", &_sc_multi55_RCTL1noniso, "_sc_multi55_RCTL1noniso[25]/I");
  

}

// ====================================================================================
SpikeStudy::~SpikeStudy()
// ====================================================================================
{

	
  if(type_ == "MC") {
  } // if MC
}

// ====================================================================================
void SpikeStudy::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
// ====================================================================================
{
  
  if(PrintDebug_) std::cout << "Update Field" << std::endl;
  // Clemy's Stuff for Charge
  bool updateField(false);
  if (cacheIDMagField_!=iSetup.get<IdealMagneticFieldRecord>().cacheIdentifier()){
    updateField = true;
    cacheIDMagField_=iSetup.get<IdealMagneticFieldRecord>().cacheIdentifier();
    iSetup.get<IdealMagneticFieldRecord>().get(theMagField);
  }
	
  if(PrintDebug_) std::cout << "Update Geo" << std::endl;
  bool updateGeometry(false);
  if (cacheIDTDGeom_!=iSetup.get<TrackerDigiGeometryRecord>().cacheIdentifier()){
    updateGeometry = true;
    cacheIDTDGeom_=iSetup.get<TrackerDigiGeometryRecord>().cacheIdentifier();
    iSetup.get<TrackerDigiGeometryRecord>().get(trackerHandle_);
  }
  
  if(PrintDebug_) std::cout << "MutliTrajectory" << std::endl;
  if(updateField || updateGeometry){
    mtsTransform_ = new MultiTrajectoryStateTransform(trackerHandle_.product(),theMagField.product());
  }
	
  // Tree Maker
  //std::cout << "Init()" << std::endl;
  Init();
  if (funcbase_) funcbase_->init(iSetup);

  if(PrintDebug_) std::cout << "FillEvent" << std::endl;
  FillEvent (iEvent, iSetup);
  // for Skimming
  if(PrintDebug_) std::cout << "Skimming" << std::endl;
	
  //
  if(PrintDebug_) std::cout << "FillTpData (iEvent, iSetup);" << std::endl;
  FillTpData (iEvent, iSetup);

  if(type_ == "MC") {
  } // if MC
  if(PrintDebug_) std::cout << "FillEle (iEvent, iSetup);" << std::endl;
	
  if(fillsc_) FillSuperClusters(iEvent, iSetup);
  mytree_->Fill();
	
} // analyze

// ====================================================================================
void SpikeStudy::FillEvent (const edm::Event& iEvent, const edm::EventSetup& iSetup)
// ====================================================================================
{
  nEvent = iEvent.id().event();

//  std::cout<<"Fill events here asscess the number of envent= "<<nEvent<<std::endl;
  nRun   = iEvent.id().run();
  nLumi  = iEvent.luminosityBlock();
//  cout<<"test"<<endl;
  // -----------------
  // Pile-up
  // -----------------
  if(type_ == "MC") {
    Handle<vector<PileupSummaryInfo> > PupInfo;
    iEvent.getByLabel(PileupSrc_, PupInfo);
    for (vector<PileupSummaryInfo>::const_iterator cand = PupInfo->begin();cand != PupInfo->end(); ++ cand) {
	    
      _PU_N = cand->getPU_NumInteractions();
      //cout << " PU = "<< _PU_N << endl;
    } // loop on Pile up
  } // if MC

  // -----------------
  // Vertices
  // -----------------
  if(PrintDebug_) std::cout << "Vertices" << std::endl;
  Handle<reco::VertexCollection> recoPrimaryVertexCollection;
  iEvent.getByLabel(VerticesTag_,recoPrimaryVertexCollection);
  
  const reco::VertexCollection & vertices = *recoPrimaryVertexCollection.product();

  edm::Handle<reco::BeamSpot> recoBeamSpotHandle;
  ///iEvent.getByType(recoBeamSpotHandle);
  const reco::BeamSpot bs = *recoBeamSpotHandle;
	
  int vtx_counter=0;
  _vtx_N = recoPrimaryVertexCollection->size();
	
  // select the primary vertex as the one with higest sum of (pt)^2 of tracks                                                                               
  //PrimaryVertexSorter PVSorter;
  //std::vector<reco::Vertex> sortedVertices = PVSorter.sortedList( *(recoPrimaryVertexCollection.product()) );

  if(_vtx_N > 0) {
    reco::VertexCollection::const_iterator firstPV = vertices.begin();
    GlobalPoint local_vertexPosition(firstPV->position().x(),
				     firstPV->position().y(),
				     firstPV->position().z());
    vertexPosition = local_vertexPosition;
  }
  else {
    // 		GlobalPoint local_vertexPosition(bs.position().x(),
    // 					  bs.position().y(),
    // 					  bs.position().z());
    GlobalPoint local_vertexPosition(0,0,0); //TMP
    
    vertexPosition = local_vertexPosition;
  }

  // if(_vtx_N > 0) {
//     GlobalPoint local_vertexPosition(sortedVertices.front().position().x(),
// 				     sortedVertices.front().position().y(),
// 				     sortedVertices.front().position().z());
//     vertexPosition = local_vertexPosition;
//   }
//   else {
//     GlobalPoint local_vertexPosition(bs.position().x(),
// 				     bs.position().y(),
// 				     bs.position().z());
//     vertexPosition = local_vertexPosition;
//   }

  if(PrintDebug_) std::cout << "Loop on vertices" << std::endl;
  for( std::vector<reco::Vertex>::const_iterator PV = vertices.begin(); PV != vertices.end(); ++PV){
    if(vtx_counter > 14 ) continue;
		
    _vtx_normalizedChi2[vtx_counter] = PV->normalizedChi2();
    _vtx_ndof[vtx_counter] = PV->ndof();
    _vtx_nTracks[vtx_counter] = PV->tracksSize();
    _vtx_d0[vtx_counter] = PV->position().Rho();
    _vtx_x[vtx_counter] = PV->x();
    _vtx_y[vtx_counter] = PV->y();
    _vtx_z[vtx_counter] = PV->z();
		
    vtx_counter++;
  } // for loop on primary vertices
	
  if(vtx_counter>34) { _vtx_N = 35; cout << "Number of primary vertices>35, vtx_N set to 35" << endl;}
	
}

// ====================================================================================
void SpikeStudy::FillTpData (const edm::Event& iEvent, const edm::EventSetup& iSetup)
// ====================================================================================
{

        // geometry
  ESHandle<CaloGeometry> theGeometry;
  ESHandle<CaloSubdetectorGeometry> theEndcapGeometry_handle, theBarrelGeometry_handle;
  
  iSetup.get<CaloGeometryRecord>().get( theGeometry );
  iSetup.get<EcalEndcapGeometryRecord>().get("EcalEndcap",theEndcapGeometry_handle);
  iSetup.get<EcalBarrelGeometryRecord>().get("EcalBarrel",theBarrelGeometry_handle);
  
  iSetup.get<IdealGeometryRecord>().get(eTTmap_);
  theEndcapGeometry_ = &(*theEndcapGeometry_handle);
  theBarrelGeometry_ = &(*theBarrelGeometry_handle);
  
//        Numbers::initGeometry(iSetup, false);




  map<EcalTrigTowerDetId, towerEner> mapTower ;
  map<EcalTrigTowerDetId, towerEner>::iterator itTT ;

  ///////////////////////////                                                                                                                                                      
  // Get TP data         Nab                                                                                                                                             
  ///////////////////////////                                                                                                                                                      

  edm::Handle<EcalTrigPrimDigiCollection> tp;
  iEvent.getByLabel(tpCollectionNormal_,tp);
  // std::cout<<"TP collection size="<<tp.product()->size()<<std::endl ;

  for (unsigned int i=0;i<tp.product()->size();i++) {
    EcalTriggerPrimitiveDigi d = (*(tp.product()))[i];
    const EcalTrigTowerDetId TPtowid= d.id();
    towerEner tE ;
    tE.iphi_ = TPtowid.iphi() ;
    tE.ieta_ = TPtowid.ieta() ;
    tE.ttFlag_ = d[0].ttFlag();
    tE.tpgADC_ = (d[0].raw()&0xfff) ;
    tE.twrADC_ = (d[0].raw()&0xff) ;

    
    // if ((d[0].raw()&0xfff)!=0 ||  (d[0].raw()&0xff)!=0){                                                                                                                      
    //   if (fabs(TPtowid.ieta()) < 18 && d[0].sFGVB()==0) {                                                                                                                     
    //       std::cout << i  << " evt:" << myevt<<  " adc size: " << d[0].raw() <<  " tpgADC: " << (d[0].raw()&0xfff) << " twrADC: " << (d[0].raw()&0xff)  <<  " sFGVB: " << d[0].sFGVB() <<   " eta: " << TPtowid.ieta() << endl;                                                                                                                                   
  //     }                                                                                                                                                                     
  // }                                                                                                                                                                         
    tE.sFGVB_ = (d[0].sFGVB());
    // if (tE.twrADC_>6)
    //   {
    // 	cout<<tE.twrADC_<<" jakmubaley"<<endl;
    // 	cout<<d[0].sFGVB()<< " chaparemka"<<endl;
    //   }
    // if ((d[0].sFGVB())==1 &&  tE.twrADC_>0)
    //   {
	
    // 	cout<<" yesfgv "<<tE.sFGVB_<<endl;
    // 	cout<<" taver adc"<<(d[0].raw()&0xff)<<endl;
    //   }
    

    mapTower[TPtowid] = tE ;
  }


  ///////////////////////////                                                                                                               
  // Get Emulators TP                                                                                                                       
  ///////////////////////////                                                                                                               

  edm::Handle<EcalTrigPrimDigiCollection> tpEmul ;
  iEvent.getByLabel(tpEmulatorCollection_, tpEmul);
  //  if (print_) std::cout<<"TPEmulator collection size="<<tpEmul.product()->size()<<std::endl ;

  for (unsigned int i=0;i<tpEmul.product()->size();i++) {
    EcalTriggerPrimitiveDigi d = (*(tpEmul.product()))[i];
    const EcalTrigTowerDetId TPtowid= d.id();
    itTT = mapTower.find(TPtowid) ;
    if (itTT != mapTower.end())
      for (int j=0 ; j<5 ; j++) {
	(itTT->second).tpgEmul_[j] = (d[j].raw()&0xfff) ;
	(itTT->second).tpgEmulFlag_[j] = d[j].ttFlag();
	(itTT->second).tpgEmulsFGVB_[j] = d[j].sFGVB();
      }
    // if(( d[2].raw()&0xff)>6 )
    // {
    //   cout<<(d[2].raw()&0xff)<<" jhauwa"<<endl;
    //   cout<<d[2].sFGVB()<<" jhauwo"<<endl;
    // }

  }




    ///////////////////////////
    // Get rechits and spikes
    ///////////////////////////

    edm::ESHandle<EcalSeverityLevelAlgo> sevlv;
    iSetup.get<EcalSeverityLevelAlgoRcd>().get(sevlv);


    
    // channel status
    edm::ESHandle<EcalChannelStatus> pChannelStatus;
    iSetup.get<EcalChannelStatusRcd>().get(pChannelStatus);
  

    //  const EcalChannelStatus *chStatus = pChannelStatus.product();
    //const EcalRecHit * rh; 
    // Get EB rechits

    edm::Handle<EcalRecHitCollection> rechitsEB; 
    iEvent.getByLabel(EcalRecHitCollectionEB_, rechitsEB) ;

    int num_bad_crystals=0;
    int num_all_rechits=0;
    int num_intime_rechits=0;
    int num_intime_rechits_sevlv3_4=0;

    if (rechitsEB.product()->size()!=0) {
      for ( EcalRecHitCollection::const_iterator rechitItr = rechitsEB->begin(); rechitItr != rechitsEB->end(); ++rechitItr ) {   

            EBDetId id = rechitItr->id(); 
            const EcalTrigTowerDetId towid = id.tower();
            itTT = mapTower.find(towid) ;


	    double theta = theBarrelGeometry_->getGeometry(id)->getPosition().theta() ;  
	    double erec_eta=theBarrelGeometry_->getGeometry(id)->getPosition().eta();
	    double erec_phi=theBarrelGeometry_->getGeometry(id)->getPosition().phi();

	    double rechit_energy=rechitItr->energy()*sin(theta);
	    float rechit_time=rechitItr->time();

	    int severity_level=sevlv->severityLevel(id, *rechitsEB);


	    // if (rechit_energy>4)
	    //   {
	    // 	cout<<"rechit time looks like : "<<rechit_time<<endl; 
	    // 	cout<<"rechit energy looks like : "<<rechit_energy<<endl; 
	    // 	cout<<"rechit sevlv is :"<<severity_level<<endl;
	    	

	    //   }
	    //cleaning cut, set to 1 when highest rechit transverse Et  >4 has time <15ns, otherwise zero

	    if (rechit_energy>1) 
	      {
	      _all_rechits_Et[num_all_rechits]=rechit_energy;
	      _all_rechits_theta[num_all_rechits]=theta;
	      _all_rechits_eta[num_all_rechits]=erec_eta;
	      _all_rechits_phi[num_all_rechits]=erec_phi;
	      _all_rechits_time[num_all_rechits]=rechit_time;
	      num_all_rechits++;
	      }


	    if (rechit_energy>1 && abs(rechit_time)<16)
	      {  
	    	_intime_rechits_Et[num_intime_rechits]=rechit_energy;
	    	_intime_rechits_theta[num_intime_rechits]=theta;
	    	_intime_rechits_eta[num_intime_rechits]=erec_eta;
	    	_intime_rechits_phi[num_intime_rechits]=erec_phi;
	    	num_intime_rechits++;
		
	    	if (severity_level==3 ||severity_level==4)
	    	  {
	    	    _intime_rechits_sevlv3_4_Et[num_intime_rechits_sevlv3_4]=rechit_energy;
	    	    _intime_rechits_sevlv3_4_theta[num_intime_rechits_sevlv3_4]=theta;
	    	    _intime_rechits_sevlv3_4_eta[num_intime_rechits_sevlv3_4]=erec_eta;
	    	    _intime_rechits_sevlv3_4_phi[num_intime_rechits_sevlv3_4]=erec_phi;
	    	    num_intime_rechits_sevlv3_4++;
	    	  }
	      }


	    if (rechit_energy>4 && rechit_energy>(itTT->second).maxRechit_)
	      {
		(itTT->second).maxRechit_ = rechit_energy;
		if (abs(rechit_time)<16)
		  {
		    (itTT->second).rechit_cleaning_cut_=1;

		  }
		else
		  { 
		    (itTT->second).rechit_cleaning_cut_=0;
		  }
	      
		//		cout<<"cleaning cut looks like : "<<(itTT->second).rechit_cleaning_cut_<<endl; 
			
	      }

      	    if (severity_level==3 ||severity_level==4)
	      { 
		_erec_eta_sevlv3_4[num_bad_crystals]=erec_eta;
	    	_erec_Et_sevlv3_4[num_bad_crystals]=rechit_energy;
	    	_erec_phi_sevlv3_4[num_bad_crystals]=erec_phi;
	    	_erec_theta_sevlv3_4[num_bad_crystals]=theta;

		num_bad_crystals+=1;
	      }



            if (itTT != mapTower.end()) {
	      
	    
                (itTT->second).eRec_ += rechit_energy ;
		if ( ((itTT->second).maxRechit_) < (rechit_energy)) 
		     {
		       if((rechit_energy) > 1.)
		      {
			(itTT->second).sevlv_ = severity_level; 
			(itTT->second).maxRechit_ = rechit_energy;
		      }
		     }     
		if ( (itTT->second).sevlv2_ !=3) 
		  {
		    if ((itTT->second).sevlv2_ !=4)
		      {
			if((rechit_energy) > 1.)
			  {
			    (itTT->second).sevlv2_ = severity_level; 
			  }
		      }
		  }     
	      
		(itTT->second).crystNb_++;
	    }

      }
    }
    _n_bad_crystals=num_bad_crystals;
    _num_all_rechits=num_all_rechits;
    _num_intime_rechits=num_intime_rechits;
    _num_intime_rechits_sevlv3_4=num_intime_rechits_sevlv3_4;
    // Get EE rechits
    edm::Handle<EcalRecHitCollection> rechitsEE; 
    if (iEvent.getByLabel(EcalRecHitCollectionEE_, rechitsEE) ) {
      
        for ( EcalRecHitCollection::const_iterator rechitItr = rechitsEE->begin(); rechitItr != rechitsEE->end(); ++rechitItr ) {   
            EEDetId id = rechitItr->id();
            const EcalTrigTowerDetId towid = (*eTTmap_).towerOf(id);
            itTT = mapTower.find(towid) ;
            if (itTT != mapTower.end()) {
                double theta = theEndcapGeometry_->getGeometry(id)->getPosition().theta() ;
                (itTT->second).eRec_ += rechitItr->energy()*sin(theta) ;
		//rh = &*rechitItr;
		(itTT->second).sevlv_ = sevlv->severityLevel(id, *rechitsEE); 
		//cout << "severity level endcap " << sevlv->severityLevel(id, *rechitsEE) << endl;
            }
        }
    }
	

    int towerNb = 0 ;
    for (itTT = mapTower.begin() ; itTT != mapTower.end() ; ++itTT) {

      // select only non zero towers                                                                                                                                             
      bool fill(true) ;
      bool nonZeroEmul(false) ;
      // if ((itTT->second).sFGVB_==1 && ((itTT->second).tpgADC_&0xff)>0 )
      // 	{
      // 	  cout<<" ee mal taver "<<((itTT->second).tpgEmul_[2]&0xff)<<endl;
      // 	}

      for (int i=0 ; i<5 ; i++) if (((itTT->second).tpgEmul_[i]&0xff) > 0) nonZeroEmul = true ;
      if (((itTT->second).tpgADC_&0xff) <= 0 && (!nonZeroEmul) ) fill = false ;
      // if (print_ && fill) {
      // 	std::cout<<"ieta="<<(itTT->second).ieta_<<" "<<(itTT->second).iphi_<<" tp="<<((itTT->second).tpgADC_&0xff)<<" tpEmul=" ;
      // 	for (int i=0 ; i<5 ; i++) std::cout<<((itTT->second).tpgEmul_[i]&0xff)<<" " ;
      // 	std::cout<<" nbXtal="<<(itTT->second).nbXtal_ ;
      // 	std::cout<<std::endl ;
      // }
      if (fill) {
	_ieta[towerNb] = (itTT->second).ieta_ ;
	_iphi[towerNb] = (itTT->second).iphi_ ;
	_nbOfXtals[towerNb] = (itTT->second).nbXtal_ ;
	_rawTPData[towerNb] = (itTT->second).tpgADC_ ;
	_rawTPEmul1[towerNb] = (itTT->second).tpgEmul_[0] ;
	_rawTPEmul2[towerNb] = (itTT->second).tpgEmul_[1] ;
	_rawTPEmul3[towerNb] = (itTT->second).tpgEmul_[2] ;
	_rawTPEmul4[towerNb] = (itTT->second).tpgEmul_[3] ;
	_rawTPEmul5[towerNb] = (itTT->second).tpgEmul_[4] ;
	_rawTPEmulttFlag1[towerNb] = (itTT->second).tpgEmulFlag_[0] ;
	_rawTPEmulttFlag2[towerNb] = (itTT->second).tpgEmulFlag_[1] ;
	_rawTPEmulttFlag3[towerNb] = (itTT->second).tpgEmulFlag_[2] ;
	_rawTPEmulttFlag4[towerNb] = (itTT->second).tpgEmulFlag_[3] ;
	_rawTPEmulttFlag5[towerNb] = (itTT->second).tpgEmulFlag_[4] ;
	_rawTPEmulsFGVB1[towerNb] = (itTT->second).tpgEmulsFGVB_[0] ;
	_rawTPEmulsFGVB2[towerNb] = (itTT->second).tpgEmulsFGVB_[1] ;
	_rawTPEmulsFGVB3[towerNb] = (itTT->second).tpgEmulsFGVB_[2] ;
	_rawTPEmulsFGVB4[towerNb] = (itTT->second).tpgEmulsFGVB_[3] ;
	_rawTPEmulsFGVB5[towerNb] = (itTT->second).tpgEmulsFGVB_[4] ;
	_crystNb[towerNb] = (itTT->second).crystNb_ ;
	_maxRechit[towerNb] = (itTT->second).maxRechit_ ;
	_eRec[towerNb] = (itTT->second).eRec_ ;
	_sevlv[towerNb] = (itTT->second).sevlv_ ;
	_sevlv2[towerNb] = (itTT->second).sevlv2_ ;
	_ttFlag[towerNb] = (itTT->second).ttFlag_ ;
	_rechit_cleaning_cut[towerNb] = (itTT->second).rechit_cleaning_cut_ ;
 
	_spike[towerNb] = (itTT->second).spike_ ;
	_twrADC[towerNb] =  (itTT->second).twrADC_;
	_sFGVB[towerNb] =  (itTT->second).sFGVB_;

	if (abs(_ieta[towerNb])>17) {
	  unsigned int maxEmul = 0 ;
	  for (int i=0 ; i<5 ; i++) if (((itTT->second).tpgEmul_[i]&0xff) > maxEmul) maxEmul = ((itTT->second).tpgEmul_[i]&0xff) ;
	}
	towerNb++ ;
      }

    }

    _nbOfTowers = towerNb ;





  if(!aod_) {

    // ----------------------
    //  get L1 EM candidate
    // ----------------------
    
    // --- CURRENT BUNCH CROSSING --- //////////////////////////////////////////////////////////////////

    edm::Handle< l1extra::L1EmParticleCollection > emNonisolColl ;
    edm::Handle< l1extra::L1EmParticleCollection > emIsolColl ;
    edm::Handle< l1extra::L1EmParticleCollection > emNonisolColl_M ;
    edm::Handle< l1extra::L1EmParticleCollection > emIsolColl_M ;  

    if( !GetL1M_ ) {      
      // standard collection ALONE
      iEvent.getByLabel("l1extraParticles","NonIsolated", emNonisolColl ) ;
      iEvent.getByLabel("l1extraParticles","Isolated", emIsolColl ) ;
    } else {
      // standard collection
      iEvent.getByLabel("l1extraParticlesOnline","NonIsolated", emNonisolColl ) ;
      iEvent.getByLabel("l1extraParticlesOnline","Isolated", emIsolColl ) ;
      // modified collection
      iEvent.getByLabel("l1extraParticles","NonIsolated", emNonisolColl_M ) ;
      iEvent.getByLabel("l1extraParticles","Isolated", emIsolColl_M ) ;
    }

    ///// STANDARD COLLECTION ALONE
    
    // Isolated candidates
    _trig_L1emIso_N = emIsolColl->size();
    //    if(PrintDebug_) cout << "N L1 candidate iso : " << _trig_L1emIso_N << endl;
    cout << "N L1 candidate iso : " << _trig_L1emIso_N << endl;
    int counter = 0;
    for( l1extra::L1EmParticleCollection::const_iterator emItr = emIsolColl->begin(); emItr != emIsolColl->end() ;++emItr) {
      // Used by Clemy
      _trig_L1emIso_ieta[counter] = emItr->gctEmCand()->regionId().ieta();
      _trig_L1emIso_iphi[counter] = emItr->gctEmCand()->regionId().iphi();
      _trig_L1emIso_rank[counter] = emItr->gctEmCand()->rank(); // ET in ADC count... 1 ADC count = 0.5 GeV
      // From Trigger twiki
      _trig_L1emIso_eta[counter]    = emItr->eta();
      _trig_L1emIso_phi[counter]    = emItr->phi();
      _trig_L1emIso_energy[counter] = emItr->energy();
      _trig_L1emIso_et[counter]     = emItr->et();
      counter++;
    }
	  
    // Non Isolated candidates
    _trig_L1emNonIso_N = emNonisolColl->size();
    //  if(PrintDebug_) 
    cout << "N L1 candidate noniso : " << _trig_L1emNonIso_N << endl;	  
    counter = 0;
    for( l1extra::L1EmParticleCollection::const_iterator emItr = emNonisolColl->begin(); emItr != emNonisolColl->end() ;++emItr){  
      // Used by Clemy
      _trig_L1emNonIso_ieta[counter] = emItr->gctEmCand()->regionId().ieta();
      _trig_L1emNonIso_iphi[counter] = emItr->gctEmCand()->regionId().iphi();
      _trig_L1emNonIso_rank[counter] = emItr->gctEmCand()->rank(); // ET in ADC count... 1 ADC count = 0.5 GeV
      // From Trigger twiki
      _trig_L1emNonIso_eta[counter]    = emItr->eta();
      _trig_L1emNonIso_phi[counter]    = emItr->phi();
      _trig_L1emNonIso_energy[counter] = emItr->energy();
      _trig_L1emNonIso_et[counter]     = emItr->et();
      counter++;
    } // for loop on Non Iso cand
	  
    if( GetL1M_ ) {
      _trig_L1emIso_N_M = emIsolColl_M->size();
      //      if(PrintDebug_) cout << "_trig_L1emIso_N_M =" << _trig_L1emIso_N_M << endl;
      cout << "_trig_L1emIso_N_M =" << _trig_L1emIso_N_M << endl;
      counter=0;
      for( l1extra::L1EmParticleCollection::const_iterator emItr = emIsolColl_M->begin();
           emItr != emIsolColl_M->end() ;++emItr) {
           _trig_L1emIso_ieta_M[counter] = emItr->gctEmCand()->regionId().ieta();
           _trig_L1emIso_iphi_M[counter] = emItr->gctEmCand()->regionId().iphi();
           _trig_L1emIso_rank_M[counter] = emItr->gctEmCand()->rank();
           _trig_L1emIso_eta_M[counter]    = emItr->eta();
           _trig_L1emIso_phi_M[counter]    = emItr->phi();
           _trig_L1emIso_energy_M[counter] = emItr->energy();
           _trig_L1emIso_et_M[counter]     = emItr->et();
           counter++;
      }
    ///// MODIFIED COLLECTION IF ASKED
//      if( GetL1M_ ) {

      // Isolated candidates
//      _trig_L1emIso_N_M = emIsolColl_M->size();
//      if(PrintDebug_) cout << "_trig_L1emIso_N_M =" << _trig_L1emIso_N_M << endl;
//      counter=0;
//      for( l1extra::L1EmParticleCollection::const_iterator emItr = emIsolColl_M->begin(); 
//	   emItr != emIsolColl_M->end() ;++emItr) {
	// Used by Clemy
//	_trig_L1emIso_ieta_M[counter] = emItr->gctEmCand()->regionId().ieta();
	//_trig_L1emcounter++;
//      }
      
      // Non Isolated candidates
      _trig_L1emNonIso_N_M = emNonisolColl_M->size();
      cout << "_trig_L1emNonIso_N_M =" << _trig_L1emNonIso_N_M << endl;
      counter = 0;  
      for( l1extra::L1EmParticleCollection::const_iterator emItr = emNonisolColl_M->begin(); 
	   emItr != emNonisolColl_M->end() ;++emItr){  
	// Used by Clemy
	_trig_L1emNonIso_ieta_M[counter] = emItr->gctEmCand()->regionId().ieta();
	_trig_L1emNonIso_iphi_M[counter] = emItr->gctEmCand()->regionId().iphi();
	_trig_L1emNonIso_rank_M[counter] = emItr->gctEmCand()->rank(); 
	// ET in ADC count... 1 ADC count = 0.5 GeV
	// From Trigger twiki
	_trig_L1emNonIso_eta_M[counter]    = emItr->eta();
	_trig_L1emNonIso_phi_M[counter]    = emItr->phi();
	_trig_L1emNonIso_energy_M[counter] = emItr->energy();
	_trig_L1emNonIso_et_M[counter]     = emItr->et();
	counter++;
      } // for loop on Non Iso cand
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////

	  
    // --- PRE- AND POST-FIRING ---
	  
    edm::Handle< L1GlobalTriggerReadoutRecord > gtRecord;
    iEvent.getByLabel( edm::InputTag(gtRecordCollectionTag_), gtRecord);
    //PRE-FIRING
    const L1GtPsbWord psb = gtRecord->gtPsbWord(0xbb0d, -1);
    //psb.print(cout); 
    std::vector<int> psbel;
    psbel.push_back(psb.aData(4));
    psbel.push_back(psb.aData(5));
    psbel.push_back(psb.bData(4));
    psbel.push_back(psb.bData(5));
    counter = 0;
    std::vector<int>::const_iterator ipsbel;
    for(ipsbel=psbel.begin(); ipsbel!=psbel.end(); ipsbel++) {
      int rank = (*ipsbel)&0x3f; // ET in ADC count... 1 ADC count = 0.5 GeV
      if(rank>0) {
	int iEta = int(((*ipsbel)>>6)&7);
	int sign = ( ((*ipsbel>>9)&1) ? -1. : 1. ); 
	int regionEtaRec;
	if(sign > 0) regionEtaRec = iEta + 11;
	if(sign < 0) regionEtaRec = 10 - iEta;
	if(sign==0) std::cout<<"WEIRD (pre, non-iso)"<<std::endl;
	// Used by Clemy
	_trig_preL1emNonIso_ieta[counter] = regionEtaRec;
	_trig_preL1emNonIso_iphi[counter] = int(((*ipsbel)>>10)&0x1f);
	_trig_preL1emNonIso_rank[counter] = rank;
	counter++;
      }
    }//loop Noniso
    _trig_preL1emNonIso_N = counter;
	  
    psbel.clear();
    psbel.push_back(psb.aData(6));
    psbel.push_back(psb.aData(7));
    psbel.push_back(psb.bData(6));
    psbel.push_back(psb.bData(7));
    counter = 0;
    for(ipsbel=psbel.begin(); ipsbel!=psbel.end(); ipsbel++) {
      int rank = (*ipsbel)&0x3f; // ET in ADC count... 1 ADC count = 0.5 GeV
      if(rank>0) {
	int iEta = int(((*ipsbel)>>6)&7);
	int sign = ( ((*ipsbel>>9)&1) ? -1. : 1. ); 
	int regionEtaRec;
	if(sign > 0) regionEtaRec = iEta + 11;
	if(sign < 0) regionEtaRec = 10 - iEta;
	if(sign==0) std::cout<<"WEIRD (pre, iso)"<<std::endl;
	// Used by Clemy
	_trig_preL1emIso_ieta[counter] = regionEtaRec;
	_trig_preL1emIso_iphi[counter] = int(((*ipsbel)>>10)&0x1f);
	_trig_preL1emIso_rank[counter] = rank;
	counter++;
      }
    }//loop Iso
    _trig_preL1emIso_N = counter;
	  
	  
    //POST-FIRING
    const L1GtPsbWord psb2 = gtRecord->gtPsbWord(0xbb0d, 1);
    std::vector<int> psbel2;
    psbel2.push_back(psb2.aData(4));
    psbel2.push_back(psb2.aData(5));
    psbel2.push_back(psb2.bData(4));
    psbel2.push_back(psb2.bData(5));
    counter = 0;
    std::vector<int>::const_iterator ipsbel2;
    for(ipsbel2=psbel2.begin(); ipsbel2!=psbel2.end(); ipsbel2++) {
      int rank = (*ipsbel2)&0x3f; // ET in ADC count... 1 ADC count = 0.5 GeV
      if(rank>0) {
	int iEta = int(((*ipsbel2)>>6)&7);
	int sign = ( ((*ipsbel2>>9)&1) ? -1. : 1. ); 
	int regionEtaRec;
	if(sign > 0) regionEtaRec = iEta + 11;
	if(sign < 0) regionEtaRec = 10 - iEta;
	if(sign==0) std::cout<<"WEIRD (post, non-iso)"<<std::endl;
	// Used by Clemy
	_trig_postL1emNonIso_ieta[counter] = regionEtaRec;
	_trig_postL1emNonIso_iphi[counter] = int(((*ipsbel2)>>10)&0x1f);
	_trig_postL1emNonIso_rank[counter] = rank;
	counter++;
      }
    }//loop Noniso
    _trig_postL1emNonIso_N = counter;
	  
    psbel2.clear();
    psbel2.push_back(psb2.aData(6));
    psbel2.push_back(psb2.aData(7));
    psbel2.push_back(psb2.bData(6));
    psbel2.push_back(psb2.bData(7));
    counter = 0;
    for(ipsbel2=psbel2.begin(); ipsbel2!=psbel2.end(); ipsbel2++) {
      int rank = (*ipsbel2)&0x3f; // ET in ADC count... 1 ADC count = 0.5 GeV
      if(rank>0) {
	int iEta = int(((*ipsbel2)>>6)&7);
	int sign = ( ((*ipsbel2>>9)&1) ? -1. : 1. ); 
	int regionEtaRec;
      	if(sign > 0) regionEtaRec = iEta + 11;
	if(sign < 0) regionEtaRec = 10 - iEta;
	if(sign==0) std::cout<<"WEIRD (post, iso)"<<std::endl;
	// Used by Clemy
	_trig_postL1emIso_ieta[counter] = regionEtaRec;
	_trig_postL1emIso_iphi[counter] = int(((*ipsbel2)>>10)&0x1f);
	_trig_postL1emIso_rank[counter] = rank;
	counter++;
      }
    }//loop Iso
    _trig_postL1emIso_N = counter;
	  
  }   // if AOD
	


} // end of FillTpData

	
// ====================================================================================
void SpikeStudy::FillSuperClusters(const edm::Event& iEvent, const edm::EventSetup& iSetup)
// ====================================================================================
{
  //std::cout << "FillSuperClusters  geometry " << std::endl;
  // geometry
  ///const CaloGeometry * geometry;
  unsigned long long cacheIDGeom = 0;
  edm::ESHandle<CaloGeometry> theCaloGeom;
  if(cacheIDGeom!=iSetup.get<CaloGeometryRecord>().cacheIdentifier()) 
    {
    cacheIDGeom = iSetup.get<CaloGeometryRecord>().cacheIdentifier();
    iSetup.get<CaloGeometryRecord>().get(theCaloGeom_);
    }
  //geometry = theCaloGeom.product() ;
  
  //std::cout << "FillSuperClusters  TPGTowerStatus " << std::endl;
  
  // TPGTowerStatus
  edm::ESHandle<EcalTPGTowerStatus> theEcalTPGTowerStatus_handle;
  iSetup.get<EcalTPGTowerStatusRcd>().get(theEcalTPGTowerStatus_handle);
  const EcalTPGTowerStatus * ecaltpgTowerStatus=theEcalTPGTowerStatus_handle.product();
  
  const EcalTPGTowerStatusMap &towerMap=ecaltpgTowerStatus->getMap();
  EcalTPGTowerStatusMapIterator  it;
  //std::cout << __LINE__ << std::endl;
  //for42x	
  unsigned long long cacheSevLevel = 0;
  edm::ESHandle<EcalSeverityLevelAlgo> sevLevel;
  if(cacheSevLevel != iSetup.get<EcalSeverityLevelAlgoRcd>().cacheIdentifier()){
    cacheSevLevel = iSetup.get<EcalSeverityLevelAlgoRcd>().cacheIdentifier();
    iSetup.get<EcalSeverityLevelAlgoRcd>().get(sevLevel);
  }
  const EcalSeverityLevelAlgo* sl=sevLevel.product();
  //std::cout << __LINE__ << std::endl;

  // for H/E
  //if (towerIso1_) delete towerIso1_ ; towerIso1_ = 0 ;
  //if (towerIso2_) delete towerIso2_ ; towerIso2_ = 0 ;
  //if (towersH_) delete towersH_ ; towersH_ = 0 ;
  towersH_ = new edm::Handle<CaloTowerCollection>() ;
  if (!iEvent.getByLabel(hcalTowers_,*towersH_))
    { edm::LogError("ElectronHcalHelper::readEvent")<<"failed to get the hcal towers of label "<<hcalTowers_ ; }
  towerIso1_ = new EgammaTowerIsolation(hOverEConeSize_,0.,hOverEPtMin_,1,towersH_->product()) ;
  towerIso2_ = new EgammaTowerIsolation(hOverEConeSize_,0.,hOverEPtMin_,2,towersH_->product()) ;
  
  //std::cout << "FillSuperClusters  Beam Spot " << std::endl;
  
  reco::BeamSpot bs; //beamSpot;
  edm::Handle<reco::BeamSpot> beamSpotHandle;
  iEvent.getByLabel("offlineBeamSpot", beamSpotHandle);
  if (beamSpotHandle.isValid() ) bs = *beamSpotHandle;
  else cout << "No Beam spot ! " << endl;
// // Beam Spot
//   edm::Handle<reco::BeamSpot> recoBeamSpotHandle ;
//   ///iEvent.getByType(recoBeamSpotHandle) ;
//   const reco::BeamSpot bs = *recoBeamSpotHandle ;
  
  //std::cout << __LINE__ << std::endl;
  //std::cout << "FillSuperClusters  Isolation " << std::endl;	

  // Isolation
  edm::Handle<TrackCollection> ctfTracksH;  
  iEvent.getByLabel("generalTracks", ctfTracksH); // ctfTracks_
  //   //get the tracks
  //   edm::Handle<reco::TrackCollection> tracks;
  //   e.getByLabel(trackInputTag_,tracks);
  //   if(!tracks.isValid()) {
  //     return;
  //   }
  //   const reco::TrackCollection* trackCollection = tracks.product();
  
  //std::cout << "FillSuperClusters  Iso Track " << std::endl;
  
  // Iso Track
  double isolationtrackThresholdB_Barrel = 0.7;     //0.0; 
  double TrackConeOuterRadiusB_Barrel    = 0.3; 
  double TrackConeInnerRadiusB_Barrel    = 0.015;   //0.04;
  double isolationtrackEtaSliceB_Barrel  = 0.015;
  double longImpactParameterB_Barrel     = 0.2;  
  double transImpactParameterB_Barrel    = 999999.; //0.1;  
  
  double isolationtrackThresholdB_Endcap  = 0.7;    // 0.0
  double TrackConeOuterRadiusB_Endcap     = 0.3;
  double TrackConeInnerRadiusB_Endcap     = 0.015;  //0.04;
  double isolationtrackEtaSliceB_Endcap   = 0.015;
  double longImpactParameterB_Endcap      = 0.2;
  double transImpactParameterB_Endcap     = 999999.; //0.1;
  
  //std::cout << __LINE__ << std::endl;
  //  double intRadiusBarrel = 0.015; 
  //   double intRadiusEndcap = 0.015; 
  //   double stripBarrel     = 0.015; 
  //   double stripEndcap     = 0.015; 
  //   double ptMin           = 0.7; 
  //   double maxVtxDist      = 0.2; 
  //   double drb             = 999999999.;  //  maxDrbTk 
  
  
  //std::cout << "FillSuperClusters  Iso HCAL " << std::endl;
  // Iso HCAL
  float egHcalIsoConeSizeOutSmall=0.3;
  //float egHcalIsoConeSizeOutLarge=0.4;
  int egHcalDepth1=1, egHcalDepth2=2; //float egHcalIsoConeSizeIn=intRadiusHcal_,egHcalIsoPtMin=etMinHcal_;
  double egHcalIsoConeSizeIn = 0.15;  //intRadiusHcal   = 0.15;
  double egHcalIsoPtMin      = 0.0;   //  etMinHcal 
  EgammaTowerIsolation hadDepth1Isolation03(egHcalIsoConeSizeOutSmall,egHcalIsoConeSizeIn,egHcalIsoPtMin,egHcalDepth1,towersH_->product()) ;
  EgammaTowerIsolation hadDepth2Isolation03(egHcalIsoConeSizeOutSmall,egHcalIsoConeSizeIn,egHcalIsoPtMin,egHcalDepth2,towersH_->product()) ;
  //std::cout << __LINE__ << std::endl;
 
  //std::cout << "FillSuperClusters  Iso ECAL " << std::endl;
  // Iso ECAL
  double egIsoConeSizeInBarrel  = 3.0; // intRadiusEcalBarrel 
  double egIsoConeSizeInEndcap  = 3.0;  // intRadiusEcalEndcaps
  double egIsoJurassicWidth     = 1.5;  // jurassicWidth 
  double egIsoPtMinBarrel       = 0.0;  // etMinBarrel
  double egIsoEMinBarrel        = 0.08; // eMinBarrel
  double egIsoPtMinEndcap       = 0.1;  // etMinEndcaps
  double egIsoEMinEndcap        = 0.0;  // egIsoEMinEndcaps
  bool vetoClustered   = false;  
  bool useNumCrystals  = true;  
  // for SpikeRemoval -- not in 361p4 -- 
  //int severityLevelCut           = 4;
  //double severityRecHitThreshold = 5.0;
  //double spikeIdThreshold        = 0.95;
  //string spId                    = "kSwissCrossBordersIncluded";  // ikeIdString 
  //for42x
  //EcalSeverityLevelAlgo::SpikeId spId = EcalSeverityLevelAlgo::kSwissCrossBordersIncluded;
  //
  //float extRadiusSmall=0.3, extRadiusLarge=0.4 ;
  float egIsoConeSizeOutSmall=0.3; //, egIsoConeSizeOutLarge=0.4;
  
  
  //std::cout << "FillSuperClusters  EB SuperCluster " << std::endl;
  
  // -----------------
  //  EB SuperCluster
  // -----------------
  // Retrieve SuperCluster
  edm::Handle<reco::SuperClusterCollection> sc_coll_EB;
  iEvent.getByLabel(edm::InputTag("correctedHybridSuperClusters"), sc_coll_EB);
  
  //cout << " size EB = " << sc_coll_EB->size() << endl;
  
  _sc_hybrid_N = sc_coll_EB->size();
  int index_sc = 0;
  
  //std::cout << "FillSuperClusters  SpikeRemoval " << std::endl;
  
  // Define stuff for SpikeRemoval
  const CaloTopology * topology ;
  ///const EcalChannelStatus *chStatus ;
  edm::Handle< EcalRecHitCollection > reducedEBRecHits;
  edm::Handle< EcalRecHitCollection > reducedEERecHits;
  //std::cout << __LINE__ << std::endl;
  
  unsigned long long cacheIDTopo_=0;
  edm::ESHandle<CaloTopology> theCaloTopo;
  if (cacheIDTopo_!=iSetup.get<CaloTopologyRecord>().cacheIdentifier()){
    cacheIDTopo_=iSetup.get<CaloTopologyRecord>().cacheIdentifier();
    iSetup.get<CaloTopologyRecord>().get(theCaloTopo);
  }
  topology = theCaloTopo.product() ;
  
  edm::ESHandle<EcalChannelStatus> pChannelStatus;
  iSetup.get<EcalChannelStatusRcd>().get(pChannelStatus);
  ///chStatus = pChannelStatus.product();

  // reduced rechits
  if(!aod_){
    iEvent.getByLabel( edm::InputTag("ecalRecHit:EcalRecHitsEB"), reducedEBRecHits );
    iEvent.getByLabel( edm::InputTag("ecalRecHit:EcalRecHitsEE"), reducedEERecHits );
  }
  else{
    iEvent.getByLabel( edm::InputTag("reducedEcalRecHitsEB"), reducedEBRecHits );
    iEvent.getByLabel( edm::InputTag("reducedEcalRecHitsEE"), reducedEERecHits );
  }
  
  // For L1
  int nTow=0;
  int nReg=0;
  
  //std::cout << "FillSuperClusters  Loop EB SuperCluster " << std::endl;
  //std::cout << __LINE__ << std::endl;
  
  // --------------------------
  // Loop on SuperClusters EB
  // --------------------------
  for( reco::SuperClusterCollection::const_iterator isc=sc_coll_EB->begin(); isc!=sc_coll_EB->end(); isc++) {
    if(index_sc>24) continue;
    double R  = TMath::Sqrt(isc->x()*isc->x() + isc->y()*isc->y() +isc->z()*isc->z());
    double Rt = TMath::Sqrt(isc->x()*isc->x() + isc->y()*isc->y());
    
    _sc_hybrid_E[index_sc]   = isc->energy();
    _sc_hybrid_Et[index_sc]  = isc->energy()*(Rt/R);
    _sc_hybrid_Eta[index_sc] = isc->eta();
    _sc_hybrid_Phi[index_sc] = isc->phi();
    
    const EcalRecHitCollection * reducedRecHits = 0 ;
    reducedRecHits = reducedEBRecHits.product() ; 
    
    //seed cluster analysis
    const edm::Ptr<reco::CaloCluster> & seedCluster = isc->seed(); //(*EleHandle)[i].superCluster()->seed() ;  
    std::pair<DetId, float> id = EcalClusterTools::getMaximum(seedCluster->hitsAndFractions(),reducedRecHits);
    const EcalRecHit & rh = getRecHit(id.first,reducedRecHits);
    int flag = rh.recoFlag();   
    
    // Out of time
    if (flag == EcalRecHit::kOutOfTime) 
      _sc_hybrid_outOfTimeSeed[index_sc] = 1;   
    else 
      _sc_hybrid_outOfTimeSeed[index_sc] = 0;   
    
    //std::cout << __LINE__ << std::endl;
    // Severity Level
    //for42X
    //    int sev = EcalSeverityLevelAlgo::severityLevel(id.first,*reducedRecHits,*chStatus, 5., EcalSeverityLevelAlgo::kSwissCross,0.95) ;
    int sev=sl->severityLevel(id.first,*reducedRecHits);
    _sc_hybrid_severityLevelSeed[index_sc] = sev ;
    
    // Old SpikeRemoval e1/e9
    const reco::CaloCluster & seedCluster1 = *(isc->seed());
    _sc_hybrid_e1[index_sc]   = EcalClusterTools::eMax(seedCluster1,reducedRecHits)  ;
    _sc_hybrid_e33[index_sc]  = EcalClusterTools::e3x3(seedCluster1,reducedRecHits,topology)  ;
    
    // H/E
    reco::SuperCluster EmSCCand = *isc;
    double HoE = towerIso1_->getTowerESum(&EmSCCand) + towerIso2_->getTowerESum(&EmSCCand) ;
    HoE /= 	isc->energy() ;     
    _sc_hybrid_he[index_sc] = HoE ;
    
    // SigmaIetaIeta
    std::vector<float> localCovariances = EcalClusterTools::localCovariances(seedCluster1,reducedRecHits,topology) ;
    _sc_hybrid_sigmaietaieta[index_sc]  = sqrt(localCovariances[0]) ;
    
    //	std::cout << "FillSuperClusters  Iso Track " << std::endl;
    
    // Iso Track
    //ElectronTkIsolation tkIsolation03(extRadiusSmall,intRadiusBarrel,intRadiusEndcap,stripBarrel,stripEndcap,ptMin,maxVtxDist,drb,ctfTracksH.product(),bs.position()) ;
    //_sc_hybrid_tkSumPt_dr03[index_sc] = tkIsolation03.getPtTracks(isc);
    
    //	std::cout << "FillSuperClusters  Iso HCAL " << std::endl;
    
    // Iso HCAL
    _sc_hybrid_hcalDepth1TowerSumEt_dr03[index_sc] = hadDepth1Isolation03.getTowerEtSum(&EmSCCand);
    _sc_hybrid_hcalDepth2TowerSumEt_dr03[index_sc] = hadDepth2Isolation03.getTowerEtSum(&EmSCCand);
    
    //std::cout << "FillSuperClusters  Iso ECAL " << std::endl;
    
    // Iso ECAL
//    EcalRecHitMetaCollection ecalBarrelHits(*reducedEBRecHits);
    EgammaRecHitIsolation ecalBarrelIsol03( egIsoConeSizeOutSmall,egIsoConeSizeInBarrel,egIsoJurassicWidth,
                                            egIsoPtMinBarrel,egIsoEMinBarrel,theCaloGeom_,*reducedEBRecHits,
                                            sevLevel.product(),DetId::Ecal );
    //for42x
    //    EgammaRecHitIsolation ecalBarrelIsol03(egIsoConeSizeOutSmall,egIsoConeSizeInBarrel,egIsoJurassicWidth,egIsoPtMinBarrel,egIsoEMinBarrel,theCaloGeom,&ecalBarrelHits,DetId::Ecal);
  //  EgammaRecHitIsolation ecalBarrelIsol03(egIsoConeSizeOutSmall,egIsoConeSizeInBarrel,egIsoJurassicWidth,egIsoPtMinBarrel,egIsoEMinBarrel,theCaloGeom,&ecalBarrelHits,sevLevel.product(),DetId::Ecal);
    ecalBarrelIsol03.setUseNumCrystals(useNumCrystals);
    ecalBarrelIsol03.setVetoClustered(vetoClustered);
    //std::cout << __LINE__ << std::endl;
   //for42x
    //   // !!! Spike Removal... not in 361p4! Have to add it after !!!
    //    ecalBarrelIsol03.doSpikeRemoval(reducedEBRecHits.product(),pChannelStatus.product(),severityLevelCut,severityRecHitThreshold,spId,spikeIdThreshold);
    
    //std::cout << "FillSuperClusters  ugly " << std::endl;
    
    // ugly...
    reco::RecoEcalCandidate * cand = new RecoEcalCandidate();
    math::XYZPoint v(0,0,0); math::XYZVector p = isc->energy() * (isc->position() -v).unit(); double t = sqrt(0. + p.mag2());
    cand->setCharge(0); cand->setVertex(v); cand->setP4(reco::Candidate::LorentzVector(p.x(), p.y(), p.z(), t));		
    const reco::SuperClusterRef sc_ref(sc_coll_EB, index_sc);
    cand->setSuperCluster(sc_ref);
    //reco::SuperClusterRef sc = cand->get<reco::SuperClusterRef>();
    
    _sc_hybrid_ecalRecHitSumEt_dr03[index_sc] = ecalBarrelIsol03.getEtSum(cand);
    
    //	std::cout << "FillSuperClusters  Track Isolation " << std::endl;
    
    // Track Isolation
    // Calculate hollow cone track isolation, CONE 0.3
    reco::Photon * newPhoton = new Photon(); 
    newPhoton->setVertex(v); newPhoton->setCharge(0); newPhoton->setMass(0);
    newPhoton->setP4(reco::Candidate::LorentzVector(p.x(), p.y(), p.z(), isc->energy()));
    
    //int ntrk_03 = 0.; 
    double trkiso_hc_03 = 0.;
    ///int counter = 0;
    double ptSum = 0.;
    
    PhotonTkIsolation phoIso(TrackConeOuterRadiusB_Barrel, //RCone, 
			     TrackConeInnerRadiusB_Barrel, //RinnerCone, 
			     isolationtrackEtaSliceB_Barrel, //etaSlice,  
			     isolationtrackThresholdB_Barrel, //pTThresh, 
			     longImpactParameterB_Barrel, //lip , 
			     transImpactParameterB_Barrel, //d0, 
			     ctfTracksH.product(), //trackCollection, ctfTracksH.product(),bs.position()
			     bs.position());       //math::XYZPoint(vertexBeamSpot.x0(),vertexBeamSpot.y0(),vertexBeamSpot.z0()));
    
    ///counter  = phoIso.getIso(newPhoton).first;
    ptSum    = phoIso.getIso(newPhoton).second;
    trkiso_hc_03 = ptSum;
    
    _sc_hybrid_trkiso_dr03[index_sc] = trkiso_hc_03;
    
    //	std::cout << "FillSuperClusters SC EB -- modif-alex l1 matching " << std::endl;

    if(!aod_){
      // ____________________________
      // SC EB -- modif-alex l1 matching
      // LOOP MATCHING ON L1 trigger 
      // ____________________________
      nTow=0;
      nReg=0;
      for(int icc = 0; icc < 50; ++icc) {
	_sc_hybrid_TTetaVect[index_sc][icc] = -999;
	_sc_hybrid_TTphiVect[index_sc][icc] = -999;
	_sc_hybrid_TTetVect[index_sc][icc]  = 0.;
      }
      for(int icc = 0; icc < 10; ++icc) {
	_sc_hybrid_RCTetaVect[index_sc][icc]      = -999;
	_sc_hybrid_RCTphiVect[index_sc][icc]      = -999;
	_sc_hybrid_RCTetVect[index_sc][icc]       = 0.;
	_sc_hybrid_RCTL1isoVect[index_sc][icc]    = -999;
	_sc_hybrid_RCTL1nonisoVect[index_sc][icc] = -999;
      }
      
      for (reco::CaloCluster_iterator clus = isc->clustersBegin () ;
	   clus != isc->clustersEnd () ;
	   ++clus){
	std::vector<std::pair<DetId, float> > clusterDetIds = (*clus)->hitsAndFractions() ; //get these from the cluster                                            
	//loop on xtals in cluster                                                                                                                                  
	for (std::vector<std::pair<DetId, float> >::const_iterator detitr = clusterDetIds.begin () ;
	     detitr != clusterDetIds.end () ;
	     ++detitr)
	  {
	    //Here I use the "find" on a digi collection... I have been warned...                                                                                   
	    if ( (detitr -> first).det () != DetId::Ecal)
	      {
		std::cout << " det is " << (detitr -> first).det () << std::endl ;
		continue ;
	      }
	    EcalRecHitCollection::const_iterator thishit;
	    EcalRecHit myhit;
	    EcalTrigTowerDetId towid;
	    float thetahit;
	    //if ( (detitr -> first).subdetId () == EcalBarrel)
	    //{
	    thishit = reducedRecHits->find ( (detitr -> first) ) ;
	    if (thishit == reducedRecHits->end ()) continue;
	    myhit = (*thishit) ;
	    EBDetId detid(thishit->id());
	    towid= detid.tower();
	    thetahit =  theBarrelGeometry_->getGeometry((detitr -> first))->getPosition().theta();
	    //}//barrel rechit
	    //  else {
	    // 	    if ( (detitr -> first).subdetId () == EcalEndcap)
	    // 	      {
	    // 		thishit = reducedRecHits->find ( (detitr -> first) ) ;
	    // 		if (thishit == reducedRecHits->end ()) continue;
	    // 		myhit = (*thishit) ;
	    // 		EEDetId detid(thishit->id());
	    // 		towid= (*eTTmap_).towerOf(detid);
	    // 		thetahit =  theEndcapGeometry_->getGeometry((detitr -> first))->getPosition().theta();
	    // 	      }
	    // 	    else continue;
	    // 	  }//endcap rechit
	    
	    int iETA=towid.ieta();
	    int iPHI=towid.iphi();
	    int iReta=getGCTRegionEta(iETA);
	    int iRphi=getGCTRegionPhi(iPHI);
	    double iET=myhit.energy()*sin(thetahit);
	    
	    bool newTow = true;
	    if(nTow>0) {
	      for (int iTow=0; iTow<nTow; ++iTow) {
		if(_sc_hybrid_TTetaVect[index_sc][iTow] == iETA && _sc_hybrid_TTphiVect[index_sc][iTow] == iPHI) {
		  newTow = false;
		  _sc_hybrid_TTetVect[index_sc][iTow] +=  iET;
		}
	      }
	    } // if nTow>0
	    if(newTow) {
	      _sc_hybrid_TTetaVect[index_sc][nTow] = iETA;
	      _sc_hybrid_TTphiVect[index_sc][nTow] = iPHI;
	      _sc_hybrid_TTetVect[index_sc][nTow] =  iET;
	      nTow++;
	    } // if newTow
	    
	    bool newReg = true;
	    if(nReg>0) {
	      for (int iReg=0; iReg<nReg; ++iReg) {
		if(_sc_hybrid_RCTetaVect[index_sc][iReg] == iReta && _sc_hybrid_RCTphiVect[index_sc][iReg] == iRphi) {
		  newReg = false;
		  _sc_hybrid_RCTetVect[index_sc][iReg] +=  iET;
		}
	      }
	    } // if newreg>0
	    
	    if(newReg) {
	      _sc_hybrid_RCTetaVect[index_sc][nReg] = iReta;
	      _sc_hybrid_RCTphiVect[index_sc][nReg] = iRphi;
	      _sc_hybrid_RCTetVect[index_sc][nReg] =  iET;
	      
	      for(int il1=0; il1<_trig_L1emIso_N; ++il1) {
		if(_trig_L1emIso_iphi[il1] == iRphi && _trig_L1emIso_ieta[il1] == iReta) _sc_hybrid_RCTL1isoVect[index_sc][nReg] = _trig_L1emIso_rank[il1];
	      }
	      for(int il1=0; il1<_trig_L1emNonIso_N; ++il1) {
		if(_trig_L1emNonIso_iphi[il1] == iRphi && _trig_L1emNonIso_ieta[il1] == iReta) _sc_hybrid_RCTL1nonisoVect[index_sc][nReg] = _trig_L1emNonIso_rank[il1];
	      }
	      nReg++;
	    } // if newReg
	  }//loop crystal
      }//loop cluster
      
      //double TTetmax=0.;
      //int iTTmax=-1;
      double TTetmax2 = 0.;
      int iTTmax2     = -1;
      
      for (int iTow=0; iTow<nTow; ++iTow) {
	bool nomaskTT = true;
	for (it=towerMap.begin();it!=towerMap.end();++it) {
	  if ((*it).second > 0) {
	    EcalTrigTowerDetId  ttId((*it).first);
	    if(ttId.ieta() == _sc_hybrid_TTetaVect[index_sc][iTow] && ttId.iphi() == _sc_hybrid_TTphiVect[index_sc][iTow]) {
	      nomaskTT=false;
	    } // if ttId ieta
	  } // if ut.second>0
	}//loop trigger towers
	
	if(nomaskTT && _sc_hybrid_TTetVect[index_sc][iTow] > TTetmax2) {
	  iTTmax2 = iTow;
	  TTetmax2 = _sc_hybrid_TTetVect[index_sc][iTow];
	} // if nomaskTT
      } // for loop on towers
      
      //int TTetamax = getGCTRegionEta(_sc_hybrid_TTetaVect[index_sc][iTTmax]);
      //int TTphimax = getGCTRegionPhi(_sc_hybrid_TTphiVect[index_sc][iTTmax]);
      //_sc_hybrid_RCTeta[index_sc]=TTetamax;
      //_sc_hybrid_RCTphi[index_sc]=TTphimax;
      
      //for(int il1=0; il1<_trig_L1emIso_N; ++il1) {
      //if(_trig_L1emIso_iphi[il1] == TTphimax && _trig_L1emIso_ieta[il1] == TTetamax) _sc_hybrid_RCTL1iso[index_sc] = _trig_L1emIso_rank[il1];
      //}
      //for(int il1=0; il1<_trig_L1emNonIso_N; ++il1) {
      //if(_trig_L1emNonIso_iphi[il1] == TTphimax && _trig_L1emNonIso_ieta[il1] == TTetamax) _sc_hybrid_RCTL1noniso[index_sc] = _trig_L1emNonIso_rank[il1];
      //}
      
      if(iTTmax2>=0) {
	int TTetamax2 = getGCTRegionEta(_sc_hybrid_TTetaVect[index_sc][iTTmax2]);
	int TTphimax2 = getGCTRegionPhi(_sc_hybrid_TTphiVect[index_sc][iTTmax2]);
	_sc_hybrid_RCTeta[index_sc] = TTetamax2;
	_sc_hybrid_RCTphi[index_sc] = TTphimax2;
	
	for(int il1=0; il1<_trig_L1emIso_N; ++il1) {
	  if(_trig_L1emIso_iphi[il1] == TTphimax2 && _trig_L1emIso_ieta[il1] == TTetamax2) _sc_hybrid_RCTL1iso[index_sc] = _trig_L1emIso_rank[il1];
	}
	for(int il1=0; il1<_trig_L1emNonIso_N; ++il1) {
	  if(_trig_L1emNonIso_iphi[il1] == TTphimax2 && _trig_L1emNonIso_ieta[il1] == TTetamax2) _sc_hybrid_RCTL1noniso[index_sc] = _trig_L1emNonIso_rank[il1];
	}
      } // if iTTmax2
    }//!AOD	
    index_sc++;
  } // for loop on super clusters
  
  if(index_sc>24) { _sc_hybrid_N = 25; cout << "Number of SuperCluster > 25; _sc_hybrid_N set to 25" << endl;}
  
  //	std::cout << "FillSuperClusters  EE SuperCluster  " << std::endl;
  
  // -----------------
  //  EE SuperCluster
  // -----------------
  edm::Handle<reco::SuperClusterCollection> sc_coll_EE;
  iEvent.getByLabel(edm::InputTag("correctedMulti5x5SuperClustersWithPreshower"), sc_coll_EE);
  
  _sc_multi55_N = sc_coll_EE->size();
  
  //std::cout << " size EE = " << sc_coll_EE->size() << std::endl;
  
  int index_sc_EE = 0;
  
  for( reco::SuperClusterCollection::const_iterator isc=sc_coll_EE->begin(); isc!=sc_coll_EE->end(); isc++) { 
    if(index_sc_EE>24) continue;
    
    const EcalRecHitCollection * reducedRecHits = 0 ;
    reducedRecHits = reducedEERecHits.product() ; 
    //seed cluster analysis
    const reco::CaloCluster & seedCluster1 = *(isc->seed());
    
    // 4-vector
    double R  = TMath::Sqrt(isc->x()*isc->x() + isc->y()*isc->y() +isc->z()*isc->z());
    double Rt = TMath::Sqrt(isc->x()*isc->x() + isc->y()*isc->y());
    
    _sc_multi55_E[index_sc_EE]   = isc->energy();
    _sc_multi55_Et[index_sc_EE]  = isc->energy()*(Rt/R);
    _sc_multi55_Eta[index_sc_EE] = isc->eta();
    _sc_multi55_Phi[index_sc_EE] = isc->phi();
    
    // H/E
    reco::SuperCluster EmSCCand = *isc;
    double HoE = towerIso1_->getTowerESum(&EmSCCand) + towerIso2_->getTowerESum(&EmSCCand) ;
    HoE /= 	isc->energy() ;     
    _sc_multi55_he[index_sc_EE] = HoE;
    
    // SigmaIetaIeta
    std::vector<float> localCovariances = EcalClusterTools::localCovariances(seedCluster1,reducedRecHits,topology) ;
    _sc_multi55_sigmaietaieta[index_sc_EE]  = sqrt(localCovariances[0]) ;
		
    // Iso HCAL
    _sc_multi55_hcalDepth1TowerSumEt_dr03[index_sc_EE] = hadDepth1Isolation03.getTowerEtSum(&EmSCCand);
    _sc_multi55_hcalDepth2TowerSumEt_dr03[index_sc_EE] = hadDepth2Isolation03.getTowerEtSum(&EmSCCand);
    
    // Iso ECAL
  //  EcalRecHitMetaCollection ecalEndcapHits(*reducedEERecHits);
    //for42x
    //    EgammaRecHitIsolation ecalEndcapIsol03(egIsoConeSizeOutSmall,egIsoConeSizeInEndcap,egIsoJurassicWidth,egIsoPtMinEndcap,egIsoEMinEndcap,theCaloGeom,&ecalEndcapHits,DetId::Ecal);
    EgammaRecHitIsolation ecalEndcapIsol03(egIsoConeSizeOutSmall,egIsoConeSizeInEndcap,egIsoJurassicWidth,
                                           egIsoPtMinEndcap,egIsoEMinEndcap,theCaloGeom_,*reducedEERecHits,sevLevel.product(),DetId::Ecal);
//    EgammaRecHitIsolation ecalEndcapIsol03(egIsoConeSizeOutSmall,egIsoConeSizeInEndcap,egIsoJurassicWidth,egIsoPtMinEndcap,egIsoEMinEndcap,theCaloGeom,&ecalEndcapHits,sevLevel.product(),DetId::Ecal);
    ecalEndcapIsol03.setUseNumCrystals(useNumCrystals);
    ecalEndcapIsol03.setVetoClustered(vetoClustered);
    // ugly...
    reco::RecoEcalCandidate * cand = new RecoEcalCandidate();
    math::XYZPoint v(0,0,0); math::XYZVector p = isc->energy() * (isc->position() -v).unit(); double t = sqrt(0. + p.mag2());
    cand->setCharge(0); cand->setVertex(v);
    cand->setP4(reco::Candidate::LorentzVector(p.x(), p.y(), p.z(), t));		
    const reco::SuperClusterRef sc_ref(sc_coll_EE, index_sc_EE);
    cand->setSuperCluster(sc_ref);
		
    _sc_multi55_ecalRecHitSumEt_dr03[index_sc_EE] = ecalEndcapIsol03.getEtSum(cand);
    
    // Track Isolation
    // Calculate hollow cone track isolation, CONE 0.3
    reco::Photon * newPhoton = new Photon(); 
    newPhoton->setVertex(v); newPhoton->setCharge(0); newPhoton->setMass(0);
    newPhoton->setP4(reco::Candidate::LorentzVector(p.x(), p.y(), p.z(), isc->energy()));
    
    //int ntrk_03 = 0.; 
    double trkiso_hc_03 = 0.;
    ///int counter = 0;
    double ptSum = 0.;
    
    PhotonTkIsolation phoIso(TrackConeOuterRadiusB_Endcap, //RCone, 
			     TrackConeInnerRadiusB_Endcap, //RinnerCone, 
			     isolationtrackEtaSliceB_Endcap, //etaSlice,  
			     isolationtrackThresholdB_Endcap, //pTThresh, 
			     longImpactParameterB_Endcap, //lip , 
			     transImpactParameterB_Endcap, //d0, 
			     ctfTracksH.product(), //trackCollection, ctfTracksH.product(),bs.position()
			     bs.position());       //math::XYZPoint(vertexBeamSpot.x0(),vertexBeamSpot.y0(),vertexBeamSpot.z0()));
    
    ///counter  = phoIso.getIso(newPhoton).first;
    ptSum    = phoIso.getIso(newPhoton).second;
    trkiso_hc_03 = ptSum;
    
    _sc_multi55_trkiso_dr03[index_sc_EE] = trkiso_hc_03;
    
    //std::cout << "FillSuperClusters  SC EE -- modif-alex l1 matching  " << std::endl;
    
    
    // ____________________________
    // SC EE -- modif-alex l1 matching
    // LOOP MATCHING ON L1 trigger 
    // ____________________________
    if(!aod_){
      nTow = 0;
      nReg = 0;
      for(int icc = 0; icc < 50; ++icc) {
	_sc_multi55_TTetaVect[index_sc_EE][icc] = -999;
	_sc_multi55_TTphiVect[index_sc_EE][icc] = -999;
	_sc_multi55_TTetVect[index_sc_EE][icc]  = 0.;
      }
      for(int icc = 0; icc < 10; ++icc) {
	_sc_multi55_RCTetaVect[index_sc_EE][icc]      = -999;
	_sc_multi55_RCTphiVect[index_sc_EE][icc]      = -999;
	_sc_multi55_RCTetVect[index_sc_EE][icc]       = 0.;
	_sc_multi55_RCTL1isoVect[index_sc_EE][icc]    = -999;
	_sc_multi55_RCTL1nonisoVect[index_sc_EE][icc] = -999;
      }
      
      for (reco::CaloCluster_iterator clus = isc->clustersBegin () ;
	   clus != isc->clustersEnd () ;
	   ++clus){
	std::vector<std::pair<DetId, float> > clusterDetIds = (*clus)->hitsAndFractions() ; //get these from the cluster                                            
	//loop on xtals in cluster                                                                                                                                  
	for (std::vector<std::pair<DetId, float> >::const_iterator detitr = clusterDetIds.begin () ;
	     detitr != clusterDetIds.end () ;
	     ++detitr)
	  {
	    //Here I use the "find" on a digi collection... I have been warned...                                                                                   
	    if ( (detitr -> first).det () != DetId::Ecal)
	      {
		std::cout << " det is " << (detitr -> first).det () << std::endl ;
		continue ;
	      }
	    EcalRecHitCollection::const_iterator thishit;
	    EcalRecHit myhit;
	    EcalTrigTowerDetId towid;
	    float thetahit;
	    // 	    if ( (detitr -> first).subdetId () == EcalEndcap)
	    // 	      {
	    thishit = reducedRecHits->find ( (detitr -> first) ) ;
	    if (thishit == reducedRecHits->end ()) continue;
	    myhit = (*thishit) ;
	    EEDetId detid(thishit->id());
	    towid= (*eTTmap_).towerOf(detid);
	    thetahit =  theEndcapGeometry_->getGeometry((detitr -> first))->getPosition().theta();
	    // 	      }
	    // 	    else continue;
	    // 	  }//endcap rechit
	    
	    int iETA=towid.ieta();
	    int iPHI=towid.iphi();
	    int iReta=getGCTRegionEta(iETA);
	    int iRphi=getGCTRegionPhi(iPHI);
	    double iET=myhit.energy()*sin(thetahit);
	    
	    bool newTow = true;
	    if(nTow>0) {
	      for (int iTow=0; iTow<nTow; ++iTow) {
		if(_sc_multi55_TTetaVect[index_sc_EE][iTow] == iETA && _sc_multi55_TTphiVect[index_sc_EE][iTow] == iPHI) {
		  newTow = false;
		  _sc_multi55_TTetVect[index_sc_EE][iTow] +=  iET;
		}
	      }
	    } // if nTow>0
	    if(newTow) {
	      _sc_multi55_TTetaVect[index_sc_EE][nTow] = iETA;
	      _sc_multi55_TTphiVect[index_sc_EE][nTow] = iPHI;
	      _sc_multi55_TTetVect[index_sc_EE][nTow] =  iET;
	      nTow++;
	    } // if newTow
	    
	    bool newReg = true;
	    if(nReg>0) {
	      for (int iReg=0; iReg<nReg; ++iReg) {
		if(_sc_multi55_RCTetaVect[index_sc_EE][iReg] == iReta && _sc_multi55_RCTphiVect[index_sc_EE][iReg] == iRphi) {
		  newReg = false;
		  _sc_multi55_RCTetVect[index_sc_EE][iReg] +=  iET;
		}
	      }
	    } // if newreg>0
	    
	    if(newReg) {
	      _sc_multi55_RCTetaVect[index_sc_EE][nReg] = iReta;
	      _sc_multi55_RCTphiVect[index_sc_EE][nReg] = iRphi;
	      _sc_multi55_RCTetVect[index_sc_EE][nReg] =  iET;
	      
	      for(int il1=0; il1<_trig_L1emIso_N; ++il1) {
		if(_trig_L1emIso_iphi[il1] == iRphi && _trig_L1emIso_ieta[il1] == iReta) _sc_multi55_RCTL1isoVect[index_sc_EE][nReg] = _trig_L1emIso_rank[il1];
	      }
	      for(int il1=0; il1<_trig_L1emNonIso_N; ++il1) {
		if(_trig_L1emNonIso_iphi[il1] == iRphi && _trig_L1emNonIso_ieta[il1] == iReta) _sc_multi55_RCTL1nonisoVect[index_sc_EE][nReg] = _trig_L1emNonIso_rank[il1];
	      }
	      nReg++;
	    } // if newReg
	  }//loop crystal
      }//loop cluster
      
      //double TTetmax=0.;
      //int iTTmax=-1;
      double TTetmax2 = 0.;
      int iTTmax2     = -1;
      
      for (int iTow=0; iTow<nTow; ++iTow) {
	bool nomaskTT = true;
	for (it=towerMap.begin();it!=towerMap.end();++it) {
	  if ((*it).second > 0) {
	    EcalTrigTowerDetId  ttId((*it).first);
	    if(ttId.ieta() == _sc_multi55_TTetaVect[index_sc_EE][iTow] && ttId.iphi() == _sc_multi55_TTphiVect[index_sc_EE][iTow]) {
	      nomaskTT=false;
	    } // if ttId ieta
	  } // if ut.second>0
	}//loop trigger towers
	
	if(nomaskTT && _sc_multi55_TTetVect[index_sc_EE][iTow] > TTetmax2) {
	  iTTmax2 = iTow;
	  TTetmax2 = _sc_multi55_TTetVect[index_sc_EE][iTow];
	} // if nomaskTT
      } // for loop on towers
      
      if(iTTmax2>=0) {
	int TTetamax2 = getGCTRegionEta(_sc_multi55_TTetaVect[index_sc_EE][iTTmax2]);
	int TTphimax2 = getGCTRegionPhi(_sc_multi55_TTphiVect[index_sc_EE][iTTmax2]);
	_sc_multi55_RCTeta[index_sc_EE] = TTetamax2;
	_sc_multi55_RCTphi[index_sc_EE] = TTphimax2;
	
	for(int il1=0; il1<_trig_L1emIso_N; ++il1) {
	  if(_trig_L1emIso_iphi[il1] == TTphimax2 && _trig_L1emIso_ieta[il1] == TTetamax2) _sc_multi55_RCTL1iso[index_sc_EE] = _trig_L1emIso_rank[il1];
	}
	for(int il1=0; il1<_trig_L1emNonIso_N; ++il1) {
	  if(_trig_L1emNonIso_iphi[il1] == TTphimax2 && _trig_L1emNonIso_ieta[il1] == TTetamax2) _sc_multi55_RCTL1noniso[index_sc_EE] = _trig_L1emNonIso_rank[il1];
	}
      } // if iTTmax2
    }
    
    index_sc_EE++;
  } // for loop on super clusters
  
  if(index_sc_EE>24) { _sc_multi55_N = 25; cout << "Number of SuperCluster > 25; _sc_multi55_N set to 25" << endl;}
	
} // FillSuperCluster 


// ====================================================================================
void SpikeStudy::Init()
// ====================================================================================
{
	
  nEvent = 0;
  nRun = 0;
  nLumi = 0;

  //Pile-up
  _PU_N     = 0;
  _PU_rho   = 0.;
  _PU_sigma = 0.;
	
	
  // Vertices
  _vtx_N = 0; 
  for(int iv=0;iv<35;iv++) {
    _vtx_normalizedChi2[iv] = 0.;
    _vtx_ndof[iv] = 0.;
    _vtx_nTracks[iv] = 0.;
    _vtx_d0[iv] = 0.;
    _vtx_x[iv] = 0.;
    _vtx_y[iv] = 0.;
    _vtx_z[iv] = 0.;
  }// for loop on vertices
	
  // Beam Spot
  BS_x = 0.;
  BS_y = 0.;
  BS_z = 0.;
	
  BS_dz = 0.;
  BS_dxdz = 0.;
  BS_dydz = 0.;
	
  BS_bw_x = 0.;
  BS_bw_y = 0.;
	
  // MC truth
  _MC_pthat  = 0.;
  _MC_flavor[0] = 10;
  _MC_flavor[1] = 10;
	
  // Trigger towers
  // L1
  _trig_L1emIso_N    = 0; 
  _trig_L1emNonIso_N = 0;
  _trig_L1emIso_N_M    = 0; 
  _trig_L1emNonIso_N_M = 0;
  _trig_preL1emIso_N    = 0; 
  _trig_preL1emNonIso_N = 0;
  _trig_postL1emIso_N    = 0; 
  _trig_postL1emNonIso_N = 0;
  // max set to 4
  for(int il1=0;il1<4;il1++) {
    // Used by Clemy
    _trig_L1emIso_ieta[il1] = 0; 
    _trig_L1emIso_iphi[il1] = 0; 
    _trig_L1emIso_rank[il1] = 0; 
    _trig_L1emIso_ieta_M[il1] = 0; 
    _trig_L1emIso_iphi_M[il1] = 0; 
    _trig_L1emIso_rank_M[il1] = 0; 
     // From Trigger twiki
    _trig_L1emIso_eta[il1]    = 0.; 
    _trig_L1emIso_phi[il1]   = 0.; 
    _trig_L1emIso_energy[il1] = 0.; 
    _trig_L1emIso_et[il1]     = 0.; 
    _trig_L1emIso_eta_M[il1]    = 0.; 
    _trig_L1emIso_phi_M[il1]   = 0.; 
    _trig_L1emIso_energy_M[il1] = 0.; 
    _trig_L1emIso_et_M[il1]     = 0.; 
		
    // Used by Clemy
    _trig_L1emNonIso_ieta[il1] = 0; 
    _trig_L1emNonIso_iphi[il1] = 0; 
    _trig_L1emNonIso_rank[il1] = 0; 
    _trig_L1emNonIso_ieta_M[il1] = 0; 
    _trig_L1emNonIso_iphi_M[il1] = 0; 
    _trig_L1emNonIso_rank_M[il1] = 0; 
    // From Trigger twiki
    _trig_L1emNonIso_eta[il1]    = 0.; 
    _trig_L1emNonIso_phi[il1]   = 0.; 
    _trig_L1emNonIso_energy[il1] = 0.; 
    _trig_L1emNonIso_et[il1]     = 0.; 
    _trig_L1emNonIso_eta_M[il1]    = 0.; 
    _trig_L1emNonIso_phi_M[il1]   = 0.; 
    _trig_L1emNonIso_energy_M[il1] = 0.; 
    _trig_L1emNonIso_et_M[il1]     = 0.; 
		
    // Used by Clemy
    _trig_preL1emIso_ieta[il1] = 0; 
    _trig_preL1emIso_iphi[il1] = 0; 
    _trig_preL1emIso_rank[il1] = 0;
    // Used by Clemy
    _trig_preL1emNonIso_ieta[il1] = 0; 
    _trig_preL1emNonIso_iphi[il1] = 0; 
    _trig_preL1emNonIso_rank[il1] = 0; 
		
    // Used by Clemy
    _trig_postL1emIso_ieta[il1] = 0; 
    _trig_postL1emIso_iphi[il1] = 0; 
    _trig_postL1emIso_rank[il1] = 0;
    // Used by Clemy
    _trig_postL1emNonIso_ieta[il1] = 0; 
    _trig_postL1emNonIso_iphi[il1] = 0; 
    _trig_postL1emNonIso_rank[il1] = 0;  
		
		
  } // for loop on L1 cand
	
	
  for (int ii=0;ii<100;ii++)
    {
      _trig_iMaskedRCTeta[ii]   = -999;
      _trig_iMaskedRCTphi[ii]   = -999;
      _trig_iMaskedRCTcrate[ii] = -999;
      _trig_iMaskedTTeta[ii]    = -999;
      _trig_iMaskedTTphi[ii]    = -999;
    }//loop  masks
	
	
  // SuperClusters
  _sc_hybrid_N = 0; 
  for(int isc=0;isc<25;isc++) {
    _sc_hybrid_E[isc]   = 0.; 
    _sc_hybrid_Et[isc]  = 0.; 
    _sc_hybrid_Eta[isc] = 0.; 
    _sc_hybrid_Phi[isc] = 0.; 
    _sc_hybrid_outOfTimeSeed[isc]     = 0;
    _sc_hybrid_severityLevelSeed[isc] = 0;
    _sc_hybrid_e1[isc]  = 0.;
    _sc_hybrid_e33[isc] = 0.;
    _sc_hybrid_he[isc]  = -10.;
    _sc_hybrid_sigmaietaieta[isc] = 0.;
    _sc_hybrid_hcalDepth1TowerSumEt_dr03[isc] = 0.;
    _sc_hybrid_hcalDepth2TowerSumEt_dr03[isc] = 0.;
    _sc_hybrid_ecalRecHitSumEt_dr03[isc]      = 0.;
    _sc_hybrid_trkiso_dr03[isc]               = 0.;
		
    _sc_hybrid_RCTeta[isc]=-999;
    _sc_hybrid_RCTphi[isc]=-999;
    _sc_hybrid_RCTL1iso[isc]     = -999;
    _sc_hybrid_RCTL1noniso[isc]  = -999;
		
    for (int li=0;li<50;li++) {
      _sc_hybrid_TTetaVect[isc][li]=-999;
      _sc_hybrid_TTphiVect[isc][li]=-999;
      _sc_hybrid_TTetVect[isc][li]=0.;
    } // for loop on 50
    for (int li=0;li<10;li++) {
      _sc_hybrid_RCTetaVect[isc][li]=-999;
      _sc_hybrid_RCTphiVect[isc][li]=-999;
      _sc_hybrid_RCTetVect[isc][li]=0.;
      _sc_hybrid_RCTL1isoVect[isc][li]=-999;
      _sc_hybrid_RCTL1nonisoVect[isc][li]=-999;
    } // for loop on 10
		
  } // for loop on EB superclusters
	
  _sc_multi55_N = 0; 
  for(int isc=0;isc<25;isc++) {
    _sc_multi55_E[isc]   = 0.; 
    _sc_multi55_Et[isc]  = 0.; 
    _sc_multi55_Eta[isc] = 0.; 
    _sc_multi55_Phi[isc] = 0.; 
    _sc_multi55_he[isc]  = -10.;
    _sc_multi55_sigmaietaieta[isc] = 0.;
    _sc_multi55_hcalDepth1TowerSumEt_dr03[isc] = 0.;
    _sc_multi55_hcalDepth2TowerSumEt_dr03[isc] = 0.;
    _sc_multi55_ecalRecHitSumEt_dr03[isc]      = 0.;
    _sc_multi55_trkiso_dr03[isc]               = 0.;
		
    _sc_multi55_RCTeta[isc]=-999;
    _sc_multi55_RCTphi[isc]=-999;
    _sc_multi55_RCTL1iso[isc]     = -999;
    _sc_multi55_RCTL1noniso[isc]  = -999;
		
    for (int li=0;li<50;li++) {
      _sc_multi55_TTetaVect[isc][li]=-999;
      _sc_multi55_TTphiVect[isc][li]=-999;
      _sc_multi55_TTetVect[isc][li]=0.;
    } // for loop on 50
    for (int li=0;li<10;li++) {
      _sc_multi55_RCTetaVect[isc][li]=-999;
      _sc_multi55_RCTphiVect[isc][li]=-999;
      _sc_multi55_RCTetVect[isc][li]=0.;
      _sc_multi55_RCTL1isoVect[isc][li]=-999;
      _sc_multi55_RCTL1nonisoVect[isc][li]=-999;
    } // for loop on 10
		
  } // for loop on EE superclusters
	
	
}

// ====================================================================================
void SpikeStudy::beginJob(const edm::ParameterSet& conf)
// ====================================================================================
{
  //hcalhelper_ = new ElectronHcalHelper(conf);
  //edm::Ref<reco::GsfElectronCollection> electronEdmRef(EleHandle,i);
  
  //	reco::SuperCluster EmSCCand1; // = *isc;
  //reco::RecoEcalCandidate EcalCand;
  
  //sc_struct = new converter::SuperClusterToCandidate(conf);
	
}

// ====================================================================================
void SpikeStudy::endJob() {}
// ====================================================================================

// ====================================================================================
void SpikeStudy::setMomentum (TLorentzVector &myvector, const LorentzVector & mom)
// ====================================================================================
{
	myvector.SetPx (mom.Px());
	myvector.SetPy (mom.Py());
	myvector.SetPz (mom.Pz());
	myvector.SetE (mom.E());
}

// ====================================================================================
bool SpikeStudy::IsConv (const reco::GsfElectron & eleRef) //edm::Ref<reco::GsfElectronCollection> eleRef)
// ====================================================================================
{
	
	bool isAmbiguous = true, isNotFromPixel = true;
	if (eleRef.ambiguousGsfTracksSize() == 0 ) {isAmbiguous = false;}
	
	/*
	 TrackingRecHitRef rhit =  eleRef->gsfTrack()->extra()->recHit(0);
	 int subdetId = rhit->geographicalId().subdetId();
	 int layerId  = 0;
	 DetId id = rhit->geographicalId();
	 if (id.subdetId()==3) layerId = ((TIBDetId)(id)).layer();
	 if (id.subdetId()==5) layerId = ((TOBDetId)(id)).layer();
	 if (id.subdetId()==1) layerId = ((PXBDetId)(id)).layer();
	 if (id.subdetId()==4) layerId = ((TIDDetId)(id)).wheel();
	 if (id.subdetId()==6) layerId = ((TECDetId)(id)).wheel();
	 if (id.subdetId()==2) layerId = ((PXFDetId)(id)).disk();
	 //std::cout << " subdetIdele layerIdele = " << id.subdetId() << "   " << layerId << std::endl;
	 
	 if ((id.subdetId()==1 && layerId == 1) || (id.subdetId()==2 && layerId == 1)) {isNotFromPixel = false;}
	 */
	
//	int  mishits = eleRef.gsfTrack()->trackerExpectedHitsInner().numberOfHits(reco::HitPattern::MISSING_INNER_HITS);
	int  mishits = eleRef.gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS);
	
	//std::cout << "mishits = " << mishits << std::endl;
	if (mishits == 0){isNotFromPixel = false;}
	
	// 
	bool is_conversion = false;
	
	if(isAmbiguous || isNotFromPixel) is_conversion = true;
	
	return is_conversion;
	
}

// ====================================================================================
const EcalRecHit SpikeStudy::getRecHit(DetId id, const EcalRecHitCollection *recHits)
// ====================================================================================
{
	if ( id == DetId(0) ) {
		return EcalRecHit();
	} else {
		EcalRecHitCollection::const_iterator it = recHits->find( id );
		if ( it != recHits->end() ) {
			return (*it);
		} else {
			//throw cms::Exception("EcalRecHitNotFound") << "The recHit corresponding to the DetId" << id.rawId() << " not found in the EcalRecHitCollection";
			// the recHit is not in the collection (hopefully zero suppressed)
			return EcalRecHit();
		}
	}
	return EcalRecHit();
}



//modif-alex
//GETTING RCT regions
// ====================================================================================
int SpikeStudy::getGCTRegionPhi(int ttphi)
// ====================================================================================
{
	int gctphi=0;
	gctphi = (ttphi+1)/4;
	if(ttphi<=2) gctphi=0;
	if(ttphi>=71) gctphi=0;
	
	return gctphi;
}

// ====================================================================================
int SpikeStudy::getGCTRegionEta(int tteta)
// ====================================================================================
{
	int gcteta = 0;
	
	if(tteta>0) gcteta = (tteta-1)/4 + 11;
	else if(tteta<0) gcteta = (tteta+1)/4 + 10;
	
	return gcteta;
}
/*
// ===============================================================================================
// unified acces to isolations
std::pair<int,double> ElectronTkIsolation::getIso(const reco::GsfElectron* electron) const  
// ===============================================================================================
{
	int counter  =0 ;
	double ptSum =0.;
	//Take the electron track
	reco::GsfTrackRef tmpTrack = electron->gsfTrack() ;
	math::XYZVector tmpElectronMomentumAtVtx = (*tmpTrack).momentum () ; 
	double tmpElectronEtaAtVertex = (*tmpTrack).eta();
	
	
	for ( reco::TrackCollection::const_iterator itrTr  = (*trackCollection_).begin() ; 
		 itrTr != (*trackCollection_).end()   ; 
		 ++itrTr ) {
		
		math::XYZVector tmpTrackMomentumAtVtx = (*itrTr).momentum () ; 
		
		double this_pt  = (*itrTr).pt();
		if ( this_pt < ptLow_ ) continue;
		
		double dzCut = 0;
		switch( dzOption_ ) {
			case egammaisolation::EgammaTrackSelector::dz : dzCut = fabs( (*itrTr).dz() - (*tmpTrack).dz() ); break;
			case egammaisolation::EgammaTrackSelector::vz : dzCut = fabs( (*itrTr).vz() - (*tmpTrack).vz() ); break;
			case egammaisolation::EgammaTrackSelector::bs : dzCut = fabs( (*itrTr).dz(beamPoint_) - (*tmpTrack).dz(beamPoint_) ); break;
			case egammaisolation::EgammaTrackSelector::vtx: dzCut = fabs( (*itrTr).dz(tmpTrack->vertex()) ); break;
			default : dzCut = fabs( (*itrTr).vz() - (*tmpTrack).vz() ); break;
		}
		if (dzCut > lip_ ) continue;
		if (fabs( (*itrTr).dxy(beamPoint_) ) > drb_   ) continue;
		double dr = ROOT::Math::VectorUtil::DeltaR(itrTr->momentum(),tmpElectronMomentumAtVtx) ;
		double deta = (*itrTr).eta() - tmpElectronEtaAtVertex;
		if (fabs(tmpElectronEtaAtVertex) < 1.479) { 
			if ( fabs(dr) < extRadius_ && fabs(dr) >= intRadiusBarrel_ && fabs(deta) >= stripBarrel_)
			{
				++counter ;
				ptSum += this_pt;
			}
		}
		else {
			if ( fabs(dr) < extRadius_ && fabs(dr) >= intRadiusEndcap_ && fabs(deta) >= stripEndcap_)
			{
				++counter ;
				ptSum += this_pt;
			}
		}
		
	}//end loop over tracks                 
	
	std::pair<int,double> retval;
	retval.first  = counter;
	retval.second = ptSum;
	
	return retval;
} // end of get TrkIso
*/



// ====================================================================================
float SpikeStudy::E2overE9( const DetId id, const EcalRecHitCollection & recHits, 
			     float recHitEtThreshold, float recHitEtThreshold2 , 
			     bool avoidIeta85, bool KillSecondHit)
// ====================================================================================
// taken from CMSSW/RecoLocalCalo/EcalRecAlgos/src/EcalSeverityLevelAlgo.cc CMSSW_3_9_0_pre5

{

        // compute e2overe9
        //  
        //   | | | |
        //   +-+-+-+
        //   | |1|2|
        //   +-+-+-+
        //   | | | |
        //
        //   1 - input hit,  2 - highest energy hit in a 3x3 around 1
        // 
        //   rechit 1 must have E_t > recHitEtThreshold
        //   rechit 2 must have E_t > recHitEtThreshold2
        //
        //   function returns value of E2/E9 centered around 1 (E2=energy of hits 1+2) if energy of 1>2
        //
        //   if energy of 2>1 and KillSecondHit is set to true, function returns value of E2/E9 centered around 2
        //   *provided* that 1 is the highest energy hit in a 3x3 centered around 2, otherwise, function returns 0


        if ( id.subdetId() == EcalBarrel ) {
	  
                EBDetId ebId( id );

                // avoid recHits at |eta|=85 where one side of the neighbours is missing
                if ( abs(ebId.ieta())==85 && avoidIeta85){  return 0;}

                // select recHits with Et above recHitEtThreshold

 
                float e1 = recHitE( id, recHits );
		

                float ete1=recHitApproxEt( id, recHits );


		// check that rechit E_t is above threshold

		if (ete1 < std::min(recHitEtThreshold,recHitEtThreshold2) ) { return 0;}
		
		if (ete1 < recHitEtThreshold && !KillSecondHit ) {return 0;}
		

                float e2=-1;
                float ete2=0;
                float s9 = 0;

                // coordinates of 2nd hit relative to central hit
                int e2eta=0;
                int e2phi=0;

		// LOOP OVER 3x3 ARRAY CENTERED AROUND HIT 1

                for ( int deta = -1; deta <= +1; ++deta ) {
                   for ( int dphi = -1; dphi <= +1; ++dphi ) {
 
		      // compute 3x3 energy

                      float etmp=recHitE( id, recHits, deta, dphi );
                      s9 += etmp;

                      EBDetId idtmp=EBDetId::offsetBy(id,deta,dphi);
                      float eapproxet=recHitApproxEt( idtmp, recHits );

                      // remember 2nd highest energy deposit (above threshold) in 3x3 array 
                      if (etmp>e2 && eapproxet>recHitEtThreshold2 && !(deta==0 && dphi==0)) {

                         e2=etmp;
                         ete2=eapproxet;
                         e2eta=deta;
                         e2phi=dphi;
        
                      }

                   }
                }

                if ( e1 == 0 )  { return 0;}
  
                // return 0 if 2nd hit is below threshold
                if ( e2 == -1 ) {return 0;}

                // compute e2/e9 centered around 1st hit

                float e2nd=e1+e2;
                float e2e9=0;

                if (s9!=0) e2e9=e2nd/s9;
  
                // if central hit has higher energy than 2nd hit
                //  return e2/e9 if 1st hit is above E_t threshold

                if (e1 > e2 && ete1>recHitEtThreshold) return e2e9;

                // if second hit has higher energy than 1st hit

                if ( e2 > e1 ) { 


                  // return 0 if user does not want to flag 2nd hit, or
                  // hits are below E_t thresholds - note here we
		  // now assume the 2nd hit to be the leading hit.

		  if (!KillSecondHit || ete2<recHitEtThreshold || ete1<recHitEtThreshold2) {
		    
                     return 0;
  
                 }


                  else {
 
                    // LOOP OVER 3x3 ARRAY CENTERED AROUND HIT 2

		    float s92nd=0;
           
                    float e2nd_prime=0;
                    int e2prime_eta=0;
                    int e2prime_phi=0;

                    EBDetId secondid=EBDetId::offsetBy(id,e2eta,e2phi);


                     for ( int deta = -1; deta <= +1; ++deta ) {
                        for ( int dphi = -1; dphi <= +1; ++dphi ) {
 
		           // compute 3x3 energy

                           float etmp=recHitE( secondid, recHits, deta, dphi );
                           s92nd += etmp;

                           if (etmp>e2nd_prime && !(deta==0 && dphi==0)) {
			     e2nd_prime=etmp;
                             e2prime_eta=deta;
                             e2prime_phi=dphi;
			   }

			}
		     }

		     // if highest energy hit around E2 is not the same as the input hit, return 0;

		     if (!(e2prime_eta==-e2eta && e2prime_phi==-e2phi)) 
		       { 
			 return 0;
		       }


		     // compute E2/E9 around second hit 
		     float e2e9_2=0;
		     if (s92nd!=0) e2e9_2=e2nd/s92nd;
                 
		     //   return the value of E2/E9 calculated around 2nd hit
                   
		     return e2e9_2;


		  }
		  
		}


        } else if ( id.subdetId() == EcalEndcap ) {
	  // only used for EB at the moment
          return 0;
        }
        return 0;
}



// ====================================================================================
float SpikeStudy::recHitE( const DetId id, const EcalRecHitCollection &recHits )
// ====================================================================================
{
        if ( id == DetId(0) ) {
                return 0;
        } else {
                EcalRecHitCollection::const_iterator it = recHits.find( id );
                if ( it != recHits.end() ) return (*it).energy();
        }
        return 0;
}


// ====================================================================================
float SpikeStudy::recHitE( const DetId id, const EcalRecHitCollection & recHits,
                                           int di, int dj )
// ====================================================================================
{
        // in the barrel:   di = dEta   dj = dPhi
        // in the endcap:   di = dX     dj = dY
  
        DetId nid;
        if( id.subdetId() == EcalBarrel) nid = EBDetId::offsetBy( id, di, dj );
        else if( id.subdetId() == EcalEndcap) nid = EEDetId::offsetBy( id, di, dj );

        return ( nid == DetId(0) ? 0 : recHitE( nid, recHits ) );
}




// ====================================================================================
float SpikeStudy::recHitApproxEt( const DetId id, const EcalRecHitCollection &recHits )
// ====================================================================================
{
        // for the time being works only for the barrel
        if ( id.subdetId() == EcalBarrel ) {
                return recHitE( id, recHits ) / cosh( EBDetId::approxEta( id ) );
        }
        return 0;
}
DEFINE_FWK_MODULE(SpikeStudy);

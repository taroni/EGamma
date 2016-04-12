 ///g++ -o    makePairs makePairs.cc  -lboost_system `root-config --cflags --libs` -L $ROOFITSYS/lib -lRooFit -lRooFitCore
#include <iostream>
#include <utility>
#include <ostream>
#include <fstream>
#include <sstream>
#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TFrame.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TMath.h>
#include <TStyle.h>
#include <TSystem.h>
#include "baseFuncNad.h"
#include <sys/types.h>
#include <sys/stat.h>
//#include <boost/filesystem.hpp>
#include <iostream>

#define DEBUG 1

struct stat info;

void makeTriggerCand(string file, int nEntries, string dirOut, TString dirIn, TString data, string nameChain, TString RunPhase, bool debug);

int processCand(string nameChain, string file, int iFile, int nEntries, string dirOut, TString dirIn, ofstream& outlog,  TString RunPhase, bool debug);

string myjobid ;
string file;
using namespace std;
//-------
int main(int argc, char**argv){

  //std::cout << "Have " << argc << " arguments:" << std::endl;
  cout << "JOBID: " << argv[1] << endl;
  myjobid = argv[1];
  file = argv[2];
  
  makeTriggerCand(file, -1, "makePairsDir",  "/afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_5_3_28/src/EGamma/ECGelec/test/", "Run2012D_22Jan", "produceNtuple/eIDSimpleTree", "2012D", false) ;
  
  return 0;
}
//------
int processCand(string nameChain, string file, int iFile, int nEntries, string dirOut, TString dirIn, ofstream& outlog, TString RunPhase, bool debug)  {
  
  if (DEBUG) cout << __LINE__<< endl;

  long oldEvent=0; 
  if (DEBUG) cout << __LINE__<< endl;

  TH1F * h_Entries = new TH1F("h_Entries", "number of events", 2,0,2);
  
  TH1F * h_trigIsoCandSpectrum= new TH1F ("L1_IsoCandSpectrum", "L1 Iso Cand Spectrum", 100, 0., 100.); 
  TH1F * h_trigNonIsoCandSpectrum= new TH1F ("L1_NonIsoCandSpectrum", "L1 NonIso Cand Spectrum", 100, 0., 100.); 
  TH1F * h_trigIsoCandSpectrum_M= new TH1F ("L1_IsoCandSpectrum_M", "new L1 Iso Cand Spectrum", 100, 0., 100.); 
  TH1F * h_trigNonIsoCandSpectrum_M= new TH1F ("L1_NonIsoCandSpectrum_M", "new L1 NonIso Cand Spectrum", 100, 0., 100.); 
  TH1F * h_trigIsoCands= new TH1F ("L1_IsoCands", "L1 Iso Cands", 100, 0., 100.); 
  TH1F * h_trigNonIsoCands= new TH1F ("L1_NonIsoCands", "L1 NonIso Cands", 100, 0., 100.); 
  TH1F * h_trigIsoCands_M= new TH1F ("L1_IsoCands_M", "new L1 Iso Cands", 100, 0., 100.); 
  TH1F * h_trigNonIsoCands_M= new TH1F ("L1_NonIsoCands_M", "new L1 NonIso Cands", 100, 0., 100.); 
  TH1F * h_trigAllCands= new TH1F ("L1_AllCands", "L1 All Cands", 100, 0., 100.); 
  TH1F * h_trigAllCands_M= new TH1F ("L1_AllCands_M", "new L1 All Cands", 100, 0., 100.); 
  if (DEBUG) cout << __LINE__<< endl;


  int thresholds[7] ={5, 7, 12, 20, 22, 24, 30};
  vector < pair < string, int > >  thresholdsIsopairs;
  vector < pair < string, int > >  thresholdsNonIsopairs;
  vector < pair < string, int > >  thresholdsIsopairs_M;
  vector < pair < string, int > >  thresholdsNonIsopairs_M;
  if (DEBUG) cout << __LINE__<< endl;
  for (int ithr = 0; ithr<sizeof(thresholds)/sizeof(*thresholds) ; ithr++){
    if (DEBUG) cout << __LINE__<< " size " <<  sizeof(thresholds)/sizeof(*thresholds)<<  endl;
    pair < string, int > mypair;
    string thrname = "EG"+thresholds[ithr];
    if (DEBUG) cout << __LINE__<< " " << ithr <<  endl;
    mypair=std::make_pair(thrname, 0);
    thresholdsIsopairs.push_back(mypair);
  if (DEBUG) cout << __LINE__<< " " << ithr<< endl;
    thresholdsNonIsopairs.push_back(mypair);
  if (DEBUG) cout << __LINE__<< " " << ithr<< endl;
    thresholdsIsopairs_M.push_back(mypair);
  if (DEBUG) cout << __LINE__<< " " << ithr<< endl;
    thresholdsNonIsopairs_M.push_back(mypair);
  if (DEBUG) cout << __LINE__<< " " << ithr<< endl;
  }
  if (DEBUG) cout << __LINE__<< endl;
  
  // OUTPUT FILE //
  std::size_t pos = file.find("tree");
  std::string str2 = file.substr(pos); 
  TString name=(TString)("rate_"+str2);
  TFile *outfile = new TFile(name,"RECREATE");
  // INPUT TREE //
  TChain * myChain = new TChain(nameChain.c_str());
  myChain->Add(file.c_str());
  if (DEBUG) cout << __LINE__<< endl;
  
  int nEvent, nRun, nLumi ;
  // Vertices //
  int _vtx_N;
  double _vtx_x[200], _vtx_y[200], _vtx_z[200];
  double _vtx_normalizedChi2[200], _vtx_ndof[200], _vtx_nTracks[200], _vtx_d0[200];
  
  // Trigger Paths //
  int trig_hltInfo[250];
  int _trig_isEleHLTpath;
  int trig_HLT_path[4]; // unbias, EG5, EG8, EG12
  char trig_fired_names[5000];
  //
  vector<string> m_HLT_pathsV;
  vector<string> m_HLT_triggered;
  vector<int> m_HLT_pathsV_check;

  // Electrons
  TClonesArray * electrons = new TClonesArray ("TLorentzVector");
  int ele_N, sc_hybrid_N; 
  int ele_outOfTimeSeed[10],ele_severityLevelSeed[10];
  double ele_he[10], ele_sigmaietaieta[10];
  double ele_hcalDepth1TowerSumEt_dr03[10], ele_hcalDepth2TowerSumEt_dr03[10];
  double ele_ecalRecHitSumEt_dr03[10], ele_tkSumPt_dr03[10];
  double ele_sclEta[10], ele_sclEt[10];
  //double ecalIsoRel03,hcalIsoRel03,trackIsoRel03;
  double ele_deltaphiin[10], ele_deltaetain[10];
  double ele_conv_dist[10], ele_conv_dcot[10];
  double ele_fbrem[10];
  int ele_expected_inner_hits[10];
  //int ele_ambiguousGsfTracks[10];
  int ele_isConversion[10];
  int ele_echarge[10];
  //
  int ele_RCTeta[10], ele_RCTphi[10], ele_RCTL1iso[10], ele_RCTL1noniso[10], ele_RCTL1iso_M[10], ele_RCTL1noniso_M[10];
  int ele_TTetaVect[10][50], ele_TTphiVect[10][50];
  double ele_TTetVect[10][50];
  int ele_RCTetaVect[10][10], ele_RCTphiVect[10][10], ele_RCTL1isoVect[10][10], 
    ele_RCTL1nonisoVect[10][10],ele_RCTL1isoVect_M[10][10], ele_RCTL1nonisoVect_M[10][10];
  double ele_RCTetVect[10][10];
  if (DEBUG) cout << __LINE__<< endl;

  // TP info
  const int nTow = 4032;
  int trig_tower_N,trig_tower_ieta[nTow],trig_tower_iphi[nTow],trig_tower_adc[nTow],trig_tower_sFGVB[nTow]; 
  int trig_tower_N_modif,trig_tower_ieta_modif[nTow],trig_tower_iphi_modif[nTow],trig_tower_adc_modif[nTow],trig_tower_sFGVB_modif[nTow];
  int trig_tower_N_emul,trig_tower_ieta_emul[nTow],trig_tower_iphi_emul[nTow],trig_tower_adc_emul[nTow][5],trig_tower_sFGVB_emul[nTow][5];

  // HCAL TP
  int trig_tower_hcal_N, trig_tower_hcal_ieta[4032], trig_tower_hcal_iphi[4032], trig_tower_hcal_FG[4032],trig_tower_hcal_et[4032];
  int trig_L1emIso_N, trig_L1emNonIso_N, trig_L1emIso_N_M, trig_L1emNonIso_N_M;

  // L1 candidates info
  int trig_L1emIso_ieta[4], trig_L1emIso_iphi[4], trig_L1emIso_rank[4]; 
  double trig_L1emIso_et[4], trig_L1emNonIso_et[4], trig_L1emIso_et_M[4], trig_L1emNonIso_et_M[4];
  int trig_L1emNonIso_ieta[4], trig_L1emNonIso_iphi[4], trig_L1emNonIso_rank[4];
  int trig_L1emIso_ieta_M[4], trig_L1emIso_iphi_M[4], trig_L1emIso_rank_M[4];
  int trig_L1emNonIso_ieta_M[4], trig_L1emNonIso_iphi_M[4], trig_L1emNonIso_rank_M[4];

  // L1 prefiring
  int trig_preL1emIso_N; 
  int trig_preL1emNonIso_N;
  int trig_preL1emIso_ieta[4], trig_preL1emIso_iphi[4], trig_preL1emIso_rank[4]; 
  int trig_preL1emNonIso_ieta[4], trig_preL1emNonIso_iphi[4],trig_preL1emNonIso_rank[4];
  // L1 postfiring
  int trig_postL1emIso_N; 
  int trig_postL1emNonIso_N;
  int trig_postL1emIso_ieta[4], trig_postL1emIso_iphi[4], trig_postL1emIso_rank[4]; 
  int trig_postL1emNonIso_ieta[4], trig_postL1emNonIso_iphi[4],trig_postL1emNonIso_rank[4];
  
  // Masking
  int trig_nMaskedRCT, trig_nMaskedCh;
  int trig_iMaskedRCTeta[100], trig_iMaskedRCTphi[100], trig_iMaskedRCTcrate[100], trig_iMaskedTTeta[100], trig_iMaskedTTphi[100];

  int trig_strip_mask_N;
  int trig_strip_mask_TTieta[1000], trig_strip_mask_TTiphi[1000], trig_strip_mask_status[1000],
    trig_strip_mask_StripID[1000], trig_strip_mask_PseudoStripID[1000], trig_strip_mask_TccID[1000], trig_strip_mask_CCU[1000],
    trig_strip_mask_xtal_ix[1000][5], trig_strip_mask_xtal_iy[1000][5], trig_strip_mask_xtal_iz[1000][5];

  int trig_xtal_mask_N; // [EB+EE]
  int trig_xtal_mask_ieta[1000],trig_xtal_mask_iphi[1000], // for EE : xtal ieta->ix ; iphi -> iy
    trig_xtal_mask_TTieta[1000],trig_xtal_mask_TTiphi[1000], // but for EE towers, still ieta, iphi...
    trig_xtal_mask_Rieta[1000],trig_xtal_mask_Riphi[1000],
    trig_xtal_mask_status[1000], trig_xtal_mask_EBEE[1000]; // EBEE = {0,1} => 0=EB ; 1=EE
  //double trig_xtal_mask_eT[1000];  

  if (DEBUG) cout << __LINE__<< endl;
  // INITIALIZATION //
  //
  // Global
  nEvent = 0;
  nRun = 0;
  nLumi = 0;
  //
  // Vertices
  _vtx_N = 0; 
  for(int iv=0;iv<200;iv++) {
    _vtx_normalizedChi2[iv] = 0.;
    _vtx_ndof[iv] = 0.;
    _vtx_nTracks[iv] = 0.;
    _vtx_d0[iv] = 0.;
    _vtx_x[iv] = 0.;
    _vtx_y[iv] = 0.;
    _vtx_z[iv] = 0.;
  }
  if (DEBUG) cout << __LINE__<< endl;
  //
  // L1 candidates
  trig_L1emIso_N    = 0; 
  trig_L1emNonIso_N = 0;
  trig_preL1emIso_N   = 0; 
  trig_preL1emNonIso_N  = 0;
  trig_postL1emIso_N    = 0; 
  trig_postL1emNonIso_N = 0;
  //
  for(int il1=0 ; il1<4 ; il1++) {
    trig_L1emIso_ieta[il1] = 0; 
    trig_L1emIso_iphi[il1] = 0; 
    trig_L1emIso_rank[il1] = 0; 
    trig_L1emIso_et[il1] = 0; 
    trig_L1emNonIso_ieta[il1] = 0; 
    trig_L1emNonIso_iphi[il1] = 0; 
    trig_L1emNonIso_rank[il1] = 0; 
    trig_L1emNonIso_et[il1] = 0; 
		
    trig_preL1emIso_ieta[il1] = 0; 
    trig_preL1emIso_iphi[il1] = 0; 
    trig_preL1emIso_rank[il1] = 0;
    trig_preL1emNonIso_ieta[il1] = 0; 
    trig_preL1emNonIso_iphi[il1] = 0; 
    trig_preL1emNonIso_rank[il1] = 0; 
		
    trig_postL1emIso_ieta[il1] = 0; 
    trig_postL1emIso_iphi[il1] = 0; 
    trig_postL1emIso_rank[il1] = 0;
    trig_postL1emNonIso_ieta[il1] = 0; 
    trig_postL1emNonIso_iphi[il1] = 0; 
    trig_postL1emNonIso_rank[il1] = 0;  
  }
  // 
  if (DEBUG) cout << __LINE__<< endl;
  // Trigger towers
  trig_tower_N = 0;
  for(int iTow=0 ; iTow<nTow ; iTow++) {
    trig_tower_ieta[iTow] = trig_tower_iphi[iTow]  = -999;
    trig_tower_adc[iTow]  = trig_tower_sFGVB[iTow] = -999;
  }
  trig_tower_N_modif = 0;
  for(int iTow=0 ; iTow<nTow ; iTow++) {
    trig_tower_ieta_modif[iTow] = trig_tower_iphi_modif[iTow]  = -999;
    trig_tower_adc_modif[iTow]  = trig_tower_sFGVB_modif[iTow] = -999;
  }
  trig_tower_N_emul = 0;
  for(int iTow=0 ; iTow<nTow ; iTow++) {
    trig_tower_ieta_emul[iTow] = trig_tower_iphi_emul[iTow] = -999;
    for(int i=0 ; i<5 ; i++)
      trig_tower_adc_emul[iTow][i] = trig_tower_sFGVB_emul[iTow][i] = -999;
  }
  trig_tower_hcal_N = 0;
  for(int iTow=0 ; iTow<nTow ; iTow++) {
    trig_tower_hcal_ieta[iTow] = trig_tower_hcal_iphi[iTow]  = -999;
    trig_tower_hcal_FG[iTow]  = trig_tower_hcal_et[iTow] = -999;
  }
  //
  // Masked Towers
  trig_nMaskedRCT=0;
  trig_nMaskedCh=0;
  //
  for (int ii=0;ii<100;ii++) {
    trig_iMaskedRCTeta[ii]   = -999;
    trig_iMaskedRCTphi[ii]   = -999;
    trig_iMaskedRCTcrate[ii] = -999;
    trig_iMaskedTTeta[ii]    = -999;
    trig_iMaskedTTphi[ii]    = -999;
  }
  if (DEBUG) cout << __LINE__<< endl;
  //
  // Masked strip/xtals
  trig_strip_mask_N = 0;
  trig_xtal_mask_N = 0;
  //
  for(int i=0 ; i<1000 ; i++) {
    trig_strip_mask_TTieta[i] = -999;
    trig_strip_mask_TTiphi[i] = -999;
    trig_strip_mask_status[i] = -999;
    trig_strip_mask_StripID[i] = -999;
    trig_strip_mask_PseudoStripID[i] = -999;
    trig_strip_mask_TccID[i] = -999;
    trig_strip_mask_CCU[i] = -999;
    //
    for(int j=0 ; j<5 ; j++) {
      trig_strip_mask_xtal_ix[i][j] = -999;
      trig_strip_mask_xtal_iy[i][j] = -999;
      trig_strip_mask_xtal_iz[i][j] = -999;
    }
    trig_xtal_mask_ieta[i] = -999;
    trig_xtal_mask_iphi[i] = -999;
    trig_xtal_mask_TTieta[i] = -999;
    trig_xtal_mask_TTiphi[i] = -999;
    trig_xtal_mask_Rieta[i] = -999;
    trig_xtal_mask_Riphi[i] = -999;
    trig_xtal_mask_status[i] = -999;
    trig_xtal_mask_EBEE[i] = -999;
  }
  if (DEBUG) cout << __LINE__<< endl;

  // Disable useless branches
  //myChain->SetBranchStatus("spike_*",0);
  //myChain->SetBranchStatus("vtx_*",0);
  //myChain->SetBranchStatus("skim_*",0);
  //myChain->SetBranchStatus("trig_pre*",0);
  //myChain->SetBranchStatus("trig_post*",0);
  //myChain->SetBranchStatus("trig_HLT*",0);
  //myChain->SetBranchStatus("BS*",0);
  //myChain->SetBranchStatus("MC_*",0);
  //myChain->SetBranchStatus("ele_MC*",0);
  ////myChain->SetBranchStatus("ele_eid*",0);
  ////myChain->SetBranchStatus("ele_Seed*",0);
  //myChain->SetBranchStatus("ele_charge*",0);
  //myChain->SetBranchStatus("met_*",0);
  //myChain->SetBranchStatus("muons*",0);
  //myChain->SetBranchStatus("jets*",0);
  myChain->SetBranchStatus("sc*",0);
  //myChain->SetBranchStatus("sc_hybrid_N",1);
  //myChain->SetBranchStatus("",0);
  if (DEBUG) cout << __LINE__<< endl;

  // Global
  myChain->SetBranchAddress("nEvent",&nEvent);
  myChain->SetBranchAddress("nRun",&nRun);
  myChain->SetBranchAddress("nLumi",&nLumi);

  // Trigger
//   myChain->SetBranchAddress ("trig_HLT_triggered", &m_HLT_triggered);
//   myChain->SetBranchAddress ("trig_HLT_pathsV", &m_HLT_pathsV);
//   myChain->SetBranchAddress ("trig_HLT_pathsV_check", &m_HLT_pathsV_check);
  //
  myChain->SetBranchAddress("trig_HLT_path",&trig_HLT_path);
  // unbias, EG5, EG8, EG12
  //
  myChain->SetBranchAddress("trig_fired_names",&trig_fired_names);
  myChain->SetBranchAddress("trig_hltInfo",&trig_hltInfo);

  // SC
  //myChain->SetBranchAddress("sc_hybrid_N",   &sc_hybrid_N);
  
  // Electrons
  myChain->SetBranchAddress("ele_N",   &ele_N);
  myChain->SetBranchAddress("electrons",&electrons);
  myChain->SetBranchAddress("ele_severityLevelSeed", &ele_severityLevelSeed);
  myChain->SetBranchAddress("ele_he",&ele_he);
  myChain->SetBranchAddress("ele_sigmaietaieta",&ele_sigmaietaieta);
  myChain->SetBranchAddress("ele_hcalDepth1TowerSumEt_dr03", &ele_hcalDepth1TowerSumEt_dr03);
  myChain->SetBranchAddress("ele_hcalDepth2TowerSumEt_dr03", &ele_hcalDepth2TowerSumEt_dr03);
  myChain->SetBranchAddress("ele_ecalRecHitSumEt_dr03", &ele_ecalRecHitSumEt_dr03);
  myChain->SetBranchAddress("ele_tkSumPt_dr03",&ele_tkSumPt_dr03);
  myChain->SetBranchAddress("ele_sclEta",&ele_sclEta);
  myChain->SetBranchAddress("ele_sclEt",&ele_sclEt);
  myChain->SetBranchAddress("ele_expected_inner_hits",&ele_expected_inner_hits);
  myChain->SetBranchAddress("ele_deltaphiin",&ele_deltaphiin);
  myChain->SetBranchAddress("ele_deltaetain",&ele_deltaetain);
  myChain->SetBranchAddress("ele_conv_dist",&ele_conv_dist);
  myChain->SetBranchAddress("ele_conv_dcot",&ele_conv_dcot);
  myChain->SetBranchAddress("ele_fbrem",&ele_fbrem);
  //myChain->SetBranchAddress("ele_ambiguousGsfTracks", &ele_ambiguousGsfTracks);
  myChain->SetBranchAddress("ele_isConversion",&ele_isConversion);
  myChain->SetBranchAddress("ele_echarge",&ele_echarge);

  // L1 electron informations
  myChain->SetBranchAddress("ele_TTetaVect", &ele_TTetaVect);
  myChain->SetBranchAddress("ele_TTphiVect", &ele_TTphiVect);
  myChain->SetBranchAddress("ele_TTetVect", &ele_TTetVect);
  //
  myChain->SetBranchAddress("ele_RCTeta", &ele_RCTeta);
  myChain->SetBranchAddress("ele_RCTphi", &ele_RCTphi);
  myChain->SetBranchAddress("ele_RCTL1iso", &ele_RCTL1iso);
  myChain->SetBranchAddress("ele_RCTL1noniso", &ele_RCTL1noniso);
  myChain->SetBranchAddress("ele_RCTL1iso_M", &ele_RCTL1iso_M);
  myChain->SetBranchAddress("ele_RCTL1noniso_M", &ele_RCTL1noniso_M);

  myChain->SetBranchAddress("ele_RCTetaVect", &ele_RCTetaVect);
  myChain->SetBranchAddress("ele_RCTphiVect", &ele_RCTphiVect);
  myChain->SetBranchAddress("ele_RCTetVect", &ele_RCTetVect);
  myChain->SetBranchAddress("ele_RCTL1isoVect", &ele_RCTL1isoVect);
  myChain->SetBranchAddress("ele_RCTL1nonisoVect", &ele_RCTL1nonisoVect);
  myChain->SetBranchAddress("ele_RCTL1isoVect_M", &ele_RCTL1isoVect_M);
  myChain->SetBranchAddress("ele_RCTL1nonisoVect_M", &ele_RCTL1nonisoVect_M);

  // L1 candidates
  myChain->SetBranchAddress("trig_L1emIso_N", &trig_L1emIso_N);
  myChain->SetBranchAddress("trig_L1emIso_ieta", &trig_L1emIso_ieta);
  myChain->SetBranchAddress("trig_L1emIso_iphi", &trig_L1emIso_iphi);
  myChain->SetBranchAddress("trig_L1emIso_rank", &trig_L1emIso_rank);
  myChain->SetBranchAddress("trig_L1emIso_et", &trig_L1emIso_et);

  myChain->SetBranchAddress("trig_L1emNonIso_N", &trig_L1emNonIso_N);
  myChain->SetBranchAddress("trig_L1emNonIso_ieta", &trig_L1emNonIso_ieta);
  myChain->SetBranchAddress("trig_L1emNonIso_iphi", &trig_L1emNonIso_iphi); 
  myChain->SetBranchAddress("trig_L1emNonIso_rank", &trig_L1emNonIso_rank);
  myChain->SetBranchAddress("trig_L1emNonIso_et", &trig_L1emNonIso_et);

  myChain->SetBranchAddress("trig_L1emIso_N_M", &trig_L1emIso_N_M);
  myChain->SetBranchAddress("trig_L1emIso_ieta_M", &trig_L1emIso_ieta_M);
  myChain->SetBranchAddress("trig_L1emIso_iphi_M", &trig_L1emIso_iphi_M);
  myChain->SetBranchAddress("trig_L1emIso_rank_M", &trig_L1emIso_rank_M);
  myChain->SetBranchAddress("trig_L1emIso_et_M", &trig_L1emIso_et_M);
  
  myChain->SetBranchAddress("trig_L1emNonIso_N_M", &trig_L1emNonIso_N_M);
  myChain->SetBranchAddress("trig_L1emNonIso_ieta_M", &trig_L1emNonIso_ieta_M);
  myChain->SetBranchAddress("trig_L1emNonIso_iphi_M", &trig_L1emNonIso_iphi_M);
  myChain->SetBranchAddress("trig_L1emNonIso_rank_M", &trig_L1emNonIso_rank_M);
  myChain->SetBranchAddress("trig_L1emNonIso_et_M", &trig_L1emNonIso_et_M);

  // Pre/post - firing L1 candidates
  myChain->SetBranchAddress("trig_preL1emIso_N",     &trig_preL1emIso_N);
  myChain->SetBranchAddress("trig_preL1emIso_ieta",  &trig_preL1emIso_ieta);
  myChain->SetBranchAddress("trig_preL1emIso_iphi",  &trig_preL1emIso_iphi);
  myChain->SetBranchAddress("trig_preL1emIso_rank",  &trig_preL1emIso_rank);
  //
  myChain->SetBranchAddress("trig_preL1emNonIso_N",     &trig_preL1emNonIso_N);
  myChain->SetBranchAddress("trig_preL1emNonIso_ieta",  &trig_preL1emNonIso_ieta);
  myChain->SetBranchAddress("trig_preL1emNonIso_iphi",  &trig_preL1emNonIso_iphi);
  myChain->SetBranchAddress("trig_preL1emNonIso_rank",  &trig_preL1emNonIso_rank);
  //
  myChain->SetBranchAddress("trig_postL1emIso_N",     &trig_postL1emIso_N);
  myChain->SetBranchAddress("trig_postL1emIso_ieta",  &trig_postL1emIso_ieta);
  myChain->SetBranchAddress("trig_postL1emIso_iphi",  &trig_postL1emIso_iphi);
  myChain->SetBranchAddress("trig_postL1emIso_rank",  &trig_postL1emIso_rank);
  //
  myChain->SetBranchAddress("trig_postL1emNonIso_N",     &trig_postL1emNonIso_N);
  myChain->SetBranchAddress("trig_postL1emNonIso_ieta",  &trig_postL1emNonIso_ieta);
  myChain->SetBranchAddress("trig_postL1emNonIso_iphi",  &trig_postL1emNonIso_iphi);
  myChain->SetBranchAddress("trig_postL1emNonIso_rank",  &trig_postL1emNonIso_rank);
  if (DEBUG) cout << __LINE__<< endl;

  // Trigger Towers
  // normal collection
  myChain->SetBranchAddress("trig_tower_N", &trig_tower_N);
  myChain->SetBranchAddress("trig_tower_ieta",  &trig_tower_ieta);
  myChain->SetBranchAddress("trig_tower_iphi",  &trig_tower_iphi);
  myChain->SetBranchAddress("trig_tower_adc",  &trig_tower_adc);
  myChain->SetBranchAddress("trig_tower_sFGVB",  &trig_tower_sFGVB);
 
  // modified collection
  myChain->SetBranchAddress("trig_tower_N_modif", &trig_tower_N_modif);
  myChain->SetBranchAddress("trig_tower_ieta_modif",  &trig_tower_ieta_modif);
  myChain->SetBranchAddress("trig_tower_iphi_modif",  &trig_tower_iphi_modif);
  myChain->SetBranchAddress("trig_tower_adc_modif",  &trig_tower_adc_modif);
  myChain->SetBranchAddress("trig_tower_sFGVB_modif",  &trig_tower_sFGVB_modif);

  myChain->SetBranchAddress("trig_tower_N_emul",     &trig_tower_N_emul);
  myChain->SetBranchAddress("trig_tower_ieta_emul",  &trig_tower_ieta_emul);
  myChain->SetBranchAddress("trig_tower_iphi_emul",  &trig_tower_iphi_emul);
  myChain->SetBranchAddress("trig_tower_adc_emul",   &trig_tower_adc_emul);
  myChain->SetBranchAddress("trig_tower_sFGVB_emul", &trig_tower_sFGVB_emul);
  
  // HCAL TP
  myChain->SetBranchAddress("trig_tower_hcal_N", &trig_tower_hcal_N);
  myChain->SetBranchAddress("trig_tower_hcal_ieta",  &trig_tower_hcal_ieta);
  myChain->SetBranchAddress("trig_tower_hcal_iphi",  &trig_tower_hcal_iphi);
  myChain->SetBranchAddress("trig_tower_hcal_et",  &trig_tower_hcal_et);
  myChain->SetBranchAddress("trig_tower_hcal_FG",  &trig_tower_hcal_FG);

  // Strip masking
  myChain->SetBranchAddress("trig_strip_mask_N", &trig_strip_mask_N);
  myChain->SetBranchAddress("trig_strip_mask_TTieta", &trig_strip_mask_TTieta);
  myChain->SetBranchAddress("trig_strip_mask_TTiphi", &trig_strip_mask_TTiphi);
  myChain->SetBranchAddress("trig_strip_mask_StripID", &trig_strip_mask_StripID);
  myChain->SetBranchAddress("trig_strip_mask_PseudoStripID", &trig_strip_mask_PseudoStripID);
  myChain->SetBranchAddress("trig_strip_mask_TccID", &trig_strip_mask_TccID);
  myChain->SetBranchAddress("trig_strip_mask_CCU", &trig_strip_mask_CCU);
  myChain->SetBranchAddress("trig_strip_mask_xtal_ix", &trig_strip_mask_xtal_ix);
  myChain->SetBranchAddress("trig_strip_mask_xtal_iy", &trig_strip_mask_xtal_iy);
  myChain->SetBranchAddress("trig_strip_mask_xtal_iz", &trig_strip_mask_xtal_iz);
  //
  // Crystal masking
  myChain->SetBranchAddress("trig_xtal_mask_N", &trig_xtal_mask_N);
  myChain->SetBranchAddress("trig_xtal_mask_ieta", &trig_xtal_mask_ieta);
  myChain->SetBranchAddress("trig_xtal_mask_iphi", &trig_xtal_mask_iphi);
  myChain->SetBranchAddress("trig_xtal_mask_TTieta", &trig_xtal_mask_TTieta);
  myChain->SetBranchAddress("trig_xtal_mask_TTiphi", &trig_xtal_mask_TTiphi);
  myChain->SetBranchAddress("trig_xtal_mask_Rieta", &trig_xtal_mask_Rieta);
  myChain->SetBranchAddress("trig_xtal_mask_Riphi", &trig_xtal_mask_Riphi);
  myChain->SetBranchAddress("trig_xtal_mask_status", &trig_xtal_mask_status);
  myChain->SetBranchAddress("trig_xtal_mask_EBEE", &trig_xtal_mask_EBEE);

  // Masking
  myChain->SetBranchAddress("trig_nMaskedRCT",      &trig_nMaskedRCT);      
  myChain->SetBranchAddress("trig_iMaskedRCTeta",   &trig_iMaskedRCTeta);                                          
  myChain->SetBranchAddress("trig_iMaskedRCTcrate", &trig_iMaskedRCTcrate);
  myChain->SetBranchAddress("trig_iMaskedRCTphi",   &trig_iMaskedRCTphi);
  myChain->SetBranchAddress("trig_nMaskedCh",       &trig_nMaskedCh);    
  myChain->SetBranchAddress("trig_iMaskedTTeta",    &trig_iMaskedTTeta);   
  myChain->SetBranchAddress("trig_iMaskedTTphi",    &trig_iMaskedTTphi);      	


  if(debug) cout << "got the input tree, start to define output tree" << endl;
  if (DEBUG) cout << __LINE__<< endl;
  
  // OUTPUT TREE //
  TTree * outtree = new TTree("ElePairs","ElePairs");

  // General informations
  outtree->Branch("nRun",&nRun,"nRun/I");
  outtree->Branch("nLumi",&nLumi,"nLumi/I");
  outtree->Branch("nEvent",&nEvent,"nEvent/I");

  // Vertices
  outtree->Branch("vtx_N",&_vtx_N,"vtx_N/I");
  outtree->Branch("vtx_normalizedChi2",&_vtx_normalizedChi2,"vtx_normalizedChi2[200]/D");
  outtree->Branch("vtx_ndof",&_vtx_ndof,"vtx_ndof[200]/D");
  outtree->Branch("vtx_nTracks",&_vtx_nTracks,"vtx_nTracks[200]/D");
  outtree->Branch("vtx_d0",&_vtx_d0,"vtx_d0[200]/D");
  outtree->Branch("vtx_x",&_vtx_x,"vtx_x[200]/D");
  outtree->Branch("vtx_y",&_vtx_y,"vtx_y[200]/D");
  outtree->Branch("vtx_z",&_vtx_z,"vtx_z[200]/D");

  // HLT informations
//   outtree->Branch ("trig_HLT_triggered", &m_HLT_triggered, 256000,0);
//   outtree->Branch ("trig_HLT_pathsV", &m_HLT_pathsV, 256000,0);
//   outtree->Branch ("trig_HLT_pathsV_check", &m_HLT_pathsV_check, 256000,0);
  //
  outtree->Branch("trig_HLT_path",&trig_HLT_path,"trig_HLT_path[4]/I");
  // unbias, EG5, EG8, EG12
  //
  outtree->Branch("trig_fired_names",&trig_fired_names,"trig_fired_names[5000]/C");
  outtree->Branch("trig_hltInfo",&trig_hltInfo,"trig_hltInfo[250]/I");  

  // Trigger towers
  outtree->Branch("trig_tower_N",&trig_tower_N,"trig_tower_N/I");
  outtree->Branch("trig_tower_ieta",&trig_tower_ieta,"trig_tower_ieta[4032]/I");
  outtree->Branch("trig_tower_iphi",&trig_tower_iphi,"trig_tower_iphi[4032]/I");
  outtree->Branch("trig_tower_adc",&trig_tower_adc,"trig_tower_adc[4032]/I");
  outtree->Branch("trig_tower_sFGVB",&trig_tower_sFGVB,"trig_tower_sFGVB[4032]/I");
  //
  outtree->Branch("trig_tower_N_modif",&trig_tower_N_modif,"trig_tower_N_modif/I");
  outtree->Branch("trig_tower_ieta_modif",&trig_tower_ieta_modif,"trig_tower_ieta_modif[4032]/I");
  outtree->Branch("trig_tower_iphi_modif",&trig_tower_iphi_modif,"trig_tower_iphi_modif[4032]/I");
  outtree->Branch("trig_tower_adc_modif",&trig_tower_adc_modif,"trig_tower_adc_modif[4032]/I");
  outtree->Branch("trig_tower_sFGVB_modif",&trig_tower_sFGVB_modif,"trig_tower_sFGVB_modif[4032]/I");
  //
  outtree->Branch("trig_tower_N_emul",&trig_tower_N_emul,"trig_tower_N_emul/I");
  outtree->Branch("trig_tower_ieta_emul",&trig_tower_ieta_emul,"trig_tower_ieta_emul[4032]/I");
  outtree->Branch("trig_tower_iphi_emul",&trig_tower_iphi_emul,"trig_tower_iphi_emul[4032]/I");
  outtree->Branch("trig_tower_adc_emul",&trig_tower_adc_emul,"trig_tower_adc_emul[4032][5]/I");
  outtree->Branch("trig_tower_sFGVB_emul",&trig_tower_sFGVB_emul,"trig_tower_sFGVB_emul[4032][5]/I");

  // HCAL TP
  outtree->Branch("trig_tower_hcal_N", &trig_tower_hcal_N, "trig_tower_hcal_N/I");
  outtree->Branch("trig_tower_hcal_ieta",  &trig_tower_hcal_ieta,  "trig_tower_hcal_ieta[trig_tower_N]/I");
  outtree->Branch("trig_tower_hcal_iphi",  &trig_tower_hcal_iphi,  "trig_tower_hcal_iphi[trig_tower_N]/I");
  outtree->Branch("trig_tower_hcal_et",  &trig_tower_hcal_et,  "trig_tower_hcal_et[trig_tower_N]/I");
  outtree->Branch("trig_tower_hcal_FG",  &trig_tower_hcal_FG,  "trig_tower_hcal_FG[trig_tower_N]/I");

  // Strip masking
  outtree->Branch("trig_strip_mask_N", &trig_strip_mask_N, "trig_strip_mask_N/I");
  outtree->Branch("trig_strip_mask_TTieta", &trig_strip_mask_TTieta, "trig_strip_mask_TTieta[trig_strip_mask_N]/I");
  outtree->Branch("trig_strip_mask_TTiphi", &trig_strip_mask_TTiphi, "trig_strip_mask_TTiphi[trig_strip_mask_N]/I");
  outtree->Branch("trig_strip_mask_StripID", &trig_strip_mask_StripID, "trig_strip_mask_StripID[trig_strip_mask_N]/I");
  outtree->Branch("trig_strip_mask_PseudoStripID", &trig_strip_mask_PseudoStripID, "trig_strip_mask_PseudoStripID[trig_strip_mask_N]/I");
  outtree->Branch("trig_strip_mask_TccID", &trig_strip_mask_TccID, "trig_strip_mask_TccID[trig_strip_mask_N]/I");
  outtree->Branch("trig_strip_mask_CCU", &trig_strip_mask_CCU, "trig_strip_mask_CCU[trig_strip_mask_N]/I");
  outtree->Branch("trig_strip_mask_xtal_ix", &trig_strip_mask_xtal_ix, "trig_strip_mask_xtal_ix[trig_strip_mask_N][5]/I");
  outtree->Branch("trig_strip_mask_xtal_iy", &trig_strip_mask_xtal_iy, "trig_strip_mask_xtal_iy[trig_strip_mask_N][5]/I");
  outtree->Branch("trig_strip_mask_xtal_iz", &trig_strip_mask_xtal_iz, "trig_strip_mask_xtal_iz[trig_strip_mask_N][5]/I");
  //
  // Crystal masking
  outtree->Branch("trig_xtal_mask_N", &trig_xtal_mask_N, "trig_xtal_mask_N/I");
  outtree->Branch("trig_xtal_mask_ieta", &trig_xtal_mask_ieta, "trig_xtal_mask_ieta[trig_xtal_mask_N]/I");
  outtree->Branch("trig_xtal_mask_iphi", &trig_xtal_mask_iphi, "trig_xtal_mask_iphi[trig_xtal_mask_N]/I");
  outtree->Branch("trig_xtal_mask_TTieta", &trig_xtal_mask_TTieta, "trig_xtal_mask_TTieta[trig_xtal_mask_N]/I");
  outtree->Branch("trig_xtal_mask_TTiphi", &trig_xtal_mask_TTiphi, "trig_xtal_mask_TTiphi[trig_xtal_mask_N]/I");
  outtree->Branch("trig_xtal_mask_Rieta", &trig_xtal_mask_Rieta, "trig_xtal_mask_Rieta[trig_xtal_mask_N]/I");
  outtree->Branch("trig_xtal_mask_Riphi", &trig_xtal_mask_Riphi, "trig_xtal_mask_Riphi[trig_xtal_mask_N]/I");
  outtree->Branch("trig_xtal_mask_status", &trig_xtal_mask_status, "trig_xtal_mask_status[trig_xtal_mask_N]/I");
  outtree->Branch("trig_xtal_mask_EBEE", &trig_xtal_mask_EBEE, "trig_xtal_mask_EBEE[trig_xtal_mask_N]/I");
  // L1 candidates
  outtree->Branch("trig_L1emIso_N",     &trig_L1emIso_N,     "trig_L1emIso_N/I");
  outtree->Branch("trig_L1emIso_ieta",  &trig_L1emIso_ieta,  "trig_L1emIso_ieta[4]/I");
  outtree->Branch("trig_L1emIso_iphi",  &trig_L1emIso_iphi,  "trig_L1emIso_iphi[4]/I");
  outtree->Branch("trig_L1emIso_rank",  &trig_L1emIso_rank,  "trig_L1emIso_rank[4]/I");
  //outtree->Branch("trig_L1emIso_eta",   &trig_L1emIso_eta,   "trig_L1emIso_eta[4]/D");
  //outtree->Branch("trig_L1emIso_phi",   &trig_L1emIso_phi,   "trig_L1emIso_phi[4]/D");
//   outtree->Branch("trig_L1emIso_energy",&trig_L1emIso_energy,"trig_L1emIso_energy[4]/D");
  outtree->Branch("trig_L1emIso_et",    &trig_L1emIso_et,    "trig_L1emIso_et[4]/I");
  //
  outtree->Branch("trig_L1emNonIso_N",     &trig_L1emNonIso_N,     "trig_L1emNonIso_N/I");
  outtree->Branch("trig_L1emNonIso_ieta",  &trig_L1emNonIso_ieta,  "trig_L1emNonIso_ieta[4]/I");
  outtree->Branch("trig_L1emNonIso_iphi",  &trig_L1emNonIso_iphi,  "trig_L1emNonIso_iphi[4]/I");
  outtree->Branch("trig_L1emNonIso_rank",  &trig_L1emNonIso_rank,  "trig_L1emNonIso_rank[4]/I");
//   outtree->Branch("trig_L1emNonIso_eta",   &trig_L1emNonIso_eta,   "trig_L1emNonIso_eta[4]/D");
//   outtree->Branch("trig_L1emNonIso_phi",   &trig_L1emNonIso_phi,   "trig_L1emNonIso_phi[4]/D");
//   outtree->Branch("trig_L1emNonIso_energy",&trig_L1emNonIso_energy,"trig_L1emNonIso_energy[4]/D");
  outtree->Branch("trig_L1emNonIso_et",    &trig_L1emNonIso_et,    "trig_L1emNonIso_et[4]/I");
  //
  outtree->Branch("trig_L1emIso_N_M",     &trig_L1emIso_N_M,     "trig_L1emIso_N_M/I");
  outtree->Branch("trig_L1emIso_ieta_M",  &trig_L1emIso_ieta_M,  "trig_L1emIso_ieta_M[4]/I");
  outtree->Branch("trig_L1emIso_iphi_M",  &trig_L1emIso_iphi_M,  "trig_L1emIso_iphi_M[4]/I");
  outtree->Branch("trig_L1emIso_rank_M",  &trig_L1emIso_rank_M,  "trig_L1emIso_rank_M[4]/I");
//   outtree->Branch("trig_L1emIso_eta_M",   &trig_L1emIso_eta_M,   "trig_L1emIso_eta_M[4]/D");
//   outtree->Branch("trig_L1emIso_phi_M",   &trig_L1emIso_phi_M,   "trig_L1emIso_phi_M[4]/D");
//   outtree->Branch("trig_L1emIso_energy_M",&trig_L1emIso_energy_M,"trig_L1emIso_energy_M[4]/D");
  outtree->Branch("trig_L1emIso_et_M",    &trig_L1emIso_et_M,    "trig_L1emIso_et_M[4]/I");
  //
  outtree->Branch("trig_L1emNonIso_N_M", &trig_L1emNonIso_N_M, "trig_L1emNonIso_N_M/I");
  outtree->Branch("trig_L1emNonIso_ieta_M", &trig_L1emNonIso_ieta_M, "trig_L1emNonIso_ieta_M[4]/I");
  outtree->Branch("trig_L1emNonIso_iphi_M", &trig_L1emNonIso_iphi_M, "trig_L1emNonIso_iphi_M[4]/I");
  outtree->Branch("trig_L1emNonIso_rank_M", &trig_L1emNonIso_rank_M, "trig_L1emNonIso_rank_M[4]/I");
//   outtree->Branch("trig_L1emNonIso_eta_M", &trig_L1emNonIso_eta_M, "trig_L1emNonIso_eta_M[4]/D");
//   outtree->Branch("trig_L1emNonIso_phi_M", &trig_L1emNonIso_phi_M, "trig_L1emNonIso_phi_M[4]/D");
//   outtree->Branch("trig_L1emNonIso_energy_M",&trig_L1emNonIso_energy_M,"trig_L1emNonIso_energy_M[4]/D");
  outtree->Branch("trig_L1emNonIso_et_M", &trig_L1emNonIso_et_M, "trig_L1emNonIso_et_M[4]/I");

  // pre-post firing L1 candidates
  outtree->Branch("trig_preL1emIso_N",     &trig_preL1emIso_N,     "trig_preL1emIso_N/I");
  outtree->Branch("trig_preL1emIso_ieta",  &trig_preL1emIso_ieta,  "trig_preL1emIso_ieta[4]/I");
  outtree->Branch("trig_preL1emIso_iphi",  &trig_preL1emIso_iphi,  "trig_preL1emIso_iphi[4]/I");
  outtree->Branch("trig_preL1emIso_rank",  &trig_preL1emIso_rank,  "trig_preL1emIso_rank[4]/I");
  //
  outtree->Branch("trig_preL1emNonIso_N",     &trig_preL1emNonIso_N,     "trig_preL1emNonIso_N/I");
  outtree->Branch("trig_preL1emNonIso_ieta",  &trig_preL1emNonIso_ieta,  "trig_preL1emNonIso_ieta[4]/I");
  outtree->Branch("trig_preL1emNonIso_iphi",  &trig_preL1emNonIso_iphi,  "trig_preL1emNonIso_iphi[4]/I");
  outtree->Branch("trig_preL1emNonIso_rank",  &trig_preL1emNonIso_rank,  "trig_preL1emNonIso_rank[4]/I");
  //
  outtree->Branch("trig_postL1emIso_N",     &trig_postL1emIso_N,     "trig_postL1emIso_N/I");
  outtree->Branch("trig_postL1emIso_ieta",  &trig_postL1emIso_ieta,  "trig_postL1emIso_ieta[4]/I");
  outtree->Branch("trig_postL1emIso_iphi",  &trig_postL1emIso_iphi,  "trig_postL1emIso_iphi[4]/I");
  outtree->Branch("trig_postL1emIso_rank",  &trig_postL1emIso_rank,  "trig_postL1emIso_rank[4]/I");
  //
  outtree->Branch("trig_postL1emNonIso_N",     &trig_postL1emNonIso_N,     "trig_postL1emNonIso_N/I");
  outtree->Branch("trig_postL1emNonIso_ieta",  &trig_postL1emNonIso_ieta,  "trig_postL1emNonIso_ieta[4]/I");
  outtree->Branch("trig_postL1emNonIso_iphi",  &trig_postL1emNonIso_iphi,  "trig_postL1emNonIso_iphi[4]/I");
  outtree->Branch("trig_postL1emNonIso_rank",  &trig_postL1emNonIso_rank,  "trig_postL1emNonIso_rank[4]/I");
  //
  outtree->Branch("trig_nMaskedRCT",      &trig_nMaskedRCT,     "trig_nMaskedRCT/I");      
  outtree->Branch("trig_iMaskedRCTeta",   &trig_iMaskedRCTeta,  "trig_iMaskedRCTeta[trig_nMaskedRCT]/I");                                          
  outtree->Branch("trig_iMaskedRCTcrate", &trig_iMaskedRCTcrate,"trig_iMaskedRCTcrate[trig_nMaskedRCT]/I");                                                    
  outtree->Branch("trig_iMaskedRCTphi",   &trig_iMaskedRCTphi,  "trig_iMaskedRCTphi[trig_nMaskedRCT]/I");
  outtree->Branch("trig_nMaskedCh",       &trig_nMaskedCh,      "trig_nMaskedCh/I");    
  outtree->Branch("trig_iMaskedTTeta",    &trig_iMaskedTTeta,   "trig_iMaskedTTeta[trig_nMaskedCh]/I");   
  outtree->Branch("trig_iMaskedTTphi",    &trig_iMaskedTTphi,   "trig_iMaskedTTphi[trig_nMaskedCh]/I");      	


  // Pairs informations
  double pair_M;
  double pair_eta[2], pair_sclEta[2], pair_sclEt[2], pair_phi[2], pair_pT[2], pair_eT[2], pair_E[2];
  int pair_cuts[2], pair_HLT_Ele27_cut[2], pair_fidu[2], pair_charge[2], pair_RCTeta[2], pair_RCTphi[2], 
    pair_L1iso[2], pair_L1noniso[2], pair_L1iso_M[2], pair_L1noniso_M[2];
  int pair_RCTetaVect[2][10], pair_RCTphiVect[2][10], 
    pair_L1isoVect[2][10], pair_L1nonisoVect[2][10],pair_L1isoVect_M[2][10], pair_L1nonisoVect_M[2][10];
  double pair_RCTetVect[2][10];
  int pair_TTetaVect[2][50], pair_TTphiVect[2][50];
  double pair_TTetVect[2][50];

  // //
  // outtree->Branch("pair_M",&pair_M,"pair_M/D");
  // //
  // outtree->Branch("pair_cuts",&pair_cuts,"pair_cuts[2]/I");
  // // 0 : noCut | 1 : VBTF 95 | 2 : VBTF 80 | 3 : VBTF 60
  // outtree->Branch("pair_HLT_Ele27_cut",&pair_HLT_Ele27_cut,"pair_HLT_Ele27_cut[2]/I");
  // outtree->Branch("pair_fidu",&pair_fidu,"pair_fidu[2]/I");
  // //
  // outtree->Branch("pair_eta",&pair_eta,"pair_eta[2]/D");
  // outtree->Branch("pair_sclEta",&pair_sclEta,"pair_sclEta[2]/D");
  // outtree->Branch("pair_phi",&pair_phi,"pair_phi[2]/D");
  // outtree->Branch("pair_RCTeta",&pair_RCTeta,"pair_RCTeta[2]/I");
  // outtree->Branch("pair_RCTphi",&pair_RCTphi,"pair_RCTphi[2]/I");
  // //
  // outtree->Branch("pair_charge",&pair_charge,"pair_charge[2]/I");
  // outtree->Branch("pair_pT",&pair_pT,"pair_pT[2]/D");
  // outtree->Branch("pair_eT",&pair_eT,"pair_eT[2]/D");
  // outtree->Branch("pair_sclEt",&pair_sclEt,"pair_sclEt[2]/D");
  // outtree->Branch("pair_E",&pair_E,"pair_E[2]/D");

  // outtree->Branch("pair_TTetaVect", &pair_TTetaVect,"pair_TTetaVect[2][50]/I");
  // outtree->Branch("pair_TTphiVect", &pair_TTphiVect,"pair_TTphiVect[2][50]/I");
  // outtree->Branch("pair_TTetVect", &pair_TTetVect,"pair_TTetVect[2][50]/D");

  // outtree->Branch("pair_L1iso",&pair_L1iso,"pair_L1iso[2]/I");
  // outtree->Branch("pair_L1noniso",&pair_L1noniso,"pair_L1noniso[2]/I");
  // outtree->Branch("pair_L1iso_M",&pair_L1iso_M,"pair_L1iso_M[2]/I");
  // outtree->Branch("pair_L1noniso_M",&pair_L1noniso_M,"pair_L1noniso_M[2]/I");
  // //
  // outtree->Branch("pair_RCTetVect",&pair_RCTetVect,"pair_RCTetVect[2][10]/D");
  // outtree->Branch("pair_RCTetaVect",&pair_RCTetaVect,"pair_RCTetaVect[2][10]/I");
  // outtree->Branch("pair_RCTphiVect",&pair_RCTphiVect,"pair_RCTphiVect[2][10]/I");
  // outtree->Branch("pair_L1isoVect",&pair_L1isoVect,"pair_L1isoVect[2][10]/I");
  // outtree->Branch("pair_L1nonisoVect",&pair_L1nonisoVect,"pair_L1nonisoVect[2][10]/I");
  // outtree->Branch("pair_L1isoVect_M",&pair_L1isoVect_M,"pair_L1isoVect_M[2][10]/I");
  // outtree->Branch("pair_L1nonisoVect_M",&pair_L1nonisoVect_M,"pair_L1nonisoVect_M[2][10]/I");

  if(debug) cout << "output tree defined" << endl;

  // USEFUL VARIABLES //
  vector<int> pairIdx;
  int cutEle[2], fidu[2];
  bool cut_HLT_Ele27[2];
  TLorentzVector* cand[2];
  TLorentzVector total;
  bool isGoodRun;
  TString filename;
  
  // -------------------------------------------------------------------------------
  // LOOP OVER EVENTS
  // -------------------------------------------------------------------------------  
  if(debug) cout << "gonna loop over events" << endl;

  int numEntries = myChain->GetEntries () ;
  int nProcess = numEntries;
  if(nEntries>=0 && nEntries<numEntries)
    nProcess = nEntries;

  int nCurrentRun = -999;
  outlog << "will process " << nProcess << "/" << numEntries << "entries" << endl;
  oldEvent = 0;
  for (int iEvent = 0 ; iEvent < nProcess ; iEvent++ )
    { 
      //if (iEvent > 20000) break ; 
      if (iEvent > 0 && iEvent/1000. ==  int(iEvent/1000.) ) cout << "processing " << iEvent << "th entry" << endl;

      // HLT information
      for(int i=0 ; i<4 ; i++)
	trig_HLT_path[i]=0;
      
      for(int i=0 ; i<250 ; i++)
	trig_hltInfo[i]=0;

      // TP Initialization
      trig_tower_N = 0;
      for(int iTow=0 ; iTow<nTow ; iTow++) {
	trig_tower_ieta[iTow] = trig_tower_iphi[iTow]  = -999;
	trig_tower_adc[iTow]  = trig_tower_sFGVB[iTow] = -999;
      }
      trig_tower_N_modif = 0;
      for(int iTow=0 ; iTow<nTow ; iTow++) {
	trig_tower_ieta_modif[iTow] = trig_tower_iphi_modif[iTow]  = -999;
	trig_tower_adc_modif[iTow]  = trig_tower_sFGVB_modif[iTow] = -999;
      }
      trig_tower_N_emul = 0;
      for(int iTow=0 ; iTow<nTow ; iTow++) {
	trig_tower_ieta_emul[iTow] = trig_tower_iphi_emul[iTow] = -999;
	for(int i=0 ; i<5 ; i++)
	  trig_tower_adc_emul[iTow][i] = trig_tower_sFGVB_emul[iTow][i] = -999;
      }

      myChain->GetEntry (iEvent) ;
     
      if (nEvent == oldEvent ){
	cout << "duplicate event " << nEvent << " " << oldEvent<< endl;
	break;
      }
      oldEvent=nEvent; 

      // show processed file
      if(iEvent==0) {
        filename = myChain->GetFile()->GetName() ;
        outlog << "File : " << filename << endl << endl;
      }
      else if( filename != myChain->GetFile()->GetName() ) {
        filename = myChain->GetFile()->GetName() ;
        outlog << "File : " << myChain->GetFile()->GetName() << endl << endl;
      }
     
      // show current run/iCat processed
      if(iEvent==0) {
	nCurrentRun = nRun ;
	outlog << "nRun=" << nRun << endl;
      }
      else if(nRun!=nCurrentRun) {
	nCurrentRun=nRun ;
	outlog << "nRun=" << nRun << endl;
      }
      
      // at least 1 electron
      if(ele_N<1) continue;
      else outlog << "ele_N=" << ele_N << endl;

      /// fill the vector with the trigger cand Et  (hIso and hNonIso) 
      bool isoRank, nonIsoRank, isoRank_M, nonIsoRank_M; 
      
      for (size_t ithr = 0 ; ithr < sizeof(thresholds)/sizeof(*thresholds); ithr++){
	isoRank=false; 
	nonIsoRank=false; 
	isoRank_M=false; 
	nonIsoRank_M=false; 
	for (int i =0 ; i< 4 ; i++){
	  //cout << __LINE__ << " " << trig_L1emIso_et[i] << " " << trig_L1emIso_et_M[i] << " " << 2*trig_L1emIso_rank[i] << " " << 2*trig_L1emIso_rank_M[i] << endl; 
	  if (isoRank==false && trig_L1emIso_et[i] >= thresholds[ithr] ) {
	    thresholdsIsopairs[ithr].second++;
	    h_trigIsoCandSpectrum->Fill(trig_L1emIso_et[i]);
	    h_trigIsoCands->Fill(thresholds[ithr]);
	    h_trigAllCands->Fill(thresholds[ithr]);
	  }
	  if (nonIsoRank==false && trig_L1emNonIso_et[i] >= thresholds[ithr] ){
	    thresholdsNonIsopairs[ithr].second++;
	    h_trigNonIsoCandSpectrum->Fill(trig_L1emNonIso_et[i]);
	    h_trigNonIsoCands->Fill(thresholds[ithr]);
	    h_trigAllCands->Fill(thresholds[ithr]);
	  }
	  if (isoRank_M==false && trig_L1emIso_et_M[i] >= thresholds[ithr] ){
	    thresholdsIsopairs_M[ithr].second++;
	    h_trigIsoCandSpectrum_M->Fill(trig_L1emIso_et_M[i]);
	    h_trigIsoCands_M->Fill(thresholds[ithr]);
	    h_trigAllCands_M->Fill(thresholds[ithr]);
	  }
	  if (nonIsoRank_M==false && trig_L1emNonIso_et_M[i] >= thresholds[ithr] ){
	    thresholdsNonIsopairs_M[ithr].second++;
	    h_trigNonIsoCandSpectrum_M->Fill(trig_L1emNonIso_et_M[i]);
	    h_trigNonIsoCands_M->Fill(thresholds[ithr]);
	    h_trigAllCands_M->Fill(thresholds[ithr]);
	  }
	}
      }

      h_Entries->Fill(1);
     

    }//loop over events

  
  
  outfile->cd();
  h_Entries->Write(); 
  h_trigIsoCands->Write();
  h_trigNonIsoCands->Write();
  h_trigIsoCands_M->Write();
  h_trigNonIsoCands_M->Write();
  h_trigNonIsoCandSpectrum_M->Write();
  h_trigIsoCandSpectrum_M->Write();
  h_trigNonIsoCandSpectrum->Write();
  h_trigIsoCandSpectrum->Write();
  h_trigAllCands_M->Write();
  h_trigAllCands->Write();

  

  if(debug) cout << "End loop events" << endl;
  outlog << "End loop events" << endl;

  // Record tree
  if(debug) cout << "recording tree..." << endl;
  outlog << "recording tree..." << endl;


  if(debug) cout << "recorded !" << endl;
  outlog << "recorded !" << endl;

  outfile->Close();

  if(debug) cout << "file closed." << endl;
  outlog << "file closed." << endl;

  return 1;

}
void makeTriggerCand(string file, int nEntries,  string dirOut, TString dirIn, TString data, string nameChain,  TString RunPhase, bool debug){
  // Output Log
  // if( stat( dirOut.c_str(), &info ) == 0 ) {
  //   boost::filesystem::path dir(dirOut.c_str());
  //   boost::filesystem::create_directory(dirOut);
  //}
  
  ofstream outlog(*dirOut.c_str()+"/log.txt",ios::out);
 
  outlog << " <--------------------------------- START --------------------------------->" << endl;

  // Process the tree
  if(debug) cout << "process the tree" << endl;

  vector<string> treeList;
  string eosdir; 
  eosdir="root://eoscms//eos/cms/store/caf/user/taroni/TestTriggerOptimization/";
  vector<string> files; 
  
  files.push_back(file.c_str());
  
  int nFiles =files.size();
  cout << file << " " << nFiles << endl;

  for(int i = 0; i < nFiles; i++) {
    
    stringstream inputfile; 
    inputfile.str(""); 
    inputfile << eosdir << myjobid<< "/"<< files[i];
    cout << "input file "<< inputfile.str() << endl;
    cout << "process file #" << i+1 << " of " << nFiles << " files " << endl;
    processCand(nameChain,inputfile.str(),i,nEntries,dirOut,dirIn,outlog,RunPhase,debug);
    cout << "processed" << endl;
  }

  outlog << " <--------------------------------- DONE --------------------------------->" << endl;
  cout << "DONE" << endl;

}
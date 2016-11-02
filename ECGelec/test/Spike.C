#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <map>
#include <boost/tokenizer.hpp>
#include <vector>

#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TProfile2D.h>

///#define DEBUG 1

using namespace std; 

int getEt(uint val) {return (val&0xff) ;}

uint getFg(uint val) {return ((val&0x100)!=0) ;}

uint getTtf(uint val) {return ((val>>9)&0x7) ;}

double getEffErr(int passed ,int total)
{
  // cout<<"passed "<<passed<<endl;
  // cout<<"total "<<total<<endl;
  double efficiency=double(passed)/double(total);
  if (efficiency<1)
    {
      return sqrt(double(efficiency*(1-efficiency))/double(total));
    }
  else
    {
      return sqrt(double(efficiency*(efficiency-1))/double(passed));
    }
}

std::vector<std::string> split(std::string msg, std::string separator)
{
   boost::char_separator<char> sep(separator.c_str());
   boost::tokenizer<boost::char_separator<char> > tok(msg, sep );
   std::vector<std::string> token ;
   for ( boost::tokenizer<boost::char_separator<char> >::const_iterator i = tok.begin(); i != tok.end(); ++i ) {
      token.push_back(std::string(*i)) ;
   }
   return token ;
}



double getEta(int ietaTower) 
{
   // Paga: to be confirmed, specially in EE:
   return 0.0174*fabs(ietaTower) ;
}



void getTTnb(int iphi, int ieta, int & SM, int & TT)
{
   if (iphi == 1 || iphi == 2 || iphi == 71 || iphi == 72) {
      SM = 1 ;
      if (iphi<71)
         TT = (iphi-1) + 3 + 4*(abs(ieta)-1) ;
      else
         TT = (iphi-71) + 1 + 4*(abs(ieta)-1) ;      
   }
   else {
      SM = (iphi-3)/4 + 2 ; 
      TT = (iphi-3)%4 + 1 + 4*(abs(ieta)-1) ;
   }
   if (ieta<0) SM = -SM ;
   else {
      uint col = (TT-1)%4;
      uint row = (TT-1)-col;
      uint mirror[4] = {4,3,2,1};
      TT = row+mirror[col];
   }
}



int getGCTRegionPhi(int ttphi)
{
   int gctphi=0;
   gctphi = (ttphi+1)/4;
   if(ttphi<=2) gctphi=0;
   if(ttphi>=71) gctphi=0;
   
   return gctphi;
}



int getGCTRegionPhi2(int crate, int phi)
{
   int gctphi2=0;
   int crate2;
   if(crate<9) crate2=crate;
   else crate2=crate-9;
   //gctphi2 = (ttphi+1)/4;
   if(crate2<2 || (crate2==2 && phi==0)) gctphi2 = 4 - 2*crate2 - phi;
   if(crate2>2 || (crate2==2 && phi==1)) gctphi2 = 22 - phi - 2*crate2;
   return gctphi2;
}



std::vector <int> getECALRegionEta2(int crate, int eta)
{
   std::vector <int> tteta2;
   
   if(crate<9){ 
      for(int i=0;i<4;++i){ 
         tteta2.push_back( -eta + i); 
      }
   }
   else{ 
      for(int i=0;i<4;++i){
         tteta2.push_back( eta - i); 
      }
   }
   
   return tteta2;
}



int getGCTRegionEta(int tteta)
{
   int gcteta = 0;
   
   if(tteta>0) gcteta = (tteta-1)/4 + 11;
   else if(tteta<0) gcteta = (tteta+1)/4 + 10;
   
   return gcteta;
}



std::vector <int> getECALRegionPhi(int gctphi)
{
   std::vector <int> ttphi;
   
   if(gctphi==0){
      ttphi.push_back(71);
      ttphi.push_back(72);
      ttphi.push_back(1);
      ttphi.push_back(2);
   }
   else{
      for(int i=0;i!=4;++i) ttphi.push_back(gctphi*4-1+i);
   } 
   
   return ttphi;
}



std::vector <int> getECALRegionEta(int gcteta)
{
   std::vector <int> tteta;
   
   if(gcteta>=11){ 
      for(int i=1;i<=4;++i){ 
         tteta.push_back( (gcteta-11)*4+i ); 
      }
   }
   else{ 
      for(int i=0;i!=4;++i){
         tteta.push_back( (gcteta-11)*4+i); 
      }
   }
   
   return tteta;
}

//void CompareTPAllVSSpikes(TString run,TString campaign,TString sfgvb,TString etkill){
void Spike(TString run,TString campaign,TString sfgvb,TString etkill){

  gROOT->SetBatch(kTRUE);

  gStyle->SetPalette(1,0);      
  gROOT->SetStyle("Plain"); 
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleFillColor(0);
  gStyle->SetStatFont(42);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetStatX(0.90);
  gStyle->SetStatY(0.90);
  gStyle->SetPalette(1);
  gStyle->SetOptStat(000000);
  //  gStyle->SetOptFit(1111);
  gStyle->SetTitleX(0.3); // X position of the title box from left 
  gStyle->SetOptTitle(kFALSE);  

  
  //  TString filestart="/afs/cern.ch/work/n/ndev/Spikes/CMSSW_7_4_15/src/EGamma/ECGelec/test/ECALTPGtree_"+sfgvb+"_"+etkill+"_";
  TString filestart="/afs/cern.ch/work/n/ndev/CMSSW_7_6_3/src/EGamma/ECGelec/test/ECALTPGtree_testing_";
  TString runnum=run;
  filestart+=runnum;
  TString ext=".root";
  filestart+=ext;
  // TString filestart="/afs/cern.ch/work/n/ndev/CMSSW_7_4_15/src/EGamma/ECGelec/test/ECALTPGtree_testing_tree11.root";  
  
  // TString filestart="/afs/cern.ch/work/n/ndev/CMSSW_7_4_15/src/EGamma/ECGelec/test/tree.root";
  TFile *inf = TFile::Open(filestart);

  TTree *tr_ECAL = (TTree*)inf->Get("produceNtuple/eIDSimpleTree");


  // ECAL Variables 
  // event variables
  Int_t runNb ;
  Int_t evtNb ;
  Int_t lumiBlock ;

  // uint bxNb ;
  // ULong64_t orbitNb ;
  // double timeStamp ; 
  
  int nVertices;

  //crystal level info
  int n_bad_crystals;
  int erec_Et_sevlv3_4[4032];
  double erec_eta_sevlv3_4[4032];
  double erec_phi_sevlv3_4[4032];
  double erec_theta_sevlv3_4[4032];  

  // tower variables
  uint nbOfTowers ; //max 4032 EB+EE
  int ieta[4032] ;
  int iphi[4032] ;
  int nbOfXtals[4032] ;
  int rawTPData[4032] ;
  int rawTPEmul1[4032] ;
  int rawTPEmul2[4032] ;
  int rawTPEmul3[4032] ;
  int rawTPEmul4[4032] ;
  int rawTPEmul5[4032] ;
  int rawTPEmulttFlag1[4032] ;
  int rawTPEmulttFlag2[4032] ;
  int rawTPEmulttFlag3[4032] ;
  int rawTPEmulttFlag4[4032] ;
  int rawTPEmulttFlag5[4032] ;
  int rawTPEmulsFGVB1[4032] ;
  int rawTPEmulsFGVB2[4032] ;
  int rawTPEmulsFGVB3[4032] ;
  int rawTPEmulsFGVB4[4032] ;
  int rawTPEmulsFGVB5[4032] ;
  
  float eRec[4032] ;
  int crystNb[4032] ;
  int spike[4032] ;
  int sevlv[4032];
  int sevlv2[4032];
  int ttFlag[4032];
  int trig_tower_adc[4032], sFGVB[4032]; 
   
  
  Int_t nbOfL1IsoCands ;
  Int_t L1IsoIeta[4] ;
  Int_t L1IsoIphi[4] ;
  Int_t L1IsoRank[4] ; 
  Int_t nbOfL1NonisoCands ;
  Int_t L1NonisoIeta[4] ;
  Int_t L1NonisoIphi[4] ;
  Int_t L1NonisoRank[4] ; 

  Int_t nbOfL1IsoEmulCands ;
  Int_t L1IsoEmulIeta[4] ;
  Int_t L1IsoEmulIphi[4] ;
  Int_t L1IsoEmulRank[4] ; 
  Int_t nbOfL1NonisoEmulCands ;
  Int_t L1NonisoEmulIeta[4] ;
  Int_t L1NonisoEmulIphi[4] ;
  Int_t L1NonisoEmulRank[4] ; 

  
  uint nbOfL1preIsoCands ;
  int L1preIsoIeta[4] ;
  int L1preIsoIphi[4] ;
  int L1preIsoRank[4] ; 
  uint nbOfL1preNonisoCands ;
  int L1preNonisoIeta[4] ;
  int L1preNonisoIphi[4] ;
  int L1preNonisoRank[4] ; 
  
  uint nbOfL1postIsoCands ;
  int L1postIsoIeta[4] ;
  int L1postIsoIphi[4] ;
  int L1postIsoRank[4] ; 
  uint nbOfL1postNonisoCands ;
  int L1postNonisoIeta[4] ;
  int L1postNonisoIphi[4] ;
  int L1postNonisoRank[4] ; 
 
  tr_ECAL->SetBranchAddress ("nRun",&runNb) ; 
  tr_ECAL->SetBranchAddress ("nEvent",&evtNb) ; 
  tr_ECAL->SetBranchAddress ("nLumi",&lumiBlock) ;
  // tr_ECAL->SetBranchAddress ("orbitNb",&orbitNb) ; 
  // tr_ECAL->SetBranchAddress ("timeStamp",&timeStamp) ; 
  // tr_ECAL->SetBranchAddress ("bxNb",&bxNb) ;   
  
  //vertices
  tr_ECAL->SetBranchAddress ("vtx_N",&nVertices) ; 

  //crystal level info
  tr_ECAL->SetBranchAddress ("n_bad_crystals",&n_bad_crystals) ; 
  tr_ECAL->SetBranchAddress ("erec_Et_sevlv3_4",&erec_Et_sevlv3_4) ; 
  tr_ECAL->SetBranchAddress ("erec_eta_sevlv3_4",&erec_eta_sevlv3_4) ; 
  tr_ECAL->SetBranchAddress ("erec_phi_sevlv3_4",&erec_phi_sevlv3_4) ; 
   tr_ECAL->SetBranchAddress ("erec_theta_sevlv3_4",&erec_theta_sevlv3_4) ; 

  tr_ECAL->SetBranchAddress ("nbOfTowers",&nbOfTowers) ; 
  tr_ECAL->SetBranchAddress ("ieta",ieta) ; 
  tr_ECAL->SetBranchAddress ("iphi",iphi) ; 
  tr_ECAL->SetBranchAddress ("nbOfXtals",nbOfXtals) ; 
  tr_ECAL->SetBranchAddress ("rawTPData",rawTPData) ; 
  tr_ECAL->SetBranchAddress ("rawTPEmul1",rawTPEmul1) ; 
  tr_ECAL->SetBranchAddress ("rawTPEmul2",rawTPEmul2) ; 
  tr_ECAL->SetBranchAddress ("rawTPEmul3",rawTPEmul3) ; 
  tr_ECAL->SetBranchAddress ("rawTPEmul4",rawTPEmul4) ; 
  tr_ECAL->SetBranchAddress ("rawTPEmul5",rawTPEmul5) ; 
  tr_ECAL->SetBranchAddress ("eRec",eRec) ; 
  tr_ECAL->SetBranchAddress ("crystNb",crystNb) ;
  tr_ECAL->SetBranchAddress ("spike",spike) ;
  tr_ECAL->SetBranchAddress ("sevlv", sevlv);
  tr_ECAL->SetBranchAddress ("sevlv2", sevlv2);
  tr_ECAL->SetBranchAddress ("ttFlag", ttFlag);
  tr_ECAL->SetBranchAddress ("trig_tower_adc",trig_tower_adc) ; 
  tr_ECAL->SetBranchAddress ("sFGVB",sFGVB) ; 
  tr_ECAL->SetBranchAddress ("rawTPEmulsFGVB1",rawTPEmulsFGVB1) ; 
  tr_ECAL->SetBranchAddress ("rawTPEmulsFGVB2",rawTPEmulsFGVB2) ; 
  tr_ECAL->SetBranchAddress ("rawTPEmulsFGVB3",rawTPEmulsFGVB3) ; 
  tr_ECAL->SetBranchAddress ("rawTPEmulsFGVB4",rawTPEmulsFGVB4) ; 
  tr_ECAL->SetBranchAddress ("rawTPEmulsFGVB5",rawTPEmulsFGVB5) ; 
  tr_ECAL->SetBranchAddress ("rawTPEmulttFlag1",rawTPEmulttFlag1) ; 
  tr_ECAL->SetBranchAddress ("rawTPEmulttFlag2",rawTPEmulttFlag2) ; 
  tr_ECAL->SetBranchAddress ("rawTPEmulttFlag3",rawTPEmulttFlag3) ; 
  tr_ECAL->SetBranchAddress ("rawTPEmulttFlag4",rawTPEmulttFlag4) ; 
  tr_ECAL->SetBranchAddress ("rawTPEmulttFlag5",rawTPEmulttFlag5) ; 
  tr_ECAL->SetBranchAddress ("trig_L1emIso_N",&nbOfL1IsoCands); //
  tr_ECAL->SetBranchAddress ("trig_L1emIso_ieta", L1IsoIeta);//
  tr_ECAL->SetBranchAddress ("trig_L1emIso_iphi", L1IsoIphi);//
  tr_ECAL->SetBranchAddress ("trig_L1emIso_rank", L1IsoRank);//
  
  tr_ECAL->SetBranchAddress ("trig_L1emNonIso_N",&nbOfL1NonisoCands); //
  tr_ECAL->SetBranchAddress ("trig_L1emNonIso_ieta", L1NonisoIeta);//
  tr_ECAL->SetBranchAddress ("trig_L1emNonIso_iphi", L1NonisoIphi);//
  tr_ECAL->SetBranchAddress ("trig_L1emNonIso_rank", L1NonisoRank);//
  
  
  //emul l1
  tr_ECAL->SetBranchAddress ("trig_L1emIso_N_M",&nbOfL1IsoEmulCands); //
  tr_ECAL->SetBranchAddress ("trig_L1emIso_ieta_M", L1IsoEmulIeta);//
  tr_ECAL->SetBranchAddress ("trig_L1emIso_iphi_M", L1IsoEmulIphi);//
  tr_ECAL->SetBranchAddress ("trig_L1emIso_rank_M", L1IsoEmulRank);//
  
  tr_ECAL->SetBranchAddress ("trig_L1emNonIso_N_M",&nbOfL1NonisoEmulCands); //
  tr_ECAL->SetBranchAddress ("trig_L1emNonIso_ieta_M", L1NonisoEmulIeta);//
  tr_ECAL->SetBranchAddress ("trig_L1emNonIso_iphi_M", L1NonisoEmulIphi);//
  tr_ECAL->SetBranchAddress ("trig_L1emNonIso_rank_M", L1NonisoEmulRank);//
  
 
  tr_ECAL->SetBranchAddress ("nbOfL1preIsoCands",&nbOfL1preIsoCands); //
  tr_ECAL->SetBranchAddress ("L1preIsoIeta", L1preIsoIeta);//
  tr_ECAL->SetBranchAddress ("L1preIsoIphi", L1preIsoIphi);//
  tr_ECAL->SetBranchAddress ("L1preIsoRank", L1preIsoRank);//
  
  tr_ECAL->SetBranchAddress ("nbOfL1preNonisoCands",&nbOfL1preNonisoCands); //
  tr_ECAL->SetBranchAddress ("L1preNonisoIeta", L1preNonisoIeta);//
  tr_ECAL->SetBranchAddress ("L1preNonisoIphi", L1preNonisoIphi);//
  tr_ECAL->SetBranchAddress ("L1preNonisoRank", L1preNonisoRank);//
  
  tr_ECAL->SetBranchAddress ("nbOfL1postIsoCands",&nbOfL1postIsoCands); //
  tr_ECAL->SetBranchAddress ("L1postIsoIeta", L1postIsoIeta);//
  tr_ECAL->SetBranchAddress ("L1postIsoIphi", L1postIsoIphi);//
  tr_ECAL->SetBranchAddress ("L1postIsoRank", L1postIsoRank);//
  
  tr_ECAL->SetBranchAddress ("nbOfL1postNonisoCands",&nbOfL1postNonisoCands); //
  tr_ECAL->SetBranchAddress ("L1postNonisoIeta", L1postNonisoIeta);//
  tr_ECAL->SetBranchAddress ("L1postNonisoIphi", L1postNonisoIphi);//
  tr_ECAL->SetBranchAddress ("L1postNonisoRank", L1postNonisoRank);//  

  //   std::string inputfiles = "ECALTPGtree.root";
  //  std::string inputfiles = "/afs/cern.ch/work/n/ndev/Spikes/CMSSW_7_4_12/src/EcalPFG/Scripts/ECALTPGtree_18_22_254833.root";
  std::string geomName = "/afs/cern.ch/work/d/dkonst/TPG/CMSSW_7_3_2/src/EcalPFG/Scripts/TriggerAnalysis/macros/endcapGeometry.txt";
  std::string inputdir, maskfileName  ;
  std::string outputRootName = "histoTPG.root" ;
  int verbose = 0 ;
  int occupancyCut = 3 ;
  
  Double_t slMinEvt = 0;
  Double_t slMaxEvt = 100000000;
  Double_t slMinOrb = 0;
  Double_t slMaxOrb = 100000000;
  Double_t slMinLS = 0;
  Double_t slMaxLS = 1000;
  Double_t slMinTime = 0;
  Double_t slMaxTime = 100;
  Long64_t firstEntry = 0;
  Long64_t lastEntry = -1;
  // This one we define eg = 2 (can change to something else for different eg threshold)
  int eg = 2;
  ///////////////////////
  // parse geometry file
  ///////////////////////

  uint ref = 2 ;  //emulator array index corresponding to 3rd sample of 5.
  //this should match the real TP data (theoretically)

  ifstream fin ;
  std::string line;
  int hashedIndex, ix, iy;
  std::map < int , std::vector< std::pair<int, int> > > geometry;
  
  fin.open(geomName.c_str());
  if (fin.is_open())
    {
      while(!fin.eof()) {
	fin >> hashedIndex >> ix >> iy;
	if(fin.eof()){
	  break ; // avoid last line duplication
	}
	geometry[hashedIndex].push_back( std::pair<int,int>(ix,iy) ); 
      }
    }
  
  ///////////////////
  //PU vertices histos
  ///////////////////
  
  //numberoftps

   //barrel
   TH1F * vertices_num_tpEB = new TH1F("NUM_TPEB_N_vertices", "NUM_TPEB_N_Vertices",80, 0., 80.) ;
   vertices_num_tpEB->GetXaxis()->SetTitle("Number of Vertices") ;
   vertices_num_tpEB->GetYaxis()->SetTitle("Number of Tp") ;
   

   TH1F * vertices_num_tpEBspike = new TH1F("NUM_TPEB_spike_N_vertices", "NUM_TPEB_spike_N_Vertices",80, 0., 80.) ;
   vertices_num_tpEBspike->GetXaxis()->SetTitle("Number of Vertices") ;
   vertices_num_tpEBspike->GetYaxis()->SetTitle("Number of Tp") ;

   TH1F * vertices_num_tpEBemul = new TH1F("NUM_TPEB_emul_N_vertices", "NUM_TPEB_emul_N_Vertices",80, 0., 80.) ;
   vertices_num_tpEBemul->GetXaxis()->SetTitle("Number of Vertices") ;
   vertices_num_tpEBemul->GetYaxis()->SetTitle("Number of Tp") ;

   TH1F * vertices_num_tpEBemul_spike = new TH1F("NUM_TPEB_emulspike_N_vertices", "NUM_TPEB_emulspike_N_Vertices",80, 0., 80.) ;
   vertices_num_tpEBemul_spike->GetXaxis()->SetTitle("Number of Vertices") ;
   vertices_num_tpEBemul_spike->GetYaxis()->SetTitle("Number of Tp") ;

   //same plots for eg 30 and eg 23

   TH1F * vertices_num_tpEBeg23_ = new TH1F("NUM_TPEBeg23__N_vertices", "NUM_TPEBeg23__N_Vertices",80, 0., 80.) ;
   vertices_num_tpEBeg23_->GetXaxis()->SetTitle("Number of Vertices") ;
   vertices_num_tpEBeg23_->GetYaxis()->SetTitle("Number of Tp") ;
   

   TH1F * vertices_num_tpEBeg23_spike = new TH1F("NUM_TPEBeg23__spike_N_vertices", "NUM_TPEBeg23__spike_N_Vertices",80, 0., 80.) ;
   vertices_num_tpEBeg23_spike->GetXaxis()->SetTitle("Number of Vertices") ;
   vertices_num_tpEBeg23_spike->GetYaxis()->SetTitle("Number of Tp") ;

   TH1F * vertices_num_tpEBeg23_emul = new TH1F("NUM_TPEBeg23__emul_N_vertices", "NUM_TPEBeg23__emul_N_Vertices",80, 0., 80.) ;
   vertices_num_tpEBeg23_emul->GetXaxis()->SetTitle("Number of Vertices") ;
   vertices_num_tpEBeg23_emul->GetYaxis()->SetTitle("Number of Tp") ;

   TH1F * vertices_num_tpEBeg23_emul_spike = new TH1F("NUM_TPEBeg23__emulspike_N_vertices", "NUM_TPEBeg23__emulspike_N_Vertices",80, 0., 80.) ;
   vertices_num_tpEBeg23_emul_spike->GetXaxis()->SetTitle("Number of Vertices") ;
   vertices_num_tpEBeg23_emul_spike->GetYaxis()->SetTitle("Number of Tp") ;
   

   TH1F * vertices_num_tpEBeg30_ = new TH1F("NUM_TPEBeg30__N_vertices", "NUM_TPEBeg30__N_Vertices",80, 0., 80.) ;
   vertices_num_tpEBeg30_->GetXaxis()->SetTitle("Number of Vertices") ;
   vertices_num_tpEBeg30_->GetYaxis()->SetTitle("Number of Tp") ;
   

   TH1F * vertices_num_tpEBeg30_spike = new TH1F("NUM_TPEBeg30__spike_N_vertices", "NUM_TPEBeg30__spike_N_Vertices",80, 0., 80.) ;
   vertices_num_tpEBeg30_spike->GetXaxis()->SetTitle("Number of Vertices") ;
   vertices_num_tpEBeg30_spike->GetYaxis()->SetTitle("Number of Tp") ;

   TH1F * vertices_num_tpEBeg30_emul = new TH1F("NUM_TPEBeg30__emul_N_vertices", "NUM_TPEBeg30__emul_N_Vertices",80, 0., 80.) ;
   vertices_num_tpEBeg30_emul->GetXaxis()->SetTitle("Number of Vertices") ;
   vertices_num_tpEBeg30_emul->GetYaxis()->SetTitle("Number of Tp") ;

   TH1F * vertices_num_tpEBeg30_emul_spike = new TH1F("NUM_TPEBeg30__emulspike_N_vertices", "NUM_TPEBeg30__emulspike_N_Vertices",80, 0., 80.) ;
   vertices_num_tpEBeg30_emul_spike->GetXaxis()->SetTitle("Number of Vertices") ;
   vertices_num_tpEBeg30_emul_spike->GetYaxis()->SetTitle("Number of Tp") ;
   

   //endcap plus
   TH1F * vertices_num_tpEEPlus = new TH1F("NUM_TPEEPlus_N_vertices", "NUM_TPEEPlusN_Vertices",80, 0., 80.) ;
   vertices_num_tpEEPlus->GetXaxis()->SetTitle("Number of Vertices") ;
   vertices_num_tpEEPlus->GetYaxis()->SetTitle("Number of Tp") ;

   TH1F * vertices_num_tpEEPlusemul = new TH1F("NUM_TPEEPlus_emul_N_vertices", "NUM_TPEEPlus_emul_N_Vertices",80, 0., 80.) ;
   vertices_num_tpEEPlusemul->GetXaxis()->SetTitle("Number of Vertices") ;
   vertices_num_tpEEPlusemul->GetYaxis()->SetTitle("Number of Tp") ;

   //endcap minus
   TH1F * vertices_num_tpEEMinus = new TH1F("NUM_TPEEMinus_N_vertices", "NUM_TPEEMinus_N_Vertices",80, 0., 80.) ;
   vertices_num_tpEEMinus->GetXaxis()->SetTitle("Number of Vertices") ;
   vertices_num_tpEEMinus->GetYaxis()->SetTitle("Number of Tp") ;

   TH1F * vertices_num_tpEEMinusemul = new TH1F("NUM_TPEEMinus_emul_N_vertices", "NUM_TPEEMinus_emul_N_Vertices",80, 0., 80.) ;
   vertices_num_tpEEMinusemul->GetXaxis()->SetTitle("Number of Vertices") ;
   vertices_num_tpEEMinusemul->GetYaxis()->SetTitle("Number of Tp") ;

   //numberoftps-eta wise for different num_vertices range

   //vertex range 1 - less than 10 vertices
   //vertex range 2 - 10-22 vertices
   //vertex range 3 - 23-35 vertices
   //vertex range 4 - more than 35 vertices
   TH1F *numTP_ieta_Vrange1 = new TH1F("Num_TP_eta_less_than_10_vertices","Num_TP_eta_less_than_10_vertices",64,-32.,32.)  ;
   numTP_ieta_Vrange1->GetXaxis()->SetTitle("ieta");
   numTP_ieta_Vrange1->GetYaxis()->SetTitle("numTP");
   TH1F *numTP_ieta_Vrange2 = new TH1F("Num_TP_eta_10-22_vertices","Num_TP_eta_less_than_10_vertices",64,-32.,32.)  ;
   numTP_ieta_Vrange2->GetXaxis()->SetTitle("ieta");
   numTP_ieta_Vrange2->GetYaxis()->SetTitle("numTP");
   TH1F *numTP_ieta_Vrange3 = new TH1F("Num_TP_eta_23-25_vertices","Num_TP_eta_less_than_10_vertices",64,-32.,32.)  ;
   numTP_ieta_Vrange3->GetXaxis()->SetTitle("ieta");
   numTP_ieta_Vrange3->GetYaxis()->SetTitle("numTP");
   TH1F *numTP_ieta_Vrange4 = new TH1F("Num_TP_eta_more_than_35_vertices","Num_TP_eta_less_than_10_vertices",64,-32.,32.)  ;
   numTP_ieta_Vrange4->GetXaxis()->SetTitle("ieta"); 
   numTP_ieta_Vrange4->GetYaxis()->SetTitle("numTP");    

   //tp_spectrum for different num_vertices range
   TH1F *tpSpectrum_Vrange1 = new TH1F("TP_spectrum_less_than_10_vertices","TP_spectrum_less_than_10_vertices",256,0.,256.)  ;
   tpSpectrum_Vrange1->GetXaxis()->SetTitle("TP(ADC)");
   tpSpectrum_Vrange1->GetYaxis()->SetTitle("numTP");
   TH1F *tpSpectrum_Vrange2 = new TH1F("TP_spectrum_10-22_vertices","TP_spectrum_less_than_10_vertices",256,0.,256.)  ;
   tpSpectrum_Vrange2->GetXaxis()->SetTitle("TP(ADC)");
   tpSpectrum_Vrange2->GetYaxis()->SetTitle("numTP");
   TH1F *tpSpectrum_Vrange3 = new TH1F("TP_spectrum_23-25_vertices","TP_spectrum_less_than_10_vertices",256,0.,256.)  ;
   tpSpectrum_Vrange3->GetXaxis()->SetTitle("TP(ADC)");
   tpSpectrum_Vrange3->GetYaxis()->SetTitle("numTP");
   TH1F *tpSpectrum_Vrange4 = new TH1F("TP_spectrum_more_than_35_vertices","TP_spectrum_less_than_10_vertices",256,0.,256.)  ;
   tpSpectrum_Vrange4->GetXaxis()->SetTitle("TP(ADC)"); 
   tpSpectrum_Vrange4->GetYaxis()->SetTitle("numTP");    

   //Crystal level info 
   TH1F *eRec_badXtal = new TH1F("Et_xtal_sevlv_3_or_4","Et_xtal_sevlv_3_or_4",256,0.,256.)  ;
   eRec_badXtal->GetXaxis()->SetTitle("Et(GeV)");
   TH1F *eta_badXtal = new TH1F("eta_xtal_sevlv_3_or_4","eta_xtal_sevlv_3_or_4",30,0.,30.)  ;
   eta_badXtal->GetXaxis()->SetTitle("Eta");
   TH1F *phi_badXtal = new TH1F("phi_xtal_sevlv_3_or_4","phi_xtal_sevlv_3_or_4",6,0.,6.)  ;
   phi_badXtal->GetXaxis()->SetTitle("Phi");
   TH1F *theta_badXtal = new TH1F("theta_xtal_sevlv_3_or_4","theta_xtal_sevlv_3_or_4",6,0.,6.)  ;
   theta_badXtal->GetXaxis()->SetTitle("Theta");

   

   /////////////////////////
   //L1 candidate histograms
   /////////////////////////

   // TH1F * nbOfL1IsoCands = new TH1F("nbOfL1IsoCands", "nbOfL1IsoCands", 5, 0., 5.) ;
   // TH1F * nbOfL1NonisoCands = new TH1F("nbOfL1NonisoCands", "nbOfL1NonisoCands", 5, 0., 5.) ; 
   TH1F * L1IsoCandRank = new TH1F("L1IsoCandRank", "L1IsoCandRank", 64, 0., 64.) ;
   TH1F * L1NonisoCandRank = new TH1F("L1NonisoCandRank", "L1NonisoCandRank", 64, 0., 64.) ; 

   TH1F * L1IsoCandRank_spikes = new TH1F("L1IsoCandRank_spikes", "L1IsoCandRank_spikes", 64, 0., 64.) ;
   TH1F * L1NonisoCandRank_spikes = new TH1F("L1NonisoCandRank_spikes", "L1NonisoCandRank_spikes", 64, 0., 64.) ; 

   //emul l1
   TH1F * L1IsoEmulCandRank = new TH1F("L1IsoEmulCandRank", "L1IsoEmulCandRank", 64, 0., 64.) ;
   TH1F * L1NonisoEmulCandRank = new TH1F("L1NonisoEmulCandRank", "L1NonisoEmulCandRank", 64, 0., 64.) ; 

   TH1F * L1IsoEmulCandRank_spikes = new TH1F("L1IsoEmulCandRank_spikes", "L1IsoEmulCandRank_spikes", 64, 0., 64.) ;
   TH1F * L1NonisoEmulCandRank_spikes = new TH1F("L1NonisoEmulCandRank_spikes", "L1NonisoEmulCandRank_spikes", 64, 0., 64.) ; 


   // TH1F * nbOfL1preIsoCands = new TH1F("nbOfL1preIsoCands", "nbOfL1preIsoCands", 5, 0., 5.) ;
   // TH1F * nbOfL1preNonisoCands = new TH1F("nbOfL1preNonisoCands", "nbOfL1preNonisoCands", 5, 0., 5.) ; 
   TH1F * L1preIsoCandRank = new TH1F("L1preIsoCandRank", "L1preIsoCandRank", 64, 0., 64.) ;
   TH1F * L1preNonisoCandRank = new TH1F("L1preNonisoCandRank", "L1preNonisoCandRank", 64, 0., 64.) ; 
   
   // TH1F * nbOfL1postIsoCands = new TH1F("nbOfL1postIsoCands", "nbOfL1postIsoCands", 5, 0., 5.) ;
   // TH1F * nbOfL1postNonisoCands = new TH1F("nbOfL1postNonisoCands", "nbOfL1postNonisoCands", 5, 0., 5.) ; 
   TH1F * L1postIsoCandRank = new TH1F("L1postIsoCandRank", "L1postIsoCandRank", 64, 0., 64.) ;
   TH1F * L1postNonisoCandRank = new TH1F("L1postNonisoCandRank", "L1postNonisoCandRank", 64, 0., 64.) ; 
   
   TH1F * L1Isotiming = new TH1F("L1Isotiming", "L1Isotiming", 5, -2., 3.) ;
   TH1F * L1Nonisotiming = new TH1F("L1Nonisotiming", "L1Nonisotiming", 5, -2., 3.) ;
   
   TH1F * L1IsotimingEB = new TH1F("L1IsotimingEB", "L1IsotimingEB", 5, -2., 3.) ;
   TH1F * L1NonisotimingEB = new TH1F("L1NonisotimingEB", "L1NonisotimingEB", 5, -2., 3.) ;
   
   TH1F * L1IsotimingEEPlus = new TH1F("L1IsotimingEEPlus", "L1IsotimingEEPlus", 5, -2., 3.) ;
   TH1F * L1NonisotimingEEPlus = new TH1F("L1NonisotimingEEPlus", "L1NonisotimingEEPlus", 5, -2., 3.) ;
   
   TH1F * L1IsotimingEEMinus = new TH1F("L1IsotimingEEMinus", "L1IsotimingEEMinus", 5, -2., 3.) ;
   TH1F * L1NonisotimingEEMinus = new TH1F("L1NonisotimingEEMinus", "L1NonisotimingEEMinus", 5, -2., 3.) ;

   

   //Spikes information

   TH2F * spikesEB2D = new TH2F("TowerSpikeEB2D", "Tower Spikes: Barrel", 72, 1, 73, 38, -19, 19) ;
   TH2F * spikesEB2DADC = new TH2F("TowerSpikeEB2DADC", "Tower Spikes, ADC weight: Barrel", 72, 1, 73, 38, -19, 19) ;
   TH2F * spikesEEPlus2D = new TH2F("TowerSpikeEEPlus2D", "Tower Spikes: EE+", 121, -10, 111, 121, -10, 111) ;
   TH2F * spikesEEPlus2DADC = new TH2F("TowerSpikeEEPlus2DADC", "Tower Spikes, ADC weight: EE+", 121, -10, 111, 121, -10, 111) ;
   TH2F * spikesEEMinus2D = new TH2F("TowerSpikeEEMinus2D", "Tower Spikes: EE-", 121, -10, 111, 121, -10, 111) ;
   TH2F * spikesEEMinus2DADC = new TH2F("TowerSpikeEEMinus2DADC", "Tower Spikes, ADC weight: EE-", 121, -10, 111, 121, -10, 111) ;

   //TPMatchEmulEB


   // ============================= Fill histos related to time evolution
   int timeBin = int(slMaxTime-slMinTime);
   
   //std::cout << "time bin: " << timeBin << std::endl;
   TH1F * occupancyTP_vs_EvtNb = new TH1F("occupancyTP_vs_EvtNb", "TP occupancy vs. EvtNb", 1000, slMinEvt, slMaxEvt) ;
   occupancyTP_vs_EvtNb->GetXaxis()->SetTitle("EvtNb / 10^6") ;
   occupancyTP_vs_EvtNb->GetYaxis()->SetTitle("# of TPs") ;
   
   //TP Spectrum

   TH1F * TPEB = new TH1F("TPEB", "TP: Barrel", 256, 0., 256.) ;
   TPEB->GetXaxis()->SetTitle("TP (ADC)") ;
   TH1F * TPEB_Spike = new TH1F("TPEB_Spike", "TP: Barrel (sevlv3 or 4)", 256, 0., 256.) ;
   TPEB->GetXaxis()->SetTitle("TP (ADC)") ;
   TH1F * TPEB_Spike2 = new TH1F("TPEB_Spike2", "TP: Barrel (sevlv3 or 4)", 256, 0., 256.) ;
   TPEB->GetXaxis()->SetTitle("TP (ADC)") ;
   TH1F * TPEB_sFGVB0 = new TH1F("TPEB_sFGVB0", "TP (sFGVB0): Barrel", 256, 0., 256.) ;
   TPEB_sFGVB0->GetXaxis()->SetTitle("TP (ADC)") ;
   
   //spectrum by PU bins
   int num_pu_bins=4;
   int pu_binning[4]={0,10,20,9999}; 

   //real/online TP

   TH1F * ALL_TP[num_pu_bins];
   TH1F * TP_sevlv3_4[num_pu_bins];   
   TH1F * TP_sFGVB0[num_pu_bins];
   int events_by_bin[4]={0};
   
   for (int i=0;i<num_pu_bins;i++)
     {
       
       char all_tp_name[120];
       char tp_sevlv3_4_name[120];
       char tp_sFGVB0_name[120];

       sprintf(all_tp_name,"ALL_TP_PUbin%i",i+1);
       sprintf(tp_sFGVB0_name,"TP_SFGVB0_PUbin%i",i+1);
       sprintf(tp_sevlv3_4_name,"TP_SEVLV3_4_PUbin%i",i+1);


       char all_tp_title[120];
       char tp_sevlv3_4_title[120];
       char tp_sFGVB0_title[120];

       sprintf(all_tp_title,"ALL TP: Barrel, PUbin%i",i+1);
       sprintf(tp_sFGVB0_title,"TP: Barrel (sevlv3 or 4), PUbin%i",i+1);
       sprintf(tp_sevlv3_4_title,"TP (sFGVB0): Barrel, PUbin%i",i+1);

       ALL_TP[i]=new TH1F(all_tp_name,all_tp_title, 256, 0., 256.) ;
       TP_sevlv3_4[i]=new TH1F(tp_sevlv3_4_name, tp_sevlv3_4_title, 256, 0., 256.) ;
       TP_sFGVB0[i]=new TH1F(tp_sFGVB0_name, tp_sFGVB0_title, 256, 0., 256.) ;
     }
   

   //emualted TP
   TH1F * ALL_TPEMUL[num_pu_bins];
   TH1F * TPEMUL_sevlv3_4[num_pu_bins];   
   TH1F * TPEMUL_sFGVB0[num_pu_bins];
   int events_by_bin_emul[4]={0};
   
   for (int i=0;i<num_pu_bins;i++)
     {
       
       char all_tpEMUL_name[120];
       char tpEMUL_sevlv3_4_name[120];
       char tpEMUL_sFGVB0_name[120];

       sprintf(all_tpEMUL_name,"ALL_TPEMUL_PUbin%i",i+1);
       sprintf(tpEMUL_sFGVB0_name,"TPEMUL_SFGVB0_PUbin%i",i+1);
       sprintf(tpEMUL_sevlv3_4_name,"TPEMUL_SEVLV3_4_PUbin%i",i+1);


       char all_tpEMUL_title[120];
       char tpEMUL_sevlv3_4_title[120];
       char tpEMUL_sFGVB0_title[120];

       sprintf(all_tpEMUL_title,"ALL TPEMUL: Barrel, PUbin%i",i+1);
       sprintf(tpEMUL_sFGVB0_title,"TPEMUL: Barrel (sevlv3 or 4), PUbin%i",i+1);
       sprintf(tpEMUL_sevlv3_4_title,"TPEMUL (sFGVB0): Barrel, PUbin%i",i+1);

       ALL_TPEMUL[i]=new TH1F(all_tpEMUL_name,all_tpEMUL_title, 256, 0., 256.) ;
       TPEMUL_sevlv3_4[i]=new TH1F(tpEMUL_sevlv3_4_name, tpEMUL_sevlv3_4_title, 256, 0., 256.) ;
       TPEMUL_sFGVB0[i]=new TH1F(tpEMUL_sFGVB0_name, tpEMUL_sFGVB0_title, 256, 0., 256.) ;
     }

   
   

 
   TH1F * TPEB_noSpike = new TH1F("TPEB_noSpike", "low energy spike removed TP: Barrel", 256, 0., 256.) ;
   TPEB_noSpike->GetXaxis()->SetTitle("TP (ADC)") ;
   TH1F * TPEB_sevlv0 = new TH1F("TPEB_sevlv0", "TP (sevlv0): Barrel", 256, 0., 256.) ;
   TPEB_sevlv0->GetXaxis()->SetTitle("TP (ADC)") ;
   TH1F * TPEEPlus = new TH1F("TPEEPlus", "TP: EE Plus", 256, 0., 256.) ;
   TPEEPlus->GetXaxis()->SetTitle("TP (ADC)") ;
   TH1F * TPEEMinus = new TH1F("TPEEMinus", "TP: EE Minus", 256, 0., 256.) ;
   TPEEMinus->GetXaxis()->SetTitle("TP (ADC)") ;

   TH2F * TPEB_vs_ieta = new TH2F("TPEB_vs_ieta", "TP vs ieta", 57, -28, 29, 256, 0., 256.) ;
   TPEB_vs_ieta->GetXaxis()->SetTitle("ieta") ;
   TPEB_vs_ieta->GetYaxis()->SetTitle("TP (ADC)") ;
   TH2F * TPEB_vs_ieta_noSpike = new TH2F("TPEB_vs_ieta_noSpike", "TP vs ieta (barrel: low energy spike removed)", 57, -28, 29, 256, 0., 256.) ;
   TPEB_vs_ieta_noSpike->GetXaxis()->SetTitle("ieta") ;
   TPEB_vs_ieta_noSpike->GetYaxis()->SetTitle("TP (ADC)") ;
   TH2F * TPEB_vs_ieta_sevlv0 = new TH2F("TPEB_vs_ieta_sevlv0", "TP vs ieta (sevlv0): Barrel", 57, -28, 29, 256, 0., 256.) ;
   TPEB_vs_ieta_sevlv0->GetXaxis()->SetTitle("ieta") ;
   TPEB_vs_ieta_sevlv0->GetYaxis()->SetTitle("TP (ADC)") ;

   TH1F *TPEB_fullReadout = new TH1F("TPEB_fullReadout", "TP, full readout: Barrel", 256, 0., 256.) ;
   TPEB_fullReadout->GetXaxis()->SetTitle("TP (ADC)") ;
   TH1F *TP_5x5Energy_ratio_EB = new TH1F( "TP_5x5Energy_ratio_EB", "TP 5x5 crystal energy ratio: Barrel", 100, 0.,20.) ;
   TH1F *TP_5x5Energy_ratio_fullReadout_EB = new TH1F( "TP_5x5Energy_ratio_fullReadout_EB", "TP 5x5 crystal energy ratio (ttFlag 3): Barrel", 100, 0.,20.) ;
   TH1F *TP_5x5Energy_ratio_24_EB = new TH1F( "TP_5x5Energy_ratio_24_EB", "TP 5x5 crystal energy ratio (tp > 24 ADC): Barrel", 100, 0.,20.) ;
   TH1F *TP_5x5Energy_ratio_fullReadout_24_EB = new TH1F( "TP_5x5Energy_ratio_fullReadout_24_EB", "TP 5x5 crystal energy ratio (tp > 24 ADC, ttFlag 3): Barrel", 100, 0.,20.) ;
   TH2F *TP_vs_5x5Energy_EB = new TH2F( "TP_vs_5x5Energy_EB", "TP vs 5x5 crystal energy: Barrel", 256,0,256,256,0,256);
   TH2F *TP_vs_5x5Energy_fullReadout_EB = new TH2F( "TP_vs_5x5Energy_fullReadout_EB", "TP vs 5x5 crystal energy (ttFlag 3): Barrel", 256,0,256,256,0,256);
   TH2F *TP_vs_5x5Energy_24_EB = new TH2F( "TP_vs_5x5Energy_24_EB", "TP vs 5x5 crystal energy (tp > 24 ADC): Barrel", 256,0,256,256,0,256);
   TH2F *TP_vs_5x5Energy_fullReadout_24_EB = new TH2F( "TP_vs_5x5Energy_fullReadout_24_EB", "TP vs 5x5 crystal energy ( (tp > 24 ADC, ttFlag 3): Barrel", 256,0,256,256,0,256);
   TH1F *TPEB_fullReadout_spikesIn = new TH1F("TPEB_fullReadout_spikesIn", "TP, full readout (ttFlag 3, spikes incl.): Barrel", 256, 0., 256.) ;
   TPEB_fullReadout->GetXaxis()->SetTitle("TP (ADC)") ;
   TH1F *TP_5x5Energy_ratio_spikesIn_EB = new TH1F( "TP_5x5Energy_ratio_spikesIn_EB", "TP 5x5 crystal energy ratio  (spikes incl.): Barrel", 100, 0.,20.) ;
   TH1F *TP_5x5Energy_ratio_spikesIn_fullReadout_EB = new TH1F( "TP_5x5Energy_ratio_spikesIn_fullReadout_EB", "TP 5x5 crystal energy ratio (ttFlag 3, spikes incl.): Barrel", 100, 0.,20.) ;
   TH1F *TP_5x5Energy_ratio_spikesIn_24_EB = new TH1F( "TP_5x5Energy_ratio_spikesIn_24_EB", "TP 5x5 crystal energy ratio  (tp > 24 ADC, spikes incl.): Barrel", 100, 0.,20.) ;
   TH1F *TP_5x5Energy_ratio_spikesIn_fullReadout_24_EB = new TH1F( "TP_5x5Energy_ratio_spikesIn_fullReadout_24_EB", "TP 5x5 crystal energy ratio (tp > 24 ADC, ttFlag 3, spikes incl.): Barrel", 100, 0.,20.) ;
   TH2F *TP_vs_5x5Energy_spikesIn_EB = new TH2F( "TP_vs_5x5Energy_spikesIn_EB", "TP vs 5x5 crystal energy ratio  (spikes incl.): Barrel", 256,0,256,256,0,256);
   TH2F *TP_vs_5x5Energy_spikesIn_fullReadout_EB = new TH2F( "TP_vs_5x5Energy_spikesIn_fullReadout_EB", "TP vs 5x5 crystal energy  (ttFlag 3, spikes incl.): Barrel", 256,0,256,256,0,256);
   TH2F *TP_vs_5x5Energy_spikesIn_24_EB = new TH2F( "TP_vs_5x5Energy_spikesIn_24_EB", "TP vs 5x5 crystal energy ratio  (tp > 24 ADC, spikes incl.): Barrel", 256,0,256,256,0,256);
   TH2F *TP_vs_5x5Energy_spikesIn_fullReadout_24_EB = new TH2F( "TP_vs_5x5Energy_spikesIn_fullReadout_24_EB", "TP vs 5x5 crystal energy  (tp > 24 ADC, ttFlag 3, spikes incl.): Barrel", 256,0,256,256,0,256);
   TH1F *TPEB_fullReadout_sevlv0 = new TH1F("TPEB_fullReadout_sevlv0", "TP, full readout (ttFlag 3, sevlv0): Barrel", 256, 0., 256.) ;
   TPEB_fullReadout->GetXaxis()->SetTitle("TP (ADC)") ;
   TH1F *TP_5x5Energy_ratio_sevlv0_EB = new TH1F( "TP_5x5Energy_ratio_sevlv0_EB", "TP 5x5 crystal energy ratio  (sevlv0): Barrel", 100, 0.,20.) ;
   TH1F *TP_5x5Energy_ratio_fullReadout_sevlv0_EB = new TH1F( "TP_5x5Energy_ratio_sevlv0_fullReadout_EB", "TP 5x5 crystal energy ratio (ttFlag 3, sevlv0): Barrel", 100, 0.,20.) ;
   TH1F *TP_5x5Energy_ratio_sevlv0_24_EB = new TH1F( "TP_5x5Energy_ratio_sevlv0_24_EB", "TP 5x5 crystal energy ratio  (tp > 24 ADC, sevlv0): Barrel", 100, 0.,20.) ;
   TH1F *TP_5x5Energy_ratio_fullReadout_sevlv0_24_EB = new TH1F( "TP_5x5Energy_ratio_sevlv0_fullReadout_24_EB", "TP 5x5 crystal energy ratio (tp > 24 ADC, ttFlag 3, sevlv0): Barrel", 100, 0.,20.) ;
   TH2F *TP_vs_5x5Energy_sevlv0_EB = new TH2F( "TP_vs_5x5Energy_sevlv0_EB", "TP vs 5x5 crystal energy  (sevlv0): Barrel",256, 0.,256,256,0,256) ;
   TH2F *TP_vs_5x5Energy_fullReadout_sevlv0_EB = new TH2F( "TP_vs_5x5Energy_sevlv0_fullReadout_EB", "TP vs 5x5 crystal energy  (ttFlag 3, sevlv0): Barrel",256,0,256,256,0,256);
   TH2F *TP_vs_5x5Energy_sevlv0_24_EB = new TH2F( "TP_vs_5x5Energy_sevlv0_24_EB", "TP vs 5x5 crystal energy  (tp > 24 ADC, sevlv0): Barrel",256, 0.,256,256,0,256) ;
   TH2F *TP_vs_5x5Energy_fullReadout_sevlv0_24_EB = new TH2F( "TP_vs_5x5Energy_sevlv0_fullReadout_24_EB", "TP vs 5x5 crystal energy  (tp > 24 ADC, ttFlag 3, sevlv0): Barrel",256,0,256,256,0,256);
   
   TH1F *TPEEPlus_fullReadout = new TH1F("TPEEPlus_fullReadout", "TP, full readout (ttFlag 3): EE Plus", 256, 0., 256.) ;
   TPEEPlus_fullReadout->GetXaxis()->SetTitle("TP (ADC)") ;
   TH1F *TP_5x5Energy_ratio_EEPlus = new TH1F( "TP_5x5Energy_ratio_EEPlus", "TP  crystal energy ratio: EE Plus",100, 0., 20.) ;
   TH1F *TP_5x5Energy_ratio_fullReadout_EEPlus = new TH1F( "TP_5x5Energy_ratio_fullReadout_EEPlus", "TP 5x5 crystal energy ratio (ttFlag 3): EE Plus",100, 0., 20.) ;
   TH2F *TP_vs_5x5Energy_EEPlus = new TH2F( "TP_vs_5x5Energy_EEPlus", "TP 5x5 crystal energy: EE Plus",256,0,256,256,0,256);
   TH2F *TP_vs_5x5Energy_fullReadout_EEPlus = new TH2F( "TP_vs_5x5Energy_fullReadout_EEPlus", "TP vs 5x5 crystal energy (ttFlag 3): EE Plus",256,0,256,256,0,256);
   
   TH1F *TPEEMinus_fullReadout = new TH1F("TPEEMinus_fullReadout", "TP, full readout: EE Minus", 256, 0., 256.) ;
   TPEEMinus_fullReadout->GetXaxis()->SetTitle("TP (ADC)") ;
   TH1F *TP_5x5Energy_ratio_EEMinus = new TH1F( "TP_5x5Energy_ratio_EEMinus", "TP 5x5 crystal energy ratio: EE Minus", 100, 0., 20.) ;
   TH1F *TP_5x5Energy_ratio_fullReadout_EEMinus = new TH1F( "TP_5x5Energy_ratio_fullReadout_EEMinus", "TP 5x5 crystal energy ratio (ttFlag 3): EE Minus", 100, 0., 20.) ;
   TH2F *TP_vs_5x5Energy_EEMinus = new TH2F( "TP_vs_5x5Energy_EEMinus", "TP  vs 5x5 crystal energy: EE Minus",256,0,256,256,0,256);
   TH2F *TP_vs_5x5Energy_fullReadout_EEMinus = new TH2F( "TP_vs_5x5Energy_fullReadout_EEMinus", "TP vs 5x5 crystal energy  (ttFlag 3): EE Minus",256,0,256,256,0,256);

   TH2F * ratio_vs_ieta = new TH2F ("TP_5x5Energy_ratio_vs_ieta","TP 5x5Energy ratio vs ieta", 57, -28, 29, 100, 0, 10) ;
   ratio_vs_ieta->GetXaxis()->SetTitle("ieta") ;
   ratio_vs_ieta->GetYaxis()->SetTitle("TP/5x5Energy") ;
   TH2F * ratio_vs_ieta_noSpikes = new TH2F ("TP_5x5Energy_ratio_vs_ieta_noSpikes","TP 5x5Energy ratio vs ieta (no spikes)", 57, -28, 29, 100, 0, 10) ;
   ratio_vs_ieta_noSpikes->GetXaxis()->SetTitle("ieta") ;
   ratio_vs_ieta_noSpikes->GetYaxis()->SetTitle("TP/5x5Energy") ;
   TH2F * ratio_vs_ieta_24ADC = new TH2F ("TP_5x5Energy_ratio_vs_ieta_24ADC","TP 5x5Energy ratio vs ieta (tp > 24 ADC)", 57, -28, 29, 100, 0, 10) ;
   ratio_vs_ieta_24ADC->GetXaxis()->SetTitle("ieta") ;
   ratio_vs_ieta_24ADC->GetYaxis()->SetTitle("TP/5x5Energy") ;
   TH2F * ratio_vs_ieta_cleaned = new TH2F ("TP_5x5Energy_ratio_vs_ieta_cleaned","TP 5x5Energy ratio vs ieta (tp > 24 ADC, cleaned)", 57, -28, 29, 100, 0, 10) ;
   ratio_vs_ieta_cleaned->GetXaxis()->SetTitle("ieta") ;
   ratio_vs_ieta_cleaned->GetYaxis()->SetTitle("TP/5x5Energy") ;
   TH1F * ratio_ieta_27_28 = new TH1F("ratio_ieta_27_28", "ratio (ieta = 27 || ieta = 28)", 100, 0, 10) ;
   ratio_ieta_27_28->GetXaxis()->SetTitle("ratio") ;
   TH1F * ratio_tp255 = new TH1F("ratio_tp255", "ratio (tp=255)", 100, 0, 10) ;
   ratio_tp255->GetXaxis()->SetTitle("ratio") ;


   TH1F *hTotTPEnergy_EB = new TH1F("hTotTPEnergy_EB", "Sum of TPs: Barrel"   , 500, 0., 10000) ;
   hTotTPEnergy_EB->GetXaxis()->SetTitle("TP (ADC)") ;
   TH1F *hTotTPEnergy_EEP= new TH1F("hTotTPEnergy_EEP", "Sum of TPs: EE Plus" , 500, 0., 10000) ;
   hTotTPEnergy_EEP->GetXaxis()->SetTitle("TP (ADC)") ;
   TH1F *hTotTPEnergy_EEM= new TH1F("hTotTPEnergy_EEM", "Sum of TPs: EE Minus", 500, 0., 10000) ;
   hTotTPEnergy_EEM->GetXaxis()->SetTitle("TP (ADC)") ;

   TH1F * TPEmulEB = new TH1F("TPEmulEB", "Emulated TPs: Barrel", 256, 0., 256.) ;
   TPEmulEB->GetXaxis()->SetTitle("TP (ADC)") ;


   TH1F * TPEmulEEPlus = new TH1F("TPEmulEEPlus", "Emulated TPs: EE Plus", 256, 0., 256.) ;
   TPEmulEEPlus->GetXaxis()->SetTitle("TP (ADC)") ;
   TH1F * TPEmulEEMinus = new TH1F("TPEmulEEMinus", "Emulated TPs: EE Minus", 256, 0., 256.) ;
   TPEmulEEMinus->GetXaxis()->SetTitle("TP (ADC)") ;

   TH1F * TPEmulEB_realTP = new TH1F("TPEmulEB_realTP", "Emulated TPs, real TP > 0: Barrel", 256, 0., 256.) ;
   TPEmulEB_realTP->GetXaxis()->SetTitle("TP (ADC)") ;
   TH1F * TPEmulEB_realTP_Spike = new TH1F("TPEmulEB_realTP_Spike", "TP: Barrel (sevlv3 or 4)", 256, 0., 256.) ;
   TPEmulEB->GetXaxis()->SetTitle("Emulated TP (ADC)") ;
   TH1F * TPEmulEB_realTP_Spike2 = new TH1F("TPEmulEB_realTP_Spike2", "TP: Barrel (sevlv3 or 4)", 256, 0., 256.) ;
   TPEmulEB->GetXaxis()->SetTitle("Emulated TP (ADC)") ;
   TH1F * TPEmulEB_sFGVB0 = new TH1F("TPEmulEB_sFGVB0", "TP (sFGVB0): Barrel", 256, 0., 256.) ;
   TPEmulEB_sFGVB0->GetXaxis()->SetTitle("TP (ADC)") ;


   TH1F * TPEmulEEPlus_realTP = new TH1F("TPEmulEEPlus_realTP", "Emulated TPs, real TP > 0: EE Plus", 256, 0., 256.) ;
   TPEmulEEPlus_realTP->GetXaxis()->SetTitle("TP (ADC)") ;
   TH1F * TPEmulEEMinus_realTP = new TH1F("TPEmulEEMinus_realTP", "Emulated TPs, real TP > 0: EE Minus", 256, 0., 256.) ;
   TPEmulEEMinus_realTP->GetXaxis()->SetTitle("TP (ADC)") ;
   
   TH1F * TPEmulMaxEB = new TH1F("TPEmulMaxEB", "TP Emulator max: Barrel", 256, 0., 256.) ;
   TPEmulMaxEB->GetXaxis()->SetTitle("TP (ADC)") ;
   TH1F * TPEmulMaxEEPlus = new TH1F("TPEmulMaxEEPlus", "TP Emulator max: Plus Endcap", 256, 0., 256.) ;
   TPEmulMaxEEPlus->GetXaxis()->SetTitle("TP (ADC)") ;
   TH1F * TPEmulMaxEEMinus = new TH1F("TPEmulMaxEEMinus", "TP Emulator max: Minus Endcap", 256, 0., 256.) ;
   TPEmulMaxEEMinus->GetXaxis()->SetTitle("TP (ADC)") ;

   TH1F * TPEmulMaxEB_realTP = new TH1F("TPEmulMaxEB_realTP", "TP Emulator max, real TP > 0: Barrel", 256, 0., 256.) ;
   TPEmulMaxEB_realTP->GetXaxis()->SetTitle("TP (ADC)") ;
   TH1F * TPEmulMaxEEPlus_realTP = new TH1F("TPEmulMaxEEPlus_realTP", "TP Emulator max, real TP > 0: Plus Endcap", 256, 0., 256.) ;
   TPEmulMaxEEPlus_realTP->GetXaxis()->SetTitle("TP (ADC)") ;
   TH1F * TPEmulMaxEEMinus_realTP = new TH1F("TPEmulMaxEEMinus_realTP", "TP Emulator max, real TP > 0: Minus Endcap", 256, 0., 256.) ;
   TPEmulMaxEEMinus_realTP->GetXaxis()->SetTitle("TP (ADC)") ;
   
   TH2F * TPEmulMapMaxIndexEEPlus = new TH2F("TPEmulMapMaxIndexEEPlus", "Map Index of the max TP from Emulator: Plus Endcap",  121, -10, 111, 121, -10, 111);
   TPEmulMapMaxIndexEEPlus->GetYaxis()->SetTitle("y index") ;
   TPEmulMapMaxIndexEEPlus->GetXaxis()->SetTitle("x index") ;
   TH2F * TPEmulMapMaxIndexEEMinus = new TH2F("TPEmulMapMaxIndexEEMinus", "Map Index of the max TP from Emulator: Minus Endcap",  121, -10, 111, 121, -10, 111)  ;
   TPEmulMapMaxIndexEEMinus->GetYaxis()->SetTitle("y index") ;
   TPEmulMapMaxIndexEEMinus->GetXaxis()->SetTitle("x index") ;
   TH2F * TPEmulMapMaxIndexEB = new TH2F("TPEmulMapMaxIndexEB", "Map Index of the max TP from Emulator: Barrel",   72, 1, 73, 38, -19, 19) ;
   TPEmulMapMaxIndexEB->GetYaxis()->SetTitle("eta index") ;
   TPEmulMapMaxIndexEB->GetXaxis()->SetTitle("phi index") ;
   
   
   TH3F * TPspectrumMap3DEB = new TH3F("TPspectrumMap3DEB", "TP  spectrum map: Barrel", 72, 1, 73, 38, -19, 19, 256, 0., 256.) ;
   TPspectrumMap3DEB->GetYaxis()->SetTitle("eta index") ;
   TPspectrumMap3DEB->GetXaxis()->SetTitle("phi index") ;
   
   TH3F * TPspectrumMap3DEEPlus = new TH3F("TPspectrumMap3DEEPlus", "TP  spectrum map: Plus Endcap", 121, -10, 111, 121, -10, 111, 256, -0.5, 255.5) ;
   TPspectrumMap3DEEPlus->GetYaxis()->SetTitle("y index") ;
   TPspectrumMap3DEEPlus->GetXaxis()->SetTitle("x index") ;
   TH3F * TPspectrumMap3DEEMinus = new TH3F("TPspectrumMap3DEEMinus", "TP  spectrum map: Minus Endcap", 121, -10, 111, 121, -10, 111, 256, 0., 256.) ;
   TPspectrumMap3DEEMinus->GetYaxis()->SetTitle("y index") ;
   TPspectrumMap3DEEMinus->GetXaxis()->SetTitle("x index") ;



   ULong64_t maxEvNb=0;
   int   num_events_Vrange1=0;
   int   num_events_Vrange2=0;
   int   num_events_Vrange3=0;
   int   num_events_Vrange4=0;

   int badcrystalnum=0;
   ////////////////////////////
   // Main loop over entries///
   ////////////////////////////

   int iRCT=0;
   double totTPEnergy_EB = 0; 
   double totTPEnergy_EEP = 0; 
   double totTPEnergy_EEM = 0; 
   lastEntry = tr_ECAL->GetEntries();
   cout << "Total number of events == " << lastEntry <<endl; 
     //   lastEntry = 1000;
   for (int entry = firstEntry ; entry < lastEntry ; ++entry)
     //for (int entry = firstEntry ; entry < 1000 ; ++entry)
     {
     tr_ECAL->GetEntry (entry) ;
     //     cout<<"event "<< entry<<endl;
     if (entry%10000==0) std::cout <<"------> Entry "<< entry <<" is being processed" << " <------\n" ; 
     bool keepEvent = false ;
     
     ULong64_t evNb = evtNb;
      if(evNb>maxEvNb) {
	maxEvNb=evNb;
      }
      
     
      //crystal level info
      for (int nxtal=0;nxtal<n_bad_crystals;nxtal++)
	{
	  if ( erec_Et_sevlv3_4[nxtal] >1) 
	    {
	       badcrystalnum++;
	       eRec_badXtal->Fill(erec_Et_sevlv3_4[nxtal]);
	       eta_badXtal->Fill(erec_eta_sevlv3_4[nxtal]);
	       phi_badXtal->Fill(erec_phi_sevlv3_4[nxtal]);
	       theta_badXtal->Fill(erec_theta_sevlv3_4[nxtal]);
	     }
	}
	     

      ///////////////////////////
      //fill some L1 histograms//
      ///////////////////////////

      //=========================================

      //counters for filling L1 histos after looping over all the towers
      
      int MaxTPinIsoRegion[4];
      int MaxTPinIsoRegionIeta[4];
      int MaxTPinIsoRegionIphi[4];
      int SecondTPinIsoRegion[4];
      int sevlev_Iso[4];

      int MaxTPinNonisoRegion[4];
      int MaxTPinNonisoRegionIeta[4];
      int MaxTPinNonisoRegionIphi[4];
      int SecondTPinNonisoRegion[4];
      int sevlev_NonIso[4];

      for(int i=0;i<4;++i)
      {
         MaxTPinIsoRegion[i]=0;
         MaxTPinIsoRegionIeta[i]=0;
         MaxTPinIsoRegionIphi[i]=0;
         SecondTPinIsoRegion[i]=0;
	 sevlev_Iso[i]=0;         
        
	 MaxTPinNonisoRegion[i]=0;
         MaxTPinNonisoRegionIeta[i]=0;
         MaxTPinNonisoRegionIphi[i]=0;
         SecondTPinNonisoRegion[i]=0;
	 sevlev_NonIso[i]=0;         
      }      
   
      for (uint tower = 0 ; tower < nbOfTowers ; tower++)
        {

          //std::cout<<"event kept, in tower loop"<<std::endl;                                                                                

          int tp = getEt(rawTPData[tower]) ;
          int raw_spike = sFGVB[tower];
          float erec_val=eRec[tower];
          int severity_level=sevlv2[tower];

          //do comparison with L1 objects                                                                                                    

	  int ieta_tp = ieta[tower] ;
	  int iphi_tp = iphi[tower] ; 


          for (uint l1isocand = 0 ; l1isocand < nbOfL1IsoCands ; l1isocand++)
            {
              if(L1IsoRank[l1isocand]>=eg)
                {
		  // cout<<"l1iphi : "<<L1IsoIphi[l1isocand]<<" l1ieta : "<<L1IsoIeta[l1isocand]<<" tpiphi:tpeta "<< iphi_tp<<":"<<ieta_tp<<"  gctphi : "<< getGCTRegionPhi(iphi_tp)<<" gcteta : "<<getGCTRegionEta(ieta_tp)<<endl;
                  //require EG1 by default, EG eg in general                                                                                  
                  if(L1IsoIphi[l1isocand]==getGCTRegionPhi(iphi_tp) && L1IsoIeta[l1isocand]==getGCTRegionEta(ieta_tp))
                    {

                      //geometric match                                                                                                       
                      if(tp>= MaxTPinIsoRegion[l1isocand])
                        {//we have a new maximum                                                                                              
                          MaxTPinIsoRegion[l1isocand]=tp;
                          MaxTPinIsoRegionIeta[l1isocand]=ieta_tp;
                          MaxTPinIsoRegionIphi[l1isocand]=iphi_tp;
                          sevlev_Iso[l1isocand]=severity_level;
			}
                    }
                }
            }


          for (uint l1nonisocand = 0 ; l1nonisocand < nbOfL1NonisoCands ; l1nonisocand++)
            {
              if(L1NonisoRank[l1nonisocand]>=eg)
                {
                  //require EG1 by default, EG eg in general                                                                                  
                  if(L1NonisoIphi[l1nonisocand]==getGCTRegionPhi(iphi_tp) && L1NonisoIeta[l1nonisocand]==getGCTRegionEta(ieta_tp))
                    {
                      //geometric match                                                                                                       
                      if(tp>= MaxTPinNonisoRegion[l1nonisocand])
                        {//we have a new maximum                                                                                              
                          MaxTPinNonisoRegion[l1nonisocand]=tp;
                          MaxTPinNonisoRegionIeta[l1nonisocand]=ieta_tp;
                          MaxTPinNonisoRegionIphi[l1nonisocand]=iphi_tp;
                          sevlev_NonIso[l1nonisocand]=severity_level;
                        }
                    }
                }
            }
	}
   
      for (uint l1isocand = 0 ; l1isocand < nbOfL1IsoCands ; l1isocand++)
        {
   
          if(L1IsoRank[l1isocand]>=eg)
            { 
	      L1IsoCandRank->Fill(L1IsoRank[l1isocand]);
	      if (sevlev_Iso[l1isocand]==3 | sevlev_Iso[l1isocand]==4)
		{
		  L1IsoCandRank_spikes->Fill(L1IsoRank[l1isocand]);
		}
	    }
	}

      for (uint l1nonisocand = 0 ; l1nonisocand < nbOfL1NonisoCands ; l1nonisocand++)
        {
   
          if(L1NonisoRank[l1nonisocand]>=eg)
            { 
	      L1NonisoCandRank->Fill(L1NonisoRank[l1nonisocand]);
	      if (sevlev_NonIso[l1nonisocand]==3 | sevlev_NonIso[l1nonisocand]==4)
		{
		  L1NonisoCandRank_spikes->Fill(L1NonisoRank[l1nonisocand]);
		}
	    }
	}


      int MaxTPinIsoEmulRegion[4];
      int MaxTPinIsoEmulRegionIeta[4];
      int MaxTPinIsoEmulRegionIphi[4];
      int SecondTPinIsoEmulRegion[4];
      int sevlev_IsoEmul[4];

      int MaxTPinNonisoEmulRegion[4];
      int MaxTPinNonisoEmulRegionIeta[4];
      int MaxTPinNonisoEmulRegionIphi[4];
      int SecondTPinNonisoEmulRegion[4];
      int sevlev_NonIsoEmul[4];

      for(int i=0;i<4;++i)
      {
         MaxTPinIsoEmulRegion[i]=0;
         MaxTPinIsoEmulRegionIeta[i]=0;
         MaxTPinIsoEmulRegionIphi[i]=0;
         SecondTPinIsoEmulRegion[i]=0;
	 sevlev_IsoEmul[i]=0;         
        
	 MaxTPinNonisoEmulRegion[i]=0;
         MaxTPinNonisoEmulRegionIeta[i]=0;
         MaxTPinNonisoEmulRegionIphi[i]=0;
         SecondTPinNonisoEmulRegion[i]=0;
	 sevlev_NonIsoEmul[i]=0;         
      }      
   
      for (uint tower = 0 ; tower < nbOfTowers ; tower++)
        {

          //std::cout<<"event kept, in tower loop"<<std::endl;                                                                                
	  int tp_emul = getEt(rawTPEmul3[tower]) ;
          int raw_spike = sFGVB[tower];
          float erec_val=eRec[tower];
          int severity_level=sevlv2[tower];

          //do comparisoEmuln with L1 objects                                                                                                    
	  
	  int ieta_tp_emul = ieta[tower] ;
	  int iphi_tp_emul = iphi[tower] ; 


          for (uint l1isoEmulcand = 0 ; l1isoEmulcand < nbOfL1IsoEmulCands ; l1isoEmulcand++)
            {
              if(L1IsoEmulRank[l1isoEmulcand]>=eg)
                {
                  //require EG1 by default, EG eg in general                                                                                  
                  if(L1IsoEmulIphi[l1isoEmulcand]==getGCTRegionPhi(iphi_tp_emul) && L1IsoEmulIeta[l1isoEmulcand]==getGCTRegionEta(ieta_tp_emul))
                    {
                      //geometric match                                                                                                       
                      if(tp_emul>= MaxTPinIsoEmulRegion[l1isoEmulcand])
                        {//we have a new maximum                                                                                              
                          MaxTPinIsoEmulRegion[l1isoEmulcand]=tp_emul;
                          MaxTPinIsoEmulRegionIeta[l1isoEmulcand]=ieta_tp_emul;
                          MaxTPinIsoEmulRegionIphi[l1isoEmulcand]=iphi_tp_emul;
                          sevlev_IsoEmul[l1isoEmulcand]=severity_level;
                        }
                    }
                }
            }


          for (uint l1nonisoEmulcand = 0 ; l1nonisoEmulcand < nbOfL1NonisoEmulCands ; l1nonisoEmulcand++)
            {
              if(L1NonisoEmulRank[l1nonisoEmulcand]>=eg)
                {
                  //require EG1 by default, EG eg in general                                                                                  
                  if(L1NonisoEmulIphi[l1nonisoEmulcand]==getGCTRegionPhi(iphi_tp_emul) && L1NonisoEmulIeta[l1nonisoEmulcand]==getGCTRegionEta(ieta_tp_emul)\
		     )
                    {
                      //geometric match                                                                                                       
                      if(tp_emul>= MaxTPinNonisoEmulRegion[l1nonisoEmulcand])
                        {//we have a new maximum                                                                                              
                          MaxTPinNonisoEmulRegion[l1nonisoEmulcand]=tp_emul;
                          MaxTPinNonisoEmulRegionIeta[l1nonisoEmulcand]=ieta_tp_emul;
                          MaxTPinNonisoEmulRegionIphi[l1nonisoEmulcand]=iphi_tp_emul;
                          sevlev_NonIsoEmul[l1nonisoEmulcand]=severity_level;
                        }
                    }
                }
            }
	  }
   
      for (uint l1isoEmulcand = 0 ; l1isoEmulcand < nbOfL1IsoEmulCands ; l1isoEmulcand++)
        {
   
          if(L1IsoEmulRank[l1isoEmulcand]>=eg)
            { 
	      L1IsoEmulCandRank->Fill(L1IsoEmulRank[l1isoEmulcand]);
	      if (sevlev_IsoEmul[l1isoEmulcand]==3 | sevlev_IsoEmul[l1isoEmulcand]==4)
		{
		  L1IsoEmulCandRank_spikes->Fill(L1IsoEmulRank[l1isoEmulcand]);
		}
	    }
	}

      for (uint l1nonisoEmulcand = 0 ; l1nonisoEmulcand < nbOfL1NonisoEmulCands ; l1nonisoEmulcand++)
        {
   
          if(L1NonisoEmulRank[l1nonisoEmulcand]>=eg)
            { 
	      L1NonisoEmulCandRank->Fill(L1NonisoEmulRank[l1nonisoEmulcand]);
	      if (sevlev_NonIsoEmul[l1nonisoEmulcand]==3 | sevlev_NonIsoEmul[l1nonisoEmulcand]==4)
		{
		  L1NonisoEmulCandRank_spikes->Fill(L1NonisoEmulRank[l1nonisoEmulcand]);
		}
	    }
	}


      bool eventBool=false;
      // loop on towers
      for (uint tower = 0 ; tower < nbOfTowers ; tower++){
         //std::cout<<"event kept, in tower loop"<<std::endl;
         int tp = getEt(rawTPData[tower]) ;
	 int raw_spike = sFGVB[tower];
	 int sFGVB_bit = sFGVB[tower];
	 int sFGVB_emulbit = rawTPEmulsFGVB3[tower];
          int severity_level=sevlv2[tower];
	 // if (sFGVB_bit==1){
	 //   cout<< "sfgvb :"<<sFGVB_bit<<endl;}
         int emul[5] = {
	   getEt(rawTPEmul1[tower]),  
	   getEt(rawTPEmul2[tower]),
	   getEt(rawTPEmul3[tower]),
	   getEt(rawTPEmul4[tower]),
	   getEt(rawTPEmul5[tower])} ;
	 int emulttFlag[5] = {rawTPEmulttFlag1[tower], 
			      rawTPEmulttFlag2[tower], 
			      rawTPEmulttFlag3[tower], 
			      rawTPEmulttFlag4[tower], 
			      rawTPEmulttFlag5[tower]};
	 int emulsFGVB[5] = {rawTPEmulsFGVB1[tower], 
			     rawTPEmulsFGVB2[tower], 
			     rawTPEmulsFGVB3[tower], 
			     rawTPEmulsFGVB4[tower], 
			     rawTPEmulsFGVB5[tower]};
       
         int maxOfTPEmul = 0 ;
         int indexOfTPEmulMax = -1 ;
         for (int i=0 ; i<5 ; i++) if (emul[i]>maxOfTPEmul)  {
	     maxOfTPEmul = emul[i] ; 
	     indexOfTPEmulMax = i ;
	   }
         int ieta_tp = ieta[tower] ;
         int iphi_tp = iphi[tower] ;
	
	 if (abs(ieta_tp)==27 || abs(ieta_tp) ==28 ) {
	   //cout << __LINE__ << " " <<  tp << endl; 
	   tp = 2*tp; 
	   //cout << __LINE__ << " " <<tp << endl ;
	 }

	 if (ieta_tp >0)
	   hashedIndex = ieta_tp * 100 + iphi_tp ;
	 else 
	   hashedIndex = ieta_tp * 100 - iphi_tp ; 
	 
	 if (abs(ieta_tp) < 18 ) 
	   {
	     if ( raw_spike==0 &&  tp > occupancyCut)  
	       {
		 spikesEB2D ->Fill (iphi_tp, ieta_tp); 
		 spikesEB2DADC->Fill (iphi_tp, ieta_tp, tp ) ; 
	       }
	     if (tp > occupancyCut){
	       if (severity_level == 3 ||severity_level == 4)
		 {
		   TPEB_Spike->Fill(tp);
		   vertices_num_tpEBspike->Fill(nVertices);
		   if (tp>46)
		     {
		       vertices_num_tpEBeg23_spike->Fill(nVertices);
		     }
		   if (tp>60)
		     {
		       vertices_num_tpEBeg30_spike->Fill(nVertices);
		     }	  
		 }
	     }
	   }
	 else if (ieta_tp>=18){
	   if (tp > occupancyCut) {
	     if (raw_spike ==0) {
	       for (uint i=0; i !=geometry[hashedIndex].size();++i){
		 spikesEEPlus2D ->Fill (geometry[hashedIndex][i].first,geometry[hashedIndex][i].second); 
		 spikesEEPlus2DADC->Fill (geometry[hashedIndex][i].first,geometry[hashedIndex][i].second, tp ) ; 
	       }
	     }
	   }
	 }else if (ieta_tp <= -18) {
	   if (tp > occupancyCut) {
	     if (raw_spike ==0 ) {
	       for (uint i=0; i !=geometry[hashedIndex].size();++i){
		 spikesEEMinus2D ->Fill (geometry[hashedIndex][i].first,geometry[hashedIndex][i].second); 
		 spikesEEMinus2DADC->Fill (geometry[hashedIndex][i].first,geometry[hashedIndex][i].second, tp) ; 
	       }
	     }
	   }
	 }


         //apply hashedIndex filter
         int hashedIndex =0 ;
         if (ieta_tp >0)
            hashedIndex = ieta_tp * 100 + iphi_tp ;
         else 
            hashedIndex = ieta_tp * 100 - iphi_tp ;
         
	    uint nbXtals = nbOfXtals[tower] ;
	    int ttf = getTtf(rawTPData[tower]) ;

	    if (verbose>9 && (tp>0 || maxOfTPEmul>0)) {
	      std::cout<<"(phi,eta, Nbxtals)="<<std::dec<<iphi_tp<<" "<<ieta_tp<<" "<<nbXtals<<std::endl ;
	      std::cout<<"Data Et, TTF: "<<tp<<" "<<ttf<<std::endl ;
	      std::cout<<"Emulator: " ;
	      for (int i=0 ; i<5 ; i++) std::cout<<emul[i]<<" " ;
	      std::cout<<std::endl ;
	    }
      
     	    //////////////////////
	    // Fill TP spectra ///
	    //////////////////////
	    
	    if (tp >occupancyCut){
	      ratio_vs_ieta->Fill(ieta_tp, tp/(2.*eRec[tower]));
	      if (ttFlag[tower]==3){
		if (abs(ieta_tp) > 18)ratio_vs_ieta_noSpikes -> Fill(ieta_tp, tp/(2.*eRec[tower]));
		if(abs(ieta_tp) < 18 && raw_spike!=0) ratio_vs_ieta_noSpikes->Fill(ieta_tp, tp/(2.*eRec[tower]));
	      }
	      if (tp > 24 && 2.*eRec[tower]>24 && ttFlag[tower]==3 && (abs(ieta_tp)>18 || raw_spike!=0)) {
		ratio_vs_ieta_24ADC->Fill(ieta_tp, tp/(2.*eRec[tower]));
		if (abs(ieta_tp) == 27 || abs(ieta_tp) ==28){
		  ratio_ieta_27_28->Fill(tp/(2.*eRec[tower]));
		}
		if (tp ==255) ratio_tp255->Fill(tp/(2.*eRec[tower]));
		if (abs(ieta_tp) != 27 && abs(ieta_tp) !=28 && tp !=255){
		  ratio_vs_ieta_cleaned -> Fill(ieta_tp, tp/(2.*eRec[tower]));
		  if (abs((tp-2.*eRec[tower])/(2.*eRec[tower]))>0.3){

		  }
		}
	      }
	    }



	    if (tp>occupancyCut){
	      TPEB_vs_ieta->Fill(ieta_tp, tp) ;
	      if ((fabs(ieta_tp)<18 && raw_spike!=0 ) || fabs(ieta_tp)>18) TPEB_vs_ieta_noSpike->Fill(ieta_tp, tp) ;
	      if (sevlv[tower]==0) TPEB_vs_ieta_sevlv0->Fill(ieta_tp, tp);
	    }
	    
	    //by num_vertices(PU):tp_eta,tp_spectrum
	          //num_events counter by number of vertices

	    if (tp>occupancyCut)
	      {
		if(nVertices<9)
		  {
		    num_events_Vrange1+=1;
		    numTP_ieta_Vrange1->Fill(ieta_tp);
		    tpSpectrum_Vrange1->Fill(tp);
		  }
		else if(9<=nVertices && nVertices<17)
		  {
		    num_events_Vrange2+=1;
		    numTP_ieta_Vrange2->Fill(ieta_tp);
		    tpSpectrum_Vrange2->Fill(tp);
		    
		  }
		else if(17<=nVertices && nVertices<25)
		  {
		    num_events_Vrange3+=1;
		    numTP_ieta_Vrange3->Fill(ieta_tp);
		    tpSpectrum_Vrange3->Fill(tp);
		  }
		else if(nVertices>=25)
		  {
		    num_events_Vrange4+=1;
		    numTP_ieta_Vrange4->Fill(ieta_tp);
		    tpSpectrum_Vrange4->Fill(tp);
		  }
	      }

	    // HISTOS FOR REJECTION EFFICIENCY AND RESIDUAL CONTAMINATION CALCULATION BY PU BINS
	    
	    if (abs(ieta_tp)<18)
	      {
		if (tp>occupancyCut)// && emul[ref]>occupancyCut)
		  {
		    for (int i=1;i<num_pu_bins;i++)
		      {
			if(nVertices>pu_binning[i-1] && nVertices<pu_binning[i])
			  {
			    events_by_bin[i-1]+=1;
			    ALL_TP[i-1]->Fill(tp);
			    if (sFGVB_bit==0)
			      {
				TP_sFGVB0[i-1]->Fill(tp);
			      }	    	    
			    if (severity_level==3 || severity_level==4)
			      {
				TP_sevlv3_4[i-1]->Fill(tp);
			      }
			  }
		      }
		  }
		
		if (emul[ref]>occupancyCut && tp>occupancyCut)
		  {
		    for (int i=1;i<num_pu_bins;i++)
		      {
			if(nVertices>pu_binning[i-1] && nVertices<pu_binning[i])
			  {
			    events_by_bin[i-1]+=1;
			    ALL_TPEMUL[i-1]->Fill(emul[ref]);
			    if (sFGVB_bit==0)
			      {
				TPEMUL_sFGVB0[i-1]->Fill(emul[ref]);
							      }	    	    
			    if (severity_level==3 || severity_level==4)
			      {
				TPEMUL_sevlv3_4[i-1]->Fill(emul[ref]);
			      }
			  }
		      }
		  }
	      }
	    if (abs(ieta_tp)<18)
	      {
		//barrel
		if (tp>occupancyCut){
		  vertices_num_tpEB->Fill(nVertices);
		  TPEB->Fill(tp) ;
		  
		  if (tp>46)
		     {
		       vertices_num_tpEBeg23_->Fill(nVertices);
		     }
		   if (tp>60)
		     {
		       vertices_num_tpEBeg30_->Fill(nVertices);
		     }	
		  TP_5x5Energy_ratio_spikesIn_EB->Fill(tp/(2.*eRec[tower])) ;
		  TP_vs_5x5Energy_spikesIn_EB->Fill((2.*eRec[tower]),tp) ;
		  if (tp > 24 ){
		    TP_5x5Energy_ratio_spikesIn_24_EB->Fill(tp/(2.*eRec[tower])) ;
		    TP_vs_5x5Energy_spikesIn_24_EB->Fill((2.*eRec[tower]),tp) ;
		    //cout << __LINE__ << " " << tp << " " << ttFlag[tower] << " " << sevlv[tower] << " " << raw_spike << endl; 
		  }
		  totTPEnergy_EB+=tp;
		  if (ttFlag[tower]==3){
		    TPEB_fullReadout_spikesIn->Fill(tp) ; 
		    TP_5x5Energy_ratio_spikesIn_fullReadout_EB->Fill(tp/(2.*eRec[tower])) ; 
		    TP_vs_5x5Energy_spikesIn_fullReadout_EB->Fill((2.*eRec[tower]),tp) ; 
		    if (tp > 24){
		      TP_5x5Energy_ratio_spikesIn_fullReadout_24_EB->Fill(tp/(2.*eRec[tower])) ; 
		      TP_vs_5x5Energy_spikesIn_fullReadout_24_EB->Fill((2.*eRec[tower]),tp) ; 
		    }
		  }
		  if (sFGVB_bit==0)
		    {
		      TPEB_sFGVB0->Fill(tp) ;
		    }

		  if (sevlv[tower]==0) {
		    TPEB_sevlv0->Fill(tp) ; 
		    TP_5x5Energy_ratio_sevlv0_EB->Fill(tp/(2*eRec[tower])) ; 
		    TP_vs_5x5Energy_sevlv0_EB->Fill((2*eRec[tower]),tp) ; 
		    if(tp>24){
		      TP_5x5Energy_ratio_sevlv0_24_EB->Fill(tp/(2*eRec[tower])) ; 
		      TP_vs_5x5Energy_sevlv0_24_EB->Fill((2*eRec[tower]),tp) ; 
		    }
		    if (ttFlag[tower]==3){
		      TPEB_fullReadout_sevlv0->Fill(tp) ; 
		      TP_5x5Energy_ratio_fullReadout_sevlv0_EB->Fill(tp/(2.*eRec[tower])) ; 
		      TP_vs_5x5Energy_fullReadout_sevlv0_EB->Fill((2.*eRec[tower]),tp) ; 
		      if (tp>24){
			TP_5x5Energy_ratio_fullReadout_sevlv0_24_EB->Fill(tp/(2.*eRec[tower])) ; 
			TP_vs_5x5Energy_fullReadout_sevlv0_24_EB->Fill((2.*eRec[tower]),tp) ; 
		      }
		    }
		  }
		  //Fill TPEB spectrum only with Spike (sevlv3/4);
		  //		  cout << "raw_spike = " << raw_spike <<  " : TP = " << tp  << endl;
		  //		  if (raw_spike == 3 || raw_spike == 4){TPEB_Spike->Fill(tp);}
		  if (raw_spike!=0) {
		    TPEB_noSpike->Fill(tp) ; 
		    TP_5x5Energy_ratio_EB->Fill(tp/(2*eRec[tower])) ; 
		    TP_vs_5x5Energy_EB->Fill((2.*eRec[tower]),tp) ; 
		    if (tp>24){
		      TP_5x5Energy_ratio_24_EB->Fill(tp/(2*eRec[tower])) ; 
		      TP_vs_5x5Energy_24_EB->Fill((2.*eRec[tower]),tp) ; 
		    }
		    if (ttFlag[tower]==3){
		      TPEB_fullReadout->Fill(tp) ; 
		      TP_5x5Energy_ratio_fullReadout_EB->Fill(tp/(2.*eRec[tower])) ; 
		      TP_vs_5x5Energy_fullReadout_EB->Fill((2.*eRec[tower]),tp) ; 
		      if(tp>24){
			TP_5x5Energy_ratio_fullReadout_24_EB->Fill(tp/(2.*eRec[tower])) ; 
			TP_vs_5x5Energy_fullReadout_24_EB->Fill((2.*eRec[tower]),tp) ; 
		      }
		    }
		  }
		}
		if (tp>occupancyCut) TPspectrumMap3DEB->Fill(iphi_tp, ieta_tp, tp) ;
		if (emul[ref]>occupancyCut){
		  TPEmulEB->Fill(emul[ref]) ;		  
		  //		  if (tp>occupancyCut)
		  // {
		      if (sFGVB_emulbit==0) 
			{
			  TPEmulEB_sFGVB0->Fill(emul[ref]) ;
			}
		      vertices_num_tpEBemul->Fill(nVertices);
		      if (tp>46)
			{
			  vertices_num_tpEBeg23_emul->Fill(nVertices);
			}
		      if (tp>60)
			{
			  vertices_num_tpEBeg30_emul->Fill(nVertices);
			}	
		      TPEmulEB_realTP->Fill(emul[ref]+20); 
		      if (sevlv[tower] == 3 || sevlv[tower] == 4)
			{ 
			  TPEmulEB_realTP_Spike2->Fill(emul[ref]);
			}
		      if (severity_level == 3 || severity_level == 4)
			{ 
			  TPEmulEB_realTP_Spike->Fill(emul[ref]);
			  vertices_num_tpEBemul_spike->Fill(nVertices);
			  if (tp>46)
			    {
			      vertices_num_tpEBeg23_emul_spike->Fill(nVertices);
			    }
			  if (tp>60)
			{
			  vertices_num_tpEBeg30_emul_spike->Fill(nVertices);
			}	
		      
			}
		      //  }
		}
		if (maxOfTPEmul>occupancyCut){
		  TPEmulMaxEB->Fill(maxOfTPEmul) ;
		  if (tp>0) TPEmulMaxEB_realTP->Fill(maxOfTPEmul) ;
		}
	      }
      
	    if (geometry.size()>0)
	      {
         
		if (ieta_tp>=18)
		  {
		    // EE+
		    if (tp>occupancyCut) {
		      TPEEPlus->Fill(tp) ;
		      totTPEnergy_EEP+=tp;
		      vertices_num_tpEEPlus->Fill(nVertices);
		      TP_5x5Energy_ratio_EEPlus->Fill(tp/(2*eRec[tower])) ; 
		      TP_vs_5x5Energy_EEPlus->Fill((2*eRec[tower]),tp) ; 
		      if (ttFlag[tower]==3){
			TPEEPlus_fullReadout->Fill(tp) ; 
			TP_5x5Energy_ratio_fullReadout_EEPlus->Fill(tp/(2.*eRec[tower])) ; 
			TP_vs_5x5Energy_fullReadout_EEPlus->Fill((2.*eRec[tower]),tp) ; 
		      }
		    }
		    if (emul[ref]>occupancyCut) {
		      TPEmulEEPlus->Fill(emul[ref]) ;
		      vertices_num_tpEEPlusemul->Fill(nVertices);
		      if (tp>0) {
			TPEmulEEPlus_realTP->Fill(emul[ref]) ;
		
		      }
		    }
		    if (maxOfTPEmul>occupancyCut) {
		      TPEmulMaxEEPlus->Fill(maxOfTPEmul) ;
		      if (tp>0) TPEmulMaxEEPlus_realTP->Fill(maxOfTPEmul) ;
		    }
		    if (tp>occupancyCut)
		      {
			for (uint i=0; i !=geometry[hashedIndex].size();++i)
			  TPspectrumMap3DEEPlus->Fill(geometry[hashedIndex][i].first,geometry[hashedIndex][i].second,tp) ;
		      }

		  }
         
		if (ieta_tp<=-18)
		  {
		    //EE-
		    if (tp>occupancyCut) {
		      TPEEMinus->Fill(tp) ;
		      vertices_num_tpEEMinus->Fill(nVertices);
		      totTPEnergy_EEM+=tp;
		      TP_5x5Energy_ratio_EEMinus->Fill(tp/(2.*eRec[tower])) ; 
		      TP_vs_5x5Energy_EEMinus->Fill((2.*eRec[tower]),tp) ; 

		      if (ttFlag[tower]==3){
			TPEEMinus_fullReadout->Fill(tp) ; 
			TP_5x5Energy_ratio_fullReadout_EEMinus->Fill(tp/(2.*eRec[tower])) ; 
			TP_vs_5x5Energy_fullReadout_EEMinus->Fill((2.*eRec[tower]),tp) ; 
		      }
		    }
		    if (emul[ref]>occupancyCut) {
		      vertices_num_tpEEMinusemul->Fill(nVertices);
		      TPEmulEEMinus->Fill(emul[ref]) ;
		      if (tp>0) {
			TPEmulEEMinus_realTP->Fill(emul[ref]) ;
		
		      }
		    }
		    if (maxOfTPEmul>occupancyCut) {
		      TPEmulMaxEEMinus->Fill(maxOfTPEmul) ;
		      if (tp>0) TPEmulMaxEEMinus_realTP->Fill(maxOfTPEmul) ;
		    }
		    if (tp>occupancyCut){   
		      for (uint i=0; i !=geometry[hashedIndex].size();++i)
			TPspectrumMap3DEEMinus->Fill(geometry[hashedIndex][i].first,geometry[hashedIndex][i].second,tp) ;
		    }
	    		    
		  }
	      }	  
	    
      }//loop over trigger tower     

      //      cout << "========= End of event loop =========" << endl;

   }

   
   cout<<"Printing residual contamination info by pileup bins: "<<endl;
   cout<<endl;
   cout<<"Online TP Spectrum | Default working point |12-12"<<endl;
   cout<<endl;
   cout<<"PU_Bin_Range       "<<"Entire_Spectrum                "<<"Above_23_GeV                 "<<"Above_30_GeV"<<endl;
   for (int i=1;i<num_pu_bins;i++)
     {
       cout<<pu_binning[i-1]<<"-"<<pu_binning[i]<<"             "<<(TP_sevlv3_4[i-1])->Integral()/(ALL_TP[i-1])->Integral()<<" +/- "<<getEffErr((TP_sevlv3_4[i-1])->Integral(),(ALL_TP[i-1])->Integral())<<"         "<<(TP_sevlv3_4[i-1])->Integral(46,256)/(ALL_TP[i-1])->Integral(46,256)<<" +/- "<<getEffErr((TP_sevlv3_4[i-1])->Integral(46,256),(ALL_TP[i-1])->Integral(46,256))<<"           "<<(TP_sevlv3_4[i-1])->Integral(61,256)/(ALL_TP[i-1])->Integral(61,256)<<" +/- "<< getEffErr((TP_sevlv3_4[i-1])->Integral(61,256),(ALL_TP[i-1])->Integral(61,256))<<"    "<<endl;}  
   cout<<endl;
   cout<<"Emulated TP Spectrum | New working point |18-22"<<endl;
   cout<<endl;
   cout<<"PU_Bin_Range       "<<"Entire_Spectrum                "<<"Above_23_GeV                 "<<"Above_30_GeV"<<endl;
   for (int i=1;i<num_pu_bins;i++)
     {
       cout<<pu_binning[i-1]<<"-"<<pu_binning[i]<<"             "<<(TPEMUL_sevlv3_4[i-1])->Integral()/(ALL_TPEMUL[i-1])->Integral()<<" +/- "<<getEffErr((TPEMUL_sevlv3_4[i-1])->Integral(),(ALL_TPEMUL[i-1])->Integral())<<"         "<<(TPEMUL_sevlv3_4[i-1])->Integral(46,256)/(ALL_TPEMUL[i-1])->Integral(46,256)<<" +/- "<<getEffErr((TPEMUL_sevlv3_4[i-1])->Integral(46,256),(ALL_TPEMUL[i-1])->Integral(46,256))<<"           "<<(TPEMUL_sevlv3_4[i-1])->Integral(61,256)/(ALL_TPEMUL[i-1])->Integral(61,256)<<" +/- "<< getEffErr((TPEMUL_sevlv3_4[i-1])->Integral(61,256),(ALL_TPEMUL[i-1])->Integral(61,256))<<"    "<<endl;
     } 
   int thresholds[]={1,46,61};

   cout<<endl;
   cout<<endl;
   cout<<"Printing spike rejection efficiency info by pileup bins: "<<endl;
   cout<<endl;
   cout<<"PU_Bin_Range       "<<"Entire_Spectrum                "<<"Above_23_GeV                 "<<"Above_30_GeV"<<endl;
   for (int i=1;i<num_pu_bins;i++){
     cout<<pu_binning[i-1]<<"-"<<pu_binning[i];     
     for (int thres=0;thres<3;thres++)
       {
	 cout<<"             "<<(TPEMUL_sFGVB0[i-1])->Integral(thresholds[thres],256)/(TPEMUL_sevlv3_4[i-1])->Integral(thresholds[thres],256)<<" +/- "<<getEffErr((TPEMUL_sFGVB0[i-1])->Integral(thresholds[thres],256),(TPEMUL_sevlv3_4[i-1])->Integral(thresholds[thres],256));
     	    }
     cout<<endl;
   }
   

   TCanvas *c15 = new TCanvas("dataTPVSemulTP", "TP Spikes", 800, 600);
   c15->cd();
   c15->SetLogy();
   c15->SetTicks();
   TPEB->Draw("hist");
   TPEB->SetYTitle("#bf{Number of TP}"); 
   TPEB->SetXTitle("#bf{TP (ADC)}"); 
   TPEmulEB_realTP->SetLineColor(4);
   TPEmulEB_realTP->Draw("same hist");

   TLatex *   tex0 = new TLatex(0.13,0.88,"#bf{CMS Preliminary}");
   tex0->SetNDC();
   tex0->SetTextAlign(13);
   tex0->SetTextFont(42);
   tex0->SetTextSize(0.04);
   tex0->SetLineWidth(2);
   tex0->Draw();                                                                                                                                                                                                          
   TLatex *   tex01 = new TLatex(0.5,0.43,"sFGVB_{th} = 18 GeV");
   tex01->SetNDC();
   tex01->SetTextAlign(13);
   tex01->SetTextFont(42);
   tex01->SetTextSize(0.03);
   tex01->SetLineWidth(2);
   tex01->Draw();                                                                                                                                                                                          

   TLatex *   tex02 = new TLatex(0.5,0.38,"Spike_{th}   = 22 GeV");
   tex02->SetNDC();
   tex02->SetTextAlign(13);
   tex02->SetTextFont(42);
   tex02->SetTextSize(0.03);
   tex02->SetLineWidth(2);
   tex02->Draw();                                                                                                                                                                                          
   

   TLatex *   tex2 = new TLatex(0.56,0.98,campaign+" ZeroBias Run "+ runnum);

   tex2->SetNDC();
   tex2->SetTextAlign(13);
   tex2->SetTextFont(42);
   tex2->SetTextSize(0.04);
   tex2->SetLineWidth(2);
   tex2->Draw();                                                                                                                                                                                              
   TLegend *leg0 = new TLegend(0.42,0.65,0.80,0.88,"", "brNDC");
   leg0->SetTextFont(62); //22, 62
   leg0->SetTextSize(0.032); // 0.03, 0.048
   leg0->SetLineColor(1);
   leg0->SetLineStyle(1);
   leg0->SetLineWidth(1);
   leg0->SetFillStyle(1001);
   leg0->SetFillColor(10);
   leg0->AddEntry(TPEB,"#bf{All dataTP in EB}","l");
   leg0->AddEntry(TPEmulEB_realTP,"#bf{All emulTP in EB}","l");
   leg0->Draw();
   c15->Print("plots/"+runnum+"-TPSpectrumComparison"+sfgvb+"_"+etkill+".png");

   // TCanvas *c16 = new TCanvas("TPEB_Spikes", "TP Spikes", 800, 600);
   // c16->cd();
   // TPEB->Draw("hist");
   // TPEB_Spike->SetLineColor(4);
   // TPEB_Spike->Draw("hist same");
 
   TCanvas *c9628 = new TCanvas("numTP_ieta_Vrange1_Spikes", "TP Spikes", 800, 600);
   c9628->cd();
   c9628->SetTicks();
   c9628->SetLogy();
   numTP_ieta_Vrange1->Draw("hist");
   numTP_ieta_Vrange1->SetYTitle("#bf{Number of TP}");
   numTP_ieta_Vrange1->GetXaxis()->SetLabelFont(42);
   numTP_ieta_Vrange1->GetYaxis()->SetLabelFont(42);
   numTP_ieta_Vrange1->SetLineColor(1);
   numTP_ieta_Vrange2->SetLineColor(kRed);
   numTP_ieta_Vrange2->Draw("hist same");
   numTP_ieta_Vrange3->SetLineColor(kGreen);
   numTP_ieta_Vrange3->Draw("hist same");
   numTP_ieta_Vrange4->SetLineColor(kBlue);
   numTP_ieta_Vrange4->Draw("hist same");

   TLegend *leg00 = new TLegend(0.42,0.65,0.80,0.88,"", "brNDC");
   leg00->SetTextFont(62); //22, 62
   leg00->SetTextSize(0.032); // 0.03, 0.048
   leg00->SetLineColor(1);
   leg00->SetLineStyle(1);
   leg00->SetLineWidth(1);
   leg00->SetFillStyle(1001);
   leg00->SetFillColor(10);
   leg00->AddEntry(numTP_ieta_Vrange1,"#bf{0-8 vertices}","l");
   leg00->AddEntry(numTP_ieta_Vrange2,"#bf{9-17 vertices}","l");
   leg00->AddEntry(numTP_ieta_Vrange3,"#bf{18-24 vertices}","l");
   leg00->AddEntry(numTP_ieta_Vrange4,"#bf{25 or more vertices}","l");

   leg00->Draw();

   //   numTP_ieta_Vrange2->SetFillColor(kYellow-10);  
   c9628->Print("plots/"+runnum+"numtp_ieta_vertranges.png");

   TCanvas *c9728 = new TCanvas("numTP_ieta_Vrange1_Spikes", "TP Spikes", 800, 600);
   c9728->cd();
   c9728->SetTicks();
   c9728->SetLogy();
   tpSpectrum_Vrange1->Draw("hist");
   tpSpectrum_Vrange1->SetYTitle("#bf{Number of TP}");
   tpSpectrum_Vrange1->GetXaxis()->SetLabelFont(42);
   tpSpectrum_Vrange1->GetYaxis()->SetLabelFont(42);
   tpSpectrum_Vrange1->SetLineColor(1);
   tpSpectrum_Vrange2->SetLineColor(kRed);
   tpSpectrum_Vrange2->Draw("hist same");
   tpSpectrum_Vrange3->SetLineColor(kGreen);
   tpSpectrum_Vrange3->Draw("hist same");
   tpSpectrum_Vrange4->SetLineColor(kBlue);
   tpSpectrum_Vrange4->Draw("hist same");
 
   TLegend *leg000 = new TLegend(0.42,0.65,0.80,0.88,"", "brNDC");
   leg000->SetTextFont(62); //22, 62
   leg000->SetTextSize(0.032); // 0.03, 0.048
   leg000->SetLineColor(1);
   leg000->SetLineStyle(1);
   leg000->SetLineWidth(1);
   leg000->SetFillStyle(1001);
   leg000->SetFillColor(10);
   leg000->AddEntry(tpSpectrum_Vrange1,"#bf{0-8 vertices}","l");
   leg000->AddEntry(tpSpectrum_Vrange2,"#bf{9-17 vertices}","l");
   leg000->AddEntry(tpSpectrum_Vrange3,"#bf{18-24 vertices}","l");
   leg000->AddEntry(tpSpectrum_Vrange4,"#bf{25 or more vertices}","l");

   leg000->Draw();


   //   numTP_ieta_Vrange2->SetFillColor(kYellow-10);  
   c9728->Print("plots/"+runnum+"tpSpectrum_vertranges.png");

   //normalize by number of events by Num_vertices range

   TCanvas *c9629 = new TCanvas("numTP_ieta_Vrange1_Spikes", "TP Spikes", 800, 600);
   c9629->cd();
   c9629->SetTicks();
   c9629->SetLogy();
   numTP_ieta_Vrange1->Rebin(2);
   numTP_ieta_Vrange1->Scale(1./num_events_Vrange1);
   numTP_ieta_Vrange1->Draw("");
   numTP_ieta_Vrange1->SetYTitle("#bf{Number of TP}");
   numTP_ieta_Vrange1->GetXaxis()->SetLabelFont(42);
   numTP_ieta_Vrange1->GetYaxis()->SetLabelFont(42);
   numTP_ieta_Vrange1->SetLineColor(1);

   numTP_ieta_Vrange2->Rebin(2);
   numTP_ieta_Vrange2->Scale(1./num_events_Vrange2);
   numTP_ieta_Vrange2->SetLineColor(kRed);
   numTP_ieta_Vrange2->Draw("same");

   numTP_ieta_Vrange3->Rebin(2);
   numTP_ieta_Vrange3->Scale(1./num_events_Vrange3);
   numTP_ieta_Vrange3->SetLineColor(kGreen);
   numTP_ieta_Vrange3->Draw("same");

   numTP_ieta_Vrange4->Rebin(2);
   numTP_ieta_Vrange4->Scale(1./num_events_Vrange4);
   numTP_ieta_Vrange4->SetLineColor(kBlue);
   numTP_ieta_Vrange4->Draw("same");

   TLegend *leg01 = new TLegend(0.42,0.65,0.80,0.88,"", "brNDC");
   leg01->SetTextFont(62); //22, 62
   leg01->SetTextSize(0.032); // 0.03, 0.048
   leg01->SetLineColor(1);
   leg01->SetLineStyle(1);
   leg01->SetLineWidth(1);
   leg01->SetFillStyle(1001);
   leg01->SetFillColor(10);
   leg01->AddEntry(numTP_ieta_Vrange1,"#bf{0-8 vertices}","l");
   leg01->AddEntry(numTP_ieta_Vrange2,"#bf{9-17 vertices}","l");
   leg01->AddEntry(numTP_ieta_Vrange3,"#bf{18-24 vertices}","l");
   leg01->AddEntry(numTP_ieta_Vrange4,"#bf{25 or more vertices}","l");

   leg01->Draw();

   //   numTP_ieta_Vrange2->SetFillColor(kYellow-10);  
   c9629->Print("plots/"+runnum+"numtp_ieta_vertranges_normalizedbyevents.png");

   TCanvas *c9729 = new TCanvas("numTP_ieta_Vrange1_Spikes", "TP Spikes", 800, 600);
   c9729->cd();
   c9729->SetTicks();
   c9729->SetLogy();
  
   tpSpectrum_Vrange1->Rebin(2);
   tpSpectrum_Vrange1->Scale(1./num_events_Vrange1);
   tpSpectrum_Vrange1->Draw("");
   tpSpectrum_Vrange1->SetYTitle("#bf{Number of TP}");
   tpSpectrum_Vrange1->GetXaxis()->SetLabelFont(42);
   tpSpectrum_Vrange1->GetYaxis()->SetLabelFont(42);
   tpSpectrum_Vrange1->SetLineColor(1);

   tpSpectrum_Vrange2->Rebin(2);
   tpSpectrum_Vrange2->Scale(1./num_events_Vrange2);
   tpSpectrum_Vrange2->SetLineColor(kRed);
   tpSpectrum_Vrange2->Draw("same");

   tpSpectrum_Vrange3->Rebin(2);
   tpSpectrum_Vrange3->Scale(1./num_events_Vrange3);
   tpSpectrum_Vrange3->SetLineColor(kGreen);
   tpSpectrum_Vrange3->Draw("same");

   tpSpectrum_Vrange4->Rebin(2);
   tpSpectrum_Vrange4->Scale(1./num_events_Vrange4);
   tpSpectrum_Vrange4->SetLineColor(kBlue);
   tpSpectrum_Vrange4->Draw("same");
 
   TLegend *leg010 = new TLegend(0.42,0.65,0.80,0.88,"", "brNDC");
   leg010->SetTextFont(62); //22, 62
   leg010->SetTextSize(0.032); // 0.03, 0.048
   leg010->SetLineColor(1);
   leg010->SetLineStyle(1);
   leg010->SetLineWidth(1);
   leg010->SetFillStyle(1001);
   leg010->SetFillColor(10);
   leg010->AddEntry(tpSpectrum_Vrange1,"#bf{0-8 vertices}","l");
   leg010->AddEntry(tpSpectrum_Vrange2,"#bf{9-17 vertices}","l");
   leg010->AddEntry(tpSpectrum_Vrange3,"#bf{18-24 vertices}","l");
   leg010->AddEntry(tpSpectrum_Vrange4,"#bf{25 or more vertices}","l");

   leg010->Draw();


   //   numTP_ieta_Vrange2->SetFillColor(kYellow-10);  
   c9729->Print("plots/"+runnum+"tpSpectrum_vertranges_normalizedbyevents.png");



   TCanvas *c1777 = new TCanvas("vertices_tpEB_Spikes", "TP Spikes", 800, 600);
   c1777->cd();
   c1777->SetTicks();
   c1777->SetLogy();
   vertices_num_tpEB->Draw("hist");
   vertices_num_tpEB->SetYTitle("#bf{Number of TP}");
   vertices_num_tpEB->SetXTitle("#bf{Number of Vertices}");
   vertices_num_tpEB->GetXaxis()->SetLabelFont(42);
   vertices_num_tpEB->GetYaxis()->SetLabelFont(42);
   vertices_num_tpEB->SetLineColor(kBlack);
   vertices_num_tpEBspike->SetLineColor(kBlue);
   vertices_num_tpEBspike->SetFillColor(kYellow-10);
   vertices_num_tpEBspike->Draw("hist same");

   TLegend *leg0000 = new TLegend(0.42,0.65,0.80,0.88,"", "brNDC");
   leg0000->SetTextFont(62); //22, 62
   leg0000->SetTextSize(0.032); // 0.03, 0.048
   leg0000->SetLineColor(1);
   leg0000->SetLineStyle(1);
   leg0000->SetLineWidth(1);
   leg0000->SetFillStyle(1001);
   leg0000->SetFillColor(10);
   leg0000->AddEntry(vertices_num_tpEB,"#bf{All TP}","l");
   leg0000->AddEntry(vertices_num_tpEBspike,"#bf{Spike induced TP}","l");
   leg0000->Draw();
   c1777->Print("plots/"+runnum+"vertices_realtp.png");



   TCanvas *c1888 = new TCanvas("vertices_num_tpEB_Spikes", "TP Spikes", 800, 600);
   c1888->cd();
   c1888->SetTicks();
   c1888->SetLogy();
   vertices_num_tpEBemul->Draw("hist");
   vertices_num_tpEBemul->SetYTitle("#bf{Number of TP}");
   vertices_num_tpEBemul->SetXTitle("#bf{Number of Vertices}");
   vertices_num_tpEBemul->GetXaxis()->SetLabelFont(42);
   vertices_num_tpEBemul->GetYaxis()->SetLabelFont(42);
   vertices_num_tpEBemul->SetLineColor(1);
   vertices_num_tpEBemul_spike->SetLineColor(kBlue);
   vertices_num_tpEBemul_spike->SetFillColor(kYellow-10);
   vertices_num_tpEBemul_spike->Draw("hist same");

   TLegend *leg00000 = new TLegend(0.42,0.65,0.80,0.88,"", "brNDC");
   leg00000->SetTextFont(62); //22, 62
   leg00000->SetTextSize(0.032); // 0.03, 0.048
   leg00000->SetLineColor(1);
   leg00000->SetLineStyle(1);
   leg00000->SetLineWidth(1);
   leg00000->SetFillStyle(1001);
   leg00000->SetFillColor(10);
   leg00000->AddEntry(vertices_num_tpEBemul,"#bf{All TP}","l");
   leg00000->AddEntry(vertices_num_tpEBemul_spike,"#bf{Spike induced TP}","l");
   leg00000->Draw();
   c1888->Print("plots/"+runnum+"vertices_emultp.png");



   TCanvas *c1889 = new TCanvas("vertices_num_tpEB_Spikes", "TP Spikes", 800, 600);
   c1889->cd();

   vertices_num_tpEB->GetYaxis()->SetTitle("Fraction of Spikes") ;
   vertices_num_tpEBspike->GetYaxis()->SetTitle("Fraction of Spikes") ;

   vertices_num_tpEB->Rebin(2);
   vertices_num_tpEBspike->Rebin(2);


   TEfficiency* pEff = 0;

   if(TEfficiency::CheckConsistency(*vertices_num_tpEBspike,*vertices_num_tpEB))
     {
       pEff = new TEfficiency(*vertices_num_tpEBspike,*vertices_num_tpEB);
       pEff->SetMarkerStyle(3);
       pEff->SetLineColor(kBlue);
       pEff->Draw();
     }
   gPad->Update();
   pEff->GetPaintedGraph()->GetYaxis()->SetRangeUser(0.0,1.0);

   
   vertices_num_tpEBeg23_->Rebin(2);
   vertices_num_tpEBeg23_spike->Rebin(2);

   TEfficiency* pEff_eg23 = 0;

   if(TEfficiency::CheckConsistency(*vertices_num_tpEBeg23_spike,*vertices_num_tpEBeg23_))
     {
       pEff_eg23 = new TEfficiency(*vertices_num_tpEBeg23_spike,*vertices_num_tpEBeg23_);
       pEff_eg23->SetMarkerStyle(3);
       pEff_eg23->SetLineColor(kBlack);
       pEff_eg23->Draw("same");
     }
   gPad->Update();
   pEff_eg23->GetPaintedGraph()->GetYaxis()->SetRangeUser(0.0,1.0);



   vertices_num_tpEBeg30_->Rebin(2);
   vertices_num_tpEBeg30_spike->Rebin(2);

   TEfficiency* pEff_eg30 = 0;

   if(TEfficiency::CheckConsistency(*vertices_num_tpEBeg30_spike,*vertices_num_tpEBeg30_))
     {
       pEff_eg30 = new TEfficiency(*vertices_num_tpEBeg30_spike,*vertices_num_tpEBeg30_);
       pEff_eg30->SetLineColor(kRed);
       //       pEff_eg30->Draw("same");
     }
   TLegend *leg9001 = new TLegend(0.52,0.65,0.92,0.88,"", "brNDC");
   leg9001->SetTextFont(62); //22, 62
   leg9001->SetTextSize(0.032); // 0.03, 0.048
   leg9001->SetLineColor(1);
   leg9001->SetLineStyle(1);
   leg9001->SetLineWidth(1);
   leg9001->SetFillStyle(1001);
   leg9001->SetFillColor(10);
   leg9001->AddEntry(pEff,"#bf{ALL TP}","l");
   leg9001->AddEntry(pEff_eg23,"#bf{Above 20 GeV}","l");
   leg9001->AddEntry(pEff_eg30,"#bf{Above 30 GeV}","l");

   leg9001->Draw();

   c1889->Print("plots/"+runnum+"Fraction_spikes_vertices.png");


   TCanvas *c1899 = new TCanvas("vertices_num_tpEBemul_Spikes", "TP Spikes", 800, 600);
   c1899->cd();

   vertices_num_tpEBemul->GetYaxis()->SetTitle("Fraction of Spikes") ;
   vertices_num_tpEBemul_spike->GetYaxis()->SetTitle("Fraction of Spikes") ;

   vertices_num_tpEBemul->Rebin(2);
   vertices_num_tpEBemul_spike->Rebin(2);

   TEfficiency* pEff2 = 0;

   if(TEfficiency::CheckConsistency(*vertices_num_tpEBemul_spike,*vertices_num_tpEBemul))
     {
       pEff2 = new TEfficiency(*vertices_num_tpEBemul_spike,*vertices_num_tpEBemul);
       pEff2->SetMarkerStyle(3);
       pEff2->SetLineColor(kBlue);
     }
   
   pEff2->Draw();
   gPad->Update();
   pEff2->GetPaintedGraph()->GetYaxis()->SetRangeUser(0.0,1.0);
   

   vertices_num_tpEBeg23_emul->Rebin(2);
   vertices_num_tpEBeg23_emul_spike->Rebin(2);


   TEfficiency* pEff_eg23_2 = 0;

   if(TEfficiency::CheckConsistency(*vertices_num_tpEBeg23_emul_spike,*vertices_num_tpEBeg23_emul))
     {
       pEff_eg23_2 = new TEfficiency(*vertices_num_tpEBeg23_emul_spike,*vertices_num_tpEBeg23_emul);
       pEff_eg23_2->SetMarkerStyle(3);
       pEff_eg23_2->SetLineColor(kBlack);
       pEff_eg23_2->Draw("same");
     }
   gPad->Update();
   pEff_eg23_2->GetPaintedGraph()->GetYaxis()->SetRangeUser(0.0,1.0);

   vertices_num_tpEBeg30_emul->Rebin(2);
   vertices_num_tpEBeg30_emul_spike->Rebin(2);


   TEfficiency* pEff_eg30_2 = 0;

   if(TEfficiency::CheckConsistency(*vertices_num_tpEBeg30_emul_spike,*vertices_num_tpEBeg30_emul))
     {
       pEff_eg30_2 = new TEfficiency(*vertices_num_tpEBeg30_emul_spike,*vertices_num_tpEBeg30_emul);
       pEff_eg30_2->SetLineColor(kRed);
       //       pEff_eg30_2->Draw("same");
     }

   TLegend *leg9101 = new TLegend(0.57,0.65,0.95,0.88,"", "brNDC");
   leg9101->SetTextFont(62); //22, 62
   leg9101->SetTextSize(0.032); // 0.03, 0.048
   leg9101->SetLineColor(1);
   leg9101->SetLineStyle(1);
   leg9101->SetLineWidth(1);
   leg9101->SetFillStyle(1001);
   leg9101->SetFillColor(10);
   leg9101->AddEntry(pEff2,"#bf{ALL TP}","l");
   leg9101->AddEntry(pEff_eg23_2,"#bf{Above 20 GeV}","l");
   leg9101->AddEntry(pEff_eg30_2,"#bf{Above 30 GeV}","l");

   leg9101->Draw();


   c1899->Print("plots/"+runnum+"Fraction_spikes_emul_vertices.png");


   TCanvas *c2889 = new TCanvas("vertices_num_tpEB_Spikes", "TP Spikes", 800, 600);
   c2889->cd();
   c2889->SetLogy();
   vertices_num_tpEEPlus->Draw();
   c2889->Print("plots/"+runnum+"TPEEPlus_vertices.png");


   TCanvas *c2778 = new TCanvas("vertices_num_tpEB_Spikes", "TP Spikes", 800, 600);
   c2778->cd();
   c2778->SetLogy();
   vertices_num_tpEEPlusemul->Draw();
   c2778->Print("plots/"+runnum+"TPEMULEEPlus_vertices.png");


   TCanvas *c3889 = new TCanvas("vertices_num_tpEB_Spikes", "TP Spikes", 800, 600);
   c3889->cd();
   c3889->SetLogy();
   vertices_num_tpEEMinus->Draw();
   c3889->Print("plots/"+runnum+"TPEEMinus_vertices.png");


   TCanvas *c3778 = new TCanvas("vertices_num_tpEB_Spikes", "TP Spikes", 800, 600);
   c3778->cd();
   c3778->SetLogy();
   vertices_num_tpEEMinusemul->Draw();
   c3778->Print("plots/"+runnum+"TPEMULEEMinus_vertices.png");




   TCanvas *c17 = new TCanvas("TPEB_Spikes", "TP Spikes", 800, 600);
   c17->cd();
   c17->SetTicks();
   c17->SetLogy();
   TPEB->Draw("hist");
   TPEB->SetYTitle("#bf{Number of TP}");
   TPEB->SetXTitle("#bf{TP (ADC)}");
   TPEB->GetXaxis()->SetLabelFont(42);
   TPEB->GetYaxis()->SetLabelFont(42);
   TPEB->SetLineColor(kBlack);
   TPEB_Spike->SetLineColor(kBlue);
   //   TPEB_Spike->SetFillColor(kYellow-10);
   TPEB_Spike->Draw("hist same");
   TPEB_sFGVB0->SetLineColor(kGreen);
   //   TPEB_sFGVB0->SetFillColor(kRed);
   TPEB_sFGVB0->Draw("hist same");

   //   TPEB_Spike2->SetLineColor(7);
   // TPEB_Spike2->SetFillColor(kGreen-10);
   // TPEB_Spike2->Draw("hist same");
   
   cout << "number of good VS bad TP " << TPEB->Integral() <<  " : " <<  TPEB_Spike->Integral() << " : " << TPEB_Spike->Integral()/TPEB->Integral() <<  endl;
   cout << "number of good VS bad TP above 23 GeV " << TPEB->Integral(46, 256) <<  " : " <<  TPEB_Spike->Integral(46, 256) << " : " <<  TPEB_Spike->Integral(46, 256)/TPEB->Integral(46, 256) << endl;
   cout << "number of good VS bad TP above 30 GeV " << TPEB->Integral(60, 256) <<  " : " <<  TPEB_Spike->Integral(60, 256) << " : " <<  TPEB_Spike->Integral(60, 256)/TPEB->Integral(60, 256) << endl;
   cout << "number of good VS bad TP >= 256 " << TPEB->GetBinContent(256) <<  " : " <<  TPEB_Spike->GetBinContent(256) << " : "  <<   TPEB_Spike->GetBinContent(256)/TPEB->GetBinContent(256)  <<endl;

   TLatex *   tex11 = new TLatex(0.6,0.30,"sFGVB_{th} = 12 ");
   tex11->SetNDC();
   tex11->SetTextAlign(13);
   tex11->SetTextFont(42);
   tex11->SetTextSize(0.03);
   tex11->SetLineWidth(2);
   tex11->Draw(); 

   TLatex *   tex12 = new TLatex(0.6,0.25,"Spike_{th}   = 12 GeV");
   tex12->SetNDC();
   tex12->SetTextAlign(13);
   tex12->SetTextFont(42);
   tex12->SetTextSize(0.03);
   tex12->SetLineWidth(2);
   tex12->Draw(); 

   TLatex *   tex = new TLatex(0.13,0.88,"#bf{CMS Preliminary}");
   tex->SetNDC();
   tex->SetTextAlign(13);
   tex->SetTextFont(42);
   tex->SetTextSize(0.04);
   tex->SetLineWidth(2);
   tex->Draw();                                                                                                                                                                                            
   float Rp=TPEB_Spike->Integral()/TPEB->Integral();
   float Rp20=TPEB_Spike->Integral(40,256)/TPEB->Integral(40,256);
   float Rp30=TPEB_Spike->Integral(60,256)/TPEB->Integral(60,256);
   char Rper[100];
   char Rper20[100];
   char Rper30[100];

   sprintf(Rper,"%f",Rp);
   sprintf(Rper20,"%f",Rp20);
   sprintf(Rper30,"%f",Rp30);

   TString spike_Rperc(Rper);
   TString spike_Rperc20(Rper20);
   TString spike_Rperc30(Rper30);
  


   // TLatex *   tex28 = new TLatex(0.5,0.53,"%Spikes: "+spike_Rperc);
   // tex28->SetNDC();
   // tex28->SetTextAlign(13);
   // tex28->SetTextFont(42);
   // tex28->SetTextSize(0.03);
   // tex28->SetLineWidth(2);
   // tex28->Draw();                                                                                                                            

   // TLatex *   tex29 = new TLatex(0.5,0.48,"%Spikes>20GeV: "+spike_Rperc20);
   // tex29->SetNDC();
   // tex29->SetTextAlign(13);
   // tex29->SetTextFont(42);
   // tex29->SetTextSize(0.03);
   // tex29->SetLineWidth(2);
   // tex29->Draw();                                                                                                                            


   // TLatex *   tex30 = new TLatex(0.5,0.43,"%Spikes>30GeV: "+spike_Rperc30);
   // tex30->SetNDC();
   // tex30->SetTextAlign(13);
   // tex30->SetTextFont(42);
   // tex30->SetTextSize(0.03);
   // tex30->SetLineWidth(2);
   // tex30->Draw();                                                                                                                            

   TLatex *   tex33 = new TLatex(0.45,0.275,"#bf{DEFAULT}#rightarrow");
   tex33->SetNDC();
   tex33->SetTextAlign(13);
   tex33->SetTextFont(42);
   tex33->SetTextColor(kRed);
   tex33->SetTextSize(0.03);
   tex33->SetLineWidth(2);
   tex33->Draw();                                                                                                                            
                                                                                                                                                                                                          
   TLatex *   tex1 = new TLatex(0.56,0.98,campaign+" ZeroBias Run " +runnum);

   tex1->SetNDC();
   tex1->SetTextAlign(13);
   tex1->SetTextFont(42);
   tex1->SetTextSize(0.04);
   tex1->SetLineWidth(2);
   tex1->Draw();                                                                                                                                                                                              
   TLegend *leg = new TLegend(0.42,0.65,0.80,0.88,"", "brNDC");
   leg->SetTextFont(62); //22, 62
   leg->SetTextSize(0.032); // 0.03, 0.048
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillStyle(1001);
   leg->SetFillColor(10);
   leg->AddEntry(TPEB,"#bf{All TP in EB}","l");
   leg->AddEntry(TPEB_Spike,"#bf{TP with severity level 3 or 4}","l");
   leg->AddEntry(TPEB_sFGVB0,"#bf{TP with sFGVB = 0}","l");
   leg->Draw();                                                                                                                                                                                            
   c17->Print("plots/"+runnum+"-SpikeComparison"+sfgvb+"_"+etkill+".png");
                                     
   //=============This is for emulated TP ===================

   TCanvas *c18 = new TCanvas("TPEmulEB_Spikes", "Emulated TP Spikes", 800, 600);
   c18->cd();
   c18->SetTicks();
   c18->SetLogy();
   TPEmulEB_realTP->Draw("hist");
   TPEmulEB_realTP->SetYTitle("#bf{Number of TP}");
   TPEmulEB_realTP->SetXTitle("#bf{TP (ADC)}");
   TPEmulEB_realTP->GetXaxis()->SetLabelFont(42);
   TPEmulEB_realTP->GetYaxis()->SetLabelFont(42);
   TPEmulEB_realTP->SetLineColor(kBlack);
   TPEmulEB_realTP_Spike->SetLineColor(kBlue);
   //   TPEmulEB_realTP_Spike->SetFillColor(kYellow-10);
   TPEmulEB_realTP_Spike->Draw("hist same");
   TPEmulEB_sFGVB0->SetLineColor(kGreen);
   //   TPEmulEB_sFGVB0->SetFillColor(kRed);
   TPEmulEB_sFGVB0->Draw("hist same");

   cout << "number of good VS bad TP " << TPEmulEB_realTP->Integral() <<  " : " <<  TPEmulEB_realTP_Spike->Integral() << " : " << TPEmulEB_realTP_Spike->Integral()/TPEmulEB_realTP->Integral() <<  endl;
   cout << "number of good VS bad TP above 23 GeV " << TPEmulEB_realTP->Integral(46, 256) <<  " : " <<  TPEmulEB_realTP_Spike->Integral(46, 256) << " : " <<  TPEmulEB_realTP_Spike->Integral(46, 256)/TPEmulEB_realTP->Integral(46, 256) << endl;
   cout << "number of good VS bad TP above 30 GeV " << TPEmulEB_realTP->Integral(60, 256) <<  " : " <<  TPEmulEB_realTP_Spike->Integral(60, 256) << " : " <<  TPEmulEB_realTP_Spike->Integral(60, 256)/TPEmulEB_realTP->Integral(60, 256) << endl;
   cout << "number of good VS bad TP >= 256 " << TPEmulEB_realTP->GetBinContent(256) <<  " : " <<  TPEmulEB_realTP_Spike->GetBinContent(256) << " : "  <<   TPEmulEB_realTP_Spike->GetBinContent(256)/TPEmulEB_realTP->GetBinContent(256)  <<endl;

   TLatex *   tex3 = new TLatex(0.13,0.88,"#bf{CMS Preliminary}");
   tex3->SetNDC();
   tex3->SetTextAlign(13);
   tex3->SetTextFont(42);
   tex3->SetTextSize(0.04);
   tex3->SetLineWidth(2);
   tex3->Draw();                                                                                                                                                                                                                                                                                                                                                                                                      
   TLatex *   tex4 = new TLatex(0.56,0.98,campaign+" ZeroBias Run "+runnum);
   //   TLatex *   tex4 = new TLatex(0.56,0.98,"2015C SinglePhoton Run 254833");
   //   TLatex *   tex4 = new TLatex(0.56,0.98,"2015C ZeroBias Run 254833");
   //   TLatex *   tex4 = new TLatex(0.56,0.98,"2015C ZeroBias Run 254833");
   tex4->SetNDC();
   tex4->SetTextAlign(13);
   tex4->SetTextFont(42);
   tex4->SetTextSize(0.04);
   tex4->SetLineWidth(2);
   tex4->Draw();                                                                                                                                                                                          
 
   TLatex *   tex31 = new TLatex(0.4,0.275,"#bf{NewWorkingPoint}#rightarrow");
   tex31->SetNDC();
   tex31->SetTextAlign(13);
   tex31->SetTextFont(42);
   tex31->SetTextColor(kBlue);
   tex31->SetTextSize(0.03);
   tex31->SetLineWidth(2);
   tex31->Draw();                                                                                                                            


   TLatex *   tex21 = new TLatex(0.6,0.30,"sFGVB_{th} = "+sfgvb);
   tex21->SetNDC();
   tex21->SetTextAlign(13);
   tex21->SetTextFont(42);
   tex21->SetTextSize(0.03);
   tex21->SetLineWidth(2);
   tex21->Draw();                                                                                                                                                                                          
   TLatex *   tex22 = new TLatex(0.6,0.25,"Spike_{th}  = "+etkill+" GeV");
   tex22->SetNDC();
   tex22->SetTextAlign(13);
   tex22->SetTextFont(42);
   tex22->SetTextSize(0.03);
   tex22->SetLineWidth(2);
   tex22->Draw();                                                                                                                            
   
   float p=TPEmulEB_realTP_Spike->Integral()/TPEmulEB_realTP->Integral();
   float p20=TPEmulEB_realTP_Spike->Integral(40,256)/TPEmulEB_realTP->Integral(40,256);
   float p30=TPEmulEB_realTP_Spike->Integral(60,256)/TPEmulEB_realTP->Integral(60,256);
   char per[100];
   char per20[100];
   char per30[100];

   sprintf(per,"%f",p);
   sprintf(per20,"%f",p20);
   sprintf(per30,"%f",p30);

   TString spike_perc(per);
   TString spike_perc20(per20);
   TString spike_perc30(per30);
  


   // TLatex *   tex23 = new TLatex(0.5,0.53,"%Spikes: "+spike_perc);
   // tex23->SetNDC();
   // tex23->SetTextAlign(13);
   // tex23->SetTextFont(42);
   // tex23->SetTextSize(0.03);
   // tex23->SetLineWidth(2);
   // tex23->Draw();                                                                                                                            

   // TLatex *   tex24 = new TLatex(0.5,0.48,"%Spikes>20GeV: "+spike_perc20);
   // tex24->SetNDC();
   // tex24->SetTextAlign(13);
   // tex24->SetTextFont(42);
   // tex24->SetTextSize(0.03);
   // tex24->SetLineWidth(2);
   // tex24->Draw();                                                                                                                            


   // TLatex *   tex25 = new TLatex(0.5,0.43,"%Spikes>30GeV: "+spike_perc30);
   // tex25->SetNDC();
   // tex25->SetTextAlign(13);
   // tex25->SetTextFont(42);
   // tex25->SetTextSize(0.03);
   // tex25->SetLineWidth(2);
   // tex25->Draw();                                                                                                                            
                                                              

   
   TLegend *leg2 = new TLegend(0.42,0.65,0.80,0.88,"", "brNDC");
   leg2->SetTextFont(62); //22, 62
   leg2->SetTextSize(0.032); // 0.03, 0.048
   leg2->SetLineColor(1);
   leg2->SetLineStyle(1);
   leg2->SetLineWidth(1);
   leg2->SetFillStyle(1001);
   leg2->SetFillColor(10);
   leg2->AddEntry(TPEmulEB_realTP,"#bf{All Emulated TP in EB}","l");
   leg2->AddEntry(TPEmulEB_realTP_Spike,"#bf{Emulated TP with Severity Level 3 or 4 (Spikes)}","l");
   leg2->AddEntry(TPEmulEB_sFGVB0,"#bf{Emulated TP with sFGVB =0}","l");

   leg2->Draw();
   c18->Print("plots/"+runnum+"-SpikeComparison-Emul"+sfgvb+"_"+etkill+".png");


   cout << "number of good VS bad L1 " << L1IsoCandRank->Integral() <<  " : " <<  L1IsoCandRank_spikes->Integral() << " : " << L1IsoCandRank_spikes->Integral()/L1IsoCandRank->Integral() <<  endl;
   cout << "number of good VS bad L1 above 23 GeV " << L1IsoCandRank->Integral(23, 64) <<  " : " <<  L1IsoCandRank_spikes->Integral(23, 64) << " : " <<  L1IsoCandRank_spikes->Integral(23, 64)/L1IsoCandRank->Integral(23, 64) << endl;
   cout << "number of good VS bad L1 above 30 GeV " << L1IsoCandRank->Integral(30, 64) <<  " : " <<  L1IsoCandRank_spikes->Integral(30, 64) << " : " <<  L1IsoCandRank_spikes->Integral(30, 64)/L1IsoCandRank->Integral(30, 64) << endl;
   cout << "number of good VS bad L1 >= 64 " << L1IsoCandRank->GetBinContent(64) <<  " : " <<  L1IsoCandRank_spikes->GetBinContent(64) << " : "  <<   L1IsoCandRank_spikes->GetBinContent(64)/L1IsoCandRank->GetBinContent(64)  <<endl;



   TCanvas *c4 = new TCanvas("L1 Spectrum", "L1 Spectrum", 800, 600);
   c4->cd();
   c4->SetTicks();
   c4->SetLogy();
   L1IsoCandRank->Draw("hist");
   L1IsoCandRank->SetYTitle("#bf{Number of L1}");
   L1IsoCandRank->SetXTitle("#bf{Et (GeV)}");
   L1IsoCandRank->GetXaxis()->SetLabelFont(42);
   L1IsoCandRank->GetYaxis()->SetLabelFont(42);
   L1IsoCandRank->SetLineColor(1);
   L1IsoCandRank_spikes->SetLineColor(4);
   L1IsoCandRank_spikes->SetFillColor(kYellow-10);
   L1IsoCandRank_spikes->Draw("hist same");c4->SetTicks();

   TLatex *   tex5 = new TLatex(0.56,0.98,campaign+" ZeroBias Run "+runnum);
   tex5->SetNDC();
   tex5->SetTextAlign(13);
   tex5->SetTextFont(42);
   tex5->SetTextSize(0.04);
   tex5->SetLineWidth(2);
   tex5->Draw();                                                                                                                                                                                           


   TLegend *lego = new TLegend(0.42,0.65,0.80,0.88,"", "brNDC");
   lego->SetTextFont(62); //22, 62                                                                                                            
   lego->SetTextSize(0.032); // 0.03, 0.048                                                                                                   
   lego->SetLineColor(1);
   lego->SetLineStyle(1);
   lego->SetLineWidth(1);
   lego->SetFillStyle(1001);
   lego->SetFillColor(10);
   lego->AddEntry(L1IsoCandRank,"#bf{All L1 candidates}","l");
   lego->AddEntry(L1IsoCandRank_spikes,"#bf{L1 candidates from Spike induced TP}","fe");
   lego->Draw();

   c4->Print("plots/"+runnum+"L1-candidates-comp"+sfgvb+"_"+etkill+".png");

   TCanvas *c5 = new TCanvas("L1 ratio", "L1 Ratio", 800, 600);
   c5->cd();
   L1IsoCandRank_spikes->Divide(L1IsoCandRank);
   L1IsoCandRank_spikes->Draw("hist");
   L1IsoCandRank_spikes->SetYTitle("#bf{ratio}");
   L1IsoCandRank_spikes->SetXTitle("#bf{Et (GeV)}");
   L1IsoCandRank_spikes->GetXaxis()->SetLabelFont(42);
   L1IsoCandRank_spikes->GetYaxis()->SetLabelFont(42);


   TLatex *   tex8 = new TLatex(0.56,0.98,campaign+" ZeroBias Run "+runnum);                                                                                                                                             
   tex8->SetNDC();
   tex8->SetTextAlign(13);
   tex8->SetTextFont(42);
   tex8->SetTextSize(0.04);
   tex8->SetLineWidth(2);
   tex8->Draw();                                                                                                                                                                                           

   c5->Print("plots/"+runnum+"L1-spikeinduced-total-ratio"+sfgvb+"_"+etkill+".png");


   cout << "number of good VS bad L1 Emul" << L1IsoEmulCandRank->Integral() <<  " : " <<  L1IsoEmulCandRank_spikes->Integral() << " : " << L1IsoEmulCandRank_spikes->Integral()/L1IsoEmulCandRank->Integral() <<  endl;
   cout << "number of good VS bad L1 Emulabove 23 GeV " << L1IsoEmulCandRank->Integral(23, 64) <<  " : " <<  L1IsoEmulCandRank_spikes->Integral(23, 64) << " : " <<  L1IsoEmulCandRank_spikes->Integral(23, 64)/L1IsoEmulCandRank->Integral(23, 64) << endl;
   cout << "number of good VS bad L1 Emulabove 30 GeV " << L1IsoEmulCandRank->Integral(30, 64) <<  " : " <<  L1IsoEmulCandRank_spikes->Integral(30, 64) << " : " <<  L1IsoEmulCandRank_spikes->Integral(30, 64)/L1IsoEmulCandRank->Integral(30, 64) << endl;
   cout << "number of good VS bad L1 Emul>= 64 " << L1IsoEmulCandRank->GetBinContent(64) <<  " : " <<  L1IsoEmulCandRank_spikes->GetBinContent(64) << " : "  <<   L1IsoEmulCandRank_spikes->GetBinContent(64)/L1IsoEmulCandRank->GetBinContent(64)  <<endl;



   TCanvas *c44 = new TCanvas("L1 EmulSpectrum", "L1 EmulSpectrum", 800, 600);
   c44->cd();
   c44->SetTicks();
   c44->SetLogy();
   L1IsoEmulCandRank->Draw("hist");
   L1IsoEmulCandRank->SetYTitle("#bf{Number of L1}");
   L1IsoEmulCandRank->SetXTitle("#bf{Et (GeV)}");
   L1IsoEmulCandRank->GetXaxis()->SetLabelFont(42);
   L1IsoEmulCandRank->GetYaxis()->SetLabelFont(42);
   L1IsoEmulCandRank->SetLineColor(1);
   L1IsoEmulCandRank_spikes->SetLineColor(4);
   L1IsoEmulCandRank_spikes->SetFillColor(kYellow-10);
   L1IsoEmulCandRank_spikes->Draw("hist same");c4->SetTicks();

   TLatex *   tex55 = new TLatex(0.56,0.98,campaign+" ZeroBias Run "+runnum);
   tex55->SetNDC();
   tex55->SetTextAlign(13);
   tex55->SetTextFont(42);
   tex55->SetTextSize(0.04);
   tex55->SetLineWidth(2);
   tex55->Draw();                                                                                                                                                                                           

 
   TLegend *leg0las = new TLegend(0.42,0.65,0.80,0.88,"", "brNDC");
   leg0las->SetTextFont(62); //22, 62                                                                                                            
   leg0las->SetTextSize(0.032); // 0.03, 0.048                                                                                                   
   leg0las->SetLineColor(1);
   leg0las->SetLineStyle(1);
   leg0las->SetLineWidth(1);
   leg0las->SetFillStyle(1001);
   leg0las->SetFillColor(10);
   leg0las->AddEntry(L1IsoEmulCandRank,"#bf{All L1 Emulcandidates}","l");
   leg0las->AddEntry(L1IsoEmulCandRank_spikes,"#bf{L1 Emulcandidates from Spike induced TP}","fe");
   leg0las->Draw();

   c44->Print("plots/"+runnum+"L1emul-candidates-comp"+sfgvb+"_"+etkill+".png");

   TCanvas *c55 = new TCanvas("L1 Emulratio", "L1 EmulRatio", 800, 600);
   c55->cd();
   L1IsoEmulCandRank_spikes->Divide(L1IsoEmulCandRank);
   L1IsoEmulCandRank_spikes->Draw("hist");
   L1IsoEmulCandRank_spikes->SetYTitle("#bf{ratio}");
   L1IsoEmulCandRank_spikes->SetXTitle("#bf{Et (GeV)}");
   L1IsoEmulCandRank_spikes->GetXaxis()->SetLabelFont(42);
   L1IsoEmulCandRank_spikes->GetYaxis()->SetLabelFont(42);


   TLatex *   tex88 = new TLatex(0.56,0.98,campaign+" ZeroBias Run "+runnum);                                                                                                                                             
   tex88->SetNDC();
   tex88->SetTextAlign(13);
   tex88->SetTextFont(42);
   tex88->SetTextSize(0.04);
   tex88->SetLineWidth(2);
   tex88->Draw();                                                                                                                                                                                           

   c55->Print("plots/"+runnum+"L1emul-spikeinduced-total-ratio"+sfgvb+"_"+etkill+".png");

   TCanvas *crystalinf = new TCanvas("crystal info", "crystal info", 800, 600);
   crystalinf->cd();

   eta_badXtal->Draw();
   crystalinf->SaveAs("plots/"+runnum+"eta_badXtal"+sfgvb+"_"+etkill+".png");
   crystalinf->SaveAs("plots/"+runnum+"eta_badXtal"+sfgvb+"_"+etkill+".root");
   crystalinf->Update();

   theta_badXtal->Draw();
   crystalinf->SaveAs("plots/"+runnum+"theta_badXtal"+sfgvb+"_"+etkill+".png");
   crystalinf->SaveAs("plots/"+runnum+"theta_badXtal"+sfgvb+"_"+etkill+".root");
   crystalinf->Update();

   phi_badXtal->Draw();
   crystalinf->SaveAs("plots/"+runnum+"phi_badXtal"+sfgvb+"_"+etkill+".png");
   crystalinf->SaveAs("plots/"+runnum+"phi_badXtal"+sfgvb+"_"+etkill+".root");
   crystalinf->Update();
   
   crystalinf->SetLogy();
   eRec_badXtal->Draw();
   crystalinf->SaveAs("plots/"+runnum+"erec_badXtal"+sfgvb+"_"+etkill+".png");
   crystalinf->SaveAs("plots/"+runnum+"erec_badXtal"+sfgvb+"_"+etkill+".root");
   crystalinf->Update();


}


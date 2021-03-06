///g++ -o  fitEfficiency fitEfficiency.cc  `root-config --cflags --libs` -L $ROOFITSYS/lib -lRooFit -lRooFitCore -I$ROOFITSYS/include
/////////////////////////////////////////////////////
// FITTING WITH A ROOFIT-USER DEFINED CRYSTAL BALL //
/////////////////////////////////////////////////////

#ifndef DEF_FASTEFF
#define DEF_FASTEFF

// General C++
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <glob.h>
#include <cstdlib>
#include <stdio.h> 
#include <stdlib.h>
#include <vector>
#include <glob.h>

// RooFit headers
#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif

#include "RooFitResult.h"
#include "RooPlot.h"
#include "RooRealVar.h"
#include "RooCategory.h"
#include "RooEfficiency.h"
#include "RooDataSet.h"
#include "RooBinning.h"
#include "RooHist.h"
#include "RooWorkspace.h"

// Root headers
#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TFrame.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TGraph2D.h>
#include <TMath.h>
#include <TStyle.h>
#include <TSystem.h>
#include "TTree.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TString.h"

// Personal headers
#include "FuncCB.h"
#include "tdrstyle.h"

#define DEBUG 1

using namespace RooFit ;

TString fileIn;

using namespace std;
#include <string>
#include <iostream>
//#include <boost/filesystem.hpp>
using namespace std;
//using namespace boost::filesystem;

vector<string> globVector(const string& pattern){
  glob_t glob_result;
  glob(pattern.c_str(),GLOB_TILDE,NULL,&glob_result);
  vector<string> files;
  for(unsigned int i=0;i<glob_result.gl_pathc;++i){
    //cout << string(glob_result.gl_pathv[i]) << endl;
    files.push_back(string(glob_result.gl_pathv[i]));
  }
  globfree(&glob_result);
  return files;
}

bool dirExists(const char *path)
{
    struct stat info;

    if(stat( path, &info ) != 0)
        return false;
    else if(info.st_mode & S_IFDIR)
      return true;
    else
        return false;
}


void  fitManyFiles(unsigned int iEG, int iECAL1, int iColl1, int iECAL2, int iColl2, 
		     vector<string> fileIn0, TString label0, TString label1,
			     string dirIn, TString lumi, int nCPU, int color1, int style1, int color2, int style2,
			     TString probe, TString tag);


void loadPresentationStyle();
string myjobid; 
stringstream mydir ; 
vector<string> myfilevector;
//-------
int main(int argc, char**argv){
  myjobid = argv[1];
  cout << argc << endl;
  for (int i = 2 ; i <argc ; i++){
    myfilevector.push_back(argv[i]);
    cout << argv[i] << endl;
  }
  
  mydir.str("");
  mydir << "root://eoscms//eos/cms/store/caf/user/taroni/TestTriggerOptimization/";
  mydir << myjobid <<"/"<< "selectPairsDir"; 
  cout << __LINE__ <<" " << mydir << endl; 
  TApplication app("App",&argc, argv);

  // fastEfficiencySilvia(20, 0, 0, 0, 1,  "effi_TagProbe_tree_changed.root","effi_TagProbe_tree_changed.root", 
  // 		       "FGVB ratio theshold 0.90", "FGVB ratio threshold 0.95", mydir.str(), "19.7 fb^{-1}", 4, 
  // 		       kBlack, kFullCircle, kRed, kOpenSquare, "WP80", "WP80");

  fitManyFiles(20, 0, 1, 0, 1,  myfilevector,
	       "FGVB ratio theshold 0.90", "FGVB ratio threshold 0.95", mydir.str(), "19.7 fb^{-1}", 4, 
	       kBlack, kFullCircle, kRed, kOpenSquare, "WP80", "WP80");
  app.Run(); 
  
  return 0;
}

void loadPresentationStyle(){
  gROOT->SetStyle("Plain");
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetTitleXOffset(1.2);
  gStyle->SetTitleYOffset(0.01);
  gStyle->SetLabelOffset(0.005, "XYZ");
  gStyle->SetTitleSize(0.07, "XYZ");
  gStyle->SetTitleFont(22,"X");
  gStyle->SetTitleFont(22,"Y");
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.15);
  gStyle->SetHistLineWidth(2);
  setTDRStyle();
}


void fitManyFiles(unsigned int iEG, int iECAL1, int iColl1, int iECAL2, int iColl2, 
		  vector<string> fileIn0=myfilevector,
			  TString label0 = "FGVB ratio theshold 0.90", TString label1 = "FGVB ratio threshold 0.95",
			  string dirIn=mydir.str()
			 , TString lumi="", int nCPU=8, 
			 int color1=kBlack, int style1=kFullCircle, int color2=kRed, int style2=kOpenSquare,
			 TString probe="WP80", TString tag="WP80"){
  if (DEBUG) cout << __LINE__ << endl;
  gROOT->Reset();
  loadPresentationStyle();  
  gROOT->ForceStyle();
  const int nEG = 71;
  double thres[nEG];
  for(int i=0 ; i<nEG ; i++) thres[i]=i;
  if (DEBUG) cout << __LINE__ << endl;

  TString names[nEG];
  ostringstream ossi;
  for(int i=0;i<(int)nEG;i++) {
    ossi.str("");
    ossi << thres[i] ;
    names[i] = ossi.str();
  }
  if (DEBUG) cout << __LINE__ << endl;

  // NAMES //
  const int nECAL=2;
  const int nColl=2;

  TString name_leg_ecal[nECAL] = {"Barrel","Endcaps"};
  TString name_leg_coll[nColl] = {"Online","Emulation"};  
  if (DEBUG) cout << __LINE__ << endl;

  TString name_ecal[nECAL] = {"_EB","_EE"};
  TString name_coll[nColl] = {"_N","_M"};
  if (DEBUG) cout << __LINE__ << endl;

  stringstream dirResults;
  dirResults.str("");
  //  dirResults <<  dirIn <<  "/turnons/EG"<<names[iEG]<<"/" ;
  dirResults <<   "turnons/EG"<<names[iEG]<<"/" ;
  stringstream createDir; 
  createDir.str(""); 
  if(DEBUG) createDir<<  "turnons"; 
  if (dirExists(createDir.str().c_str())==false) {
    createDir.str("");
    createDir << "mkdir "<<   "turnons";
    system(createDir.str().c_str());
  }
  if(DEBUG) cout << __LINE__ << " " << dirExists(dirResults.str().c_str()) << endl;
  createDir.str(""); 
  createDir<< "mkdir " << dirResults.str().c_str()<< ""; 
  if (dirExists(dirResults.str().c_str())==false) system(createDir.str().c_str());
  cout << __LINE__ << " " << dirExists(dirResults.str().c_str()) << endl;
  cout << "DIR CREATED" << endl; 
  stringstream name_image ;
  name_image.str("");
  name_image << dirResults.str() << "eff_EG"<<names[iEG]<<"_tag"<<tag<<"_probe"<<probe<<name_ecal[iECAL1]<<name_coll[iColl1]<<"_vs"<<name_ecal[iECAL2]<<name_coll[iColl2] ;
  if (DEBUG) cout << __LINE__ << endl;

  // Output log //
  name_image << ".txt"; 
  ofstream fichier(name_image.str().c_str(), ios::out);


  // BINNING //
  const int nbins[nEG] = {29,29,29,29,21,21,21,22,22,21,22,21,22,18,19,18,18,18,18,20,20,20,20,19,20,20,20,20,21,21,
			  21,21,21,21,21,21,21,21,21,21, //EG30
			  22,22,22,22,22,22,22,22,22,22, //EG40
			  29,29,29,29,29,29,29,29,29,29, //EG50
			  29,29,29,29,29,29,29,29,29,29};//EG60

  if (DEBUG) cout << __LINE__ << endl;
  Double_t bins_0[29] = {1,1.5,1.8,2,2.2,2.4,2.6,2.8, 3, 3.5, 4,4.2,4.5,4.7,5,5.5,6,6.5,7,7.5,8,8.5,9,10,12,15,20,50,150};// EG0
  Double_t bins_1[29] = {1,1.5,1.8,2,2.2,2.4,2.6,2.8, 3, 3.5, 4,4.2,4.5,4.7,5,5.5,6,6.5,7,7.5,8,8.5,9,10,12,15,20,50,150};// EG1
  Double_t bins_2[29] = {1,1.5,1.8,2,2.2,2.4,2.6,2.8, 3, 3.5, 4,4.2,4.5,4.7,5,5.5,6,6.5,7,7.5,8,8.5,9,10,12,15,20,50,150};// EG2 
  Double_t bins_3[29] = {1,1.5,1.8,2,2.2,2.4,2.6,2.8, 3, 3.5, 4,4.2,4.5,4.7,5,5.5,6,6.5,7,7.5,8,8.5,9,10,12,15,20,50,150};// EG3

  Double_t bins_4[21] = {1, 2, 3, 4, 5, 6, 7, 9, 11, 13, 15, 17, 19, 21, 27, 32, 41, 50, 60, 70, 150}; // EG4
  Double_t bins_5[21] = {2, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 18, 20, 22, 24, 31, 40, 50, 60, 70, 150}; // EG5
  Double_t bins_6[21] = {3, 4, 5, 6, 7, 8, 9, 11, 13, 15, 17, 19, 21, 23, 27, 32, 41, 50, 60, 70, 150}; // EG6

  Double_t bins_7[22] = {2, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 18, 20, 22, 24, 26, 31, 40, 50, 60, 70, 150}; // EG7
  Double_t bins_8[22] = {3, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 21, 23, 25, 27, 32, 41, 50, 60, 70, 150}; // EG8
  Double_t bins_9[21] = {4, 6, 7, 8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 31, 40, 50, 60, 70, 150}; // EG9

  Double_t bins_10[22] = {5, 7, 8, 9, 10, 11, 12, 13, 15, 17, 19, 21, 23, 25, 27, 29, 32, 41, 50, 60, 70, 150}; // EG10
  Double_t bins_11[21] = {6, 8, 9, 10, 11, 12, 13, 14, 16, 18, 20, 22, 24, 26, 28, 31, 40, 50, 60, 70, 150}; // EG11
  Double_t bins_12[22] = {5, 7, 9, 10, 11, 12, 13, 14, 15, 17, 19, 21, 23, 25, 27, 29, 32, 41, 50, 60, 70, 150}; // EG12

  if (DEBUG) cout << __LINE__ << endl;
  Double_t bins_13[18] = {5, 7, 9, 11, 12, 13, 14, 15, 17, 19, 22, 25, 29, 37, 50, 60, 70, 150}; // EG13
  Double_t bins_14[19] = {6, 8, 10, 12, 13, 14, 15, 16, 18, 20, 22, 25, 30, 35, 40, 50, 60, 70, 150}; // EG14
  Double_t bins_15[18] = {5, 7, 9, 11, 13, 14, 15, 16, 17, 19, 22, 25, 29, 37, 50, 60, 70, 150}; // EG15

  Double_t bins_16[18] = {8, 10, 12, 14, 16, 17, 18, 19, 20, 22, 25, 30, 35, 40, 50, 60, 70, 150}; // EG16
  Double_t bins_17[18] = {9, 11, 13, 15, 16, 17, 18, 19, 21, 23, 25, 30, 35, 40, 50, 60, 70, 150}; // EG17
  Double_t bins_18[18] = {8, 10, 12, 14, 16, 17, 18, 19, 20, 22, 25, 30, 35, 40, 50, 60, 70, 150}; // EG18

  Double_t bins_19[20] = {9, 11, 13, 15, 17, 18, 19, 20, 21, 23, 25, 27, 30, 35, 40, 45, 50, 60, 70, 150}; // EG19
  // Double_t bins_20[20] = {8, 10, 12, 14, 16, 18, 19, 20, 21, 22, 24, 26, 30, 35, 40, 45, 50, 60, 70, 100}; // EG20
  Double_t bins_20[20] = {10, 14,  18,  20,  22, 26, 30, 35,  40, 45, 50, 60, 70, 100, 150, 200, 400, 600, 800, 1000}; // EG20
  
  Double_t bins_21[20] = {9, 11, 13, 15, 17, 19, 20, 21, 22, 23, 25, 27, 30, 35, 40, 45, 50, 60, 70, 150}; // EG21

  Double_t bins_22[20] = {10, 12, 14, 16, 18, 20, 21, 22, 23, 24, 26, 28, 30, 35, 40, 45, 50, 60, 70, 150}; // EG22
  Double_t bins_23[19] = {11, 13, 15, 17, 19, 21, 22, 23, 24, 25, 27, 30, 35, 40, 45, 50, 60, 70, 150}; // EG23
  Double_t bins_24[20] = {10, 12, 14, 16, 18, 20, 22, 23, 24, 25, 26, 28, 30, 35, 40, 45, 50, 60, 70, 150}; // EG24

  Double_t bins_25[20] = {11, 13, 15, 17, 19, 21, 23, 24, 25, 26, 27, 29, 30, 35, 40, 45, 50, 60, 70, 150}; // EG25
  Double_t bins_26[20] = {10, 12, 14, 16, 18, 20, 22, 24, 25, 26, 27, 28, 30, 35, 40, 45, 50, 60, 70, 150}; // EG26
  Double_t bins_27[20] = {11, 13, 15, 17, 19, 21, 23, 25, 26, 27, 28, 29, 33, 35, 40, 45, 50, 60, 70, 150}; // EG27
  if (DEBUG) cout << __LINE__ << endl;

  Double_t bins_28[21] = {10, 12, 14, 16, 18, 20, 22, 24, 26, 27, 28, 29, 30, 32, 35, 40, 45, 50, 60, 70, 150}; // EG28
  Double_t bins_29[21] = {11, 13, 15, 17, 19, 21, 23, 25, 27, 28, 29, 30, 31, 33, 35, 40, 45, 50, 60, 70, 150}; // EG29
  Double_t bins_30[21] = {10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 29, 30, 31, 32, 35, 40, 45, 50, 60, 70, 150}; // EG30

  Double_t bins_40[22] = {10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 38, 39, 40, 42, 45, 50, 60, 70, 150}; // EG40
  Double_t bins_50[29] = {10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 48, 50, 55, 60, 70, 90, 110, 130, 150, 170, 190}; // EG50
  Double_t bins_60[29] = {10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 48, 50, 55, 60, 70, 90, 110, 130, 150, 170, 190}; // EG60
  if (DEBUG) cout << __LINE__ << endl;

  vector< Double_t* > bins;
  bins.push_back( bins_0 ); bins.push_back( bins_1 ); bins.push_back( bins_2 ); bins.push_back( bins_3 ); bins.push_back( bins_4 ); 
  bins.push_back( bins_5 ); bins.push_back( bins_6 ); bins.push_back( bins_7 ); bins.push_back( bins_8 ); bins.push_back( bins_9 ); 
  bins.push_back( bins_10 ); bins.push_back( bins_11 ); bins.push_back( bins_12 ); bins.push_back( bins_13 ); bins.push_back( bins_14 ); 
  bins.push_back( bins_15 ); bins.push_back( bins_16 ); bins.push_back( bins_17 ); bins.push_back( bins_18 ); bins.push_back( bins_19 ); 
  bins.push_back( bins_20 ); bins.push_back( bins_21 ); bins.push_back( bins_22 ); bins.push_back( bins_23 ); bins.push_back( bins_24 ); 
  bins.push_back( bins_25 ); bins.push_back( bins_26 ); bins.push_back( bins_27 ); bins.push_back( bins_28 ); bins.push_back( bins_29 ); 
  if (DEBUG) cout << __LINE__ << endl;

  for(int iV=0 ; iV<10 ; iV++) bins.push_back( bins_30 );
  for(int iV=0 ; iV<10 ; iV++) bins.push_back( bins_40 );
  for(int iV=0 ; iV<10 ; iV++) bins.push_back( bins_50 );
  for(int iV=0 ; iV<10 ; iV++) bins.push_back( bins_60 );

  RooBinning binning = RooBinning(nbins[iEG]-1, bins[iEG], "binning");
  if (DEBUG) cout << __LINE__ << endl;

  vector <string> fileNameVector;

  TCanvas* ca = new TCanvas("ca","Trigger Efficiency") ;
    
  ca->SetGridx();
  ca->SetGridy();

  TLegend *leg = new TLegend(0.2,0.435,0.6,0.560,NULL,"brNDC"); // mid : x=353.5
  leg->SetLineColor(1);
  leg->SetTextColor(1);
  leg->SetTextFont(42);
  leg->SetTextSize(0.025);
  leg->SetShadowColor(kWhite);
  leg->SetFillColor(kWhite);
  leg->SetMargin(0.2);

  TLegend *leg2 = new TLegend(0.16,0.725,0.58,0.905,NULL,"brNDC");
  leg2->SetBorderSize(0);
  leg2->SetTextFont(62);
  leg2->SetTextSize(0.03);
  leg2->SetLineColor(0);
  leg2->SetLineStyle(1);
  leg2->SetLineWidth(1);
  leg2->SetFillColor(0);
  leg2->SetFillStyle(0);
  leg2->AddEntry("NULL","Threshold : "+names[iEG]+" GeV","h");

    
  TPaveText *pt2 = new TPaveText(0.220,0.605,0.487,0.685,"brNDC"); // mid : x=353.5                                          
  pt2->SetLineColor(1);
  pt2->SetTextColor(1);
  pt2->SetTextFont(42);
  pt2->SetTextSize(0.03);
  pt2->SetFillColor(kWhite);
  pt2->SetShadowColor(kWhite);
  pt2->AddText("L1 E/Gamma Trigger");
  pt2->AddText("Electrons from Z");

  vector <RooFitResult*> vRes;
  vector <TH1F*> vHisto;
  vector <double> maxEff_, et_95Eff_;
  vector <string> name_; 
  vector<RooDataSet> dataset; 
  TTree * lastTree = new TTree("Efficiency","Efficiency");
  lastTree->Branch("maxEff",&maxEff_);
  lastTree->Branch("et_95Eff"  ,&et_95Eff_   );
  lastTree->Branch("name", &name_);

  vector<RooPlot*> vFrame;
  //INPUT DATA //
  stringstream filename ;
  //vector<string> filenames; 
  // stringstream dirname;
  // dirname.str("");
  // dirname<< dirIn.c_str()<< "effi_TagProbe_tree_/*.root"; 
  // filenames= globVector(dirname.str().c_str());
  // cout << "number of files: " << filenames.size()<< endl; 
  // for (int i =0 ; i< filenames.size() ; i++){
  //   cout << filenames[i] << endl;
  // }

  // filename.str(""); 
  // filename << dirIn << "/" <<fileIn0; 
  // cout << "files" << fileIn0 << endl;
  // //fileNameVector= globVector(filename.str().c_str());
  // fileNameVector=findFile(dirIn);
  cout << "number of files: " << fileIn0.size()<< endl; 

  for (int i = 0 ; i < fileIn0.size() ; i++){
    filename.str(""); 
    filename << dirIn << "/" <<fileIn0[i];

    TFile* f0 = TFile::Open(filename.str().c_str());
    //  filename.str(""); 
    //filename << dirIn << "/" <<fileIn1;   
    //TFile* f1 = TFile::Open(filename.str().c_str());
    cout << filename.str().c_str() << endl;
    TTree* treenew;
    //TTree* treenew_2;
    
    treenew = (TTree*) f0->Get( "treenew"+name_ecal[iECAL1]+name_coll[iColl1] ) ;
    // treenew_2 = (TTree*) f1->Get( "treenew"+name_ecal[iECAL2]+name_coll[iColl2] ) ;
    if (DEBUG) cout << __LINE__ << endl;
    
    TString name_scet[2], name_scdr[2], name_l1bin[2];
    name_scet[0] = "sc_et"+name_ecal[iECAL1]+name_coll[iColl1];
    //name_scet[1] = "sc_et"+name_ecal[iECAL2]+name_coll[iColl2];
    name_scdr[0] = "sc_dr"+name_ecal[iECAL1]+name_coll[iColl1];
    //name_scdr[1] = "sc_dr"+name_ecal[iECAL2]+name_coll[iColl2];
    
    name_l1bin[0] = "l1_"+names[iEG]+name_ecal[iECAL1]+name_coll[iColl1];
    //name_l1bin[1] = "l1_"+names[iEG]+name_ecal[iECAL2]+name_coll[iColl2];
    //cout << __LINE__ << " " << name_l1bin[0] << " " << name_l1bin[1] << endl;
    
    RooRealVar et_plot(name_scet[0],name_scet[0],0,1000) ;
    RooRealVar dr(name_scdr[0],name_scdr[0],0.5,1.5) ; 
    //RooRealVar et_plot2(name_scet[1],name_scet[1],0,1000) ;
    //RooRealVar dr2(name_scdr[1],name_scdr[1],0.5,1.5) ;
    if (DEBUG) cout << __LINE__ << endl;
    
    // Acceptance state cut (1 or 0)
    RooCategory cut(name_l1bin[0],name_l1bin[0]) ;
    cut.defineType("accept",1) ;
    cut.defineType("reject",0) ;
    // RooCategory cut2(name_l1bin[1],name_l1bin[1]) ;
    // cut2.defineType("accept",1) ;
    //cut2.defineType("reject",0) ;
  
  // PARAMETRES ROOFIT CRYSTAL BALL
    RooRealVar norm("norm","N",1,0.6,1);
    RooRealVar alpha("alpha","#alpha",0.5,0.1,8);
    RooRealVar n("n","n",2.,1.1,35);
    RooRealVar mean("mean","mean",21.,15,1000);
    //mean.setVal(thres[iEG]);
    RooRealVar sigma("sigma","#sigma",0.9,0.1,5);
    //RooRealVar pedestal("pedestal","pedestal",0.01,0,0.4);
    if (DEBUG) cout << __LINE__ << endl;
    
    // RooRealVar norm2("norm2","N",0.999069,0.6,1);
    // RooRealVar alpha2("alpha2","#alpha",0.492303,0.01,8);
    // RooRealVar n2("n2","n",11.6694,1.1,35);
    // RooRealVar mean2("mean2","mean",21.4582,0,100);
    // //mean2.setVal(thres[iEG]);
    // RooRealVar sigma2("sigma2","#sigma",1.19,0.01,5);
    // //RooRealVar pedestal2("pedestal2","pedestal",0.01,0,0.4);
    // if (DEBUG) cout << __LINE__ << endl;

    FuncCB cb("cb","Crystal Ball Integree",et_plot,mean,sigma,alpha,n,norm) ;
    //FuncCB cb2("cb2","Crystal Ball Integree",et_plot2,mean2,sigma2,alpha2,n2,norm2) ;
    if (DEBUG) cout << __LINE__ << endl;
  
    // EFFICIENCY //
    RooEfficiency eff("eff","efficiency",cb,cut,"accept");
    //RooEfficiency eff2("eff2","efficiency",cb2,cut2,"accept");
    
    // DATASETS //
    if (DEBUG) cout << __LINE__ << endl;
    stringstream dataname; 
    dataname.str(""); 
    dataname << "data"<< i;
    RooDataSet dataSet(dataname.str().c_str(),dataname.str().c_str(), RooArgSet(et_plot, cut, dr), Import(*treenew));//,Import(*treenew)); 
    dataset.push_back(dataSet);
    // if (DEBUG) cout << __LINE__ << endl;
    // RooDataSet dataSet2("data2","data2", RooArgSet(et_plot2, cut2, dr2), Import(*treenew_2));//,Import(*treenew_2));
    if (DEBUG) cout << __LINE__ << endl;
    cout << __LINE__ << endl;
    dataSet.Print();
    //    dataSet2.Print();
    if (DEBUG) cout << __PRETTY_FUNCTION__ << " " << __LINE__ << endl;
  
    // PLOT //
    
    RooPlot* frame = et_plot.frame(Bins(18000),Title("Fitted efficiency")) ;
    dataname.str(""); 
    dataname << "frame"<< i;
    
    // TH1* hh_data_all = dataSet.createHistogram("hh_data_all", et_plot,Binning(binning)) ;
    // TH1* hh_data_sel = dataSet.createHistogram("hh_data_sel",eff,Binning(binning)) ; 

    // TH1* hh_eff = (TH1*)hh_data_sel->Clone();
    // //hh_eff->Divide(hh_data_all);

    // hh_eff->Draw();
    // ca->SaveAs("histoEffic.root") ;


    frame->SetName(dataname.str().c_str()); 
    //RooPlot* frame2 = et_plot2.frame(Bins(18000),Title("Fitted efficiency")) ;

    dataSet.plotOn(frame, Binning(binning), Efficiency(cut), MarkerColor(i+1), LineColor(i+1), MarkerStyle(20+i) );
    //dataSet2.plotOn(frame2, Binning(binning), Efficiency(cut2), MarkerColor(color2), LineColor(color2), MarkerStyle(style2) );
    if (DEBUG) cout << __LINE__ << endl;
    
    /////////////////////// FITTING /////////////////////////////
    
    double fit_cuts_min = thres[iEG]-2.5 ;
    double fit_cuts_max = 150;
    if (DEBUG) cout << __LINE__ << endl;
    
    et_plot.setRange("interesting",fit_cuts_min,fit_cuts_max);
    //et_plot2.setRange("interesting",fit_cuts_min,fit_cuts_max);
    stringstream fitres; 
    fitres.str(""); 
    fitres << "roofitres" << i ; 

    RooFitResult* roofitres1 = new RooFitResult(fitres.str().c_str(),fitres.str().c_str());
    //RooFitResult* roofitres2 = new RooFitResult("roofitres2","roofitres2");
    if (DEBUG) cout << __LINE__ << endl;

    fichier << "Fit characteristics :"   << endl ;
    fichier << "EG "     << names[iEG] << endl ;
    fichier << "Fit Range , EB Coll : [" << fit_cuts_min << "," << fit_cuts_max << "]" << endl ;
    fichier << "Fit Range , EE Coll : [" << fit_cuts_min << "," << fit_cuts_max << "]" << endl ;
    fichier << "----------------------"  << endl ;
    if (DEBUG) cout << __LINE__ << endl;
  
    // Fit #1 //
    //roofitres1 = eff.fitTo(dataSet,ConditionalObservables(et_plot),Range("interesting"),Minos(kTRUE),Warnings(kFALSE),NumCPU(nCPU),Save(kTRUE));
    if (DEBUG) cout << __LINE__ << endl;
    vRes.push_back(roofitres1);

   if (DEBUG) cout << __LINE__ << endl;
   //cb.plotOn(frame,LineColor(i+1),LineWidth(2));
    
    if (DEBUG) cout << __LINE__ << endl;
    name_.push_back(filename.str().substr(filename.str().find_last_of('/')+1,filename.str().substr(filename.str().find_last_of('/')).length()-5));
    maxEff_.push_back(cb.value(1000.));
    et_95Eff_.push_back(cb.find095Et(cb.value(1000.)));
    cout << "FIT results " << filename.str().substr(filename.str().find_last_of('/')+1,filename.str().substr(filename.str().find_last_of('/')).length()-6) << " " << cb.value(1000.)<< " " << cb.find095Et(cb.value(1000.)) << endl;

    if (DEBUG) cout << __LINE__ << endl;
    double res_norm1  = norm.getVal();
    double err_norm1  = norm.getErrorLo();
    double res_mean1  = mean.getVal();
    double err_mean1  = mean.getError();
    double res_sigma1 = sigma.getVal();
    double err_sigma1 = sigma.getError();
    double res_n1     = n.getVal();
    double err_n1     = n.getError();
    double res_alpha1 = alpha.getVal();
    double err_alpha1 = alpha.getError();
    
    fichier << "<----------------- EB ----------------->" << endl
	    << "double res_mean="  << res_mean1  << "; "
	    << "double res_sigma=" << res_sigma1 << "; "
	    << "double res_alpha=" << res_alpha1 << "; "
	    << "double res_n="     << res_n1     << "; "
	    << "double res_norm="  << res_norm1  << "; "
	    << endl
	    << "double err_mean="  << err_mean1  << "; "
	    << "double err_sigma=" << err_sigma1 << "; "
	    << "double err_alpha=" << err_alpha1 << "; "
	    << "double err_n="     << err_n1     << "; "
	    << "double err_norm="  << err_norm1  << "; "
	    << endl;
    if (DEBUG) cout << __LINE__ << endl;

    ////////////////////////////  DRAWING PLOTS AND LEGENDS /////////////////////////////////
    ca->cd();
    
    gPad->SetLogx();
    gPad->SetObjectStat(1);
    if (DEBUG) cout << __LINE__ << endl;
   
    
    frame->GetYaxis()->SetRangeUser(0,1.05);
    frame->GetXaxis()->SetRangeUser(10,1000.);// set to 1000 to see the plateau
    frame->GetYaxis()->SetTitle("Efficiency");
    frame->GetXaxis()->SetTitle("E_{T} [GeV]");
    vFrame.push_back(frame); 

    //vFrame.push_back(frame); 
    if (i==0){
      vFrame[i]->Draw() ;
    }else{
      vFrame[i]->Draw("SAME"); 
    }
    // if (DEBUG) cout << __LINE__ << endl;
    // ca->SaveAs("histoEffic.root") ;

    stringstream histoname; 
    histoname.str(""); 
    histoname<< "SCeta" << i; 
    if (DEBUG) cout << __LINE__ << endl;

    TH1F *SCeta1 = new TH1F(histoname.str().c_str(), histoname.str().c_str(),50,-2.5,2.5);
    SCeta1->SetLineColor(i+1) ;
    SCeta1->SetMarkerColor(i+1);
    SCeta1->SetMarkerStyle(i+20);
    
    SCeta1->SetTitle(filename.str().substr(filename.str().find_last_of('/')+1,filename.str().substr(filename.str().find_last_of('/')).length()-6).c_str()); 
    vHisto.push_back(SCeta1); 
    cout <<  __LINE__ << " " << filename.str().substr(filename.str().find_last_of('/')+1,filename.str().substr(filename.str().find_last_of('/')).length()-6).c_str()<< endl;
    string name = filename.str().substr(filename.str().find_last_of('/')+1,filename.str().substr(filename.str().find_last_of('/')).length()-6);
    if (DEBUG) cout << __LINE__ << endl;
    cout<<  __LINE__ << " " <<name.substr(name.find("tree")) << endl;
    // if (i==fileNameVector.size()-1) {
    name_image.str("");
    name_image << dirResults.str() << "eff_EG"<<names[iEG]<<"_tag"<<tag<<"_probe"<<probe << "_"<<name.c_str()<< ".root" ; 
    if (DEBUG) cout << __LINE__ << endl;
    cout << name_image.str() << endl;
    
    ca->SaveAs(name_image.str().c_str());
    if (DEBUG) cout << __LINE__ << endl;
    
    ca->SaveAs(name_image.str().replace(name_image.str().find(".root"), 5, ".cxx").c_str());
    if (DEBUG) cout << __LINE__ << endl;
    ca->SaveAs(name_image.str().replace(name_image.str().find(".root"), 5, ".png").c_str());//+".png","png");
    ca->SaveAs(name_image.str().replace(name_image.str().find(".root"), 5, ".gif").c_str());//+".gif","gif");
    ca->SaveAs(name_image.str().replace(name_image.str().find(".root"), 5, ".pdf").c_str());//+".pdf","pdf");
    ca->SaveAs(name_image.str().replace(name_image.str().find(".root"), 5, ".ps", 0, 3).c_str()); //,+".ps","ps");
    ca->SaveAs(name_image.str().replace(name_image.str().find(".root"), 5, ".eps").c_str());//,+".eps","eps");

    // }
    
    //f0->Close();
     if (DEBUG) cout << __LINE__ << endl;
   
  }//loop on files 

  if (DEBUG) cout << __LINE__ << endl;
 for (int i=0; i< vHisto.size() ; i++){
   leg->AddEntry(vHisto[i],vHisto[i]->GetTitle(),"pl");
  }
 if (DEBUG) cout << __LINE__ << endl;
      
  leg->Draw();
  leg2->Draw();
  pt2->Draw();
  if (DEBUG) cout << __LINE__ << endl;
 
  // ca->SaveAs(name_image.str().replace(name_image.str().find(".txt"), 4, ".cxx").c_str());
  // if (DEBUG) cout << __LINE__ << endl;
  // ca->SaveAs(name_image.str().replace(name_image.str().find(".txt"), 4, ".png").c_str());//+".png","png");
  // ca->SaveAs(name_image.str().replace(name_image.str().find(".txt"), 4, ".root").c_str());//+".png","png");
  // ca->SaveAs(name_image.str().replace(name_image.str().find(".txt"), 4, ".gif").c_str());//+".gif","gif");
  // ca->SaveAs(name_image.str().replace(name_image.str().find(".txt"), 4, ".pdf").c_str());//+".pdf","pdf");
  // ca->SaveAs(name_image.str().replace(name_image.str().find(".txt"), 4, ".ps", 0, 3).c_str()); //,+".ps","ps");
  // ca->SaveAs(name_image.str().replace(name_image.str().find(".txt"), 4, ".eps").c_str());//,+".eps","eps");



  //TString name_image="eff_EG20_2012_12fb";
  
  if (DEBUG) cout << __LINE__ << endl;

  // /////////////////////////////
  // // SAVE THE ROO FIT RESULT //
  // /////////////////////////////
  // TFile * lastoutputFile = new TFile(name_image.str().replace(name_image.str().find(".txt"), 4, "_fitres.root", 0,12).c_str(), "RECREATE"); //name_image+"_fitres.root", "RECREATE"); 
  // lastoutputFile->cd();
  // if (DEBUG) cout << __LINE__ << endl;
  
  // // cout << "old configuration, max eff  " << cb.value(100.) << endl;
  // // cout << "new configuration, max eff  " << cb2.value(100.) << endl;
  // // cout << "old configuration, 0.95 eff " << 0.95*(cb.value(100.)) << " at " << cb.find095Et(cb.value(100.)) << endl;
  // // cout << "new configuration, 0.95 eff " << 0.95*(cb2.value(100.))<< " at " << cb2.find095Et(cb2.value(100.)) << endl;


  
  // lastTree->Fill(); 
  // if (DEBUG) cout << __LINE__ << endl;

 
  // lastTree->Write();
  // if (DEBUG) cout << __LINE__ << endl;

  // RooWorkspace *w = new RooWorkspace("workspace","workspace") ;
  
  // if (DEBUG) cout << __LINE__ << endl;

  // for (int i=0; i< vRes.size(); i++){
  //   w->import(dataset[i]);
  //   w->import(*vRes[i],vRes[i]->GetName());
  // }

  // cout << "CREATES WORKSPACE : " << endl;
  // w->Print();
  // w->Write(); 
  // ca->Write(); 

  // lastoutputFile->Close();
  // if (DEBUG) cout << __LINE__ << endl;


  gSystem->Exit(0);
}



#endif

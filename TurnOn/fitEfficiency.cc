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
#include "TLatex.h"
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

void  fastEfficiencySilvia(unsigned int iEG, int iECAL1, int iColl1, int iECAL2, int iColl2, 
			   TString fileIn0, TString fileIn1, TString label0, TString label1,
			     string dirIn, TString lumi, int nCPU, int color1, int style1, int color2, int style2,
			     TString probe, TString tag);
void  fitManyFiles(unsigned int iEG, int iECAL1, int iColl1, int iECAL2, int iColl2, 
			     TString fileIn0, TString label0, TString label1,
			     string dirIn, TString lumi, int nCPU, int color1, int style1, int color2, int style2,
			     TString probe, TString tag);


void loadPresentationStyle();
string myjobid; 
stringstream mydir ; 

//-------
int main(int argc, char**argv){
  myjobid = argv[1];
  mydir.str("");
  mydir << myjobid <<"/"<< "selectPairsDir"; 

  ////TApplication app("App",&argc, argv);

   // fastEfficiencySilvia(20,0,1,0,1, "New_LC.root","Old_LC.root", 
   // 		       "New_LC", "Old_LC", mydir.str(), "19.7 fb^{-1}", 4, 
   //		       kBlack, kFullCircle, kRed, kOpenSquare, "WP80", "WP80");
   fastEfficiencySilvia(20,1,0,1,0, "effi_TagProbe_tree_2012D.root","effi_TagProbe_tree_2015D.root", 
   		       "2012", "2015", mydir.str(), "19.7 fb^{-1}", 4, 
   		       kBlack, kFullCircle, kRed, kOpenSquare, "WP80", "WP80");
   // fastEfficiencySilvia(20,0,0,0,0, "2015D_new_et14.root","effi_TagProbe_tree_2015D.root", 
   // 		       "2015D no HLT selection", "2015D HLT selection", mydir.str(), "19.7 fb^{-1}", 4, 
   // 		       kBlack, kFullCircle, kRed, kOpenSquare, "WP80", "WP80");

   ////app.Run(); 
  
  return 0;
}

void loadPresentationStyle(){
  gROOT->SetStyle("Plain");
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetTitleXOffset(1.5);
  gStyle->SetTitleYOffset(0.01);
  gStyle->SetLabelOffset(0.005, "XYZ");
  gStyle->SetTitleSize(0.07, "XYZ");
  gStyle->SetTitleFont(22,"X");
  gStyle->SetTitleFont(22,"Y");
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.15);
  gStyle->SetHistLineWidth(2);
// For the canvas:
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetCanvasDefH(600); //Height of canvas
  gStyle->SetCanvasDefW(600); //Width of canvas
  gStyle->SetCanvasDefX(0);   //POsition on screen
  gStyle->SetCanvasDefY(0);

// For the Pad:
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadColor(kWhite);
  gStyle->SetPadGridX(false);
  gStyle->SetPadGridY(false);
  gStyle->SetGridColor(0);
  gStyle->SetGridStyle(3);
  gStyle->SetGridWidth(1);

// For the frame:
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(1);
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameLineColor(1);
  gStyle->SetFrameLineStyle(1);
  gStyle->SetFrameLineWidth(1);

// For the histo:
  gStyle->SetHistLineColor(1);
  gStyle->SetHistLineStyle(0);
  gStyle->SetHistLineWidth(1);
  gStyle->SetEndErrorSize(2);
  gStyle->SetErrorX(0.);
  gStyle->SetMarkerStyle(20);

//For the fit/function:
  gStyle->SetOptFit(1);
  gStyle->SetFitFormat("5.4g");
  gStyle->SetFuncColor(2);
  gStyle->SetFuncStyle(1);
  gStyle->SetFuncWidth(1);

//For the date:
  gStyle->SetOptDate(0);

// For the statistics box:
  gStyle->SetOptFile(0);
  gStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  gStyle->SetStatColor(kWhite);
  gStyle->SetStatFont(42);
  gStyle->SetStatFontSize(0.025);
  gStyle->SetStatTextColor(1);
  gStyle->SetStatFormat("6.4g");
  gStyle->SetStatBorderSize(1);
  gStyle->SetStatH(0.1);
  gStyle->SetStatW(0.15);

// Margins:
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetPadLeftMargin(0.14);
  gStyle->SetPadRightMargin(0.04);

// For the Global title:
  gStyle->SetOptTitle(0);
  gStyle->SetTitleFont(42);
  gStyle->SetTitleColor(1);
  gStyle->SetTitleTextColor(1);
  gStyle->SetTitleFillColor(10);
  gStyle->SetTitleFontSize(0.05);

// For the axis titles:
  gStyle->SetTitleColor(1, "XYZ");
  gStyle->SetTitleFont(42, "XYZ");
  gStyle->SetTitleSize(0.06, "XYZ");
  //gStyle->SetTitleXOffset(1.7);
  gStyle->SetTitleYOffset(1.25);

// For the axis labels:
  gStyle->SetLabelColor(1, "XYZ");
  gStyle->SetLabelFont(42, "XYZ");
  gStyle->SetLabelOffset(0.007, "XYZ");
  gStyle->SetLabelSize(0.05, "XYZ");

// For the axis:
  gStyle->SetAxisColor(1, "XYZ");
  gStyle->SetStripDecimals(kTRUE);
  gStyle->SetTickLength(0.03, "XYZ");
  gStyle->SetNdivisions(510, "XYZ");
  gStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  gStyle->SetPadTickY(1);

// Change for log plots:
  gStyle->SetOptLogx(0);
  gStyle->SetOptLogy(0);
  gStyle->SetOptLogz(0);

// Postscript options:
  gStyle->SetPaperSize(20.,20.);
  //  setTDRStyle();
}

void fastEfficiencySilvia(unsigned int iEG, int iECAL1, int iColl1, int iECAL2, int iColl2, 
			  TString fileIn0="effi_TagProbe_tree_changed.root",TString fileIn1="effi_TagProbe_tree_changed.root", 
			  TString label0 = "FGVB ratio theshold 0.90", TString label1 = "FGVB ratio threshold 0.95",
			  string dirIn="result/"//=mydir.str()
			 , TString lumi="", int nCPU=4, 
			 int color1=kBlack, int style1=kFullCircle, int color2=kRed, int style2=kOpenSquare,
			 TString probe="WP80", TString tag="WP80"){
  // STYLE //
  if (DEBUG) cout << __LINE__ << endl;
  gROOT->Reset();
  gROOT->ForceStyle();
  //gROOT->LoadMacro("FuncCB.C+");
  // EG THRESHOLDS //
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
  dirResults <<  dirIn <<  "/turnons/EG"<<names[iEG]<<"/" ;
  stringstream createDir; 
  createDir.str(""); 
  if(DEBUG) createDir<< dirIn << "/turnons"; 
  if (dirExists(createDir.str().c_str())==false) {
    createDir.str("");
    createDir << "mkdir "<<  dirIn << "/turnons";
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
  name_image << dirResults.str() << "eff_EG"<<names[iEG]<<"_tag"<<tag<<"_probe"<<probe<<name_ecal[iECAL1]<<name_coll[iColl1]<<"_vs"<<name_ecal[iECAL2]<<name_coll[iColl2];// << "_DoubleEle2015" ;
  if (DEBUG) cout << __LINE__ << endl;

  // Output log //
  name_image << ".txt"; 
  ofstream fichier(name_image.str().c_str(), ios::out);


  // BINNING //
  const int nbins[nEG] = {29,29,29,29,21,21,21,22,22,21,22,21,22,18,19,18,18,18,18,20,23,20,20,19,20,20,20,20,21,21,
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
 // Double_t bins_20[20] = {10, 14,  18,  20,22, 26, 30, 35,  40, 45, 50, 60, 70, 100, 150, 200, 400, 600, 800, 1000}; // EG20
  Double_t bins_20[23] = {10, 14,  18,  20,21,22,24, 26,28, 30, 35,  40, 45, 50, 60, 70, 100, 150, 200, 400, 600, 800, 1000}; // EG20
  
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


  //INPUT DATA //
  stringstream filename ;
  filename.str(""); 
  filename << dirIn << "/" <<fileIn0; 
  TFile* f0 = TFile::Open(filename.str().c_str());
  filename.str(""); 
  filename << dirIn << "/" <<fileIn1;   
  TFile* f1 = TFile::Open(filename.str().c_str());

  TTree* treenew;
  TTree* treenew_2;

  treenew = (TTree*) f0->Get( "treenew"+name_ecal[iECAL1]+name_coll[iColl1] ) ;
  treenew_2 = (TTree*) f1->Get( "treenew"+name_ecal[iECAL2]+name_coll[iColl2] ) ;
  if (DEBUG) cout << __LINE__ << endl;

  TString name_scet[2], name_scdr[2], name_l1bin[2];
  name_scet[0] = "sc_et"+name_ecal[iECAL1]+name_coll[iColl1];
  name_scet[1] = "sc_et"+name_ecal[iECAL2]+name_coll[iColl2];
  name_scdr[0] = "sc_dr"+name_ecal[iECAL1]+name_coll[iColl1];
  name_scdr[1] = "sc_dr"+name_ecal[iECAL2]+name_coll[iColl2];
 
  name_l1bin[0] = "l1_"+names[iEG]+name_ecal[iECAL1]+name_coll[iColl1];
  name_l1bin[1] = "l1_"+names[iEG]+name_ecal[iECAL2]+name_coll[iColl2];
  cout << __LINE__ << " " << name_l1bin[0] << " " << name_l1bin[1] << endl;
  
  RooRealVar et_plot(name_scet[0],name_scet[0],0,1000) ;
  RooRealVar dr(name_scdr[0],name_scdr[0],0.5,1.5) ; 
  RooRealVar et_plot2(name_scet[1],name_scet[1],0,1000) ;
  RooRealVar dr2(name_scdr[1],name_scdr[1],0.5,1.5) ;
  if (DEBUG) cout << __LINE__ << endl;

  // Acceptance state cut (1 or 0)
  RooCategory cut(name_l1bin[0],name_l1bin[0]) ;
  cut.defineType("accept",1) ;
  cut.defineType("reject",0) ;
  RooCategory cut2(name_l1bin[1],name_l1bin[1]) ;
  cut2.defineType("accept",1) ;
  cut2.defineType("reject",0) ;
  
  // PARAMETRES ROOFIT CRYSTAL BALL
  RooRealVar norm("norm","N",1,0.6,1);
  RooRealVar alpha("alpha","#alpha",0.671034,0.01,8);
  RooRealVar n("n","n",4.07846,1.1,35);
  RooRealVar mean("mean","mean",20.8,0,100);
  mean.setVal(thres[iEG]);
  RooRealVar sigma("sigma","#sigma",0.972825,0.01,5);
  //RooRealVar pedestal("pedestal","pedestal",0.01,0,0.4);
  if (DEBUG) cout << __LINE__ << endl;

  RooRealVar norm2("norm2","N",0.999069,0.6,1);
  RooRealVar alpha2("alpha2","#alpha",0.492303,0.01,8);
  RooRealVar n2("n2","n",11.6694,1.1,35);
  RooRealVar mean2("mean2","mean",21.4582,0,100);
  mean2.setVal(thres[iEG]);
  RooRealVar sigma2("sigma2","#sigma",1.19,0.01,5);
  //RooRealVar pedestal2("pedestal2","pedestal",0.01,0,0.4);
  if (DEBUG) cout << __LINE__ << endl;

  FuncCB cb("cb","Crystal Ball Integree",et_plot,mean,sigma,alpha,n,norm) ;
  FuncCB cb2("cb2","Crystal Ball Integree",et_plot2,mean2,sigma2,alpha2,n2,norm2) ;
  if (DEBUG) cout << __LINE__ << endl;
  
  // EFFICIENCY //
  RooEfficiency eff("eff","efficiency",cb,cut,"accept");
  RooEfficiency eff2("eff2","efficiency",cb2,cut2,"accept");

  // DATASETS //
  if (DEBUG) cout << __LINE__ << endl;
  RooDataSet dataSet("data","data", RooArgSet(et_plot, cut, dr), Import(*treenew));//,Import(*treenew)); 
  if (DEBUG) cout << __LINE__ << endl;
  RooDataSet dataSet2("data2","data2", RooArgSet(et_plot2, cut2, dr2), Import(*treenew_2));//,Import(*treenew_2));
  if (DEBUG) cout << __LINE__ << endl;
  cout << __LINE__ << endl;
  dataSet.Print();
  dataSet2.Print();
  if (DEBUG) cout << __PRETTY_FUNCTION__ << " " << __LINE__ << endl;
  
  // PLOT //
  RooPlot* frame = et_plot.frame(Bins(18000),Title("Fitted efficiency")) ;
  RooPlot* frame2 = et_plot2.frame(Bins(18000),Title("Fitted efficiency")) ;

  dataSet.plotOn(frame, Binning(binning), Efficiency(cut), MarkerColor(color1), LineColor(color1), MarkerStyle(style1) );
  dataSet2.plotOn(frame2, Binning(binning), Efficiency(cut2), MarkerColor(color2), LineColor(color2), MarkerStyle(style2) );
  if (DEBUG) cout << __LINE__ << endl;


  /////////////////////// FITTING /////////////////////////////

  double fit_cuts_min = thres[iEG]-5.0;
  double fit_cuts_max = 150;
  if (DEBUG) cout << __LINE__ << endl;

  et_plot.setRange("interesting",fit_cuts_min,fit_cuts_max);
  et_plot2.setRange("interesting",fit_cuts_min,fit_cuts_max);

  RooFitResult* roofitres1 = new RooFitResult("roofitres1","roofitres1");
  RooFitResult* roofitres2 = new RooFitResult("roofitres2","roofitres2");
  if (DEBUG) cout << __LINE__ << endl;

  fichier << "Fit characteristics :"   << endl ;
  fichier << "EG "     << names[iEG] << endl ;
  fichier << "Fit Range , EB Coll : [" << fit_cuts_min << "," << fit_cuts_max << "]" << endl ;
  fichier << "Fit Range , EE Coll : [" << fit_cuts_min << "," << fit_cuts_max << "]" << endl ;
  fichier << "----------------------"  << endl ;
  if (DEBUG) cout << __LINE__ << endl;

  // Fit #1 //
  roofitres1 = eff.fitTo(dataSet,ConditionalObservables(et_plot),Range("interesting"),Minos(kFALSE),Warnings(kFALSE),NumCPU(nCPU),Save(kTRUE));
  if (DEBUG) cout << __LINE__ << endl;

  cb.plotOn(frame,LineColor(color1),LineWidth(2));

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

  // Fit #2 //
  roofitres2 = eff2.fitTo(dataSet2,ConditionalObservables(et_plot2),Range("interesting"),Minos(kFALSE),Warnings(kFALSE),NumCPU(nCPU),Save(kTRUE));
 
  cb2.plotOn(frame2,LineColor(color2),LineWidth(2));

  double res_norm2  = norm2.getVal();
  double err_norm2  = norm2.getErrorLo();
  double res_mean2  = mean2.getVal();
  double err_mean2  = mean2.getError();
  double res_sigma2 = sigma2.getVal();
  double err_sigma2 = sigma2.getError();
  double res_n2     = n2.getVal();
  double err_n2     = n2.getError();
  double res_alpha2 = alpha2.getVal();
  double err_alpha2 = alpha2.getError();

  fichier << "<----------------- EE ----------------->" << endl
  	  << "double res_mean="  << res_mean2  << "; "
  	  << "double res_sigma=" << res_sigma2 << "; "
  	  << "double res_alpha=" << res_alpha2 << "; "
  	  << "double res_n="     << res_n2     << "; "
  	  << "double res_norm="  << res_norm2  << "; "
  	  << endl
  	  << "double err_mean="  << err_mean2  << "; "
  	  << "double err_sigma=" << err_sigma2 << "; "
  	  << "double err_alpha=" << err_alpha2 << "; "
  	  << "double err_n="     << err_n2     << "; "
  	  << "double err_norm="  << err_norm2  << "; "
  	  << endl;
    

  ////////////////////////////  DRAWING PLOTS AND LEGENDS /////////////////////////////////
  loadPresentationStyle();  
  TCanvas* ca = new TCanvas("ca","Trigger Efficiency") ;

  ca->SetGridx();
  ca->SetGridy();
  ca->cd();
  
  gPad->SetLogx();
  gPad->SetObjectStat(1);

  gROOT->ForceStyle();

  frame->GetYaxis()->SetRangeUser(0,1.05);
  frame->GetXaxis()->SetRangeUser(5,100.);// set to 1000 to see the plateau
  frame->GetYaxis()->SetTitle("Efficiency");
  frame->GetXaxis()->SetTitle("E_{T} [GeV]");
  frame->GetXaxis()->SetTitleOffset(1.3);
  
  frame->Draw() ;

  frame2->GetYaxis()->SetRangeUser(0,1.05);
  frame2->GetXaxis()->SetRangeUser(1,100.);// set to 1000 to see the plateau
  frame2->GetYaxis()->SetTitle("Efficiency");
  frame2->GetXaxis()->SetTitle("E_{T} [GeV]");
  frame2->Draw("same") ;
  TLatex *text = new TLatex(5,1.06,"CMS Preliminary");
  text->SetTextSize(0.035);
  text->SetTextFont(42);
  text->Draw("same");
  TH1F *SCeta1 = new TH1F("SCeta1","SCeta1",50,-2.5,2.5);
  TH1F *SCeta2 = new TH1F("SCeta2","SCeta2",50,-2.5,2.5);

  SCeta1->SetLineColor(color1) ;
  SCeta1->SetMarkerColor(color1);
  SCeta1->SetMarkerStyle(style1);

  SCeta2->SetLineColor(color2) ;
  SCeta2->SetMarkerColor(color2);
  SCeta2->SetMarkerStyle(style2);

  //TLegend *leg = new TLegend(0.2,0.435,0.6,0.560,NULL,"brNDC"); // mid : x=353.5
  TLegend *leg = new TLegend(0.17,0.435,0.450,0.560,NULL,"brNDC");
  leg->SetLineColor(1);
  leg->SetTextColor(1);
  leg->SetTextFont(42);
  leg->SetTextSize(0.025);
  leg->SetShadowColor(kWhite);
  leg->SetFillColor(kWhite);
  leg->SetMargin(0.2);
  leg->AddEntry("", "Z #rightarrow e^{+} e^{-}", "");
  if (iECAL1==0)
      leg->AddEntry("", "ECAL Barrel", "");
  if (iECAL1==1)
    leg->AddEntry("", "ECAL Endcap", "");
  leg->AddEntry(SCeta1,label0+",#sqrt{s}=8 TeV","p");
  leg->AddEntry(SCeta2,label1+",#sqrt{s}= 13 TeV","p");
  //leg->AddEntry(SCeta1,label0,"p");
  //leg->AddEntry(SCeta2,label1,"p");
  
  leg->Draw();

  TLegend * leg2 = new TLegend(0.16,0.725,0.58,0.905,NULL,"brNDC");
  leg2->SetBorderSize(0);
  leg2->SetTextFont(62);
  leg2->SetTextSize(0.03);
  leg2->SetLineColor(0);
  leg2->SetLineStyle(1);
  leg2->SetLineWidth(1);
  leg2->SetFillColor(0);
  leg2->SetFillStyle(0);
  // leg->AddEntry("NULL","#int L dt = "+lumi+"^{-1}","h");
  //leg2->AddEntry("NULL","L1 trigger : "+" EG"+names[iEG],"h");
  leg2->AddEntry("NULL","L1 Trigger EG"+names[iEG],"h");
  leg2->Draw();

//  TPaveText *pt2 = new TPaveText(0.220,0.605,0.487,0.685,"brNDC"); // mid : x=353.5                                         
  TPaveText *pt2 = new TPaveText(0.170,0.605,0.437,0.685,"brNDC"); 
  pt2->SetLineColor(1);
  pt2->SetTextColor(1);
  pt2->SetTextFont(42);
  pt2->SetTextSize(0.03);
  pt2->SetFillColor(kWhite);
  pt2->SetShadowColor(kWhite);
  pt2->AddText("L1 E/Gamma Trigger");
  pt2->AddText("Electrons from Z");
  pt2->Draw();
  
  //TString name_image="eff_EG20_2012_12fb";

  ca->Print(name_image.str().replace(name_image.str().find(".txt"), 4, ".cxx").c_str());
  ca->Print(name_image.str().replace(name_image.str().find(".txt"), 4, ".root").c_str());
  ca->Print(name_image.str().replace(name_image.str().find(".txt"), 4, ".png").c_str());//+".png","png");
  ca->Print(name_image.str().replace(name_image.str().find(".txt"), 4, ".gif").c_str());//+".gif","gif");
  ca->Print(name_image.str().replace(name_image.str().find(".txt"), 4, ".pdf").c_str());//+".pdf","pdf");
  ca->Print(name_image.str().replace(name_image.str().find(".txt"), 4, ".ps", 0, 3).c_str()); //,+".ps","ps");
  ca->Print(name_image.str().replace(name_image.str().find(".txt"), 4, ".eps").c_str());//,+".eps","eps");

  /////////////////////////////
  // SAVE THE ROO FIT RESULT //
  /////////////////////////////
  TFile * lastoutputFile = new TFile(name_image.str().replace(name_image.str().find(".txt"), 4, "_fitres.root", 0,12).c_str(), "RECREATE"); //name_image+"_fitres.root", "RECREATE"); 
  lastoutputFile->cd();
  cout << "old configuration, max eff  " << cb.value(100.) << endl;
  cout << "new configuration, max eff  " << cb2.value(100.) << endl;
  cout << "old configuration, 0.95 eff " << 0.95*(cb.value(100.)) << " at " << cb.find095Et(cb.value(100.)) << endl;
  cout << "new configuration, 0.95 eff " << 0.95*(cb2.value(100.))<< " at " << cb2.find095Et(cb2.value(100.)) << endl;

  double oldMaxEff, old95Eff, old95Et, newMaxEff, new95Eff, new95Et;
  oldMaxEff= cb.value(100.);
  newMaxEff= cb2.value(100.);
  old95Et  = cb.find095Et(cb.value(100.));
  new95Et  = cb2.find095Et(cb2.value(100.));
  old95Eff = cb.value(old95Et);
  new95Eff = cb2.value(new95Et);
  
  TTree * lastTree = new TTree("Efficiency","Efficiency");
  lastTree->Branch("oldMaxEff",&oldMaxEff,"oldMaxEff/D");
  lastTree->Branch("old95Eff" ,&old95Eff ,"old95Eff/D" );
  lastTree->Branch("old95Et"  ,&old95Et  ,"old95Et/D"  );
  lastTree->Branch("newMaxEff",&newMaxEff,"newMaxEff/D");
  lastTree->Branch("new95Eff" ,&new95Eff ,"new95Eff/D" );
  lastTree->Branch("new95Et"  ,&new95Et  ,"new95Et/D"  );

  lastTree->Fill();
  lastTree->Write();
  
  RooWorkspace *w = new RooWorkspace("workspace","workspace") ;

  w->import(dataSet);
  w->import(dataSet2);
  
  w->import(*roofitres1,"roofitres1");
  w->import(*roofitres2,"roofitres2");

  cout << "CREATES WORKSPACE : " << endl;
  w->Print();
  w->Write(); 
  ca->Write(); 

  lastoutputFile->Close();
  f1->Close();

  gSystem->Exit(0);
}

void fitManyFiles(unsigned int iEG, int iECAL1, int iColl1, int iECAL2, int iColl2, 
			  TString fileIn0="effi_TagProbe_tree_*.root",
			  TString label0 = "FGVB ratio theshold 0.90", TString label1 = "FGVB ratio threshold 0.95",
			  string dirIn=mydir.str()
			 , TString lumi="", int nCPU=4, 
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
  dirResults <<  dirIn <<  "/turnons/EG"<<names[iEG]<<"/" ;
  stringstream createDir; 
  createDir.str(""); 
  if(DEBUG) createDir<< dirIn << "/turnons"; 
  if (dirExists(createDir.str().c_str())==false) {
    createDir.str("");
    createDir << "mkdir "<<  dirIn << "/turnons";
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
 // Double_t bins_20[20] = {12, 14,  18,  20,  22, 26, 30, 35,  40, 45, 50, 60, 70, 100, 150, 200, 400, 600, 800, 1000}; // EG20
  Double_t bins_20[20] = {14, 16,  18,  20,  22, 26, 30, 35,  40, 45, 50, 60, 70, 100, 150, 200, 400, 600, 800, 1000}; // EG20
  
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

//  TLegend *leg = new TLegend(0.2,0.435,0.6,0.560,NULL,"brNDC"); // mid : x=353.5
  TLegend *leg = new TLegend(0.17,0.435,0.450,0.560,NULL,"brNDC"); // mid : x=353.5
  leg->SetLineColor(1);
  leg->SetTextColor(1);
  leg->SetTextFont(42);
  leg->SetTextSize(0.025);
  leg->SetShadowColor(kWhite);
  leg->SetFillColor(kWhite);
  leg->SetMargin(0.2);

  TLegend *leg2 = new TLegend(0.16,0.725,0.58,0.905,NULL,"brNDC");
 // TLegend *leg2 = new TLegend(0.10,0.725,0.58,0.905,NULL,"brNDC");
  leg2->SetBorderSize(0);
  leg2->SetTextFont(62);
  leg2->SetTextSize(0.03);
  leg2->SetLineColor(0);
  leg2->SetLineStyle(1);
  leg2->SetLineWidth(1);
  leg2->SetFillColor(0);
  leg2->SetFillStyle(0);
  leg2->AddEntry("NULL","Threshold : "+names[iEG]+" GeV","h");

    
 // TPaveText *pt2 = new TPaveText(0.220,0.605,0.487,0.685,"brNDC"); // mid : x=353.5                                          
  TPaveText *pt2 = new TPaveText(0.170,0.605,0.437,0.685,"brNDC"); // mid : x=353.5                                          
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
  filename.str(""); 
  filename << dirIn << "/" <<fileIn0; 

  fileNameVector= globVector(filename.str().c_str());
  cout << "number of files: " << fileNameVector.size()<< endl; 

  for (int i = 0 ; i < fileNameVector.size() ; i++){
    
    TFile* f0 = TFile::Open(fileNameVector[i].c_str());
    //  filename.str(""); 
    //filename << dirIn << "/" <<fileIn1;   
    //TFile* f1 = TFile::Open(filename.str().c_str());

    TTree* treenew;
    TTree* treenew_2;
//******FANBO********
    if (i!=0)
    {
   //  iColl1=iColl1+1;
   //  iColl2=iColl2+1;
     iColl1=iColl1-1;
     iColl2=iColl2-1;
      
    }    
    
    treenew = (TTree*) f0->Get( "treenew"+name_ecal[iECAL1]+name_coll[iColl1] ) ;
    treenew_2 = (TTree*) f0->Get( "treenew"+name_ecal[iECAL2]+name_coll[iColl2] ) ;
    if (DEBUG) cout << __LINE__ << endl;
    TString name_scet[2], name_scdr[2], name_l1bin[2];
    name_scet[0] = "sc_et"+name_ecal[iECAL1]+name_coll[iColl1];
    name_scet[1] = "sc_et"+name_ecal[iECAL2]+name_coll[iColl2];
    name_scdr[0] = "sc_dr"+name_ecal[iECAL1]+name_coll[iColl1];
    name_scdr[1] = "sc_dr"+name_ecal[iECAL2]+name_coll[iColl2];
    
    name_l1bin[0] = "l1_"+names[iEG]+name_ecal[iECAL1]+name_coll[iColl1];
    name_l1bin[1] = "l1_"+names[iEG]+name_ecal[iECAL2]+name_coll[iColl2];
    //cout << __LINE__ << " " << name_l1bin[0] << " " << name_l1bin[1] << endl;
    
    RooRealVar et_plot(name_scet[0],name_scet[0],0,1000) ;
    RooRealVar dr(name_scdr[0],name_scdr[0],0.5,1.5) ; 
    RooRealVar et_plot2(name_scet[1],name_scet[1],0,1000) ;
    RooRealVar dr2(name_scdr[1],name_scdr[1],0.5,1.5) ;
    if (DEBUG) cout << __LINE__ << endl;
    
    // Acceptance state cut (1 or 0)
    RooCategory cut(name_l1bin[0],name_l1bin[0]) ;
    cut.defineType("accept",1) ;
    cut.defineType("reject",0) ;
    RooCategory cut2(name_l1bin[1],name_l1bin[1]) ;
    cut2.defineType("accept",1) ;
    cut2.defineType("reject",0) ;
  
  // PARAMETRES ROOFIT CRYSTAL BALL
    RooRealVar norm("norm","N",1,0.6,1);
    RooRealVar alpha("alpha","#alpha",0.671034,0.01,8);
    RooRealVar n("n","n",4.07846,1.1,35);
    RooRealVar mean("mean","mean",20.8,0,100);
    mean.setVal(thres[iEG]);
    RooRealVar sigma("sigma","#sigma",0.972825,0.01,5);
    //RooRealVar pedestal("pedestal","pedestal",0.01,0,0.4);
    if (DEBUG) cout << __LINE__ << endl;
    
     RooRealVar norm2("norm2","N",0.999069,0.6,1);
     RooRealVar alpha2("alpha2","#alpha",0.492303,0.01,8);
     RooRealVar n2("n2","n",11.6694,1.1,35);
     RooRealVar mean2("mean2","mean",21.4582,0,100);
     mean2.setVal(thres[iEG]);
     RooRealVar sigma2("sigma2","#sigma",1.19,0.01,5);
    // //RooRealVar pedestal2("pedestal2","pedestal",0.01,0,0.4);
    // if (DEBUG) cout << __LINE__ << endl;

    FuncCB cb("cb","Crystal Ball Integree",et_plot,mean,sigma,alpha,n,norm) ;
    FuncCB cb2("cb2","Crystal Ball Integree",et_plot2,mean2,sigma2,alpha2,n2,norm2) ;
    if (DEBUG) cout << __LINE__ << endl;
  
    // EFFICIENCY //
    RooEfficiency eff("eff","efficiency",cb,cut,"accept");
    RooEfficiency eff2("eff2","efficiency",cb2,cut2,"accept");
    
    // DATASETS //
    if (DEBUG) cout << __LINE__ << endl;
    stringstream dataname; 
    dataname.str(""); 
    dataname << "data"<< i;
    RooDataSet dataSet(dataname.str().c_str(),dataname.str().c_str(), RooArgSet(et_plot, cut, dr), Import(*treenew));//,Import(*treenew)); 
    dataset.push_back(dataSet);
    // if (DEBUG) cout << __LINE__ << endl;
    dataname.str(""); 
    dataname << "data"<< i+2;
    RooDataSet dataSet2(dataname.str().c_str(),dataname.str().c_str(),RooArgSet(et_plot2, cut2, dr2), Import(*treenew_2));//,Import(*treenew_2));
    dataset.push_back(dataSet2);
    if (DEBUG) cout << __LINE__ << endl;
    cout << __LINE__ << endl;
    dataSet.Print();
    dataSet2.Print();
    if (DEBUG) cout << __PRETTY_FUNCTION__ << " " << __LINE__ << endl;
  
    // PLOT //
    
    RooPlot* frame = et_plot.frame(Bins(18000),Title("ECAL Turn-On")) ;
    dataname.str(""); 
    dataname << "frame"<< i;
    
    frame->SetName(dataname.str().c_str()); 
    RooPlot* frame2 = et_plot2.frame(Bins(18000),Title("ECAL Turn-On")) ;
    dataname.str(""); 
    dataname << "frame2"<< i;
    
    frame2->SetName(dataname.str().c_str()); 

    dataSet.plotOn(frame, Binning(binning), Efficiency(cut), MarkerColor(i+1), LineColor(i+1), MarkerStyle(20+i) );
    dataSet2.plotOn(frame2, Binning(binning), Efficiency(cut2), MarkerColor(i+3), LineColor(i+3), MarkerStyle(20+i) );
    if (DEBUG) cout << __LINE__ << endl;
    
    
    /////////////////////// FITTING /////////////////////////////
    
    //double fit_cuts_min = thres[iEG]-1.5 ;
    double fit_cuts_min = thres[iEG]-5.0 ;
    double fit_cuts_max = 150;
    if (DEBUG) cout << __LINE__ << endl;
    
    et_plot.setRange("interesting",fit_cuts_min,fit_cuts_max);
    et_plot2.setRange("interesting",fit_cuts_min,fit_cuts_max);
    stringstream fitres; 
    fitres.str(""); 
    fitres << "roofitres" << i ; 

    RooFitResult* roofitres1 = new RooFitResult(fitres.str().c_str(),fitres.str().c_str());
    fitres.str(""); 
    fitres << "roofitres2" << i ; 
    RooFitResult* roofitres2 = new RooFitResult(fitres.str().c_str(),fitres.str().c_str());
    if (DEBUG) cout << __LINE__ << endl;

    fichier << "Fit characteristics :"   << endl ;
    fichier << "EG "     << names[iEG] << endl ;
    fichier << "Fit Range , EB Coll : [" << fit_cuts_min << "," << fit_cuts_max << "]" << endl ;
    fichier << "Fit Range , EE Coll : [" << fit_cuts_min << "," << fit_cuts_max << "]" << endl ;
    fichier << "----------------------"  << endl ;
    if (DEBUG) cout << __LINE__ << endl;
  
    // Fit #1 //
    roofitres1 = eff.fitTo(dataSet,ConditionalObservables(et_plot),Range("interesting"),Minos(kFALSE),Warnings(kFALSE),NumCPU(nCPU),Save(kTRUE));
    if (DEBUG) cout << __LINE__ << endl;
    vRes.push_back(roofitres1);

   if (DEBUG) cout << __LINE__ << endl;
     cb.plotOn(frame,LineColor(i+1),LineWidth(2));
    
    if (DEBUG) cout << __LINE__ << endl;
    name_.push_back(fileNameVector[i].substr(fileNameVector[i].find_last_of('/')+1,fileNameVector[i].substr(fileNameVector[i].find_last_of('/')).length()-5));
    maxEff_.push_back(cb.value(100.));
    et_95Eff_.push_back(cb.find095Et(cb.value(100.)));
    cout << "FIT results " << fileNameVector[i].substr(fileNameVector[i].find_last_of('/')+1,fileNameVector[i].substr(fileNameVector[i].find_last_of('/')).length()-6) << " " << cb.value(100.)<< " " << cb.find095Et(cb.value(100.)) << endl;

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
   // Fit 2
//    roofitres1 = eff.fitTo(dataSet,ConditionalObservables(et_plot),Range("interesting"),Minos(kFALSE),Warnings(kFALSE),NumCPU(nCPU),Save(kTRUE));
    roofitres2 = eff2.fitTo(dataSet2,ConditionalObservables(et_plot2),Range("interesting"),Minos(kFALSE),Warnings(kFALSE),NumCPU(nCPU),Save(kTRUE));
    vRes.push_back(roofitres2);
    cb2.plotOn(frame2,LineColor(i+3),LineWidth(2));
    double res_norm2  = norm2.getVal();
    double err_norm2  = norm2.getErrorLo();
    double res_mean2  = mean2.getVal();
    double err_mean2  = mean2.getError();
    double res_sigma2 = sigma2.getVal();
    double err_sigma2 = sigma2.getError();
    double res_n2     = n2.getVal();
    double err_n2     = n2.getError();
    double res_alpha2 = alpha2.getVal();
    double err_alpha2 = alpha2.getError();
  
    fichier << "<----------------- EE ----------------->" << endl
            << "double res_mean="  << res_mean2  << "; "
            << "double res_sigma=" << res_sigma2 << "; "
            << "double res_alpha=" << res_alpha2 << "; "
            << "double res_n="     << res_n2     << "; "
            << "double res_norm="  << res_norm2  << "; "
            << endl
            << "double err_mean="  << err_mean2  << "; "
            << "double err_sigma=" << err_sigma2 << "; "
            << "double err_alpha=" << err_alpha2 << "; "
            << "double err_n="     << err_n2     << "; "
            << "double err_norm="  << err_norm2  << "; "
            << endl;

    ////////////////////////////  DRAWING PLOTS AND LEGENDS /////////////////////////////////
    ca->cd();
    
    gPad->SetLogx();
    gPad->SetObjectStat(1);
    if (DEBUG) cout << __LINE__ << endl;
   
    
    frame->GetYaxis()->SetRangeUser(0,1.05);
    frame->GetXaxis()->SetRangeUser(5,100.);// set to 1000 to see the plateau
    frame->GetYaxis()->SetTitle("Efficiency");
    frame->GetXaxis()->SetTitle("E_{T} [GeV]");
    vFrame.push_back(frame); 
  //  frame->Draw();
    frame2->GetYaxis()->SetRangeUser(0,1.05);
    frame2->GetXaxis()->SetRangeUser(1,100.);// set to 1000 to see the plateau
    frame2->GetYaxis()->SetTitle("Efficiency");
    frame2->GetXaxis()->SetTitle("E_{T} [GeV]");
  //  frame2->Draw("SAME");
    vFrame.push_back(frame2); 
    //vFrame.push_back(frame); 
    if (i==0){
      vFrame[i]->Draw() ;
      vFrame[i+1]->Draw("SAME") ;
      }
    else{
      vFrame[2*i]->Draw("SAME"); 
      vFrame[2*i+1]->Draw("SAME"); 
    }
    if (DEBUG) cout << __LINE__ << endl;
   
    stringstream histoname; 
    histoname.str(""); 
    histoname<< "SCeta" << i; 
    if (DEBUG) cout << __LINE__ << endl;

    TH1F *SCeta1 = new TH1F(histoname.str().c_str(), histoname.str().c_str(),50,-2.5,2.5);
    SCeta1->SetLineColor(i+1) ;
    SCeta1->SetMarkerColor(i+1);
    SCeta1->SetMarkerStyle(i+20);
    
    if (DEBUG) cout << __LINE__ << endl;
    std::cout<<"-------------File  Name---------------"<<fileNameVector[i].substr(fileNameVector[i].find_last_of('/')+1,fileNameVector[i].substr(fileNameVector[i].find_last_of('/')).length()-6)<<std::endl;
    string title1=fileNameVector[i].substr(fileNameVector[i].find_last_of('/')+1,fileNameVector[i].substr(fileNameVector[i].find_last_of('/')).length()-6)+"_EB";
//    SCeta1->SetTitle(fileNameVector[i].substr(fileNameVector[i].find_last_of('/')+1,fileNameVector[i].substr(fileNameVector[i].find_last_of('/')).length()-6).c_str());
    SCeta1->SetTitle(title1.c_str()); 
    vHisto.push_back(SCeta1); 
    histoname.str(""); 
    histoname<< "SCeta2" << i; 
    if (DEBUG) cout << __LINE__ << endl;

    TH1F *SCeta2 = new TH1F(histoname.str().c_str(), histoname.str().c_str(),50,-2.5,2.5);
    SCeta2->SetLineColor(i+3) ;
    SCeta2->SetMarkerColor(i+3);
    SCeta2->SetMarkerStyle(i+20);
    string title2=fileNameVector[i].substr(fileNameVector[i].find_last_of('/')+1,fileNameVector[i].substr(fileNameVector[i].find_last_of('/')).length()-6)+"_EE";
//    SCeta2->SetTitle(fileNameVector[i].substr(fileNameVector[i].find_last_of('/')+1,fileNameVector[i].substr(fileNameVector[i].find_last_of('/')).length()-6).c_str()+"EE"); 
    SCeta2->SetTitle(title2.c_str());
    vHisto.push_back(SCeta2); 

    if (DEBUG) cout << __LINE__ << endl;
    // if (i==fileNameVector.size()-1) {
    //   ca->SaveAs(name_image.str().replace(name_image.str().find(".txt"), 4, ".cxx").c_str());
    //   if (DEBUG) cout << __LINE__ << endl;
    //   ca->SaveAs(name_image.str().replace(name_image.str().find(".txt"), 4, ".png").c_str());//+".png","png");
    //   ca->SaveAs(name_image.str().replace(name_image.str().find(".txt"), 4, ".gif").c_str());//+".gif","gif");
    //   ca->SaveAs(name_image.str().replace(name_image.str().find(".txt"), 4, ".pdf").c_str());//+".pdf","pdf");
    //   ca->SaveAs(name_image.str().replace(name_image.str().find(".txt"), 4, ".ps", 0, 3).c_str()); //,+".ps","ps");
    //   ca->SaveAs(name_image.str().replace(name_image.str().find(".txt"), 4, ".eps").c_str());//,+".eps","eps");

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
 
  ca->SaveAs(name_image.str().replace(name_image.str().find(".txt"), 4, ".cxx").c_str());
  if (DEBUG) cout << __LINE__ << endl;
  ca->SaveAs(name_image.str().replace(name_image.str().find(".txt"), 4, ".png").c_str());//+".png","png");
  ca->SaveAs(name_image.str().replace(name_image.str().find(".txt"), 4, ".gif").c_str());//+".gif","gif");
  ca->SaveAs(name_image.str().replace(name_image.str().find(".txt"), 4, ".pdf").c_str());//+".pdf","pdf");
  ca->SaveAs(name_image.str().replace(name_image.str().find(".txt"), 5, ".root").c_str());//+".pdf","pdf");
  ca->SaveAs(name_image.str().replace(name_image.str().find(".txt"), 4, ".ps", 0, 3).c_str()); //,+".ps","ps");
  ca->SaveAs(name_image.str().replace(name_image.str().find(".txt"), 4, ".eps").c_str());//,+".eps","eps");

  ca->Write(); 


  //TString name_image="eff_EG20_2012_12fb";
  
/*  if (DEBUG) cout << __LINE__ << endl;

  /////////////////////////////
  // SAVE THE ROO FIT RESULT //
  /////////////////////////////
  TFile * lastoutputFile = new TFile(name_image.str().replace(name_image.str().find(".txt"), 4, "_fitres.root", 0,12).c_str(), "RECREATE"); //name_image+"_fitres.root", "RECREATE"); 
  lastoutputFile->cd();
  if (DEBUG) cout << __LINE__ << endl;
  
  // cout << "old configuration, max eff  " << cb.value(100.) << endl;
  // cout << "new configuration, max eff  " << cb2.value(100.) << endl;
  // cout << "old configuration, 0.95 eff " << 0.95*(cb.value(100.)) << " at " << cb.find095Et(cb.value(100.)) << endl;
  // cout << "new configuration, 0.95 eff " << 0.95*(cb2.value(100.))<< " at " << cb2.find095Et(cb2.value(100.)) << endl;


  
  lastTree->Fill(); 
  if (DEBUG) cout << __LINE__ << endl;

 
  lastTree->Write();
  if (DEBUG) cout << __LINE__ << endl;

  RooWorkspace *w = new RooWorkspace("workspace","workspace") ;
  
  if (DEBUG) cout << __LINE__ << endl;

  for (int i=0; i< vRes.size(); i++){
    w->import(dataset[i]);
    w->import(*vRes[i],vRes[i]->GetName());
  }

  cout << "CREATES WORKSPACE : " << endl;
  w->Print();
  w->Write(); 
  ca->Write(); 

  lastoutputFile->Close();
  if (DEBUG) cout << __LINE__ << endl;
*/

  gSystem->Exit(0);
}



#endif

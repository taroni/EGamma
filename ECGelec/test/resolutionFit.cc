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
#include "RooFFTConvPdf.h"
#include "RooPlot.h"
#include "RooRealVar.h"
#include "RooCategory.h"
#include "RooEfficiency.h"
#include "RooDataSet.h"
#include "RooBinning.h"
#include "RooHist.h"
#include "RooWorkspace.h"
#include "RooCBShape.h"
#include "RooAddPdf.h"
//#include "crytalball.h"
#include "RooGaussian.h"
#include "RooDataHist.h"
#include "RooBreitWigner.h"
// Root headers
#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TH2F.h>
#include <TF2.h>
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
#include "RooCruijff.h"

using namespace RooFit ;
void mergeHigto(TH1F *Hist1,TH1F *Hist2,TH1F *Hist3);
void Fitresolution(TString fileIn0,int IECAL);
void normalize(TH1F * Hist);
void loadPresentationStyle();
int main(){

  //Fitresolution("2015D_jason_default_reso.root",1);
  //Fitresolution("2012D.root",1);
  Fitresolution("2015D_new_et14.root",1);
return 0;
}
Double_t crystalball_function(Double_t *x, Double_t *par){
    double xx=-x[0];
    double alpha=par[0];
    double n=par[1];
    double sigma=par[3];
    double mean=par[2];
      // evaluate the crystal ball function
      if (sigma < 0.)     return 0.;
      double z = (xx - mean)/sigma; 
      if (alpha < 0) z = -z; 
      double abs_alpha = std::abs(alpha);
      // double C = n/abs_alpha * 1./(n-1.) * std::exp(-alpha*alpha/2.);
      // double D = std::sqrt(M_PI/2.)*(1.+ROOT::Math::erf(abs_alpha/std::sqrt(2.)));
      // double N = 1./(sigma*(C+D));
      if (z  > - abs_alpha)
         return std::exp(- 0.5 * z * z);
      else {
         //double A = std::pow(n/abs_alpha,n) * std::exp(-0.5*abs_alpha*abs_alpha);
         double nDivAlpha = n/abs_alpha;
         double AA =  std::exp(-0.5*abs_alpha*abs_alpha);
         double B = nDivAlpha -abs_alpha;
         double arg = nDivAlpha/(B-z);
         return AA * std::pow(arg,n);
      }
   }


void Fitresolution(TString fileIn0,int IECAL){

   TFile *f =TFile::Open(fileIn0);
   TTree *t1 = (TTree*)f->Get("resolution_plot");
   Float_t resoTagEB,resoTagEE,resoProbeEB,resoProbeEE,etabinreso[4];
   t1->SetBranchAddress("resolution_ProbeEE",&resoProbeEE);  
   t1->SetBranchAddress("resolution_TagEE",&resoTagEE);  
   t1->SetBranchAddress("resolution_ProbeEB",&resoProbeEB);  
   t1->SetBranchAddress("resolution_TagEB",&resoTagEB);  
   t1->SetBranchAddress("resolution_etabin",etabinreso);  
   loadPresentationStyle();
   Long64_t nentries = t1->GetEntries();
   TH1F *etaResolution   = new TH1F("etaResolution","etaResolution",100,-2.5,2.5);
   TH1F *ProbeEE   = new TH1F("probeEE","probeEE ",200,-1.,1.);

   TH1F *TagEE   = new TH1F("TagEE","TagEE ",200,-1.,1.);
   TH1F *TotalEE   = new TH1F("TotalEE","TotalEE ",200,-1.,1.);
   TH1F *ProbeEB   = new TH1F("probeEB","probeEB ",400,-1.,1.);
   TH1F *TagEB   = new TH1F("TagEB","TagEB ",400,-1.,1.);
   TH1F *TotalEB   = new TH1F("TotalEB","TotalEB",400,-1.,1.);
   TH1F *Distribution   = new TH1F("TotalEB_Distribution","TotalEB",100,-2.5,2.5);

   TH2F *Etabinreso   = new TH2F("Etabin","Etabin_",50,-2.5,2.5,50,-1,1);
   std::cout<<"the number of total events "<<nentries<<std::endl;
   for(int i=0;i<nentries;i++){
      t1->GetEntry(i);
      if (i/1000. == int(i/1000.)) std::cout << "analyzing " << i << "th entry of "<< nentries << std::endl;
     if(fabs(etabinreso[2])>=1.566 && fabs(etabinreso[2])<=2.5)
         ProbeEE->Fill(resoProbeEE);
     if(fabs(etabinreso[0])>=1.566 && fabs(etabinreso[0])<=2.5)
         TagEE->Fill(resoTagEE);

      ProbeEB->Fill(resoProbeEB);
      TagEB->Fill(resoTagEB);
      Etabinreso->Fill(etabinreso[0],etabinreso[1]);
      Etabinreso->Fill(etabinreso[2],etabinreso[3]);
 
      if((etabinreso[1]>=0.3)&&(etabinreso[1]<=0.5))
        Distribution->Fill(etabinreso[0]);
      if((etabinreso[3]>=0.3)&&(etabinreso[3]<=0.5))
        Distribution->Fill(etabinreso[2]);
   }
   mergeHigto(ProbeEE,TagEE,TotalEE);
   mergeHigto(ProbeEB,TagEB,TotalEB);
   TotalEB->Sumw2();
   TotalEE->Sumw2();
   normalize(TotalEE);
   normalize(TotalEB);
 
   TFile * outfile = new TFile ("outfile2015.root", "RECREATE");
   outfile->cd();
   TotalEB->Write();
   TotalEE->Write();
   outfile->Close();
   std::cout << "file closed"<< std::endl;
   // TF1 *funcCB = new TF1("funCB",crystalball_function,-1,1,4); 

   // //funcCB->SetParameter(4,1.);  //normalization
   // funcCB->SetParameter(2,0.04);   ///mean
   // funcCB->SetParameter(3,0.01);     //sigma
   // funcCB->SetParameter(0,0.2); //alpha    
   // funcCB->SetParameter(1,5.);  ///n 
   // //funcCB->SetParLimits(4,0.5, 1.5);  
   // funcCB->SetParLimits(2,-0.1,0.1);   
   // funcCB->SetParLimits(3,0.0,0.5);     
   // funcCB->SetParLimits(0,-0.5,0.5);     
   // funcCB->SetParLimits(1,0.1,10.);   
  
   // //funcCB->FixParameter(4,1.);
   
   // funcCB->SetLineColor(kBlue);
   // funcCB->SetLineWidth(2);
   // TCanvas* ca = new TCanvas("ca","Trigger Efficiency",700,700) ;
   // TotalEB->Draw();
   // TotalEB->Fit(funcCB,"RS", " ", -0.1,1.);
   // funcCB->Draw("SAME");
   // ca->Print ("trialEB.png");
   // TotalEE->Draw();
   // TotalEE->Fit(funcCB,"RS", " ", -0.1,1.);
   // funcCB->Draw("SAME");
   // ca->Print ("trialEE.png");


//Fitting
   // RooRealVar x("x", "resolution",-1.,1.);

   // RooRealVar x1("x1", "resolution EE",-1.,1.);

   // RooDataHist data("data", "dataset", x,TotalEB);
   // RooDataHist dataEE("dataEE", "datasetEE", x1,TotalEE);
   // RooRealVar cbmean("cb mean", "cb mean" ,0,-0.1,0.1);
   // RooRealVar cbsigma("cb sigma", "cb #sigma" , 0.05,0.,0.5);

   // RooRealVar n("n","n",5.,0.1, 10.);
   // RooRealVar alpha("alpha","#alpha",-10.,-100.,0.);


   // RooRealVar alphaL("alphaL","#alphaL",-10.,-100.,0.);
   
   // RooCruijff  mypdf("mypdf","Cruijff", x, cbmean,cbsigma,cbsigmaL,alpha,alphaL);
   

   // RooCBShape cball("cball", "crystal ball",x,cbmean, cbsigma, alpha, n);
   // RooCBShape cballEE("cball", "crystal ball",x1,cbmean, cbsigma, alpha, n);

   // RooPlot* xframe = x.frame();
   // data.plotOn(xframe,LineStyle(2),MarkerSize(0.5));
   // x.setRange("fitrange",-0.045,0.5); 
   // //x.setRange("fitrange",-1.,1.); 

   // RooFitResult *fitRes=cball.fitTo(data,SumW2Error(true), Range("fitrange") );
   // cball.plotOn(xframe,LineStyle(1),LineColor(kBlue), Range(-1, 1) );
   // //cball.paramOn(xframe, Format("NEU", AutoPrecision(2)), Layout(0.6, 0.95, 0.9) );
   // TCanvas* ca = new TCanvas("ca","Trigger Efficiency",700,700) ;
   // ca->SetGridx();
   // ca->SetGridy();
   // ca->cd();
   // xframe->Draw();
   // ca->Update();
   // ca->Print ("trialEB.png");
   // ca->Print ("trialEB.pdf");
   // ca->SaveAs("trialEB.root");
   
   // cbmean.Print();
   // cbsigma.Print();
   
   // std::cout << "BARREL FITTING DONE" << std::endl;

   // RooPlot* xframeEE = x1.frame();
   // dataEE.plotOn(xframeEE,LineStyle(2),MarkerSize(0.5));
   // x1.setRange("fitrange1",-0.18,1.); 
   // //x1.setRange("fitrange1",-1.,1.); 

   // RooFitResult *fitResEE=cballEE.fitTo(dataEE,SumW2Error(true),  Range("fitrange1") );
   // cballEE.plotOn(xframeEE,LineStyle(1),LineColor(kBlue), Range(-1,1));
   // //cballEE.paramOn(xframeEE, Format("NELU", AutoPrecision(2)), Layout(0.2, 0.5, 0.9) );
   // ca->Clear();
   // ca->cd();
   // xframeEE->Draw();
   // ca->Update();
   // ca->Print ("trialEE.png");
   // ca->Print ("trialEE.pdf");
   // ca->SaveAs("trialEE.root");
   // cbmean.Print();
   // cbsigma.Print();
   
   

}
void mergeHigto(TH1F *Hist1,TH1F *Hist2,TH1F *Hist3){
  int Number=Hist1->GetSize()-2; 
  for(int i=1;i<=Number;i++){
     Hist3->SetBinContent(i,Hist1->GetBinContent(i)+Hist2->GetBinContent(i));  

  }



}
void normalize(TH1F * Hist){
   Double_t integral = Hist->TH1F::Integral();
   if (integral > 0) 
       Hist->Scale(1/integral);
}
void loadPresentationStyle(){
  gROOT->SetStyle("Plain");
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
//  gStyle->SetPadTickX(1);
//  gStyle->SetPadTickY(1);
  gStyle->SetTitleXOffset(1.);
  gStyle->SetTitleYOffset(2.);
  gStyle->SetLabelOffset(0.01, "XYZ");
//  gStyle->SetTitleSize(0.07, "XYZ");
//  gStyle->SetTitleFont(22,"X");
//  gStyle->SetTitleFont(22,"Y");
//  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetPadLeftMargin(0.2);
  gStyle->SetPadRightMargin(0.1);
  gStyle->SetHistLineWidth(2);
 // setTDRStyle();
}

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
#include <TH2.h>
#include <TH1.h>
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

using namespace RooFit ;
void mergeHigto(TH1F *Hist1,TH1F *Hist2,TH1F *Hist3);
void Fitresolution(TFile *f0,TFile *output,int IECAL);
void normalize(TH1F * Hist);
void normalizeD(TH1D * Hist);
void loadPresentationStyle();
void sigmaCal(TH1F *Hist,float xmin,float xmax);
int main(){

//Fitresolution("ratioScan/selectPairsDir/2015D_jason_default_reso.root","ratioScan/selectPairsDir/Old_LC.root",1);
//TFile *f0 =TFile::Open("ratioScan/selectPairsDir/Old_LC.root");
TFile *f0 =TFile::Open("result/effi_TagProbe_tree_2012D.root");
TFile *outfile=new TFile("2012D.root","RECREATE");
Fitresolution(f0,outfile,1);
//TFile *f1 =TFile::Open("ratioScan/selectPairsDir/2015D_new_et14.root");
TFile *f1 =TFile::Open("result/effi_TagProbe_tree_2015D.root");
TFile *outfile1=new TFile("2015D.root","RECREATE");

Fitresolution(f1,outfile1,1);
//outfile->Write();
return 0;
}

void Fitresolution(TFile *f0,TFile *output,int IECAL){
   TTree *t0 = (TTree*)f0->Get("resolution_plot");
   Float_t resoTagEB,resoTagEE,resoProbeEB,resoProbeEE,etabinreso[4];
   t0->SetBranchAddress("resolution_ProbeEE",&resoProbeEE);  
   t0->SetBranchAddress("resolution_TagEE",&resoTagEE);  
   t0->SetBranchAddress("resolution_ProbeEB",&resoProbeEB);  
   t0->SetBranchAddress("resolution_TagEB",&resoTagEB);  
   t0->SetBranchAddress("resolution_etabin",etabinreso);  
   loadPresentationStyle();
   Long64_t nentries = t0->GetEntries();
   TH1F *etaResolution   = new TH1F("etaResolution","etaResolution",100,-2.5,2.5);
   TH1F *ProbeEE   = new TH1F("probeEE","probeEE ",200,-1.,1.);

   TH1F *TagEE   = new TH1F("TagEE","TagEE ",200,-1.,1.);
   TH1F *TotalEE   = new TH1F("TotalEE","TotalEE ",200,-1.,1.);
   TH1F *ProbeEB   = new TH1F("probeEB","probeEB ",200,-1.,1.);
   TH1F *TagEB   = new TH1F("TagEB","TagEB ",200,-1.,1.);
   TH1F *TotalEB   = new TH1F("TotalEB","TotalEB",200,-1.,1.);
   TH1F *Distribution   = new TH1F("TotalEB_Distribution","TotalEB",100,-2.5,2.5);

   int binnumber=18;
  // int binnumber=Etabinreso->GetBin(Etabinreso->GetXaxis()->GetLast());  
   TH2D *Etabinreso   = new TH2D("Etabinreso","Etabinreso",binnumber,-2.5,2.5,200,-1,1);
   TH1D **hlist = new TH1D*[binnumber];
   char *name   = new char[2000];
   char *title  = new char[2000];
   
   std::cout<<"the number of total events"<<nentries<<std::endl;
   for(int i=0;i<nentries;i++){
      t0->GetEntry(i);

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
   for (int i=1;i<=binnumber;i++){
      snprintf(name,2000,"Histbin_%d",i); 
    //  snprintf(title,2000,"Fitted value of bin_[%d]",i);
  //    snprintf(title,2000,"Fitted value of bin_[%d]",i);
      Etabinreso->ProjectionY(name,i,i,"e");
  //    delete gDirectory->FindObject(name);
   //   hlist[i] = new TH1D(name,title,100, -1.,1.);
   }
//   for (int i=0;i<binnumber;i++){
//      hlist[i]=Etabinreso->ProjectionY("_temp",i,i+1,"e");
//      normalizeD(hlist[i]);
//   }
   delete [] name;
   delete [] title;
   delete [] hlist;
   TotalEB->Sumw2();
   TotalEE->Sumw2();
   normalize(TotalEE);
   normalize(TotalEB);
   std::cout<<"***********************************"<<std::endl;
   std::cout<<"the lower edge stuff"<<TotalEB->GetBinLowEdge(1)<<std::endl;
   std::cout<<"the naieve mean from the histogram directly "<<TotalEB->GetMean()<<"  the rms value is here"<<TotalEB->GetRMS()<<std::endl; 
   std::cout<<"the last bin stuff for 1d histogram"<<TotalEB->GetBin(TotalEB->GetXaxis()->GetLast())<<std::endl;
 //  f0->Close();
 //  output->Open();
//   TotalEE->Write();
//   TotalEB->Write();
   output->Write();
   output->Close();
//Fitting
/*   RooRealVar x("x", "resolution",-1.,1.);

   RooRealVar x1("x1", "resolution EE",-1.,1.);

   RooDataHist data("data", "dataset", x,TotalEB);
   RooDataHist dataEE("dataEE", "datasetEE", x1,TotalEE);
   RooRealVar cbmean("cbmean", "cbmean" ,0,-1.,1.);
   RooRealVar cbsigma("cbsigma", "cbsigma" , 0.05,0.,1.);
   RooRealVar cbsig("cbsig", "cbsignal" ,10,0,1000000);
   RooRealVar n("n","n",5.,-1, 2000.);
   RooRealVar alpha("alpha","alpha",-10.,-100.,0.);


   RooCBShape cball("cball", "crystal ball",x,cbmean, cbsigma, alpha, n);
   RooCBShape cballEE("cball", "crystal ball",x1,cbmean, cbsigma, alpha, n);

   RooPlot* xframe = x.frame();
   data.plotOn(xframe,LineStyle(2),MarkerSize(0.5));
   x.setRange("fitrange",-0.06,0.5); 
   


   RooFitResult *fitRes=cball.fitTo(data,SumW2Error(kTRUE), Range("fitrange") );
   cball.plotOn(xframe,LineStyle(1),LineColor(kBlue));
   cball.paramOn(xframe, Format("NELU", AutoPrecision(2)), Layout(0.1, 0.4, 0.9) );
   TCanvas* ca = new TCanvas("ca","Trigger Efficiency",700,700) ;
   ca->SetGridx();
   ca->SetGridy();
   ca->cd();
   xframe->Draw();
   ca->Update();
   ca->Print ("trialEB.png");
   ca->Print ("trialEB.pdf");
   ca->SaveAs("trialEB.root");
   std::cout << "BARREL FITTING DONE" << std::endl;

   RooPlot* xframeEE = x1.frame();
   dataEE.plotOn(xframeEE,LineStyle(2),MarkerSize(0.5));
   x1.setRange("fitrange1",-0.18,0.7); 

   RooFitResult *fitResEE=cballEE.fitTo(dataEE,SumW2Error(false),Minos(false),  Range("fitrange1") );
   cballEE.plotOn(xframeEE,LineStyle(1),LineColor(kBlue));
   cballEE.paramOn(xframeEE, Format("NELU", AutoPrecision(2)), Layout(0.1, 0.4, 0.9) );
   ca->Clear();
   ca->cd();
   xframeEE->Draw();
   ca->Update();
   ca->Print ("trialEE.png");
   ca->Print ("trialEE.pdf");
   ca->SaveAs("trialEE.root");

*/
//   TotalEE->Integral(1,-1);
//   TotalEE->Draw();
//   ca->Update();
//   ca->Print ("TotalEE_2.png");
}
void mergeHigto(TH1F *Hist1,TH1F *Hist2,TH1F *Hist3){
  int Number=Hist1->GetSize()-2; 
  for(int i=1;i<=Number;i++){
     Hist3->SetBinContent(i,Hist1->GetBinContent(i)+Hist2->GetBinContent(i));  

  }

}

void sigmaCal(TH1F *Hist,float xmin,float xmax){
 std::vector<float> v;
 double binintegral=0;
 TAxis *axis = Hist->GetXaxis();
 int bmin = axis->FindBin(xmin);
 int bmax = axis->FindBin(xmax);
 for(int i=bmin;i<=bmax;i++){
   binintegral=binintegral+Hist->GetBinContent(i);
   v.push_back(binintegral); 

 }



}
void normalize(TH1F * Hist){
   Double_t integral = Hist->TH1F::Integral();
   if (integral > 0) 
       Hist->Scale(1/integral);
}
void normalizeD(TH1D * Hist){
   Double_t integral = Hist->TH1D::Integral();
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

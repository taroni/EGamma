void fitY() {
//
// To see the output of this macro, click begin_html <a href="gif/fitslicesy.gif" >here</a> end_html
//    This macro illustrates how to use the TH1::FitSlicesY function
//    It uses the TH2F histogram generated in macro hsimple.C
//    It invokes FitSlicesY and draw the fitted "mean" and "sigma"
//    in 2 sepate pads.
//    This macro shows also how to annotate a picture, change
//    some pad parameters.
//Author: Rene Brun
//changes:I.Rubinsky

// Change some default parameters in the current style
//   gStyle->SetLabelSize(0.06,"x");
//   gStyle->SetLabelSize(0.06,"y");
//   gStyle->SetFrameFillColor(38);
//   gStyle->SetTitleW(0.6);
//   gStyle->SetTitleH(0.1);
   TFile* hsimple = TFile::Open("histostrial.root");
   if (!hsimple) return;
//   TH2F *hpxpy = new TH2D("hist","hist",100,-2.5,2.5,100,-1.,1.);
   TH2F * Etabin = (TH2F*)hsimple->Get("Etabin"); 
   if (!Etabin)
      std::cout<<"wrong"<<std::endl;
// Connect the input file and get the 2-d histogram in memory
//   hpxpy->Draw();
// Create a canvas and divide it
//   TCanvas *c1 = new TCanvas("c1","c1",700,500);
//   c1->SetFillColor(42);
//   c1->Divide(2,1);
//   c1->cd(1);
//   TPad *left = (TPad*)gPad;
//   left->Divide(1,2);
//   std::cout<<"what is the problem"<<std::endl;
//// Draw 2-d original histogram
//   left->cd(1);
//   gPad->SetTopMargin(0.12);
//   gPad->SetFillColor(33);
//   hpxpy->Draw();
//   hpxpy->GetXaxis()->SetLabelSize(0.06);
//   hpxpy->GetYaxis()->SetLabelSize(0.06);
//   hpxpy->SetMarkerColor(kYellow);
//
//// Fit slices projected along Y fron bins in X [7,32]
//   hpxpy->FitSlicesY(0,0,0,20);
   Etabin->FitSlicesY();
//
//// Show fitted "mean" for each slice
//   left->cd(2);
//   gPad->SetFillColor(33);
   TH2F *Etabin_0 = (TH2F*)hsimple->Get("Etabin_0");
   Etabin_0->Draw();
//   c1->cd(2);
//   TPad *right = (TPad*)gPad;
//   right->Divide(1,2);
//   right->cd(1);
//   gPad->SetTopMargin(0.12);
//   gPad->SetLeftMargin(0.15);
//   gPad->SetFillColor(33);
//   hpxpy_1->Draw();
//
//// Show fitted "sigma" for each slice
//   right->cd(2);
//   gPad->SetTopMargin(0.12);
//   gPad->SetLeftMargin(0.15);
//   gPad->SetFillColor(33);
//   hpxpy_2->SetMinimum(0.8);
//   hpxpy_2->Draw();
//
////attributes
//   hpxpy_0->SetLineColor(kYellow);
//   hpxpy_1->SetLineColor(kYellow);
//   hpxpy_2->SetLineColor(kYellow);
//   hpxpy_0->SetMarkerColor(kRed);
//   hpxpy_1->SetMarkerColor(kRed);
//   hpxpy_2->SetMarkerColor(kRed);
//   hpxpy_0->SetMarkerStyle(21);
//   hpxpy_1->SetMarkerStyle(21);
//   hpxpy_2->SetMarkerStyle(21);
//   hpxpy_0->SetMarkerSize(0.6);
//   hpxpy_1->SetMarkerSize(0.6);
//   hpxpy_2->SetMarkerSize(0.6);
}

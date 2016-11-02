import ROOT 
ROOT.gSystem.Load("RooCruijff_cc")
from ROOT import RooCruijff
from PeakFitter import *
import matplotlib.pyplot as pyplot


ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptTitle(0)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetTitleXOffset(1.2)
ROOT.gStyle.SetTitleYOffset(2.)
ROOT.gStyle.SetLabelOffset(0.01, "XYZ")
ROOT.gStyle.SetPadLeftMargin(0.2)
ROOT.gStyle.SetPadRightMargin(0.1)
ROOT.gStyle.SetPadTopMargin(0.05)
ROOT.gStyle.SetPadBottomMargin(0.15)
ROOT.gStyle.SetHistLineWidth(2)
ROOT.gStyle.SetOptTitle(0)
ROOT.gROOT.SetStyle("Plain");
ROOT.gStyle.SetOptTitle(0);
ROOT.gStyle.SetOptStat(0);
ROOT.gStyle.SetPadTickX(1);
ROOT.gStyle.SetPadTickY(1);
ROOT.gStyle.SetTitleXOffset(1.15);
ROOT.gStyle.SetTitleYOffset(0.01);
ROOT.gStyle.SetLabelOffset(0.005, "XYZ");
ROOT.gStyle.SetTitleSize(0.07, "XYZ");
ROOT.gStyle.SetTitleFont(22,"X");
ROOT.gStyle.SetTitleFont(22,"Y");
#ROOT.gStyle.SetPadBottomMargin(0.13);
#ROOT.gStyle.SetPadLeftMargin(0.15);
#ROOT.gStyle.SetPadRightMargin(0.15);
ROOT.gStyle.SetHistLineWidth(2);
## For the canvas:
ROOT.gStyle.SetCanvasBorderMode(0);
ROOT.gStyle.SetCanvasColor(0);
ROOT.gStyle.SetCanvasDefH(600); ##Height of canvas
ROOT.gStyle.SetCanvasDefW(600); ##Width of canvas
ROOT.gStyle.SetCanvasDefX(0);   ##POsition on screen
ROOT.gStyle.SetCanvasDefY(0);

## For the Pad:
ROOT.gStyle.SetPadBorderMode(0);
ROOT.gStyle.SetPadColor(0);
ROOT.gStyle.SetPadGridX(False);
ROOT.gStyle.SetPadGridY(False);
ROOT.gStyle.SetGridColor(0);
ROOT.gStyle.SetGridStyle(3);
ROOT.gStyle.SetGridWidth(1);

## For the frame:
ROOT.gStyle.SetFrameBorderMode(0);
ROOT.gStyle.SetFrameBorderSize(1);
ROOT.gStyle.SetFrameFillColor(0);
ROOT.gStyle.SetFrameFillStyle(0);
ROOT.gStyle.SetFrameLineColor(1);
ROOT.gStyle.SetFrameLineStyle(1);
ROOT.gStyle.SetFrameLineWidth(1);

## For the histo:
ROOT.gStyle.SetHistLineColor(1);
ROOT.gStyle.SetHistLineStyle(0);
#ROOT.gStyle.SetHistLineWidth(1);
ROOT.gStyle.SetEndErrorSize(2);
ROOT.gStyle.SetErrorX(0.);
#ROOT.gStyle.SetMarkerStyle(20);

##For the fit/function:
ROOT.gStyle.SetOptFit(1);
ROOT.gStyle.SetFitFormat("5.4g");
ROOT.gStyle.SetFuncColor(2);
ROOT.gStyle.SetFuncStyle(1);
#ROOT.gStyle.SetFuncWidth(1);

##For the date:
ROOT.gStyle.SetOptDate(0);

## For the statistics box:
ROOT.gStyle.SetOptFile(0);
ROOT.gStyle.SetOptStat(0); ## To display the mean and RMS:   SetOptStat("mr");
ROOT.gStyle.SetStatColor(0);
ROOT.gStyle.SetStatFont(42);
ROOT.gStyle.SetStatFontSize(0.025);
ROOT.gStyle.SetStatTextColor(1);
ROOT.gStyle.SetStatFormat("6.4g");
ROOT.gStyle.SetStatBorderSize(1);
ROOT.gStyle.SetStatH(0.1);
ROOT.gStyle.SetStatW(0.15);

## Margins:
ROOT.gStyle.SetPadTopMargin(0.05);
ROOT.gStyle.SetPadBottomMargin(0.13);
ROOT.gStyle.SetPadLeftMargin(0.15);
ROOT.gStyle.SetPadRightMargin(0.03);

## For the Global title:
ROOT.gStyle.SetOptTitle(0);
ROOT.gStyle.SetTitleFont(42);
ROOT.gStyle.SetTitleColor(1);
ROOT.gStyle.SetTitleTextColor(1);
ROOT.gStyle.SetTitleFillColor(10);
ROOT.gStyle.SetTitleFontSize(0.05);

## For the axis titles:
ROOT.gStyle.SetTitleColor(1, "XYZ");
ROOT.gStyle.SetTitleFont(42, "XYZ");
ROOT.gStyle.SetTitleSize(0.05, "XYZ");
##ROOT.gStyle.SetTitleXOffset(1.7);
ROOT.gStyle.SetTitleYOffset(1.45);

## For the axis labels:
ROOT.gStyle.SetLabelColor(1, "XYZ");
ROOT.gStyle.SetLabelFont(42, "XYZ");
ROOT.gStyle.SetLabelOffset(0.007, "XYZ");
ROOT.gStyle.SetLabelSize(0.04, "XYZ");

## For the axis:
ROOT.gStyle.SetAxisColor(1, "XYZ");
ROOT.gStyle.SetStripDecimals(True);
ROOT.gStyle.SetTickLength(0.03, "XYZ");
ROOT.gStyle.SetNdivisions(510, "XYZ");
ROOT.gStyle.SetPadTickX(1);  ## To get tick marks on the opposite side of the frame
ROOT.gStyle.SetPadTickY(1);

## Change for log plots:
ROOT.gStyle.SetOptLogx(0);
ROOT.gStyle.SetOptLogy(0);
ROOT.gStyle.SetOptLogz(0);

## Postscript options:
ROOT.gStyle.SetPaperSize(20.,20.);
ROOT.gROOT.ForceStyle()


filein=ROOT.TFile.Open("2015D.root", "READ")
filein1=ROOT.TFile.Open("2012D.root", "READ")
title_name_string=("2012D")
Brange=18
histoEB = filein.Get("TotalEB")
histoEB.GetXaxis().SetTitle("Resolution")
histoEB.GetYaxis().SetTitle("Events/0.005")

c= ROOT.TCanvas("c", "c", 700, 700) 
c.Draw()


xvalue=[0 for x in range(0,Brange)]
yvalue=[0 for x in range(0,Brange)]
yerrorup=[0 for x in range(0,Brange)]
yerrordown=[0 for x in range(0,Brange)]
xvalue1=[0 for x in range(0,Brange)]
yvalue1=[0 for x in range(0,Brange)]
yerrorup1=[0 for x in range(0,Brange)]
yerrordown1=[0 for x in range(0,Brange)]


Bwidth=5.0/Brange
Etareso= ROOT.TGraphAsymmErrors(Brange)
Etareso2= ROOT.TGraphAsymmErrors(Brange)
for binnumber in range(0,Brange):
    m          = ROOT.RooRealVar('res', 'Resolution=(E_{T} - L1)/E_{T}',0., -0.4, 1.)
    mean       = ROOT.RooRealVar('mean', 'mean', 0.1,-0.25,.4)
    sigmaL     = ROOT.RooRealVar('sigmaL' ,'sigmaL' ,0.05 ,0 ,0.5)
    sigmaR     = ROOT.RooRealVar('sigmaR' ,'sigmaR' ,0.05 ,0 ,0.5)
    alphaL     = ROOT.RooRealVar('alphaL' ,'alphaL' ,0.1  ,0 ,30 )
    alphaR     = ROOT.RooRealVar('alphaR' ,'alphaR' ,0.1  ,0 ,30 )
    hist=filein.Get("Histbin_%d" %(binnumber+1))
    hist.Sumw2()
    myhist = ROOT.RooDataHist(hist.GetName(),hist.GetName(),ROOT.RooArgList(m), hist,1.)
    frame = m.frame()
    myhist.plotOn(frame, ROOT.RooFit.MarkerColor(1))
    myhist.plotOn(frame).GetYaxis().SetTitle("a. u.") 
    if binnumber <5 or binnumber >13:
        m.setRange("fitrange", -0.2,0.3)
    else:
        m.setRange("fitrange", -0.1,0.15)
    func= ROOT.RooCruijff( 'func', 'func', m, mean, sigmaL, sigmaR, alphaL, alphaR)
    fitresult= func.fitTo(
        myhist,
        ROOT.RooFit.Save(True),
        ROOT.RooFit.Range("fitrange")
    )
    func.plotOn(frame, ROOT.RooFit.LineColor(2))
    frame.Draw()
    c.SaveAs("frame_%s.pdf" %(str(binnumber)))
    c.SaveAs("frame_%s.png" %(str(binnumber)))
    c.Clear()
    #fit= doubleCBFit(hist,5.)
    hist.GetXaxis().SetTitle("Resolution")
    hist.GetYaxis().SetTitle("Events/0.005")
    hist.SetTitle(title_name_string+", #eta bin range "+str(round(-2.5+binnumber*Bwidth,2))+" to "+ str(round(-2.5+(binnumber+1)*Bwidth,2)) )
    #   func=fit[0]
    peak = mean.getVal()
    yvalue[binnumber]=peak
    #yerrorup[binnumber]=FindRightResolution(fit[0], fit[1])
    #yerrordown[binnumber]=FindLeftResolution(fit[0], fit[1])
    yerrorup[binnumber]=sigmaR.getVal()
    yerrordown[binnumber]=sigmaL.getVal()
    print peak,sigmaR.getVal(),sigmaL.getVal()
    xvalue[binnumber]=-2.5+(5.0/Brange)*binnumber+5.0/2/Brange
    Etareso.SetPoint(binnumber,xvalue[binnumber],yvalue[binnumber])
    Etareso.SetPointEYhigh(binnumber,yerrorup[binnumber])
    Etareso.SetPointEYlow(binnumber,yerrordown[binnumber])
    Etareso2.SetPoint(binnumber,xvalue[binnumber],0.)
    Etareso2.SetPointEYhigh(binnumber,yerrorup[binnumber])
    Etareso2.SetPointEYlow(binnumber,yerrordown[binnumber])

histoEE = filein.Get("TotalEE")
histoEB.GetXaxis().SetTitle("Resolution")
histoEB.GetYaxis().SetTitle("Events/0.005")
   
Etareso.SetMarkerStyle(20)
Etareso.GetYaxis().SetTitle("(E_{T}-L1)/E_{T}")
Etareso.GetXaxis().SetTitle("#eta")
Etareso2.SetMarkerStyle(20)
#Etareso2.GetYaxis().SetTitle("(E_{T}-L1)/E_{T}")
Etareso2.GetXaxis().SetTitle("#eta")

Etareso1= ROOT.TGraphAsymmErrors(Brange)
Etareso3= ROOT.TGraphAsymmErrors(Brange)
for binnumber in range(0,Brange):
    m          = ROOT.RooRealVar('res', 'Resolution=(E_{T} - L1)/E_{T}',0., -0.4, 1.)
    mean       = ROOT.RooRealVar('mean', 'mean', 0.1,-0.25,.4)
    sigmaL     = ROOT.RooRealVar('sigmaL' ,'sigmaL' ,0.05 ,0 ,0.5)
    sigmaR     = ROOT.RooRealVar('sigmaR' ,'sigmaR' ,0.05 ,0 ,0.5)
    alphaL     = ROOT.RooRealVar('alphaL' ,'alphaL' ,0.1  ,0 ,30 )
    alphaR     = ROOT.RooRealVar('alphaR' ,'alphaR' ,0.1  ,0 ,30 )
    hist=filein1.Get("Histbin_%d" %(binnumber+1))
    hist.Sumw2()
    myhist = ROOT.RooDataHist(hist.GetName(),hist.GetName(),ROOT.RooArgList(m), hist,1.)
    frame = m.frame()
    myhist.plotOn(frame, ROOT.RooFit.MarkerColor(1))
    myhist.plotOn(frame).GetYaxis().SetTitle("a. u.") 
    if binnumber<5 or binnumber >13:
        m.setRange("fitrange", -0.2,0.4)
    else:
        m.setRange("fitrange", -0.1,0.25)
    func= ROOT.RooCruijff( 'func', 'func', m, mean, sigmaL, sigmaR, alphaL, alphaR)
    fitresult= func.fitTo(
        myhist,
        ROOT.RooFit.Save(True),
        ROOT.RooFit.Range("fitrange")
    )
    func.plotOn(frame, ROOT.RooFit.LineColor(2))
    frame.Draw()
    c.SaveAs("frame_2012_%s.pdf" %(str(binnumber)))
    c.SaveAs("frame_2012_%s.png" %(str(binnumber)))
    c.Clear()
    hist.GetXaxis().SetTitle("Resolution")
    hist.GetYaxis().SetTitle("Events/0.005")
    hist.SetTitle(title_name_string+", #eta bin range "+str(round(-2.5+binnumber*Bwidth,2))+" to "+ str(round(-2.5+(binnumber+1)*Bwidth,2)) )
    peak = mean.getVal()
    yvalue1[binnumber]=peak
    yerrorup1[binnumber]=sigmaR.getVal()
    yerrordown1[binnumber]=sigmaL.getVal()
    print peak,sigmaR.getVal(),sigmaL.getVal()
    xvalue1[binnumber]=(-2.5+(5.0/Brange)*binnumber+5.0/2/Brange)-0.05
    Etareso1.SetPoint(binnumber,xvalue1[binnumber],yvalue1[binnumber])
    Etareso1.SetPointEYhigh(binnumber,yerrorup1[binnumber])
    Etareso1.SetPointEYlow(binnumber,yerrordown1[binnumber])
    Etareso3.SetPoint(binnumber,xvalue1[binnumber],0.)
    Etareso3.SetPointEYhigh(binnumber,yerrorup1[binnumber])
    Etareso3.SetPointEYlow(binnumber,yerrordown1[binnumber])

legend = ROOT.TLegend(0.35,0.85,0.75,0.75)
legend.SetFillColor(0)
legend.SetLineColor(0)
#legend.SetTextFont(70)
legend.AddEntry(Etareso, "2015 data, #sqrt{s} =13 TeV","lp")
legend.AddEntry(Etareso1, "2012 data, #sqrt{s} = 8 TeV","lp")
#legend.AddEntry(None, "Threshold: 20 GeV", "")
Etareso1.SetMarkerStyle(21)
Etareso1.SetMarkerColor(2)
Etareso1.SetLineColor (2)
Etareso1.GetYaxis().SetTitle("(E_{T}-L1)/E_{T}")
Etareso1.GetXaxis().SetTitle("#eta")
Etareso3.SetMarkerStyle(21)
Etareso3.SetMarkerColor(2)
Etareso3.SetLineColor (2)
#Etareso3.GetYaxis().SetTitle("(E_{T}-L1)/E_{T}")
Etareso3.GetXaxis().SetTitle("#eta")

Etareso1.SetMaximum(0.27)
Etareso1.SetMinimum(-0.15)
Etareso1.Draw("AP")
c.Update()
Etareso.Draw("P")
text = ROOT.TLatex(-2.6, .24, "CMS")
text.SetTextSize(0.04);
textp = ROOT.TLatex(-2.6, .225, "Preliminary")
textp.SetTextSize(0.03);
textp.SetTextFont(52);

text.Draw("same");
textp.Draw("same");

textl1 = ROOT.TLatex(-1., .225, "L1 Threshold: 20 GeV")
textl1.SetTextSize(0.03);
textl1.Draw("same");


legend.Draw("SAME")


c.SaveAs("Etareso_2015_2012.png")
c.SaveAs("Etareso_2015_2012.pdf")


c.Clear()
Etareso3.Draw("AP")
c.Update()
Etareso2.Draw("P")
text = ROOT.TLatex(-2.6, .24, "CMS")
text.SetTextSize(0.04);
textp = ROOT.TLatex(-2.6, .225, "Preliminary")
textp.SetTextSize(0.03);
textp.SetTextFont(52);
legend.Draw("SAME")



text.Draw("same");
textp.Draw("same");
c.SaveAs("EtaresoWidth_2015_2012.png")
c.SaveAs("EtaresoWidth_2015_2012.pdf")



c2= ROOT.TCanvas("c2","c2", 600, 600)
c2.Draw()
c2.Divide(1,2)

c2.cd(1)

errDown=ROOT.TGraph(Brange)
errUp=ROOT.TGraph(Brange)
err=ROOT.TGraph(Brange)
errDown1=ROOT.TGraph(Brange)
errUp1=ROOT.TGraph(Brange)
err1=ROOT.TGraph(Brange)

texfile = open("sigmaRandLComprison.tex", "w")
texfile.write("\\begin{table}\n")
texfile.write("\\begin{tabular}{|c|c|c||c|c||c|c|}\n")
texfile.write("& \multicolumn{2}{c}{2012}& \multicolumn{2}{c}{2015} \\\\\hline\n")
texfile.write("$\eta$ & $\sigma_L$ & $\sigma_R$& $\sigma_L$ & $\sigma_R$\\\\\hline\n")

for binnumber in range(0, Brange):
    errDown.SetPoint(binnumber,xvalue[binnumber],yerrordown[binnumber])
    errUp.SetPoint(binnumber,xvalue[binnumber],yerrorup[binnumber])
    errDown1.SetPoint(binnumber,xvalue[binnumber],yerrordown1[binnumber])
    errUp1.SetPoint(binnumber,xvalue[binnumber],yerrorup1[binnumber])
    
    olderr=yerrordown1[binnumber]*yerrordown1[binnumber]+yerrorup1[binnumber]*yerrorup1[binnumber]
    newerr=yerrordown[binnumber]*yerrordown[binnumber]+yerrorup[binnumber]*yerrorup[binnumber]
    err.SetPoint(binnumber,xvalue[binnumber], math.sqrt(newerr))
    err1.SetPoint(binnumber,xvalue[binnumber], math.sqrt(olderr))
    #print  (xvalue[binnumber], yerrordown1[binnumber], yerrorup1[binnumber], yerrordown[binnumber],yerrorup[binnumber], (yerrordown1[binnumber]-yerrordown[binnumber])/yerrordown1[binnumber] , ( yerrorup1[binnumber] - yerrorup[binnumber])/ yerrorup1[binnumber])
    texfile.write("%.3f & %.3f & %.3f & %.3f & %.3f & %.3f & %.3f \\\\\hline\n" %(xvalue[binnumber], yerrordown1[binnumber], yerrorup1[binnumber], yerrordown[binnumber],yerrorup[binnumber], (yerrordown[binnumber]-yerrordown1[binnumber])/yerrordown1[binnumber] , ( yerrorup[binnumber] - yerrorup1[binnumber])/ yerrorup1[binnumber]))

texfile.write("\end{tabular}\n")
texfile.write("\end{table}\n")
texfile.close()
    
errDown.SetMarkerStyle(20)
errUp.SetMarkerStyle(20)
errDown1.SetMarkerStyle(21)
errUp1.SetMarkerStyle(21)
err1.SetMarkerStyle(21)
err.SetMarkerStyle(20)
errDown.SetMaximum(0.15)
errDown.Draw("AP")
errDown.GetYaxis().SetTitle("left width (#sigma_{L})")
errDown1.SetMarkerColor(2)
errDown.GetXaxis().SetTitle("#eta")
errDown1.Draw("P")
legend.Draw()
c2.cd(2)
errUp.SetMaximum(0.15)
errUp.GetYaxis().SetTitle("right width (#sigma_{R})")
errUp.GetXaxis().SetTitle("#eta")
errUp.Draw("AP")
errUp1.SetMarkerColor(2)
errUp1.Draw("P")
legend.Draw()
c3= ROOT.TCanvas("c3","c3", 600, 600)
c3.Draw()
err.SetMaximum(0.2)
err.Draw("AP")
err.GetYaxis().SetTitle("Width")
err.GetXaxis().SetTitle("#eta")

err1.SetMarkerColor(2)
err1.Draw("P")
legend.Draw()



c2.SaveAs("sigmaRandL.pdf")
c3.SaveAs("totalSigma.pdf")    
c2.SaveAs("sigmaRandL.png")
c3.SaveAs("totalSigma.png")    

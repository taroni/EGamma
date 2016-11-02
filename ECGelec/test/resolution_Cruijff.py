import ROOT 
ROOT.gSystem.Load("RooCruijff_cc")
from ROOT import RooCruijff
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
ROOT.gStyle.SetPadLeftMargin(0.14);
ROOT.gStyle.SetPadRightMargin(0.04);

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


filein=ROOT.TFile.Open("outfile2012.root", "READ")
filein1=ROOT.TFile.Open("outfile2015.root", "READ")
histoEB = filein.Get("TotalEB")
histoEB_2015 = filein1.Get("TotalEB")
histoEB_2015.SetName("TotalEB_2015")
histoEB.SetLineColor(1)
histoEB.SetMarkerColor(1)
histoEB.SetMarkerStyle(20)
histoEB_2015.SetLineColor(2)
histoEB_2015.SetMarkerColor(2)
histoEB_2015.SetMarkerStyle(20)


c= ROOT.TCanvas("c", "c", 700, 700) 
c.Draw()
c.SetGridx(0)
c.SetGridy(0)
m          = ROOT.RooRealVar('res', '(E_{T} - L1)/E_{T}',0., -0.4, 1.)
mean       = ROOT.RooRealVar('mean', 'mean', 0,-0.25,.4)
sigmaL     = ROOT.RooRealVar('sigmaL' ,'sigmaL' ,0.05 ,0 ,0.5)
sigmaR     = ROOT.RooRealVar('sigmaR' ,'sigmaR' ,0.05 ,0 ,0.5)
alphaL     = ROOT.RooRealVar('alphaL' ,'alphaL' ,0.1  ,0 ,30 )
alphaR     = ROOT.RooRealVar('alphaR' ,'alphaR' ,0.1  ,0 ,30 )
m.setRange("fitrange", -0.05,0.15)

frame = m.frame()

myhist = ROOT.RooDataHist(histoEB.GetName(),histoEB.GetName(),ROOT.RooArgList(m), histoEB,1.)
myhist.plotOn(frame, ROOT.RooFit.MarkerColor(1))
myhist.plotOn(frame).GetYaxis().SetTitle("a. u.") 

func= ROOT.RooCruijff( 'func', 'func', m, mean, sigmaL, sigmaR, alphaL, alphaR)
fitresult= func.fitTo(
    myhist,
    ROOT.RooFit.Save(True),
    ROOT.RooFit.Range("fitrange")
)
m.setRange("plotrange", -0.04,0.15)
print 'fit 2012 EB: sigmaL %f, sigmaR %f' %(sigmaL.getVal(), sigmaR.getVal())
func.plotOn(frame, ROOT.RooFit.LineColor(1),ROOT.RooFit.Range("plotrange") )

#m2          = ROOT.RooRealVar('res2', 'Resolution=(E_{T} - L1)/E_{T}',0., -0.4, 1.)

myhist2 = ROOT.RooDataHist(histoEB_2015.GetName(),histoEB_2015.GetName(),ROOT.RooArgList(m), histoEB_2015,1.)
myhist2.plotOn(frame, ROOT.RooFit.MarkerColor(2))
m.setRange("fitrange2", -0.08,0.15)

func2= ROOT.RooCruijff( 'func2', 'func2', m, mean, sigmaL, sigmaR, alphaL, alphaR)
fitresult2= func2.fitTo(
    myhist2,
    ROOT.RooFit.Save(True),
    ROOT.RooFit.Range("fitrange2")
)
m.setRange("plotrange2", -0.09,.15)
func2.plotOn(frame, ROOT.RooFit.LineColor(2),ROOT.RooFit.Range("plotrange2"))
print 'fit 2015 EB: sigmaL %f, sigmaR %f' %(sigmaL.getVal(), sigmaR.getVal())

frame.Draw()
text = ROOT.TLatex(-0.35, .043, "CMS")
text.SetTextSize(0.04);
textp = ROOT.TLatex(-0.35, .041, "Preliminary")
textp.SetTextSize(0.03);
textp.SetTextFont(52);

text.Draw("same");
textp.Draw("same");
legend = ROOT.TLegend(0.50,0.9,0.88,0.63)
legend.SetFillColor(0)
legend.SetLineColor(0)
legend.SetTextSize(0.035)
legend.AddEntry('', 'Z #rightarrow e^{+} e^{-}', '')
legend.AddEntry('', 'ECAL Barrel', '')
legend.AddEntry('', 'L1 Threshold: 20 GeV', '')

legend.AddEntry(histoEB, "2012 data, #sqrt{s}= 8 TeV","lp")
legend.AddEntry(histoEB_2015, "2015 data, #sqrt{s}=13 TeV", "lp")

legend.Draw("SAME")


c.SaveAs("frame.pdf")
c.SaveAs("frame.png")


histoEE = filein.Get("TotalEE")
histoEE_2015 = filein1.Get("TotalEE")
histoEE_2015.SetName("TotalEE_2015")
histoEE.SetLineColor(1)
histoEE.SetMarkerColor(1)
histoEE.SetMarkerStyle(20)
histoEE_2015.SetLineColor(2)
histoEE_2015.SetMarkerColor(2)
histoEE_2015.SetMarkerStyle(20)

c.Clear()
frame3 = m.frame()

m.setRange("fitrange3", -0.18,0.5)

myhist3 = ROOT.RooDataHist(histoEE.GetName(),histoEE.GetName(),ROOT.RooArgList(m), histoEE,1.)
myhist3.plotOn(frame3, ROOT.RooFit.MarkerColor(1))
myhist3.plotOn(frame3).GetYaxis().SetTitle("a. u.") 

func3= ROOT.RooCruijff( 'func3', 'func3', m, mean, sigmaL, sigmaR, alphaL, alphaR)
fitresult3= func3.fitTo(
    myhist3,
    ROOT.RooFit.Save(True),
    ROOT.RooFit.Range("fitrange3")
)
m.setRange("plotrange3", -0.18,0.4)
func3.plotOn(frame3, ROOT.RooFit.LineColor(1),ROOT.RooFit.Range("plotrange3") )

#m2          = ROOT.RooRealVar('res2', 'Resolution=(E_{T} - L1)/E_{T}',0., -0.4, 1.)
print 'fit 2012 EE: sigmaL %f, sigmaR %f' %(sigmaL.getVal(), sigmaR.getVal())

myhist4 = ROOT.RooDataHist(histoEE_2015.GetName(),histoEE_2015.GetName(),ROOT.RooArgList(m), histoEE_2015,1.)
myhist4.plotOn(frame3, ROOT.RooFit.MarkerColor(2), ROOT.RooFit.LineColor(2))
m.setRange("fitrange4", -0.25,0.5)

func4= ROOT.RooCruijff( 'func4', 'func4', m, mean, sigmaL, sigmaR, alphaL, alphaR)
fitresult4= func4.fitTo(
    myhist4,
    ROOT.RooFit.Save(True),
    ROOT.RooFit.Range("fitrange4")
)
m.setRange("plotrange4", -0.15,.35)
func4.plotOn(frame3, ROOT.RooFit.LineColor(2),ROOT.RooFit.Range("plotrange4"))
print 'fit 2015 EE: sigmaL %f, sigmaR %f' %(sigmaL.getVal(), sigmaR.getVal())

frame3.Draw()
text.DrawLatex(-0.35,0.036,"CMS");
textp.DrawLatex(-0.35, 0.034, "Preliminary")
legend.Clear()
legend.AddEntry('', 'Z #rightarrow e^{+} e^{-}', '')
legend.AddEntry('', 'ECAL Endcaps', '')
legend.AddEntry('', 'L1 Threshold: 20 GeV', '')

legend.AddEntry(histoEE, "2012 data, #sqrt{s}= 8 TeV","l")
legend.AddEntry(histoEE_2015, "2015 data, #sqrt{s}=13 TeV", "l")

legend.Draw("SAME")

c.SaveAs("frameEE.pdf")

c.SaveAs("frameEE.png")



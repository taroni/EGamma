import ROOT 
from PeakFitter import *



def FindLeftResolution(function,fitter):
    resolution=0.
    err=0.
    division = 1000
    resolP=0.
    resolM=0.

    xbool=False
    for ix in range (1,division):
        a =  float(ix-1)/float(division)
        b =  ix/float(division)
        peak = func.GetParameter(1)
        partialIntegral=function.Integral(peak-b, peak)
        if partialIntegral>=0.683*function.Integral(-1,peak):
            for iy in range(1, division):
                c=iy*(b-a)/float(division)
                
                if function.Integral(peak-a-c, peak)>=0.683*function.Integral(-1,peak):
                    resolution=a+c     
                    params = function.GetParameters()
                    covMat = fitter.GetCovarianceMatrix()
                    err=function.IntegralError(peak-a-c, peak, params, covMat )
                    xbool=True
                    break
        if xbool==True : break
    return resolution
def FindRightResolution(function,fitter):
    resolution=0.
    err=0.
    division = 1000
    resolP=0.
    resolM=0.
    
    xbool=False
    for ix in range (1,division):
        a =  float(ix-1)/float(division)
        b =  ix/float(division)
        peak = func.GetParameter(1)
        partialIntegral=function.Integral(peak, peak+b) 
        if partialIntegral>=0.683*function.Integral(peak,1):
            for iy in range(1, division):
                c=iy*(b-a)/float(division)
                
                if function.Integral(peak, peak+a+c)>=0.683*function.Integral(peak,1):
                    resolution=a+c     
                    params = function.GetParameters()
                    covMat = fitter.GetCovarianceMatrix()
                    err=function.IntegralError(peak, peak+a+c, params, covMat )
                    xbool=True
                    break
        if xbool==True : break
    return resolution

def FindResolution(function,fitter):
    resolution=0.
    err=0.
    division = 1000
    resolP=0.
    resolM=0.
    
    xbool=False
    for ix in range (1,division):
        a =  float(ix-1)/float(division)
        b =  ix/float(division)
        peak = func.GetParameter(1)
        partialIntegral=function.Integral(peak-b, peak+ b) 
        if partialIntegral>=0.683*function.Integral(-1,1):
            for iy in range(1, division):
                c=iy*(b-a)/float(division)
                
                if function.Integral(peak-a-c, peak+a+c)>=0.683*function.Integral(-1,1):
                    resolution=peak+a+c     
                    params = function.GetParameters()
                    covMat = fitter.GetCovarianceMatrix()
                    err=function.IntegralError(peak-a-c, peak+a+c, params, covMat )
                    xbool=True
                    break
        if xbool==True : break
                
    xbool=False
    for ix in range (1,division):
        a =  float(ix-1)/float(division)
        b =  ix/float(division)
        peak = func.GetParameter(1)
        partialIntegral=function.Integral(peak-b, peak+b) 
        # print ix, function.Integral(-resolution, resolution)+err, function.Integral(-b,b)
        if partialIntegral>=function.Integral(peak-resolution,peak+resolution)+err:
            for iy in range(1, division):
                c=iy*(b-a)/float(division)

                if function.Integral(peak-a-c, peak+a+c)-(function.Integral(peak-resolution, peak+resolution)+err)>=0:
                    resolP=resolution-(a+c)
                    xbool=True
                    break
        if xbool==True : break
    xbool=False  
    for ix in range (1,division):
        a =  float(ix-1)/float(division)
        b =  ix/float(division)
        peak = func.GetParameter(1)
        partialIntegral=function.Integral(peak-b, peak+b) 
        if partialIntegral>=function.Integral(peak-resolution,peak+resolution)-err:
            for iy in range(1, division):
                c=iy*(b-a)/float(division)
                if function.Integral(peak-a-c, peak+a+c)-(function.Integral(peak-resolution, peak+resolution)-err)>=0: 
                #if function.Integral(-a-c, a+c)-err>=0.683*function.Integral(-1,1):
                    resolM=resolution-(a+c)
                    xbool=True
                    break
        if xbool : break
       
    return (resolution,resolM, resolP)

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
ROOT.gStyle.SetTitleYOffset(1.35);

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
histoEB_2015.SetName('TotalEB_2015')
histoEB.GetXaxis().SetTitle("Resolution=(E_{T} - L1)/E_{T}")
histoEB.GetXaxis().SetTitleOffset(1.2)
histoEB.GetYaxis().SetTitle("a.u.")
histoEB.Rebin(2) 
histoEB_2015.Rebin(2) 
fit = doubleCBFit(histoEB,3.)


c= ROOT.TCanvas("c", "c", 700, 700) 
c.Draw()
c.SetGridx(0)
c.SetGridy(0)
histoEB.Draw('EP')
#histoEB.GetYaxis().SetRangeUser(0,0.05)
histoEB.GetYaxis().SetRangeUser(0,0.1)

func=fit[0]
func.SetLineColor(1)
func.SetNpx(10000)
#func.Draw("SAME")
#print "BARREL:",func.GetMaximumX(-1, 1), FindResolution(fit)
print "BARREL:",func.GetParameter(1), '+/-', func.GetParError(1) ,FindLeftResolution(fit[0], fit[1]), FindRightResolution(fit[0], fit[1]) #FindResolution(fit[0], fit[1])
peak=func.GetParameter(1)
line0= ROOT.TLine(peak-FindLeftResolution(fit[0], fit[1]), 0, peak-FindLeftResolution(fit[0], fit[1]), 0.03) 
line1= ROOT.TLine(peak+FindRightResolution(fit[0], fit[1]), 0, peak+FindRightResolution(fit[0], fit[1]), 0.03) 

#line0.Draw("SAME")
#line1.Draw("SAME")

fit2 = doubleCBFit(histoEB_2015,3.)
histoEB_2015.Draw("EPSAME")
histoEB_2015.SetLineColor(2)
histoEB_2015.SetMarkerColor(2)
func2=fit2[0]
func2.SetNpx(10000)
func2.SetLineColor(2)
func2.Draw("SAME")

legend = ROOT.TLegend(0.47,0.9,0.89,0.65)
legend.SetFillColor(0)
legend.SetLineColor(0)
legend.SetTextSize(0.04)
legend.AddEntry('', 'Z #rightarrow e^{+} e^{-}', '')
legend.AddEntry('', 'ECAL Barrel', '')

legend.AddEntry(histoEB, "2012 data, #sqrt{s}= 8 TeV","l")
#legend.AddEntry(None, "#sqrt{s}= 8 TeV","")
legend.AddEntry(histoEB_2015, "2015 data, #sqrt{s}=13 TeV", "l")
#legend.AddEntry(None, "#sqrt{s}=13 TeV", "")
histoEB.Draw('EP')
histoEB.GetXaxis().SetRangeUser(-0.4,1.)
func.Draw("SAME")
histoEB_2015.Draw("EPSAME")
func2.Draw("SAME")
legend.Draw("SAME")

#text = ROOT.TLatex(-0.4, .0505, "CMS Preliminary")
text = ROOT.TLatex(-0.35, .093, "CMS")
text.SetTextSize(0.04);
#text.SetTextFont(42);
text.Draw("same");
textp = ROOT.TLatex(-0.35, .088, "Preliminary")
textp.SetTextSize(0.03);
textp.SetTextFont(52);
textp.Draw("same");


#texts=ROOT.TLatex(0.6, 0.0505, '#sqrt{s}=13 TeV')
#texts.SetTextSize(0.03);
#texts.SetTextFont(42);
#texts.Draw("same");
c.SaveAs("fitResultEB_func.png")
c.SaveAs("fitResultEB_func.pdf")
##histoEB.GetXaxis().SetRangeUser(-0.4, 0.4)
##text = ROOT.TLatex(-0.4, .0505, "CMS Preliminary")
##text.SetTextSize(0.03);
##text.SetTextFont(42);
##text.Draw("same");
###texts=ROOT.TLatex(0.25, 0.0505, '#sqrt{s}=13 TeV')
###texts.SetTextSize(0.03);
###texts.SetTextFont(42);
###texts.Draw("same");
##c.SaveAs("fitResultEB_zoomed.pdf")
##c.SaveAs("fitResultEB_zoomed.png")
##c.SaveAs("fitResultEB_zoomed.root")


histoEE=filein.Get("TotalEE")
histoEE_2015 = filein1.Get("TotalEE")
histoEE_2015.SetName('TotalEE_2015')
histoEE_2015.Rebin(2)
histoEE.Rebin(2) 
fit= doubleCBFit(histoEE,3.)

histoEE.Draw('EP')
#histoEE.GetYaxis().SetRangeUser(0,0.04)
histoEE.GetYaxis().SetRangeUser(0,0.08)
histoEE.GetXaxis().SetTitle("Resolution=(E_{T} - L1)/E_{T}")
histoEE.GetXaxis().SetTitleOffset(1.2)
#histoEE.GetYaxis().SetTitle("Events/0.005")
histoEE.GetYaxis().SetTitle("a.u.")
func=fit[0]
func.SetLineColor(1)
func.Draw("SAME")
peak=func.GetParameter(1)
line0= ROOT.TLine(peak-FindLeftResolution(fit[0], fit[1]), 0, peak-FindLeftResolution(fit[0], fit[1]), 0.03) 
line1= ROOT.TLine(peak+FindRightResolution(fit[0], fit[1]), 0, peak+FindRightResolution(fit[0], fit[1]), 0.03) 
print peak, peak-FindLeftResolution(fit[0], fit[1]), peak+FindLeftResolution(fit[0], fit[1])
#line0.Draw("SAME")
#line1.Draw("SAME")

#print "ENDCAPS:", func.GetMaximumX(-1, 1), FindResolution(fit)
print "ENDCAPS:", func.GetParameter(1), '+/-', func.GetParError(1),FindLeftResolution(fit[0], fit[1]), FindRightResolution(fit[0], fit[1])


m          = ROOT.RooRealVar('res', 'Resolution=(E_{T} - L1)/E_{T}',0., -0.4, 1.)
mean       = ROOT.RooRealVar('mean', 'mean', 0,-0.4,1.)
sigmaL     = ROOT.RooRealVar('sigmaL' ,'sigmaL' ,0.1 ,0 ,0.5)
sigmaR     = ROOT.RooRealVar('sigmaR' ,'sigmaR' ,0.1 ,0 ,0.5)
alphaL     = ROOT.RooRealVar('alphaL' ,'alphaL' ,1  ,0 ,30 )
alphaR     = ROOT.RooRealVar('alphaR' ,'alphaR' ,1  ,0 ,30 )
m.setRange("fitrange", -0.2,0.5)
frame = m.frame(ROOT.RooFit.Title("2015"))

myhist = ROOT.RooDataHist(histoEE_2015.GetName(),histoEE_2015.GetName(),ROOT.RooArgList(m), histoEE_2015,1.)
myhist.plotOn(frame)
myhist.plotOn(frame).GetYaxis().SetTitle("a. u.") 

func2= ROOT.RooCruijff( 'func2', 'func2', m, mean, sigmaL, sigmaR, alphaL, alphaR)
fitresult= func2.fitTo(
    myhist,
    ROOT.RooFit.Save(True),
    ROOT.RooFit.Range("fitrange")
)
m.setRange("plotrange", -0.4,1.)
func2.plotOn(frame, ROOT.RooFit.LineColor(2))
#fit2 = doubleCBFit(histoEE_2015,3.)
histoEE_2015.Draw("EP")
histoEE_2015.SetLineColor(2)
histoEE_2015.SetMarkerColor(2)
#func2=fit2[0]
#func2.SetLineColor(2)
#func2.Draw("SAME")
legend.Clear()
legend.AddEntry(None, 'Z #rightarrow e^{+} e^{-}', '')
legend.AddEntry(None, 'ECAL Endcaps', '')
legend.AddEntry(histoEE, "2012 data, #sqrt{s}= 8 TeV","l")
#legend.AddEntry(None, "#sqrt{s}= 8 TeV","")
legend.AddEntry(histoEE_2015, "2015 data, #sqrt{s}=13 TeV", "l")
#legend.AddEntry(None, "#sqrt{s}=13 TeV", "")

histoEE.Draw('EP')
histoEE.GetXaxis().SetRangeUser(-0.4,1.)
func.Draw("SAME")
histoEE_2015.Draw("EPSAME")
#func2.Draw("SAME")
legend.Draw("SAME")
text.Draw("same");
#text = ROOT.TLatex(-0.4, .0405, "CMS Preliminary")
#text = ROOT.TLatex(-0.4, .801, "CMS Preliminary")
text = ROOT.TLatex(-0.35, .074, "CMS")

text.SetTextSize(0.04);
#text.SetTextFont(42);
text.Draw("same");
textp = ROOT.TLatex(-0.35, .070, "Preliminary")
textp.SetTextSize(0.03);
textp.SetTextFont(52);
textp.Draw("same");

#texts=ROOT.TLatex(0.6, 0.0405, '#sqrt{s}=13 TeV')#, Z #rightarrow ee')
#event=ROOT.TLatex(-0.9, 0.038, ' Z #rightarrow ee')
##texts.SetTextSize(0.03);
#texts.SetTextFont(42);
#texts.Draw("same");
#event.SetTextSize(0.03);
#event.SetTextFont(42);
#event.Draw("SAME")
c.SaveAs("fitResultEE_func.png")
c.SaveAs("fitResultEE_func.pdf")
c.SaveAs("fitResultEE_func.root")
c.Clear()
frame.Draw()
c.SaveAs("frame.pdf")

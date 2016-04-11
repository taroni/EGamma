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
                #print iy, c, function.Integral(-resolution, resolution)+err,function.Integral(-b, b),function.Integral(-a, a),function.Integral(-a-c, a+c),function.Integral(-a-c, a+c)-(function.Integral(-resolution, resolution)+err)
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


#ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptTitle(0)
#ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetTitleXOffset(1.)
ROOT.gStyle.SetTitleYOffset(2.)
ROOT.gStyle.SetLabelOffset(0.01, "XYZ")
ROOT.gStyle.SetPadLeftMargin(0.2)
ROOT.gStyle.SetPadRightMargin(0.1)
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
ROOT.gStyle.SetHistLineWidth(1);
ROOT.gStyle.SetEndErrorSize(2);
ROOT.gStyle.SetErrorX(0.);
ROOT.gStyle.SetMarkerStyle(20);

##For the fit/function:
ROOT.gStyle.SetOptFit(1);
ROOT.gStyle.SetFitFormat("5.4g");
ROOT.gStyle.SetFuncColor(2);
ROOT.gStyle.SetFuncStyle(1);
ROOT.gStyle.SetFuncWidth(1);

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
ROOT.gStyle.SetTitleYOffset(1.25);

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
#outfile=ROOT.TFile("output_reso.root","RECREATE");
#title_name_string=("2012D","2015D")
title_name_string=("2012D")
Brange=18
histoEB = filein.Get("TotalEB")
Etabinreso = filein.Get("Etabinreso")
histoEB.GetXaxis().SetTitle("Resolution")
histoEB.GetYaxis().SetTitle("Events/0.005")

c= ROOT.TCanvas("c", "c", 700, 700) 
c.Draw()
xvalue=[0 for x in range(0,Brange)]
yvalue=[0 for x in range(0,Brange)]
yerrorup=[0 for x in range(0,Brange)]
yerrordown=[0 for x in range(0,Brange)]
Bwidth=5.0/Brange
Etareso= ROOT.TGraphAsymmErrors(Brange)
Etareso.SetName("Run2015D") 
for binnumber in range(0,Brange):
   hist=filein.Get("Histbin_%d" %(binnumber+1))
   hist.Sumw2()
   fit= doubleCBFit(hist,6.)
   hist.GetXaxis().SetTitle("Resolution")
   hist.GetYaxis().SetTitle("Events/0.005")
   hist.SetTitle(title_name_string+", #eta bin range "+str(round(-2.5+binnumber*Bwidth,2))+" to "+ str(round(-2.5+(binnumber+1)*Bwidth,2)) )
   func=fit[0]
   peak = func.GetParameter(1)
   #hist.Draw()
   #func.SetLineColor(4)
   #func.Draw("SAME")
   #c.Update()
#   c.SaveAs("Etareso"+title_name_string+"/Histbin_"+str(binnumber)+".pdf")
#   c.SaveAs("Etareso"+title_name_string+"/Histbin_"+str(binnumber)+".png")
  # value=FindResolution(fit[0],fit[1])
   yvalue[binnumber]=peak
   yerrorup[binnumber]=FindRightResolution(fit[0], fit[1])
   yerrordown[binnumber]=FindLeftResolution(fit[0], fit[1])
#   print yvalue[binnumber], yerrorup[binnumber],yerrordown[binnumber]
   xvalue[binnumber]=-2.5+(5.0/Brange)*binnumber+5.0/2/Brange
   Etareso.SetPoint(binnumber,xvalue[binnumber],yvalue[binnumber])
   Etareso.SetPointEYhigh(binnumber,abs(yerrorup[binnumber]))
   Etareso.SetPointEYlow(binnumber,abs(yerrordown[binnumber]))
   
Etareso.SetMarkerColor(2)
Etareso.SetLineColor(2)
Etareso.SetMarkerStyle(20)
#Etareso.SetTitle("Resolution vs #eta")
text = ROOT.TLatex(-2.8, .315, "CMS Preliminary")
text.SetTextSize(0.04)
text.SetTextFont(42)


Etareso1= ROOT.TGraphAsymmErrors(Brange)
Etareso1.SetName("Run2012D") 
for binnumber in range(0,Brange):
   hist=filein1.Get("Histbin_%d" %(binnumber+1))
   hist.Sumw2()
   fit= doubleCBFit(hist,10.)
   hist.GetXaxis().SetTitle("Resolution")
   hist.GetYaxis().SetTitle("Events/0.005")
   hist.SetTitle(title_name_string+", #eta bin range "+str(round(-2.5+binnumber*Bwidth,2))+" to "+ str(round(-2.5+(binnumber+1)*Bwidth,2)) )
   func=fit[0]
   peak = func.GetParameter(1)
   #hist.Draw()
   #func.SetLineColor(4)
   #func.Draw("SAME")
   #c.Update()
#   c.SaveAs("Etareso"+title_name_string+"/Histbin_"+str(binnumber)+".pdf")
#   c.SaveAs("Etareso"+title_name_string+"/Histbin_"+str(binnumber)+".png")
  # value=FindResolution(fit[0],fit[1])
   yvalue[binnumber]=peak
   yerrorup[binnumber]=FindRightResolution(fit[0], fit[1])
   yerrordown[binnumber]=FindLeftResolution(fit[0], fit[1])
#   print yvalue[binnumber], yerrorup[binnumber],yerrordown[binnumber]
   xvalue[binnumber]=-2.5+(5.0/Brange)*binnumber+5.0/2/Brange
   Etareso1.SetPoint(binnumber,xvalue[binnumber],yvalue[binnumber])
   Etareso1.SetPointEYhigh(binnumber,abs(yerrorup[binnumber]))
   Etareso1.SetPointEYlow(binnumber,abs(yerrordown[binnumber]))
Etareso1.SetTitle("2012 data, #sqrt{s}= 8 TeV")
Etareso.SetTitle("2015 data, #sqrt{s}=13 TeV")

legend = ROOT.TLegend(0.35,0.85,0.75,0.75)
legend.SetFillColor(0)
legend.SetLineColor(0)
#legend.SetTextFont(70)
legend.AddEntry(Etareso, Etareso.GetTitle(),"lp")
legend.AddEntry(Etareso1, Etareso1.GetTitle(),"lp")

Etareso1.SetMarkerStyle(21)
Etareso1.SetMarkerColor(1)
Etareso1.SetLineColor (1)
Etareso1.GetYaxis().SetTitle("Resolution=(E_{T}-L1)/E_{T}")
Etareso1.GetXaxis().SetTitle("#eta")
Etareso1.Draw("AP")
Etareso1.SetMaximum(0.31)
Etareso1.SetMinimum(-0.15)
Etareso.Draw("P")
text.Draw("same")
legend.Draw()
c.Update()
c.SaveAs("Etareso_2015_2012.png")
c.SaveAs("Etareso_2015_2012.pdf")

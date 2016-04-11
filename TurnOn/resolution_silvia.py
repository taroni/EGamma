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



ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptTitle(0)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetTitleXOffset(1.)
ROOT.gStyle.SetTitleYOffset(2.)
ROOT.gStyle.SetLabelOffset(0.01, "XYZ")
ROOT.gStyle.SetPadLeftMargin(0.2)
ROOT.gStyle.SetPadRightMargin(0.1)
ROOT.gStyle.SetHistLineWidth(2)


filein=ROOT.TFile.Open("outfile2012.root", "READ")
histoEB = filein.Get("TotalEB")
histoEB.GetXaxis().SetTitle("Resolution")
histoEB.GetYaxis().SetTitle("Events/0.005")
#histoEB.Rebin(2) 

fit = doubleCBFit(histoEB,3.)


c= ROOT.TCanvas("c", "c", 700, 700) 
c.Draw()
c.SetGridx(1)
c.SetGridy(1)
histoEB.Draw()

func=fit[0]
func.SetLineColor(4)
func.Draw("SAME")
#print "BARREL:",func.GetMaximumX(-1, 1), FindResolution(fit)
print "BARREL:",func.GetParameter(1), '+/-', func.GetParError(1) ,FindLeftResolution(fit[0], fit[1]), FindRightResolution(fit[0], fit[1]) #FindResolution(fit[0], fit[1])
peak=func.GetParameter(1)
line0= ROOT.TLine(peak-FindLeftResolution(fit[0], fit[1]), 0, peak-FindLeftResolution(fit[0], fit[1]), 0.03) 
line1= ROOT.TLine(peak+FindRightResolution(fit[0], fit[1]), 0, peak+FindRightResolution(fit[0], fit[1]), 0.03) 

line0.Draw("SAME")
line1.Draw("SAME")

c.SaveAs("fitResultEB_2012.png")
histoEB.GetXaxis().SetRangeUser(-0.4, 0.4)
c.SaveAs("fitResultEB_2012_zoomed.png")
c.SaveAs("fitResultEB_2012_zoomed.root")


histoEE=filein.Get("TotalEE")
fit= doubleCBFit(histoEE,5.)
#histoEE.Rebin(2) 
histoEE.Draw()
histoEE.GetXaxis().SetTitle("Resolution")
histoEE.GetYaxis().SetTitle("Events/0.005")
func=fit[0]
func.SetLineColor(4)
func.Draw("SAME")
peak=func.GetParameter(1)
line0= ROOT.TLine(peak-FindLeftResolution(fit[0], fit[1]), 0, peak-FindLeftResolution(fit[0], fit[1]), 0.03) 
line1= ROOT.TLine(peak+FindRightResolution(fit[0], fit[1]), 0, peak+FindRightResolution(fit[0], fit[1]), 0.03) 
print peak, peak-FindLeftResolution(fit[0], fit[1]), peak+FindLeftResolution(fit[0], fit[1])
line0.Draw("SAME")
line1.Draw("SAME")

#print "ENDCAPS:", func.GetMaximumX(-1, 1), FindResolution(fit)
print "ENDCAPS:", func.GetParameter(1), '+/-', func.GetParError(1),FindLeftResolution(fit[0], fit[1]), FindRightResolution(fit[0], fit[1])
c.SaveAs("fitResultEE_2012.png")
c.SaveAs("fitResultEE_2012.root")

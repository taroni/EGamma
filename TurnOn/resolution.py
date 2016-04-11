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
#ROOT.gStyle.SetOptTitle(0)
#ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetTitleXOffset(1.)
ROOT.gStyle.SetTitleYOffset(2.)
ROOT.gStyle.SetLabelOffset(0.01, "XYZ")
ROOT.gStyle.SetPadLeftMargin(0.2)
ROOT.gStyle.SetPadRightMargin(0.1)
ROOT.gStyle.SetHistLineWidth(2)


filein=[ROOT.TFile.Open("2012D.root", "READ"),ROOT.TFile.Open("2015D.root", "READ")]
title_name_string=['2012D','2015D']
lumi=(8,2)
#title_name_string=("2015D")
Brange=18
#histoEB = filein.Get("TotalEB")
#Etabinreso = filein.Get("Etabinreso")
#histoEB.GetXaxis().SetTitle("Resolution")
#histoEB.GetYaxis().SetTitle("Events/0.005")
#fit = doubleCBFit(histoEB,5.)
#func=fit[0]
#func.SetLineColor(4)
#print "BARREL:",func.GetParameter(1), '+/-', func.GetParError(1) , FindResolution(fit[0], fit[1])
c= ROOT.TCanvas("c", "c", 700, 700) 
c.Draw()


#filein1=ROOT.TFile.Open("2015D.root", "READ")
#Etareso=TH1F("Etareso","Etareso",25,-2.5,2.5)
xvalue=[0 for x in range(0,Brange)]
yvalue=[0 for x in range(0,Brange)]
yerrorup=[0 for x in range(0,Brange)]
yerrordown=[0 for x in range(0,Brange)]
Bwidth=5.0/Brange
groupdata=2
Etareso=(ROOT.TGraphAsymmErrors(Brange),ROOT.TGraphAsymmErrors(Brange))
for i in range(0,groupdata):
   for binnumber in range(0,Brange):
      hist=filein[i].Get("Histbin_%d" %(binnumber+1))
      hist.Sumw2()
      fit= doubleCBFit(hist,3.)
      #print fit[0], fit[1]
      hist.GetXaxis().SetTitle("Resolution")
      hist.GetYaxis().SetTitle("Events/0.005")
      hist.SetTitle(title_name_string[i]+", #eta bin range "+str(round(-2.5+binnumber*Bwidth,2))+" to "+ str(round(-2.5+(binnumber+1)*Bwidth,2)) )
      func=fit[0]
      peak = func.GetParameter(1)
   #   hist.Draw()
   #   func.SetLineColor(4)
   #   func.Draw("SAME")
   #   c.Update()
   #   c.SaveAs("Etareso"+title_name_string[i]+"/Histbin_"+str(binnumber)+".pdf")
   #   c.SaveAs("Etareso"+title_name_string[i]+"/Histbin_"+str(binnumber)+".png")
     # value=FindResolution(fit[0],fit[1])
      yvalue[binnumber]=peak
      yerrorup[binnumber]=FindRightResolution(fit[0], fit[1])
      yerrordown[binnumber]=FindLeftResolution(fit[0], fit[1])
   #   print yvalue[binnumber], yerrorup[binnumber],yerrordown[binnumber]
      xvalue[binnumber]=-2.5+(5.0/Brange)*binnumber+5.0/2/Brange
      Etareso[i].SetPoint(binnumber,xvalue[binnumber],yvalue[binnumber])
      Etareso[i].SetPointEYhigh(binnumber,abs(yerrorup[binnumber]))
      Etareso[i].SetPointEYlow(binnumber,abs(yerrordown[binnumber]))
      
   #pyplot.errorbar(xvalue,yvalue, yerr=(yerrordown,yerrorup),linestyle='',marker='o')
   #pyplot.savefig('Etaresolution_2015D.png')
   #   Etareso.SetBinError(binnumber,value[1])
   #   Etareso.SetBinContent(binnumber,FindResolution(fit[0], fit[1]))
   #Etareso=ROOT.TGraphAsymmErrors(20,xvalue,yvalue,0,0,yerrordown,yerrorup);   
   Etareso[i].SetMarkerStyle(20+i)
   Etareso[i].SetMarkerStyle(1-i)
#   if i==1 and groupdata==2:
#      Etareso[i].Draw("AP SAME")
#   else:
#      text =ROOT.TLatex(-1,0.0405,"CMS Preliminary");
#      text.SetTextSize(0.03);
#      text.SetTextFont(42);
#      if lumi[i]>0:
#         texts =ROOT.TLatex(5,1.06,"CMS Preliminary, #sqrt{s}=13 TeV, Z#rightarrowee"+str(lumi[i])+" pb^{-1}");
   #   Etareso[i].SetTitle("Resolution vs #eta")
   Etareso[i].GetYaxis().SetTitle("Resolution")
   Etareso[i].GetXaxis().SetTitle("#eta")
#   if i==0:
#      Etareso[i].Draw("AP")
#   else:
#      Etareso[i].Draw("AP same")
    #  texts=ROOT.TLatex(0.3, 0.0405, '#sqrt{s}=13 TeV, Z #rightarrow ee')
 #     texts.SetTextSize(0.03)
 #     texts.SetTextFont(42)
 #     text.Draw("same")
 #     texts.Draw("same")
     # c.SaveAs("Etareso"+title_name_string[i]+"/Etaresolution_"+title_name_string[i]+".png")
     # c.SaveAs("Etareso"+title_name_string[i]+"/Etaresolution_"+title_name_string[i]+".pdf")
text =ROOT.TLatex(-1,0.0405,"CMS Preliminary");
text.SetTextSize(0.03);
text.SetTextFont(42);
text.Draw("SAME")
Etareso[0].Draw("AP")

if groupdata==2:
   c.SaveAs("Etareso_2012vs2015.png")
   c.SaveAs("Etareso_2012vs2015.pdf")
#else:
# c.SaveAs("Etareso"+title_name_string+"/Etaresolution_"+title_name_string+".png")
#c.SaveAs("Etareso"+title_name_string+"Etaresolution_"+title_name_string+".pdf")
#print "ENDCAPS:", func.GetMaximumX(-1, 1), FindResolution(fit)

#print "ENDCAPS:", func.GetParameter(1), '+/-', func.GetParError(1) ,FindResolution(fit[0], fit[1])

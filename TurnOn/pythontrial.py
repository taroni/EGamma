from ROOT import *
from PeakFitter import *
import matplotlib.pyplot as pyplot


n=10
xvalue = (-0.22, 0.05, 0.25, 0.35, 0.5, 0.61,0.7,0.85,0.89,0.95)
yvalue= (1,2.9,5.6,-7.4,9,9.6,8.7,-6.3,4.5,1)
exl = (.05,.1,.07,.07,.04,.05,.06,.07,.08,.05)
yerrordown= (.8,.7,.6,.5,.4,.4,.5,.6,.7,.8)
exh = (.02,.08,.05,.05,.03,.03,.04,.05,.06,.03)
yerrorup= (.6,.5,.4,.3,.2,.2,.3,.4,.5,.6)
Etareso= ROOT.TGraphAsymmErrors(20)
c= ROOT.TCanvas("c", "c", 700, 700)
c.Draw()
#tareso=ROOT.TGraphAsymmErrors(n,x,y,exl,exh,eyl,eyh)
for binnumber in range(0,10):
  Etareso.SetPoint(binnumber,xvalue[binnumber],yvalue[binnumber])
  Etareso.SetPointEYhigh(binnumber,abs(yerrorup[binnumber]))
  Etareso.SetPointEYlow(binnumber,abs(yerrordown[binnumber]))
#gr =TGraph(x,y,exl,exh,eyl,eyh)
#gr.Draw("ALP")
#tareso.Draw()
#pyplot.errorbar(x,y, yerr=(eyl,eyh),linestyle='', marker='o')
#pyplot.savefig('example01.png') 
Etareso.SetMarkerStyle(20)
Etareso.Draw("lego""P""A")
c.SaveAs("zero.png")

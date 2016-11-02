import math
import ROOT
import ROOT.RooRealVar
import ROOT.RooRealConstant
import ROOT.RooAbsData
import ROOT.RooArgSet
import ROOT.TH1


def RooCruijff(name,title, _x,  _m0, _sigmaL,  _sigmaR,_alphaL, _alphaR):
    RooCruijff()=ROOT.RooAbsPdf()
    
def evaluate() :
        sigma = 0.0
        alpha = 0.0
        dx = (x - m0)
        if dx<0 :
            sigma = sigmaL
            alpha = alphaL
        else:
            sigma = sigmaR
            alpha = alphaR
        double f = 2*sigma*sigma + alpha*dx*dx 
        return math.exp(-dx*dx/f) 

    def estimateParameters( data, errorFactor) :
            print  "RooCruijff::Estimating parameters"
            print "Printing x: "
            print this.Print("v");
            print "x: ", x 
            print "var: ", x.arg().GetName()

            histo= ROOT.TH1(data.createHistogram(x.arg().GetName()));
            
            print "Setting m0"
            m0 = histo.GetBinCenter(histo.GetMaximumBin());
            sigmaL = histo.GetRMS();
            sigmaR = histo.GetRMS();
            return ROOT.RooArgSet(m0.arg(), sigmaR.arg(), sigmaL.arg());
        

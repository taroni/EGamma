import ROOT
import math
import numpy

def median(histo):
    nbins = histo.GetNbinsX()
    totalIntegral = histo.Integral(0,nbins+2)
    medianBin = 0
    sumBins = 0.
    for b in range(0,nbins+1):
        sumBins += histo.GetBinContent(b)
        if(sumBins>=0.5*totalIntegral):
            medianBin = b
            break
    median = histo.GetXaxis().GetBinCenter(medianBin)
    medianError = histo.GetXaxis().GetBinWidth(medianBin)/2.
    return (median,medianError)

def effectiveRMS(histo, fraction=0.683, fitrebin=1):
    histoCopy = histo.Clone(histo.GetName()+"_copy")
    histoCopy.__class__ = ROOT.TH1F
    histoCopy.SetDirectory(0)
    nbins = histoCopy.GetNbinsX()

    fit = doubleCBFit(histoCopy,3.,fitrebin)
    maxX = fit.GetParameter(1)
    #maxBinX = histo.GetMaximumBin()
    maxBinX = histoCopy.GetXaxis().FindBin(maxX)
    binWidth = histoCopy.GetXaxis().GetBinWidth(maxBinX)
    histoCopy.Delete()

    indexRMSLeftList = []
    indexRMSRightList = []
    rmsList = []

    nTries = 20
    random = ROOT.TRandom3()
    goLeft = True
    for n in range(0,nTries):
        histoCopy = histo.Clone(histo.GetName()+"_copy")
        histoCopy.__class__ = ROOT.TH1F
        histoCopy.SetDirectory(0)
        for b in range(1, nbins+1):
            newValue = random.Poisson(histoCopy.GetBinContent(b))
            histoCopy.SetBinContent(b, newValue)
        totalIntegral = histoCopy.Integral(0,nbins+1)
        #print "Integral = ", totalIntegral
        sumBins = 0.
        sumErrorBins = 0.
        indexRMSLeft = 0
        indexRMSRight = 0
        indexLeft = 0
        indexRight = 0
        sumBins += histoCopy.GetBinContent(maxBinX)/totalIntegral
        for b in range(0,nbins+1):
            #print "indexright = ", indexRight
            #print "indexleft = ", indexLeft
            bRight = maxBinX + indexRight + 1
            bLeft = maxBinX - indexLeft - 1
            if bRight>nbins:
                print "WARNING: effectiveRMS: bin reached histo boundary"
                binRMSRight = bRight
                binRMSLeft = bLeft
                break;
            if bLeft<=0:
                print "WARNING: effectiveRMS: bin reached histo boundary"
                binRMSRight = bRight
                binRMSLeft = bLeft
                break

            nRight = histoCopy.GetBinContent(bRight)
            nLeft  = histoCopy.GetBinContent(bLeft)
 
            #print "nright = ", nRight
            #print "nleft = ", nLeft
            if nLeft>nRight:
                sumBins += nLeft/totalIntegral
                indexLeft += 1
            elif nRight>nLeft:
                sumBins += nRight/totalIntegral
                indexRight += 1
            else:
                if goLeft:
                    sumBins += nLeft/totalIntegral
                    indexLeft += 1
                    goLeft = False
                else:
                    sumBins += nRight/totalIntegral
                    indexRight += 1
                    goLeft = True

            if sumBins >= fraction:
                indexRMSLeft  = indexLeft
                indexRMSRight  = indexRight
                break


        xLeft = histoCopy.GetXaxis().GetBinCenter(maxBinX - indexRMSLeft)
        xRight = histoCopy.GetXaxis().GetBinCenter(maxBinX + indexRMSRight)
        print xLeft, xRight
        #binError = 0.5
        rms = (xRight - xLeft)/2.
        rmsList.append(rms)
        #rmsError = (histoCopy.GetXaxis().GetBinWidth(maxBinX + indexRMSRight)*binError + histoCopy.GetXaxis().GetBinWidth(maxBinX - indexRMSLeft)*binError)/2.
    print  rmsList
    rms = numpy.mean(rmsList)
    rmsError = max(numpy.std(rmsList), 1./math.sqrt(12.)*binWidth)
    print rms, rmsError

    return (rms,rmsError)



def gaussianFit(histo, rangeInSigma=1.):
    mean = histo.GetMean()
    rms = histo.GetRMS()
    fit = ROOT.TF1("gaussFit", "gaus", mean-rangeInSigma*rms, mean+rangeInSigma*rms)
    histo.Fit(fit, "RN")
    mean = fit.GetParameter(1)
    rms = fit.GetParameter(2)
    fit = ROOT.TF1("gaussFit", "gaus", mean-rangeInSigma*rms, mean+rangeInSigma*rms)
    histo.Fit(fit, "RN")
    mean = fit.GetParameter(1)
    rms = fit.GetParameter(2)
    par0 = fit.GetParameter(0)
    fit = ROOT.TF1("gaussFit", "gaus", mean-rangeInSigma*rms, mean+rangeInSigma*rms)
    fit.SetParameter(0,par0)
    fit.SetParameter(1,mean)
    fit.SetParameter(2,rms)
    histo.Fit(fit, "RN")
    return fit

def DoubleCB(x, par):
    xx = x[0]
    norm = par[0]
    mean = par[1]
    width = par[2]
    alpha1 = par[3]
    n1 = par[4]
    alpha2 = par[5]
    n2 = par[6]
    t = (xx-mean)/width
    if t>-alpha1 and t<alpha2:
        return norm*math.exp(-0.5*t*t)
    elif t<=-alpha1:
        A1 = ((n1/abs(alpha1))**n1)*math.exp(-alpha1*alpha1/2.)
        B1 = n1/abs(alpha1)-abs(alpha1)
        return norm*A1*(B1-t)**(-n1)
    else:# t>=alpha2:
        A2 = (n2/abs(alpha2))**n2*math.exp(-alpha2*alpha2/2.)
        B2 = n2/abs(alpha2)-abs(alpha2)
        return norm*A2*(B2+t)**(-n2)



def doubleCBFit(histo, rangeInSigma=2., fitrebin=1):
    histoCopy = histo.Clone(histo.GetName()+"_copy")
    histoCopy.__class__ = ROOT.TH1F
    histoCopy.SetDirectory(0)
    mean = histoCopy.GetMean()
    rms = histoCopy.GetRMS()
    histoCopy.Rebin(fitrebin)
    fit = ROOT.TF1("gaussFit", "gaus", mean-rms, mean+rms)
    fit.SetParameter(1,mean)
    fit.SetParameter(2,rms)
    histoCopy.Fit(fit, "RN")
    mean = fit.GetParameter(1)
    rms = fit.GetParameter(2)
    norm = fit.GetParameter(0)
    fit = ROOT.TF1("gaussFit", "gaus", mean-rms, mean+rms)
    fit.SetParameter(1,mean)
    fit.SetParameter(2,rms)
    histoCopy.Fit(fit, "RN")
    mean = fit.GetParameter(1)
    rms = fit.GetParameter(2)
    norm = fit.GetParameter(0)
    fit = ROOT.TF1("doubleCBFit", DoubleCB, histoCopy.GetBinLowEdge(1), histoCopy.GetBinLowEdge(histoCopy.GetXaxis().GetLast())+histoCopy.GetBin(histoCopy.GetXaxis().GetLast()),7 )
    #fit.SetParLimits(1, 0.5, 1.5)
    #fit.SetParLimits(2, 0.001, 1.)
    #fit.SetParLimits(3, 0.1, 10.)
    #fit.SetParLimits(4, 0., 20.)
    #fit.SetParLimits(5, 0.1, 10.)
    #fit.SetParLimits(6, 0., 20.)
    fit.SetParLimits(1, mean-rms, mean+rms)
    fit.SetParLimits(2, rms/rangeInSigma, rms*rangeInSigma)
    fit.SetParLimits(3, 0.1, 10.)
    fit.SetParLimits(4, 0., 110.)
    fit.SetParLimits(5, 0.1, 10.)
    fit.SetParLimits(6, 0., 30.)
    fit.SetParameter(0,norm)
    fit.SetParameter(1,mean)
    fit.SetParameter(2,rms)
    fit.SetParameter(3, 2.)
    fit.SetParameter(4, 1.)
    fit.SetParameter(5, 1.)
    fit.SetParameter(6, 4.)
    histoCopy.Fit(fit, ""," ",mean-rangeInSigma*rms, mean+rangeInSigma*rms)
    fitter = ROOT.TVirtualFitter.GetFitter();
    histoCopy.Delete()
    return (fit, fitter)


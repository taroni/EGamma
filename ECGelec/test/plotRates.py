import ROOT

filelist =[
'myRateStudy_090.root',
'myRateStudy_080.root',
'myRateStudy_085.root',
'myRateStudy_091.root',
'myRateStudy_092.root',
'myRateStudy_093.root',
'myRateStudy_094.root',
'myRateStudy_095.root'
]

FGlist = [
0.90,
0.80,
0.85,
0.91,
0.92,
0.93,
0.94,
0.95
]


outfile = ROOT.TFile.Open("rateHistos/summaryHistos.root","RECREATE") 
histolist = ["L1_IsoCands", "L1_NonIsoCands", "L1_AllCands"]
graphList=[]
newhistos=[]
diffList=[]
for n,filename in enumerate(filelist):
    graph=[]
    histos=[]
    diffGraph=[]
    for histo in histolist:
        myfile=ROOT.TFile.Open("rateHistos/"+filename, "READ")
       
        h_old = myfile.Get(histo)
        h_new = myfile.Get(histo+"_M")
        print histo
        h_new.SetName(histo+str(FGlist[n])+"_M")
        h_Norm = myfile.Get("h_Entries") 
        
        Norm = h_Norm.Integral()
        print 'Normalization', Norm
        h_old.Scale(1./Norm)
        h_new.Scale(1./Norm)
        
        
        labels = []
        myDiffGraph = ROOT.TGraph()
        myDiffGraph.SetName("rateDiff_"+str(FGlist[n])+h_old.GetName())
        myDiffGraph.SetTitle(str(FGlist[n])+"_"+h_old.GetTitle())
        
        myGraph_old = ROOT.TGraph()
        myGraph_new = ROOT.TGraph()
        myGraph_old.SetName("gr_"+str(FGlist[n])+h_old.GetName())
        myGraph_new.SetName("gr_"+str(FGlist[n])+h_new.GetName())
        myGraph_old.SetTitle(h_old.GetTitle())
        myGraph_new.SetTitle(h_new.GetTitle())
    

        for bin in range(1, h_old.GetXaxis().GetNbins()) :
            if h_old.GetBinContent(bin) == 0: continue
            
            labels.append("EG"+ str(int(h_old.GetXaxis().GetBinCenter(bin)-0.2)))
            #print labels
        
            myGraph_old.SetPoint(len(labels)-1,int(h_old.GetXaxis().GetBinCenter(bin)-0.2),float(h_old.GetBinContent(bin)))
            myGraph_new.SetPoint(len(labels)-1,int(h_old.GetXaxis().GetBinCenter(bin)-0.2),float(h_new.GetBinContent(bin)))
            myDiffGraph.SetPoint(len(labels)-1,int(h_old.GetXaxis().GetBinCenter(bin)-0.2), float(h_old.GetBinContent(bin) - h_new.GetBinContent(bin))/float(h_old.GetBinContent(bin)))
        
        
        
        outfile.cd()
        myGraph_old.Write()
        myGraph_new.Write()
        myDiffGraph.Write()
        h_new.Write()
        histos.append(h_new.Clone())
        #print histos
        graph.append(myGraph_new)
        diffGraph.append(myDiffGraph)
        
    graphList.append(tuple(graph))
    newhistos.append(tuple(histos))
    diffList.append(tuple(diffGraph))

mg_all = ROOT.TMultiGraph()        
mg_iso = ROOT.TMultiGraph()
mg_noniso = ROOT.TMultiGraph()
mg_diff_all = ROOT.TMultiGraph()
mg_diff_iso = ROOT.TMultiGraph()
mg_diff_noniso = ROOT.TMultiGraph()
leg = ROOT.TLegend(0.6, 0.8, 0.8, 0.4)
leg.SetFillColor(0)

for n,gr in enumerate(graphList):
    #print gr
    gr[0].SetMarkerStyle(20)
    gr[0].SetMarkerColor(n+1)
    gr[1].SetMarkerStyle(20)
    gr[1].SetMarkerColor(n+1)
    gr[2].SetMarkerStyle(20)
    gr[2].SetMarkerColor(n+1)
    mg_iso.Add(gr[0])
    mg_noniso.Add(gr[1])
    mg_all.Add(gr[2])
    leg.AddEntry(gr[0], "FG > "+str(FGlist[n]), "p") 


canvas  = ROOT.TCanvas()
canvas.Draw()
canvas.SetGridx(1)
canvas.SetGridy(1)
mg_iso.Draw("AP")
leg.Draw()
canvas.SetName("IsoCandidateRate")
mg_iso.GetXaxis().SetTitle("trig cand E_{T} [GeV]")  
canvas.SaveAs("IsoCandidateRate.pdf") 
canvas.SaveAs("IsoCandidateRate.png") 
canvas.Write()

canvas.Clear()
mg_noniso.Draw("AP")
mg_noniso.GetXaxis().SetTitle("trig cand E_{T} [GeV]")  
leg.Draw()
canvas.SetName("NonIsoCandidateRate")
canvas.SaveAs("NonIsoCandidateRate.pdf") 
canvas.SaveAs("NonIsoCandidateRate.png") 
canvas.Write()

canvas.Clear()
mg_all.Draw("AP")
leg.Draw()
canvas.SetName("AllCandidateRate")
mg_iso.GetXaxis().SetTitle("trig cand E_{T} [GeV]")  
canvas.SaveAs("AllCandidateRate.pdf") 
canvas.SaveAs("AllCandidateRate.png") 
canvas.Write()

mg_all.SetName("AllCandidateRate")
mg_iso.SetName("IsoCandidateRate")
mg_noniso.SetName("NonIsoCandidateRate")
mg_iso.Write()
mg_all.Write()
mg_noniso.Write()

mg_diff_all=ROOT.TMultiGraph()
mg_diff_iso=ROOT.TMultiGraph()
mg_diff_noniso=ROOT.TMultiGraph()

newLeg =  ROOT.TLegend(0.15, 0.95, 0.3, 0.65)
newLeg.SetFillColor(0)

diffallgraphlist=[]
diffisographlist=[]
diffnonisographlist=[]
for n,h in enumerate(newhistos):
    print h
    iso_h = h[0].Clone()
    ##iso_h.Add(newhistos[0][0], -1)
    iso_h.Divide(newhistos[0][0])
    iso_h.SetTitle("Iso Cands, Rate Change") 
    iso_h.SetName("rate_change_iso_"+str(FGlist[n]))
    noniso_h = h[1].Clone()
    ##noniso_h.Add(newhistos[0][1], -1)
    noniso_h.Divide(newhistos[0][1])
    noniso_h.SetTitle("Non Iso Cands, Rate Change") 
    noniso_h.SetName("rate_change_nonIso_"+str(FGlist[n]))

    all_h = h[2].Clone()
    ##noniso_h.Add(newhistos[0][1], -1)
    all_h.Divide(newhistos[0][2])
    all_h.SetTitle("All Cands, Rate Change") 
    all_h.SetName("rate_change_all_"+str(FGlist[n]))
    
    print iso_h.GetName(), noniso_h.GetName()
    allGraph = ROOT.TGraph()
    allGraph.SetName("AllRateChange_"+str(FGlist[n]))

    isoGraph = ROOT.TGraph()
    isoGraph.SetName("IsoRateChange_"+str(FGlist[n]))

    nonIsoGraph = ROOT.TGraph()
    nonIsoGraph.SetName("NonIsoRateChange_"+str(FGlist[n]))

    thepoint =0
    for ibin in range(1, iso_h.GetXaxis().GetNbins()):
        if iso_h.GetBinContent(ibin) == 0: continue
        isoGraph.SetPoint(thepoint, ibin-1,  iso_h.GetBinContent(ibin))
        thepoint+=1
        print thepoint, ibin, iso_h.GetBinContent(ibin)
    thepoint =0
    for ibin in range(1, noniso_h.GetXaxis().GetNbins()):
        if noniso_h.GetBinContent(ibin) == 0: continue
        nonIsoGraph.SetPoint(thepoint, ibin-1,  noniso_h.GetBinContent(ibin))
        thepoint+=1
    thepoint =0
    for ibin in range(1, all_h.GetXaxis().GetNbins()):
        if all_h.GetBinContent(ibin) == 0: continue
        allGraph.SetPoint(thepoint, ibin-1,  all_h.GetBinContent(ibin))
        thepoint+=1

    isoGraph.SetMarkerStyle(20)
    isoGraph.SetMarkerColor(n+1)
    allGraph.SetMarkerStyle(20)
    allGraph.SetMarkerColor(n+1)
    nonIsoGraph.SetMarkerStyle(20)
    nonIsoGraph.SetMarkerColor(n+1)
    newLeg.AddEntry(isoGraph, "FG > "+str(FGlist[n]), "p")

    diffallgraphlist.append(allGraph)
    diffisographlist.append(isoGraph)
    diffnonisographlist.append(nonIsoGraph)
    outfile.cd()
    allGraph.Write()
    isoGraph.Write()
    nonIsoGraph.Write()
    
    mg_diff_all.Add(allGraph)
    mg_diff_iso.Add(isoGraph)
    mg_diff_noniso.Add(nonIsoGraph)
    

canvas.Clear()
mg_diff_iso.Draw("AP")   
mg_diff_iso.GetXaxis().SetTitle("trig cand E_{T} [GeV]")  
newLeg.Draw()
canvas.SaveAs("IsoCandidateRateChange.pdf")
canvas.SaveAs("IsoCandidateRateChange.png")
canvas.SetName("IsoCandidateRateChange")
canvas.Write()
    
canvas.Clear()
mg_diff_noniso.Draw("AP") 
mg_diff_noniso.GetXaxis().SetTitle("trig cand E_{T} [GeV]")  
newLeg.Draw()
canvas.SaveAs("NonIsoCandidateRateChange.pdf")
canvas.SaveAs("NonIsoCandidateRateChange.png")
canvas.SetName("NonsoCandidateRateChange")
canvas.Write()

canvas.Clear()
mg_diff_all.Draw("AP")   
mg_diff_all.GetXaxis().SetTitle("trig cand E_{T} [GeV]")  
newLeg.Draw()
canvas.SaveAs("AllCandidateRateChange.pdf")
canvas.SaveAs("AllCandidateRateChange.png")
canvas.SetName("AllCandidateRateChange")
canvas.Write()

mg_diff_iso.SetName("mg_IsoCandidateRateChange")
mg_diff_all.SetName("mg_AllCandidateRateChange")
mg_diff_noniso.SetName("mg_NonIsoCandidateRateChange")

mg_diff_all.Write()
mg_diff_iso.Write()
mg_diff_noniso.Write()

outfile.Close()

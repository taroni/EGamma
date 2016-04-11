import ROOT

filelist =[
'myRateStudy_090.root',
'myRateStudy_080.root',
'myRateStudy_085.root',
#'myRateStudy_091.root',
#'myRateStudy_092.root',
'myRateStudy_093.root',
'myRateStudy_094.root',
'myRateStudy_095.root'
]

FGlist = [
"0.90",
"0.80",
"0.85",
#"0.91",
#"0.92",
"0.93",
"0.94",
"0.95"
]


infile = ROOT.TFile.Open("rateHistos/summaryHistos.root","READ") 
outfile = ROOT.TFile.Open("rateHistos/summaryGraphs.root","RECREATE") 
histolist = ["L1_IsoCands", "L1_NonIsoCands", "L1_AllCands"]
graphList=[]
newhistos=[]
diffList=[]


leg = ROOT.TLegend(0.6, 0.8, 0.8, 0.4)
leg.SetFillColor(0)
canvas  = ROOT.TCanvas()
canvas.Draw()
canvas.SetGridx(1)
canvas.SetGridy(1)

for histo in histolist:
    mg = ROOT.TMultiGraph()        
    mg_diff = ROOT.TMultiGraph()
    leg.Clear()
    for n,fg  in enumerate(FGlist):
        h_old = infile.Get(histo+"0.90_M")
        h_new = infile.Get(histo+fg+"_M")

        myGraph_new = ROOT.TGraph()
        myGraph_new.SetTitle(h_new.GetTitle())
        myGraph_new.SetName("gr_"+h_new.GetName())

        labels = []
        myDiffGraph = ROOT.TGraph()
        myDiffGraph.SetName("rateDiff_"+h_new.GetName())
        myDiffGraph.SetTitle(h_new.GetTitle())
    
        for bin in range(1, h_new.GetXaxis().GetNbins()) :
            if h_new.GetBinContent(bin) == 0: continue
            labels.append("EG"+ str(int(h_new.GetXaxis().GetBinCenter(bin)-0.2)))
            myGraph_new.SetPoint(len(labels)-1,int(h_new.GetXaxis().GetBinCenter(bin)-0.2),float(h_new.GetBinContent(bin)))
            if n == 0 :
                myDiffGraph.SetPoint(len(labels)-1,int(h_new.GetXaxis().GetBinCenter(bin)-0.2), float(1))
            else:
                
                myDiffGraph.SetPoint(len(labels)-1,int(h_new.GetXaxis().GetBinCenter(bin)-0.2), float(h_new.GetBinContent(bin))/float(h_old.GetBinContent(bin)))
        
        
        
        outfile.cd()
        myGraph_new.Write()
        myDiffGraph.Write()

        myGraph_new.SetMarkerStyle(20)
        myGraph_new.SetMarkerColor(n+1)
        myDiffGraph.SetMarkerStyle(20)
        myDiffGraph.SetMarkerColor(n+1)
        mg.Add(myGraph_new)
        mg_diff.Add(myDiffGraph)

        leg.AddEntry(myDiffGraph, "FG > "+str(FGlist[n]), "p")
        #print n, len(FGlist)
        if n == len(FGlist)-1:
            canvas.Clear()
            mg.Draw("AP")
            mg.GetXaxis().SetTitle("trig cand E_{T} [GeV]")  
            leg.Draw()
            print histo
            if histo=="L1_IsoCands":
                canvas.SetName("IsoCandidateRate")
                canvas.SaveAs("IsoCandidateRate.pdf") 
                canvas.SaveAs("IsoCandidateRate.png") 
                canvas.Write()
            if histo=="L1_NonIsoCands":
                canvas.SetName("NonIsoCandidateRate")
                canvas.SaveAs("NonIsoCandidateRate.pdf") 
                canvas.SaveAs("NonIsoCandidateRate.png") 
                canvas.Write()
            if histo=="L1_AllCands":
                canvas.SetName("AllCandidateRate")
                canvas.SaveAs("AllCandidateRate.pdf") 
                canvas.SaveAs("AllCandidateRate.png") 
                canvas.Write()
            canvas.Clear()
            mg_diff.Draw("AP")
            mg_diff.GetXaxis().SetTitle("trig cand E_{T} [GeV]")  
            leg.Draw()
            if histo=="L1_IsoCands":
                canvas.SetName("IsoCandidateRateChange")
                canvas.SaveAs("IsoCandidateRateChange.pdf") 
                canvas.SaveAs("IsoCandidateRateChange.png") 
                canvas.Write()
            if histo=="L1_NonIsoCands":
                canvas.SetName("NonIsoCandidateRateChange")
                canvas.SaveAs("NonIsoCandidateRateChange.pdf") 
                canvas.SaveAs("NonIsoCandidateRateChange.png") 
                canvas.Write()
            if histo=="L1_AllCands":
                canvas.SetName("AllCandidateRateChange")
                canvas.SaveAs("AllCandidateRateChange.pdf") 
                canvas.SaveAs("AllCandidateRateChange.png") 
                canvas.Write()


outfile.Close()
infile.Close()


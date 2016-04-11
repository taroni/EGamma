import ROOT 
from ROOT import TCanvas, TFile, RooHist, TLegend, TMath, TF1

filelist = ['eff_EG30_tagWP80_probeWP80_fit_effi_TagProbe_tree_change080.root', 
            'eff_EG30_tagWP80_probeWP80_fit_effi_TagProbe_tree_change085.root', 
            'eff_EG30_tagWP80_probeWP80_fit_effi_TagProbe_tree_change090.root',
            'eff_EG30_tagWP80_probeWP80_fit_effi_TagProbe_tree_change093.root', 
            'eff_EG30_tagWP80_probeWP80_fit_effi_TagProbe_tree_change094.root', 
            'eff_EG30_tagWP80_probeWP80_fit_effi_TagProbe_tree_change095.root'
]

names = ["0.80", "0.85", "0.90", "0.93", "0.94", "0.95"]

labels = ["FG > 0.80", 
          "FG > 0.85", 
          "FG > 0.90",
          "FG > 0.93", 
          "FG > 0.94", 
          "FG > 0.95"
          ]



c = TCanvas("c", "c" , 500, 500) 
c.Draw()
c.SetLogx(1)
c.SetGridx(1)
c.SetGridy(1)
outfile = TFile("efficiencyGraphs.root", "RECREATE")
histlist=[]
curvelist=[]
for n,myfile in enumerate(filelist):

    f= TFile.Open("turnons/EG30/"+myfile) 
    print f.GetName()
    ca=f.Get("ca")
    legend = TLegend(0.6, 0.6, 0.85, 0.2) 
    legend.SetFillColor(0)
    hist = ca.FindObject("h_data0_Eff[l1_30_EB_M]")
    hist.SetTitle("Tag and probe efficiency, Run2015D") 
    curve = ca.FindObject("cb_Norm[sc_et_EB_M]")
    outfile.cd()
    histlist.append(hist)
    curvelist.append(curve)

for n, hist in enumerate(histlist):
    outfile.cd()
    c.cd()
    
    if n!=0 :
        hist.Draw("PSAME")
        curvelist[n].Draw("SAME")
    else :
        hist.Draw("AP")
        hist.GetXaxis().SetRangeUser(10, 900)
        curvelist[n].Draw("SAME")
    hist.SetMarkerColor(n+1)
    hist.SetLineColor(n+1)
    curvelist[n].SetLineColor(n+1)
    curvelist[n].SetName("cb_"+names[n])
    hist.SetName("h_data0_Eff_"+names[n])
    
    legend.AddEntry(hist, labels[n], "lp")
        
    hist.Write()
    curve.Write()
    
    if n ==len(filelist)-1 :
        legend.Draw()
        c.SaveAs("EfficiencyPlot.pdf")
        c.Write()

outfile.Close()
    

import ROOT

ROOT.gStyle.SetOptTitle(0)

EG_List = [15, 20, 30, 40]

outfile= ROOT.TFile.Open("output_2015.root","RECREATE")
canvas = ROOT.TCanvas("c", "c", 600, 600)
canvas.Draw()
canvas.SetGridx(1)
canvas.SetGridy(1)
canvas.SetLogx(1)
canvas.SetLeftMargin(0.2)
canvas.SetTopMargin(0.05)
canvas.SetBottomMargin(0.15)
leg = ROOT.TLegend(0.59, 0.45, 0.89, 0.25)
leg.SetTextSize(0.035)
#leg.AddEntry(None, "Z #rightArrow e^{+}e^{-}", "")
#leg.AddEntry(None, "ECAL Barrel", "") 
histList=[]
for (n,iEG) in enumerate(EG_List):
    filein = ROOT.TFile.Open("ratioScan/selectPairsDir/turnons/EG%s/eff_EG%s_tagWP80_probeWP80_EB_N_vs_EB_N_fitres.root" %(str(iEG), str(iEG)), "READ")
    print filein.GetName()
    c= filein.Get("ca")
    print c.GetName()
    outfile.cd()
    plist = c.GetListOfPrimitives()
    for pl in  plist: print pl
    histo=c.FindObject("h_data2_Eff[l1_%s_EB_N]" %(str(iEG)))
    histo.SetName("h_data2_Eff_l1_%s_EB_N" %(str(iEG)))
    print 'point number', histo.GetN()
    histo.SetPoint(histo.GetN(),300.,1.)
    curve=c.FindObject("cb2_Norm[sc_et_EB_N]")
    curve.SetName("cb2_Norm_sc_et_EB_N_%s" %(str(iEG)))
    curve.Write()
    canvas.cd()
    if n==0 : 
        histo.Draw("AP")
        histo.GetYaxis().SetTitle("Efficiency")
        histo.GetXaxis().SetTitle("E_{T} [GeV]")
        histo.GetXaxis().SetTitleOffset(1.2)
        histo.GetXaxis().SetRangeUser(8.,170.)
        histo.GetYaxis().SetTitleOffset(1.2)
        curve.Draw("SAME")
        curve.SetLineColor(n+1)
    print histo.GetName()
    #histo.GetXaxis().SetRangeUser(5, 100) 
    if n > 0 :
        histo.Draw("P")
        curve.Draw("SAME")
        curve.SetLineColor(n+1)
    histo.SetLineColor(n+1)
    histo.SetMarkerStyle(20)
    histo.SetMarkerColor(n+1)
    for point in range(0, histo.GetN()):
        histo.SetPointEXlow(point, 0)
        histo.SetPointEXhigh(point, 0)
    canvas.Update()
    leg.AddEntry(histo, "L1 Thr: %s GeV" %(str(iEG)), "lp")

label1  = ROOT.TLatex(8.5, 1.05, "CMS")
#label1.SetTextFont(42) 
label1.SetTextSize(0.04)
label1.Draw()
label3  = ROOT.TLatex(8.5, 1.01, "Preliminary") 
label3.SetTextFont(52) 
label3.SetTextSize(0.03)
label3.Draw()

label2  = ROOT.TLatex(80, 1.11, "#sqrt{s}=13 TeV") 
label2.SetTextFont(42) 
label2.SetTextSize(0.04)
label2.Draw()


text = ROOT.TLatex(8.5, 0.95, "Z #rightarrow e^{+}e^{-}")
text.SetTextFont(42) 
text.SetTextSize(0.035)
text.Draw()
text2 = ROOT.TLatex(8.5, 0.9, "ECAL Barrel")
text2.SetTextFont(42) 
text2.SetTextSize(0.035)
text2.Draw()

leg.Draw()
outfile.cd()
canvas.Write()

canvas.Print("plot_iEG_2015.png")
canvas.Print("plot_iEG_2015.pdf")

canvas.Clear()

for (n,iEG) in enumerate(EG_List):
    filein = ROOT.TFile.Open("ratioScan/selectPairsDir/turnons/EG%s/eff_EG%s_tagWP80_probeWP80_EE_N_vs_EE_N_fitres.root" %(str(iEG), str(iEG)), "READ")
    print filein.GetName()
    c= filein.Get("ca")
    print c.GetName()
    outfile.cd()
    plist = c.GetListOfPrimitives()
    for pl in  plist: print pl
    histo=c.FindObject("h_data2_Eff[l1_%s_EE_N]" %(str(iEG)))
    histo.SetName("h_data2_Eff_l1_%s_EE_N" %(str(iEG)))
    histo.SetPoint(histo.GetN(),300.,1.)
    curve=c.FindObject("cb2_Norm[sc_et_EE_N]")
    curve.SetName("cb2_Norm_sc_et_EE_N_%s" %(str(iEG)))
    curve.Write()
    canvas.cd()
    if n==0 : 
        histo.Draw("AP")
        histo.GetYaxis().SetTitle("Efficiency")
        histo.GetXaxis().SetTitle("E_{T} [GeV]")
        histo.GetXaxis().SetTitleOffset(1.2)
        histo.GetXaxis().SetRangeUser(8.,170.)
        histo.GetYaxis().SetTitleOffset(1.2)
        for point in range(0, histo.GetN()):
            histo.SetPointEXlow(point, 0)
            histo.SetPointEXhigh(point, 0)

        curve.Draw("SAME")
        curve.SetLineColor(n+1)
    print histo.GetName()
    #histo.GetXaxis().SetRangeUser(5, 100) 
    if n > 0 :
        histo.Draw("SAMEP")
        curve.Draw("SAME")
        curve.SetLineColor(n+1)
    histo.SetLineColor(n+1)
    histo.SetMarkerStyle(20)
    histo.SetMarkerColor(n+1)
    for point in range(0, histo.GetN()):
        histo.SetPointEXlow(point, 0)
        histo.SetPointEXhigh(point, 0)
    canvas.Update()
    
label1  = ROOT.TLatex(8.3, 1.05, "CMS")
#label1.SetTextFont(42) 
label1.SetTextSize(0.04)
label1.Draw()
label3  = ROOT.TLatex(8.3, 1.01, "Preliminary") 
label3.SetTextFont(52) 
label3.SetTextSize(0.03)
label3.Draw()


label2  = ROOT.TLatex(80, 1.11, "#sqrt{s}=13 TeV") 
label2.SetTextFont(42) 
label2.SetTextSize(0.04)
label2.Draw()

text = ROOT.TLatex(8.3, 0.95, "Z #rightarrow e^{+}e^{-}")
text.SetTextFont(42) 
text.SetTextSize(0.035)
text.Draw()
text2 = ROOT.TLatex(8.3, 0.9, "ECAL Endcaps")
text2.SetTextFont(42) 
text2.SetTextSize(0.035)
text2.Draw()


leg.Draw()
outfile.cd()
canvas.Write()

canvas.Print("plot_EE_iEG_2015.png")
canvas.Print("plot_EE_iEG_2015.pdf")

canvas2=ROOT.TCanvas("c2012", "c2012", 600, 600)

canvas2.Draw()
canvas2.SetGridx(1)
canvas2.SetGridy(1)
canvas2.SetLogx(1)
canvas2.SetLeftMargin(0.2)
canvas2.SetTopMargin(0.05)
canvas2.SetBottomMargin(0.15)
histList=[]
for (n,iEG) in enumerate(EG_List):
    filein = ROOT.TFile.Open("ratioScan/selectPairsDir/turnons/EG%s/eff_EG%s_tagWP80_probeWP80_EB_N_vs_EB_N_fitres.root" %(str(iEG), str(iEG)), "READ")
    print filein.GetName()
    c= filein.Get("ca")
    print c.GetName()
    outfile.cd()
    plist = c.GetListOfPrimitives()
    for pl in  plist: print pl
    histo=c.FindObject("h_data_Eff[l1_%s_EB_N]" %(str(iEG)))
    histo.SetName("h_data_Eff_l1_%s_EB_N" %(str(iEG)))
    curve=c.FindObject("cb_Norm[sc_et_EB_N]")
    curve.SetName("cb_Norm_sc_et_EB_N_%s" %(str(iEG)))
    curve.Write()
    canvas2.cd()
    if n==0 : 
        histo.Draw("AP")
        histo.GetYaxis().SetTitle("Efficiency")
        histo.GetXaxis().SetTitle("E_{T} [GeV]")
        histo.GetXaxis().SetTitleOffset(1.2)
        histo.GetYaxis().SetTitleOffset(1.2)
        curve.Draw("SAME")
        curve.SetLineColor(n+1)
    print histo.GetName()
    #histo.GetXaxis().SetRangeUser(5, 100) 
    if n > 0 :
        histo.Draw("SAMEP")
        curve.Draw("SAME")
        curve.SetLineColor(n+1)
    histo.SetLineColor(n+1)
    histo.SetMarkerStyle(20)
    histo.SetMarkerColor(n+1)
    for point in range(0, histo.GetN()):
        histo.SetPointEXlow(point, 0)
        histo.SetPointEXhigh(point, 0)
    canvas.Update()
label1  = ROOT.TLatex(5, 1.11, "CMS Preliminary") 
label1.SetTextFont(42) 
label1.SetTextSize(0.04)
label1.Draw()

label2  = ROOT.TLatex(70, 1.11, "#sqrt{s}=8 TeV") 
label2.SetTextFont(42) 
label2.SetTextSize(0.04)
label2.Draw()


text = ROOT.TLatex(6, 1.03, "Z #rightarrow e^{+}e^{-}")
text.SetTextFont(42) 
text.SetTextSize(0.04)
text.Draw()
text2 = ROOT.TLatex(6, 0.96, "ECAL Barrel")
text2.SetTextFont(42) 
text2.SetTextSize(0.04)
text2.Draw()


leg.Draw()
outfile.cd()
canvas2.Write()
canvas2.Print("plot_iEG_2012.png")
canvas2.Print("plot_iEG_2012.pdf")

canvas2.Clear()
histList=[]
for (n,iEG) in enumerate(EG_List):
    filein = ROOT.TFile.Open("ratioScan/selectPairsDir/turnons/EG%s/eff_EG%s_tagWP80_probeWP80_EE_N_vs_EE_N_fitres.root" %(str(iEG), str(iEG)), "READ")
    print filein.GetName()
    c= filein.Get("ca")
    print c.GetName()
    outfile.cd()
    plist = c.GetListOfPrimitives()
    for pl in  plist: print pl
    histo=c.FindObject("h_data_Eff[l1_%s_EE_N]" %(str(iEG)))
    histo.SetName("h_data_Eff_l1_%s_EE_N" %(str(iEG)))
    curve=c.FindObject("cb_Norm[sc_et_EE_N]")
    curve.SetName("cb_Norm_sc_et_EE_N_%s" %(str(iEG)))
    curve.Write()
    canvas2.cd()
    if n==0 : 
        histo.Draw("AP")
        histo.GetYaxis().SetTitle("Efficiency")
        histo.GetXaxis().SetTitle("E_{T} [GeV]")
        histo.GetXaxis().SetTitleOffset(1.2)
        curve.Draw("SAME")
        curve.SetLineColor(n+1)
    print histo.GetName()
    #histo.GetXaxis().SetRangeUser(5, 100) 
    if n > 0 :
        histo.Draw("SAMEP")
        curve.Draw("SAME")
        curve.SetLineColor(n+1)
    histo.SetLineColor(n+1)
    histo.SetMarkerStyle(20)
    histo.SetMarkerColor(n+1)
    for point in range(0, histo.GetN()):
        histo.SetPointEXlow(point, 0)
        histo.SetPointEXhigh(point, 0)
        
    canvas2.Update()
label1  = ROOT.TLatex(5, 1.11, "CMS Preliminary") 
label1.SetTextFont(42) 
label1.SetTextSize(0.04)
label1.Draw()

label2  = ROOT.TLatex(70, 1.11, "#sqrt{s}=8 TeV") 
label2.SetTextFont(42) 
label2.SetTextSize(0.04)
label2.Draw()


text = ROOT.TLatex(6, 1.03, "Z #rightarrow e^{+}e^{-}")
text.SetTextFont(42) 
text.SetTextSize(0.04)
text.Draw()
text2 = ROOT.TLatex(6, 0.96, "ECAL Endcap")
text2.SetTextFont(42) 
text2.SetTextSize(0.04)
text2.Draw()


leg.Draw()
outfile.cd()
canvas2.Write()
canvas2.Print("plot_EE_iEG_2012.png")
canvas2.Print("plot_EE_iEG_2012.pdf")



outfile.Close()

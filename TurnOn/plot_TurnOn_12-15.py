import ROOT
import ROOT.TLatex

ROOT.gStyle.SetOptTitle(0)

EG_List = [20]

outfile= ROOT.TFile.Open("output_2015.root","RECREATE")
canvas = ROOT.TCanvas("c", "c", 600, 600)
canvas.Draw()
canvas.SetGridx(1)
canvas.SetGridy(1)
canvas.SetLogx(1)
canvas.SetLeftMargin(0.2)
canvas.SetTopMargin(0.05)
canvas.SetBottomMargin(0.15)
leg = ROOT.TPaveText(38, 0.58, 100, 0.38, "NB")
leg.SetFillColor(0)
#leg.SetBorderSize(1)

leg.SetTextSize(0.03)
leg.SetTextFont(42)
leg.AddText( "Z #rightarrow e^{+}e^{-}")
leg.AddText( "ECAL Barrel")
leg.AddText( "L1 Trigger EG20")
text = ROOT.TLatex(32, 0.52, "Z #rightarrow e^{+}e^{-}")
text.SetTextFont(42) 
text.SetTextSize(0.035)

text2 = ROOT.TLatex(32, 0.47, "ECAL Barrel")
text2.SetTextFont(42) 
text2.SetTextSize(0.035)

text3 = ROOT.TLatex(32, 0.42, "L1 Threshold: 20 GeV")
text3.SetTextFont(42) 
text3.SetTextSize(0.035)





leg2 = ROOT.TLegend(0.5, 0.38, 0.85, 0.23)
#leg2.SetTextSize(0.028)
histList=[20]
for (n,iEG) in enumerate(EG_List):
    filein = ROOT.TFile.Open("ratioScan/selectPairsDir/turnons/EG%s/eff_EG%s_tagWP80_probeWP80_EB_N_vs_EB_N_fitres.root" %(str(iEG), str(iEG)), "READ")
    print filein.GetName()
    c= filein.Get("ca")
    print c.GetName()
    outfile.cd()
    plist = c.GetListOfPrimitives()
    for pl in  plist: print pl
    histo2=c.FindObject("h_data2_Eff[l1_%s_EB_N]" %(str(iEG)))
    histo2.SetName("h_data2_Eff_l1_%s_EB_N" %(str(iEG)))
    curve2=c.FindObject("cb2_Norm[sc_et_EB_N]")
    curve2.SetName("cb2_Norm_sc_et_EB_N_%s" %(str(iEG)))
    curve2.Write()
    histo=c.FindObject("h_data_Eff[l1_%s_EB_N]" %(str(iEG)))
    histo.SetName("h_data_Eff_l1_%s_EB_N" %(str(iEG)))
    curve=c.FindObject("cb_Norm[sc_et_EB_N]")
    curve.SetName("cb_Norm_sc_et_EB_N_%s" %(str(iEG)))
    curve.Write()
    canvas.cd()
    if n==0 : 
        histo.Draw("AP")
        histo.GetYaxis().SetTitle("Efficiency")
        histo.GetXaxis().SetTitle("E_{T} [GeV]")
        histo.GetXaxis().SetTitleOffset(1.2)
        histo.GetYaxis().SetTitleOffset(1.2)
        histo.GetXaxis().SetRangeUser(5.,150.)
        for point in range(0, histo.GetN()):
            histo.SetPointEXlow(point, 0)
            histo.SetPointEXhigh(point, 0)
        for point in range(0, histo2.GetN()):
            histo2.SetPointEXlow(point, 0)
            histo2.SetPointEXhigh(point, 0)
        
                  
        canvas.Update()
        curve.Draw("SAME")
        curve.SetLineColor(n+1)
        histo2.Draw("SAMEP")
        curve2.Draw("SAME")
        curve2.SetLineColor(n+2)
        
    print histo.GetName()
    #histo.GetXaxis().SetRangeUser(5, 100) 
    if n > 0 :
        histo.Draw("SAMEP")
        curve.Draw("SAME")
        curve.SetLineColor(n+1)
    histo.SetLineColor(n+1)
    histo.SetMarkerStyle(20)
    histo.SetMarkerColor(n+1)
    
    leg2.AddEntry(histo, "2012, #sqrt{s} = 8  TeV", "lp")
    leg2.AddEntry(histo2, "2015, #sqrt{s} = 13 TeV", "lp")

label1  = ROOT.TLatex(10.1, 1.02, "CMS")
#label1.SetTextFont(42) 
label1.SetTextSize(0.04)
label1.Draw()
label3  = ROOT.TLatex(10.1, 0.96, "Preliminary") 
label3.SetTextFont(52) 
label3.SetTextSize(0.03)
label3.Draw()
text.Draw()
text2.Draw()
text3.Draw()


#text = ROOT.TLatex(6, 0.85, "Z #rightarrow e^{+}e^{-}")
#text.SetTextFont(42) 
#text.SetTextSize(0.04)
#text.Draw()
#text2 = ROOT.TLatex(6, 0.805, "ECAL Barrel")
#text2.SetTextFont(42) 
#text2.SetTextSize(0.04)
#text2.Draw()


#leg.Draw()
leg2.Draw()
outfile.cd()
canvas.Write()

canvas.Print("plot_EB_12-15.png")
canvas.Print("plot_EB_12-15.pdf")

canvas.Clear()
leg.Clear()
#leg2.Clear()
leg.AddText("Z #rightarrow e^{+}e^{-}")
leg.AddText("ECAL Endcap")
leg.AddText("L1 Trigger EG20")
for (n,iEG) in enumerate(EG_List):
    filein = ROOT.TFile.Open("ratioScan/selectPairsDir/turnons/EG%s/eff_EG%s_tagWP80_probeWP80_EE_N_vs_EE_N_fitres.root" %(str(iEG), str(iEG)), "READ")
    print filein.GetName()
    c= filein.Get("ca")
    print c.GetName()
    outfile.cd()
    plist = c.GetListOfPrimitives()
    for pl in  plist: print pl
    histo2=c.FindObject("h_data2_Eff[l1_%s_EE_N]" %(str(iEG)))
    histo2.SetName("h_data2_Eff_l1_%s_EE_N" %(str(iEG)))
    curve2=c.FindObject("cb2_Norm[sc_et_EE_N]")
    curve2.SetName("cb2_Norm_sc_et_EE_N_%s" %(str(iEG)))
    curve2.Write()
    histo=c.FindObject("h_data_Eff[l1_%s_EE_N]" %(str(iEG)))
    histo.SetName("h_data_Eff_l1_%s_EE_N" %(str(iEG)))
    curve=c.FindObject("cb_Norm[sc_et_EE_N]")
    curve.SetName("cb_Norm_sc_et_EE_N_%s" %(str(iEG)))
    curve.Write()
    canvas.cd()
    if n==0 : 
        histo.Draw("AP")
        histo.GetYaxis().SetTitle("Efficiency")
        histo.GetXaxis().SetTitle("E_{T} [GeV]")
        histo.GetXaxis().SetTitleOffset(1.2)
        histo.GetYaxis().SetTitleOffset(1.2)
        for point in range(0, histo.GetN()):
            histo.SetPointEXlow(point, 0)
            histo.SetPointEXhigh(point, 0)
        for point in range(0, histo2.GetN()):
            histo2.SetPointEXlow(point, 0)
            histo2.SetPointEXhigh(point, 0)
        histo.GetXaxis().SetRangeUser(5.,150.)

        canvas.Update()
        curve.Draw("SAME")
        curve.SetLineColor(n+1)
        histo2.Draw("SAMEP")
        curve2.Draw("SAME")
        curve2.SetLineColor(n+2)
    print histo.GetName()
    #histo.GetXaxis().SetRangeUser(5, 100) 
    if n > 0 :
        histo.Draw("SAMEP")
        curve.Draw("SAME")
        curve.SetLineColor(n+1)
    histo.SetLineColor(n+1)
    histo.SetMarkerStyle(20)
    histo.SetMarkerColor(n+1)
    #leg2.AddEntry(histo, "2012, #sqrt{s} =8  TeV", "lp")
    #leg2.AddEntry(histo2, "2015, #sqrt{s} =13 TeV", "lp")

 
label1  = ROOT.TLatex(10.1, 1.02, "CMS")
#label1.SetTextFont(42) 
label1.SetTextSize(0.04)
label1.Draw()
label3  = ROOT.TLatex(10.1, 0.96, "Preliminary") 
label3.SetTextFont(52) 
label3.SetTextSize(0.03)
label3.Draw()
label2  = ROOT.TLatex(50., 0.5, "L1 Trigger EG20") 
label2.SetTextFont(42) 
label2.SetTextSize(0.04)
#label2.Draw()
text.Draw()
text2.DrawText(32,0.47,"Ecal Endcap")
text3.Draw()



#leg.Draw()
leg2.Draw()
outfile.cd()
canvas.Write()

canvas.Print("plot_EE_12-15.png")
canvas.Print("plot_EE_12-15.pdf")



outfile.Close()

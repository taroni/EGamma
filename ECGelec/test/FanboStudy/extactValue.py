import ROOT

infile = ROOT.TFile("outfile.root", "READ")

totEB = infile.Get("TotalEB")

c=ROOT.TCanvas("c", "c", 500, 500)
c.Draw()
totEB.Draw()
tpt

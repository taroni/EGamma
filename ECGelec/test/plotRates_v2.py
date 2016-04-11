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
"0.90",
"0.80",
"0.85",
"0.91",
"0.92",
"0.93",
"0.94",
"0.95"
]


outfile = ROOT.TFile.Open("rateHistos/summaryHistos.root","RECREATE") 
histolist = ["L1_IsoCands", "L1_NonIsoCands", "L1_AllCands"]
graphList=[]
newhistos=[]
diffList=[]

for n,filename in enumerate(filelist):
    histos=[]
    print filename
    for histo in histolist:
        myfile=ROOT.TFile.Open("rateHistos/"+filename, "READ")
        #print myfile
        
        h_new = myfile.Get("rate/"+histo+"_M")
        #print histo
        h_new.SetName(histo+FGlist[n]+"_M")
        h_Norm = myfile.Get("rate/h_Entries") 
        
        Norm = h_Norm.Integral()
        #print 'Normalization', Norm
        h_new.Scale(1./Norm)
        
        outfile.cd()
        h_new.Write()
        histos.append(h_new)

outfile.Close()

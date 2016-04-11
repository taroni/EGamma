#!/usr/bin/env python 
from os import popen

filelist = [
#"tree_changed_095.root", 
#"tree_noChange.root", 
"tree_changed_080.root",
#"tree_changed_085.root",
#"tree_changed_091.root",
#"tree_changed_092.root",
#"tree_changed_093.root",
#"tree_changed_094.root"
]



for myfile in filelist:
    sh = """#!/bin/bash

    cd /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_5_3_28/src/L1Studies/EGamma/Macros/myAnalysis/

    eval `scram runtime -sh`
    
    cd -
    
    cp /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_5_3_28/src/L1Studies/EGamma/Macros/myAnalysis/*cc . 
    cp /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_5_3_28/src/L1Studies/EGamma/Macros/myAnalysis/*.h . 
    cp /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_5_3_28/src/L1Studies/EGamma/Macros/myAnalysis/Makefile .
   
    g++  -o makeTriggerCand makeTriggerCand.cc `root-config --cflags --libs` -L /afs/cern.ch/cms/slc6_amd64_gcc472/lcg/roofit/5.32.03-cms//lib -lRooFit -lRooFitCore -I/afs/cern.ch/cms/slc6_amd64_gcc472/lcg/roofit/5.32.03-cms//include  

    ./makeTriggerCand ratioScan/ZeroBias/RateStudy """+myfile+"""

    cmsStage -f rate_"""+myfile+"""  /store/caf/user/taroni/TestTriggerOptimization/ratioScan/ZeroBias/RateStudy/
    cp rate_"""+myfile+"""  /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_5_3_28/src/L1Studies/EGamma/Macros/myAnalysis/rateHistos/
    
    
    """

    sh_file=open("launchRate"+myfile.replace(".root",".sh"), "w")
    sh_file.write(sh)
    sh_file.close
    popen("chmod a+x launchRate"+myfile.replace(".root",".sh"))

    popen("bsub -q cmscaf1nd launchRate"+myfile.replace(".root",".sh"))

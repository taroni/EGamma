#!/usr/bin/env python 
from os import popen

filelist = [
"effi_TagProbe_tree_changed.root", 
"effi_TagProbe_tree_noChange.root", 
"effi_TagProbe_tree_changed_080.root",
"effi_TagProbe_tree_changed_085.root",
"effi_TagProbe_tree_changed_091.root",
"effi_TagProbe_tree_changed_092.root",
"effi_TagProbe_tree_changed_093.root",
"effi_TagProbe_tree_changed_094.root"
]

eosdir="/store/caf/user/taroni/TestTriggerOptimization/ratioScan/Tutanon/selectPairsDir"
popen("xrd eoscms  mkdir "+eosdir+"/turnons/EG20/")


for myfile in filelist:
    sh = """#!/bin/bash

    cd /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_5_3_28/src/L1Studies/EGamma/Macros/myAnalysis/

    eval `scram runtime -sh`
    
    cd -
    
    cp /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_5_3_28/src/L1Studies/EGamma/Macros/myAnalysis/*cc . 
    cp /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_5_3_28/src/L1Studies/EGamma/Macros/myAnalysis/*.h . 
    cp /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_5_3_28/src/L1Studies/EGamma/Macros/myAnalysis/Makefile .
    
    make runNewFit JOBID=ratioScan/Tutanon FILE="""+myfile+"""
    
    cmsStage -f turnons/EG20/eff_EG20_tagWP80_probeWP80_"""+myfile+"""   /store/caf/user/taroni/TestTriggerOptimization/ratioScan/Tutanon/selectPairsDir/turnons/EG20/
    cp turnons/EG20/eff_EG20_tagWP80_probeWP80_"""+myfile+"""  /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_5_3_28/src/L1Studies/EGamma/Macros/myAnalysis/turnons/EG20/
    
    
    """

    sh_file=open("launchEff"+myfile.replace(".root",".sh"), "w")
    sh_file.write(sh)
    sh_file.close
    popen("chmod a+x launchEff"+myfile.replace(".root",".sh"))

    popen("bsub -q cmscaf1nd launchEff"+myfile.replace(".root",".sh"))

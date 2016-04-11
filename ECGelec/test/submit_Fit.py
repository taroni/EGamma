#!/usr/bin/env python 
from os import popen

filelist = [
"effi_TagProbe_tree_change080.root",
"effi_TagProbe_tree_change085.root",
"effi_TagProbe_tree_change090.root",
"effi_TagProbe_tree_change095.root",
"effi_TagProbe_tree_change093.root",
"effi_TagProbe_tree_change094.root"
]

eosdir="/store/caf/user/taroni/TestTriggerOptimization2015/ratioScan/TurnOn/selectPairsDir"
popen("xrd eoscms  mkdir "+eosdir+"/turnons/EG30/")


for myfile in filelist:
    sh = """#!/bin/bash

    cd /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_7_4_12/src/EGamma/ECGelec/test

    eval `scram runtime -sh`
    
    cd -
    
    cp /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_7_4_12/src/EGamma/ECGelec/test/*cc . 
    cp /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_7_4_12/src/EGamma/ECGelec/test/*.h . 
    cp /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_7_4_12/src/EGamma/ECGelec/test/Makefile .
    
    make runFit JOBID=ratioScan/TurnOn2015 FILE="""+myfile+"""
    
    cmsStage -f turnons/EG30/eff_EG30_tagWP80_probeWP80_fit_"""+myfile+"""   /store/caf/user/taroni/TestTriggerOptimization2015/ratioScan/TurnOn2015/selectPairsDir/turnons/EG30/
    cp turnons/EG30/eff_EG30_tagWP80_probeWP80_fit_"""+myfile+"""  /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_7_4_12/src/EGamma/ECGelec/test/turnons/EG30/
    
    
    """

    sh_file=open("launchFit"+myfile.replace(".root",".sh"), "w")
    sh_file.write(sh)
    sh_file.close
    popen("chmod a+x launchFit"+myfile.replace(".root",".sh"))

    popen("bsub -q cmscaf1nd launchFit"+myfile.replace(".root",".sh"))

#!/usr/bin/env python 
from os import popen

filelist = [
"tree_change080.root",
"tree_change085.root",
"tree_change090.root",
"tree_change095.root",
"tree_change093.root",
"tree_change094.root"
]

eosdir="/store/caf/user/taroni/TestTriggerOptimization2015/"
popen("xrd eoscms  mkdir "+eosdir+"ratioScan")
popen("xrd eoscms  mkdir "+eosdir+"ratioScan/TurnOn2015")
popen("xrd eoscms  mkdir "+eosdir+"ratioScan/TurnOn2015/selectPairsDir")


for myfile in filelist:
    sh = """#!/bin/bash

    cd /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_7_4_12/src/EGamma/ECGelec/test/

    eval `scram runtime -sh`
    
    cd -
    
    cp /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_7_4_12/src/EGamma/ECGelec/test/*cc . 
    cp /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_7_4_12/src/EGamma/ECGelec/test/*.h . 
    cp /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_7_4_12/src/EGamma/ECGelec/test/Makefile .
    
    make selectPairs
    
    ls -lhrt 
    
    make runSelect JOBID=ratioScan/TurnOn2015/ FILE="""+myfile+"""

    cmsStage -f ratioScan/TurnOn2015/selectPairsDir/effi_TagProbe_"""+myfile+"""  """+eosdir+"""ratioScan/TurnOn2015/selectPairsDir/
    
    
    """

    sh_file=open("launchSelect"+myfile.replace(".root",".sh"), "w")
    sh_file.write(sh)
    sh_file.close
    popen("chmod a+x launchSelect"+myfile.replace(".root",".sh"))

    popen("bsub -q cmscaf1nd launchSelect"+myfile.replace(".root",".sh"))

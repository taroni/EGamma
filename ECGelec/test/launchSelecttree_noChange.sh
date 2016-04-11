#!/bin/bash

    cd /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_5_3_28/src/L1Studies/EGamma/Macros/myAnalysis/

    eval `scram runtime -sh`
    
    cd -
    
    cp /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_5_3_28/src/L1Studies/EGamma/Macros/myAnalysis/*cc . 
    cp /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_5_3_28/src/L1Studies/EGamma/Macros/myAnalysis/*.h . 
    cp /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_5_3_28/src/L1Studies/EGamma/Macros/myAnalysis/Makefile .
    
    make selectPairs
    
    ls -lhrt 
    
    make runSelect JOBID=ratioScan/TurnOn2015/ FILE=tree_noChange.root

    cmsStage -f ratioScan/TurnOn2015/selectPairsDir/effi_TagProbe_tree_noChange.root  /store/caf/user/taroni/TestTriggerOptimization2015/ratioScan/TurnOn2015/selectPairsDir/
    
    
    
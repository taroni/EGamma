#!/bin/bash

    cd /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_7_4_12/src/EGamma/ECGelec/test/

    eval `scram runtime -sh`
    
    cd -
    
    cp /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_7_4_12/src/EGamma/ECGelec/test/*cc . 
    cp /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_7_4_12/src/EGamma/ECGelec/test/*.h . 
    cp /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_7_4_12/src/EGamma/ECGelec/test/Makefile .
    
    make selectPairs
    
    ls -lhrt 
    
    make runSelect JOBID=ratioScan/TurnOn2015/ FILE=tree_change090.root

    cmsStage -f ratioScan/TurnOn2015/selectPairsDir/effi_TagProbe_tree_change090.root  /store/caf/user/taroni/TestTriggerOptimization2015/ratioScan/TurnOn2015/selectPairsDir/
    
    
    
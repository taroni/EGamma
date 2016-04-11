#!/bin/bash

    cd /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_7_4_12/src/EGamma/ECGelec/test

    eval `scram runtime -sh`
    
    cd -
    
    cp /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_7_4_12/src/EGamma/ECGelec/test/*cc . 
    cp /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_7_4_12/src/EGamma/ECGelec/test/*.h . 
    cp /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_7_4_12/src/EGamma/ECGelec/test/Makefile .
    
    make makePairs
    
    ls -lhrt 
    
    make runPairs JOBID=ratioScan/ZeroBias/TurnOn2015D FILE=tree_change085.root

    cmsStage ratioScan/ZeroBias/TurnOn2015D/makePairsDir/elepairs_tree_change085.root  /store/caf/user/taroni/TestTriggerOptimization2015/ratioScan/Tutanon/makePairsDir/
    
    
    
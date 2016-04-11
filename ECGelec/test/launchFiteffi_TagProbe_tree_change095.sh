#!/bin/bash

    cd /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_7_4_12/src/EGamma/ECGelec/test

    eval `scram runtime -sh`
    
    cd -
    
    cp /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_7_4_12/src/EGamma/ECGelec/test/*cc . 
    cp /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_7_4_12/src/EGamma/ECGelec/test/*.h . 
    cp /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_7_4_12/src/EGamma/ECGelec/test/Makefile .
    
    make runFit JOBID=ratioScan/TurnOn2015 FILE=effi_TagProbe_tree_change095.root
    
    cmsStage -f turnons/EG30/eff_EG30_tagWP80_probeWP80_fit_effi_TagProbe_tree_change095.root   /store/caf/user/taroni/TestTriggerOptimization2015/ratioScan/TurnOn2015/selectPairsDir/turnons/EG30/
    cp turnons/EG30/eff_EG30_tagWP80_probeWP80_fit_effi_TagProbe_tree_change095.root  /afs/cern.ch/work/t/taroni/private/ECAL/EcalEleStudy/CMSSW_7_4_12/src/EGamma/ECGelec/test/turnons/EG30/
    
    
    
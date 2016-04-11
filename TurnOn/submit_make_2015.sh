#!/bin/bash

W_DIR='/afs/cern.ch/user/t/taroni/scratch0/CMSSW_7_4_12/src/TurnOn'

cd $W_DIR
eval `scram runtime -sh`
cd - 
cp  $W_DIR/*.cc .
cp  $W_DIR/*.h  .
cp  $W_DIR/Makefile .

make runPairs

cmsStage -f ratioScan/makePairsDir/elepairs_tree_2015D.root /store/user/taroni/SingleEle_run2015D/DoubleEG/crab_ECAL_turnon_2015D/elepairs_tree_2015D.root
cmsStage -f ratioScan/makePairsDir/elepairs_tree_2015D.root /store/caf/user/taroni/elepairs_tree_2015D.root

make runSelect

cmsStage -f ratioScan/selectPairsDir/effi_TagProbe_tree_2015D.root /store/user/taroni/SingleEle_run2015D/DoubleEG/crab_ECAL_turnon_2015D/effi_TagProbe_tree_2015D.root
cmsStage -f ratioScan/selectPairsDir/effi_TagProbe_tree_2015D.root /store/caf/user/taroni/effi_TagProbe_tree_2015D.root







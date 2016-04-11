#!/bin/bash

ps | grep `echo $$` | awk '{ print $4 }' 

export mypwd=`pwd`




cd /afs/cern.ch/work/f/fmeng/FANBOWORKINGAREA/CMSSW_7_4_12/src/EGamma/ECGelec/test/
batchdir=/afs/cern.ch/work/f/fmeng/FANBOWORKINGAREA/CMSSW_7_4_12/src/EGamma/ECGelec/test

#export SCRAM_ARCH slc5_amd64_gcc434
#source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.sh
voms-proxy-init --voms cms
cp `find /tmp/x509up_u* -user fmeng` $batchdir/. 
eval `scram runtime -sh`
if [ $X509_USER_PROXY!="" ]; then
    export X509_USER_PROXY=`find /afs/cern.ch/work/f/fmeng/FANBOWORKINGAREA/CMSSW_7_4_12/src/EGamma/ECGelec/test/x509up_u*`
fi
echo 
cd -
eval `scramv1 runtime -sh`
echo "Starting CMSSW" 
cp /afs/cern.ch/work/f/fmeng/FANBOWORKINGAREA/CMSSW_7_4_12/src/EGamma/ECGelec/test/eleTreeProd_Simon_newRun2Id_255_.py  .
cd -
#cd \${lxbatchpwd}

echo "Starting CMSSW" 
cmsRun  eleTreeProd_Simon_newRun2Id_255_.py  

mv -f  tree_testFastSim_TPG_checknow_255_NEWID25ns.root  /afs/cern.ch/work/f/fmeng/FANBOWORKINGAREA/CMSSW_7_4_12/src/EGamma/ECGelec/test/NewConfigue/.  

echo "LAST PWD" 'pwd'


import FWCore.ParameterSet.Config as cms

from EGamma.ECGelec.Options import *
options=getAnalysisStepOptions()

options.maxEvents=-1
options.isMC=0
options.goodlumi=''
#After parsing, options cannot be changed
options.parseArguments()

print "cmsRun with options: "
print "===================="
print options

import os
PyFilePath = os.environ['CMSSW_BASE'] + "/src/EGamma/ECGelec/test/"

# Read CFG file so that it is customized with the above globals
namespace = {'IsMC':options.isMC}
#execfile(PyFilePath + "eleTreeProd_L1Study_2012D_PRV1.py",namespace)
execfile(PyFilePath + "eleTreeProd_L1Study_SLHC.py",namespace)
print len(namespace)

process = namespace.get('process') 

if len(options.files) > 0 :
    process.source.fileNames =  cms.untracked.vstring(options.files)
print process.source.fileNames 
    
filesuffix = str(options.tag)
if options.tag=='':
    filesuffix = ""
process.TFileService.fileName = cms.string(options.filePrepend+"tree_L1"+filesuffix+".root")

print process.TFileService.fileName 



if not bool(options.isMC):
  if options.goodlumi!='':
      print "JSON: " + options.goodlumi
      
      import FWCore.PythonUtilities.LumiList  as LumiList
      myLumis = LumiList.LumiList(filename = PyFilePath + options.goodlumi).getCMSSWString().split(',')
      process.source.lumisToProcess.extend(myLumis)
      #print process.source.lumisToProcess
  else :
      print "***WARNING: Running on data with no JSON goodlumi selection."

process.maxEvents.input = options.maxEvents
print process.maxEvents.input 

# ---------------------------------------------------------------------
# Global Tag
# ---------------------------------------------------------------------

# 2012D PRV1 534
process.GlobalTag.globaltag = 'GR_P_V42_AN2::All'
if bool(options.isMC) and options.globaltag=='NOT_PROVIDED':
  my_global_tag='XXXXXXXXXXXXXX'
elif options.globaltag=='NOT_PROVIDED':
  print "Not MC and no global tag provided. Using default"
  my_global_tag='GR_P_V42_AN2'
else:
  my_global_tag=options.globaltag
  
process.GlobalTag.globaltag = my_global_tag+'::All'
print process.GlobalTag.globaltag


process.MyHLTSelection.HLT_paths = cms.vstring(
        'HLT_Ele17_CaloIdL_CaloIsoVL_v*',
        'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Jet30_v*',
        'HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*',
        'HLT_Ele23_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_HFT30_v*', 
        'HLT_Ele27_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele15_CaloIdT_CaloIsoVL_trackless_v*', 
        'HLT_Ele27_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_HFT15_v*', 
        'HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_SC17_Mass50_v*',
        'HLT_Ele8_CaloIdL_CaloIsoVL_v*',
        'HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Jet30_v*',
        'HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*',
        'HLT_Ele8_CaloIdT_TrkIdVL_EG7_v*',
        'HLT_Ele8_CaloIdT_TrkIdVL_Jet30_v*',
        'HLT_Ele8_CaloIdT_TrkIdVL_v*',
        'HLT_Ele20_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC4_Mass50_v*'        
        )
        










          







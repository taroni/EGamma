# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step2 --filein file:MyDIGIs.root --fileout file:MyRECO.root --mc --eventcontent RAW,RECO --runUnscheduled --datatier RAW,RECO --conditions 80X_mcRun2_asymptotic_2016_v3 --step RAW2DIGI,L1Reco,RECO,EI --era Run2_25ns --no_exec
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('RECO',eras.Run2_2016)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
#process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('CommonTools.ParticleFlow.EITopPAG_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')


process.load('Configuration.Geometry.GeometryExtended2016Reco_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(5)
)

# Input source
process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring('file:myDIGIs.root'),
#    fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/n/ndev/public/for_nancy/splash_events_2016_run267996.root'),
    fileNames = cms.untracked.vstring('root://cmsxrootd.fnal.gov//store/data/Run2016A/ZeroBias1/RAW/v1/000/271/195/00000/AA165F77-170A-E611-A554-02163E01440F.root' ),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step2 nevts:1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RAWoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RAW'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('file:MyRawOutput.root'),
    outputCommands = process.RAWEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

process.RECOoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RECO'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('file:MyRECOZeroBias2016.root'),
    outputCommands = process.RECOEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_Prompt_v4', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')




# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.eventinterpretaion_step = cms.Path(process.EIsequence)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWoutput_step = cms.EndPath(process.RAWoutput)
process.RECOoutput_step = cms.EndPath(process.RECOoutput)


from EventFilter.L1TRawToDigi.caloStage2Digis_cfi import caloStage2Digis
process.rawCaloStage2Digis = caloStage2Digis.clone()
process.rawCaloStage2Digis.InputLabel = cms.InputTag('rawDataCollector')



# Schedule definition
#process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.eventinterpretaion_step,process.endjob_step,process.RAWoutput_step,process.RECOoutput_step)

process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.RECOoutput_step)



process.RECOoutput.outputCommands.append('drop *')

process.RECOoutput.outputCommands.append('keep *_*_EcalRecHitsEB_*')
process.RECOoutput.outputCommands.append('keep *_*_EcalRecHitsEE_*')
process.RECOoutput.outputCommands.append('keep *_*_EcalRecHitsES_*')
process.RECOoutput.outputCommands.append('keep *_*_EcalUncalibRecHitsEB_*')
process.RECOoutput.outputCommands.append('keep *_*_EcalUncalibRecHitsEE_*')
process.RECOoutput.outputCommands.append('keep *_reducedEcalRecHitsEB_*_*')
process.RECOoutput.outputCommands.append('keep *_reducedEcalRecHitsEE_*_*')
process.RECOoutput.outputCommands.append('keep *_reducedEcalRecHitsES_*_*')
process.RECOoutput.outputCommands.append('keep *_ecalDigis_EcalTriggerPrimitives_*')
process.RECOoutput.outputCommands.append('keep *_ecalDigis_ebDigis_*')
process.RECOoutput.outputCommands.append('keep *_ecalDigis_eeDigis_*')
process.RECOoutput.outputCommands.append('keep *_ecalPreshowerDigis_*_*')
process.RECOoutput.outputCommands.append('keep *_gtDigis_*_*')
process.RECOoutput.outputCommands.append('keep *_rawDataCollector_*_*')
process.RECOoutput.outputCommands.append('keep *_offlinePrimaryVerticesWithBS_*_*')
process.RECOoutput.outputCommands.append('keep *_TriggerResults_*_*')
process.RECOoutput.outputCommands.append('keep *_hltTriggerSummaryAOD_*_*')
process.RECOoutput.outputCommands.append('keep *_gedGsfElectrons_*_*')
process.RECOoutput.outputCommands.append('keep *_gedGsfElectronCores_*_*')
process.RECOoutput.outputCommands.append('keep *_electronGsfTracks_*_*')
process.RECOoutput.outputCommands.append('keep *_electronMergedSeeds_*_*')
process.RECOoutput.outputCommands.append('keep *_allConversions_*_*')
process.RECOoutput.outputCommands.append('keep recoSuperClusters_*_*_*')
process.RECOoutput.outputCommands.append('keep *_towerMaker_*_*')
process.RECOoutput.outputCommands.append('keep *_generalTracks_*_*')
process.RECOoutput.outputCommands.append('keep recoCaloClusters_*_*_*')
process.RECOoutput.outputCommands.append('keep *_offlineBeamSpot_*_*')
















#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)
from FWCore.ParameterSet.Utilities import cleanUnscheduled
process=cleanUnscheduled(process)


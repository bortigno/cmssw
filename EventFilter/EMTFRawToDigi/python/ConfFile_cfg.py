import FWCore.ParameterSet.Config as cms

process = cms.Process("OWNPARTICLES")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("NewEventStreamFileReader",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
#	'file:///afs/cern.ch/user/a/abrinke1/public/dat_dumps/run261113_ls0035_streamA_StorageManager.dat'
#	'file:///afs/cern.ch/user/a/abrinke1/public/dat_dumps/run261113_ls0025_streamA_StorageManager.dat'
    #    '/store/t0streamer/Minidaq/A/000/260/696/run260696_ls0006_streamA_StorageManager.dat'
#        '/store/t0streamer/Minidaq/A/000/261/105/run261105_ls0001_streamA_StorageManager.dat'
        'file:////afs/cern.ch/work/a/abrinke1/public/EMTF/miniDAQ/dat_dumps/2015_11_11/261105/run261105_ls0001_streamA_StorageManager.dat'
    )
)

process.myProducerLabel = cms.EDProducer('EMTFUnpacker',
	   producer = cms.InputTag("rawDataCollector")
	   )

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('myOutputFile.root')
)

process.load("Configuration.StandardSequences.RawToDigi_Data_cff")

 
process.rawToDigiPath = cms.Path(process.RawToDigi)
 
process.p = cms.Path(process.myProducerLabel)

process.e = cms.EndPath(process.out)

process.schedule = cms.Schedule(process.p,
				process.e)
#process.schedule = cms.Schedule(process.rawToDigiPath,
                              #  process.p,
                               # process.e)


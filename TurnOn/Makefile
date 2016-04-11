JOBID         = ratioScan
CC            = g++ -g
LIBS          = `root-config --cflags --libs` -L $(ROOFITSYS)/lib -lRooFit -lRooFitCore -I$(ROOFITSYS)/include 

SRCS          = FuncCB.cc fitEfficiency.cc 
SRCS_LC       = FuncCB.cc fitEfficiency_EGtrigger.cc 
SRCSERR       = FuncCB.cc FindValueErr.C 
SELECTSRC     = selectPairs.cc
SELECTSRC_2     = selectPairs_2.cc
MAKESRC       = makePairs.cc
MAKESRC_2       = makePairs_2.cc
RESOLUTIONSRC =  resolutionFit.cc 

OBJS          = FuncCB.o fitEfficiency.o 
OBJS_LC       = FuncCB.o fitEfficiency_EGtrigger.o 

FITERROR      =FindValueErr
FIT           = fitEfficiency
FIT_LC        = fitEfficiency_EGtrigger
SELECT        = selectPairs
SELECT_2        = selectPairs_2
MAKEPAIRS     = makePairs
MAKEPAIRS_2     = makePairs_2
RESOLUTION    = resolution

all: $(FIT) $(FILE_EXISTS)

$(FIT):   $(SRCS) $(SELECT) 
	echo "Linking $(FIT) ..."
	g++  -o $(FIT) $(SRCS) $(LIBS)

$(FIT_LC):   $(SRCS_LC)
	echo "Linking $(FIT) ..."
	g++  -o $(FIT_LC) $(SRCS_LC) $(LIBS)


$(FITERROR):   $(SRCSERR) 
	echo "Linking $(FITERROR) ..."
	g++  -o $(FITERROR) $(SRCSERR) $(LIBS)

$(SELECT): $(SELECTSRC) $(MAKEPAIRS) 
	g++  -o $(SELECT) $(SELECTSRC) $(LIBS)
	test -d $(JOBID) || make $(MAKEPAIRS)  make runPairs
	test -d $(JOBID)/selectPairsDir || mkdir $(JOBID)/selectPairsDir
$(SELECT_2): $(SELECTSRC_2) $(MAKEPAIRS_2) 
	g++  -o $(SELECT_2) $(SELECTSRC_2) $(LIBS)
	test -d $(JOBID) || make $(MAKEPAIRS_2)  make runPairs
	test -d $(JOBID)/selectPairsDir || mkdir $(JOBID)/selectPairsDir

$(MAKEPAIRS): $(MAKESRC) 
	g++  -o $(MAKEPAIRS) $(MAKESRC) $(LIBS)
	test -d $(JOBID) || mkdir $(JOBID)
	test -d $(JOBID)/makePairsDir || mkdir $(JOBID)/makePairsDir
$(MAKEPAIRS_2): $(MAKESRC_2) 
	g++  -o $(MAKEPAIRS_2) $(MAKESRC_2) $(LIBS)
	test -d $(JOBID) || mkdir $(JOBID)
	test -d $(JOBID)/makePairsDir || mkdir $(JOBID)/makePairsDir

$(RESOLUTION): $(RESOLUTIONSRC) 
	g++  -o $(RESOLUTION) $(RESOLUTIONSRC) $(LIBS)
	test -d $(JOBID) || mkdir $(JOBID)
	test -d $(JOBID)/resolutionDir || mkdir $(JOBID)/resolutionDir


runPairs: $(MAKEPAIRS)
	./$(MAKEPAIRS)  $(JOBID)
runPairs_2: $(MAKEPAIRS_2)
	./$(MAKEPAIRS_2)  $(JOBID)
runSelect: $(SELECT)
	./$(SELECT) $(JOBID)
runSelect_2: $(SELECT_2)
	./$(SELECT_2) $(JOBID)
runFit: $(FIT)
	./$(FIT) $(JOBID)
runFitERR: $(FITERROR)
	./$(FITERROR) $(JOBID)
runResolution: $(RESOLUTION)
	./$(RESOLUTION) $(JOBID)
runAll: $(FIT)
	./$(MAKEPAIRS)  $(JOBID)
	./$(SELECT) $(JOBID)
	./$(FIT) $(JOBID)


cleanSelect:
	rm -f $(SELECT)

cleanPairs: 
	rm -f $(MAKEPAIRS)
cleanFit:
	rm -f $(FIT)
clean:
	rm -f $(OBJS) core $(FIT) $(SELECT) $(MAKEPAIRS)


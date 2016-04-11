JOBID         = ratioScan/EE2015
dir1=ratioScan
dir2=EE2015
BOOST_LIB=/afs/cern.ch/sw/lcg/external/Boost/1.55.0_python2.7/x86_64-slc6-gcc47-opt/lib
BOOST_PATH=/afs/cern.ch/sw/lcg/external/Boost/1.55.0_python2.7/x86_64-slc6-gcc47-opt/
CC            = g++ -g
LIBS          = `root-config --cflags --libs` -L $(ROOFITSYS)/lib -lRooFit -lRooFitCore -I$(ROOFITSYS)/include 
# -I$(BOOST_PATH)/include -L/usr/local/lib  -L$(BOOST_PATH)/lib -lboost_system -lboost_filesystem


SRCS          = FuncCB.cc fitEfficiency.cc 
NEWFITSRC     = FuncCB.cc newFitEfficiency.cc 
SELECTSRC     = selectPairs.cc
MAKESRC       = makePairs.cc
CANDSRC       = makeTriggerCand.cc

OBJS          = FuncCB.o fitEfficiency.o 

FIT           = fitEfficiency
NEWFIT        = newFitEfficiency
SELECT        = selectPairs
MAKEPAIRS     = makePairs
MAKECANDS     = makeTriggerCand

FILE =tree_31_1_4Fg.root

SUBDIR = /

all: $(FIT) $(FILE_EXISTS)

$(FIT):   $(SRCS) $(SELECT) 
	echo "Linking $(FIT) ..."
	g++  -o $(FIT) $(SRCS) $(LIBS)
$(NEWFIT):   $(NEWFITSRC) $(SELECT) 
	echo "Linking $(NEWFIT) ..."
	g++  -o $(NEWFIT) $(NEWFITSRC) $(LIBS)

$(SELECT): $(SELECTSRC) $(MAKEPAIRS) 
	g++  -o $(SELECT) $(SELECTSRC) $(LIBS)
ifeq ($(dir2), $(findstring $(dir2), $(JOBID)))
	mkdir -p $(dir1)
endif
	mkdir -p $(JOBID)
	test -d $(JOBID)/selectPairsDir || mkdir $(JOBID)/selectPairsDir

$(MAKEPAIRS): $(MAKESRC) 
	g++  -o $(MAKEPAIRS) $(MAKESRC) $(LIBS)
ifeq ($(dir2), $(findstring $(dir2), $(JOBID)))
	test -d $(dir1) || mkdir $(dir1)
endif
	test -d $(JOBID) || mkdir $(JOBID)
	test -d $(JOBID)/makePairsDir || mkdir $(JOBID)/makePairsDir

$(MAKECANDS): $(CANDSRC)
	g++  -o $(MAKECANDS) $(CANDSRC) $(LIBS)



runPairs: $(MAKEPAIRS)
	./$(MAKEPAIRS)  $(JOBID)  $(FILE)
runSelect: $(SELECT)
	./$(SELECT) $(JOBID)  $(FILE)
runFit: $(FIT)
	./$(FIT) $(JOBID) $(FILE)
runNewFit: $(NEWFIT)
	./$(NEWFIT) $(JOBID) $(FILE)

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


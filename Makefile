#dir for .h files
IDIR = include

#dir for source files - .c files
SDIR = src

#dir for .o files
ODIR = obj

#dir for library files
LDIR = lib

#specify compiler
CC = mpic++

#Flags: -I => spec path gcc should look into for deps
CFLAGS = -I$(IDIR) -g -std=gnu++0x

#Spec library
LIBS = -lm
	
#list .h files
_DEPS = Event.h LP.h TrafficLoader.h TopologyUtils.h EventComparator.h Communicator.h SE.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
	
#spec rules for making .o files
$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
#list obj files	
_OBJ = Event.o LP.o TopologyUtils.o TrafficLoader.o Communicator.o SE.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
	
_OBJT = Event.o test/TestQ.o
OBJT = $(patsubst %,$(ODIR)/%,$(_OBJT))

_OBJCANTOR = CantorPairing.o test/TestCantorPairing.o
OBJCANTOR = $(patsubst %,$(ODIR)/%,$(_OBJCANTOR))

_TRAFF_OBJ = Event.o LP.o TopologyUtils.o TrafficLoader.o
TRAFF_OBJ = $(patsubst %,$(ODIR)/%,$(_TRAFF_OBJ))
		
sim: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

traffic: $(TRAFF_OBJ)
	$(CC) $(CFLAGS) -o $@ $(TRAFF_OBJ)

trafficGen: $(ODIR)/TrafficGen.o
	$(CC) $(CFLAGS) -o $@ $(ODIR)/TrafficGen.o

test: $(OBJT)
	$(CC) $(CFLAGS) -o $@ $(OBJT)
	
testCantor: $(OBJCANTOR)
	$(CC) $(CFLAGS) -o $@ $(OBJCANTOR)
		
clean:
	rm -f $(ODIR)/*.o  $(ODIR)/test/* *~ core $(IDIR)/*~ $(SDIR)/*~ $(IDIR)/*.gch tw testQ testHeap collComm foo testPosMsg testAntiMsg trafficGen testConstructLPs bar sim test traffic

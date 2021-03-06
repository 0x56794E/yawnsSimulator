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
_DEPS = Event.h LP.h TrafficLoader.h TopologyUtils.h Communicator.h SE.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
	
#spec rules for making .o files
$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
#list obj files	
_OBJ = Event.o LP.o TopologyUtils.o TrafficLoader.o Communicator.o SE.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
	
_OBJT = test/TestMisc.o
OBJT = $(patsubst %,$(ODIR)/%,$(_OBJT))

_OBJCANTOR = CantorPairing.o test/TestCantorPairing.o
OBJCANTOR = $(patsubst %,$(ODIR)/%,$(_OBJCANTOR))

_TRAFF_OBJ = Event.o LP.o TopologyUtils.o TrafficLoader.o
TRAFF_OBJ = $(patsubst %,$(ODIR)/%,$(_TRAFF_OBJ))
		
sim: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

traffic: $(TRAFF_OBJ)
	$(CC) $(CFLAGS) -o $@ $(TRAFF_OBJ)

test: $(OBJT)
	$(CC) $(CFLAGS) -o $@ $(OBJT)
	
testCantor: $(OBJCANTOR)
	$(CC) $(CFLAGS) -o $@ $(OBJCANTOR)
		
clean:
	rm -f $(ODIR)/*.o  $(ODIR)/test/* *~ core $(IDIR)/*~ $(SDIR)/*~ $(IDIR)/*.gch tw testMisc testHeap collComm foo testPosMsg testAntiMsg testConstructLPs bar sim test traffic testCantor *.class

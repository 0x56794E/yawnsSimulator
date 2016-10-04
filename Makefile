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
_DEPS = Event.h SimExec.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
	
#spec rules for making .o files
$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
#list obj files	
_OBJ = Event.o SimExec.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
	
_OBJT = Event.o test/TestQ.o
OBJT = $(patsubst %,$(ODIR)/%,$(_OBJT))
		
sim: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)
	
test: $(OBJT)
	$(CC) $(CFLAGS) -o $@ $(OBJT)
	
clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~ $(SDIR)/*~ $(IDIR)/*.gch tw testQ testHeap collComm foo testPosMsg testAntiMsg testConstructLPs bar sim test

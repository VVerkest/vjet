os = $(shell uname -s)

INCFLAGS      = -I$(shell root-config --incdir) $(shell fastjet-config --cxxflags) -I$(shell pythia8-config --includedir) -I$(BOOSTDIR)/include -I$(STARPICODIR) -I$(FJCONTRIB)/RecursiveTools

ifeq ($(os),Linux)
CXXFLAGS      = -std=c++11
else
CXXFLAGS      = -O -std=c++11 -fPIC -pipe -Wall -Wno-deprecated-writable-strings -Wno-unused-variable -Wno-unused-private-field -Wno-gnu-static-float-init
## for debugging:
# CXXFLAGS      = -g -O0 -fPIC -pipe -Wall -Wno-deprecated-writable-strings -Wno-unused-variable -Wno-unused-private-field -Wno-gnu-static-float-init
endif

ifeq ($(os),Linux)
LDFLAGS       = -g
LDFLAGSS      = -g --shared
else
LDFLAGS       = -O -Xlinker -bind_at_load -flat_namespace
LDFLAGSS      = -flat_namespace -undefined suppress
LDFLAGSSS     = -bundle
endif

ifeq ($(os),Linux)
CXX          = g++
else
CXX          = clang
endif


ROOTLIBS      = $(shell root-config --libs)
FJLIBS        = $(shell fastjet-config --plugins=yes --libs)
PYTHIALIBS    = $(shell pythia8-config --ldflags)
LIBPATH       = -L$(FASTJETDIR)/lib -L$(STARPICODIR) $(shell root-config --libs) -L$(FJCONTRIB)
LIBS          =  $(ROOTLIBS) $(FJLIBS) -I$(FJCONTRIB) $(PYTHIALIBS) -lfastjet -lfastjettools -lTStarJetPico -lRecursiveTools

# for cleanup
SDIR          = src
ODIR          = src/obj
BDIR          = bin


###############################################################################
################### Remake when these headers are touched #####################
###############################################################################


###############################################################################
# standard rules
$(ODIR)/%.o : $(SDIR)/%.cxx $(INCS)
	@echo
	@echo COMPILING
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -c $< -o $@

$(BDIR)/%  : $(ODIR)/%.o
	@echo
	@echo LINKING
	$(CXX) $(LDFLAGS) $^ -o $@ $(LIBPATH) $(LIBS)

###############################################################################

###############################################################################
############################# Main Targets ####################################
###############################################################################
all : $(BDIR)/vjetfinder

$(ODIR)/vjfunctions.o : $(SDIR)/vjfunctions.cxx $(SDIR)/vjfunctions.hh
$(ODIR)/vjetfinder.o : $(SDIR)/vjetfinder.cxx

#data analysis
$(BDIR)/vjetfinder :	$(ODIR)/vjetfinder.o	$(ODIR)/vjfunctions.o


###############################################################################
##################################### MISC ####################################
###############################################################################

clean :
	@echo
	@echo CLEANING
	rm -vf $(ODIR)/*.o
	rm -vf $(BDIR)/*
	rm -vf lib/*


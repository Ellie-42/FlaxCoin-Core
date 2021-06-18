CXXFLAGS = -Wall -Wextra -Wpedantic -O3 -g -march=native -fomit-frame-pointer -flto -mavx2 -msse2
LDFLAGS = -lpthread -ljsoncpp -lbotan-2 -lSphincs -lbz2 -llzma -lz
otherFlags = -static -m64 -fopenmp


sphincsLib = $(CURDIR)/SPMSource/lib/
sphincsHeaders = $(CURDIR)/SPMSource/include/

#the mingw64 libraries are only because I compile on windows. You can omit these if you are not using msys2
INCLUDES = -I$(CURDIR)/ -I$(CURDIR)/include/ -I$(sphincsHeaders) -I/c/msys64/mingw64/include/ -I/c/msys64/mingw64/include/botan-2

srcDir = $(CURDIR)/src/
objDir = $(CURDIR)/obj/
INSTALLDIR = $(CURDIR)/lib/
bin = $(CURDIR)/bin/

source = $(wildcard $(srcDir)*.cpp)
objects = $(patsubst $(srcDir)%, $(objDir)%, $(source:.cpp=.o))
test_src = $(wildcard *.cpp)
test_obj = $(test_src:.cpp=.o)

tests = testMain

sphincs : 
	$(MAKE) -C SPMSource install

$(objects) : %.o : $(source)
	mkdir -p $(objDir)
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $< -o $@ $(otherFlags) $(LDFLAGS)

$(test_obj) : %.o : %.cpp
	echo $@
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) -L$(INSTALLDIR) -L$(sphincsLib) $< -o $@ $(otherFlags) $(LDFLAGS) -lFlax

$(tests) : libFlax.a $(test_obj)
	mkdir -p $(bin)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -L$(INSTALLDIR) -L$(sphincsLib) -o $@  $(test_obj) $(otherFlags) $(LDFLAGS) -lFlax

libFlax.a : $(objects) sphincs
	mkdir -p $(INSTALLDIR)
	ar rcs $@ $(objects)
	cp -p libFlax.a $(INSTALLDIR)
	rm libFlax.a
	
.PHONY : install
install : libFlax.a

default : libFlax.a

	


	
.PHONY : test
test : $(tests)

.PHONY : all
all : test

.PHONY : clean
clean :
	-$(RM) -rf *.a *.o $(INSTALLDIR) $(objDir) $(srcDir)/.o $(bin)
	
.PHONY : cleansphincs
cleansphincs :
	$(MAKE) -C SPMSource clean

.PHONY : cleanall
cleanall : clean cleansphincs

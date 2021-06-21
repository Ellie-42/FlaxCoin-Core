#call "make" to generate the library SPHINCS+ and FlaxCore
#call "make test" to generate the executables for the test programs. Currently a WIP

#C++ Flags
CXXFLAGS = -Wall -Wextra -Wpedantic -O3 -g -march=native -fomit-frame-pointer -flto -mavx2 -msse2
#Linker flags
LDFLAGS =  -lSphincs -lbz2 -llzma -lz -lpthread -ljsoncpp -lbotan-2
#flags that I find are necessary
otherFlags = -m64 -fopenmp 

#Where SPHINCS+ is located
sphincsLib = $(CURDIR)/SPMSource/lib/
#Where SPHINCS+ Headers are located
sphincsHeaders = $(CURDIR)/SPMSource/include/

#the mingw64 libraries are only because I compile on windows. You can omit these if you are not using msys2
INCLUDES = -I$(CURDIR)/ -I$(CURDIR)/include/ -I$(sphincsHeaders) -I/c/msys64/mingw64/include/ -I/c/msys64/mingw64/include/botan-2

#Source directory
srcDir = $(CURDIR)/src/
#Where to place the object files
objDir = $(CURDIR)/obj/
#Where to install the library
INSTALLDIR = $(CURDIR)/lib/
#Where to place the test files
bin = $(CURDIR)/bin/

#Gather all the source files in the src directory
source = $(wildcard $(srcDir)*.cpp)
#Define the object files to be made from the source files
#It changes the file path from the src directory to the obj directory
objects = $(patsubst $(srcDir)%, $(objDir)%, $(source:.cpp=.o))
#The source files for the test code. Currently jus the CWD root
test_src = $(wildcard *.cpp)
#Create object files from the source files
test_obj = $(test_src:.cpp=.o)

#List the test programs that are to be compiled. Use the name they are called without an extension
tests = testMain

#compile the sphics library
sphincs : 
	$(MAKE) -C SPMSource install

#Compile the object files from the corresponding source files
$(objects) : %.o : $(source)
	#create the object directory if it doesnt exist
	mkdir -p $(objDir)
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $< -o $@ $(otherFlags) $(LDFLAGS)

#compile the test object files. Currently a WIP because the makefile wont work but my codeblocks compile does. FTW.
$(test_obj) : %.o : %.cpp
	echo $@
	#This works on my computer. The original code did not. What the hell.
	#g++.exe -Wall -fopenmp -march=native -g -fopenmp -march=native -I/p/FLAX/FLAX_Lib_Dev/include -I/p/FLAX/FLAX_Lib_Dev/SPMSource/include -I/c/msys64/mingw64/include/botan-2 -I/c/msys64/mingw64/include -c $< -o $@

#This is supposed to make the executable file. See above, ftw.
$(tests) : libFlax.a $(test_obj)
	mkdir -p $(bin)
	echo $@
	#it literally throws undefined errors. No clue why. CodeBlocks compilation works just fine. See the dev branch for the most recent codeblocks builds
	$(CXX) -L$(INSTALLDIR) -L$(sphincsLib) -o $@  $(test_obj) -fopenmp -lFlax $(LDFLAGS) 

#Archive the library and place it in the install directory
libFlax.a : $(objects) sphincs
	#make sure the directory exists
	mkdir -p $(INSTALLDIR)
	#archive the objects into a library called libFlax.a
	ar rcs $@ $(objects)
	#copy the library into the new directory
	cp -p libFlax.a $(INSTALLDIR)
	#remove the original copy
	rm libFlax.a
	
	
.PHONY : install
install : libFlax.a

#If the user calls plain "make" it will compile the library by default, and in turn, SPHINCS
default : libFlax.a


	
.PHONY : test
test : $(tests)

.PHONY : all
all : test

#cleans only the immediate files invovled for the FLAX library
.PHONY : clean
clean :
	-$(RM) -rf *.a *.o $(INSTALLDIR) $(bin) $(objDir) $(srcDir)/*.o 
	
#cleans only the sphincs library
.PHONY : cleansphincs
cleansphincs :
	$(MAKE) -C SPMSource clean

#Cleans both SPHINCS and FLAX
.PHONY : cleanall
cleanall : clean cleansphincs

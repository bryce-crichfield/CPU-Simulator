CC = g++
CXXFLAGS =  -Wextra -std=c++17 -lm 
CXXLIBS = 

SRCDIR = src
OBJDIR = obj
INCDIR = inc

#Target Executable Name
EXE = exe

#Object Files
OBJS = $(OBJDIR)/bus.o $(OBJDIR)/cpu.o $(OBJDIR)/ram.o $(OBJDIR)/main.o $(OBJDIR)/def.o

## [ Compile ] ##
# Link C++ and CUDA compiled object files to target exe
$(EXE) : $(OBJS)
	$(CC) $(CXXFLAGS) $(OBJS) -o $@ 

# Compile main .cpp file to object files:
$(OBJDIR)/%.o : %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@ 

# Compile C++ source files to object files
$(OBJDIR)/%.o : $(SRCDIR)/%.cpp $(INCDIR)/%.hpp
	$(CC) $(CXXFLAGS) -c $< -o $@ 

# Compile C++ header files to object files
# $(OBJDIR)/%.o : $(INCDIR)/%.cpp
# 	$(CC) $(CXXFLAGS) -c $< -o $@ 
	
# Clean objects in object directory
clean:
	rm -rf bin/* *.o $(EXE)
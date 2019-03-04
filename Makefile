# set this variable to the director in which you saved the common files
commondir = ./common/

COMMON_SOURCE = $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
CCFLAGS = -g -std=c++17
WARNFLAGS = -Wall -Wextra -pedantic -Wsuggest-attribute=const
# WARNFLAGS = -Wall -Wextra -Weffc++ -pedantic -Wsuggest-attribute=const
LIB_FLAGS = -lXt -lX11 -lGL -lm
BUILD_DIR = build/

OUT_DIR = bin/
OUT_FILENAME = waww
OUT = $(OUT_DIR)$(OUT_FILENAME)

all: $(BUILD_DIR)main.o $(BUILD_DIR)GL_utilities.o $(BUILD_DIR)VectorUtils3.o $(BUILD_DIR)loadobj.o $(BUILD_DIR)LoadTGA.o $(BUILD_DIR)MicroGlut.o
	g++ $(CCFLAGS) $(BUILD_DIR)main.o $(BUILD_DIR)GL_utilities.o $(BUILD_DIR)VectorUtils3.o $(BUILD_DIR)loadobj.o $(BUILD_DIR)LoadTGA.o $(BUILD_DIR)MicroGlut.o -o $(OUT) $(LIB_FLAGS)

$(BUILD_DIR)main.o: main.cc
	g++ $(CCFLAGS) $(WARNFLAGS) -c -DGL_GLEXT_PROTOTYPES $(LIB_FLAGS) main.cc -I$(commondir) -I$(commondir)Linux -o $(BUILD_DIR)main.o

$(BUILD_DIR)GL_utilities.o: $(commondir)GL_utilities.c $(commondir)GL_utilities.h
	g++ $(CCFLAGS) -c -DGL_GLEXT_PROTOTYPES $(LIB_FLAGS) $(commondir)GL_utilities.c -I$(commondir) -I$(commondir)Linux -o $(BUILD_DIR)GL_utilities.o

$(BUILD_DIR)VectorUtils3.o: $(commondir)VectorUtils3.c $(commondir)VectorUtils3.h
	g++ $(CCFLAGS) -c -DGL_GLEXT_PROTOTYPES $(LIB_FLAGS) $(commondir)VectorUtils3.c -I$(commondir) -I$(commondir)Linux -o $(BUILD_DIR)VectorUtils3.o

$(BUILD_DIR)loadobj.o: $(commondir)loadobj.c $(commondir)loadobj.h
	g++ $(CCFLAGS) -c -DGL_GLEXT_PROTOTYPES $(LIB_FLAGS) $(commondir)loadobj.c -I$(commondir) -I$(commondir)Linux -o $(BUILD_DIR)loadobj.o

$(BUILD_DIR)LoadTGA.o: $(commondir)LoadTGA.c $(commondir)LoadTGA.h
	g++ $(CCFLAGS) -c -DGL_GLEXT_PROTOTYPES $(LIB_FLAGS) $(commondir)LoadTGA.c -I$(commondir) -I$(commondir)Linux -o $(BUILD_DIR)LoadTGA.o

$(BUILD_DIR)MicroGlut.o: $(commondir)Linux/MicroGlut.c $(commondir)Linux/MicroGlut.h
	g++ $(CCFLAGS) -c -DGL_GLEXT_PROTOTYPES $(LIB_FLAGS) $(commondir)Linux/MicroGlut.c -I$(commondir) -I$(commondir)Linux -o $(BUILD_DIR)MicroGlut.o


clean:
	$(RM) *.o
	$(RM) *.out
	$(RM) $(BUILD_DIR)/*
	$(RM) $(OUT)/*
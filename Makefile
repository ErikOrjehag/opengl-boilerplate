# set this variable to the director in which you saved the common files
commondir = ./common/

COMMON_SOURCE = $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
SRC = src/
SOURCE_FILES = main.cc
OBJECTS=$(patsubst %, $(SRC)%, $(SOURCE_FILES))
CCFLAGS = -g -std=c++17
WARNFLAGS = -Wall -Wextra -pedantic -Wsuggest-attribute=const
# WARNFLAGS = -Wall -Wextra -Weffc++ -pedantic -Wsuggest-attribute=const
LIB_FLAGS = -lXt -lX11 -lGL -lm
BUILD_DIR = build/

OUT_DIR = bin/
OUT_FILENAME = waww
OUT = $(OUT_DIR)$(OUT_FILENAME)

COMMON_OFILES = $(BUILD_DIR)GL_utilities.o $(BUILD_DIR)VectorUtils3.o $(BUILD_DIR)loadobj.o $(BUILD_DIR)LoadTGA.o $(BUILD_DIR)MicroGlut.o

all: $(BUILD_DIR)main.o $(COMMON_OFILES)
	g++ $(CCFLAGS) $(BUILD_DIR)main.o $(COMMON_OFILES)  -o $(OUT) $(LIB_FLAGS)

$(BUILD_DIR)main.o: $(OBJECTS)
	g++ $(CCFLAGS) $(WARNFLAGS) -c -DGL_GLEXT_PROTOTYPES $(LIB_FLAGS) $(OBJECTS) -I$(SRC) -I$(commondir) -I$(commondir)Linux -o $(BUILD_DIR)main.o

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
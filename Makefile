# set this variable to the director in which you saved the common files
COMMON_DIR = ./common/

COMMON_SOURCE = $(COMMON_DIR)GL_utilities.c $(COMMON_DIR)VectorUtils3.c $(COMMON_DIR)loadobj.c $(COMMON_DIR)LoadTGA.c $(COMMON_DIR)Linux/MicroGlut.c
SRC = src/
SOURCE_FILES = main2.cc
SOURCE_FILES_PADDED=$(patsubst %, $(SRC)%, $(SOURCE_FILES))
CCFLAGS = -g -std=c++17
WARNFLAGS = -Wall -Wextra -pedantic -Wsuggest-attribute=const
# WARNFLAGS = -Wall -Wextra -Weffc++ -pedantic -Wsuggest-attribute=const
LIB_FLAGS = -lXt -lX11 -lGL -lm
BUILD_DIR = build/

OUT_DIR = bin/
OUT_FILENAME = waww
OUT = $(OUT_DIR)$(OUT_FILENAME)

COMMON_OFILES = $(BUILD_DIR)GL_utilities.o $(BUILD_DIR)VectorUtils3.o $(BUILD_DIR)loadobj.o $(BUILD_DIR)LoadTGA.o $(BUILD_DIR)MicroGlut.o


OBJDIR = build
OBJ = $(addprefix $(OBJDIR)/, $(patsubst %.cc, %.o, $(wildcard *.cc)))

$(OBJDIR):
	mkdir $(OBJDIR)

all: $(OBJDIR) waww

waww: $(BUILD_DIR)*.o
	g++ $(CCFLAGS) $(BUILD_DIR)*.o -o $(OUT) $(LIB_FLAGS)

$(OBJDIR)/%.o: $(SRC)%.cc $(COMMON_OFILES)
	g++ $(CCFLAGS) $(WARNFLAGS) -c $< -DGL_GLEXT_PROTOTYPES $(LIB_FLAGS) -I$(SRC) -I$(COMMON_DIR) -I$(COMMON_DIR)Linux -o $@

$(BUILD_DIR)GL_utilities.o: $(COMMON_DIR)GL_utilities.c $(COMMON_DIR)GL_utilities.h
	g++ $(CCFLAGS) -c -DGL_GLEXT_PROTOTYPES $(LIB_FLAGS) $(COMMON_DIR)GL_utilities.c -I$(COMMON_DIR) -I$(COMMON_DIR)Linux -o $(BUILD_DIR)GL_utilities.o

$(BUILD_DIR)VectorUtils3.o: $(COMMON_DIR)VectorUtils3.c $(COMMON_DIR)VectorUtils3.h
	g++ $(CCFLAGS) -c -DGL_GLEXT_PROTOTYPES $(LIB_FLAGS) $(COMMON_DIR)VectorUtils3.c -I$(COMMON_DIR) -I$(COMMON_DIR)Linux -o $(BUILD_DIR)VectorUtils3.o

$(BUILD_DIR)loadobj.o: $(COMMON_DIR)loadobj.c $(COMMON_DIR)loadobj.h
	g++ $(CCFLAGS) -c -DGL_GLEXT_PROTOTYPES $(LIB_FLAGS) $(COMMON_DIR)loadobj.c -I$(COMMON_DIR) -I$(COMMON_DIR)Linux -o $(BUILD_DIR)loadobj.o

$(BUILD_DIR)LoadTGA.o: $(COMMON_DIR)LoadTGA.c $(COMMON_DIR)LoadTGA.h
	g++ $(CCFLAGS) -c -DGL_GLEXT_PROTOTYPES $(LIB_FLAGS) $(COMMON_DIR)LoadTGA.c -I$(COMMON_DIR) -I$(COMMON_DIR)Linux -o $(BUILD_DIR)LoadTGA.o

$(BUILD_DIR)MicroGlut.o: $(COMMON_DIR)Linux/MicroGlut.c $(COMMON_DIR)Linux/MicroGlut.h
	g++ $(CCFLAGS) -c -DGL_GLEXT_PROTOTYPES $(LIB_FLAGS) $(COMMON_DIR)Linux/MicroGlut.c -I$(COMMON_DIR) -I$(COMMON_DIR)Linux -o $(BUILD_DIR)MicroGlut.o

clean:
	$(RM) *.o
	$(RM) *.out
	$(RM) $(BUILD_DIR)*
	$(RM) $(OUT)/*
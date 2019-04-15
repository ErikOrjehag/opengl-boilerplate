# set this variable to the director in which you saved the common files
COMMON_DIR = ./common/

COMMON_SOURCE = $(COMMON_DIR)GL_utilities.c $(COMMON_DIR)VectorUtils3.c $(COMMON_DIR)loadobj.c $(COMMON_DIR)LoadTGA.c $(COMMON_DIR)Linux/MicroGlut.c
SRC = src/
SOURCE_FILES = main.cc
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

all: $(BUILD_DIR)main.o $(BUILD_DIR)Shader.o $(BUILD_DIR)ScreenFill.o $(BUILD_DIR)FrameBuffer.o $(BUILD_DIR)Camera.o $(BUILD_DIR)Skybox.o $(BUILD_DIR)Terrain.o $(BUILD_DIR)Water.o $(BUILD_DIR)Object.o $(COMMON_OFILES)
	g++ $(CCFLAGS) $? -o $(OUT) $(LIB_FLAGS)

$(BUILD_DIR)main.o: $(SRC)main.cc
	g++ $(CCFLAGS) $(WARNFLAGS) -c -DGL_GLEXT_PROTOTYPES $(LIB_FLAGS) $(SRC)main.cc -I$(SRC) -I$(COMMON_DIR) -I$(COMMON_DIR)Linux -o $(BUILD_DIR)main.o

$(BUILD_DIR)Shader.o: $(SRC)Shader.cc $(SRC)Shader.hh
	g++ $(CCFLAGS) $(WARNFLAGS) -c -DGL_GLEXT_PROTOTYPES $(LIB_FLAGS) $(SRC)Shader.cc -I$(SRC) -I$(COMMON_DIR) -I$(COMMON_DIR)Linux -o $@

$(BUILD_DIR)ScreenFill.o: $(SRC)ScreenFill.cc $(SRC)ScreenFill.hh
	g++ $(CCFLAGS) $(WARNFLAGS) -c -DGL_GLEXT_PROTOTYPES $(LIB_FLAGS) $(SRC)ScreenFill.cc -I$(SRC) -I$(COMMON_DIR) -I$(COMMON_DIR)Linux -o $@

$(BUILD_DIR)FrameBuffer.o: $(SRC)FrameBuffer.cc $(SRC)FrameBuffer.hh
	g++ $(CCFLAGS) $(WARNFLAGS) -c -DGL_GLEXT_PROTOTYPES $(LIB_FLAGS) $(SRC)FrameBuffer.cc -I$(SRC) -I$(COMMON_DIR) -I$(COMMON_DIR)Linux -o $@

$(BUILD_DIR)Camera.o: $(SRC)Camera.cc $(SRC)Camera.hh
	g++ $(CCFLAGS) $(WARNFLAGS) -c -DGL_GLEXT_PROTOTYPES $(LIB_FLAGS) $(SRC)Camera.cc -I$(SRC) -I$(COMMON_DIR) -I$(COMMON_DIR)Linux -o $@

$(BUILD_DIR)Water.o: $(SRC)Water.cc $(SRC)Water.hh
	g++ $(CCFLAGS) $(WARNFLAGS) -c -DGL_GLEXT_PROTOTYPES $(LIB_FLAGS) $(SRC)Water.cc -I$(SRC) -I$(COMMON_DIR) -I$(COMMON_DIR)Linux -o $@

$(BUILD_DIR)Skybox.o: $(SRC)Skybox.cc $(SRC)Skybox.hh
	g++ $(CCFLAGS) $(WARNFLAGS) -c -DGL_GLEXT_PROTOTYPES $(LIB_FLAGS) $(SRC)Skybox.cc -I$(SRC) -I$(COMMON_DIR) -I$(COMMON_DIR)Linux -o $@

$(BUILD_DIR)Terrain.o: $(SRC)Terrain.cc $(SRC)Terrain.hh
	g++ $(CCFLAGS) $(WARNFLAGS) -c -DGL_GLEXT_PROTOTYPES $(LIB_FLAGS) $(SRC)Terrain.cc -I$(SRC) -I$(COMMON_DIR) -I$(COMMON_DIR)Linux -o $@

$(BUILD_DIR)Object.o: $(SRC)Object.cc $(SRC)Object.hh
	g++ $(CCFLAGS) $(WARNFLAGS) -c -DGL_GLEXT_PROTOTYPES $(LIB_FLAGS) $(SRC)Object.cc -I$(SRC) -I$(COMMON_DIR) -I$(COMMON_DIR)Linux -o $@

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

run:
	bin/waww

clean:
	$(RM) *.o
	$(RM) *.out
	$(RM) $(BUILD_DIR)*
	$(RM) $(OUT)/*
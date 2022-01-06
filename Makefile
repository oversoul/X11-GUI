CC = g++
BUILD = build
FILENAME = libxgui.so
SRC = $(wildcard src/*.cpp)
LIBS = $(wildcard libs/*.cpp)
OBJ_DIR = ${BUILD}/src
OBJ_LIB = ${BUILD}/libs
STANDARD = -std=c++0x 
OBJECTS = $(SRC:%.cpp=$(BUILD)/%.o)
LIBSOBJ = $(LIBS:%.cpp=$(BUILD)/%.o)
CFLAGS = -Wall -ansi ${STANDARD} -c -fpic
LDFLAGS = -lX11 -lXext -lXrandr -lXft
INCLUDE_DIR = -I include/ -I libs/ `pkg-config --cflags freetype2`

$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) ${INCLUDE_DIR} -c $< -o $@ ${LDFLAGS}

$(OBJ_LIB)/%.o: libs/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) ${INCLUDE_DIR} -c $< -o $@ ${LDFLAGS}

all : ${FILENAME}

xgui: $(OBJECTS) $(LIBSOBJ)
	$(CC) -c xgui.cpp -fpic -o $(BUILD)/xgui.o $(LDFLAGS) $(STANDARD) $(INCLUDE_DIR)

$(FILENAME): xgui
	@mkdir -p $(@D)
	$(CC) $(BUILD)/xgui.o $(OBJECTS) $(LIBSOBJ) -shared -o $(FILENAME) $(LDFLAGS) $(STANDARD) $(INCLUDE_DIR)

clean:
	@$(RM) -rf $(BUILD)
	@$(RM) -rf libxgui.so
	@echo "cleaning..."

CC = g++
BUILD = build
FILENAME = libxgui.so
SRC = $(wildcard src/*.cpp)
OBJ_DIR = ${BUILD}/src
STANDARD = -std=c++0x 
OBJECTS = $(SRC:%.cpp=$(BUILD)/%.o)
CFLAGS = -Wall -ansi ${STANDARD} -c -fpic
LDFLAGS = -lX11 -lXext -lXrandr -lXft
INCLUDE_DIR = -I include/  `pkg-config --cflags freetype2`

$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) ${INCLUDE_DIR} -c $< -o $@ ${LDFLAGS}

all : ${FILENAME}

xgui:
	$(CC) -c xgui.cpp -fpic -o $(BUILD)/xgui.o $(LDFLAGS) $(STANDARD) $(INCLUDE_DIR)

$(FILENAME): $(OBJECTS) xgui
	@mkdir -p $(@D)
	$(CC) $(BUILD)/xgui.o $(OBJECTS) -shared -o $(FILENAME) $(LDFLAGS) $(STANDARD) $(INCLUDE_DIR)

clean:
	@$(RM) -rf $(BUILD)/*.o
	@$(RM) -rf $(OBJ_DIR)/*.o
	@echo "cleaning..."

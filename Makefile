CC = g++
BUILD = build
FILENAME = libxgui.so
SRC = $(wildcard src/*.cpp)
OBJ_DIR = ${BUILD}/src
STANDARD = -std=c++0x 
OBJECTS = $(SRC:%.cpp=$(BUILD)/%.o)
CFLAGS = -Wall -ansi ${STANDARD} -c -fpic
LDFLAGS = -lX11 -lXext -lXrandr
INCLUDE_DIR = -I include/

$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) ${INCLUDE_DIR} -c $< -o $@ ${LDFLAGS}

all : ${FILENAME}

xgui:
	$(CC) -c xgui.cpp -fpic -o $(BUILD)/xgui.o $(LDFLAGS) $(STANDARD)

$(FILENAME): $(OBJECTS) xgui
	@mkdir -p $(@D)
	$(CC) $(BUILD)/xgui.o $(OBJECTS) -shared -o $(FILENAME) $(LDFLAGS) $(STANDARD)

clean:
	@$(RM) -rf build/
	@echo "cleaning..."

CC = g++
BUILD = build
SRC = $(wildcard src/*.cpp)
OBJECTS = $(SRC:%.cpp=$(BUILD)/%.o)
OBJ_DIR = ${BUILD}/src
STANDARD = -std=c++0x 
CFLAGS = -Wall -ansi ${STANDARD}
LDFLAGS = -lX11 -lXext -lXrandr
INCLUDE_DIR = -I include/


OBJECTS = $(BUILD)/src/painter.o \
	$(BUILD)/src/application.o \
	$(BUILD)/src/textinput.o \
	$(BUILD)/src/listview.o \
	$(BUILD)/src/widget.o \
	$(BUILD)/src/layout.o \
	$(BUILD)/src/button.o \
	$(BUILD)/src/label.o \
	$(BUILD)/src/typedefs.o



objects:
	$(CC) -c -fpic src/painter.cpp -o $(BUILD)/src/painter.o $(LDFLAGS) $(STANDARD) $(INCLUDE_DIR)
	$(CC) -c -fpic src/widget.cpp -o $(BUILD)/src/widget.o $(LDFLAGS) $(STANDARD) $(INCLUDE_DIR)
	$(CC) -c -fpic src/button.cpp -o $(BUILD)/src/button.o $(LDFLAGS) $(STANDARD) $(INCLUDE_DIR)
	$(CC) -c -fpic src/application.cpp -o $(BUILD)/src/application.o $(LDFLAGS) $(STANDARD) $(INCLUDE_DIR)
	$(CC) -c -fpic src/label.cpp -o $(BUILD)/src/label.o $(LDFLAGS) $(STANDARD) $(INCLUDE_DIR)
	$(CC) -c -fpic src/layout.cpp -o $(BUILD)/src/layout.o $(LDFLAGS) $(STANDARD) $(INCLUDE_DIR)
	$(CC) -c -fpic src/listview.cpp -o $(BUILD)/src/listview.o $(LDFLAGS) $(STANDARD) $(INCLUDE_DIR)
	$(CC) -c -fpic src/textinput.cpp -o $(BUILD)/src/textinput.o $(LDFLAGS) $(STANDARD) $(INCLUDE_DIR)
	$(CC) -c -fpic src/typedefs.cpp -o $(BUILD)/src/typedefs.o $(LDFLAGS) $(STANDARD) $(INCLUDE_DIR)
	$(CC) -c xgui.cpp -fpic -o $(BUILD)/xgui.o $(LDFLAGS) $(STANDARD)

	$(CC) $(BUILD)/xgui.o $(OBJECTS) -shared -o libxgui.so $(LDFLAGS) $(STANDARD)




#libxgui.so: xgui.cpp xgui.h $(OBJECTS)
#	$(CC) -c xgui.cpp -fpic -o $(BUILD)/xgui.so.o -lX11 $(STANDARD)
#	$(CC) $(BUILD)/xgui.so.o -shared -o libxgui.so -lX11 $(STANDARD)

#libxguia: xgui.cpp xgui.h
#	$(CC) -c xgui.cpp -o $(BUILD)/xgui.a.o -lX11 $(STANDARD)
#	$(AR) libhgui.a $(BUILD)/hgui.a.o

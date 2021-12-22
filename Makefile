CC = g++
C_STANDARD = -std=c++0x #-std=c11
INCLUDE_DIR = -I headers/ 
SOURCES_DIR = sources
BUILD_DIR = build
APPLICATION_NAME = app
FILENAME = ${BUILD_DIR}/${APPLICATION_NAME}
CFLAGS = -Wall -ansi ${C_STANDARD}
LDFLAGS = -lX11
DEBUG_SUFFIX = _debug
CFLAGS_DEBUG = -gdwarf-2 -DDEBUG
SRC = $(wildcard ${SOURCES_DIR}/*.cpp)
OBJ_DIR = ${BUILD_DIR}/${SOURCES_DIR}
OBJECTS = $(SRC:%.cpp=$(BUILD_DIR)/%.o)

$(OBJ_DIR)/%.o: $(SOURCES_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) ${INCLUDE_DIR} -c $< -o $@ ${LDFLAGS}

all : ${FILENAME} ${FILENAME}${DEBUG_SUFFIX}

$(FILENAME): $(OBJECTS)
	@mkdir -p $(@D)
	${CC} ${CFLAGS} -o $@  $^ ${INCLUDE_DIR} ${LDFLAGS}

$(FILENAME)$(DEBUG_SUFFIX): ${OBJECTS}
	${CC} ${CFLAGS} ${CFLAGS_DEBUG} -o $@ $^ ${INCLUDE_DIR} ${LDFLAGS}

clean:
	${RM} *.o ${FILENAME} ${FILENAME}${DEBUG_SUFFIX}
	${RM} -rf ${BUILD_DIR}/*.dSYM
	${RM} -rf ${OBJ_DIR}

run: ${FILENAME}
	${BUILD_DIR}/${APPLICATION_NAME}

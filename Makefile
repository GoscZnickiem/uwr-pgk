# ==================================================
# Portable makefile for compilation on platforms: 
#     Linux   /   Windows with MSYS2 using gcc
# install required packages like:
#       e.g. gcc, make,   libglfw3-dev, libglew-dev 
# ==================================================

# ==== TARGET ARCHITECTURE =========================
UNAMES := $(shell uname -o)
ifeq      (${UNAMES},GNU/Linux)
	CXXFLAGS = 
	LINKFLAGS = -lGL -lglfw -lGLEW -ldl
	EXE = 
else ifeq (${UNAMES},Msys)
	CXXFLAGS = -I /msys64/mingw64/include/
	LINKFLAGS = -L /msys64/mingw64/lib/ -lopengl32 -lglfw3 -lglew32
	EXE = .exe
	CXXFLAGS += -DARCH_MSYS=${UNAMES}
else
	TARGET = unknown-target
defaulterror:
	echo Unknown target architecture: uname -s 
endif
# ==================================================

# ==== PROJECT CONFIGURATION =======================
APP_NAME = app
COMPILER = g++

# yup
CXXFLAGS += -Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wconversion -Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wformat=2 -Wimplicit-fallthrough -Wmisleading-indentation -Wduplicated-cond -Wlogical-op -Wuseless-cast
# CXXFLAGS += -Werror	# possibly not helpful
DEBFLAGS = -g

SRC_DIR = src
OBJ_DIR = bin
DEB_DIR = debug
# ==================================================

TARGET_NAME := $(APP_NAME)$(EXE)
TARGET := $(TARGET_NAME)
TARGET_DEBUG := $(DEB_DIR)/$(TARGET_NAME)

SRCS := $(foreach x, $(SRC_DIR), $(wildcard $(addprefix $(x)/*,.c*)))
OBJS := $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(notdir $(basename $(SRCS)))))
OBJS_DEB := $(addprefix $(DEB_DIR)/, $(addsuffix .o, $(notdir $(basename $(SRCS)))))

CLEAN_LIST := $(TARGET) \
			  $(OBJ_DIR) \
			  $(DEB_DIR) 

default: release

$(TARGET): $(OBJS)
	$(COMPILER) $(OBJS) -o $@ $(LINKFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c*
	$(COMPILER) $(CXXFLAGS) -c -o $@ $<

$(TARGET_DEBUG): $(OBJS_DEB)
	$(COMPILER) $(OBJS_DEB) -o $@ $(LINKFLAGS)

$(DEB_DIR)/%.o: $(SRC_DIR)/%.c*
	$(COMPILER) $(CXXFLAGS) $(DEBFLAGS) -c -o $@ $<

.PHONY: makedir
makedir:
	@mkdir -p $(OBJ_DIR) $(DEB_DIR)

.PHONY: release
release: makedir $(TARGET)

.PHONY: debug
debug: makedir $(TARGET_DEBUG)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -fr $(CLEAN_LIST)

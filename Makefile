# ==================================================
# Portable makefile for compilation on platforms: 
#     Linux   /   Windows with MSYS2 using gcc
# install required packages like:
#       e.g. gcc, make,   libglfw3-dev, libglew-dev 
# ==================================================

# ==================================================
# ==== TARGET ARCHITECTURE =========================
# ==================================================
UNAMES := $(shell uname -o)
ifeq      (${UNAMES},GNU/Linux)
	COMPILER_FLAGS = 
	LINKFLAGS = -lGL -lglfw -lGLEW -ldl
	EXE = 
else ifeq (${UNAMES},Msys)
	COMPILER_FLAGS = -I /msys64/mingw64/include/
	LINKFLAGS = -L /msys64/mingw64/lib/ -lopengl32 -lglfw3 -lglew32
	EXE = .exe
	COMPILER_FLAGS += -DARCH_MSYS=${UNAMES}
else
	TARGET = unknown-target
defaulterror:
	echo Unknown target architecture: uname -s 
endif
# ==================================================

# ==================================================
# ==== PROJECT CONFIGURATION =======================
# ==================================================
APP_NAME = app
COMPILER = g++

# yup
COMPILER_FLAGS += -std=c++20 -Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wconversion -Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wformat=2 -Wimplicit-fallthrough -Wmisleading-indentation -Wduplicated-cond -Wlogical-op -Wuseless-cast
# COMPILER_FLAGS += -Werror	# possibly not helpful
DEBUG_FLAGS = -g

SRC_DIR = src
OBJ_DIR = bin
DEB_DIR = debug

TARGET_NAME := $(APP_NAME)$(EXE)
TARGET := $(TARGET_NAME)
TARGET_DEBUG := $(DEB_DIR)/$(TARGET_NAME)

SRCS := $(foreach x, $(SRC_DIR), $(wildcard $(addprefix $(x)/*,.c*)))
OBJS := $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(notdir $(basename $(SRCS)))))
OBJS_DEB := $(addprefix $(DEB_DIR)/, $(addsuffix .o, $(notdir $(basename $(SRCS)))))
# ==================================================

# ==================================================
# ==== BUILD TARGETS ===============================
# ==================================================
default: release

$(TARGET): $(OBJS)
	$(COMPILER) $(OBJS) -o $@ $(LINKFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c*
	$(COMPILER) $(COMPILER_FLAGS) -c -o $@ $<
# ==================================================

# ==================================================
# ==== DEBUG TARGETS ===============================
# ==================================================
$(TARGET_DEBUG): $(OBJS_DEB)
	$(COMPILER) $(OBJS_DEB) -o $@ $(LINKFLAGS)

$(DEB_DIR)/%.o: $(SRC_DIR)/%.c*
	$(COMPILER) $(COMPILER_FLAGS) $(DEBUG_FLAGS) -c -o $@ $<
# ==================================================

# ==================================================
# ==== PHONY TARGETS ===============================
# ==================================================
.PHONY: makedir
makedir:
	@mkdir -p $(OBJ_DIR) $(DEB_DIR)

.PHONY: release
release: makedir $(TARGET)

.PHONY: debug
debug: makedir $(TARGET_DEBUG)

.PHONY: clean
clean:
	@rm -fr $(OBJ_DIR) $(OBJS_DEB) 

.PHONY: clear
clear:
	@rm -fr $(OBJ_DIR) $(OBJS_DEB) $(TARGET) $(DEB_DIR)
# ==================================================

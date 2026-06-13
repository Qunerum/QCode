CC_LINUX = gcc
# CC_WIN = i686-w64-mingw32-gcc # 32 bit
CC_WIN = x86_64-w64-mingw32-gcc # 64 bit

CFLAGS = -Wall -Wextra -O2 -Iinclude
LDFLAGS_WIN = -static -static-libgcc -static-libstdc++

SRC_DIR = src
OBJ = obj
BUILD_DIR = build

ifeq ($(OS),Windows_NT)
    PLATFORM = win
    EXT = .exe
    TARGET_CC = $(CC_WIN)
    MKDIR = mkdir -p
else
    PLATFORM = linux
    EXT =
    TARGET_CC = $(CC_LINUX)
    MKDIR = mkdir -p
endif

PLATFORM_DIR = $(BUILD_DIR)/$(PLATFORM)
TARGET = $(PLATFORM_DIR)/qcode$(EXT)

OBJS = $(OBJ)/$(PLATFORM)/main.o \
      $(OBJ)/$(PLATFORM)/memory.o \
      $(OBJ)/$(PLATFORM)/mainCmds.o \
      $(OBJ)/$(PLATFORM)/utility.o

all: linux win

linux:
	@$(MAKE) PLATFORM=linux EXT= CC_LINUX=gcc build_platform

win:
	@$(MAKE) PLATFORM=win EXT=.exe TARGET_CC=$(CC_WIN) LDFLAGS="$(LDFLAGS_WIN)" build_platform

build_platform: $(TARGET)

$(OBJ)/$(PLATFORM)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR) $(OBJ)/$(PLATFORM)
	$(TARGET_CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	@$(MKDIR) $(PLATFORM_DIR)
	$(TARGET_CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

run: all
	@clear
	@echo "--- Running on $(PLATFORM) ---"
	./$(TARGET)

clean:
	rm -rf $(OBJ) $(BUILD_DIR)

.PHONY: all linux win run clean build_platform

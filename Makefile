### constants ###
NAME = radshift

# installation
INSTALL_PREFIX = /usr/bin

# directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
TARGET_DIR = $(BUILD_DIR)/debug
RELEASE_TARGET_DIR = $(BUILD_DIR)/release

# compilation
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Iinclude
LIBS = -lm -lxcb -lxcb-randr

# debug mode
DBFLAGS = -Werror -Wshadow -Wstrict-overflow -fno-strict-aliasing -g -Og -DDEBUG
TARGET_NAME = $(NAME)
TARGET = $(TARGET_DIR)/$(TARGET_NAME)

# release mode
OPTFLAGS = -Os -flto
RELEASE_TARGET_NAME = $(NAME)
RELEASE_TARGET = $(RELEASE_TARGET_DIR)/$(RELEASE_TARGET_NAME)

# files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(TARGET_DIR)/%.o, $(SRC_FILES))
RELEASE_OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(RELEASE_TARGET_DIR)/%.o, $(SRC_FILES))

### targets ###
.DEFAULT:
	@echo "Target not found:" $@
	@echo "Use 'make help' to print help text."

.PRECIOUS: $(TARGET) $(OBJECTS) $(RELEASE_TARGET) $(RELEASE_OBJECTS)

.PHONY: release
release: CFLAGS += $(OPTFLAGS)
release: $(RELEASE_TARGET)

.PHONY: debug
debug: CFLAGS += $(DBFLAGS)
debug: $(TARGET)

$(TARGET_DIR):
	mkdir -p $(TARGET_DIR)

$(RELEASE_TARGET_DIR):
	mkdir -p $(RELEASE_TARGET_DIR)

$(TARGET_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(TARGET_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(RELEASE_TARGET_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(RELEASE_TARGET_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $@

$(RELEASE_TARGET): $(RELEASE_OBJECTS)
	$(CC) $(CFLAGS) $(RELEASE_OBJECTS) $(LIBS) -o $@

.PHONY: install
install:
	cp $(RELEASE_TARGET) $(INSTALL_PREFIX)/$(NAME)

.PHONY: uninstall
uninstall:
	rm $(INSTALL_PREFIX)/$(NAME)

.PHONY: help
help:
	@echo "Makefile for" $(NAME)
	@echo ""
	@echo "Targets:"
	@echo "'debug'          build executable in debug mode"
	@echo "'release'        build executable in release mode (default)"
	@echo "'init'           create required directories"
	@echo "'clean'          remove generated binary files"
	@echo "'help'           print this help"

.PHONY: clean
clean:
	rm -f $(OBJECTS)
	rm -f $(RELEASE_OBJECTS)

.PHONY: mrproper
mrproper:
	rm -rf $(BUILD_DIR)

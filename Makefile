CC = clang
CFLAGS = -g
BIN_DIR = bin
LIB_DIR = lib
OBJ_DIR = obj
PREFIX = /usr
INCLUDE_DIR = include
SRC_DIR = src
TESTS_DIR = tests

SH = /bin/bash

.PHONY: clean install tests run_tests

all: $(LIB_DIR)/libwavefront.a tests

objects := $(patsubst %.c,%.o,$(notdir $(wildcard $(SRC_DIR)/*.c)))
tests := $(patsubst %.c,%,$(notdir $(wildcard $(TESTS_DIR)/*.c)))

$(LIB_DIR)/libwavefront.a: $(addprefix $(OBJ_DIR)/,$(objects))
	@mkdir -p $(@D)
	ar -rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/%.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

tests: $(addprefix $(BIN_DIR)/tests/,$(tests))

run_tests: tests
	@echo "Running tests..."
	$(SH) run_tests.sh "./bin/tests"

$(BIN_DIR)/tests/%: $(TESTS_DIR)/%.c $(LIB_DIR)/libwavefront.a
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ -L$(LIB_DIR) $< $(addprefix -l,$(patsubst lib%.a,%,$(notdir $(filter %.a,$^))))

clean:
	rm -rf $(LIB_DIR) $(OBJ_DIR) $(BIN_DIR)

install: all
	cp -n $(LIB_DIR)/libwavefront.a $(PREFIX)/lib/libwavefront.a
	@mkdir -p $(PREFIX)/include/wavefront/
	cp -n $(INCLUDE_DIR)/wavefront.h $(PREFIX)/include/wavefront/wavefront.h

uninstall:
	rm $(PREFIX)/lib/libwavefront.a
	rm -r $(PREFIX)/include/wavefront/

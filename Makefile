CC = clang
CFLAGS = -g
LIB_DIR = lib
OBJ_DIR = obj
PREFIX = /usr
INCLUDE_DIR = include
SRC_DIR = src

.PHONY: clean install

all: $(LIB_DIR)/libwavefront.a

$(LIB_DIR)/lib%.a: $(OBJ_DIR)/%.o
	@mkdir -p $(@D)
	ar -rcs $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/%.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(LIB_DIR) $(OBJ_DIR)

install: all
	cp -n $(LIB_DIR)/libwavefront.a $(PREFIX)/lib/libwavefront.a
	@mkdir -p $(PREFIX)/include/wavefront/
	cp -n $(INCLUDE_DIR)/wavefront.h $(PREFIX)/include/wavefront/wavefront.h

uninstall:
	rm $(PREFIX)/lib/libwavefront.a
	rm -r $(PREFIX)/include/wavefront/

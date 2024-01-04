CC = clang
LIB_DIR = lib
OBJ_DIR = obj

.PHONY: clean

all: $(LIB_DIR)/libwavefront.a

$(LIB_DIR)/lib%.a: $(OBJ_DIR)/%.o
	@mkdir -p $(@D)
	ar -rcs $@ $<

$(OBJ_DIR)/%.o: %.c %.h
	@mkdir -p $(@D)
	$(CC) -c $< -o $@

clean:
	rm -rf $(LIB_DIR) $(OBJ_DIR)

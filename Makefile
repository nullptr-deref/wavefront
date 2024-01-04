CC = clang
LIB_DIR = lib
OBJ_DIR = obj

.PHONY: clean

$(LIB_DIR)/libwavefront.a: $(OBJ_DIR)/wavefront.o
	@mkdir -p $(@D)
	ar -rcs $(@D)/libwavefront.a $(OBJ_DIR)/wavefront.o

$(OBJ_DIR)/%.o: %.c %.h
	@mkdir -p $(@D)
	$(CC) -c $< -o $@

clean:
	rm -rf $(LIB_DIR) $(OBJ_DIR)

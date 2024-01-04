C_COMPILER = clang
OUT_DIR = out

libwavefront.a: wavefront.o
	ar -rcs $(OUT_DIR)/libwavefront.a $(OUT_DIR)/wavefront.o

wavefront.o: wavefront.c wavefront.h
	$(C_COMPILER) -c wavefront.c -o $(OUT_DIR)/wavefront.o

clean:
	rm -f $(OUT_DIR)/*.o $(OUT_DIR)/*.a

CC = emcc
OPTS =  -O1 -std=c++17 -s USE_SDL=2 -s USE_WEBGL2=1 -s MAX_WEBGL_VERSION=2 -s FULL_ES3=1 -I /usr/local/include -g
#  -s FULL_ES3=1

CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(addprefix output/, $(CPP_FILES:.cpp=.bc))

all: main

main: $(OBJ_FILES)
	$(CC) $(OPTS) -o $@ $^ --js-library library.js -o output/index.js

output/%.bc: %.cpp
	$(CC) $(OPTS) -c -o $@ $<

clean:
	@- rm -f output/*.bc
	@- rm -f output/index.wasm
	@- rm -f output/index.js

clean-emcc:
	@- emcc --clear-cache

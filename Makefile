CC = emcc
OPTS =  -O3 -std=c++17 -s USE_WEBGL2=1 -s USE_GLFW=3 -s FULL_ES3=1 -I /usr/local/include -g
#  -s FULL_ES3=1

CPP_FILES := $(wildcard cpp/*.cpp)
OBJ_FILES := $(addprefix output/, $(notdir $(CPP_FILES:.cpp=.bc)))

all: main

main: $(OBJ_FILES)
	$(CC) $(OPTS) -o $@ $^ --js-library js/library.js -o output/index.js

output/$(notdir %.bc): cpp/%.cpp
	$(CC) $(OPTS) -c -o $@ $<

# tsc:
# 	@- tsc -m system --outFile output/app.js ts/app.ts
# 	@- tsc -m system --outFile output/library.js ts/library.ts

clean:
	@- rm -f output/*.bc
	@- rm -f output/index.wasm
	@- rm -f output/index.js
	@- rm -f output/app.js
	@- rm -f output/library.js

clean-emcc:
	@- emcc --clear-cache

OPTS =  -O3 -std=c++17 -s USE_WEBGL2=1 -s USE_GLFW=3 -s FULL_ES3=1 -I /usr/local/include -g
TSC_OPTS = --strictNullChecks --noImplicitAny

CPP_FILES := $(wildcard src/cpp/*.cpp)
OBJ_FILES := $(addprefix output/, $(notdir $(CPP_FILES:.cpp=.bc)))

all: tsc main

main: $(OBJ_FILES)
	emcc $(OPTS) -o $@ $^ --js-library output/library.js -o output/index.js

output/$(notdir %.bc): src/cpp/%.cpp
	emcc $(OPTS) -c -o $@ $<

tsc:
	@- tsc $(TSC_OPTS) -m system --outFile output/app.js src/ts/app.ts
	@- tsc $(TSC_OPTS) -m system --outFile output/library.js src/ts/library.ts

clean:
	@- rm -f output/*.bc
	@- rm -f output/index.wasm
	@- rm -f output/index.js
	@- rm -f output/app.js
	@- rm -f output/library.js

clean-emcc:
	@- emcc --clear-cache

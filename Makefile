EMCC_OPTS =  -s WASM=1 --bind -O1 -std=c++17 -s USE_WEBGL2=1 -s USE_GLFW=3 -s FULL_ES3=1 -I /usr/local/include -g
DEPENDENCY_OPTS = -MMD -MP

TSC_OPTS = --strictNullChecks --noImplicitAny

CPP_FILES := $(wildcard src/cpp/*.cpp)
OBJ_FILES := $(addprefix output/, $(notdir $(CPP_FILES:.cpp=.bc)))
DEPENDS := $(patsubst %.bc,%.d, $(OBJ_FILES))

TS_FILES := $(wildcard src/ts/*ts)

all: data/textures_manifest.json src/cpp/bindings.h output/index.js output/app.js

excluding_cpp: data/textures_manifest.json src/cpp/bindings.h output/app.js

src/cpp/bindings.h: generate_bindings.py schema.py
	python3 generate_bindings.py

data/textures_manifest.json: generate_manifests.py schema.py
	python3 generate_manifests.py

output/app.js: $(TS_FILES)
	npx esbuild --bundle src/ts/app.ts --outfile=output/app.js --sourcemap
	tsc $(TSC_OPTS) --noEmit

output/index.js: $(OBJ_FILES)
	emcc $(EMCC_OPTS) -o $@ $^ -o output/index.js

-include $(DEPENDS)
output/$(notdir %.bc): src/cpp/%.cpp
	emcc $(EMCC_OPTS) $(DEPENDENCY_OPTS) -c -o $@ $<

clean:
	@- rm -f output/*

clean-emcc:
	@- emcc --clear-cache

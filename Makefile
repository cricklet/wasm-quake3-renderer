EMCC_OPTS =  --bind -O1 -std=c++17 -s USE_WEBGL2=1 -s USE_GLFW=3 -s FULL_ES3=1 -I /usr/local/include -g
DEPENDENCY_OPTS = -MMD -MP

TSC_OPTS = --strictNullChecks --noImplicitAny

CPP_FILES := $(wildcard src/cpp/*.cpp)
OBJ_FILES := $(addprefix output/, $(notdir $(CPP_FILES:.cpp=.bc)))
DEPENDS := $(patsubst %.bc,%.d, $(OBJ_FILES))

TS_FILES := $(wildcard src/ts/*ts)

all: tsc cpp

tsc: $(TS_FILES) Makefile
	npx esbuild --bundle src/ts/app.ts --outfile=output/app.js --sourcemap
	# tsc $(TSC_OPTS) --noEmit

cpp: $(OBJ_FILES)
	emcc $(EMCC_OPTS) -o $@ $^ -o output/index.js

-include $(DEPENDS)
output/$(notdir %.bc): src/cpp/%.cpp Makefile
	emcc $(EMCC_OPTS) $(DEPENDENCY_OPTS) -c -o $@ $<

clean:
	@- rm -f output/*

clean-emcc:
	@- emcc --clear-cache

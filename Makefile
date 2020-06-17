all : output/index.js

output/index.js : main.cpp library.js
	emcc main.cpp --js-library library.js -std=c++11 -s WASM=1 -s USE_SDL=2 -O3 -o output/index.js

clean :
	rm output/index.js


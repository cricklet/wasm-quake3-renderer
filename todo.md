
Initial work
 - [x] Figure out how to call C++ from JS & visa versa

Injesting a BSP
(http://www.mralligator.com/q3/, http://graphics.cs.brown.edu/games/quake/quake3.html)
 - [x] Load a .bsp into JS memory
 - [x] Load that .bsp into C++

Getting a WebGL pipeline started:
 - [x] Load a GLSL file
 - [x] Get GLSL version 300 to load
 - [x] Try out GLFW
 - [x] Render a triangle
 - [x] Drawing polygons (open.gl)
 - [x] Textures (open.gl)

Ramping up WebGL:
 - [x] Draw a cube w/ WebGL
 - [x] Capture mouse input
 - [x] Capture WASD
 - [x] Fly around the cube

Generate C++ <=> TS bindings from a schema file
 - [x] Reorganize directory structure
 - [x] Setup Typescript
 - [x] Setup schema.json
 - [x] Setup generate_bindings.py
 - [x] Generate a ".h" file and a ".ts" definitions file

Rendering faces
 - [x] Split up setup/rendering code into different scenarios (eg test scenario, bsp scenario)
 - [x] Figure out how to load faces + vertices + edges into OpenGL
 - [x] Render faces with random colors

Rendering textures
 - [ ] During resource loading, find out what textures to download
 - [ ] Load textures in the ResourceManager
 - [ ] Bind textures to the GPU
 - [ ] Add texture coordinates to GLSL
 - [ ] Render textures

Rendering patches
 - [ ] Learn about how beziers work
 - [ ] Tesselate patches

Rendering lightmaps
 - [ ] Load lightmaps in the ResourceManger
 - [ ] Bind lightmap texture to the GPU
 - [ ] Add lightmap coordinates to GLSL
 - [ ] Render lightmaps

Find out what leaf we're in...
Render only visible leaves

Render collision faces (for debugging)

Player physics (ground + gravity, walking, jumping)

Player physics (wall collisions)

VQ3 physics (strafe jumping!)

Rendering + lighting player model (3rd person)

Animating player model

Bots that can navigate

Railgun

Lightning gun

Rockets

Bots that can fight back!

Partical effects (explosions)

Sounds (weapons, footsteps)

Spatial sound engine

Game scripting (lua?)

Docs to read
 - [ ] https://fabiensanglard.net/quake3/
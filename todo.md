
## Initial work
 - [x] Figure out how to call C++ from JS & visa versa

## Injesting a BSP
(http://www.mralligator.com/q3/, http://graphics.cs.brown.edu/games/quake/quake3.html)
 - [x] Load a .bsp into JS memory
 - [x] Load that .bsp into C++

## Getting a WebGL pipeline started:
 - [x] Load a GLSL file
 - [x] Get GLSL version 300 to load
 - [x] Try out GLFW
 - [x] Render a triangle
 - [x] Drawing polygons (open.gl)
 - [x] Textures (open.gl)

## Ramping up WebGL:
 - [x] Draw a cube w/ WebGL
 - [x] Capture mouse input
 - [x] Capture WASD
 - [x] Fly around the cube

## Generate C++ <=> TS bindings from a schema file
 - [x] Reorganize directory structure
 - [x] Setup Typescript
 - [x] Setup schema.json
 - [x] Setup generate_bindings.py
 - [x] Generate a ".h" file and a ".ts" definitions file

## Rendering faces
 - [x] Split up setup/rendering code into different scenarios (eg test scenario, bsp scenario)
 - [x] Figure out how to load faces + vertices + edges into OpenGL
 - [x] Render faces with random colors

## Rendering textures
 - [x] During resource loading, find out what textures to download
 - [x] Load textures in the ResourceManager
 - [x] Bind textures to the GPU
 - [x] Add texture coordinates to GLSL
 - [x] Render textures
 - [ ] Look into surface + content flags (???)

## Rendering lightmaps
 - [x] Load lightmaps in the ResourceManger
 - [x] Bind lightmap texture to the GPU
 - [x] Add lightmap coordinates to GLSL
 - [x] Render lightmaps

## Rendering patches
 - [x] Implement operator* & + for vertex_t
 - [x] For each face, loop over the contained patches
 - [x] For each patch, get the control points
 - [x] Tesselate the patch based on the control points
 - [x] Bind the tesselated vertices to the GPU
 - [x] Bind the tesselated lightmap/texture coordinates
 - [x] They should render!

# Make it possible to work on the rendering pipeline in TS (?)

C++ owns the state of the world. It loads that state into the GPU via VBOs/EBOs per-face.
Maybe it would work to pass those buffer uints into TS and then have TS own the entire
rendering pipeline -- this way the somewhat texture-shader pipeline can be written entirely
in TS.

For dynamic values, maybe C++ would still 
probably own setting dynamic state like camera-transform & (eventually) the transforms for
player models.

 - [ ] Pass GLuint buffers into TS?
 - [ ] 

## Rendering script textures (shaders)
 - [ ] Load the script definitions into textures_manifest.json
 - [ ] Load the script definition into TS
 - [ ] textures/proto2/beam_blue -- uses alpha & add blend function
 - [ ] data/textures/sfx/beam.jpg -- uses scroll mod
 - [ ] data/textures/skies/meth_clouds_blue.jpg -- uses scale & scroll mod

## Improvements to the ResourceManger
 - [ ] It should assign the resource IDs
 - [ ] Have multiple loading states

## Improvements to the development environment
 - [ ] Use Catch.h to run tests
 - [ ] Add an OSX compilation target
 - [ ] Add flags which can be controlled via checkboxes in index.html

## Find out what leaf we're in... render only visible leaves

## Render collision faces (for debugging)

## Player physics (ground + gravity, walking, jumping)

## Player physics (wall collisions)

## VQ3 physics (strafe jumping!)

## Rendering + lighting player model (3rd person)

## Animating player model

## Bots that can navigate

## Railgun

## Lightning gun

## Rockets

## Bots that can fight back!

## Partical effects (explosions)

## Sounds (weapons, footsteps)

## Spatial sound engine

## Game scripting (lua?)

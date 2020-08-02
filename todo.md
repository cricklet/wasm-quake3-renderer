
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

## Rendering script textures (shaders)
 - [x] Load the script definitions into textures_manifest.json
 - [x] Load the script definition into TS
 - [x] Add basic texture commands to schema
 - [x] Add an alpha setting to the texture commands!
 - [x] Pass texture commands from TS => C++ per texture
 - [x] Read texture commands when rendering faces, starting with alpha.
 - [x] Render transparency
   - [x] Extend render_scene shader to also allow taking in a depth-buffer
   - [x] Allow render_scene shader to blend
   - [x] Render translucent elements onto a separate buffer
   - [x] Composite the translucency buffer with the solid buffer
 - [ ] Learn more about the Quake shader pipeline? http://toolz.nexuizninjaz.com/shader/shader/section2.htm#2.1
 - [ ] Organize faces into which faces need to be drawn in which pass
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

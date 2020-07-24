
Fabien Sanglard docs
 - [x] Q1 Architecture https://fabiensanglard.net/quakeSource/index.php
       > After looking over the Q3 write-up, it's nice to see a really simple main loop! I'll
       > start here because I'm a bit intimidated by the more complex engines.
 - [x] Q1 Networking https://fabiensanglard.net/quakeSource/quakeSourceNetWork.php
       > The unit of communication is a "command" -- these update player position, orientation,
       > health, damage, etc. Many of these can be sent per UDP packet. Each packet includes a
       > sequence #. This is only incremented for reliable packets. The trick for getting
       > reliability is simply that only one reliable packet can be out at a time! The clients
       > must receive an ACK before sending the next reliable batch.
 - [ ] Q1 Prediction https://fabiensanglard.net/quakeSource/quakeSourcePrediction.php
 - [ ] Q1 Rendition https://fabiensanglard.net/quakeSource/quakeSourceRendition.php
 - [ ] Q3 Networking https://fabiensanglard.net/quake3/network.php
 - [ ] D3 DMap https://fabiensanglard.net/doom3/dmap.php
 - [ ] D3 Renderer https://fabiensanglard.net/doom3/renderer.php

Reflex
 - [ ] http://wiki.reflexfiles.com/index.php?title=Netcode

Transparency
 > Just thinking from first principles... I should be able to render the opaque scene first -- and
 > store both the z-index of each pixel and the RGBA. Then, I should be able to render the
 > translucent elements in the scene, blending the result with the original RGBA if the translucent
 > elements are in front of the opaque elemnets or ignoring them otherwise. This won't allow blending
 > multiple translucent elements at a time, but should work for individual translucent elements.
 - [ ] http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-10-transparency/
 - [ ] https://learnopengl.com/Advanced-OpenGL/Blending

Future things to read about OpenGL features
 - [ ] https://litasa.github.io/blog/2017/09/04/OpenGL-MultiDrawIndirect-with-Individual-Textures
 - [ ] https://learnopengl.com/Advanced-OpenGL/Instancing

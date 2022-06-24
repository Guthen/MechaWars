## How much time did I work on?
These are rough approximations since I'm not very organized and didn't take note of my work since the beginning
bef.: ~16h00
24/04/2022: ~2h00
25/04/2022: ~5h00
26/04/2022: ~4h30
19/05/2022: ~2h30

Finding a New Game Design:
24/05/2022: ~4h00 => redesign & mockup drawing
25/05/2022: ~5h00 => clean up, tile sprites import & procedural generation of maps (see `gifs/01_map_generation.gif`)
26/05/2022: ~1h30 => camera class managing zoom & movement w/ bounding (see `gifs/02_camera.gif`)
27/05/2022: ~1h30 => 'nexus' sprite import w/ teams color support
28/05/2022: ~0h30 => importing other buildings empty classes into the game
02/06/2022: ~1h00 => thinking of an algorithm to find valid Nexus positions
03/06/2022: ~2h00 => implementation of that said algorithm (see `gifs/03_forum_position.gif`)
04/06/2022: ~1h00 => code clean-up, shuffling Nexus start positions
07/06/2022: ~1h30 => UITileCursor fits structures sizes & smooth ice tiles in diagonals
08/06/2022: ~2h00 => UITileCursor managing Structure selection & UI Base & Button
21/06/2022: ~4h00 => Adding a GameScene & completing the Button & defered calls
22/06/2022: ~1h30 => Creating Buttons in structures w/ Icons & Grayscale Shader for disabled button

Total: ~55h30

## What I learned from this project?
### C++
+ `static` members (+`const float`) have to be initialized in the .cpp
+ functions declared in a header file must be inlined or implemented in a cpp file
+ use of `std::shared_ptr`/`std::unique_ptr` for handling value & allocation of a dynamic object
+ pointers have to be used to avoid object slicing (truncating a derived class to its parent class)
+ cyclic references: use forward declaration (+pointers) & include headers in .cpp's
+ basic differences between a `r-value` & a `l-value` 
+ basically no-differences between a `.h` & a `.hpp` header files
+ must return a pointer in order to return an array of values from a function
+ `#pragma region` similar to `#region` from C# (specific to Visual Studio though)
+ MACRO! awesome
+ *surely a lot of things I don't remember right now..*
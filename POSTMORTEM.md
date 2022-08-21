## How much time did I work on?
These are rough approximations since I'm not very organized and didn't take note of my work since the beginning
+ bef.: **~16h00**
+ 24/04/2022: **~2h00**
+ 25/04/2022: **~5h00**
+ 26/04/2022: **~4h30**
+ 19/05/2022: **~2h30**

Finding a New Game Design:
+ 24/05/2022: **~4h00** => redesign & mockup drawing
+ 25/05/2022: **~5h00** => clean up, tile sprites import & procedural generation of maps (see `gifs/01_map_generation.gif`)
+ 26/05/2022: **~1h30** => camera class managing zoom & movement w/ bounding (see `gifs/02_camera.gif`)
+ 27/05/2022: **~1h30** => 'nexus' sprite import w/ teams color support
+ 28/05/2022: **~0h30** => importing other buildings empty classes into the game
+ 02/06/2022: **~1h00** => thinking of an algorithm to find valid Nexus positions
+ 03/06/2022: **~2h00** => implementation of that said algorithm (see `gifs/03_forum_position.gif`)
+ 04/06/2022: **~1h00** => code clean-up, shuffling Nexus start positions
+ 07/06/2022: **~1h30** => UITileCursor fits structures sizes & smooth ice tiles in diagonals
+ 08/06/2022: **~2h00** => UITileCursor managing Structure selection & UI Base & Button
+ 21/06/2022: **~4h00** => Adding a GameScene & completing the Button & defered calls
+ 22/06/2022: **~1h30** => Creating Buttons in structures w/ Icons & Grayscale Shader for disabled button
+ 24/06/2022: **~1h30** => Bug fix w/ `AssetsManager::get_or_load_shader` & WorldEntity now inherited by Structures & (future) Units
+ 27/06/2022: **~1h00** => Rider & VK-2 Units code base w/ a Sprite Animator
+ 30/06/2022: **~5h00** => `Bullet`, Unit State Machine (Idle, Shoot), Timers, Debug Text & Clean Up
+ 01/07/2022:
	+ **~4h00** => Rewrite Entity references to use `std::weak_ptr` instead of raw pointers
	+ **~4h00** => Coding my own implementation of A* Pathfinding w/ debug rendering
+ 02/07/2022:
	+ **~3h00** => Fixing A* Implementation, batching Pathfinder rendering into a RenderTexture, add Debug States & Unit Movement
	+ **~0h30** => Using `dest` rectangle for custom render positions + GIF (see `gifs/04_astar_pathfinding.gif`)
	+ **0h10** => Use a Render Texture for Map rendering & move Map files in `game` folder
+ 03/07/2022:
	+ **~4h00** => Fixing my A* Implementation (again) resulting in weird paths (comparator issue)
+ 04/07/2022: 
	+ **~1h00** => Fix selection of Units + Pathfinding helper finding nearest end node if disabled + Change usage of pointers in `UnitState_Idle` & `UnitState_Shoot` to `std::weak_ptr`
	+ **~0h10** => Show debug messages in debug mode
	+ **~2h00** => Explosions on `Bullet` impact w/ blast effect
+ 11/07/2022:
	+ **~0h10** => Fix animator not playing in loop mode & Play Unit animation on move
	+ **~1h00** => Pixel Particles system & Bullet trail
+ 13/07/2022:
	+ **~0h30** => Add Unit Data
	+ **~1h00** => Add Burst & Spread to Shoot
+ 20/07/2022:
	+ **~0h30** => Add Health & Damage from explosions
+ ??/07/2022:
	+ **5h00** => Add a Unit State queue to Units + `UnitState_MoveTo` + `UnitState_Attack` + tried to fix Unit State pointer deletion
+ 31/07/2022:
	+ **~3h00** => Fix that godamn issue with the Unit State pointer deletion (why didn't I looked the compiler warnings earlier?)
	+ **~2h00** => Add `LSHIFT` key to queue State + Unit State fixes
+ 17/08/2022:
	+ **~1h00** => Add World Entity Health HUD
+ 20/08/2022:
	+ **~0h10** => Removing Melee Combat & Agro Range
	+ **~2h30** => Refactoring Shot Spread 
+ 21/08/2022:
	+**~0h05** => Add non-explosion Bullet Damage
	+**~0h30** => Fix bug preventing units from moving to a previously shot position 
	+**~0h05** => Remove `Unit::should_update_render_pos`
	+**~0h05** => Fix some compilation warnings

**Total: ~87h10**

## What I learned from this project?

### C++
+ `static` members (+`const float`) have to be initialized in the .cpp
+ functions declared in a header file must either be inlined or implemented in a cpp file
+ use of `std::shared_ptr`/`std::weak_ptr` for handling value & allocation of a dynamic object
+ pointers have to be used to avoid object slicing (truncating a derived class to its parent class)
+ cyclic references: use forward declaration (+pointers) & include headers in .cpp's
+ basic differences between a `r-value` (~values) & a `l-value` (~variables)
+ basically no-differences between a `.h` & a `.hpp` header files
+ must return a pointer in order to return an array of values from a function (or use containers from `std`)
+ declare a virtual destructor on the base class if you override the destructor (otherwise the destructor isn't called)
+ `#pragma region` similar to `#region` from C# (specific to Visual Studio though)
+ MACRO! awesome
+ you must have the class definition in order to call its destructor with `delete` 
+ *surely a lot of things I don't remember right now..*

### A*
Actually, the algorithm is simpler that I thought, so it was quite fun to implement ([.h](https://github.com/Guthen/MechaWars/blob/main/src/utility/astar2.h) & [.cpp](https://github.com/Guthen/MechaWars/blob/main/src/utility/astar2.cpp)). 
Got a hard time to iron out a bug with the heap sorting but eventually got it to work.

![First Day: implementation](https://github.com/Guthen/MechaWars/blob/main/gifs/04_astar_pathfinding.gif?raw=true)
![Second Day: fix](https://github.com/Guthen/MechaWars/blob/main/gifs/04_astar_pathfinding_fixed.gif?raw=true)
![Second Day: visualizer](https://github.com/Guthen/MechaWars/blob/main/gifs/04_astar_pathfinding_visualizer.gif?raw=true)
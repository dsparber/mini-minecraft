# Mini Minecraft: ADVenture
Amelia Peng, Daniel Sparber, Veronica Bristol

## Milestone 1

### Videos

- [Game Engine Update Function and Player Physics](https://youtu.be/PDtSNNaBpVM)
- [Terrain](https://youtu.be/z__P_hIH8nM)
- [Chunking and Merged demo](https://vimeo.com/373853823)

### Procedural Terrain

Implemented by: **Veronica**

#### Implementation

- *Initial Terrain Generation:* To implement the terrain using fractal brownian motion, I used the fbm functions given in lecture notes. I manipulated the output of the fbm function by raising it to the 4th power and multiplying by 85. This gave me some nice mountains with a maximum height of 85. I then added 128 to my value to move the terrain above the 128 blocks of stone. To align my code with chunking, I then rewrote the for loops in CreateTestScene (which we have renamed to create()) to loop through each chunk in the chunk map and then loop through each block within each chunk. 

- *Delete & Add Block:* To implement delete and add block I used ray-marching to find the closest block to delete or add next to. At first I struggled with this section because I didn't understand the difference between ray-marching and ray cube intersection, but once I understood the difference between the two methods and how to implement each ray-marching was easy to implement. Ultimately, I chose to use ray-marching instead of ray cube intersection because I understand the concept better. I still don't fully understand ray cube intersection, so I thought it would be better to implement the method that I had a complete understanding of rather than use the given raycube intersection code that I don't understand. I also added a new BlockType called LAVA in order to test my add block function. I set the maxLength for adding and deleting blocks to be 8, so that you must be within 8 blocks to add or delete a block. 

- *Generating New Terrain:* To implement new terrain generation I created a new function that is called from the player class and consistently checks in every direction (8 directions) if there is terrain in that direction. If terrain does not exist in a direction then I generate new terrain. Each time new terrain is generated, I create 16 new chunks who's positions align with the initial terrain. Initially I had trouble understanding this section conceptually and visualizing how I would make the terrain continuous, but after help in office hours and talking to other students I got a better understanding. Unfortunately, the terrain generation relies on chunking, so I'm unable to display it in my video since our chunking does not completely work.



### Efficient Terrain Rendering and Chunking

Implemented by: **Amelia**

#### Implementation
- *Chunk class*: Added a chunk class so the terrain could store chunks instead of 3D array of BlockTypes to make rendering more efficient. Uses vectors of global variables to be the offset to handle chunk's creation and neighbor checking, only draws the face if the chunk doesn't have a neighbor or the neighboring block has EMPTY as blocktype.  

- *Chunk Map*: create a map of (x,z) coordinate to Chunk in terrain.cpp to store chunk's wold position to chunk. Implemented hash functions using bit logic to convert (x,z) coordinate to 64bit integer keys.   

- *interleaving VBOs*: modified the Drawable class and shaderProgram::draw in order to use a single interleaved vertex buffer object to store the position, normal, and color data of an object.

#### Difficulties
- there were a lot of repeating code when implementing create() in Chunk class. Declaring global vectors of position offsets and looping the face helped simplified the code.
- After a block is set to a blocktype, it results to be EMPTY when calling getBlockAt function. I switch from storing the physical block in the map to using pointer. 
- different behavior when calling create() in initialize GL and paintGL. If create() is only called in initialize GL, part of the terrain won't be drawed correctly, but calling create() in paintGL each time would be too inefficient. 
- After merging, the trackpad on my computer is very hard to make the "player" view the direction desired.

### Game Engine Update Function and Player Physics

Implemented by: **Daniel**

#### Implementation

- *Normal player movement:* I used an acceleration based approach to move the player around. 
First I read the user input, then I transform the input to an acceleration. 
From here on, I used classic physics to calculate the travel distance `ds`. 
`ds` is limited by the collision distance. 
The position gets updated by `ds`.
The advantage of the acceleration approach is realism. 
For instance, the x-z-velocity can not be changed when airborne. 
When a player runs and jumps, the x-y-velocity is maintained throughout the jump (no air resistance implemented).


- *Jumping:* I defined a `jumpHeigh`. Based on the height I calculate the required takeoff velocity.


- *Flight mode:* I get the desired direction from the user input and move the player according to `flightVelocity`. 
No collision dedection is used.

- *Ground check:* Based on the players bounding box, I test if there is a solid block underneath any of the players vertices.

- *Collision dedection:* I used the algorithm presented in the recitation. 
I.e. starting at a point `p` I get the closest integer intersection and test for a solid block. 
If there is no solid block, I set `p` to the intersection position and repeat. 
For performance reasons, I only test up to the length of the distance vector.

#### Difficulties

The only challenge was dealing with floating point precission. If the player moves a tiny bit to far, he ends up within a block and gets stuck.


## Milestone 2

### Video

### L-System Rivers

Implemented by: **Veronica**

#### Implementation

- *River Creation:* Implemented a Turtle and River class. The Turtle class stores a position and orientation to be used when constructing rivers. The River class has two functions to create two rivers that are placed in the initial terrain (river1 = straight river, river2 = delta river), as well as two maps for string expansion and string drawing. The first map maps chars to strings and the second maps chars to function pointers. I then implemented all of the functions needed to draw my L-System string. My main river body has a large radius, and I then decreased this value by changing the radius when a position was saved and stored. 

- *Random Numbers:* I created different rotation functions for each river so that I could have one river have slightly larger angles than the other. I incorporated randomization to influence path orientation by randomly generating the angle for each rotation within a given range. I incorporated randomization to influence the probability of branch generation by deciding whether or not to rotate based on a random number generation, thus a new branch is not always generated.

- *Terrain Carving:* I implemented terrain carving by looping through each direction for a given position in my river and carving the terrain around the river at that point by gradually increasing the given height.


#### Difficulties

- I struggled a lot with getting my initial river to appear. My L-System grammar was correct, but I had a hard time getting my moveAndDrawLine() function to work properly. At first I tried implementing it using Bresenham's line formula, but I ran into a lot of issues with this implementation and switched to a simpler method that mimicked ray-marching.
- I also struggled with terrain carving at first trying to implement it using the perpendicular and ray-marching; however, when I also simplified my implementation for this function it became much easier to implement and test.



### Texturing and Texture Animation

Implemented by: **Amelia**

#### Implementation
-*load images into OpenGL:* Implemented a texture class that alows texture image of blocktypes to be loaded into our game

-*sample textures:* Made use of sampler2D to apply texture colors to a surface in lambert.frag.glsl

-*specular reflection:* Applied a Blinn-Phong specular reflection model in the lamber shader, with a cosine power that varies accross different blocks

-*animatable blocks:* uses a varaible to animate the UVs on LAVA and WATER block.

-*alpha bending:* enables alpha bending for non-opaque blocks

-*vbo alteration:* add more handles in the vbo that supports U coordinates, cosine power, and animateable flag of each block.


#### Difficulties

- making the textures show up was very challenging since bugs could appear in every stage such as doing the right UV coordinate offset in VBOs, loading texture files the right way, and initializing the textures in the right place. Since the compiler can't tell me which step is wrong, when the texture is not showing up, I have to make use of debugger and print statement to spot the problem stage by stage. 
-  There are extra triangles showing up for transparent blocks in specific angles, but when I run the debugger with simple test terrains, it shows that the correct number of faces are drawn. It turns out that it's something with the transparency sorting appears to be fine when there are blocks around the boundary of the water. 



### Swimming and Multithreaded Terrain Generation

Implemented by: **Daniel**


#### Implementation

- *Swimming physics:* If the player is "standing" on water or on lava, the physics function considers this as swimming. I modified the physics such that there is no collision for liquids (water, lava) and the accelerations and max. velocity are scaled down by 2/3.

- *Swimming Overlays:* I reused the HW5 code to enable post shaders. I implemented 3 shaders (no effext, water, lava) to add the desired color overlay.

- *Multithreading:* I introduced a variable that determines how many chunks around the player should be drawn. If the player moves, the program checks if all required chunks are available. The required chunks are stored in a vector that the MyGL class uses. The function also creates and destroys chunks depending on whether they are needed. If a required chunk is not available, a new task is submitted to the threadpool to create the missing chunk.
The task first computes the blocks within the chunk. Then it checks if any neighbors exist and links them. To reduce the workload of the main thread when calling `create()`, the parallel task computes the `idx` and `all` vectors that are later passed to the GPU. If a neighbor needs to be recomputed, the task also calls the `compute` function. To avoid race conditions when computing a chunk, each chunk is locked by a mutex associated with the chunk.
After a chunk was created it is inserted into a vector which the main thread later reads to insert the chunks into the chunk map. This process is also secured by a mutex.


#### Difficulties

- None

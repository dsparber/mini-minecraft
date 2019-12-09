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

- [Milestone 2 Video](https://youtu.be/wEmui2oN0zU)
- [Water and Lava Animation](https://vimeo.com/376899860)

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


## Milestone 3

### Amelia
#### Implementation
- *Day and night cycle:* implemented a procedural sky background using the ray cast method. The sun's position and sky color is changed smoothly overtime using time variable in sky shader. The sky goes from sun rise to sun set to night with stary sky implemented using noise functions, where very light color(as the stars) is only drawn when it's above a certain threshold from the output of worley noise. The color of the terrain is also changed over time relative to the sky color by modifying the lambert shader using mix function.  
- *Post process shaders:* modified from the pipline seted up from the last milestone, fixed frame buffers, and implemented another set of water and lava shader as well as a set of costume shaders using worley noise and random functions. Implement a set of key press behaviors, so when the player presses number 1~9, different shaders could be viewed as follows:
    - 1: underwater (by Daniel)
    - 2: underlava (by Daniel)
    - 3: underwater
    - 4: underlava 
    - 5: horror movie
    - 6: toonify (modified from shader library*)
    - 7: hatchings (modified from shader library*)
    - 8: old movie (modified from shader toy*)
    - 9: kaledoscope
    - 0: orginal
#### Difficulties
- implementing sky shader: I struggled a lot implementing the sky shader into exisiting code. At first the sky was not showing up at all, then I experimented with a lot of different shaders, frame buffers, and changing the sky shader to a single color. It began to show up when I comment out some code, but is gone again after I run it with the same code, which makes it even harder to debug. It was finally solved by drawing it after the frame buffer is bindede using the same quad as the post process shaders and setting it to "transparent" when drawing the block for the purpose of vbo ordering.  
- sun's transition and color mixing: I tried many thing to make the sun transition smoothy, which does not work with using mix function with three hardcoded directions. I ended up calculate functions that changes the sun direction's xyz sepereatly, each in a different relation to u_Time. 
- timing of the day and night cycle: I experimented a lot to get the cycle right and make sure the sky don't go from night to day again when the sun still haven't "reset". I ended up keeping a variable that keep track of the global weight relatively to time, and in each mix function I re-calculate the weight if needed for different purposes(changing day to night, start shining, sun rising) . 
- post process shader screen ratio: There was a blue bar on top of my scene but not on my group partners, and I found it's gone when I commented post process shaders out. I debug from there and fixed it by resetting the framebuffer each time when resizeGL() is called. 
- ways to fit more shaders: Daniel and I accidentally did post process shader at the same time and we ended up using his for under water and lava effects. To save my work from being discarded, I set up a current shader pipeline and certain post process shader is used to draw all blocks when certain key is pressed. The water and lava shader I made can be seen by pressing 3 and 4, and I experimented and added a lot more shaders using noises and online resources.
- tested the game for too long when I went back to coding I felt that the screen is rotating XD

*Shader library: https://www.geeks3d.com/shader-library/
*Shader Toy Old Movie Effect: https://www.shadertoy.com/view/4dKfDm

### Daniel

#### Video 

[Video](https://youtu.be/Pr75L229qrg)

#### Implementation

- *Fog:* The vertex shader glsl calculates the alpha value based on the z distance. The fragment shader applies the alpha value to the sampled color.

- *Post process shaders:* The pipline was taken from homework 5. The frame buffer is used with a new texture slot (different from normal texture slot). The water and lava shader both use worley noise and a time unform. The noise for lava is more vivid and more fine grained than for water. The colors are also modified. 

- *Random walks / caves:* I defined a new class for a random walk state. The state has a step() and a next() method. The step method executes an arbitrary operation, the next method returns the next state. The driver class (RandomWalk) calls the step and next method as long as the next state is not null. The random walk is executed as a runnable on the thread pool. For this assignment I implemented two concrete random walk states: tunnel and cave. The tunnel step hollows out some terrain around the current position and moves into a random direction. To span side tunnels, the tunnel state randomly spans a new random walk from time to time. The tunnel has a time to live value. As long as this value is greater 0, the next state is also tunnel. Otherwise, the next state is cave. The cave hollows out a larger volume and fills the lower third with lava. The cave has no next state. 

#### Difficulties

- None


### Veronice

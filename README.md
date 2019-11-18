# Mini Minecraft: ADVenture
Amelia Peng, Daniel Sparber, Veronica Bristol


## Milestone 1

### Videos

- [Game Engine Update Function and Player Physics](https://youtu.be/PDtSNNaBpVM)
- [Terrain](https://youtu.be/z__P_hIH8nM)

### Procedural Terrain

Implemented by: **Veronica**

#### Implementation

- *Initial Terrain Generation:* To implement the terrain using fractal brownian motion, I used the fbm functions given in lecture notes. I manipulated the output of the fbm function by raising it to the 4th power and multiplying by 85. This gave me some nice mountains with a maximum height of 85. I then added 128 to my value to move the terrain above the 128 blocks of stone. To align my code with chunking, I then rewrote the for loops in CreateTestScene (which we have renamed to create()) to loop through each chunk in the chunk map and then loop through each block within each chunk. 

- *Delete & Add Block:* To implement delete and add block I used ray-marching to find the closest block to delete or add next to. At first I struggled with this section because I didn't understand the difference between ray-marching and ray cube intersection, but once I understood the difference between the two methods and how to implement each ray-marching was easy to implement. Ultimately, I chose to use ray-marching instead of ray cube intersection because I understand the concept better. I still don't fully understand ray cube intersection, so I thought it would be better to implement the method that I had a complete understanding of rather than use the given raycube intersection code that I don't understand. I also added a new BlockType called LAVA in order to test my add block function. I set the maxLength for adding and deleting blocks to be 8, so that you must be within 8 blocks to add or delete a block. 

- *Generating New Terrain:* To implement new terrain generation I created a new function that is called from the player class and consistently checks in every direction (8 directions) if there is terrain in that direction. If terrain does not exist in a direction then I generate new terrain. Each time new terrain is generated, I create 16 new chunks who's positions align with the initial terrain. Initially I had trouble understanding this section conceptually and visualizing how I would make the terrain continuous, but after help in office hours and talking to other students I got a better understanding. Unfortunately, the terrain generation relies on chunking, so I'm unable to display it in my video since our chunking does not completely work.



### Efficient Terrain Rendering and Chunking

Implemented by: **Amelia**

#### Implementation

**TODO:** briefly describe how you implemented your chosen features. 
Discuss any difficulties you encountered when coding the project, and explain the approach you 
took to implementing each feature



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

# Mini Minecraft: ADVenture
Amelia Peng, Daniel Sparber, Veronica Bristol


## Milestone 1

### Videos

- [Game Engine Update Function and Player Physics](https://youtu.be/PDtSNNaBpVM)

### Procedural Terrain

Implemented by: **Veronica**

#### Implementation

**TODO:** briefly describe how you implemented your chosen features. 
Discuss any difficulties you encountered when coding the project, and explain the approach you 
took to implementing each feature



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

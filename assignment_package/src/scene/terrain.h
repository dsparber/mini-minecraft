#pragma once
#include <QList>
#include <la.h>
#include <unordered_map>
#include <stdint.h>
#include "chunk.h"
#include "blocktype.h"
#include <smartpointerhelp.h>

class Terrain
{

private:
    /// A map of chunk's coordinate coded in 64 bits as key to the chunk
    std::unordered_map<int64_t, Chunk*> chunkMap;

    /// Open GL context. Used for chunk creation
    OpenGLContext* context;

    /// Chunks that should be drawn
    std::vector<Chunk*> chunksToDraw;

    /// Return the coordinates for the block the player is looking at
    glm::vec3 rayMarch(glm::vec3 eye, glm::vec3 look);

    /// Creates a chunk at the given position and generates the terrain
    void initializeChunk(int x, int z);

    /// Returns the chunk at the given coordinates
    Chunk* getChunk(int x, int z) const;

    /// Helper functions for converting x,z to int64
    int64_t getHashKey(int x, int z) const;
    glm::vec2 getCoordFromKey(int64_t key) const;

public:

    Terrain(OpenGLContext* context);
    ~Terrain();

    /// Generates the initial terrain, sets up the multithreading tasks
    void initialize();


    /// Returns the block at the given coordinates. Returns EMPTY if out of bounds
    BlockType getBlockOrEmpty(int x, int y, int z) const;

    /// Returns a reference to a block at the given coordinates
    BlockType& getBlockAt(int x, int y, int z) ;

    /// Given a world-space coordinate (which may have negative
    /// values) return the block stored at that point in space.
    BlockType getBlockAt(glm::vec3) const;

    /// Given a world-space coordinate (which may have negative
    /// values) return the block stored at that point in space.
    BlockType getBlockAt(int x, int y, int z) const;

    /// Given a world-space coordinate (which may have negative
    /// values) set the block at that point in space to the given type.
    void setBlockAt(int x, int y, int z, BlockType t);


    /// Adds a block at the center of the screen, updates the terrain
    void addBlock(glm::vec3 eye, glm::vec3 look);

    /// Removes a block at the center of the screen, updates the terrain
    void removeBlock(glm::vec3 eye, glm::vec3 look);


    /// Returns all chunks that should be drawn.
    std::vector<Chunk*> getChunksToDraw() const;


    /// Callback if player moved. Used to expand terrain
    void playerMoved(glm::vec3);
};

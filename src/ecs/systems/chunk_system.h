#pragma once

#include <cstdlib>
#include <ctime>
#include <map>
#include <tuple>

#include "ecs.h"

#include "ecs/components/basic_models.h"
#include "ecs/components/render_type_components.h"
#include "ecs/components/renderable_component.h"
#include "utils/log.h"

#define CHUNK_SIZE 16
#define CHUNK_DEPTH 1

struct Block {
    int type {-1};
    glm::vec3 size {1,1,1};
    ECS::Entity* block {nullptr};
};

struct Chunk {
    Chunk(std::tuple<int,int,int> offset) : offset(offset) {}
    std::tuple<int,int,int> offset;
    Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
};

class ChunkSystem {
public:
    ChunkSystem(ECS::World* world) : m_world(world) {}

    void generate_chunk(int xVal, int zVal) {
        if(get_chunk(xVal, CHUNK_DEPTH, zVal) != nullptr) ME::Log<ME::FATAL>() << "Chunk already exist";

        for(int d=0; d<CHUNK_DEPTH; d++) {
            auto chunkId = std::tie(xVal, d, zVal);
            auto chunk = m_chunks[chunkId] = m_world->create();

            Chunk chunkData(chunkId);
            for(int x = 0; x < CHUNK_SIZE; x+=1){ for(int y = 0; y < CHUNK_SIZE; y+=1){ for(int z = 0; z < CHUNK_SIZE; z+=1){
                chunkData.blocks[x][y][z] = {y == CHUNK_SIZE - 1 ? 2 : 1, glm::vec3(1,1,1), nullptr};
            }}}
            chunk->assign<Chunk>(chunkData);

            resolve_chunk(chunk);
            buffer_chunk(chunk);
        }
    }

    ECS::Entity* get_chunk(int x, int y, int z) {
        auto chunkId = std::tie(x, y, z);
        return m_chunks.find(chunkId) != m_chunks.end() ? m_chunks[chunkId] : nullptr;
    }

    void resolve_chunk(ECS::Entity* chunk) {
        auto& chunkData = chunk->get<Chunk>().get();
        auto& blocks = chunkData.blocks;
        
        for(int z=0; z < CHUNK_SIZE; z++) {
            for(int y = 0; y < CHUNK_SIZE; y++){ for(int x = 0; x < CHUNK_SIZE; x++){ 
                if(!does_block_exist(chunk, x, y, z)) continue;
                int xx = 0;
                for(xx = x + 1; xx < CHUNK_SIZE; xx+=1) {
                    if(blocks[xx][y][z].size == glm::vec3() || blocks[x][y][z].type != blocks[xx][y][z].type || blocks[xx][y][z].size.y != blocks[x][y][z].size.y) break;
                    blocks[xx][y][z].size = glm::vec3(0);
                    blocks[x][y][z].size.x += 1;
                }
                x += xx;
            }}

            for(int x = 0; x < CHUNK_SIZE; x++){ for(int y = 0; y < CHUNK_SIZE; y++){
                if(!does_block_exist(chunk, x, y, z)) continue;
                int yy = 0;
                for(yy = y + 1; yy < CHUNK_SIZE; yy++) {
                    if(blocks[x][yy][z].size == glm::vec3() || blocks[x][y][z].type != blocks[x][yy][z].type || blocks[x][yy][z].size.x != blocks[x][y][z].size.x) break;
                    blocks[x][yy][z].size = glm::vec3(0);
                    blocks[x][y][z].size.y += 1;
                }
                y += yy;
            }}
        }

        for(int y=0; y < CHUNK_SIZE; y++) {
            for(int x = 0; x < CHUNK_SIZE; x++){ for(int z = 0; z < CHUNK_SIZE; z++){
                if(!does_block_exist(chunk, x, y, z)) continue;
                int zz = 0;
                for(zz = z + 1; zz < CHUNK_SIZE; zz++) {
                    if(blocks[x][y][zz].size == glm::vec3() || blocks[x][y][z].type != blocks[x][y][zz].type || blocks[x][y][zz].size.x != blocks[x][y][z].size.x ) break;
                    blocks[x][y][zz].size = glm::vec3(0);
                    blocks[x][y][z].size.z += 1;
                }
                z += zz;
            }}
        }
    }

    bool is_ok(glm::vec3 s) {
        return s.x > 0 && s.y > 0 || s.x > 0 && s.z > 0 || s.z > 0 && s.y > 0;
    }

    void buffer_chunk(ECS::Entity* chunk) {
        auto& chunkData = chunk->get<Chunk>().get();
        auto& blocks = chunkData.blocks;

        for(int x=0; x<CHUNK_SIZE; x++) { for(int y=0; y<CHUNK_SIZE; y++) { for(int z=0; z<CHUNK_SIZE; z++) {
            auto& chunkSize = chunk->get<Chunk>().get().blocks[x][y][z].size;
            if(does_block_exist(chunk, x, y, z) && chunkSize != glm::vec3(0)) {

                int hiddenSides = 0;
                if(does_block_exist(chunk, x, y, z+chunkSize.z)) set_bit(hiddenSides, FRONT);
                if(does_block_exist(chunk, x, y, z-1)) set_bit(hiddenSides, BACK);

                if(does_block_exist(chunk, x-1, y, z)) set_bit(hiddenSides, LEFT);
                if(does_block_exist(chunk, x+chunkSize.x, y, z)) set_bit(hiddenSides, RIGHT);

                if(does_block_exist(chunk, x, y+chunkSize.y, z)) set_bit(hiddenSides, TOP);
                if(does_block_exist(chunk, x, y-1, z)) set_bit(hiddenSides, BOTTOM);

                auto [ offsetX, offsetY, offsetZ ] = chunk->get<Chunk>().get().offset;

                auto entity = create_cube(m_world, "dirt_side.jpg", glm::vec3(x+offsetX*CHUNK_SIZE, y+offsetY*CHUNK_SIZE, z+offsetZ*CHUNK_SIZE), hiddenSides, chunk->get<Chunk>().get().blocks[x][y][z].size);
                entity->assign<BatchRenderComponent>( 0 );
                entity->assign<VisibleComponent>();
                if(blocks[x][y][z].block) m_world->destroy(blocks[x][y][z].block);
                blocks[x][y][z].block = entity;
            }
        }}}

    }

    bool is_block_obscured(ECS::Entity* chunk, int x, int y, int z) {
        auto top = does_block_exist(chunk, x, y+1, z);
        auto bottom = does_block_exist(chunk, x, y-1, z);
        auto front = does_block_exist(chunk, x, y, z-1);
        auto back = does_block_exist(chunk, x, y, z+1);
        auto left = does_block_exist(chunk, x-1, y, z);
        auto right = does_block_exist(chunk, x+1, y, z);
        return top && bottom && front && back && left && right;
    }

    bool does_block_exist(ECS::Entity* chunk, int x, int y, int z) {
        return is_block_valid(x,y,z) && chunk->get<Chunk>().get().blocks[x][y][z].type != -1;
    }

    bool is_block_valid(int x, int y, int z) {
        return x >= 0 && x < CHUNK_SIZE 
            && y >= 0 && y < CHUNK_SIZE
            && z >= 0 && z < CHUNK_SIZE;
    }

    int get_block_type(ECS::Entity* chunk, int x, int y, int z) {
        return is_block_valid(x,y,z) ? chunk->get<Chunk>().get().blocks[x][y][z].type : -1;
    }

private:
    ECS::World* m_world;
    std::map<std::tuple<int, int, int>, ECS::Entity*> m_chunks;
};

//OLD CODE
void generate_chunk(ECS::World* world) {
        //INITIALIZE
        srand (time(NULL));
        int chunk[16][16];
        for(int x = 0; x < 16; x+=1){ for(int y = 0; y < 16; y+=1){
            chunk[x][y] = rand() % 2 + 1;
        }}

        auto isTouching = [&](int x, int y, int type) -> bool {
            if( x < 0 || y < 0 || x > 15 || y > 15) return false;
            return chunk[x][y] == type;
        };

        //RESOLVE
        int resolved_chunk_x[16][16];
        int resolved_chunk_y[16][16];
        for(int x = 0; x < 16; x+=1){ for(int y = 0; y < 16; y+=1){
            resolved_chunk_x[x][y] = resolved_chunk_y[x][y] = chunk[x][y] != 0 ? 1 : 0;
        }}


        for(int y = 0; y < 16; y+=1){ for(int x = 0; x < 16; x+=1){
            if(chunk[x][y] == 0) continue;
            for(int xx = x + 1; xx < 16; xx+=1) {
                if(!isTouching(xx, y, chunk[x][y])) break;
                resolved_chunk_x[xx][y] = 0;
                resolved_chunk_x[x][y] += 1;
            }
            x += resolved_chunk_x[x][y] - 1;
        }}


        for(int x = 0; x < 16; x+=1){ for(int y = 0; y < 16; y+=1){
            if(chunk[x][y] == 0) continue;
            for(int yy = y + 1; yy < 16; yy+=1) {
                if(!isTouching(x, yy, chunk[x][y])) break;
                if(resolved_chunk_x[x][y] != resolved_chunk_x[x][yy]) break;
                resolved_chunk_y[x][yy] = 0;
                resolved_chunk_y[x][y] += 1;
            }
            y += resolved_chunk_y[x][y] - 1;
        }}

        //BUILD MESH
        for(int y = 0; y < 16; y+=1){ for(int x = 0; x < 16; x+=1){
            if(resolved_chunk_x[x][y] == 0 && resolved_chunk_y[x][y] == 0) continue;
            
            auto entity = world->create();
            auto raw = create_square( chunk[x][y] == 1 ? "dirt_side.jpg" : "", glm::mat4(1.f) * glm::translate(glm::vec3(x, y, 0)) * glm::scale(glm::vec3(resolved_chunk_x[x][y], resolved_chunk_y[x][y], 1)), resolved_chunk_x[x][y], resolved_chunk_y[x][y]);
            entity->assign<VerticesComponent>(raw.vertices);
            entity->assign<IndicesComponent>(raw.indices);
            entity->assign<RenderComponent>();
        }}
}
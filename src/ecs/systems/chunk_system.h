#pragma once

#include <cstdlib>
#include <ctime>
#include <deque>
#include <map>
#include <tuple>

#include "ecs.h"

#include "ecs/components/basic_models.h"
#include "ecs/components/render_type_components.h"
#include "ecs/components/renderable_component.h"
#include "utils/log.h"
#include "utils/timer.h"
#include "utils/perlin_noise.h"
#include "camera/camera.h"

#define CHUNK_SIZE 8
#define CHUNK_DEPTH 1

struct Block
{
    int type{-1};
    glm::vec3 size{1, 1, 1};
    ECS::Entity *block{nullptr};
};

struct Chunk
{
    Chunk(std::tuple<int, int, int> offset, int batch) : offset(offset), batch_id(batch) {}
    std::tuple<int, int, int> offset;
    Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    int batch_id;
};

class ChunkSystem : public ECS::EntitySystem
{
public:
	ChunkSystem(ECS::World* world, BatchRenderSystem* batchSystem) : m_world(world), m_batchSystem(batchSystem), m_noise(237) { m_timer.reset(); }
    PerlinNoise m_noise;
	ME::Timer m_timer;
	bool generated_Chunks = false;
    void tick(ECS::World *world, float delta) override
    {
		//if (m_timer.get() < 100)
		////{
		//	return;
		//}
		//m_timer.reset();

		if (generated_Chunks) return;

        auto cameraPosition = ME::Camera::Camera3D::instance().GetPosition();
        int cameraX = (cameraPosition.x + 8) / CHUNK_SIZE;
        int cameraZ = (cameraPosition.z + 8) / CHUNK_SIZE;

        float xMul = 4. * ME::Camera::Camera3D::instance().m_cameraDirection.x;
        float zMul = 4. * ME::Camera::Camera3D::instance().m_cameraDirection.z;

        int xMin = xMul > 0 ? -xMul : xMul;
        int xMax = xMul > 0 ? xMul : -xMul;

        int zMin = zMul > 0 ? -zMul : zMul;
        int zMax = zMul > 0 ? zMul : -zMul;

        int N = 24*24;
        int x = 0;
        int y = 0;
        for (int i = 0; i < N; ++i)
        {
			int trueX = x;// +cameraX;
			int trueZ = y;// +cameraZ;
            if (!get_chunk(trueX, 0, trueZ))
            {
                generate_chunk(trueX, trueZ, m_batchSystem->get_batch({trueX * CHUNK_SIZE, 0, trueZ * CHUNK_SIZE}));
            }

            if (abs(x) <= abs(y) && (x != y || x >= 0))
                x += ((y >= 0) ? 1 : -1);
            else
                y += ((x >= 0) ? -1 : 1);
		}
		generated_Chunks = true;
	}

    void generate_chunk(int xVal, int zVal, int batchId)
    {
        if (get_chunk(xVal, CHUNK_DEPTH, zVal) != nullptr) ME::Log<ME::FATAL>() << "Chunk already exist";
        for (int d = 0; d < CHUNK_DEPTH; d++)
        {
            auto chunkId = std::tie(xVal, d, zVal);
            auto chunk = m_chunks[chunkId] = m_world->create();

            Chunk chunkData(chunkId, batchId);
            for (int x = 0; x < CHUNK_SIZE; x += 1)
            {
                for (int z = 0; z < CHUNK_SIZE; z += 1)
                {
                    int maxY = m_noise.noise(.05 * (xVal * CHUNK_SIZE + x), .05 * (zVal * CHUNK_SIZE + z), .2) * CHUNK_SIZE + 1;
                    for (int y = 0; y < CHUNK_SIZE; y += 1)
                    {
                        int type = y <= maxY ? 1 : -1;
                        chunkData.blocks[x][y][z] = {type, glm::vec3(1, 1, 1), nullptr};
                    }
                }
            }
            chunk->assign<Chunk>(chunkData);

            resolve_chunk(chunk);
            buffer_chunk(chunk);
        }
    }

    ECS::Entity *get_chunk(int x, int y, int z)
    {
        auto chunkId = std::tie(x, y, z);
        return m_chunks.find(chunkId) != m_chunks.end() ? m_chunks[chunkId] : nullptr;
    }

    void resolve_chunk(ECS::Entity *chunk)
    {
        auto &chunkData = chunk->get<Chunk>().get();
        auto &blocks = chunkData.blocks;

        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                for (int x = 0; x < CHUNK_SIZE; x++)
                {
                    if (!does_block_exist(chunk, x, y, z))
                        continue;
                    int xx = 0;
                    for (xx = x + 1; xx < CHUNK_SIZE; xx += 1)
                    {
                        if (blocks[xx][y][z].size == glm::vec3() || blocks[x][y][z].type != blocks[xx][y][z].type || blocks[xx][y][z].size.z != blocks[x][y][z].size.z)
                            break;
                        blocks[xx][y][z].size = glm::vec3(0);
                        blocks[x][y][z].size.x += 1;
                    }
                    x += xx;
                }
            }

            for (int x = 0; x < CHUNK_SIZE; x++)
            {
                for (int z = CHUNK_SIZE - 1; z >= 0; z--)
                {
                    if (!does_block_exist(chunk, x, y, z))
                        continue;
                    int zz = 0;
                    for (zz = z - 1; zz >= 0; zz--)
                    {
                        if (blocks[x][y][zz].size == glm::vec3() || blocks[x][y][z].type != blocks[x][y][zz].type || blocks[x][y][zz].size.x != blocks[x][y][z].size.x)
                            break;
                        blocks[x][y][zz].size = glm::vec3(0);
                        blocks[x][y][z].size.z += 1.f;
                    }
                    z = zz;
                }
            }
        }

        for (int z = 0; z < CHUNK_SIZE; z++)
        {
            for (int x = 0; x < CHUNK_SIZE; x++)
            {
                for (int y = 0; y < CHUNK_SIZE; y++)
                {
                    if (!does_block_exist(chunk, x, y, z))
                        continue;

                    auto &thisBlock = blocks[x][y][z];

                    int yy = 0;
                    for (yy = y + 1; yy < CHUNK_SIZE; yy++)
                    {
                        auto &curBlock = blocks[x][yy][z];
                        if (blocks[x][yy][z].size == glm::vec3() || thisBlock.type != curBlock.type || thisBlock.size.x != curBlock.size.x || thisBlock.size.z != curBlock.size.z)
                            break;
                        curBlock.size = glm::vec3(0);
                        thisBlock.size.y += 1;
                    }
                    y += yy;
                }
            }
        }
    }

    void buffer_chunk(ECS::Entity *chunk)
    {
        auto &chunkData = chunk->get<Chunk>().get();
        auto &blocks = chunkData.blocks;

        for (int x = 0; x < CHUNK_SIZE; x++)
        {
            for (int y = 0; y < CHUNK_SIZE; y++)
            {
                for (int z = 0; z < CHUNK_SIZE; z++)
                {
                    auto &chunkSize = chunk->get<Chunk>().get().blocks[x][y][z].size;
                    if (does_block_exist(chunk, x, y, z) && chunkSize != glm::vec3(0))
                    {
                        auto [offsetX, offsetY, offsetZ] = chunk->get<Chunk>().get().offset;

                        auto entity = create_cube(m_world, "dirt_side.jpg", glm::vec3(x + offsetX * CHUNK_SIZE, y + offsetY * CHUNK_SIZE, z + offsetZ * CHUNK_SIZE), 0, chunk->get<Chunk>().get().blocks[x][y][z].size);
                        entity->assign<BatchRenderComponent>(chunk->get<Chunk>().get().batch_id);
                        entity->assign<VisibleComponent>();
                        if (blocks[x][y][z].block)
                            m_world->destroy(blocks[x][y][z].block);
                        blocks[x][y][z].block = entity;
                    }
                }
            }
        }
    }

    bool is_block_obscured(ECS::Entity *chunk, int x, int y, int z)
    {
        auto top = does_block_exist(chunk, x, y + 1, z);
        auto bottom = does_block_exist(chunk, x, y - 1, z);
        auto front = does_block_exist(chunk, x, y, z - 1);
        auto back = does_block_exist(chunk, x, y, z + 1);
        auto left = does_block_exist(chunk, x - 1, y, z);
        auto right = does_block_exist(chunk, x + 1, y, z);
        return top && bottom && front && back && left && right;
    }

    bool does_block_exist(ECS::Entity *chunk, int x, int y, int z)
    {
        return is_block_valid(x, y, z) && chunk->get<Chunk>().get().blocks[x][y][z].type > 0;
    }

    bool is_block_valid(int x, int y, int z)
    {
        return x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE;
    }

    int get_block_type(ECS::Entity *chunk, int x, int y, int z)
    {
        return is_block_valid(x, y, z) ? chunk->get<Chunk>().get().blocks[x][y][z].type : -1;
    }

private:
    ECS::World *m_world;
    BatchRenderSystem *m_batchSystem;
    std::map<std::tuple<int, int, int>, ECS::Entity *> m_chunks;
};

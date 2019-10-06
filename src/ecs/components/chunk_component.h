#pragma once

#include <map>

#include "ecs.h"

#include "ecs/components/render_type_components.h"

//NB: TEST CODE FOR A FIRST ATTEMPT AT CHUNKS
#include "ecs/components/basic_models.h"

class Chunk {
public:
    Chunk() {}
    Chunk(ECS::World* world, int v, int w) : m_world(world), v(v), w(w) {
        //Init Chunk (16x16x16)
        for(int x=0; x<16; x++) {
            m_blocks[x] = std::map<int, std::map<int,ECS::Entity*>>();
            for(int y=0; y<16; y++) {
                m_blocks[x][y] = std::map<int,ECS::Entity*>();
                for(int z=0; z<16; z++) {
                    m_blocks[x][y][z] = nullptr;
                }
            }
        }
    }

    void add_block(ECS::Entity* entity, int x, int y, int z) {
        if(!valid_block(x, y , z)) {
            entity->remove<VisibleComponent>();
            return;
        }
        
        m_blocks[x][y][z] = entity;
        update_visibility(x, y, z);

        update_visibility(x+1, y, z);
        update_visibility(x-1, y, z);
        update_visibility(x, y+1, z);
        update_visibility(x, y-1, z);
        update_visibility(x, y, z+1);
        update_visibility(x, y, z-1);
    }

    void resolve_visibility() {
        if(!m_world) return;

        for(int x=0; x<16; x++) { for(int y=0; y<16; y++) { for(int z=0; z<16; z++) {
            int hiddenSides = 0;
            if(m_blocks[x][y][z]) {
                if(get_block(x, y, z+1)) set_bit(hiddenSides, FRONT);
                if(get_block(x, y, z-1)) set_bit(hiddenSides, BACK);

                if(get_block(x-1, y, z)) set_bit(hiddenSides, LEFT);
                if(get_block(x+1, y, z)) set_bit(hiddenSides, RIGHT);

                if(get_block(x, y+1, z)) set_bit(hiddenSides, TOP);
                if(get_block(x, y-1, z)) set_bit(hiddenSides, BOTTOM);

                if(hiddenSides != 0) {
                    auto batchId = m_blocks[x][y][z]->get<BatchRenderComponent>().get().batch_id;
                    auto entity = create_cube(m_world, "dirt.png", glm::vec3(x-v, y, z-w), hiddenSides);
                    entity->assign<BatchRenderComponent>( batchId );

                    m_world->destroy(m_blocks[x][y][z]);
                    add_block(entity, x, y, z);
                }
            }
        }}}
    }

    void update_visibility(int x, int y, int z) {
        if(auto entity = get_block(x, y, z)) !is_hidden(x, y, z) ? entity->assign<VisibleComponent>() : entity->remove<VisibleComponent>();
    }

    bool is_hidden(int x, int y,  int z) {
        auto top = get_block(x, y+1, z);
        auto bottom = get_block(x, y-1, z);
        auto front = get_block(x, y, z-1);
        auto back = get_block(x, y, z+1);
        auto left = get_block(x-1, y, z);
        auto right = get_block(x+1, y, z);
        return top != nullptr && bottom != nullptr && front != nullptr && back != nullptr && left != nullptr && right != nullptr;
    }

    ECS::Entity* get_block(int x, int y, int z) {
        return valid_block(x, y, z) ? m_blocks[x][y][z] : nullptr;
    }

    bool valid_block(int x, int y, int z) {
        return x >= 0 && x < 16 
            && y >= 0 && y < 16
            && z >= 0 && z < 16;
    }

private:
    //Brute force approach //TODO: Replace and Improve
    ECS::World* m_world {nullptr};
    int v, w;
    std::map<int, std::map<int, std::map<int,ECS::Entity*>>> m_blocks;
};
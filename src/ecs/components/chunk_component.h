#pragma once

#include <map>

#include "ecs.h"

#include "ecs/components/render_type_components.h"

//NB: TEST CODE FOR A FIRST ATTEMPT AT CHUNKS

//TODO: Move implementation to cpp
class Chunk {
public:
    Chunk(){
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
            return; //BAD INSERTION //TODO: Handle
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

    //TODO:
    //Remove Block from chunk
    //if surrounding blocks were hidden, show them

    void update_visibility(int x, int y, int z) {
        if(auto entity = get_block(x, y, z))
            !is_hidden(x, y, z) ? entity->assign<VisibleComponent>() : entity->remove<VisibleComponent>();
    }

    //TODO: use ecs notify system for this?
    bool is_hidden(int x, int y,  int z) {
        auto top = get_block(x, y+1, z);
        auto bottom = get_block(x, y-1, z);
        auto front = get_block(x, y, z-1);
        auto back = get_block(x, y, z+1);
        auto left = get_block(x-1, y, z);
        auto right = get_block(x+1, y, z);

        //TODO: replace this...
        //If all surrounding blocks exist...
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
    std::map<int, std::map<int, std::map<int,ECS::Entity*>>> m_blocks;
};
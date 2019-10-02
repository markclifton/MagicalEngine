#pragma once

#include <fstream>
#include <string>

#include "ecs.h"
#include "glm/glm.hpp"

#include "ecs/components/renderable_component.h"
#include "utils/log.h"

namespace {
    std::vector<std::string> split(const std::string& s, char seperator) {
        std::vector<std::string> output;
        std::string::size_type prev_pos = 0, pos = 0;
        while((pos = s.find(seperator, pos)) != std::string::npos) {
            std::string substring( s.substr(prev_pos, pos-prev_pos) );
            output.push_back(substring);
            prev_pos = ++pos;
        }

        output.push_back(s.substr(prev_pos, pos-prev_pos));
        return output;
    }
}

namespace ME { namespace Graphics {
    ECS::Entity* load_mesh(ECS::World* world, const std::string& path, glm::vec3 position) {
        std::vector<glm::vec3> normals;
        std::vector<glm::vec3> uv;
        std::vector<VertexComponent> vertices;

        VerticesComponent verticesComponent;
        IndicesComponent indicesComponent;
        auto entity = world->create();

        std::ifstream ifs(path);
        if(!ifs.is_open()) ME::Log<FATAL>() << "Failed to open model: " + path;

        std::string line;
        while ( std::getline (ifs, line) ) {
            std::vector<std::string> ls = split(line, ' ');
            if(ls.size() > 3 && ls.front().compare("v") == 0) {
                VertexComponent v;
                v.position = glm::vec3(std::stof(ls[1]), std::stof(ls[2]), std::stof(ls[3])) + position;
                vertices.push_back(v);
            }
            else if(ls.size() > 3 && ls.front().compare("vn") == 0) {
                normals.emplace_back(std::stof(ls[1]), std::stof(ls[2]), std::stof(ls[3]));
            }
            else if(ls.size() > 2 && ls.front().compare("vt") == 0) {
                uv.emplace_back(std::stof(ls[1]), std::stof(ls[2]), 0);
            }
            else if(ls.size() > 3 && ls.front().compare("f") == 0) {
                auto vertInfo1 = split(ls[1], '/');
                auto vertInfo2 = split(ls[2], '/');
                auto vertInfo3 = split(ls[3], '/');

                int vertex1 = std::stoi(vertInfo1[0]) -1;
                int vertex2 = std::stoi(vertInfo2[0]) -1;
                int vertex3 = std::stoi(vertInfo3[0]) -1;

                VertexComponent v1 = vertices[static_cast<size_t>(vertex1)];
                VertexComponent v2 = vertices[static_cast<size_t>(vertex2)];
                VertexComponent v3 = vertices[static_cast<size_t>(vertex3)];

                //Tex Coords
                if(vertInfo1.size() > 1 && strcmp(vertInfo1[1].c_str(), "") != 0 && uv.size() > 0) {
                    v1.uv = uv[static_cast<size_t>(std::stoi(vertInfo1[1]))-1];
                    v2.uv = uv[static_cast<size_t>(std::stoi(vertInfo2[1]))-1];
                    v3.uv = uv[static_cast<size_t>(std::stoi(vertInfo3[1]))-1];
                }

                //Normals
                if(vertInfo1.size() > 2 && strcmp(vertInfo1[2].c_str(), "") != 0 && normals.size() > 0) {
                    v1.uv = normals[static_cast<size_t>(std::stoi(vertInfo1[2]))-1];
                    v2.uv = normals[static_cast<size_t>(std::stoi(vertInfo2[2]))-1];
                    v3.uv = normals[static_cast<size_t>(std::stoi(vertInfo3[2]))-1];
                }

                if(vertInfo1.size() > 0) {
                    verticesComponent.verts.push_back(v1);
                    vertex1 = static_cast<int>(verticesComponent.verts.size() - 1);
                    verticesComponent.verts.push_back(v2);
                    vertex2 = static_cast<int>(verticesComponent.verts.size() - 1);
                    verticesComponent.verts.push_back(v3);
                    vertex3 = static_cast<int>(verticesComponent.verts.size() - 1);

                    indicesComponent.indices.push_back(vertex1);
                    indicesComponent.indices.push_back(vertex2);
                    indicesComponent.indices.push_back(vertex3);
                }
            }
        }
        ifs.close();

        entity->assign<VerticesComponent>(verticesComponent);
        entity->assign<IndicesComponent>(indicesComponent);
        entity->assign<VisibleComponent>();
        
        return entity;
    }
}}
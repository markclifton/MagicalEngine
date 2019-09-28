#pragma once 

#include <map>

#include "ecs/components/renderable_component.h"
#include "ecs/components/render_type_components.h"
#include "ecs.h"
#include "graphics/buffers/index_buffer.h"
#include "graphics/buffers/vertex_buffer.h"

struct BatchEntity {
    ME::Graphics::IndexBuffer ibo;
    ME::Graphics::VertexBuffer vbo;
    uint32_t numVertices {0};
    uint32_t numIndices {0};
    bool edited {false};
};

class BatchRenderSystem : public ECS::EntitySystem {
public:
    BatchRenderSystem() {}

    std::map<int32_t, BatchEntity> m_batches;
    int32_t get_batch() {
        //TODO: Support more than 1 batch
        if(m_batches.size() == 0) {
            m_batches[0] = BatchEntity();
        }
        return 0;
    }

    void tick(ECS::World* world, float delta) override {
        for(auto& cur_batch : m_batches) {
            //If edited, rebuffer the data
            if(cur_batch.second.edited) {
                cur_batch.second.vbo.buffer(cur_batch.second.numVertices * sizeof(VertexComponent), nullptr);

                int currentVerts = 0;
                std::vector<uint32_t> finalIndices;
                world->each<BatchRenderComponent>([&](ECS::Entity* ent, ECS::ComponentHandle<BatchRenderComponent> component) {
                    if(component.get().batch_id != cur_batch.first) return;

                    auto& verts = ent->get<VerticesComponent>().get().verts;
                    auto& indices = ent->get<IndicesComponent>().get().indices;

                    cur_batch.second.vbo.buffer_sub_data(sizeof(VertexComponent) * currentVerts , sizeof(VertexComponent) * verts.size(), &verts[0]);

                    for(auto index : indices) finalIndices.push_back(index + currentVerts);
                    currentVerts += verts.size();
                });

                cur_batch.second.ibo.buffer(sizeof(int32_t) * finalIndices.size(), &finalIndices[0]);
                cur_batch.second.edited = false;
            }

            //Render!
            cur_batch.second.vbo.bind();
            cur_batch.second.ibo.bind();
            glDrawElements(GL_TRIANGLES, cur_batch.second.numIndices, GL_UNSIGNED_INT, nullptr);
        }
    }
};
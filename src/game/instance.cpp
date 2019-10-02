#include "instance.h"

#include "camera/camera.h"
#include "ecs/components/basic_models.h"
#include "ecs/components/basic_shapes.h"
#include "ecs/systems/renderer.h"
#include "ecs/systems/batch_renderer.h"
#include "ecs/systems/instanced_renderer.h"
#include "ecs.h"
#include "graphics/shaders/shader.h"
#include "utils/log.h"
#include "utils/timer.h"

//TEST
#include "ecs/components/renderable_component.h"
#include "graphics/textures/texture.h"
#include "graphics/mesh.h"
#include "ecs/components/chunk_component.h"

namespace ME { namespace Game {
    Instance::Instance() 
    : m_window(std::make_unique<Window>())
    {}

    void Instance::run() {
        load();

        auto renderSystem = new RenderSystem();
        auto batchSystem = new BatchRenderSystem();
        auto instancedSystem = new InstancedRenderSystem();

        auto world = ECS::World::createWorld();
        world->registerSystem(batchSystem);
        world->registerSystem(instancedSystem);
        //world->registerSystem(renderSystem);

        std::vector<Chunk> chunks;
        for(int v=0; v<5; v++){ for(int w=0; w<5; w++){
            chunks.emplace_back();
            for(float x = 0; x < 16; x+=1){ for(float y = 0; y < 16; y+=1){ for(float z = 0; z < 16; z+=1){
                auto entity = ME::Graphics::load_mesh(world, "res/models/cube2.obj", glm::vec3(x-16*v, y, z-16*w));
                entity->assign<BatchRenderComponent>( batchSystem->get_batch() );
                batchSystem->m_batches[0].numVertices += entity->get<VerticesComponent>().get().verts.size();
                batchSystem->m_batches[0].numIndices += entity->get<IndicesComponent>().get().indices.size();
                chunks.back().add_block(entity, x, y, z); //This should improve performance...
            } } }
        } }

        ME::Graphics::Texture t("res/textures/dirt.png");
        t.bind();

        ME::Camera::Camera3D::instance().SetViewport(0, 0, 1280, 720);
        m_window->registerHandler(&ME::Camera::Camera3D::instance());

        glfwSwapInterval(0);
        Timer timer;
        while(!m_window->shouldClose()) {
            update();
            world->tick(glfwGetTime());
            ME::Log<ME::DEBUG>() << "Frame Time: " + std::to_string(timer.reset());
        }
    }

    void Instance::load() {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glClearColor(.2, .3, .8, 1.f);
    }

    void Instance::update() {
        m_window->update();
        Camera::Camera3D::instance().Update();
    }
}}
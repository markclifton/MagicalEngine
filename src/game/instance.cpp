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
#include "graphics/textures/texture_manager.h"

#include "ecs/systems/chunk_system.h"

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
        world->registerSystem(renderSystem);


        batchSystem->get_batch();
        auto chunkSystem = new ChunkSystem(world);
        for(int x=0; x<16; x++) for(int z=0; z<16; z++) chunkSystem->generate_chunk(x, z);

        batchSystem->m_batches[0].numVertices = 16 * 16 * 16 * 32;

        ME::Camera::Camera3D::instance().SetViewport(0, 0, 1280, 720);
        m_window->registerHandler(&ME::Camera::Camera3D::instance());

        Timer timer;
        int counter = 0;

        double lastUpdate = glfwGetTime();
        while(!m_window->shouldClose()) {
            update();

            world->tick( glfwGetTime() - lastUpdate );
            lastUpdate = glfwGetTime();

            counter++;
            if(timer.get() > 1000){
                ME::Log<ME::DEBUG>() << "FPS: " + std::to_string(std::round(counter / timer.reset() * 1000.));
                counter = 0;
            }
        }
    }

    void Instance::load() {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glClearColor(.2, .3, .8, 1.f);
        //glfwSwapInterval(0);
    }

    void Instance::update() {
        m_window->update();
        Camera::Camera3D::instance().Update();
    }
}}
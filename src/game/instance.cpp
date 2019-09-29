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
        world->registerSystem(renderSystem);
        world->registerSystem(batchSystem);
        //world->registerSystem(instancedSystem);

        auto e = create_cube(world, glm::vec3(-.5, -.75f, -2.5f));
        e->assign<RenderComponent>();
        
        auto step = .5;

        auto instancedEntity = create_square(world, glm::vec3(), {step*.95, step*.95});
        instancedEntity->assign<InstancedRenderComponent>(instancedEntity);
        
        auto entity = world->create();
        entity->assign<InstancedComponent>(instancedEntity);
        entity->assign<XformComponent>( glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0)) );

        for(float x = -2.f; x < 2; x+=step){
            auto batchE = create_square(world, glm::vec3(x, 0.f, 0.f), {step*.95, step*.95});
            batchE->assign<BatchRenderComponent>( batchSystem->get_batch() );
            batchSystem->m_batches[0].numVertices += 4;
            batchSystem->m_batches[0].numIndices += 6;
        }

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);

        ME::Camera::Camera3D::instance().SetViewport(0, 0, 640, 480);
        m_window->registerHandler(&ME::Camera::Camera3D::instance());

        glfwSwapInterval(0);
   //     ME::Timer t;
        while(!m_window->shouldClose()) {
            update();
            world->tick(glfwGetTime());
     //       ME::Log<ME::DEBUG>() << ("Frame time: " + std::to_string(t.reset()));
        }
    }

    void Instance::load() {
    }

    void Instance::update() {
        m_window->update();
        Camera::Camera3D::instance().Update();
    }
}}
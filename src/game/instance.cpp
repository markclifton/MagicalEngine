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
#include "graphics/textures/texture.h"

namespace ME { namespace Game {
    Instance::Instance() 
    : m_window(std::make_unique<Window>())
    {}

    void Instance::run() {
        load();
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        auto renderSystem = new RenderSystem();
        auto batchSystem = new BatchRenderSystem();
        auto instancedSystem = new InstancedRenderSystem();

        auto world = ECS::World::createWorld();
        world->registerSystem(batchSystem);
        world->registerSystem(instancedSystem);
        world->registerSystem(renderSystem);

        auto ee = create_square(world, glm::vec3(), {1, 1});
        ee->assign<RenderComponent>();

        for(float x = -2.f; x < 2; x+=1){
            auto batchE = create_cube(world, glm::vec3(x, -.75f, -2.5f));
            batchE->assign<BatchRenderComponent>( batchSystem->get_batch() );
            batchSystem->m_batches[0].numVertices += 8;
            batchSystem->m_batches[0].numIndices += 36;
        }

        ME::Graphics::Texture t("res/textures/attack_sword.png");
        t.bind();

        ME::Camera::Camera3D::instance().SetViewport(0, 0, 640, 480);
        m_window->registerHandler(&ME::Camera::Camera3D::instance());

        while(!m_window->shouldClose()) {
            update();
            world->tick(glfwGetTime());
        }
    }

    void Instance::load() {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
    }

    void Instance::update() {
        m_window->update();
        Camera::Camera3D::instance().Update();
    }
}}
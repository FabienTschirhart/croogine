#include "Croogine_App.h"
#include "Croogine_Camera.h"
#include "Croogine_RenderSystem.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <cassert>
#include <stdexcept>

namespace Croogine {

    CroogineApp::CroogineApp() {
        loadEntities();
    }

    CroogineApp::~CroogineApp() {}

	void CroogineApp::run() {

        CroogineRenderSystem renderSystem{ croogineDevice, croogineRenderer.getSwapChainRenderPass() };
        CroogineCamera camera{};

        while (!croogineWindow.shouldClose()) //Check the close flag of the application window; if there is a click on the close button, it leaves the while()
        {
            float aspect = croogineRenderer.getAspectRatio();
            
            
            //camera.setOrthographicProjection(-aspect, aspect, orthTop, orthBottom, orthNearPlane, orthFarPlane);
            camera.setPerspectiveProjection(fov, aspect, perspNearPlane, perspFarPlane);

            
            glfwPollEvents(); //check all events (click, resize, close, move, etc.) and set flags accordingly

            if (auto commandBuffer = croogineRenderer.beginFrame()) {
                croogineRenderer.beginSCRenderPass(commandBuffer);
                renderSystem.renderEntities(commandBuffer, entities, camera);
                croogineRenderer.endSCRenderPass(commandBuffer);
                croogineRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(croogineDevice.getDevice());
	}

    std::unique_ptr<CroogineModel> createCubePrimitive(CroogineDevice& device, glm::vec3 offset) {
        std::vector<CroogineModel::Vertex> vertices{

            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

        };
        for (auto& v : vertices) {
            v.position += offset;
        }
        return std::make_unique<CroogineModel>(device, vertices);
    }

    void CroogineApp::loadEntities() {
        std::shared_ptr<CroogineModel> croogineModel = createCubePrimitive(croogineDevice, { .0f, .0f, .0f });

        auto cube = CroogineEntity::createEntity();
        cube.model = croogineModel;
        cube.transform.translation = { .0f, .0f, 1.5f };
        cube.transform.scale = { .5f, .5f, .5f };

        entities.push_back(std::move(cube));
    }
}
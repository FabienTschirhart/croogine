#include "Croogine_App.h"

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

        while (!croogineWindow.shouldClose()) //Check the close flag of the application window; if there is a click on the close button, it leaves the while()
        {
            glfwPollEvents(); //check all events (click, resize, close, move, etc.) and set flags accordingly

            if (auto commandBuffer = croogineRenderer.beginFrame()) {
                croogineRenderer.beginSCRenderPass(commandBuffer);
                renderSystem.renderEntities(commandBuffer, entities);
                croogineRenderer.endSCRenderPass(commandBuffer);
                croogineRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(croogineDevice.getDevice());
	}

    void CroogineApp::loadEntities() {
        std::vector<CroogineModel::Vertex> vertices{
            {{0.0f, 0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}}
        };

        auto croogineModel = std::make_shared<CroogineModel>(croogineDevice, vertices);

        auto mesh = CroogineEntity::createEntity();
        mesh.model = croogineModel;
        mesh.color = { .1f, .8f, .1f };
        mesh.transform2D.translation.x = .2f;
        mesh.transform2D.scale = { 1.f, 1.f };
        mesh.transform2D.rotation = -.25f * glm::two_pi<float>();

        entities.push_back(std::move(mesh));
    }
}
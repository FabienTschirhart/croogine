#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// std
#include <string>

#include "Constants.h"

namespace Croogine {

	class CroogineWindow {

	public:
		CroogineWindow(int width, int height, std::string  name);
		~CroogineWindow();
		CroogineWindow(const CroogineWindow &) = delete;
		CroogineWindow &operator=(const CroogineWindow &) = delete;

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

		bool shouldClose();
		VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }
		bool wasWindowResized() { return framebufferResized; };
		void resetWindowResizedFlag() { framebufferResized = false; }
		GLFWwindow* getGLFWwindow() const { return window; }

	private:

		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
		void initWindow();

		int width;
		int height;
		bool framebufferResized = false;
		std::string windowName;
		GLFWwindow *window;
	};
}
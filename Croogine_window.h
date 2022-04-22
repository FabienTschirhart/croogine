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
		bool shouldClose();

	private:
		void initWindow();
		const int width;
		const int height;
		std::string windowName;
		GLFWwindow *window;
	};
}
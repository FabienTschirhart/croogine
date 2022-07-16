#pragma once

#include "Croogine_Camera.h"

// lib
#include <vulkan/vulkan.h>

namespace Croogine {

	struct Frame {
		int frameIndex;
		float frameDuration;
		VkCommandBuffer commandBuffer;
		CroogineCamera& camera;
	};

}
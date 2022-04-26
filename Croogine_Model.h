#pragma once

#include "Croogine_Device.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>


namespace Croogine {

	class CroogineModel {

	public:

		struct Vertex {
			glm::vec2 position;

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptionq();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptionq();

		};


		CroogineModel(CroogineDevice& device, const std::vector<Vertex>& vertices);
		~CroogineModel();

		CroogineModel(const CroogineModel &) = delete;
		CroogineModel &operator=(const CroogineModel &) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private :

		void createVertexBuffers(const std::vector<Vertex>& vertices);

		CroogineDevice &croogineDevice;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;
	};
}
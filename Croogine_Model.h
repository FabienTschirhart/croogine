#pragma once

#include "Croogine_Device.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>


namespace Croogine {

	class CroogineModel {

	public:

		struct Vertex {
			glm::vec3 position;
			glm::vec3 color;

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

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
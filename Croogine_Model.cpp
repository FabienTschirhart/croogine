#include "Croogine_Model.h"

//std
#include <vector>


namespace Croogine {

	CroogineModel::CroogineModel(CroogineDevice &device, const std::vector<Vertex> &vertices) : croogineDevice{device} {

		createVertexBuffers(vertices);
	}


	CroogineModel::~CroogineModel(){
		vkDestroyBuffer(croogineDevice.getDevice(), vertexBuffer, nullptr);
		vkFreeMemory(croogineDevice.getDevice(), vertexBufferMemory, nullptr);

	}

	void CroogineModel::createVertexBuffers(const std::vector<Vertex>& vertices) {
		vertexCount = static_cast<uint32_t>(vertices.size());
		assert(vertexCount >= 3 && "Vertex count must be at least 3");
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
		croogineDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			vertexBuffer,
			vertexBufferMemory);

		void* data;
		vkMapMemory(croogineDevice.getDevice(), vertexBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(croogineDevice.getDevice(), vertexBufferMemory);
	}

	void CroogineModel::bind(VkCommandBuffer commandBuffer){
		vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);	
	}


	void CroogineModel::draw(VkCommandBuffer commandBuffer){
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);


	}

}
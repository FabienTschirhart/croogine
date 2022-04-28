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

	void CroogineModel::draw(VkCommandBuffer commandBuffer){
		vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);	
	}


	void CroogineModel::bind(VkCommandBuffer commandBuffer){
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);


	}

	std::vector<VkVertexInputBindingDescription> CroogineModel::Vertex::getBindingDescriptions() {
		return{ 
			{ 0,sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX } 
		}; //{binding, stride, inputrate}
	}

	std::vector<VkVertexInputAttributeDescription> CroogineModel::Vertex::getAttributeDescriptions() {
		return{ 
			{0,0,VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex,position)}, 
			{1,0,VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)} 
		};  //{location, binding, format, offset}
	}

}
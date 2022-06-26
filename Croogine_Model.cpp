#define _CRT_SECURE_NO_WARNINGS
#define GLM_ENABLE_EXPERIMENTAL

#include "Croogine_Model.h"
#include "Utils.h"

//std
#include <vector>
#include <iostream>
#include <unordered_map>

//lib
#define TINYOBJLOADER_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.

#include <tiny_obj_loader.h>
#include <tiny_gltf.h>
#include <glm/gtx/hash.hpp>


namespace std {
	template <>
	struct hash<Croogine::CroogineModel::Vertex> {
		size_t operator()(Croogine::CroogineModel::Vertex const& vertex) const {
			size_t seed = 0;
			CroogineUtils::hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
			return seed;
		}
	};
}

namespace Croogine {

	CroogineModel::CroogineModel(CroogineDevice &device, const CroogineModel::Modeler &modeler) : croogineDevice{device} {

		createVertexBuffers(modeler.vertices);
		createIndexBuffers(modeler.indices);
	}


	CroogineModel::~CroogineModel(){
		vkDestroyBuffer(croogineDevice.getDevice(), vertexBuffer, nullptr);
		vkFreeMemory(croogineDevice.getDevice(), vertexBufferMemory, nullptr);

		if (hasIndexBuffer) {
			vkDestroyBuffer(croogineDevice.getDevice(), indexBuffer, nullptr);
			vkFreeMemory(croogineDevice.getDevice(), indexBufferMemory, nullptr);
		}
	}

	std::unique_ptr<CroogineModel> CroogineModel::createModel(CroogineDevice& device, const std::string& filepath, bool gltf)
	{
		Modeler modeler{};
		modeler.LoadModel(filepath, gltf);

		std::cout << "Vertex count : " << modeler.vertices.size() << "\n";
		
		return std::make_unique<CroogineModel>(device, modeler);
	}


	void CroogineModel::createVertexBuffers(const std::vector<Vertex>& vertices) {
		vertexCount = static_cast<uint32_t>(vertices.size());
		assert(vertexCount >= 3 && "Vertex count must be at least 3");
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		croogineDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer,
			stagingBufferMemory);

		void* data;
		vkMapMemory(croogineDevice.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(croogineDevice.getDevice(), stagingBufferMemory);

		croogineDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			vertexBuffer,
			vertexBufferMemory);

		croogineDevice.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

		vkDestroyBuffer(croogineDevice.getDevice(), stagingBuffer, nullptr);
		vkFreeMemory(croogineDevice.getDevice(), stagingBufferMemory, nullptr);
	}

	void CroogineModel::createIndexBuffers(const std::vector<uint32_t>& indices) {
		indexCount = static_cast<uint32_t>(indices.size());
		hasIndexBuffer = indexCount > 0 ? true : false;

		if (!hasIndexBuffer) 
			return;

		VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		croogineDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer,
			stagingBufferMemory);

		void* data;
		vkMapMemory(croogineDevice.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(croogineDevice.getDevice(), stagingBufferMemory);

		croogineDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			indexBuffer,
			indexBufferMemory);

		croogineDevice.copyBuffer(stagingBuffer, indexBuffer, bufferSize);

		vkDestroyBuffer(croogineDevice.getDevice(), stagingBuffer, nullptr);
		vkFreeMemory(croogineDevice.getDevice(), stagingBufferMemory, nullptr);
	}

	void CroogineModel::draw(VkCommandBuffer commandBuffer){
		if (hasIndexBuffer)
			vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
		else
			vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);	
	}


	void CroogineModel::bind(VkCommandBuffer commandBuffer){
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

		if (hasIndexBuffer) 
			vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
		
	}

	std::vector<VkVertexInputBindingDescription> CroogineModel::Vertex::getBindingDescriptions() {
		return{ 
			{ 0,sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX } 
		}; //{binding, stride, inputrate}
	}

	std::vector<VkVertexInputAttributeDescription> CroogineModel::Vertex::getAttributeDescriptions() {
		return{ 
			{0,0,VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex,position)}, 
			{1,0,VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)} 
		};  //{location, binding, format, offset}
	}

	void CroogineModel::Modeler::LoadModel(const std::string& filepath, bool gltf) {

		std::string warning, error;

		if (gltf)
		{
			tinygltf::Model model;
			tinygltf::TinyGLTF loader;

			if (loader.LoadASCIIFromFile(&model, &error, &warning, filepath.c_str()))
				throw std::runtime_error(warning + error);

			return;
		}

	
		tinyobj::attrib_t attribute; //position, color, norma & uv

		std::vector<tinyobj::shape_t> shapes; //index value for each vertices (attribute) of each polygon
		std::vector<tinyobj::material_t> materials; //material index value for each shape

		if (!tinyobj::LoadObj(&attribute, &shapes, &materials, &warning, &error, filepath.c_str()))
			throw std::runtime_error(warning + error);

		vertices.clear();
		indices.clear();

		std::unordered_map<Vertex, uint32_t> uniqueVertices{};

		for (const auto& shape : shapes)
		{
			for (const auto& index : shape.mesh.indices)
			{
				Vertex vertex{};

				if (index.vertex_index >= 0) { //check if there is position information for this vertex
					vertex.position = {
						attribute.vertices[3 * index.vertex_index + 0],
						attribute.vertices[3 * index.vertex_index + 1],
						attribute.vertices[3 * index.vertex_index + 2],
					};

					auto colorIndex = 3 * index.vertex_index + 2;

					if (colorIndex < attribute.colors.size()) //check if a color information is next to the position
							vertex.color = {
						attribute.colors[colorIndex - 2],
						attribute.colors[colorIndex - 1],
						attribute.colors[colorIndex - 0],
						};
					else //otherwise, generate a default grey color value
						vertex.color = { .5f, .5f, .5f };
				}
				else
					continue; //if there is no position information, go directly to the next vertex without pushing this one to the vertex buffer
				
				if (index.normal_index >= 0) //check if there is a normal associated to this vertex
					vertex.normal = {
						attribute.normals[3 * index.normal_index + 0],
						attribute.normals[3 * index.normal_index + 1],
						attribute.normals[3 * index.normal_index + 2],
					};

				if (index.normal_index >= 0) //check if there is a uv associated to this vertex
					vertex.uv = {
						attribute.texcoords[2 * index.texcoord_index + 0],
						attribute.texcoords[2 * index.texcoord_index + 1],
					};

				if (uniqueVertices.count(vertex) == 0)
				{
					uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}
				indices.push_back(uniqueVertices[vertex]);
			}
		}
	}


}
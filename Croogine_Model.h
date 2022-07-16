#pragma once

#include "Croogine_Device.h"
#include "Croogine_Buffer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <memory>


namespace Croogine {

	class CroogineModel {

	public:

		struct Vertex {
			glm::vec3 position{};
			glm::vec3 color{};
			glm::vec3 normal{};
			glm::vec2 uv{};

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

			bool operator==(const Vertex& other) const {
				return position == 
					other.position && color == other.color 
					&& 
					normal == other.normal 
					&& 
					uv == other.uv;
			}
		};

		struct Modeler {
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};

			void LoadModel(const std::string& filepath, bool gltf = false);
		};


		CroogineModel(CroogineDevice& device, const CroogineModel::Modeler &builder);
		~CroogineModel();

		CroogineModel(const CroogineModel &) = delete;
		CroogineModel &operator=(const CroogineModel &) = delete;

		static std::unique_ptr<CroogineModel> createModel(
			CroogineDevice& device, 
			const std::string& filepath, 
			bool gltf = false);

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private :

		void createVertexBuffers(const std::vector<Vertex>& vertices);
		void createIndexBuffers(const std::vector<uint32_t>& indices);


		CroogineDevice &croogineDevice;
		std::unique_ptr<CroogineBuffer> vertexBuffer;
		uint32_t vertexCount;

		bool hasIndexBuffer = false;
		std::unique_ptr<CroogineBuffer> indexBuffer;
		uint32_t indexCount;
	};
}
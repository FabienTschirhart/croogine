#pragma once

#include "Croogine_Model.h"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace Croogine {

	struct Transform {
		glm::vec3 translation{};
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
		glm::vec3 rotation{};


		glm::mat4 mat4() {
			auto transform = glm::translate(glm::mat4{ 1.f }, translation);

			transform = glm::rotate(transform, rotation.y, { 0.0f, 1.0f, 0.0f });
			transform = glm::rotate(transform, rotation.x, { 1.0f, 0.0f, 0.0f });
			transform = glm::rotate(transform, rotation.z, { 0.0f, 0.0f, 1.0f });
			transform = glm::scale(transform, scale);

			return transform;
		}
	};

	class CroogineEntity {
	
	public:
		using id_t = unsigned int;

		static CroogineEntity createEntity() {
			static id_t currentId = 0;
			return CroogineEntity{ currentId++ };
		}

		CroogineEntity(const CroogineEntity&) = delete;
		CroogineEntity& operator=(const CroogineEntity&) = delete;
		CroogineEntity(CroogineEntity&&) = default;
		CroogineEntity& operator=(CroogineEntity&&) = default;

		id_t getId() { return id; }

		std::shared_ptr<CroogineModel> model{};
		glm::vec3 color{};
		Transform transform{};

	private:
		CroogineEntity(id_t entityId): id(entityId) {}
		id_t id;
	
	
	
	};

}

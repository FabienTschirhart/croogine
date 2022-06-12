#include "Croogine_Camera.h"

//std
#include <cassert>
#include <limits>

namespace Croogine {

	void CroogineCamera::setOrthographicProjection(
		float left, float right, float top, float bottom, float nearPlane, float farPlane) {
		projectionMatrix = glm::mat4{ 1.0f };
		projectionMatrix[0][0] = 2.f / (right - left);
		projectionMatrix[1][1] = 2.f / (bottom - top);
		projectionMatrix[2][2] = 1.f / (farPlane - nearPlane);
		projectionMatrix[3][0] = -(right + left) / (right - left);
		projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
		projectionMatrix[3][2] = -nearPlane / (farPlane - nearPlane);
	}

	void CroogineCamera::setPerspectiveProjection(float fov, float aspect, float nearPlane, float farPlane) {
		assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
		const float tanHalfFovy = tan(fov / 2.f);
		projectionMatrix = glm::mat4{ 0.0f };
		projectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
		projectionMatrix[1][1] = 1.f / (tanHalfFovy);
		projectionMatrix[2][2] = farPlane / (farPlane - nearPlane);
		projectionMatrix[2][3] = 1.f;
		projectionMatrix[3][2] = -(farPlane * nearPlane) / (farPlane - nearPlane);
	}

}
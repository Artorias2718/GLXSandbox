#ifndef ENGINE_SHARED_STRANSFORM_H
#define ENGINE_SHARED_STRANSFORM_H

#include "../Math/glm/vec3.hpp"
#include "../Math/glm/gtc/quaternion.hpp"
#include "../Math/glm/mat4x4.hpp"

namespace Engine 
{
	namespace Shared
	{
		struct sTransform
		{
			glm::vec3 position;
			glm::quat orientation;
			glm::vec3 scale;

			glm::vec3 right;
			glm::vec3 up;
			glm::vec3 forward;

			sTransform() : position(glm::zero<glm::vec3>()), orientation(glm::identity<glm::quat>()), scale(glm::one<glm::vec3>()),
				right(glm::vec3(1.0f, 0.0f, 0.0f)), up(glm::vec3(0.0f, 1.0f, 0.0f)), forward(glm::vec3(0.0f, 0.0f, -1.0f)) {}
		};
	}
}

#endif // !ENGINE_SHARED_STRANSFORM_H

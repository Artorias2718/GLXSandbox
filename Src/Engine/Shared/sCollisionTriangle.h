#ifndef ENGINE_SHARED_SCOLLISIONTRIANGLE_H
#define ENGINE_SHARED_SCOLLISIONTRIANGLE_H

#include "../Math/glm/gtc/matrix_transform.hpp"
#include "../Math/glm/vec3.hpp"

namespace Engine
{
	namespace Shared
	{
		struct sCollisionTriangle
		{
			sCollisionTriangle(const glm::vec3 &i_a = glm::zero<glm::vec3>(), const glm::vec3 &i_b = glm::zero<glm::vec3>(), const glm::vec3 &i_c = glm::zero<glm::vec3>()) : a(i_a), b(i_b), c(i_c) {}
			bool ComputeNormal()
			{
				ab = b - a;
				ac = c - a;

				normal = glm::cross(ab, ac);
				return true;
			}
			glm::vec3 a, b, c, ab, ac, normal;
		};

	}
}

#endif // !ENGINE_SHARED_SCOLLISIONTRIANGLE_H

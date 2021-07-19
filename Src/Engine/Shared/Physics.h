#ifndef ENGINE_SHARED_CPHYSICS_H
#define ENGINE_SHARED_CPHYSICS_H

#include "../Math/glm/vec3.hpp"

namespace Engine 
{
	namespace Shared
	{
		class cGameObject;
		namespace Physics
		{
			bool ApplyLinearForce(cGameObject* i_gameobject, const glm::vec3& i_force, float i_deltaTime);
			bool TriangleRaycast(cGameObject* i_gameobject, bool i_isSegment,
				const glm::vec3& i_p, const glm::vec3& i_q, glm::vec3& o_triNormal, glm::vec3& o_intersectionPoint);
		}
	}
}

#endif // !ENGINE_SHARED_CPHYSICS_H

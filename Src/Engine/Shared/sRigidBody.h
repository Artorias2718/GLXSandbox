#ifndef ENGINE_SHARED_SRIGIDBODY_H
#define ENGINE_SHARED_SRIGIDBODY_H

#include "../Math/glm/vec3.hpp"
#include <vector>

namespace Engine 
{
	namespace Shared
	{
		struct sCollisionTriangle;
		class sRigidBody
		{
		public:
			bool m_applyGravity;
			float m_mass;
			float m_speed;

			glm::vec3 velocity;
			glm::vec3 acceleration;
			glm::vec3 gravity;
			std::vector<sCollisionTriangle*> m_collisionData;
		};
	}
}

#endif // !ENGINE_SHARED_SRIGIDBODY_H


#ifndef ENGINE_SHARED_CRIGIDBODY_H
#define ENGINE_SHARED_CRIGIDBODY_H

#include "../Math/cVector.h"
#include <vector>

namespace Engine
{
	namespace Shared
	{
		struct sCollisionTriangle;
		class sRigidBody
		{
		public:
			//sRigidBody();
			//~sRigidBody();

			bool m_applyGravity;
			float m_angleInDegrees;

			float m_mass;
			float m_dragCoefficient;
			float m_maxSpeed;

			Math::cVector rotationAxis;
			Math::cVector motionAxis;
			Math::cVector velocity;
			Math::cVector acceleration;
			Math::cVector gravity;
			std::vector<sCollisionTriangle*> m_collisionData;
		};
	}
}

#endif // !ENGINE_SHARED_SRIGIDBODY_H


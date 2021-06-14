#ifndef ENGINE_SHARED_PHYSICS_H
#define ENGINE_SHARED_PHYSICS_H

namespace Engine 
{
	namespace Math
	{
		class cVector;
	}
	namespace Shared
	{
		class cGameObject;
		namespace Physics
		{
			bool ApplyLinearForce(cGameObject* i_gameobject, const Math::cVector& i_force, float i_deltaTime);
			bool Raycast(cGameObject* i_gameobject, bool i_isSegment,
				const Math::cVector& i_p, const Math::cVector& i_q, Math::cVector& o_triNormal, Math::cVector& o_intersectionPoint);
		}
	}
}

#endif // !ENGINE_SHARED_PHYSICS_H


#ifndef ENGINE_SHARED_SCOLLISIONTRIANGLE_H
#define ENGINE_SHARED_SCOLLISIONTRIANGLE_H

#include "../Math/cVector.h"

namespace Engine
{
	namespace Shared
	{
		struct sCollisionTriangle
		{
			sCollisionTriangle(const Math::cVector& i_a = Math::cVector(), const Math::cVector& i_b = Math::cVector(), const Math::cVector& i_c = Math::cVector()) : a(i_a), b(i_b), c(i_c) {}
			bool ComputeNormal()
			{
				Math::cVector ab = b - a;
				Math::cVector bc = c - b;
				Math::cVector ac = c - a; 

				normal = Math::Cross(ab, ac);
				return true;
			}
			Math::cVector a, b, c, normal;
		};
	}
}


#endif // !ENGINE_SHARED_SCOLLISIONTRIANGLE_H
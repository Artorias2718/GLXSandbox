#ifndef ENGINE_SHARED_STRANSFORM_H
#define ENGINE_SHARED_STRANSFORM_H

#include "../Math/cVector.h"
#include "../Math/cQuaternion.h"
#include "../Math/cMatrix_Transformation.h"

namespace Engine 
{
	namespace Shared
	{
		struct sTransform
		{
			Math::cVector position;
			Math::cQuaternion orientation;
			Math::cVector scale;

			Math::cVector right;
			Math::cVector up;
			Math::cVector forward;

			sTransform() : position(Math::cVector()), orientation(Math::cQuaternion()), scale(Math::cVector(1.0f, 1.0f, 1.0f)),
				right(Math::cVector(1.0f, 0.0f, 0.0f)), up(Math::cVector(0.0f, 1.0f, 0.0f)), forward(Math::cVector(0.0f, 0.0f, -1.0f)) {}
		};
	}
}

#endif // !ENGINE_SHARED_STRANSFORM_H

#ifndef ENGINE_GRAPHICS_STRUCTURES_SDRAWCALL_H
#define ENGINE_GRAPHICS_STRUCTURES_SDRAWCALL_H

#include "../../Math/cMatrix_Transformation.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Structures
		{
			struct sDrawCall
			{
				Math::cMatrix_Transformation g_localToWorld;
			};
		}
	}
}

#endif // !ENGINE_GRAPHICS_STRUCTURES_SDRAWCALL_H

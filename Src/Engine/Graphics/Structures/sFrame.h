#ifndef ENGINE_GRAPHICS_STRUCTURES_SFRAME_H
#define ENGINE_GRAPHICS_STRUCTURES_SFRAME_H

#include "../../Math/cMatrix_Transformation.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Structures 
		{
			struct sFrame
			{
				Math::cMatrix_Transformation g_worldToCamera;
				Math::cMatrix_Transformation g_cameraToScreen;
				union
				{
					float g_elapsedSeconds;
					float register8[4];
				};
			};
		}
	}
}

#endif // !ENGINE_GRAPHICS_STRUCTURES_SFRAME_H

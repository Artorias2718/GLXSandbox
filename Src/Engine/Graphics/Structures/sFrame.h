#ifndef ENGINE_GRAPHICS_STRUCTURES_SFRAME_H
#define ENGINE_GRAPHICS_STRUCTURES_SFRAME_H

#include "../../Math/glm/mat4x4.hpp"

namespace Engine
{
	namespace Graphics
	{
		namespace Structures
		{
			struct sFrame
			{
				glm::mat4x4 g_worldToCamera;
				glm::mat4x4 g_cameraToScreen;
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
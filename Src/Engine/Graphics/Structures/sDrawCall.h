#ifndef ENGINE_GRAPHICS_STRUCTURES_SDRAWCALL_H
#define ENGINE_GRAPHICS_STRUCTURES_SDRAWCALL_H

#include "../../Math/glm/mat4x4.hpp"

namespace Engine
{
	namespace Graphics
	{
		namespace Structures
		{
			struct sDrawCall
			{
				glm::mat4 g_localToWorld;
			};
		}
	}
}

#endif // !ENGINE_GRAPHICS_STRUCTURES_SDRAWCALL_H

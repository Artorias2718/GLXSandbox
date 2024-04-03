#ifndef ENGINE_GRAPHICS_STRUCTURES_SVERTEX_H
#define ENGINE_GRAPHICS_STRUCTURES_SVERTEX_H

#include "sPosition.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Structures
		{
			// This struct determines the layout of the geometric data that the CPU will send to the GPU
			struct sVertex
			{
				// POSITION
				// 2 floats == 8 bytes
				// Offset = 0
				sPosition position;
			};
		}
	}
}

#endif // !ENGINE_GRAPHICS_STRUCTURES_SVERTEX_H
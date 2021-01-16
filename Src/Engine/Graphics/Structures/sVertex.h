#ifndef ENGINE_GRAPHICS_STRUCTURES_SVERTEX_H
#define ENGINE_GRAPHICS_STRUCTURES_SVERTEX_H

#include "sPosition.h"
#include "sColor.h"

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

				// COLOR 
				// 4 unsigned 8-bit integers == 4 bytes
				// Offset = 1
				sColor8 color;
			};
		}
	}
}

#endif // !ENGINE_GRAPHICS_STRUCTURES_SVERTEX_H

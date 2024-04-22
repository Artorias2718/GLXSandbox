#ifndef ENGINE_GRAPHICS_STRUCTURES_SVERTEX_H
#define ENGINE_GRAPHICS_STRUCTURES_SVERTEX_H

#include "sTexture.h"
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
				// TEXTURE
				// 2 floats = 8 bytes
				// Offset = 0
				sTexture texture;

				// POSITION
				// 3 floats == 12 bytes
				// Offset = 1
				sPosition position;

				// COLOR
				// 4 unsigned 8-bit integers
				// Offset = 2
				sColor8 color;
			};
		}
	}
}

#endif // !ENGINE_GRAPHICS_STRUCTURES_SVERTEX_H
#ifndef ENGINE_GRAPHICS_STRUCTURES_SRECTANGLE_H
#define ENGINE_GRAPHICS_STRUCTURES_SRECTANGLE_H

namespace Engine
{
	namespace Graphics
	{
		namespace Structures
		{
			struct sRectangle 
			{
				// Center point
				// Center point
				float x, y, z, w;
				sRectangle(float i_x = 0.0f, float i_y = 0.0f, float i_z = 0.0f, float i_w = 1.0f) : x(i_x), y(i_y), z(i_z), w(i_w) {}
			};
		}
	}
}

#endif // !ENGINE_GRAPHICS_STRUCTURES_SRECTANGLE_H

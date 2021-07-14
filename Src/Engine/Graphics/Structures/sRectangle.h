#ifndef ENGINE_GRAPHICS_STRUCTURES_SRECTANGLE_H
#define ENGINE_GRAPHICS_STRUCTURES_SRECTANGLE_H

namespace Engine
{
	namespace Graphics
	{
		namespace Structures
		{
			template<class tNumber>
			struct sRectangle
			{
				tNumber x, y, z, w;
			};

			typedef struct sRectangle<float> sRectangleF;
		}
	}
}

#endif // !ENGINE_GRAPHICS_STRUCTURES_SRECTANGLE_H

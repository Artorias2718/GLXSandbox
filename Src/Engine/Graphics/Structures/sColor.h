#ifndef ENGINE_GRAPHICS_STRUCTURES_SCOLOR_H
#define ENGINE_GRAPHICS_STRUCTURES_SCOLOR_H

#include <cstdint>

namespace Engine
{
	namespace Graphics
	{
		namespace Structures
		{
			template<class tNumber>
			struct sColor
			{
				tNumber r, g, b, a;
				sColor(tNumber i_r = 1, tNumber i_g = 1, tNumber i_b = 1, tNumber i_a = 1) : r(i_r), g(i_g), b(i_b), a(i_a) {}
			};

			typedef sColor<uint8_t> sColor8;
		}
	}
}

#endif // !ENGINE_GRAPHICS_STRUCTURES_SCOLOR_H

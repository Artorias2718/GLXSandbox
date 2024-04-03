#ifndef ENGINE_GRAPHICS_STRUCTURES_SINDEXSET_H
#define ENGINE_GRAPHICS_STRUCTURES_SINDEXSET_H

#include <cstdint>

namespace Engine
{
	namespace Graphics
	{
		namespace Structures
		{
			template<class tNumber>
			struct sIndexSet
			{
				tNumber a, b, c;
			};

			typedef struct sIndexSet<uint16_t> sIndexSet16;
		}
	}
}

#endif // !ENGINE_GRAPHICS_STRUCTURES_SINDEXSET_H
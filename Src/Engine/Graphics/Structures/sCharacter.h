#ifndef ENGINE_GRAPHICS_STRUCTURES_SCHARACTER_H
#define ENGINE_GRAPHICS_STRUCTURES_SCHARACTER_H

#include <cstdint>
#include "../Structures/sPosition.h"
#include "../OGL/Includes.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Structures
		{
			struct sCharacter
			{
				unsigned int textureID;			// ID handle of the glyph texture
				Structures::sPosition size;		// Size of glyph
				Structures::sPosition bearing;	// Offset from baseline to left/top of glyph
				char advance;			// Offset to advance to next glyph
			};
		}
	}
}

#endif // !ENGINE_GRAPHICS_STRUCTURES_SCHARACTER_H
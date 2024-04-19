#ifndef ENGINE_GRAPHICS_STRUCTURES_SMATERIAL_H
#define ENGINE_GRAPHICS_STRUCTURES_SMATERIAL_H

#include "sColor.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Structures
		{
			struct sMaterial
			{
				sColorF g_color;
				sMaterial(const sColorF& i_color = sColorF(1.0f, 1.0f, 1.0f, 1.0f)) : g_color(i_color) {}
			};
		}
	}
}

#endif // !ENGINE_GRAPHICS_STRUCTURES_SMATERIAL_H
#ifndef ENGINE_GRAPHICS_STRUCTURES_SFRAME_H
#define ENGINE_GRAPHICS_STRUCTURES_SFRAME_H

namespace Engine
{
	namespace Graphics
	{
		namespace Structures
		{
			struct sFrame
			{
				union
				{
					float g_elapsedSeconds;
					float register0[4];
				};
			};
		}
	}
}

#endif // !ENGINE_GRAPHICS_STRUCTURES_SFRAME_H
#ifndef ENGINE_GRAPHICS_STRUCTURES_SDRAWCALL_H
#define ENGINE_GRAPHICS_STRUCTURES_SDRAWCALL_H

namespace Engine
{
	namespace Graphics
	{
		namespace Structures
		{
			struct sDrawCall
			{
				union
				{
					float g_screenPosition[2];
					float register0[4];
				};
			};
		}
	}
}

#endif // !ENGINE_GRAPHICS_STRUCTURES_SDRAWCALL_H
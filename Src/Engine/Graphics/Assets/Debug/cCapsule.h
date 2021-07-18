#ifndef ENGINE_GRAPHICS_ASSETS_DEBUG_CCAPSULE_H
#define ENGINE_GRAPHICS_ASSETS_DEBUG_CCAPSULE_H

#if defined D3D_API
struct ID3D11Buffer;
#elif defined OGL_API
#include "../../OGL/Includes.h"
#endif

#include "../cDebug.h"
#include "../../Structures/sColor.h"

#include <map>
#include <vector>

namespace Engine 
{
	namespace Graphics
	{
		struct sVertex;
		namespace Assets
		{
			namespace Debug
			{
				class cSphere;

				struct sVertex;
				class cCapsule : public cDebug
				{
				public:
					cCapsule();
					cCapsule(float i_radius, const Structures::sColor8& i_color);

					virtual bool Render();
				private:
					cSphere* m_top;
					cSphere* m_bottom;
				};
			}
		}
	}
}

#endif // !ENGINE_GRAPHICS_ASSETS_DEBUG_CCAPSULE_H

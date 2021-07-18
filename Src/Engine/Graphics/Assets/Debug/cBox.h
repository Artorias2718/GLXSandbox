#ifndef ENGINE_GRAPHICS_ASSETS_DEBUG_CBOX_H
#define ENGINE_GRAPHICS_ASSETS_DEBUG_CBOX_H

#if defined D3D_API
struct ID3D11Buffer;
#elif defined OGL_API
#include "../../OGL/Includes.h"
#endif

#include "../cDebug.h"
#include "../../Structures/sColor.h"

namespace Engine 
{
	namespace Graphics
	{
		struct sVertex;
		namespace Assets
		{
			namespace Debug
			{
				class cBox : public Engine::Graphics::Assets::cDebug
				{
				public:
					cBox(const Structures::sColor8& i_color = { 255, 255, 255, 255 });
					~cBox();

					bool Initialize();
					bool CleanUp();

					bool Render();

#if defined D3D_API
					ID3D11Buffer* m_vertexBufferId;
#elif defined OGL_API 
					GLuint m_vertexArrayId;
					GLuint m_vertexBufferId;
#endif
				};
			}
		}
	}
}

#endif // !ENGINE_GRAPHICS_ASSETS_DEBUG_CBOX_H

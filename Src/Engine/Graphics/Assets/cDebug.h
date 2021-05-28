#ifndef ENGINE_GRAPHICS_ASSETS_CDEBUG_H
#define ENGINE_GRAPHICS_ASSETS_CDEBUG_H

#if defined D3D_API
struct ID3D11Buffer;
#elif defined OGL_API 
#include "OpenGL/Includes.h"
#endif

#include "../Structures/sIndexSet.h"

#include <vector>

namespace Engine
{
	namespace Graphics
	{
		namespace Structures
		{
			struct sVertex;
		}
		namespace Assets
		{
			class cDebug
			{
			public:
				virtual bool Initialize() = 0;
				virtual bool CleanUp() = 0;

				virtual bool Render() = 0;

#if defined D3D_API
				ID3D11Buffer* m_vertexBufferId;
				ID3D11Buffer* m_indexBufferId;
#elif defined OGL_API 
				GLuint m_vertexArrayId;
				GLuint m_vertexBufferId;
				GLuint m_indexBufferId;
#endif
				Structures::sVertex* m_vertices;
				Structures::sIndexSet16* m_indexSet16;

				uint16_t m_vertexCount;
				uint16_t m_indexSetCount;
			};
		}
	}
}

#endif // !ENGINE_GRAPHICS_ASSETS_CDEBUG_H

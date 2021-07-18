#ifndef ENGINE_GRAPHICS_ASSETS_CDEBUG_H
#define ENGINE_GRAPHICS_ASSETS_CDEBUG_H

#if defined D3D_API
struct ID3D11Buffer;
#elif defined OGL_API 
#include "../OGL/Includes.h"
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
				virtual bool Initialize();
				virtual bool CleanUp();

				virtual bool Render();

#if defined D3D_API
				ID3D11Buffer* m_vertexBufferId;
				ID3D11Buffer* m_indexBufferId;
#elif defined OGL_API 
				GLuint m_vertexArrayId;
				GLuint m_vertexBufferId;
				GLuint m_indexBufferId;
#endif
				std::vector<Structures::sVertex> m_vertices;
				std::vector<uint16_t> m_indices16;
				std::vector<Structures::sIndexSet16> m_indexSet16;

				uint16_t m_vertexCount;
				uint16_t m_indexSetCount;
			};
		}
	}
}

#endif // !ENGINE_GRAPHICS_ASSETS_CDEBUG_H

#ifndef ENGINE_GRAPHICS_ASSETS_CMESH_H
#define ENGINE_GRAPHICS_ASSETS_CMESH_H

#if defined D3D_API
struct ID3D11Buffer;
#elif defined OGL_API
#include "../OGL/Includes.h"
#endif
#include "../Structures/sIndexSet.h"

#include <string>

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
			class cMesh
			{
			public:
				cMesh(std::string i_fileName = "");
				~cMesh();

				bool Initialize();
				bool Render();
				bool CleanUp();
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
				Structures::sIndexSet32* m_indexSet32;

				uint32_t m_vertexCount;
				uint32_t m_indexSetCount;
				bool m_isShort;
			};
		}
	}
}

#endif // !ENGINE_GRAPHICS_ASSETS_CMESH_H

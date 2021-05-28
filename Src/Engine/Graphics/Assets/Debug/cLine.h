#ifndef ENGINE_GRAPHICS_ASSETS_DEBUG_CLINE_H
#define ENGINE_GRAPHICS_ASSETS_DEBUG_CLINE_H

#if defined D3D_API
struct ID3D11Buffer;
#elif defined OGL_API
#include "../OpenGL/Includes.h"
#endif

#include "../cDebug.h"
#include "../../Structures/sColor.h"
#include "../../../Math/cVector.h"

namespace Engine 
{
	namespace Graphics
	{
		struct sVertex;
		namespace Assets
		{
			namespace Debug
			{
				class cLine : public Engine::Graphics::Assets::cDebug
				{
				public:
					cLine(const Math::cVector& i_startPosition = Math::cVector::zero, const Math::cVector& i_endPosition = Math::cVector::zero, const Structures::sColor8& i_color = { 255, 255, 255, 255 });
					~cLine();

					bool Initialize();
					bool CleanUp();

					bool Render();

					bool SetPosition(int i_index, const Math::cVector& i_position);
					bool SetColor(int i_index, const Structures::sColor8& i_color);

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

#endif // !ENGINE_GRAPHICS_ASSETS_DEBUG_CLINE_H


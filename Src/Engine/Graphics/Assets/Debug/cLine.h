#ifndef ENGINE_GRAPHICS_ASSETS_DEBUG_CLINE_H
#define ENGINE_GRAPHICS_ASSETS_DEBUG_CLINE_H

#if defined D3D_API
struct ID3D11Buffer;
#elif defined OGL_API
#include "../../OGL/Includes.h"
#endif

#include "../cDebug.h"
#include "../../Structures/sColor.h"
#include "../../../Math/glm/vec3.hpp"
#include "../../../Math/glm/gtc/matrix_transform.hpp"

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
					cLine(const glm::vec3& i_startPosition = glm::zero<glm::vec3>(), const glm::vec3& i_endPosition = glm::zero<glm::vec3>(), const Structures::sColor8& i_color = {255, 255, 255, 255});
					~cLine();

					bool Initialize();
					bool CleanUp();

					bool Render();

					bool SetPosition(int i_index, const glm::vec3& i_position);
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


#ifndef ENGINE_GRAPHICS_ASSETS_DEBUG_CSPHERE_H
#define ENGINE_GRAPHICS_ASSETS_DEBUG_CSPHERE_H

#if defined D3D_API
struct ID3D11Buffer;
#elif defined OGL_API
#include "../../OGL/Includes.h"
#endif

#include "../cDebug.h"
#include "../../Structures/sColor.h"
#include "../../../Math/glm/vec3.hpp"

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
				class cSphere : public cDebug
				{
				public:
					cSphere();
					cSphere(const Structures::sColor8& i_color);
					cSphere(float i_radius, const glm::vec3& i_position, const Structures::sColor8& i_color);
					virtual bool Render();

				private:
					uint16_t m_currentIndex;
					std::map<uint16_t, uint16_t> m_indexList;

					uint16_t AddVertex(glm::vec3& i_position);
					uint16_t MidPointIdx(uint16_t i_idx1, uint16_t i_idx2);
					void SubDivide(int i_iterations);
				};

			}
		}
	}
}

#endif // !ENGINE_GRAPHICS_ASSETS_DEBUG_CSPHERE_H

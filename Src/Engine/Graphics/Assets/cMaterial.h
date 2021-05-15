#ifndef ENGINE_GRAPHICS_ASSETS_CMATERIAL_H
#define ENGINE_GRAPHICS_ASSETS_CMATERIAL_H

#if defined D3D_API 
#include <D3D11.h>
#endif

#include <string>

namespace Engine
{
	namespace Graphics
	{
		namespace Structures 
		{
			struct sMaterial;
		}

		namespace Interfaces
		{
			class cConstantBuffer;
		}

		namespace Assets
		{
			class cEffect;
			class cTexture;
			class cMaterial
			{
			public:
#if defined D3D_API 
				D3D11_BUFFER_DESC m_materialBufferDescription;
				D3D11_SUBRESOURCE_DATA m_initialMaterialData;
#endif
				cMaterial(const std::string& i_materialFileName = "");
				~cMaterial();

				bool Bind();
				bool CleanUp();

				cTexture* m_texture;
			private:
				Structures::sMaterial* m_materialData;
				Interfaces::cConstantBuffer* m_materialBuffer;
				cEffect* m_effect;
			};
		}
			}
		}


#endif // !ENGINE_GRAPHICS_ASSETS_CMATERIAL_H

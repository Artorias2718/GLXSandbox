#ifndef ENGINE_GRAPHICS_ASSETS_CEFFECT_H
#define ENGINE_GRAPHICS_ASSETS_CEFFECT_H

#if defined D3D_API 
struct ID3D11VertexShader;
struct ID3D11PixelShader;
#elif defined OGL_API 
#include "../OGL/Includes.h"
#endif

#include "../Interfaces/cRenderState/cRenderState.h"
#include <string>

namespace Engine
{
	namespace Platform
	{
		struct sDataFromFile;
	}
	namespace Graphics
	{
		namespace Assets
		{
			class cEffect
			{
			public:
				cEffect(const std::string& i_effectName = "");
				~cEffect();

				bool Bind();
				bool CleanUp();

				std::string m_effectName;
				Interfaces::cRenderState m_renderStates;
			private:
				bool LoadShaders();
				bool LoadVertexShader();
				bool LoadFragmentShader();

#if defined D3D_API 
				bool CreateVertexFormat(Platform::sDataFromFile& i_compiledShader);
				ID3D11VertexShader* m_vertexShader;
				ID3D11PixelShader* m_fragmentShader;
#elif defined OGL_API 
				struct sLogInfo
				{
					GLchar* memory;
					sLogInfo(const size_t i_size) { memory = reinterpret_cast<GLchar*>(malloc(i_size)); }
					~sLogInfo() { if (memory) free(memory); }
				};

				GLuint m_programId;
#endif
			};
		}
	}
}

#endif // !ENGINE_GRAPHICS_ASSETS_CEFFECT_H

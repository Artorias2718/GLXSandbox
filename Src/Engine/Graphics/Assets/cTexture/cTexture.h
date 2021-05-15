/*
A texture is an n-dimensional array of data
(usually a 2-dimensional array of colors)
that can be sampled in a shader
*/

#ifndef ENGINE_GRAPHICS_ASSETS_CTEXTURE_CTEXTURE_H
#define ENGINE_GRAPHICS_ASSETS_CTEXTURE_CTEXTURE_H

// Header Files
//=============

#include <cstdint>

#if defined OGL_API 
#include "../../OGL/Includes.h"
#endif

#include <string>

// Forward Declarations
//=====================

namespace Engine
{
	namespace Platform
	{
		struct sDataFromFile;
	}
}
namespace DirectX
{
	struct DDS_HEADER;
}

#ifdef D3D_API 
struct ID3D11ShaderResourceView;
#endif

// Class Declaration
//==================

namespace Engine
{
	namespace Graphics
	{
		namespace Assets
		{
			namespace Texture
			{
				struct sDdsInfo;
			}
			class cTexture
			{
				// Interface
				//==========

			public:

				// Render
				//-------

				// The ID is the HLSL register or GLSL layout ID
				// defined in the shader that uses this texture.
				// The texture doesn't know what this is
				// (a single texture could be used by many different effects)
				// and so this is the caller's responsibility to pass in.
				void Bind(const unsigned int i_id = 0) const;

				// Initialization / Clean Up
				//--------------------------

				bool Load(const std::string& i_fileName);
				bool CleanUp();

				cTexture(const std::string& i_fileName = "");
				~cTexture();

				// Data
				//=====

				uint16_t m_width;
				uint16_t m_height;

			private:

#if defined(D3D_API)
				ID3D11ShaderResourceView* m_textureView;
#elif defined(OGL_API)
				GLuint m_textureId;
#endif

				// Implementation
				//===============

			private:

				// Initialization / Clean Up
				//--------------------------

				bool Initialize(const std::string& i_fileName, const Texture::sDdsInfo& i_ddsInfo);
			};
		}
	}
}
#endif // !ENGINE_GRAPHICS_ASSETS_CTEXTURE_CTEXTURE_H

/*
	A sprite is a quad (two triangle square) drawn in 2D
*/

#ifndef ENGINE_GRAPHICS_INTERFACES_CSPRITE_H
#define ENGINE_GRAPHICS_INTERFACES_CSPRITE_H

// Header Files
//=============

#include "../Configuration.h"
#include <cstdint>

#ifdef D3D_API 
struct ID3D11Buffer;
#elif defined OGL_API 
#include "../OGL/Includes.h"
#endif

// Class Declaration
//==================

#include "../Structures/sRectangle.h"
#include "../Structures/sColor.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Interfaces
		{
			class cSprite
			{
				// Interface
				//==========

			public:

				// Render
				//-------

				void Render() const;

				// Initialization / Clean Up
				//--------------------------

				static bool Initialize();
				static bool CleanUp();

				cSprite(
					const Structures::sRectangleF& i_screenPosition = { -1.0f, 1.0f, 1.0f, -1.0f },
					const Structures::sRectangleF& i_texturePosition = { 0.0f, 1.0f, 1.0f, 0.0f },
					const Structures::sColor8& i_color = { 255, 255, 255, 255 },
					float i_scale = 1.0f
				);
				~cSprite();

				// Data
				//=====

				Structures::sRectangleF m_screenPosition;
				Structures::sRectangleF m_texturePosition;
				Structures::sColor8 m_color;
				float m_scale;

			protected:

				// The positions and texture coordinates are stored as a quad
				// (meaning that only rectangular sprites can be made),
				// and are converted into an Vertex when the draw call is made

				// A single dynamic vertex buffer is created for _all_ sprites,
				// and its contents are changed every draw call
#if defined( D3D_API )
			// A vertex buffer holds the data for each vertex
				static ID3D11Buffer* m_vertexBufferId;
#elif defined( OGL_API )
			// A vertex array encapsulates the vertex data and the vertex input layout
				static GLuint m_vertexArrayId;
				static GLuint m_vertexBufferId;
#endif
			};
		}
	}
}
#endif	// !ENGINE_GRAPHICS_INTERFACES_CSPRITE_H

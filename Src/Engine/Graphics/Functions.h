#ifndef ENGINE_GRAPHICS_FUNCTIONS_H
#define ENGINE_GRAPHICS_FUNCTIONS_H

struct D3D11_INPUT_ELEMENT_DESC;

namespace Engine
{
	namespace Graphics
	{
		namespace Functions
		{
			#if defined D3D_API
				bool CreateVertexFormat(D3D11_INPUT_ELEMENT_DESC* i_layoutDescription);
			#elif defined OGL_API
				bool CreateVertexFormat();
			#endif
		}
	}
}

#endif // !ENGINE_GRAPHICS_FUNCTIONS_H

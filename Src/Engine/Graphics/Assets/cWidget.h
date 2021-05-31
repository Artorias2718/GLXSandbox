#ifndef ENGINE_GRAPHICS_ASSETS_CWIDGET_H
#define ENGINE_GRAPHICS_ASSETS_CWIDGET_H

#if defined D3D_API
struct ID3D11Buffer;
#elif defined OGL_API 
#include "../OGL/Includes.h"
#endif

#include "../Interfaces/cSprite.h"

#include <vector>

namespace Engine
{
	namespace Graphics
	{
		namespace Assets
		{
			class cWidget {
			public:
			private:
			};
		}
	}
}

#endif // !ENGINE_GRAPHICS_ASSETS_CWIDGET_H

#ifndef ENGINE_GRAPHICS_ASSETS_WIDGET_CTEXT_H
#define ENGINE_GRAPHICS_ASSETS_WIDGET_CTEXT_H

#if defined D3D_API 
#include <D3D11.h>
#elif defined OGL_API
#include "../../OGL/Includes.h"
#endif

#include "../cWidget.h"
#include "../../Structures/sColor.h"

#include <string>
#include <map>

namespace Engine
{
	namespace Graphics
	{
		namespace Assets
		{
			class cMaterial;
		}
		namespace Structures
		{
			struct sCharacter;
		}

		namespace Assets
		{
			class cText: public cWidget
			{
			public:
				cText();
				void RenderText(cMaterial* material, std::string &text, float x, float y, float scale, Structures::sColor8& color);

			private:
				std::map<unsigned int, Structures::sCharacter*> m_characters;
			};
		}
	}
}


#endif // !ENGINE_GRAPHICS_ASSETS_WIDGET_CTEXT_H

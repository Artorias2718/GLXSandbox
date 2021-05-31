#include "../../../Assets/Widget/cText.h"
#include "../../Engine/Asserts/Asserts.h"
#include "../../Engine/Logging/Logging.h"
#include "../../Engine/Graphics/Assets/cEffect.h"
#include "../../Engine/Graphics/Assets/cMaterial.h"
#include "../../Engine/Shared/sTransform.h"
#include "../../Engine/Math/cMatrix_Transformation.h"
#include "../../Engine/UserSettings/UserSettings.h"

#include "../../../Structures/sCharacter.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace
{
	unsigned int VAO, VBO;
}

Engine::Graphics::Assets::cText::cText()
{
	Math::cMatrix_Transformation projection = Math::cMatrix_Transformation(0.0f, (float)UserSettings::GetResolutionWidth(), 0.0f, (float)UserSettings::GetResolutionHeight());

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		Engine::Logging::OutputError("Could not init FreeType Library!");
	}

	FT_Face face;
	if(FT_New_Face(ft, "data/fonts/arial.fnt", 0, &face))
	{
		Engine::Logging::OutputError("Failed to load font!");
	}
	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	// disable byte-alignment restriction

	for (unsigned char c = 0; c < 128; ++c)
	{
		// Load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			Engine::Logging::OutputError("Failed to load glyph!");
			continue;
		}

		// Generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Now store character for later use
		Structures::sCharacter character =
		{
			texture,
			Graphics::Structures::sPosition{(float)face->glyph->bitmap.width, (float)face->glyph->bitmap.rows, 0.0f },
			Graphics::Structures::sPosition{(float)face->glyph->bitmap_left, (float)face->glyph->bitmap_top, 0.0f },
			(char)face->glyph->advance.x
		};
		
		m_characters.insert(std::pair<char, Structures::sCharacter*>(c, &character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Engine::Graphics::Assets::cText::RenderText(cMaterial* material, std::string& text, float x, float y, float scale, Structures::sColor8& color)
{
	//material->Bind(false);
	//glUniform3f(glGetUniformLocation(material->m_effect->m_programId, "textColor"), color.r, color.g, color.b);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Structures::sCharacter ch = *m_characters[*c];

		float xpos = x + ch.bearing.x * scale;
		float ypos = y - (ch.size.y - ch.bearing.y) * scale;

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

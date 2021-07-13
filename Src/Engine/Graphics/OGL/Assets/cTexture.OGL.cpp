// Header Files
//=============

#include "../../Assets/cTexture/cTexture.h"

#include <algorithm>
#include "../../Assets/cTexture/Internal.h"
#include "../../../Asserts/Asserts.h"
#include "../../../Logging/Logging.h"

// Interface
//==========

// Render
//-------

void Engine::Graphics::Assets::cTexture::Bind(const unsigned int i_id) const
{
	// Make the texture unit active
	{
		glActiveTexture(GL_TEXTURE0 + static_cast<GLint>(i_id));
		ASSERT(glGetError() == GL_NO_ERROR);
	}
	// Bind the texture to the texture unit
	{
		ASSERT(m_textureId != 0);
		glBindTexture(GL_TEXTURE_2D, m_textureId);
		ASSERT(glGetError() == GL_NO_ERROR);
	}
}

// Initialization / Clean Up
//--------------------------

bool Engine::Graphics::Assets::cTexture::CleanUp()
{
	bool wereThereErrors = false;

	if (m_textureId != 0)
	{
		const GLsizei textureCount = 1;
		glDeleteTextures(textureCount, &m_textureId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Logging::OutputError("OpenGL failed to delete the texture: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
		}
		m_textureId = 0;
	}

	return !wereThereErrors;
}

// Implementation
//===============

// Initialization / Clean Up
//--------------------------

bool Engine::Graphics::Assets::cTexture::Initialize(const std::string& i_fileName, const Texture::sDdsInfo& i_ddsInfo)
{
	bool wereThereErrors = false;

	// Create a new texture and make it active
	{
		const GLsizei textureCount = 1;
		glGenTextures(textureCount, &m_textureId);
		const GLenum errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			if (m_textureId != 0)
			{
				glBindTexture(GL_TEXTURE_2D, m_textureId);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					wereThereErrors = true;
					ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Engine::Logging::OutputError("OpenGL failed to bind the new texture for %s: %s",
						i_fileName, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					goto OnExit;
				}
			}
			else
			{
				bool wereThereErrors = true;
				ASSERT(false);
				Engine::Logging::OutputError("OpenGL failed to create a texture for %s", i_fileName);
				goto OnExit;
			}
		}
		else
		{
			bool wereThereErrors = true;
			ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Engine::Logging::OutputError("OpenGL failed to create a texture for %s: %s",
				i_fileName, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			goto OnExit;
		}
	}
	// Fill in the data for each MIP level
	{
		GLsizei currentWidth = static_cast<GLsizei>(i_ddsInfo.width);
		GLsizei currentHeight = static_cast<GLsizei>(i_ddsInfo.height);
		const uint8_t* currentpCoord = reinterpret_cast<const uint8_t*>(i_ddsInfo.imageData);
		const uint8_t* const endOfFile = currentpCoord + i_ddsInfo.imageDataSize;
		const GLsizei blockSize = static_cast<GLsizei>(Texture::Internal::GetSizeOfBlock(i_ddsInfo.format));
		const GLint borderWidth = 0;
		GLenum glFormat = 0;
		switch (i_ddsInfo.format)
		{
		case Texture::Internal::BC1: glFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT; break;
		case Texture::Internal::BC3: glFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; break;
		}
		for (GLint i = 0; i < static_cast<GLint>(i_ddsInfo.mipLevelCount); ++i)
		{
			// Calculate how much memory this MIP level uses
			const GLsizei blockCount_singleRow = (currentWidth + 3) / 4;
			const GLsizei byteCount_singleRow = blockCount_singleRow * blockSize;
			const GLsizei rowCount = (currentHeight + 3) / 4;
			const GLsizei byteCount_currentMipLevel = byteCount_singleRow * rowCount;
			// Set the data into the texture
			glCompressedTexImage2D(GL_TEXTURE_2D, i, glFormat, currentWidth, currentHeight,
				borderWidth, byteCount_currentMipLevel, currentpCoord);
			const GLenum errorCode = glGetError();
			if (errorCode == GL_NO_ERROR)
			{
				// Update current data for next iteration
				{
					currentpCoord += byteCount_currentMipLevel;
					if (currentpCoord <= endOfFile)
					{
						currentWidth = std::max(currentWidth / 2, 1);
						currentHeight = std::max(currentHeight / 2, 1);
					}
					else
					{
						wereThereErrors = true;
						ASSERTF(false, "DDS file not big enough");
						Logging::OutputError("The DDS file %s isn't big enough to hold the data that its header claims to", i_fileName);
						goto OnExit;
					}

				}
			}
			else
			{
				bool wereThereErrors = true;
				ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Engine::Logging::OutputError("OpenGL failed to copy the texture data from MIP level %u of %s: %s",
					i, i_fileName, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}
		ASSERTF(currentpCoord == endOfFile, "The DDS file %s has more data than it should", i_fileName);
	}

OnExit:

	if (wereThereErrors && (m_textureId != 0))
	{
		const GLsizei textureCount = 1;
		glDeleteTextures(textureCount, &m_textureId);
		ASSERT(glGetError == GL_NO_ERROR);
		m_textureId = 0;
	}

	return !wereThereErrors;
}
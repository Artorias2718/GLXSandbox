// Header Files
//=============

#include "../../Interfaces/cSprite.h"

#include "../../../Asserts/Asserts.h"
#include "../../../Logging/Logging.h"

#include "../../Structures/sVertex.h"
#include "../../Functions.h"

// Static Data Initialization
//===========================

GLuint Engine::Graphics::Interfaces::cSprite::m_vertexArrayId = 0;
GLuint Engine::Graphics::Interfaces::cSprite::m_vertexBufferId = 0;

namespace
{
	// When the topology is a quad OpenGL only needs 4 vertices
	const unsigned int VERTEXCOUNT = 4;
}

// Interface
//==========

// Render
//-------

void Engine::Graphics::Interfaces::cSprite::Render() const
{
	// Update the vertex buffer
	{
		Structures::sVertex vertexData[VERTEXCOUNT];
		{
			// Texture Coordinates
			{
				// Lower Left
				{
					Structures::sVertex& vertex = vertexData[0];
					vertex.texture.u = m_texturePosition.x;
					vertex.texture.v = m_texturePosition.w;
				}
				// Lower Right
				{
					Structures::sVertex& vertex = vertexData[1];
					vertex.texture.u = m_texturePosition.y;
					vertex.texture.v = m_texturePosition.w;
				}
				// Upper Left
				{
					Structures::sVertex& vertex = vertexData[2];
					vertex.texture.u = m_texturePosition.x;
					vertex.texture.v = m_texturePosition.z;
				}
				// Upper Right
				{
					Structures::sVertex& vertex = vertexData[3];
					vertex.texture.u = m_texturePosition.y;
					vertex.texture.v = m_texturePosition.z;
				}
			}
			// Position
			{
				// Lower Left
				{
					Structures::sVertex& vertex = vertexData[0];
					vertex.position.x = m_scale * m_screenPosition.x;
					vertex.position.y = m_scale * m_screenPosition.w;
				}
				// Lower Right
				{
					Structures::sVertex& vertex = vertexData[1];
					vertex.position.x = m_scale * m_screenPosition.y;
					vertex.position.y = m_scale * m_screenPosition.w;
				}
				// Upper Left
				{
					Structures::sVertex& vertex = vertexData[2];
					vertex.position.x = m_scale * m_screenPosition.x;
					vertex.position.y = m_scale * m_screenPosition.z;
				}
				// Upper Right
				{
					Structures::sVertex& vertex = vertexData[3];
					vertex.position.x = m_scale * m_screenPosition.y;
					vertex.position.y = m_scale * m_screenPosition.z;
				}
			}

			// Colors
			for (unsigned int i = 0; i < VERTEXCOUNT; ++i)
			{
				Structures::sVertex& vertex = vertexData[i];
				vertex.color.r = m_color.r;
				vertex.color.g = m_color.g;
				vertex.color.b = m_color.b;
				vertex.color.a = m_color.a;
			}
		}
		// Make the vertex buffer active
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
			ASSERT(glGetError() == GL_NO_ERROR);
		}
		// Invalidate the old data
		{
			// This is to tell OpenGL that synchronization isn't necessary
			// (it can finish drawing with the contents of the previous buffer,
			// but there's no need to wait for the next update before drawing anything else)
			glInvalidateBufferData(m_vertexBufferId);
			ASSERT(glGetError() == GL_NO_ERROR);
		}
		// Re-allocate and copy the new data to the GPU
		{
			glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(vertexData)), vertexData, GL_STREAM_DRAW);
			ASSERT(glGetError() == GL_NO_ERROR);
		}
	}
	// Bind the vertex buffer to the device as a data source
	{
		glBindVertexArray(m_vertexArrayId);
		ASSERT(glGetError() == GL_NO_ERROR);
	}
	// Render triangles from the currently-bound vertex buffer
	{
		// The mode defines how to interpret multiple vertices as a single "primitive";
		// we have defined the vertex buffer as a quad, which means that a triangle "strip" will work
		// (meaning that the first primitive will be a triangle defined by three vertices
		// and the second primitive will be a triangle defined by the two most recent vertices and one new vertex)
		const GLenum mode = GL_TRIANGLE_STRIP;
		// It's possible to start rendering primitives in the middle of the stream
		const GLint indexOfFirstVertexToRender = 0;
		glDrawArrays(mode, indexOfFirstVertexToRender, static_cast<GLsizei>(VERTEXCOUNT));
		ASSERT(glGetError() == GL_NO_ERROR);
	}
}

// Initialization / Clean Up
//--------------------------

bool Engine::Graphics::Interfaces::cSprite::Initialize()
{
	bool wereThereErrors = false;

	// Store the vertex data in a vertex buffer
	{
		// Create a vertex array object and make it active
		{
			const GLsizei arrayCount = 1;
			glGenVertexArrays(arrayCount, &m_vertexArrayId);
			const GLenum errorCode = glGetError();
			if (errorCode == GL_NO_ERROR)
			{
				glBindVertexArray(m_vertexArrayId);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					wereThereErrors = true;
					ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Logging::OutputError("OpenGL failed to bind the sprites' new vertex array: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					goto OnExit;
				}
			}
			else
			{
				wereThereErrors = true;
				ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Logging::OutputError("OpenGL failed to get an unused vertex array ID for sprites: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}

		// Create a vertex buffer object and make it active
		{
			const GLsizei bufferCount = 1;
			glGenBuffers(bufferCount, &m_vertexBufferId);
			const GLenum errorCode = glGetError();
			if (errorCode == GL_NO_ERROR)
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					wereThereErrors = true;
					ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Logging::OutputError("OpenGL failed to bind the sprites' new vertex buffer: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					goto OnExit;
				}
			}
			else
			{
				wereThereErrors = true;
				ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Logging::OutputError("OpenGL failed to get an unused vertex buffer ID for the sprites: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}
		// Assign initial data to the buffer
		{
			Structures::sVertex vertexData[VERTEXCOUNT];
			{
				// Lower Left
				{
					Structures::sVertex& vertex = vertexData[0];
					vertex.texture.u = 0.0f;
					vertex.texture.v = 0.0f;
					vertex.position.x = -1.0f;
					vertex.position.y = -1.0f;
				}
				// Lower Right
				{
					Structures::sVertex& vertex = vertexData[1];
					vertex.texture.u = 1.0f;
					vertex.texture.v = 0.0f;
					vertex.position.x = 1.0f;
					vertex.position.y = -1.0f;
				}
				// Upper Left
				{
					Structures::sVertex& vertex = vertexData[2];
					vertex.texture.u = 0.0f;
					vertex.texture.v = 1.0f;
					vertex.position.x = -1.0f;
					vertex.position.y = 1.0f;
				}
				// Upper Right
				{
					Structures::sVertex& vertex = vertexData[3];
					vertex.texture.u = 1.0f;
					vertex.texture.v = 1.0f;
					vertex.position.x = 1.0f;
					vertex.position.y = 1.0f;
				}

				// Colors
				for (uint16_t i = 0; i < VERTEXCOUNT; ++i)
				{
					Structures::sVertex& vertex = vertexData[i];
					vertex.color.r = vertex.color.g = vertex.color.b = vertex.color.a = 255;
				}
			}
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), reinterpret_cast<const GLvoid*>(vertexData),
				// The buffer will change frequently, and each update will only be used once for a draw call
				GL_STREAM_DRAW);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Logging::OutputError("OpenGL failed to allocate the sprites' vertex buffer: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}

		// Initialize the vertex format
		Functions::CreateVertexFormat();
	}

OnExit:

	return !wereThereErrors;
}

bool Engine::Graphics::Interfaces::cSprite::CleanUp()
{
	bool wereThereErrors = false;

	// Make sure that the vertex array isn't bound
	{
		// Unbind the vertex array
		glBindVertexArray(0);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Logging::OutputError("OpenGL failed to unbind all vertex arrays before deleting cleaning up the sprites: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
		}
	}
	if (m_vertexBufferId != 0)
	{
		const GLsizei bufferCount = 1;
		glDeleteBuffers(bufferCount, &m_vertexBufferId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Logging::OutputError("OpenGL failed to delete the sprites' vertex buffer: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
		}
		m_vertexBufferId = 0;
	}
	if (m_vertexArrayId != 0)
	{
		const GLsizei arrayCount = 1;
		glDeleteVertexArrays(arrayCount, &m_vertexArrayId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Logging::OutputError("OpenGL failed to delete the sprites' vertex array: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
		}
		m_vertexArrayId = 0;
	}

	return !wereThereErrors;
}

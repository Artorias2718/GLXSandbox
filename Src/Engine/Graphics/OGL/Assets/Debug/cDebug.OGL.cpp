#include "../../../Assets/Debug/cLine.h"

#include "../../Includes.h"
#include "../../../../Asserts/Asserts.h"
#include "../../../../Logging/Logging.h"
#include "../../../../Math/Functions.h"
#include "../../../Functions.h"
#include "../../../Structures/sVertex.h"


bool Engine::Graphics::Assets::cDebug::Initialize()
{
	bool wereThereErrors = false;

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
				Engine::Logging::OutputError("OpenGL failed to bind the vertex array: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Engine::Logging::OutputError("OpenGL failed to get an unused vertex array ID: %s",
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
				Engine::Logging::OutputError("OpenGL failed to bind the vertex buffer: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Engine::Logging::OutputError("OpenGL failed to get an unused vertex buffer ID: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			goto OnExit;
		}
	}

	// Assign the data to the buffer
	{
		// Eventually the vertex data should come from a file but for now it is hard-coded here.
		// You will have to update this in a future assignment
		// (one of the most common mistakes in the class is to leave hard-coded values here).

		const unsigned int bufferSize = m_vertexCount * sizeof(Structures::sVertex);

		glBufferData(GL_ARRAY_BUFFER, bufferSize, reinterpret_cast<GLvoid*>(&m_vertices[0]),
			// In our class we won't ever read from the buffer
			GL_STATIC_DRAW);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Engine::Logging::OutputError("OpenGL failed to allocate the vertex buffer: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			goto OnExit;
		}
	}

	// Initialize the vertex format
	Functions::CreateVertexFormat();

	// Create a index buffer object and make it active
	{
		const GLsizei bufferCount = 1;
		glGenBuffers(bufferCount, &m_indexBufferId);
		const GLenum errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Engine::Logging::OutputError("OpenGL failed to bind the index buffer: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Engine::Logging::OutputError("OpenGL failed to get an unused index buffer ID: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			goto OnExit;
		}
	}
	// Assign the data to the buffer
	{
		const unsigned int bufferSize = m_indexSetCount * sizeof(Engine::Graphics::Structures::sIndexSet16);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, reinterpret_cast<GLvoid*>(m_indexSet16),
			// In our class we won't ever read from the buffer
			GL_STATIC_DRAW);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Engine::Logging::OutputError("OpenGL failed to allocate the index buffer: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			goto OnExit;
		}
	}

OnExit:

	if (m_vertexArrayId != 0)
	{
		// Unbind the vertex array
		// (this must be done before deleting the vertex buffer)
		glBindVertexArray(0);
		const GLenum errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			// The vertex and index buffer objects can be freed
			// (the vertex array object will still hold references to them,
			// and so they won't actually goes away until it gets freed).
			// Unfortunately debuggers don't work well when these are freed
			// (gDEBugger just doesn't show anything and RenderDoc crashes),
			// and so don't free them if debug info is enabled.
			if (m_vertexBufferId != 0)
			{
#ifndef GRAPHICS_ISDEVICEDEBUGINFOENABLED
				const GLsizei bufferCount = 1;
				glDeleteBuffers(bufferCount, &m_vertexBufferId);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					wereThereErrors = true;
					ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Engine::Logging::OutputError("OpenGL failed to vertex buffer: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					goto OnExit;
				}
#else
				m_vertexBufferId = 0;
#endif
			}


			if (m_indexBufferId != 0)
			{
#ifndef GRAPHICS_ISDEVICEDEBUGINFOENABLED
				const GLsizei bufferCount = 1;
				glDeleteBuffers(bufferCount, &m_indexBufferId);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					wereThereErrors = true;
					ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Engine::Logging::OutputError("OpenGL failed to unbind the index buffer: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					goto OnExit;
				}
#else
				m_indexBufferId = 0;
#endif
			}
		}
	}

	return !wereThereErrors;
}

bool Engine::Graphics::Assets::cDebug::Render()
{
	// Bind a specific vertex buffer to the device as a data source
	{
		glBindVertexArray(m_vertexArrayId);
		ASSERT(glGetError() == GL_NO_ERROR);
	}
	// Render triangles from the currently-bound vertex buffer
	{
		// The mode defines how to interpret multiple vertices as a single "primitive";
		// we define a triangle list
		// (meaning that every primitive is a triangle and will be defined by three vertices)
		const GLenum mode = GL_TRIANGLES;
		// Every index is a 16-bit (or 32-bit) unsigned integer
		const GLenum indexType = GL_UNSIGNED_SHORT;
		// It's possible to start rendering primitives in the middle of the stream
		const GLint indexOfFirstVertexToRender = 0;
		const GLvoid* const offset = 0;
		// There are three indices per triangle
		const unsigned int indicesPerTriangle = 3;
		glDrawElements(mode, indicesPerTriangle * m_indexSetCount, indexType, offset);
		ASSERT(glGetError() == GL_NO_ERROR);
	}
	return true;
}


bool Engine::Graphics::Assets::cDebug::CleanUp()
{
	bool wereThereErrors = false;
	if (m_vertexArrayId != 0)
	{
		const GLsizei arrayCount = 1;
		glDeleteVertexArrays(arrayCount, &m_vertexArrayId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Logging::OutputError("OpenGL failed to delete the vertex array: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
		}
		m_vertexArrayId = 0;

#ifdef GRAPHICS_ISDEVICEDEBUGINFOENABLED
		if (m_vertexBufferId != 0)
		{
			const GLsizei bufferCount = 1;
			glDeleteBuffers(bufferCount, &m_vertexBufferId);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Logging::OutputError("OpenGL failed to delete the vertex buffer: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
			}
			m_vertexBufferId = 0;
		}
#endif
	}

#ifdef GRAPHICS_ISDEVICEDEBUGINFOENABLED
	if (m_vertexBufferId != 0)
	{
		const GLsizei bufferCount = 1;
		glDeleteBuffers(bufferCount, &m_vertexBufferId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Logging::OutputError("OpenGL failed to delete the vertex buffer: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
		}
		m_vertexBufferId = 0;
	}

#endif

	return !wereThereErrors;
}
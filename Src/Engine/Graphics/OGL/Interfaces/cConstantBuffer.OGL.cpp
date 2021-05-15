#include "../../Interfaces/cConstantBuffer.h"
#include "../../../Asserts/Asserts.h"
#include "../../../Logging/Logging.h"

#include "../../Structures/sFrame.h"
#include "../../Structures/sDrawCall.h"

Engine::Graphics::Interfaces::cConstantBuffer::cConstantBuffer(eConstantBufferType i_bufferType, size_t i_bufferSize, void* i_constantBufferData)
{
	if (!Initialize(i_bufferType, i_bufferSize, i_constantBufferData))
	{
		switch (i_bufferType)
		{
		case FRAME:
			ASSERTF(false, "There was a problem initializing the frame constant buffer!");
			break;
		case DRAWCALL:
			ASSERTF(false, "There was a problem initializing the drawCall constant buffer!");
			break;
		case MATERIAL:
			ASSERTF(false, "There was a problem initializing the material constant buffer!");
			break;
		}
	}
}

bool Engine::Graphics::Interfaces::cConstantBuffer::Initialize(eConstantBufferType i_bufferType, size_t i_bufferSize, void* i_constantBufferData)
{
	bool wereThereErrors = false;
	{
		// Create a uniform buffer object and make it active
		{
			const GLuint bufferCount = 1;
			glGenBuffers(bufferCount, &m_constantBufferId);
			const GLenum errorCode = glGetError();
			if (errorCode == GL_NO_ERROR)
			{
				glBindBuffer(GL_UNIFORM_BUFFER, m_constantBufferId);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					wereThereErrors = true;
					ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					Logging::OutputError("OpenGL failed to bind the new uniform buffer %u: %s",
						m_constantBufferId, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				}
			}
			else
			{
				wereThereErrors = true;
				ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Logging::OutputError("OpenGL failed to get an unused uniform buffer ID: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
			}
		}
		// Fill in the constant buffer
		// Allocate space and copy the constant data into the uniform buffer
		{
			const GLenum usage = GL_DYNAMIC_DRAW;	// The buffer will be modified frequently and used to draw

			switch (i_bufferType)
			{
			case FRAME:
			case MATERIAL:
				glBufferData(GL_UNIFORM_BUFFER, i_bufferSize, i_constantBufferData, usage);
				break;
			case DRAWCALL:
				glBufferData(GL_UNIFORM_BUFFER, i_bufferSize, NULL, usage);
				break;
			}

			glBindBuffer(GL_UNIFORM_BUFFER, m_constantBufferId);
			const GLenum errorCode = glGetError();

			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Logging::OutputError("OpenGL failed to allocate the new uniform buffer %u: %s",
					m_constantBufferId, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}
	}
OnExit:
	return !wereThereErrors;
}

bool Engine::Graphics::Interfaces::cConstantBuffer::Bind(eConstantBufferType i_bufferType)
{
	// Update the constant buffer
	{
		// Bind the constant buffers to the shader
		glBindBufferBase(GL_UNIFORM_BUFFER, i_bufferType, m_constantBufferId);
		ASSERT(glGetError() == GL_NO_ERROR);
	}
	return true;
}

bool Engine::Graphics::Interfaces::cConstantBuffer::Update(eConstantBufferType i_bufferType, void* i_constantBufferData)
{
	// Update the struct (i.e. the memory that we own)

	// Make the uniform buffer active
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_constantBufferId);
		ASSERT(glGetError() == GL_NO_ERROR);
	}
	// Copy the updated memory to the GPU
	{
		GLintptr updateAtTheBeginning = 0;
		GLsizeiptr updateTheEntireBuffer = 0;

		switch (i_bufferType)
		{
		case FRAME:
			updateTheEntireBuffer = static_cast<GLsizeiptr>(sizeof(Structures::sFrame));
			break;
		case DRAWCALL:
			updateTheEntireBuffer = static_cast<GLsizeiptr>(sizeof(Structures::sDrawCall));
			break;
		}

		glBufferSubData(GL_UNIFORM_BUFFER, updateAtTheBeginning, updateTheEntireBuffer, i_constantBufferData);
		ASSERT(glGetError() == GL_NO_ERROR);
	}
	return true;
}

bool Engine::Graphics::Interfaces::cConstantBuffer::CleanUp()
{
	bool wereThereErrors = false;

	if (m_constantBufferId != 0)
	{
		const GLsizei bufferCount = 1;
		glDeleteBuffers(bufferCount, &m_constantBufferId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Logging::OutputError("OpenGL failed to delete the constant buffer: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
		}
		m_constantBufferId = 0;
	}
	return !wereThereErrors;
}
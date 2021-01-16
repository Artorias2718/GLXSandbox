#include "Functions.h"
#include "../Asserts/Asserts.h"
#include "../Logging/Logging.h"
#include "Structures/sVertex.h"

#if defined D3D_API
#include "D3D/Includes.h"
#elif defined OGL_API
#include "OGL/Includes.h"
#endif

#if defined D3D_API 
bool Engine::Graphics::Functions::CreateVertexFormat(D3D11_INPUT_ELEMENT_DESC* i_layoutDescription)
#elif defined OGL_API
bool Engine::Graphics::Functions::CreateVertexFormat()
#endif
{
	bool wereThereErrors = false;
#if defined D3D_API
	{
		// Slot 0

		// POSITION
		// 2 floats == 8 bytes
		// Offset = 0
		{
			D3D11_INPUT_ELEMENT_DESC& positionElement = i_layoutDescription[0];

			positionElement.SemanticName = "POSITION";
			positionElement.SemanticIndex = 0;	// (Semantics without modifying indices at the end can always use zero)
			positionElement.Format = DXGI_FORMAT_R32G32_FLOAT;
			positionElement.InputSlot = 0;
			positionElement.AlignedByteOffset = offsetof(Engine::Graphics::Structures::sVertex, position.x);
			positionElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			positionElement.InstanceDataStepRate = 0;	// (Must be zero for per-vertex data)
		}
	}
	{
		// Slot 1 

		// COLOR 
		// 4 unsigned 8-bit integers == 4 bytes
		// Offset = 1 
		{
			D3D11_INPUT_ELEMENT_DESC& colorElement = i_layoutDescription[1];

			colorElement.SemanticName = "COLOR";
			colorElement.SemanticIndex = 0;	// (Semantics without modifying indices at the end can always use zero)
			colorElement.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			//colorElement.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			colorElement.InputSlot = 0;
			colorElement.AlignedByteOffset = offsetof(Engine::Graphics::Structures::sVertex, color.r);
			colorElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			colorElement.InstanceDataStepRate = 0;	// (Must be zero for per-vertex data)
		}
	}
#elif defined OGL_API
	// The "stride" defines how large a single vertex is in the stream of data
	// (or, said another way, how far apart each position element is)
	const GLsizei stride = sizeof(Engine::Graphics::Structures::sVertex);

	// Position (0)
	// 3 floats == 12 bytes
	// Offset = 0
	{
		const GLuint vertexElementLocation = 0;
		const GLint elementCount = 2;
		const GLboolean notNormalized = GL_FALSE;	// The given floats should be used as-is
		glVertexAttribPointer(vertexElementLocation, elementCount, GL_FLOAT, notNormalized, stride,
			reinterpret_cast<GLvoid*>(offsetof(Engine::Graphics::Structures::sVertex, position.x)));
		const GLenum errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			glEnableVertexAttribArray(vertexElementLocation);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Engine::Logging::OutputError("OpenGL failed to enable the POSITION vertex attribute at location %u: %s",
					vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Engine::Logging::OutputError("OpenGL failed to set the POSITION vertex attribute at location %u: %s",
				vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			goto OnExit;
		}
	}
	// Color (1)
// 4 unsigned 8-bit integers == 32 bytes
// Offset = 1
	{
		const GLuint vertexElementLocation = 1;
		const GLint elementCount = 4;
		const GLboolean notNormalized = GL_TRUE;	// The given integers should be used as-is
		glVertexAttribPointer(vertexElementLocation, elementCount, GL_UNSIGNED_BYTE, notNormalized, stride,
			reinterpret_cast<GLvoid*>(offsetof(Engine::Graphics::Structures::sVertex, color.r)));
		const GLenum errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			glEnableVertexAttribArray(vertexElementLocation);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Engine::Logging::OutputError("OpenGL failed to enable the COLOR vertex attribute at location %u: %s",
					vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Engine::Logging::OutputError("OpenGL failed to set the COLOR vertex attribute at location %u: %s",
				vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			goto OnExit;
		}
	}
	OnExit:
#endif
	return !wereThereErrors;
}


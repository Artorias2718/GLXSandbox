#include "../../../Assets/Debug/cCapsule.h"
#include "../../../Assets/Debug/cSphere.h"

#include "../../Includes.h"
#include "../../../../Asserts/Asserts.h"
#include "../../../../Logging/Logging.h"
#include "../../../../Math/Functions.h"
#include "../../../Functions.h"
#include "../../../Structures/sVertex.h"

bool Engine::Graphics::Assets::Debug::cCapsule::Render()
{
	m_top->Render();

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
		// It's possible to start rendering primitives in the middle of the stream

		const GLenum indexType = GL_UNSIGNED_SHORT;
		const GLvoid* const offset = 0;
		glDrawElements(mode, m_indexSetCount * sizeof(Structures::sIndexSet16), indexType, offset);

		ASSERT(glGetError() == GL_NO_ERROR);
	}

	m_bottom->Render();
	return true;
}

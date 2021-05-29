#include "cLine.h"

#include "../../../Asserts/Asserts.h"
#include "../../../Logging/Logging.h"
#include "../../../Math/cVector.h"
#include "../../Structures/sVertex.h"
#include "../../Structures/sColor.h"

Engine::Graphics::Assets::Debug::cLine::cLine(const Engine::Math::cVector &i_startPosition, const Engine::Math::cVector &i_endPosition, const Engine::Graphics::Structures::sColor8& i_color)
{
	const int verticesPerLine = 2;
	for (int i = 0; i < verticesPerLine; ++i)
	{
		m_vertices.push_back(Structures::sVertex());
		m_vertexCount++;
	}
#if defined OGL_API 
	Initialize();
#endif

	SetPosition(0, i_startPosition);
	SetColor(0, i_color);
	SetPosition(1, i_endPosition);
	SetColor(1, i_color);
}

Engine::Graphics::Assets::Debug::cLine::~cLine()
{
	CleanUp();
}
bool Engine::Graphics::Assets::Debug::cLine::SetPosition(int i_idx, const Math::cVector&i_position)
{
	m_vertices[i_idx].position.x = i_position.x;
	m_vertices[i_idx].position.y = i_position.y;
	m_vertices[i_idx].position.z = i_position.z;

#if defined D3D_API 
	Initialize();
#elif defined OGL_API
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	ASSERT(glGetError() == GL_NO_ERROR);

	const unsigned int bufferSize = m_vertexCount * sizeof(Structures::sVertex);
	glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(bufferSize), &m_vertices[0]);
	ASSERT(glGetError() == GL_NO_ERROR);
#endif

	return true;
}

bool Engine::Graphics::Assets::Debug::cLine::SetColor(int i_idx, const Structures::sColor8& i_color)
{
	m_vertices[i_idx].color.r = i_color.r;
	m_vertices[i_idx].color.g = i_color.g;
	m_vertices[i_idx].color.b = i_color.b;
	m_vertices[i_idx].color.a = i_color.a;

#if defined D3D_API 
	Initialize();
#elif defined OGL_API
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	ASSERT(glGetError() == GL_NO_ERROR);

	const unsigned int bufferSize = m_vertexCount * sizeof(Structures::sVertex);
	glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(bufferSize), &m_vertices[0]);
	ASSERT(glGetError() == GL_NO_ERROR);
#endif

	return true;
}

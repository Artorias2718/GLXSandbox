#include "cBox.h"

#include "../../../Asserts/Asserts.h"
#include "../../../Logging/Logging.h"
#include "../../../Math/glm/vec3.hpp"
#include "../../Structures/sVertex.h"
#include "../../Structures/sColor.h"

Engine::Graphics::Assets::Debug::cBox::cBox(const Engine::Graphics::Structures::sColor8& i_color)
{
	const int verticesPerBox = 8;
	const int indicesPerBox = 48;

	for (int i = 0; i < verticesPerBox; ++i)
	{
		m_vertices.push_back(Structures::sVertex());
		m_vertices[i].color.r = i_color.r;
		m_vertices[i].color.g = i_color.g;
		m_vertices[i].color.b = i_color.b;
		m_vertices[i].color.a = i_color.a;
		m_vertexCount++;
	}

	for (int i = 0; i < indicesPerBox; ++i)
	{
		m_indices16.push_back(0);
		m_indexSetCount++;
	}

	Structures::sPosition extents = Structures::sPosition{ 0.5f, 0.5f, 0.5f };
	m_vertices[0].position.x = -extents.x;
	m_vertices[0].position.y = extents.y;
	m_vertices[0].position.z = extents.z;

	m_vertices[1].position.x = -extents.x;
	m_vertices[1].position.y = -extents.y;
	m_vertices[1].position.z = extents.z;

	m_vertices[2].position.x = extents.x;
	m_vertices[2].position.y = -extents.y;
	m_vertices[2].position.z = extents.z;

	m_vertices[3].position.x = extents.x;
	m_vertices[3].position.y = extents.y;
	m_vertices[3].position.z = extents.z;

	m_vertices[4].position.x = -extents.x;
	m_vertices[4].position.y = extents.y;
	m_vertices[4].position.z = -extents.z;

	m_vertices[5].position.x = -extents.x;
	m_vertices[5].position.y = -extents.y;
	m_vertices[5].position.z = -extents.z;

	m_vertices[6].position.x = extents.x;
	m_vertices[6].position.y = -extents.y;
	m_vertices[6].position.z = -extents.z;

	m_vertices[7].position.x = extents.x;
	m_vertices[7].position.y = extents.y;
	m_vertices[7].position.z = -extents.z;

#if defined D3D_API
	// Front
	m_indices16[0] = 1;
	m_indices16[1] = 0;
	m_indices16[2] = 2;
	m_indices16[3] = 1;
	m_indices16[4] = 3;
	m_indices16[5] = 2;
	m_indices16[6] = 0;
	m_indices16[7] = 3;

	// Back
	m_indices16[8] = 5;
	m_indices16[9] = 4;
	m_indices16[10] = 6;
	m_indices16[11] = 5;
	m_indices16[12] = 7;
	m_indices16[13] = 6;
	m_indices16[14] = 4;
	m_indices16[15] = 7;
	
	// Top
	m_indices16[16] = 2;
	m_indices16[17] = 3;
	m_indices16[18] = 6;
	m_indices16[19] = 2;
	m_indices16[20] = 7;
	m_indices16[21] = 6;
	m_indices16[22] = 3;
	m_indices16[23] = 7;
	
	// Bottom
	m_indices16[24] = 1;
	m_indices16[25] = 0;
	m_indices16[26] = 5;
	m_indices16[27] = 1;
	m_indices16[28] = 4;
	m_indices16[29] = 5;
	m_indices16[30] = 0;
	m_indices16[31] = 4;
	
	// Left
	m_indices16[32] = 4;
	m_indices16[33] = 0;
	m_indices16[34] = 7;
	m_indices16[35] = 4;
	m_indices16[36] = 3;
	m_indices16[37] = 7;
	m_indices16[38] = 0;
	m_indices16[39] = 3;
	
	// Right
	m_indices16[40] = 5;
	m_indices16[41] = 1;
	m_indices16[42] = 6;
	m_indices16[43] = 5;
	m_indices16[44] = 2;
	m_indices16[45] = 6;
	m_indices16[46] = 1;
	m_indices16[47] = 2;
#elif defined OGL_API 
	// Front
	m_indices16[0] = 1;
	m_indices16[1] = 2;
	m_indices16[2] = 0;
	m_indices16[3] = 1;
	m_indices16[4] = 2;
	m_indices16[5] = 3;
	m_indices16[6] = 0;
	m_indices16[7] = 3;

	// Back
	m_indices16[8] = 5;
	m_indices16[9] = 6;
	m_indices16[10] = 4;
	m_indices16[11] = 5;
	m_indices16[12] = 6;
	m_indices16[13] = 7;
	m_indices16[14] = 4;
	m_indices16[15] = 7;

	// Top
	m_indices16[16] = 2;
	m_indices16[17] = 6;
	m_indices16[18] = 3;
	m_indices16[19] = 2;
	m_indices16[20] = 6;
	m_indices16[21] = 7;
	m_indices16[22] = 3;
	m_indices16[23] = 7;

	// Bottom
	m_indices16[24] = 1;
	m_indices16[25] = 5;
	m_indices16[26] = 0;
	m_indices16[27] = 1;
	m_indices16[28] = 5;
	m_indices16[29] = 4;
	m_indices16[30] = 0;
	m_indices16[31] = 4;

	// Left
	m_indices16[32] = 4;
	m_indices16[33] = 7;
	m_indices16[34] = 0;
	m_indices16[35] = 4;
	m_indices16[36] = 7;
	m_indices16[37] = 3;
	m_indices16[38] = 0;
	m_indices16[39] = 3;

	// Right
	m_indices16[40] = 5;
	m_indices16[41] = 6;
	m_indices16[42] = 1;
	m_indices16[43] = 5;
	m_indices16[44] = 6;
	m_indices16[45] = 2;
	m_indices16[46] = 1;
	m_indices16[47] = 2;
#endif
	Initialize();
}

Engine::Graphics::Assets::Debug::cBox::~cBox()
{
	CleanUp();
}

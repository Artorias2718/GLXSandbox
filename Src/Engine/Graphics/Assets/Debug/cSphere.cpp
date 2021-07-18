#include "cSphere.h"

#include "../../../Asserts/Asserts.h"
#include "../../../Logging/Logging.h"
#include "../../../Math/glm/vec3.hpp"
#include "../../../Math/glm/gtc/matrix_transform.hpp"
#include "../../Structures/sVertex.h"
#include "../../Structures/sColor.h"

Engine::Graphics::Assets::Debug::cSphere::cSphere()
{
	const float x = (float)sqrt((5.0f - sqrt(5.0f)) / 10.0f);
	const float z = (float)sqrt((5.0f + sqrt(5.0f)) / 10.0f);

	const float n = 0.0f;

	const int initVertexCount = 20;

	for (int i = 0; i < initVertexCount; ++i)
	{
		m_indexSet16.push_back(Structures::sIndexSet16{});
		m_indexSetCount++;
	}

	AddVertex(glm::vec3(-x, z, n));
	AddVertex(glm::vec3(x, z, n));
	AddVertex(glm::vec3(-x, -z, n));
	AddVertex(glm::vec3(x, -z, n));

	AddVertex(glm::vec3(n, -x, z));
	AddVertex(glm::vec3(n, x, z));
	AddVertex(glm::vec3(n, -x, -z));
	AddVertex(glm::vec3(n, x, -z));

	AddVertex(glm::vec3(z, n, -x));
	AddVertex(glm::vec3(z, n, x));
	AddVertex(glm::vec3(-z, n, -x));
	AddVertex(glm::vec3(-z, n, x));

	// 5 faces around point 0
	m_indexSet16[0] = { 0, 11, 5 };
	m_indexSet16[1] = { 0, 5, 1 };
	m_indexSet16[2] = { 0, 1, 7 };
	m_indexSet16[3] = { 0, 7, 10 };
	m_indexSet16[4] = { 0, 10, 11 };

	// 5 adjacent faces 
	m_indexSet16[5] = { 1, 5, 9 };
	m_indexSet16[6] = { 5, 11, 4 };
	m_indexSet16[7] = { 11, 10, 2 };
	m_indexSet16[8] = { 10, 7, 6 };
	m_indexSet16[9] = { 7, 1, 8 };

	// 5 faces around point 3
	m_indexSet16[10] = { 3, 9, 4 };
	m_indexSet16[11] = { 3, 4, 2 };
	m_indexSet16[12] = { 3, 2, 6 };
	m_indexSet16[13] = { 3, 6, 8 };
	m_indexSet16[14] = { 3, 8, 9 };

	// 5 adjacent faces 
	m_indexSet16[15] = { 4, 9, 5 };
	m_indexSet16[16] = { 2, 4, 11 };
	m_indexSet16[17] = { 6, 2, 10 };
	m_indexSet16[18] = { 8, 6, 7 };
	m_indexSet16[19] = { 9, 8, 1 };

	SubDivide(3);
}

Engine::Graphics::Assets::Debug::cSphere::cSphere(const Structures::sColor8& i_color) : cSphere()
{
	for (uint16_t i = 0; i < m_vertexCount; ++i)
	{
		m_vertices[i].color.r = i_color.r;
		m_vertices[i].color.g = i_color.g;
		m_vertices[i].color.b = i_color.b;
		m_vertices[i].color.a = i_color.a;
	}

	Initialize();
}

Engine::Graphics::Assets::Debug::cSphere::cSphere(float i_radius, const glm::vec3& i_position, const Structures::sColor8& i_color) : cSphere()
{
	for (uint16_t i = 0; i < m_vertexCount; ++i)
	{
		m_vertices[i].position.x *= i_radius;
		m_vertices[i].position.y *= i_radius;
		m_vertices[i].position.z *= i_radius;
	}

	for (uint16_t i = 0; i < m_vertexCount; ++i)
	{
		m_vertices[i].position.x += i_position.x;
		m_vertices[i].position.y += i_position.y;
		m_vertices[i].position.z += i_position.z;
	}

	for (uint16_t i = 0; i < m_vertexCount; ++i)
	{
		m_vertices[i].color.r = i_color.r;
		m_vertices[i].color.g = i_color.g;
		m_vertices[i].color.b = i_color.b;
		m_vertices[i].color.a = i_color.a;
	}

	Initialize();
}


uint16_t Engine::Graphics::Assets::Debug::cSphere::AddVertex(glm::vec3& i_position)
{
	glm::vec3 unitPosition = glm::normalize(i_position);
	Structures::sVertex unitVertex;

	unitVertex.position.x = unitPosition.x;
	unitVertex.position.y = unitPosition.y;
	unitVertex.position.z = unitPosition.z;

	m_vertices.push_back(unitVertex);
	m_vertexCount++;

	return m_currentIndex++;
}

uint16_t Engine::Graphics::Assets::Debug::cSphere::MidPointIdx(uint16_t i_idx1, uint16_t i_idx2)
{
	// First check if we have it already

	bool firstIsSmaller = i_idx1 < i_idx2;
	uint16_t smallerIndex = firstIsSmaller ? i_idx1 : i_idx2;
	uint16_t greaterIndex = firstIsSmaller ? i_idx2 : i_idx1;
	uint16_t key = (smallerIndex << 8) + greaterIndex;

	std::map<uint16_t, uint16_t>::iterator indexIterator = m_indexList.find(key);

	if (indexIterator != m_indexList.end())
	{
		return indexIterator->second;
	}

	// Not in cache, calculate it
	Structures::sVertex point1 = m_vertices[i_idx1];
	Structures::sVertex point2 = m_vertices[i_idx2];

	glm::vec3 midpoint = 0.5f * glm::vec3(point1.position.x + point2.position.x, point1.position.y + point2.position.y, point1.position.z + point2.position.z);

	// Add vertex makes sure point is on unit cSphere
	uint16_t newIndex = AddVertex(midpoint);

	// Store it, return index
	m_indexList.insert(std::make_pair(key, newIndex));
	return newIndex;
}

void Engine::Graphics::Assets::Debug::cSphere::SubDivide(int i_iterations)
{
	std::vector<Structures::sIndexSet16> initialTriangleSet = m_indexSet16;
	for (int i = 0; i < i_iterations; ++i)
	{
		std::vector<Structures::sIndexSet16> newTriangleData;

		for (auto tri : initialTriangleSet)
		{
			uint16_t a = MidPointIdx(tri.a, tri.b);
			uint16_t b = MidPointIdx(tri.b, tri.c);
			uint16_t c = MidPointIdx(tri.c, tri.a);

			newTriangleData.push_back({ tri.a, a, c });
			newTriangleData.push_back({ tri.b, b, a });
			newTriangleData.push_back({ tri.c, c, b });
			newTriangleData.push_back({ a, b, c });
		}

		initialTriangleSet = newTriangleData;
	}

	for (Structures::sIndexSet16 tri : initialTriangleSet)
	{
		m_indexSet16.push_back(tri);
		m_indexSetCount++;
	}
}


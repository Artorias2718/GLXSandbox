#include "cCapsule.h"
#include "cSphere.h"

#include "../../../Asserts/Asserts.h"
#include "../../../Logging/Logging.h"
#include "../../../Math/cVector.h"
#include "../../../Math/Functions.h"
#include "../../Structures/sVertex.h"
#include "../../Structures/sColor.h"

Engine::Graphics::Assets::Debug::cCapsule::cCapsule() : m_top(nullptr), m_bottom(nullptr) {}

Engine::Graphics::Assets::Debug::cCapsule::cCapsule(float i_radius, const Structures::sColor8& i_color) : cCapsule()
{
	float ringHeight = 1;
	const uint16_t ringCount = 20;
	const uint16_t sectorCount = 20;

	float radiusStep = i_radius / sectorCount;

	for (uint16_t i = 0; i <= ringCount; i++)
	{
		float y = -0.5f * ringHeight + i * ringHeight;
		float d_theta = 2.0f *  Math::PI / sectorCount;
		for (uint16_t j = 0; j <= sectorCount; j++)
		{
			float c = cosf(j * d_theta);
			float s = sinf(j * d_theta);

			Math::cVector point(y, i_radius * c, i_radius * s);

			Structures::sVertex vertex;

			vertex.position.x = point.x;
			vertex.position.y = point.y;
			vertex.position.z = point.z;

			m_vertices.push_back(vertex);
			m_vertexCount++;
		}
	}

	uint16_t ringVertexCount = sectorCount + 1;
	for (uint16_t i = 0; i < ringCount; i++)
	{
		for (uint16_t j = 0; j < sectorCount; j++)
		{
			m_indexSet16.push_back({ static_cast<uint16_t>(i * ringVertexCount + j), static_cast<uint16_t>((i + 1) * ringVertexCount + j + 1), static_cast<uint16_t>(i * ringVertexCount + j + 1) });
			m_indexSet16.push_back({ static_cast<uint16_t>(i * ringVertexCount + j), static_cast<uint16_t>((i + 1) * ringVertexCount + j), static_cast<uint16_t>((i + 1) * ringVertexCount + j + 1) });
			m_indexSetCount += 2;
		}
	}

	for (uint16_t i = 0; i < m_vertexCount; ++i)
	{
		m_vertices[i].color.r = i_color.r;
		m_vertices[i].color.g = i_color.g;
		m_vertices[i].color.b = i_color.b;
		m_vertices[i].color.a = i_color.a;
	}

	Math::cVector midpointTop(ringHeight * ringCount, 0, 0);
	Math::cVector midpointBottom(Math::cVector::zero);

	m_top = new Engine::Graphics::Assets::Debug::cSphere(i_radius, midpointTop, i_color);
	m_bottom = new  Engine::Graphics::Assets::Debug::cSphere(i_radius, midpointBottom, i_color);

	Initialize();
}

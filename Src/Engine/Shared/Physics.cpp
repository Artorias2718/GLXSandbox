#include "Physics.h"
#include "cGameObject.h"
#include "sCollisionTriangle.h"
#include "../Math/glm/vec3.hpp"

namespace
{
	bool TriangleRayIntersection(bool i_isSegment, const glm::vec3& i_p, const glm::vec3& i_q,
		const Engine::Shared::sCollisionTriangle& i_collisionTriangle, glm::vec3& i_intersectionPoint, float& i_t);
}

bool Engine::Shared::Physics::ApplyLinearForce(cGameObject* i_gameobject, const glm::vec3& i_force, float i_deltaTime)
{
	i_gameobject->m_rigidbody.acceleration = i_force / i_gameobject->m_rigidbody.m_mass;
	i_gameobject->m_transform.position += i_deltaTime * i_gameobject->m_rigidbody.velocity + 0.5f * (i_deltaTime * i_deltaTime) * i_gameobject->m_rigidbody.acceleration;
	i_gameobject->m_rigidbody.velocity += i_deltaTime * i_gameobject->m_rigidbody.acceleration;

	return true;
}

bool Engine::Shared::Physics::TriangleRaycast(cGameObject* i_gameobject, bool i_isSegment, const glm::vec3& i_p, const glm::vec3& i_q, glm::vec3& o_tri_normal, glm::vec3& o_intersectionPoint)
{
	for (size_t i = 0; i < i_gameobject->m_rigidbody.m_collisionData.size(); ++i)
	{
		glm::vec3 intersection;
		float t;

		sCollisionTriangle tri = *i_gameobject->m_rigidbody.m_collisionData[i];

		if (TriangleRayIntersection(i_isSegment, i_p, i_q, tri, intersection, t))
		{
			o_tri_normal = glm::normalize(tri.normal);

			glm::vec3 centroid = (1.0f / 3) * (tri.a + tri.b + tri.c);

			o_intersectionPoint.x = intersection.x * centroid.x;
			o_intersectionPoint.y = intersection.y * centroid.y;
			o_intersectionPoint.z = intersection.z * centroid.z;
			return true;
		}
	}
	return false;
}

namespace
{
	bool TriangleRayIntersection(bool i_isSegment, const glm::vec3& i_p, const glm::vec3& i_q,
		const Engine::Shared::sCollisionTriangle& i_collisionTriangle, glm::vec3& i_intersectionPoint, float& i_t)
	{
		glm::vec3 qp = i_p - i_q;

		// Compute denominator d. If d <= 0, segment is parallel to or points
		// away from triangle, so exit early
		float d = glm::dot(qp, i_collisionTriangle.normal);
		if (d <= 0.0f) return false;

		// Compute intersection t value of pq with plane of triangle. A ray
		// intersects if 0 <= t. Segment intersects if 0 <= t <= 1. Delay
		// dividing by d until intersection has been found to pierce triangle
		glm::vec3 ap = i_p - i_collisionTriangle.a;
		i_t = glm::dot(ap, i_collisionTriangle.normal);
		if (i_t < 0.0f) return false;
		if (i_isSegment)
		{
			if (i_t > d) return false; // For segment; exclude this code line for a ray test
		}

		// Compute barycentric coordinate components and test if within bounds
		glm::vec3 e = glm::cross(qp, ap);
		i_intersectionPoint.y = glm::dot(i_collisionTriangle.ac, e);
		if (i_intersectionPoint.y < 0.0f || i_intersectionPoint.y > d) return false;
		i_intersectionPoint.z = -glm::dot(i_collisionTriangle.ab, e);
		if (i_intersectionPoint.z < 0.0f || i_intersectionPoint.y + i_intersectionPoint.z > d) return false;

		// Segment/ray intersects triangle. Perform delayed division and
		// compute the last barycentric coordinate component
		float ood = 1.0f / d;
		i_t *= ood;
		i_intersectionPoint.y *= ood;
		i_intersectionPoint.z *= ood;
		i_intersectionPoint.x = 1.0f - i_intersectionPoint.y - i_intersectionPoint.z;
		return true;
	}
}


// Header Files
//=============

#include "cQuaternion.h"

#include <cmath>
#include "cVector.h"
#include "../Asserts/Asserts.h"

// Static Data Initialization
//===========================

namespace
{
	const float s_epsilon = 1.0e-9f;
}

// Interface
//==========

// Initialization
//---------------------------

Engine::Math::cQuaternion::cQuaternion(const float i_w, const float i_x, const float i_y, const float i_z)
	:
	w(i_w), x(i_x), y(i_y), z(i_z) {}

// Concatenation
Engine::Math::cQuaternion Engine::Math::cQuaternion::operator *(const cQuaternion& i_rhs) const
{
	return cQuaternion(
		(w * i_rhs.w) - ((x * i_rhs.x) + (y * i_rhs.y) + (z * i_rhs.z)),
		(w * i_rhs.x) + (x * i_rhs.w) + ((y * i_rhs.z) - (z * i_rhs.y)),
		(w * i_rhs.y) + (y * i_rhs.w) + ((z * i_rhs.x) - (x * i_rhs.z)),
		(w * i_rhs.z) + (z * i_rhs.w) + ((x * i_rhs.y) - (y * i_rhs.x)));
}

// Inversion
void Engine::Math::cQuaternion::Invert()
{
	x = -x;
	y = -y;
	z = -z;
}
Engine::Math::cQuaternion Engine::Math::cQuaternion::CreateInverse() const
{
	return cQuaternion(w, -x, -y, -z);
}

// Normalization
void Engine::Math::cQuaternion::Normalize()
{
	const float length = std::sqrt((w * w) + (x * x) + (y * y) + (z * z));
	ASSERTF(length > s_epsilon, "Can't divide by zero");
	const float length_reciprocal = 1.0f / length;
	w *= length_reciprocal;
	x *= length_reciprocal;
	y *= length_reciprocal;
	z *= length_reciprocal;
}
Engine::Math::cQuaternion Engine::Math::cQuaternion::CreateNormalized() const
{
	const float length = std::sqrt((w * w) + (x * x) + (y * y) + (z * z));
	ASSERTF(length > s_epsilon, "Can't divide by zero");
	const float length_reciprocal = 1.0f / length;
	return cQuaternion(w * length_reciprocal, x * length_reciprocal, y * length_reciprocal, z * length_reciprocal);
}

// Products
float Engine::Math::Dot(const cQuaternion& i_lhs, const cQuaternion& i_rhs)
{
	return (i_lhs.w * i_rhs.w) + (i_lhs.x * i_rhs.x) + (i_lhs.y * i_rhs.y) + (i_lhs.z * i_rhs.z);
}

Engine::Math::cVector Engine::Math::operator*(const cQuaternion& i_lhs, const cVector& i_rhs)
{
	cVector quatVector(i_lhs.x, i_lhs.y, i_lhs.z);
	cVector uv(-Math::Cross(quatVector, i_rhs));
	cVector uuv(-Math::Cross(quatVector, uv));

	return i_rhs + 2.0f * ((uv * i_lhs.w) + uuv);
}

// Initialization / Shut Down
//---------------------------

Engine::Math::cQuaternion::cQuaternion()
	:
	w(1.0f), x(0.0f), y(0.0f), z(0.0f)
{

}

Engine::Math::cQuaternion::cQuaternion(const float i_angleInRadians, const cVector& i_axisOfRotation_normalized)
{
	if (i_axisOfRotation_normalized.GetSqrLength() > 0 && i_axisOfRotation_normalized.GetSqrLength() != 1)
	{
		i_axisOfRotation_normalized.CreateNormalized();
	}

	const float theta_half = i_angleInRadians * 0.5f;
	w = std::cos(theta_half);
	const float sin_theta_half = std::sin(theta_half);
	x = i_axisOfRotation_normalized.x * sin_theta_half;
	y = i_axisOfRotation_normalized.y * sin_theta_half;
	z = i_axisOfRotation_normalized.z * sin_theta_half;
}

// Implementation
//===============
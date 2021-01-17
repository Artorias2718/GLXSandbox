// Header Files
//=============

#include "cVector.h"

#include <cmath>
#include "../Asserts/Asserts.h"

// Static Data Initialization
//===========================

namespace
{
	const float s_epsilon = 1.0e-9f;
}

Engine::Math::cVector Engine::Math::cVector::right = Engine::Math::cVector(1.0f, 0.0f, 0.0f);
Engine::Math::cVector Engine::Math::cVector::up = Engine::Math::cVector(0.0f, 1.0f, 0.0f);
Engine::Math::cVector Engine::Math::cVector::forward = Engine::Math::cVector(0.0f, 0.0f, 1.0f);
Engine::Math::cVector Engine::Math::cVector::zero = Engine::Math::cVector(0.0f, 0.0f, 0.0f);

// Interface
//==========

// Initialization
//---------------------------
Engine::Math::cVector::cVector(const float i_x, const float i_y, const float i_z)
	:
	x(i_x), y(i_y), z(i_z) {}

// Addition
Engine::Math::cVector Engine::Math::cVector::operator +(const cVector& i_rhs) const
{
	return cVector(x + i_rhs.x, y + i_rhs.y, z + i_rhs.z);
}
Engine::Math::cVector& Engine::Math::cVector::operator +=(const cVector& i_rhs)
{
	x += i_rhs.x;
	y += i_rhs.y;
	z += i_rhs.z;
	return *this;
}

// Subtraction / Negation
Engine::Math::cVector Engine::Math::cVector::operator -(const cVector& i_rhs) const
{
	return cVector(x - i_rhs.x, y - i_rhs.y, z - i_rhs.z);
}
Engine::Math::cVector& Engine::Math::cVector::operator -=(const cVector& i_rhs)
{
	x -= i_rhs.x;
	y -= i_rhs.y;
	z -= i_rhs.z;
	return *this;
}
Engine::Math::cVector Engine::Math::cVector::operator -() const
{
	return cVector(-x, -y, -z);
}

// Multiplication
Engine::Math::cVector Engine::Math::cVector::operator *(const float i_rhs)
{
	x *= i_rhs;
	y *= i_rhs;
	z *= i_rhs;
	return *this;
}

Engine::Math::cVector& Engine::Math::cVector::operator *=(const float i_rhs)
{
	x *= i_rhs;
	y *= i_rhs;
	z *= i_rhs;
	return *this;
}

// Division
Engine::Math::cVector Engine::Math::cVector::operator /(const float i_rhs) const
{
	ASSERTF(std::abs(i_rhs) > s_epsilon, "Can't divide by zero");
	const float rhs_reciprocal = 1.0f / i_rhs;
	return cVector(x * rhs_reciprocal, y * rhs_reciprocal, z * rhs_reciprocal);
}
Engine::Math::cVector& Engine::Math::cVector::operator /=(const float i_rhs)
{
	ASSERTF(std::abs(i_rhs) > s_epsilon, "Can't divide by zero");
	const float rhs_reciprocal = 1.0f / i_rhs;
	x *= rhs_reciprocal;
	y *= rhs_reciprocal;
	z *= rhs_reciprocal;
	return *this;
}

// Length / Normalization
float Engine::Math::cVector::GetSqrLength() const
{
	return x * x + y * y + z * z;
}

float Engine::Math::cVector::GetLength() const
{
	return std::sqrt(GetSqrLength());
}

float Engine::Math::cVector::Normalize()
{
	const float length = GetLength();
	ASSERTF(length > s_epsilon, "Can't divide by zero");
	operator /=(length);
	return length;
}

Engine::Math::cVector Engine::Math::cVector::CreateNormalized() const
{
	const float length = GetLength();
	ASSERTF(length > s_epsilon, "Can't divide by zero");
	const float length_reciprocal = 1.0f / length;
	return cVector(x * length_reciprocal, y * length_reciprocal, z * length_reciprocal);
}

// Comparison
bool Engine::Math::cVector::operator ==(const cVector& i_rhs) const
{
	// Use & rather than && to prevent branches (all three comparisons will be evaluated)
	return (x == i_rhs.x) & (y == i_rhs.y) & (z == i_rhs.z);
}
bool Engine::Math::cVector::operator !=(const cVector& i_rhs) const
{
	// Use | rather than || to prevent branches (all three comparisons will be evaluated)
	return (x != i_rhs.x) | (y != i_rhs.y) | (z != i_rhs.z);
}

// Products
Engine::Math::cVector Engine::Math::operator *(const float i_lhs, const cVector& i_rhs)
{
	return cVector(i_lhs * i_rhs.x, i_lhs * i_rhs.y, i_lhs * i_rhs.z);
}

float Engine::Math::Dot(const cVector& i_lhs, const cVector& i_rhs)
{
	return i_lhs.x * i_rhs.x + i_lhs.y * i_rhs.y + i_lhs.z + i_rhs.z;
}

Engine::Math::cVector Engine::Math::Cross(const cVector& i_lhs, const cVector& i_rhs)
{
	return cVector(
		(i_lhs.y * i_rhs.z - i_lhs.z * i_rhs.y),
		(i_lhs.z * i_rhs.x - i_lhs.x * i_rhs.z),
		(i_lhs.x * i_rhs.y - i_lhs.y * i_rhs.x)
	);
}


/*
This class represents a position or direction
*/

#ifndef MATH_CVECTOR_H
#define MATH_CVECTOR_H

// Class Declaration
//==================

namespace Engine 
{
	namespace Math
	{
		class cVector
		{
			// Interface
			//==========

		public:

			// Addition
			cVector operator +(const cVector& i_rhs) const;
			cVector& operator +=(const cVector& i_rhs);

			// Subtraction / Negation
			cVector operator -(const cVector& i_rhs) const;
			cVector& operator -=(const cVector& i_rhs);
			cVector operator -() const;

			// Multiplication
			cVector operator *(const float i_rhs);
			cVector& operator *=(const float i_rhs);

			friend cVector operator *(const float i_lhs, const cVector& i_rhs);

			// Division
			cVector operator /(const float i_rhs) const;
			cVector& operator /=(const float i_rhs);

			// Length / Normalization
			float GetLength() const;
			float GetSqrLength() const;
			float Normalize();
			cVector CreateNormalized() const;

			// Products
			friend float Dot(const cVector& i_lhs, const cVector& i_rhs);

			friend cVector Cross(const cVector& i_lhs, const cVector& i_rhs);

			// Comparison
			bool operator ==(const cVector& i_rhs) const;
			bool operator !=(const cVector& i_rhs) const;

			// Initialization / Shut Down
			//---------------------------

			cVector(const float i_x = 0.0f, const float i_y = 0.0f, const float i_z = 0.0f);

			// Data
			//=====

			// A vector class is one of the very few examples where I make data public
		public:

			float x, y, z;

			static cVector right;
			static cVector up;
			static cVector forward;
			static cVector zero;

			// Friend Classes
			//===============

			friend class cMatrix_Transformation;
			friend class cQuaternion;
		};
	}
}

#endif	// MATH_CVECTOR_H


/*
	This class represents a full transformation
	(i.e. a 4x4 matrix composed of a 3x3 rotation matrix and a 3 element translation vector)

	Matrix Facts:
		* The float elements are stored as columns
			* In other words, the second float stored is the first element in the second row
			* This is so that all 16 floats can be copied directly to Direct3D or OpenGL
				(Both HLSL and GLSL interpret a matrix's registers to be columns)
		* The vectors are the rows
			* In other words, the first three rows are the rotation and the last row is the translation
			* This is an arbitrary choice, but it is arguably the most common one in computer graphics
				(every other field outside of computer graphics that I know of uses column vectors, though)
			* This matches standard Direct3D but is backwards from standard OpenGL
		* The vectors are right-handed
			* In other words, the first vector is right, the second vector is up, and the third vector is back
			* This is an arbitrary choice; it matches Maya and standard OpenGL, and is backwards from standard Direct3D
*/

#ifndef MATH_CMATRIX_TRANSFORMATION_H
#define MATH_CMATRIX_TRANSFORMATION_H

// Forward Declarations
//=====================

namespace Engine 
{
	namespace Math
	{
		class cVector;
		class cQuaternion;
	}
}

// Class Declaration
//==================

namespace Engine 
{
	namespace Shared
	{
		struct sTransform;
	}
	namespace Math
	{
		class cMatrix_Transformation
		{
			// Interface
			//==========

		public:

			static cMatrix_Transformation CreateWorldToCameraTransform(
				const Shared::sTransform& i_transform);
			static cMatrix_Transformation CreateCameraToScreenTransform(
				const float i_fieldOfView_y, const float i_aspectRatio,
				const float i_z_nearPlane, const float i_z_farPlane);
			cMatrix_Transformation Transpose();

			// Initialization / Shut Down
			//---------------------------

			cMatrix_Transformation(																						// Identity (with default values)
				const float i_00 = 1.0f, const float i_10 = 0.0f, const float i_20 = 0.0f, const float i_30 = 0.0f,
				const float i_01 = 0.0f, const float i_11 = 1.0f, const float i_21 = 0.0f, const float i_31 = 0.0f,
				const float i_02 = 0.0f, const float i_12 = 0.0f, const float i_22 = 1.0f, const float i_32 = 0.0f,
				const float i_03 = 0.0f, const float i_13 = 0.0f, const float i_23 = 0.0f, const float i_33 = 1.0f);

			cMatrix_Transformation(const Shared::sTransform& i_transform);

			// Data
			//=====

		private:

			// Storage is column-major; see notes at the top of the file
			float m_00, m_10, m_20, m_30,
				m_01, m_11, m_21, m_31,
				m_02, m_12, m_22, m_32,
				m_03, m_13, m_23, m_33;
		};
	}
}

#endif	// MATH_CMATRIX_TRANSFORMATION_H


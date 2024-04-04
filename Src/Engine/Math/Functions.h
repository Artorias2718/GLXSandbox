/*
	This file contains math-related functions
*/

#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H

#include "glm/mat4x4.hpp"

namespace Engine 
{
	namespace Shared
	{
		struct sTransform;
	}

	namespace Math
	{
		// Interface
		//==========

		float ConvertDegreesToRadians( const float i_degrees );
		template<typename tUnsignedInteger>
			tUnsignedInteger RoundUpToMultiple( const tUnsignedInteger i_value, const tUnsignedInteger i_multiple );
		template<typename tUnsignedInteger>
			tUnsignedInteger RoundUpToMultiple_PowerOf2( const tUnsignedInteger i_value,
				const tUnsignedInteger i_multipleWhichIsAPowerOf2 );

			glm::mat4 CreateWorldToCameraTransform(const Shared::sTransform &i_transform);
			glm::mat4 CreateCameraToScreenTransform(
				const float i_fieldOfView_y, const float i_aspectRatio,
				const float i_z_nearPlane, const float i_z_farPlane);
			glm::mat4 UpdateTransform(const Shared::sTransform& transform);

			extern glm::vec3 right;
			extern glm::vec3 up;
			extern glm::vec3 forward;
			extern glm::vec3 zero;
	}
}

#include "Functions.inl"

#endif	// MATH_FUNCTIONS_H

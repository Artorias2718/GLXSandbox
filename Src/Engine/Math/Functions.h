/*
	This file contains math-related functions
*/

#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H

namespace Engine 
{
	namespace Math
	{
		// Interface
		//==========

		extern const float PI;

		float ConvertDegreesToRadians( const float i_degrees );
		template<typename tUnsignedInteger>
			tUnsignedInteger RoundUpToMultiple( const tUnsignedInteger i_value, const tUnsignedInteger i_multiple );
		template<typename tUnsignedInteger>
			tUnsignedInteger RoundUpToMultiple_PowerOf2( const tUnsignedInteger i_value,
				const tUnsignedInteger i_multipleWhichIsAPowerOf2 );
	}
}

#include "Functions.inl"

#endif	// MATH_FUNCTIONS_H

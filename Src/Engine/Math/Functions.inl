#ifndef MATH_FUNCTIONS_INL
#define MATH_FUNCTIONS_INL

// Header Files
//=============

#include "Functions.h"

#include "../Asserts/Asserts.h"

// Implementation
//===============

template<typename tUnsignedInteger>
	tUnsignedInteger Engine::Math::RoundUpToMultiple( const tUnsignedInteger i_value, const tUnsignedInteger i_multiple )
{
	ASSERTF( i_multiple != 0, "Zero isn't a valid multiple" );
	ASSERTF( i_multiple > 0, "The multiple must be positive" );
	// Adding one less than the multiple will make the value at or above the next hiehst multiple
	// unless the value was itself a multiple.
	// Dividng and multiplying by the multiple removes any remainder
	const tUnsignedInteger returnValue = ( ( i_value + i_multiple - 1 ) / i_multiple ) * i_multiple;
	ASSERT( ( returnValue % i_multiple ) == 0 );
	return returnValue;
}

template<typename tUnsignedInteger>
	inline tUnsignedInteger Engine::Math::RoundUpToMultiple_PowerOf2( const tUnsignedInteger i_value,
		const tUnsignedInteger i_multipleWhichIsAPowerOf2 )
{
	ASSERTF( i_multipleWhichIsAPowerOf2 != 0, "Zero isn't a valid multiple" );
	ASSERTF( i_multipleWhichIsAPowerOf2 > 0, "The multiple must be positive" );
	// To be a power-of-2 the multiple can only have a single bit set;
	// get a mask of the bits less-significant than that single bit
	const tUnsignedInteger nonLeadingBits = i_multipleWhichIsAPowerOf2 - 1;
	ASSERTF( ( i_multipleWhichIsAPowerOf2 && ( i_multipleWhichIsAPowerOf2 & nonLeadingBits ) ) == 0, "The multiple must be a power-of-2" );
	// Adding the non-leading bits will make the value at or above the next highest multiple
	// unless the value was itself a multiple.
	// ANDing with inverse then removes any bits less than the multiple.
	const tUnsignedInteger returnValue = ( i_value + nonLeadingBits ) & ~nonLeadingBits;
	ASSERT( ( returnValue % i_multipleWhichIsAPowerOf2 ) == 0 );
	return returnValue;
}

#endif	// MATH_FUNCTIONS_INL
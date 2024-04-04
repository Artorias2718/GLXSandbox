// Header Files
//=============

#include "Functions.h"

// Static Data Initialization
//===========================

const float Engine::Math::PI = 3.1415926536f;

// Interface
//==========

float Engine::Math::ConvertDegreesToRadians( const float i_degrees )
{
	return i_degrees * PI / 180.0f;
}
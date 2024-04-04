/*
    This is an example of a fragment shader
*/

#include "Shaders.inc"

// Entry Point
//============

#if defined D3D_API 
void main(

	// Input
	//======

	// The GPU provides us with position
	in float4 i_position : SV_POSITION,
	in float4 i_color : COLOR,
    
	// Output
	//=======

	// Whatever color value is output from the fragment shader
	// will determine the color of the corresponding pixel on the screen
	out float4 o_color : SV_TARGET

	)
#elif defined OGL_API 

// Input
//======

// Whatever arbitrary data (i.e. everything excluding position) was output from the vertex shader
// will be interpolated across the triangle and given as input to the fragment shader
layout( location = 0 ) in vec4 i_color;

// Output
//=======

// Whatever color value is output from the fragment shader
// will determine the color of the corresponding pixel on the screen
out vec4 o_color;

// Entry Point
//============

void main()
#endif
{
	// For now set the fragment as the interpolated color
	o_color = i_color;
}
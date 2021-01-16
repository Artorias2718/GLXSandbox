/*
    This is an example of a vertex shader
*/

#include "Shaders.inc"

// Entry Point
//============

#if defined D3D_API
void main(

	// Input
	//======

	// The "semantics" (the keywords in all caps after the colon) are arbitrary,
	// but must match the C call to CreateInputLayout()

	// This value comes from one of the sVertex that we filled the vertex buffer with in C code
	in const float2 i_position : POSITION,

	// Output
	//=======

	// An SV_POSITION value must always be output from every vertex shader
	// so that the GPU can figure out which fragments need to be shaded
	out float4 gl_Position : SV_POSITION
	
    )
#elif defined OGL_API

// Input
//======

// The locations assigned are arbitrary
// but must match the C calls to glVertexAttribPointer()

// This value comes from one of the sVertex that we filled the vertex buffer with in C code
layout( location = 0 ) in vec2 i_position;

// Output
//=======

// None; the vertex shader must always output a position value,
// but unlike HLSL where the value is explicit
// GLSL has an implicit required variable called "gl_Position"

// Entry Point
//============

void main()
#endif
{
	// Calculate the position of this vertex on screen
	{
		// When we move to 3D graphics the screen position that the vertex shader outputs
		// will be different than the position that is input to it from C code,
		// but for now the "out" position is set directly from the "in" position:

		gl_Position = float4( i_position.x + cos(g_elapsedSeconds), i_position.y - sin(g_elapsedSeconds), 0.0, 1.0 );
	}
}
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
	in const float3 i_position : POSITION,
	in const float4 i_color : COLOR,

	// Output
	//=======

	// An SV_POSITION value must always be output from every vertex shader
	// so that the GPU can figure out which fragments need to be shaded
	out float4 gl_Position : SV_POSITION,

	
	// Any other data is optional; the GPU doesn't know or care what it is,
	// and will merely interpolate it across the triangle
	// and give us the resulting interpolated value in a fragment shader.
	// It is then up to us to use it however we want to.
	// The "semantics" are used to match the vertex shader outputs
	// with the fragment shader inputs
	// (note that OpenGL uses arbitrarily assignable number IDs to do the same thing).
	out float4 o_color : COLOR
	
    )
#elif defined OGL_API

// Input
//======

// The locations assigned are arbitrary
// but must match the C calls to glVertexAttribPointer()

// This value comes from one of the sVertex that we filled the vertex buffer with in C code
layout( location = 0 ) in vec3 i_position;
layout( location = 1 ) in vec4 i_color;

// Output
//=======

// Any other data is optional; the GPU doesn't know or care what it is,
// and will merely interpolate it across the triangle
// and give us the resulting interpolated value in a fragment shader.
// It is then up to us to use it however we want to.
// The locations are used to match the vertex shader outputs
// with the fragment shader inputs
// (note that Direct3D uses arbitrarily assignable "semantics").
layout( location = 0 ) out vec4 o_color;

// Entry Point
//============

void main()
#endif
{
	// Calculate the position of this vertex on screen
	{
        float4 localSpace = float4( i_position.x, i_position.y, i_position.z, 1.0 );

        // Local to world space transformation 
        float4 worldSpace = mul(g_localToWorld, localSpace);

        // World to camera space transformation
        float4 cameraSpace = mul(worldSpace, g_worldToCamera);

        // Camera to screen space
        gl_Position = mul(cameraSpace, g_cameraToScreen);
	}

    // Pass the input color to the fragment shader unchanged
    {
        o_color = i_color;
    }
}
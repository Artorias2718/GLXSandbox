/*
	This is an example of a fragment shader
*/

// Constants
//==========

cbuffer constantBuffer : register( b0 )
{
	float g_elapsedSeconds;
}

// Entry Point
//============

void main(

	// Input
	//======

	// The GPU provides us with position

	in float4 i_position : SV_POSITION,

	// Output
	//=======

	// Whatever color value is output from the fragment shader
	// will determine the color of the corresponding pixel on the screen
	out float4 o_color : SV_TARGET

	)
{
	// Set the fragment to white
	// (where color is represented by 4 floats representing "RGBA" == "Red/Green/Blue/Alpha").
	// Try experimenting with changing the values of the first three numbers
	// to something in the range [0,1] and observe the results.
	o_color = float4( cos(g_elapsedSeconds), 1.0, sin(g_elapsedSeconds), 1.0 );
}
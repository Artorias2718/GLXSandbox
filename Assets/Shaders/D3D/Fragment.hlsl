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
	o_color = float4( 1.0, 1.0, 1.0, 1.0 );

	// TODO: Change the color based on time!
	// The value g_elapsedSeconds should change every second, and so by doing something like
	// sin( g_elapsedSeconds ) or cos( g_elapsedSeconds )
	// you can get a value that will oscillate between [-1,1].
	// You should change at least one "channel" so that the color animates.
	// For example, to change red ("r") you would do something kind of like:
	//		o_color.r = ? sin( g_elapsedSeconds ) ?
	// You can change .r, .g, and .b (but leave .a as 1.0).
	// Remember that your final values should be [0,1], so you will have to do some math.
	o_color = float4( cos(g_elapsedSeconds), 1.0, sin(g_elapsedSeconds), 1.0 );
}
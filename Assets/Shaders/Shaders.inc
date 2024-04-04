// This file should be #included by all shaders

#if defined D3D_API 

	// Types
	//======

	#define vec2 float2
	#define vec3 float3
	#define vec4 float4
	#define mat2 float2x2
	#define mat3 float3x3
	#define mat4 float4x4

    // Declare Constants
    //==========

    #define DeclareConstantBuffer( i_name, i_id ) cbuffer i_name : register( b##i_id )

#elif defined OGL_API 

	// Version Information
	//====================

	// The version of GLSL to use must come first
	#version 420

	// Types
	//======

	#define float2 vec2
	#define float3 vec3
	#define float4 vec4
	#define float2x2 mat2
	#define float3x3 mat3
	#define float4x4 mat4

    // Declare Constants
    //==========
    
    #define DeclareConstantBuffer( i_name, i_id ) layout( std140, binding = i_id ) uniform i_name
#endif

// Constants
//==========
DeclareConstantBuffer( frame, 0 )
{
	float g_elapsedSeconds;
};

DeclareConstantBuffer( drawCall, 1 )
{
	float2 g_screenPosition;
};
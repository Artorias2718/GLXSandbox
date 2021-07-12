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
    
    // Multiply 4x4 Matrix by 4x1 Vector
    //==================================

    #define mul( i_matrix, i_vector) i_matrix * i_vector
	#define mul( i_vector, i_matrix) i_vector * i_matrix
#endif

// Constants
//==========
DeclareConstantBuffer( frameBuffer, 0 )
{
    float4x4 g_worldToCamera;
    float4x4 g_cameraToScreen;
	float g_elapsedSeconds;
};

DeclareConstantBuffer( drawCallBuffer, 1 )
{
    float4x4 g_localToWorld;
};

DeclareConstantBuffer( materialBuffer, 2 )
{
    float4 g_color;
};
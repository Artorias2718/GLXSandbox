// Header Files
//=============

#include "../../Interfaces/cRenderState/cRenderState.h"

#include "../../OGL/Includes.h"
#include "../../../Asserts/Asserts.h"

// Interface
//==========

// Render
//-------

void Engine::Graphics::Interfaces::cRenderState::Bind() const
{
	// Alpha Transparency
	if (IsAlphaTransparencyEnabled())
	{
		glEnable(GL_BLEND);
		ASSERT(glGetError() == GL_NO_ERROR);
		// result = ( source * source.a ) + ( destination * ( 1 - source.a ) )
		glBlendEquation(GL_FUNC_ADD);
		ASSERT(glGetError() == GL_NO_ERROR);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		ASSERT(glGetError() == GL_NO_ERROR);
	}
	else
	{
		glDisable(GL_BLEND);
		ASSERT(glGetError() == GL_NO_ERROR);
	}
	// Depth Buffering
	if (IsDepthBufferingEnabled())
	{
		// The new fragment becomes a pixel if its depth is less than what has previously been written
		glEnable(GL_DEPTH_TEST);
		ASSERT(glGetError() == GL_NO_ERROR);
		glDepthFunc(GL_LESS);
		ASSERT(glGetError() == GL_NO_ERROR);
		// Write to the depth buffer
		glDepthMask(GL_TRUE);
		ASSERT(glGetError() == GL_NO_ERROR);
	}
	else
	{
		// Don't test the depth buffer
		glDisable(GL_DEPTH_TEST);
		ASSERT(glGetError() == GL_NO_ERROR);
		// Don't write to the depth buffer
		glDepthMask(GL_FALSE);
		ASSERT(glGetError() == GL_NO_ERROR);
	}
	// Cull backfacing triangles
	if (IsBackfaceCullingEnabled())
	{
		// Cull triangles that are facing backwards
		glEnable(GL_CULL_FACE);
		ASSERT(glGetError() == GL_NO_ERROR);
		// Triangles use right-handed winding order
		// (opposite from Direct3D)
		glFrontFace(GL_CCW);
		ASSERT(glGetError() == GL_NO_ERROR);
	}
	else
	{
		// Don't cull any triangles
		glDisable(GL_CULL_FACE);
		ASSERT(glGetError() == GL_NO_ERROR);
	}
	// Draw wireframe triangles
	if (IsWireframeRenderingEnabled())
	{
		// Render unfilled triangles
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		ASSERT(glGetError() == GL_NO_ERROR);
	}
	else
	{
		// Render filled triangles
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		ASSERT(glGetError() == GL_NO_ERROR);
	}
}

// Implementation
//===============

// Initialization / Clean Up
//--------------------------

bool Engine::Graphics::Interfaces::cRenderState::InitializeFromBits()
{
	// OpenGL uses the bits directly at binding time
	return true;
}

bool Engine::Graphics::Interfaces::cRenderState::CleanUp()
{
	return true;
}

#ifndef ENGINE_SHARED_MOUSEPARAMS_H
#define ENGINE_SHARED_MOUSEPARAMS_H

#include "../Math/glm/vec3.hpp"

namespace Engine 
{
	namespace Shared
	{
		namespace MouseParams
		{
			extern glm::vec3 horizontalAxis;
			extern glm::vec3 verticalAxis;

			extern bool mouseScrolled;
			extern bool mouseMoved;
			extern float mouseWheel;
			extern float prevMouseX;
			extern float prevMouseY;
			extern float mouseX;
			extern float mouseY;
		}
	}
}

#endif // !ENGINE_SHARED_MOUSEPARAMS_H

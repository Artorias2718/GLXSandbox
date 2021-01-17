#ifndef ENGINE_SHARED_MOUSEPARAMS_H
#define ENGINE_SHARED_MOUSEPARAMS_H

namespace Engine 
{
	namespace Math
	{
		class cVector;
	}
	namespace Shared
	{
		namespace MouseParams
		{
			extern Math::cVector horizontalAxis;
			extern Math::cVector verticalAxis;

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

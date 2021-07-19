#ifndef ENGINE_SHARED_WORLDUIPARAMS_H
#define ENGINE_SHARED_WORLDUIPARAMS_H

#include <string>

namespace Engine
{
	namespace Shared
	{
		class cGameObject;
		namespace WorldUIParams
		{
			extern std::string cameraLabel;
			extern Engine::Shared::cGameObject* activeCamera;
		}
	}
}

#endif // !ENGINE_SHARED_WORLDUIPARAMS_H

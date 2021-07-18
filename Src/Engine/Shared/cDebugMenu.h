#ifndef ENGINE_SHARED_CDEBUGMENU_H
#define ENGINE_SHARED_CDEBUGMENU_H

// As this is meant to be a debugging tool, we should only really allocate anything
// in a debug build, as in, this isn't intended for players who end up
// playing a game built on this engine (this is just a dev tool).

#if defined _DEBUG
#if defined D3D_API
#include "../Graphics/imGUI/imgui_impl_dx11.h"
#elif defined OGL_API
#include "../Graphics/imGUI/imgui_impl_opengl3.h"
#endif

#include "../Graphics/imGUI/imgui.h"
#include "../Graphics/imGUI/imgui_impl_win32.h"
#endif

#include "cGameObject.h"

namespace Engine
{
	namespace Shared
	{
		class cDebugMenu : public cGameObject
		{
		public:
			bool Update();
			bool Render();
			static cDebugMenu& Instance();
			cGameObject* DebugSphere();
		private:
			cDebugMenu();
#if defined _DEBUG
			ImGuiWindowFlags m_winFlags;
			ImGuiSliderFlags m_sclFlags;
			ImVec2 m_winPos;
			std::string m_fps;
			bool m_toggleSphereVal;
			float m_scaleSphereVal;
			bool m_resetSphereClicked;
			ImVec2 m_resetSphereSize;
			cGameObject* m_debugSphere;
#endif
		};
	}
}

#endif // !ENGINE_SHARED_CDEBUGMENU_H

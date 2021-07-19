#include "cWorldUI.h"
#include "cDebugMenu.h"
#include "..\Graphics\Assets\cMaterial.h"
#include "..\Graphics\Graphics.h"
#include <string>

#include "../UserInput/UserInput.h"
#include "../Time/Time.h"
#include <math.h>

namespace
{
}

Engine::Shared::cWorldUI::cWorldUI() 
	:
	m_winFlags(ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar),
	m_sclFlags(ImGuiSliderFlags_NoRoundToFormat),
	m_winPos(ImVec2(0, 0))
{}

#if defined _DEBUG
Engine::Shared::cWorldUI& Engine::Shared::cWorldUI::Instance()
{
	static cWorldUI instance;
	return instance;
}

#endif

bool Engine::Shared::cWorldUI::Update(Engine::Shared::cGameObject *camera)
{
	if (camera->m_name == "FlyCam")
	{
		WorldUIParams::cameraLabel = "Fly Camera";
	}
	else if (camera->m_name == "PlayerCam")
	{
		WorldUIParams::cameraLabel = "Player Camera";
	}
	return true;
}

bool Engine::Shared::cWorldUI::Render()
{
	// Start the Dear ImGUI Frame
#if defined D3D_API
	ImGui_ImplDX11_NewFrame();
#elif defined OGL_API
	ImGui_ImplOpenGL3_NewFrame();
#endif
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("World UI", (bool*)0, m_winFlags);
	ImGui::SetWindowPos("World UI", ImVec2(0, 0));
	ImGui::LabelText("", WorldUIParams::cameraLabel.c_str());
	ImGui::End();

	ImGui::Render();

#if defined D3D_API
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#elif defined OGL_API
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
	return true;
}

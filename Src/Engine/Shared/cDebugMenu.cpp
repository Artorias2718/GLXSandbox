#include "cDebugMenu.h"
#include "..\Graphics\Assets\Debug\cSphere.h"
#include "..\Graphics\Assets\cMaterial.h"
#include "..\Graphics\Graphics.h"
#include <string>

#include "../UserInput/UserInput.h"
#include "../Time/Time.h"
#include <math.h>

namespace
{
	int frameCount = 0;
	const int MAXFRAMES = 1000;
}

Engine::Shared::cDebugMenu::cDebugMenu() 
#if defined _DEBUG
	:
	m_winFlags(ImGuiWindowFlags_AlwaysAutoResize),
	m_sclFlags(ImGuiSliderFlags_NoRoundToFormat),
	m_winPos(ImVec2(0, 0)),
	m_fps("0"),
	m_toggleSphereVal(false),
	m_scaleSphereVal(1.0f),
	m_resetSphereClicked(false),
	m_resetSphereSize(ImVec2(150, 50))
#endif
{
#if defined _DEBUG
	m_debugSphere = new cGameObject(new Engine::Graphics::Assets::Debug::cSphere({ 0, 255, 0, 255 }), new Graphics::Assets::cMaterial("debug"));
#endif
}

Engine::Shared::cDebugMenu& Engine::Shared::cDebugMenu::Instance()
{
	static cDebugMenu instance;
	return instance;
}

bool Engine::Shared::cDebugMenu::Update()
{
	if (Engine::UserInput::IsKeyPressed(0xc0))
	{
		m_active = !m_active;
	}
	return true;
}

bool Engine::Shared::cDebugMenu::Render()
{
#if defined _DEBUG
	// Start the Dear ImGUI Frame
#if defined D3D_API
	ImGui_ImplDX11_NewFrame();
#elif defined OGL_API
	ImGui_ImplOpenGL3_NewFrame();
#endif
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Debug Menu", (bool*)0, m_winFlags);

	ImGui::SetWindowPos("Debug Menu", m_winPos);

	ImGui::LabelText("", (m_fps + " FPS").c_str());

	ImGui::Checkbox("Toggle Sphere", &m_toggleSphereVal);

	ImGui::SliderFloat("Scale Sphere", &m_scaleSphereVal, 1.0f, 10.0f, "%1.1f", m_sclFlags);
	m_resetSphereClicked = ImGui::Button("Reset Sphere", m_resetSphereSize);
	ImGui::End();

	ImGui::Render();

#if defined D3D_API
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#elif defined OGL_API
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

	// Update FPS counter
	frameCount++;

	if (frameCount == MAXFRAMES)
	{
		m_fps = std::to_string((int)roundf((1 / Time::DeltaTime())));
		frameCount = 0;
	}

	// Toggle debug sphere visibility
	if (m_toggleSphereVal)
	{
		Engine::Graphics::SubmitGameObject(m_debugSphere);
	}

	// Update debug sphere scale
	m_debugSphere->m_transform.scale = m_scaleSphereVal * Math::cVector(1.0f, 1.0f, 1.0f);

	// Reset debug sphere scale
	if (m_resetSphereClicked)
	{
		m_scaleSphereVal = 1.0f;
	}
#endif
	return true;
}

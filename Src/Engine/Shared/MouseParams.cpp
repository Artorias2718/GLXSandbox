#include "MouseParams.h"
#include "MouseParams.h"
#include "../Math/cVector.h"

Engine::Math::cVector Engine::Shared::MouseParams::horizontalAxis(1.0f, 0.0f, 0.0f);
Engine::Math::cVector Engine::Shared::MouseParams::verticalAxis(0.0f, 1.0f, 0.0f);

bool Engine::Shared::MouseParams::mouseScrolled = 0;
bool Engine::Shared::MouseParams::mouseMoved = 0;

float Engine::Shared::MouseParams::mouseWheel = 0;
float Engine::Shared::MouseParams::prevMouseX = 0;
float Engine::Shared::MouseParams::prevMouseY = 0;
float Engine::Shared::MouseParams::mouseX = 0;
float Engine::Shared::MouseParams::mouseY = 0;

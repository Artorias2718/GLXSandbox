// Header Files
//=============

#include "cGame.h"

#include "../../Engine/Graphics/Assets/cMesh.h"
#include "../../Engine/Graphics/Graphics.h"

// Interface
//==========

namespace
{
	Engine::Graphics::Assets::cMesh* square;
}

// Initialization / Clean Up
//--------------------------

Game::cGame::~cGame()
{
	CleanUp();
}

// Inherited Implementation
//=========================

// Initialization / Clean Up
//--------------------------

bool Game::cGame::Initialize()
{
	square = new Engine::Graphics::Assets::cMesh("square");
	return true;
}

bool Game::cGame::Update()
{
	Engine::Graphics::SubmitGameObject(square);
	return true;
}

bool Game::cGame::CleanUp()
{
	return true;
}

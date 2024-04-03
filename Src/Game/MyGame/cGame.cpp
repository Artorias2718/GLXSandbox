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

Game::MyGame::cGame::~cGame()
{
	CleanUp();
}

// Inherited Implementation
//=========================

// Initialization / Clean Up
//--------------------------

bool Game::MyGame::cGame::Initialize()
{
	square = new Engine::Graphics::Assets::cMesh("square");
	return true;
}

bool Game::MyGame::cGame::Update()
{
	Engine::Graphics::SubmitGameObject(square);
	return true;
}

bool Game::MyGame::cGame::CleanUp()
{
	return true;
}

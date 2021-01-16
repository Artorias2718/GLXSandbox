// Header Files
//=============

#include "cMyGame.h"

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

Game::MyGame::cMyGame::~cMyGame()
{
	CleanUp();
}

// Inherited Implementation
//=========================

// Initialization / Clean Up
//--------------------------

bool Game::MyGame::cMyGame::Initialize()
{
	square = new Engine::Graphics::Assets::cMesh("square");
	return true;
}

bool Game::MyGame::cMyGame::Update()
{
	Engine::Graphics::SubmitGameObject(square);
	return true;
}

bool Game::MyGame::cMyGame::CleanUp()
{
	return true;
}

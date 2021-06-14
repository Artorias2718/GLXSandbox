#ifndef GAME_MYGAME_SPLAYER_H
#define GAME_MYGAME_SPLAYER_H

#include "../../../Engine/Shared/cGameObject.h"

namespace Engine
{
	namespace Shared
	{
		class cCamera;
	}
}

namespace Engine
{
	namespace Graphics
	{
		namespace Assets
		{
			namespace Debug
			{
				class cLine;
				class cSphere;
			}
		}
	}
}

namespace Game 
{
	namespace MyGame
	{
		struct sPlayer : public Engine::Shared::cGameObject
		{
			sPlayer();
			sPlayer(const std::string& i_fileName);
			~sPlayer();

			bool CheckHorizontalCollisions();
			bool CheckInclinedCollisions();
			bool CheckVerticalCollisions();

			bool Update();
			bool Render();

			Engine::Shared::cCamera* m_camera;
			Engine::Graphics::Assets::Debug::cSphere* m_headSphereMesh;
			Engine::Graphics::Assets::Debug::cSphere* m_bodySphereMesh;
			Engine::Graphics::Assets::Debug::cLine* m_hAxisMesh;
			Engine::Graphics::Assets::Debug::cLine* m_vAxisMesh;
			Engine::Shared::cGameObject* m_headSphere;
			Engine::Shared::cGameObject* m_bodySphere;
			Engine::Shared::cGameObject* m_hAxis;
			Engine::Shared::cGameObject* m_vAxis;

			float m_height;
			float m_width;
		};
	}
}

#endif // !GAME_MYGAME_SPLAYER_H


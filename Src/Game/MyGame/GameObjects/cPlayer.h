#ifndef GAME_MYGAME_CPLAYER_H
#define GAME_MYGAME_CPLAYER_H

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
		class cPlayer : public Engine::Shared::cGameObject
		{
		public:
			cPlayer();
			cPlayer(const std::string& i_fileName);
			~cPlayer();

			bool CheckHorizontalCollisions();
			bool CheckInclinedCollisions();
			bool CheckVerticalCollisions();

			bool Update();
			bool Render();

			Engine::Shared::cCamera* m_camera;
			Engine::Graphics::Assets::Debug::cSphere* m_headSphereMesh;
			Engine::Graphics::Assets::Debug::cSphere* m_bodySphereMesh;
			Engine::Graphics::Assets::Debug::cLine* m_xzAxesMesh;
			Engine::Graphics::Assets::Debug::cLine* m_yAxisMesh;
			Engine::Shared::cGameObject* m_headSphere;
			Engine::Shared::cGameObject* m_bodySphere;
			Engine::Shared::cGameObject* m_xzAxes;
			Engine::Shared::cGameObject* m_yAxis;

			float m_height;
			float m_width;
		};
	}
}

#endif // !GAME_MYGAME_CPLAYER_H

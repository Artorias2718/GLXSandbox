#ifndef GAME_MYGAME_PLAYERCONTROLLER_H
#define GAME_MYGAME_PLAYERCONTROLLER_H

#include "../../../Engine/Shared/cGameObjectController.h"

namespace Game
{
	namespace MyGame
	{
		class cPlayer;
		class cPlayerController : public Engine::Shared::cGameObjectController
		{
		public:
			cPlayerController(cPlayer* i_player);
			~cPlayerController();
			bool Move();

			cPlayer* m_player;
		};
	}
}

#endif // !GAME_MYGAME_PLAYERCONTROLLER_H

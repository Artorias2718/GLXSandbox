
#ifndef GAME_MYGAME_PLAYERCONTROLLER_H
#define GAME_MYGAME_PLAYERCONTROLLER_H

#include "../../../Engine/Shared/sGameObjectController.h"

namespace Game 
{
	namespace MyGame
	{
		struct sPlayer;
		struct sPlayerController : public Engine::Shared::sGameObjectController
		{
			sPlayerController(sPlayer* i_player);
			~sPlayerController();
			bool Move();

			sPlayer* m_player;
		};
	}
}

#endif // !GAME_MYGAME_PLAYERCONTROLLER_H
#ifndef ENGINE_SHARED_SGAMEOBJECTCONTROLLER_H
#define ENGINE_SHARED_SGAMEOBJECTCONTROLLER_H

namespace Engine 
{
	namespace Shared
	{
		//class cGameObject;
		class sGameObjectController
		{
		public:
			//cGameObjectController() {}
			//~cGameObjectController() {}

			virtual bool Move() = 0;
		};
	}
}


#endif // !ENGINE_SHARED_SGAMEOBJECTCONTROLLER_H



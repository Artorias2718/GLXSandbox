#ifndef ENGINE_SHARED_CGAMEOBJECTCONTROLLER_H
#define ENGINE_SHARED_CGAMEOBJECTCONTROLLER_H

namespace Engine 
{
	namespace Shared
	{
		class cGameObject;
		class cGameObjectController
		{
		public:
			cGameObjectController() {}
			~cGameObjectController() {}

			virtual bool Move() = 0;
		};
	}
}


#endif // !ENGINE_SHARED_CGAMEOBJECTCONTROLLER_H

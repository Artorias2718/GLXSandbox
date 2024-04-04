#ifndef ENGINE_SHARED_CGAMEOBJECT_H
#define ENGINE_SHARED_CGAMEOBJECT_H

#include <string>
#include "../Shared/sTransform.h"

namespace Engine 
{
	namespace Graphics
	{
		namespace Assets 
		{
			class cMesh;
		}
	}
	namespace Shared
	{
		class cGameObject
		{
		public:
			cGameObject();
			cGameObject(Graphics::Assets::cMesh *i_mesh);
			virtual ~cGameObject();

			Graphics::Assets::cMesh *m_mesh;
			Shared::sTransform m_transform;

			bool m_active;
		};
	}
}
#endif // !ENGINE_SHARED_CGAMEOBJECT_H
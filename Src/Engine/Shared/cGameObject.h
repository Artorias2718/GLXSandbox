#ifndef ENGINE_SHARED_CGAMEOBJECT_H
#define ENGINE_SHARED_CGAMEOBJECT_H

#include <string>
#include "../Graphics/Structures/sPosition.h"

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
			~cGameObject();

			Graphics::Assets::cMesh *m_mesh;
			Graphics::Structures::sPosition m_position;
		};
	}
}
#endif // !ENGINE_SHARED_CGAMEOBJECT_H
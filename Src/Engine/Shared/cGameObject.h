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
			class cMaterial;
		}
		namespace Interfaces
		{
			class cSprite;
		}
	}
	namespace Shared
	{
		class cGameObject
		{
		public:
			cGameObject();
			cGameObject(Graphics::Assets::cMesh* i_mesh, Graphics::Assets::cMaterial* i_material);
			cGameObject(Graphics::Assets::cMaterial* i_material,
				const Math::cVector& i_position = Math::cVector::zero, const Math::cVector& i_scale = Math::cVector(1, 1, 1));
			virtual ~cGameObject();

			Graphics::Assets::cMesh* m_mesh;
			Graphics::Interfaces::cSprite* m_sprite;
			Graphics::Assets::cMaterial* m_material;
			Shared::sTransform m_transform;

			bool m_active;
		};
	}
}
#endif // !ENGINE_SHARED_CGAMEOBJECT_H
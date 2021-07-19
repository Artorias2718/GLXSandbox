#ifndef ENGINE_SHARED_CGAMEOBJECT_H
#define ENGINE_SHARED_CGAMEOBJECT_H

#include <string>
#include "../Shared/sTransform.h"
#include "../Shared/sRigidBody.h"

namespace Engine 
{
	namespace Graphics
	{
		namespace Assets 
		{
			class cMesh;
			class cDebug;
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
			cGameObject(Graphics::Assets::cMesh *i_mesh, Graphics::Assets::cMaterial *i_material);
			cGameObject(Graphics::Assets::cDebug* i_debug, Graphics::Assets::cMaterial* i_material);
			cGameObject(Graphics::Assets::cMaterial* i_material,
				const glm::vec2& i_position = glm::zero<glm::vec2>(), const glm::vec2& i_scale = glm::one<glm::vec2>());
			virtual ~cGameObject();

			std::string m_name;
			Graphics::Assets::cMesh *m_mesh;
			Graphics::Assets::cDebug *m_debug;
			Graphics::Interfaces::cSprite *m_sprite;
			Graphics::Assets::cMaterial *m_material;
			Shared::sTransform m_transform;
			Shared::sRigidBody m_rigidbody;

			bool m_active;
		};
	}
}
#endif // !ENGINE_SHARED_CGAMEOBJECT_H
#ifndef ENGINE_SHARED_CCAMERA_H
#define ENGINE_SHARED_CCAMERA_H

#include "cGameObject.h"
#include <string>

#include "../Math/glm/mat4x4.hpp"
#include "../Shared/eCameraType.h"

namespace Engine 
{
	namespace Shared
	{
		class cCamera : public cGameObject
		{
		public:
			cCamera(const std::string& i_fileName = "");
			~cCamera();

			bool Move();
			bool Rotate();
			bool Update();

			glm::mat4x4 WorldToCamera();
			glm::mat4x4 CameraToScreen();

			eCameraType m_cameraType;
			float m_fieldOfView;
			float m_nearPlane;
			float m_farPlane;
			float m_movementSpeed;
			float m_rotationSpeed;
			float m_aspectRatio;

		private:
			glm::mat4x4 m_worldToCamera;
			glm::mat4x4 m_cameraToScreen;
		};
	}
}

#endif // !ENGINE_SHARED_CCAMERA_H

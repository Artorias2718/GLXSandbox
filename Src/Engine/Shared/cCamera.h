#ifndef ENGINE_SHARED_CCAMERA_H
#define ENGINE_SHARED_CCAMERA_H

#include "cGameObject.h"
#include <string>

#include "../Math/cMatrix_Transformation.h"
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

			Math::cMatrix_Transformation WorldToCamera();
			Math::cMatrix_Transformation CameraToScreen();

			eCameraType m_cameraType;
			float m_fieldOfView;
			float m_nearPlane;
			float m_farPlane;
			float m_movementSpeed;
			float m_rotationSpeed;
			float m_aspectRatio;

		private:
			Math::cMatrix_Transformation m_worldToCamera;
			Math::cMatrix_Transformation m_cameraToScreen;
		};
	}
}

#endif // !ENGINE_SHARED_CCAMERA_H

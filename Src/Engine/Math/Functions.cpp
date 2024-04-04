// Header Files
//=============

#include "Functions.h"
#include "../Shared/sTransform.h"

// Static Data Initialization
//===========================

glm::vec3 Engine::Math::right = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 Engine::Math::up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 Engine::Math::forward = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 Engine::Math::zero = glm::zero<glm::vec3>();

// Interface
//==========

glm::mat4 Engine::Math::CreateWorldToCameraTransform(
	const Shared::sTransform &i_transform)
{
	glm::mat4 viewToWorld = UpdateTransform(i_transform);

	// A camera can only ever have rotation and translation
	// and so a lot of simplifying assumptions can be made in order to create the inverse

	return glm::mat4(
		viewToWorld[0][0], viewToWorld[0][1], viewToWorld[0][2],
		-(viewToWorld[3][0] * viewToWorld[0][0]) - (viewToWorld[3][1] * viewToWorld[0][1]) - (viewToWorld[3][2] * viewToWorld[0][2]),

		viewToWorld[1][0], viewToWorld[1][1], viewToWorld[1][2],
		-(viewToWorld[3][0] * viewToWorld[1][0]) - (viewToWorld[3][1] * viewToWorld[1][1]) - (viewToWorld[3][2] * viewToWorld[1][2]),

		viewToWorld[2][0], viewToWorld[2][1], viewToWorld[2][2],
		-(viewToWorld[3][0] * viewToWorld[2][0]) - (viewToWorld[3][1] * viewToWorld[2][1]) - (viewToWorld[3][2] * viewToWorld[2][2]),

		0.0f, 0.0f, 0.0f, 1.0f
	);
}

glm::mat4 Engine::Math::CreateCameraToScreenTransform(
	const float i_fieldOfView_y, const float i_aspectRatio,
	const float i_z_nearPlane, const float i_z_farPlane)
{
	const float yScale = 1.0f / std::tan(i_fieldOfView_y * 0.5f);
	const float xScale = yScale / i_aspectRatio;
#if defined( D3D_API )
	const float zDistanceScale = i_z_farPlane / (i_z_nearPlane - i_z_farPlane);
	return glm::mat4(
		xScale, 0.0f, 0.0f, 0.0f,
		0.0f, yScale, 0.0f, 0.0f,
		0.0f, 0.0f, zDistanceScale, i_z_nearPlane * zDistanceScale,
		0.0f, 0.0f, -1.0f, 0.0f);
#elif defined( OGL_API )
	const float zDistanceScale = 1.0f / (i_z_nearPlane - i_z_farPlane);
	return glm::mat4(
		xScale, 0.0f, 0.0f, 0.0f,
		0.0f, yScale, 0.0f, 0.0f,
		0.0f, 0.0f, (i_z_nearPlane + i_z_farPlane) * zDistanceScale, (2.0f * i_z_nearPlane * i_z_farPlane) * zDistanceScale,
		0.0f, 0.0f, -1.0f, 0.0f);
#endif
}

glm::mat4 Engine::Math::UpdateTransform(const Shared::sTransform& transform)
{
	glm::mat4x4 result;

	const float _2x = 2.0f * transform.orientation.x;
	const float _2y = 2.0f * transform.orientation.y;
	const float _2z = 2.0f * transform.orientation.z;

	const float _2xx = _2x * transform.orientation.x;
	const float _2xy = _2x * transform.orientation.y;
	const float _2xz = _2x * transform.orientation.z;
	const float _2xw = _2x * transform.orientation.w;
	const float _2yy = _2y * transform.orientation.y;
	const float _2yz = _2y * transform.orientation.z;
	const float _2yw = _2y * transform.orientation.w;
	const float _2zz = _2z * transform.orientation.z;
	const float _2zw = _2z * transform.orientation.w;

	result[0][0] = transform.scale.x * (1.0f - _2yy - _2zz);
	result[1][0] = transform.scale.x * (_2xy + _2zw);
	result[2][0] = transform.scale.x * (_2xz - _2yw);
	result[3][0] = transform.position.x;

	result[0][1] = transform.scale.y * (_2xy - _2zw);
	result[1][1] = transform.scale.y * (1.0f - _2xx - _2zz);
	result[2][1] = transform.scale.y * (_2yz + _2xw);
	result[3][1] = transform.position.y;

	result[0][2] = transform.scale.z * (_2xz + _2yw);
	result[1][2] = transform.scale.z * (_2yz - _2xw);
	result[2][2] = transform.scale.z * (1.0f - _2xx - _2yy);
	result[3][2] = transform.position.z;

	result[0][3] = 0;
	result[1][3] = 0;
	result[2][3] = 0;
	result[3][3] = 1;

	return result;
}
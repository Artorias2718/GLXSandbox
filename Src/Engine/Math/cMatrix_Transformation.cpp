// Header Files
//=============

#include "cMatrix_Transformation.h"

#include <cmath>
#include "cQuaternion.h"
#include "cVector.h"
#include "../Shared/sTransform.h"

// Interface
//==========

Engine::Math::cMatrix_Transformation Engine::Math::cMatrix_Transformation::CreateWorldToCameraTransform(
	const Engine::Shared::sTransform& i_transform)
{
	cMatrix_Transformation transform_viewToWorld(i_transform);
	// A camera can only ever have rotation and translation
	// and so a lot of simplifying assumptions can be made in order to create the inverse
	return cMatrix_Transformation(
		transform_viewToWorld.m_00, transform_viewToWorld.m_01, transform_viewToWorld.m_02,
		-(transform_viewToWorld.m_30 * transform_viewToWorld.m_00) - (transform_viewToWorld.m_31 * transform_viewToWorld.m_01) - (transform_viewToWorld.m_32 * transform_viewToWorld.m_02),
		transform_viewToWorld.m_10, transform_viewToWorld.m_11, transform_viewToWorld.m_12,
		-(transform_viewToWorld.m_30 * transform_viewToWorld.m_10) - (transform_viewToWorld.m_31 * transform_viewToWorld.m_11) - (transform_viewToWorld.m_32 * transform_viewToWorld.m_12),
		transform_viewToWorld.m_20, transform_viewToWorld.m_21, transform_viewToWorld.m_22,
		-(transform_viewToWorld.m_30 * transform_viewToWorld.m_20) - (transform_viewToWorld.m_31 * transform_viewToWorld.m_21) - (transform_viewToWorld.m_32 * transform_viewToWorld.m_22),
		0.0f, 0.0f, 0.0f, 1.0f);
}

Engine::Math::cMatrix_Transformation Engine::Math::cMatrix_Transformation::CreateCameraToScreenTransform(
	const float i_fieldOfView_y, const float i_aspectRatio,
	const float i_z_nearPlane, const float i_z_farPlane)
{
	const float yScale = 1.0f / std::tan(i_fieldOfView_y * 0.5f);
	const float xScale = yScale / i_aspectRatio;
#if defined( D3D_API )
	const float zDistanceScale = i_z_farPlane / (i_z_nearPlane - i_z_farPlane);
	return cMatrix_Transformation(
		xScale, 0.0f, 0.0f, 0.0f,
		0.0f, yScale, 0.0f, 0.0f,
		0.0f, 0.0f, zDistanceScale, i_z_nearPlane * zDistanceScale,
		0.0f, 0.0f, -1.0f, 0.0f);
#elif defined( OGL_API )
	const float zDistanceScale = 1.0f / (i_z_nearPlane - i_z_farPlane);
	return cMatrix_Transformation(
		xScale, 0.0f, 0.0f, 0.0f,
		0.0f, yScale, 0.0f, 0.0f,
		0.0f, 0.0f, (i_z_nearPlane + i_z_farPlane) * zDistanceScale, (2.0f * i_z_nearPlane * i_z_farPlane) * zDistanceScale,
		0.0f, 0.0f, -1.0f, 0.0f);
#endif
}

Engine::Math::cMatrix_Transformation Engine::Math::cMatrix_Transformation::Transpose()
{
	return cMatrix_Transformation(
		m_00, m_01, m_02, m_03,
		m_10, m_11, m_12, m_13,
		m_20, m_21, m_22, m_23,
		m_30, m_31, m_32, m_33
	);
}

// Implementation
//===============

// Initialization / Shut Down
//---------------------------

Engine::Math::cMatrix_Transformation::cMatrix_Transformation(
	const float i_00, const float i_10, const float i_20, const float i_30,
	const float i_01, const float i_11, const float i_21, const float i_31,
	const float i_02, const float i_12, const float i_22, const float i_32,
	const float i_03, const float i_13, const float i_23, const float i_33)
	:
	m_00(i_00), m_10(i_10), m_20(i_20), m_30(i_30),
	m_01(i_01), m_11(i_11), m_21(i_21), m_31(i_31),
	m_02(i_02), m_12(i_12), m_22(i_22), m_32(i_32),
	m_03(i_03), m_13(i_13), m_23(i_23), m_33(i_33) {}

Engine::Math::cMatrix_Transformation::cMatrix_Transformation(const Engine::Shared::sTransform& i_transform)
	:
	m_30(i_transform.position.x), m_31(i_transform.position.y), m_32(i_transform.position.z),
	m_03(0.0f), m_13(0.0f), m_23(0.0f), m_33(1.0f)
{
	const float _2x = 2.0f * i_transform.orientation.x;
	const float _2y = 2.0f * i_transform.orientation.y;
	const float _2z = 2.0f * i_transform.orientation.z;

	const float _2xx = _2x * i_transform.orientation.x;
	const float _2xy = _2x * i_transform.orientation.y;
	const float _2xz = _2x * i_transform.orientation.z;
	const float _2xw = _2x * i_transform.orientation.w;
	const float _2yy = _2y * i_transform.orientation.y;
	const float _2yz = _2y * i_transform.orientation.z;
	const float _2yw = _2y * i_transform.orientation.w;
	const float _2zz = _2z * i_transform.orientation.z;
	const float _2zw = _2z * i_transform.orientation.w;

	m_00 = i_transform.scale.x * (1.0f - _2yy - _2zz);
	m_10 = i_transform.scale.x * (_2xy + _2zw);
	m_20 = i_transform.scale.x * (_2xz - _2yw);

	m_01 = i_transform.scale.y * (_2xy - _2zw);
	m_11 = i_transform.scale.y * (1.0f - _2xx - _2zz);
	m_21 = i_transform.scale.y * (_2yz + _2xw);

	m_02 = i_transform.scale.z * (_2xz + _2yw);
	m_12 = i_transform.scale.z * (_2yz - _2xw);
	m_22 = i_transform.scale.z * (1.0f - _2xx - _2yy);
}


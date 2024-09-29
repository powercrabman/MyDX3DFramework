#pragma once
#include "CAttribute.h"
#include "WindowsApp.h"

class CCamera : public CAttribute
{
public:
	CCamera(float inFov, float inNearPlane, float inFarPlane);
	virtual ~CCamera() = default;

	inline void SetPosition(const Vector3& inVector);
	inline void SetRotateDegree(float inYaw, float inPitch, float inRoll);
	inline void SetRotate(float inYaw, float inPitch, float inRoll);

	inline void AddPosition(const Vector3& inVector);
	inline void AddRotateDegree(float inYaw, float inPitch, float inRoll);
	inline void AddRotate(float inYaw, float inPitch, float inRoll);

	inline Vector3 GetPosition() const { return m_translate; };
	inline Quaternion GetRotate() const { return Quaternion::FromToRotation(sDefaultLook, m_lookTo); };

	inline void SetFOV(float inFov) { m_fov = inFov; }
	inline void SetFOVDegree(float inFov) { m_fov = ::XMConvertToRadians(inFov); }
	inline float GetFOV() const { return m_fov; }

	inline void SetNearPlane(float inNearPlane);
	inline void SetFarPlane(float inFarPlane);

	inline Matrix GetViewMatrix(const Vector3& inUp = Vector3::Up) const;
	inline Matrix GetPerspectiveMatrix() const;

private:
	inline static Vector3 sDefaultLook{ 0.f,0.f,1.f };

	Vector3 m_translate = Vector3::Zero;
	Vector3 m_lookTo = sDefaultLook;
	float m_fov = ::XMConvertToRadians(45.f);

	float m_nearPlane = 0.5f;
	float m_farPlane = 100.f;
};

inline CCamera::CCamera(float inFov, float inNearPlane, float inFarPlane)
	: m_fov(inFov)
	, m_nearPlane(inNearPlane)
	, m_farPlane(inFarPlane)
{
}

inline void CCamera::SetPosition(const Vector3& inVector)
{
	m_translate = inVector;
}

inline void CCamera::SetRotateDegree(float inYaw, float inPitch, float inRoll)
{
	inYaw = ::XMConvertToRadians(inYaw);
	inPitch = ::XMConvertToRadians(inPitch);
	inRoll = ::XMConvertToRadians(inRoll);
	m_lookTo = ::XMVector3Rotate(sDefaultLook, Quaternion::CreateFromYawPitchRoll(inYaw, inPitch, inRoll));
}

inline void CCamera::SetRotate(float inYaw, float inPitch, float inRoll)
{
	m_lookTo = ::XMVector3Rotate(sDefaultLook, Quaternion::CreateFromYawPitchRoll(inYaw, inPitch, inRoll));
}

inline void CCamera::AddPosition(const Vector3& inVector)
{
	m_translate += inVector;
}

inline void CCamera::AddRotateDegree(float inYaw, float inPitch, float inRoll)
{
	inYaw = ::XMConvertToRadians(inYaw);
	inPitch = ::XMConvertToRadians(inPitch);
	inRoll = ::XMConvertToRadians(inRoll);
	m_lookTo = ::XMVector3Rotate(m_lookTo, Quaternion::CreateFromYawPitchRoll(inYaw, inPitch, inRoll));
}

inline void CCamera::AddRotate(float inYaw, float inPitch, float inRoll)
{
	m_lookTo = ::XMVector3Rotate(m_lookTo, Quaternion::CreateFromYawPitchRoll(inYaw, inPitch, inRoll));
}

inline Matrix CCamera::GetViewMatrix(const Vector3& inUp) const
{
	return ::XMMatrixLookToLH(m_translate, m_lookTo, inUp);
}

inline Matrix CCamera::GetPerspectiveMatrix() const
{
	return ::XMMatrixPerspectiveFovLH(m_fov, WindowsApp::GetInst().GetAspectRatio(), m_nearPlane, m_farPlane);
}

inline void CCamera::SetNearPlane(float inNearPlane)
{
	m_nearPlane = inNearPlane;
}

inline void CCamera::SetFarPlane(float inFarPlane)
{
	m_farPlane = inFarPlane;
}


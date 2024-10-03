#pragma once
#include "CAttribute.h"
#include "CTransform.h"
#include "Renderer.h"

class CCamera : public CAttribute
{
public:
	CCamera(float inFov, float inNearPlane, float inFarPlane, const CTransform* inTrans);
	virtual ~CCamera();

	void SetOffsetPosition(const Vector3& inVector);
	void SetOffsetRotateDegree(float inYaw, float inPitch, float inRoll);
	void SetOffsetRotate(float inYaw, float inPitch, float inRoll);

	void AddOffsetPosition(const Vector3& inVector);
	void AddOffsetRotateDegree(float inYaw, float inPitch, float inRoll);
	void AddOffsetRotate(float inYaw, float inPitch, float inRoll);

	void ResetOffset();

	void SetFOV(float inFov) { m_fov = inFov; }
	void SetFOVDegree(float inFov) { m_fov = ::XMConvertToRadians(inFov); }
	float GetFOV() const { return m_fov; }

	void SetNearPlane(float inNearPlane);
	void SetFarPlane(float inFarPlane);

	Matrix GetViewMatrix(const Vector3& inUp = Vector3::Up) const;
	Matrix GetPerspectiveMatrix(float inAspectRatio) const;

	void RegisterToMainCamera();

	Vector3 GetPosition() { return m_offsetPosition + m_ownerTrans->GetPosition(); }

private:
	const CTransform* m_ownerTrans = nullptr;
	
	Vector3 m_offsetPosition = Vector3::Zero;
	Quaternion m_offsetRotator = Quaternion::Identity;

	float m_fov = ::XMConvertToRadians(45.f);
	float m_nearPlane = 0.5f;
	float m_farPlane = 100.f;
};

inline CCamera::CCamera(float inFov, float inNearPlane, float inFarPlane, const CTransform* inTrans)
	: m_fov(inFov)
	, m_nearPlane(inNearPlane)
	, m_farPlane(inFarPlane)
	, m_ownerTrans(inTrans)
{
}

inline void CCamera::SetOffsetPosition(const Vector3& inVector)
{
	m_offsetPosition = inVector;
}

inline void CCamera::SetOffsetRotateDegree(float inYaw, float inPitch, float inRoll)
{
	inYaw = ::XMConvertToRadians(inYaw);
	inPitch = ::XMConvertToRadians(inPitch);
	inRoll = ::XMConvertToRadians(inRoll);
	m_offsetRotator *= Quaternion::CreateFromYawPitchRoll(inYaw, inPitch, inRoll);
}

inline void CCamera::SetOffsetRotate(float inYaw, float inPitch, float inRoll)
{
	m_offsetRotator *= Quaternion::CreateFromYawPitchRoll(inYaw, inPitch, inRoll);
}

inline void CCamera::AddOffsetPosition(const Vector3& inVector)
{
	m_offsetPosition += inVector;
}

inline void CCamera::AddOffsetRotateDegree(float inYaw, float inPitch, float inRoll)
{
	inYaw = ::XMConvertToRadians(inYaw);
	inPitch = ::XMConvertToRadians(inPitch);
	inRoll = ::XMConvertToRadians(inRoll);
	m_offsetRotator *= Quaternion::CreateFromYawPitchRoll(inYaw, inPitch, inRoll);
}

inline void CCamera::AddOffsetRotate(float inYaw, float inPitch, float inRoll)
{
	m_offsetRotator *= Quaternion::CreateFromYawPitchRoll(inYaw, inPitch, inRoll);
}

inline void CCamera::ResetOffset()
{
	Vector3 m_offsetPosition = Vector3::Zero;
	Quaternion m_offsetRotator = Quaternion::Identity;
}

Matrix CCamera::GetViewMatrix(const Vector3& inUp) const
{
	Vector3 lookVec = ::XMVector3Rotate(Vector3{ 0.f,0.f,1.f }, m_offsetRotator * m_ownerTrans->GetRotate());
	return ::XMMatrixLookToLH(m_ownerTrans->GetPosition() + m_offsetPosition, lookVec, inUp);
}

inline Matrix CCamera::GetPerspectiveMatrix(float inAspectRatio) const
{
	return ::XMMatrixPerspectiveFovLH(m_fov, inAspectRatio, m_nearPlane, m_farPlane);
}

void CCamera::SetNearPlane(float inNearPlane)
{
	m_nearPlane = inNearPlane;
}

void CCamera::SetFarPlane(float inFarPlane)
{
	m_farPlane = inFarPlane;
}


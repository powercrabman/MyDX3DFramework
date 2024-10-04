#pragma once
#include "CAttribute.h"

class CTransform : public CAttribute
{
public:
	CTransform() = default;
	virtual ~CTransform() = default;

	void SetPosition(const Vector3& inVector);
	void SetRotateDegree(float inYaw, float inPitch, float inRoll);
	void SetRotateDegree(const Vector3& inYawPitchRoll);
	void SetRotate(float inYaw, float inPitch, float inRoll);
	void SetRotate(const Vector3& inYawPitchRoll);
	void SetRotateByDirection(Vector3 inDirection)
	{
		inDirection.Normalize();
		m_rotator = Quaternion::FromToRotation(Vector3{ 0,0,1 }, inDirection);
	}
	void SetRotateByLookAtPoint(const Vector3& inPoint)
	{
		Vector3 lookDirect = inPoint - m_translate;
		SetRotateByDirection(lookDirect);
	}
	void SetScale(const Vector3& inScale);

	void AddPosition(const Vector3& inVector);
	void AddRotateDegree(float inYaw, float inPitch, float inRoll);
	void AddRotate(float inYaw, float inPitch, float inRoll);
	void AddScale(const Vector3& inScale);

	Vector3 GetPosition() const { return m_translate; }
	Quaternion GetRotate() const { return m_rotator; }
	Vector3 GetScale() const { return m_scaling; }
	Vector3 GetForwardVector() const { return  ::XMVector3Rotate(Vector3{ 0.f,0.f,1.f }, m_rotator); }
	Vector3 GetBackwardVector() const { return  ::XMVector3Rotate(Vector3{ 0.f,0.f,-1.f }, m_rotator); }
	Vector3 GetRightVector() const { return  ::XMVector3Rotate(Vector3{ 1.f,0.f,0.f }, m_rotator); }
	Vector3 GetLeftVector() const { return  ::XMVector3Rotate(Vector3{ -1.f,0.f,0.f }, m_rotator); }
	Vector3 GetUpVector() const { return  ::XMVector3Rotate(Vector3{ 0.f,1.f,0.f }, m_rotator); }
	Vector3 GetDownVector() const { return  ::XMVector3Rotate(Vector3{ 0.f,-1.f,0.f }, m_rotator); }

	Matrix GetWorldMatrix() const;
	Matrix GetWorldMatrixInverse() const;

private:
	Vector3 m_translate = Vector3::Zero;
	Quaternion m_rotator = Quaternion::Identity;
	Vector3 m_scaling = Vector3::One;
};

inline void CTransform::SetPosition(const Vector3& inVector)
{
	m_translate = inVector;
}

inline void CTransform::SetRotateDegree(float inYaw, float inPitch, float inRoll)
{
	inYaw = ::XMConvertToRadians(inYaw);
	inPitch = ::XMConvertToRadians(inPitch);
	inRoll = ::XMConvertToRadians(inRoll);
	m_rotator = Quaternion::CreateFromYawPitchRoll(inYaw, inPitch, inRoll);
}

inline void CTransform::SetRotateDegree(const Vector3& inYawPitchRoll)
{
	m_rotator = Quaternion::CreateFromYawPitchRoll(
		::XMConvertToRadians(inYawPitchRoll.x),
		::XMConvertToRadians(inYawPitchRoll.y),
		::XMConvertToRadians(inYawPitchRoll.z)
	);
}

inline void CTransform::SetRotate(float inYaw, float inPitch, float inRoll)
{
	m_rotator = Quaternion::CreateFromYawPitchRoll(inYaw, inPitch, inRoll);
}

inline void CTransform::SetRotate(const Vector3& inYawPitchRoll)
{
	m_rotator = Quaternion::CreateFromYawPitchRoll(inYawPitchRoll);
}

inline void CTransform::SetScale(const Vector3& inScale)
{
	m_scaling = inScale;
}

inline void CTransform::AddPosition(const Vector3& inVector)
{
	m_translate += inVector;
}

inline void CTransform::AddRotateDegree(float inYaw, float inPitch, float inRoll)
{
	inYaw = ::XMConvertToRadians(inYaw);
	inPitch = ::XMConvertToRadians(inPitch);
	inRoll = ::XMConvertToRadians(inRoll);
	m_rotator *= Quaternion::CreateFromYawPitchRoll(inYaw, inPitch, inRoll);
}

inline void CTransform::AddRotate(float inYaw, float inPitch, float inRoll)
{
	m_rotator *= Quaternion::CreateFromYawPitchRoll(inYaw, inPitch, inRoll);
}

inline void CTransform::AddScale(const Vector3& inScale)
{
	m_scaling += inScale;
}

inline Matrix CTransform::GetWorldMatrix() const
{
	return ::XMMatrixAffineTransformation(
		m_scaling,
		Vector3::Zero,
		m_rotator,
		m_translate
	);
}

inline Matrix CTransform::GetWorldMatrixInverse() const
{
	return ::XMMatrixInverse(nullptr, GetWorldMatrix());
}


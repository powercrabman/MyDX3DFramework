#pragma once
#include "CAttribute.h"

class CTransform : public CAttribute
{
public:
	CTransform() = default;
	virtual ~CTransform() = default;

	inline void SetPosition(const Vector3& inVector);
	inline void SetRotateDegree(float inYaw, float inPitch, float inRoll);
	inline void SetRotate(float inYaw, float inPitch, float inRoll);
	inline void SetScale(const Vector3& inScale);

	inline void AddPosition(const Vector3& inVector);
	inline void AddRotateDegree(float inYaw, float inPitch, float inRoll);
	inline void AddRotate(float inYaw, float inPitch, float inRoll);
	inline void AddScale(const Vector3& inScale);

	inline Vector3 GetPosition() const { return m_translate; }
	inline Quaternion GetRotate() const { return m_rotate; }
	inline Vector3 GetScale() const { return m_scaling; }

	inline Matrix GetWorldMatrix() const;
	inline Matrix GetWorldMatrixInverse() const;

private:
	Vector3 m_translate = Vector3::Zero;
	Quaternion m_rotate = Quaternion::Identity;
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
	m_rotate = Quaternion::CreateFromYawPitchRoll(inYaw, inPitch, inRoll);
}

inline void CTransform::SetRotate(float inYaw, float inPitch, float inRoll)
{
	m_rotate = Quaternion::CreateFromYawPitchRoll(inYaw, inPitch, inRoll);
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
	m_rotate *= Quaternion::CreateFromYawPitchRoll(inYaw, inPitch, inRoll);
}

inline void CTransform::AddRotate(float inYaw, float inPitch, float inRoll)
{
	m_rotate *= Quaternion::CreateFromYawPitchRoll(inYaw, inPitch, inRoll);
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
		m_rotate,
		m_translate
	);
}

inline Matrix CTransform::GetWorldMatrixInverse() const
{
	return ::XMMatrixInverse(nullptr, GetWorldMatrix());
}


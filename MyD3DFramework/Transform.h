#pragma once

class Transform final
{
public:
	Transform() = default;
	~Transform() = default;

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

private:
	Vector3 m_translate = Vector3::Zero;
	Quaternion m_rotate = Quaternion::Identity;
	Vector3 m_scaling = Vector3::One;
};

inline void Transform::SetPosition(const Vector3& inVector)
{
	m_translate = inVector;
}

inline void Transform::SetRotateDegree(float inYaw, float inPitch, float inRoll)
{
	inYaw = ::XMConvertToRadians(inYaw);
	inPitch = ::XMConvertToRadians(inPitch);
	inRoll = ::XMConvertToRadians(inRoll);
	m_rotate = Quaternion::CreateFromYawPitchRoll(inYaw, inPitch, inRoll);
}

inline void Transform::SetRotate(float inYaw, float inPitch, float inRoll)
{
	m_rotate = Quaternion::CreateFromYawPitchRoll(inYaw, inPitch, inRoll);
}

inline void Transform::SetScale(const Vector3& inScale)
{
	m_scaling = inScale;
}

inline void Transform::AddPosition(const Vector3& inVector)
{
	m_translate += inVector;
}

inline void Transform::AddRotateDegree(float inYaw, float inPitch, float inRoll)
{
	inYaw = ::XMConvertToRadians(inYaw);
	inPitch = ::XMConvertToRadians(inPitch);
	inRoll = ::XMConvertToRadians(inRoll);
	m_rotate *= Quaternion::CreateFromYawPitchRoll(inYaw, inPitch, inRoll);
}

inline void Transform::AddRotate(float inYaw, float inPitch, float inRoll)
{
	m_rotate *= Quaternion::CreateFromYawPitchRoll(inYaw, inPitch, inRoll);
}

inline void Transform::AddScale(const Vector3& inScale)
{
	m_scaling += inScale;
}

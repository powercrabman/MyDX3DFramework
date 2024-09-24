#pragma once
#include "IGameObject.h"

class CameraObject : IGameObject
{
public:
	CameraObject() = default;
	virtual ~CameraObject() = default;

	void Init() override;
	void Update(float inDeltaTime) override;

	inline void SetPosition(const Vector3& inPosition);
	inline void SetCameraLookAt(const Vector3& inCameraLookVector);
	inline void SetCameraLootTo(const Vector3& inPoint);
	inline void SetCameraUpAxis(const Vector3& inAxis);
	inline void SetFov(float inFov);

	inline void AddPosition(const Vector3& inPosition)
	{
		m_position += inPosition;
	}
	inline void AddRotation(float inYaw, float inRoll, float inPitch)
	{
		m_lookVec = ::XMVector3Rotate(m_lookVec, Quaternion::CreateFromYawPitchRoll(inYaw, inRoll, inPitch));

	}

private:
	Vector3 m_lookVec = Vector3{0.f,0.f,1.f};
	Vector3 m_position = Vector3::Zero;
	Vector3 m_cameraUpAxis = Vector3::Up;

	float m_fov = ::XMConvertToRadians(45.f);
};



inline void CameraObject::SetPosition(const Vector3& inPosition)
{
	m_position = inPosition;
}

inline void CameraObject::SetCameraLookAt(const Vector3& inCameraLookVector)
{
	m_lookVec = inCameraLookVector;
}

inline void CameraObject::SetCameraLootTo(const Vector3& inPoint)
{
	m_lookVec = inPoint - m_position;
	m_lookVec.Normalize();
}

inline void CameraObject::SetCameraUpAxis(const Vector3& inAxis)
{
	m_cameraUpAxis = inAxis;
}

inline void CameraObject::SetFov(float inFov)
{
	m_fov = inFov;
}

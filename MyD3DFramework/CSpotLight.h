#pragma once
#include "CLight.h"

class CSpotLight : public CLight
{
public:
	CSpotLight() = default;
	virtual ~CSpotLight() = default;
	
	inline void SetPosition(const Vector3& inPosition);
	inline void SetDirection(const Vector3& inDirection);
	inline void SetAttenuation(const Vector3& inAttenuation);
	inline void SetRange(float inRange);

	inline Vector3 GetPosition() const { return m_position; }
	inline Vector3 GetDirection() const { return m_direction; }
	inline Vector3 GetAttenuation() const { return m_attenuation; }
	inline float GetRange() const { return m_range; }

private:
	Vector3 m_position = {};
	Vector3 m_direction = {};
	
	Vector3 m_attenuation = {};
	float m_range = 100.f;
};

inline void CSpotLight::SetPosition(const Vector3& inPosition)
{
	m_position = inPosition;
}

inline void CSpotLight::SetDirection(const Vector3& inDirection)
{
	m_direction = inDirection;
}

inline void CSpotLight::SetAttenuation(const Vector3& inAttenuation)
{
	m_attenuation = inAttenuation;
}

inline void CSpotLight::SetRange(float inDist)
{
	m_range = inDist;
}

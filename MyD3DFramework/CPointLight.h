#pragma once
#include "CLight.h"

class CPointLight : public CLight
{
public:
	CPointLight() = default;
	virtual ~CPointLight() = default;

	inline void SetPosition(const Vector3& inPosition);
	inline void SetAttenuation(const Vector3& inAttenuation);
	inline void SetRange(float inRange);

	inline Vector3 GetPosition() const { return m_position; }
	inline Vector3 GetAttenuation() const { return m_attenuation; }
	inline float GetRange() const { return m_range; }

private:
	Vector3 m_position = {};
	Vector3 m_attenuation = {};
	float m_range = 100.f;
};

inline void CPointLight::SetPosition(const Vector3& inPosition)
{
	m_position = inPosition;
}

inline void CPointLight::SetAttenuation(const Vector3& inAttenuation)
{
	m_attenuation = inAttenuation;
}

inline void CPointLight::SetRange(float inDist)
{
	m_range = inDist;
}

#pragma once
#include "CLight.h"

class CDirectionalLight : public CLight
{
public:
	CDirectionalLight() = default;
	virtual ~CDirectionalLight() = default;

	inline void SetDirection(const Vector3& inDirection);

	inline Vector3 GetDirection() const { return m_direction; }

private:
	Vector3 m_direction = {};
};

inline void CDirectionalLight::SetDirection(const Vector3& inDirection)
{
	m_direction = inDirection;
}

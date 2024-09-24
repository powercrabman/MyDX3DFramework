#pragma once
#include "LightObject.h"

class DirectionalLight : public LightObject
{
public:
	DirectionalLight(eLightType inType);
	virtual ~DirectionalLight() = default;

	inline void SetDirection(const Vector3& inDir);
	inline Vector3 GetDirection() const;

private:
	Vector3 m_direction = Vector3::One;

	// LightObject을(를) 통해 상속됨
	void Init() override;
	void Update(float inDeltaTime) override;
};

inline DirectionalLight::DirectionalLight(eLightType inType)
	: LightObject(inType)
{
}

inline void DirectionalLight::SetDirection(const Vector3& inDir)
{
	m_direction = inDir;
}

inline Vector3 DirectionalLight::GetDirection() const 
{
	return m_direction;
}

void DirectionalLight::Init()
{
}

void DirectionalLight::Update(float inDeltaTime)
{
}

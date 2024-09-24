#pragma once
#include "IGameObject.h"

class LightObject : public IGameObject
{
public:
	LightObject(eLightType inType);
	virtual ~LightObject() = default;

	inline void SetIntensity(float inIntensity);
	inline void AddIntensity(float inIntensity);
	inline float GetIntensity() const { return m_intensity; }

	inline void SetLightColor(const LightColor& inLightColor);
	inline LightColor GetLightColor() const;

private:
	inline void Clamp();

private:
	LightColor m_lightColor = {};

	eLightType m_type = eLightType::Unknown;
	float m_intensity = 1.f;
};

inline LightObject::LightObject(eLightType inType)
	: m_type(inType)
{
	assert(m_type != eLightType::Unknown);
}

inline void LightObject::SetIntensity(float inIntensity)
{
	m_intensity = inIntensity;
	Clamp();
}

inline void LightObject::AddIntensity(float inIntensity)
{
	m_intensity += inIntensity;
	Clamp();
}

inline void LightObject::SetLightColor(const LightColor& inLightColor)
{
	m_lightColor = inLightColor;
}

inline LightColor LightObject::GetLightColor() const
{
	return m_lightColor;
}

inline void LightObject::Clamp() 
{
	m_intensity = std::clamp(m_intensity, 0.f, 1.f);
}

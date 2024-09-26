#pragma once
#include "CAttribute.h"

class CLight : public CAttribute
{
public:
	virtual ~CLight() = default;

protected:
	CLight() = default;

	inline void SetAmbient(Color inColor) { m_ambient = inColor; ColorClamp(m_ambient); }
	inline void SetDiffuse(Color inColor) { m_diffuse = inColor; ColorClamp(m_diffuse); }
	inline void SetSpecualer(Color inColor) { m_specular = inColor; ColorClamp(m_specular); }
	inline void SetIntencity(float inIntencity) { m_intencity = inIntencity; Clamp(m_intencity); }
	inline void AddIntencity(float inIntencity) { m_intencity += inIntencity; Clamp(m_intencity); }

	inline Color GetAmbient() const { return m_ambient; }
	inline Color GetDiffuse() const { return m_diffuse; }
	inline Color GetSpecular() const { return m_specular; }
	inline float GetIntencity() const { return m_intencity; }

public:
	inline void ColorClamp(Color& inLightColor)
	{
		Clamp(inLightColor.x);
		Clamp(inLightColor.y);
		Clamp(inLightColor.z);
		Clamp(inLightColor.w);
	}

	inline void Clamp(float& inValue)
	{
		inValue = std::clamp(inValue, 0.f, 1.f);
	}

private:
	Color m_ambient = {};
	Color m_diffuse = {};
	Color m_specular = {};

	//ºûÀÇ °­µµ
	float m_intencity = 1.f;
};


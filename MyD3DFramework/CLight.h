#pragma once
#include "CTransform.h"

class CLight : public CAttribute
{
public:
	CLight(eLightType inType, const CTransform* inOwnerTrans);
	virtual ~CLight();;

	virtual void Initialize() override;

public:
	inline void SetAmbient(const Color& inColor) { m_ambient = inColor; ColorClamp(m_ambient); }
	inline void SetDiffuse(const Color& inColor) { m_diffuse = inColor; ColorClamp(m_diffuse); }
	inline void SetSpecular(const Color& inColor) { m_specular = inColor; ColorClamp(m_specular); }
	inline void AddAmbient(const Color& inColor) { m_ambient += inColor; ColorClamp(m_ambient); }
	inline void AddDiffuse(const Color& inColor) { m_diffuse += inColor; ColorClamp(m_diffuse); }
	inline void AddSpecualer(const Color& inColor) { m_specular += inColor; ColorClamp(m_specular); }

	inline void SetIntensity(float inIntencity) { m_intensity = inIntencity; Clamp(m_intensity); }
	inline void AddIntensity(float inIntencity) { m_intensity += inIntencity; Clamp(m_intensity); }

	inline Color GetAmbient() const { return m_ambient; }
	inline Color GetDiffuse() const { return m_diffuse; }
	inline Color GetSpecular() const { return m_specular; }
	inline float GetIntensity() const { return m_intensity; }

	inline void SetOffsetDirection(const Vector3& inDirection);
	inline Vector3 GetDirection() const;

	inline void SetOffsetPosition(const Vector3& inVector);
	inline Vector3 GetPosition() const;

	inline void SetAttenuation(const Vector3& inVector);
	inline Vector3 GetAttenuation() const;

	inline void SetRange(float inRange);
	inline float GetRange() const;

	inline eLightType GetLightType() const;

	inline void SetExponent(float inExponent);
	inline float GetExpoent() const;

private:

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
	eLightType m_type;

	Color m_ambient = {};
	Color m_diffuse = {};
	Color m_specular = {};

	const CTransform* m_ownerTrans = nullptr;

	Vector3 m_offsetDirection = Vector3{ 0.f,0.f,1.f };
	Vector3 m_offsetPosition = Vector3::Zero;

	//감쇠 계수
	Vector3 m_attenuation = { 0.f,1.f,0.f };

	//최대 거리
	float m_range = 100.f;

	//스폿라이트 범위
	float m_exponent = 16.f;

	//빛의 강도
	float m_intensity = 1.f;
};

inline void CLight::SetOffsetDirection(const Vector3& inDirection)
{
	m_offsetDirection = inDirection;
	m_offsetDirection.Normalize();
}

inline Vector3 CLight::GetDirection() const
{
	return ::XMVector3Rotate(m_offsetDirection, m_ownerTrans->GetRotate());
}

inline void CLight::SetOffsetPosition(const Vector3& inVector)
{
	m_offsetPosition = inVector;
}

inline Vector3 CLight::GetPosition() const
{
	return m_ownerTrans->GetPosition() + m_offsetPosition;
}

inline void CLight::SetAttenuation(const Vector3& inVector)
{
	m_attenuation = inVector;
}

inline Vector3 CLight::GetAttenuation() const
{
	return m_attenuation;
}

inline void CLight::SetRange(float inRange)
{
	m_range = inRange;
}

inline float CLight::GetRange() const
{
	return m_range;
}

inline eLightType CLight::GetLightType() const
{
	return m_type;
}

inline void CLight::SetExponent(float inExponent)
{
	m_exponent = inExponent;
}

inline float CLight::GetExpoent() const
{
	return m_exponent;
}


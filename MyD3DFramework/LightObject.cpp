#include "pch.h"
#include "LightObject.h"
#include "CLight.h"

void LightObject::Initialize()
{
	m_trans = CreateComponent<CTransform>();
	m_light = CreateComponent<CLight>(eLightType::DirectionalLight, m_trans);
	m_light->SetDiffuse(Color{ Colors::LightYellow });
	m_light->SetAmbient(Color{ Colors::LightYellow });
	m_light->SetSpecular(Color{ Colors::White * 0.3f });

	m_trans->SetRotateByDirection(Vector3{ 1,-2,2});
}

void LightObject::Update()
{
}

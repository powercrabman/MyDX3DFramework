#include "pch.h"
#include "CLight.h"
#include "Renderer.h"

CLight::CLight(eLightType inType, const CTransform* inOwnerTrans)
	: m_type(inType)
	, m_ownerTrans(inOwnerTrans)
{
}

void CLight::Initialize()
{
	Renderer::GetInst().RegisterCLight(this);
}

CLight::~CLight()
{
	Renderer::GetInst().RegisterCLight(this);
}

#include "pch.h"
#include "Ground.h"
#include "CTransform.h"
#include "Renderer.h"

Ground::Ground()
{
	m_trans = CreateComponent<CTransform>();
	m_meshCmp = CreateComponent<CMeshRenderer>(m_trans);

	m_meshCmp->SetMaterial(Renderer::sBasicMaterialKey);
	m_meshCmp->SetMesh(Renderer::sCubeMeshKey);
}

Ground::~Ground()
{
}

void Ground::Initialize()
{
	m_trans->SetScale(Vector3{ 30.f,1.f,30.f });
	m_trans->SetPosition(Vector3{ 0.f,-5.f,0.f });
}

void Ground::Update()
{
}

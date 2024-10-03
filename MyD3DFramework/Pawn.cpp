#include "pch.h"
#include "Pawn.h"
#include "CKeyInput.h"
#include "CTransform.h"
#include "CMeshRenderer.h"
#include "Renderer.h"

Pawn::Pawn()
{
	m_trans = CreateComponent<CTransform>();

	m_meshCmp = CreateComponent<CMeshRenderer>(m_trans);
	m_meshCmp->SetMaterial(Renderer::sBasicMaterialKey);
	m_meshCmp->SetMesh(Renderer::sCubeMeshKey);
}

Pawn::~Pawn()
{
}

void Pawn::Initialize()
{
	m_trans->SetPosition(Vector3{ 0,0,10 });
}

void Pawn::Update()
{
	m_trans->AddRotateDegree(30.f * DELTA_TIME, 0.f, 0.f);
}

#include "pch.h"
#include "Pawn.h"
#include "CKeyInput.h"
#include "CTransform.h"

Pawn::Pawn()
{
	m_trans = CreateComponent<CTransform>();
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
	m_trans->AddRotateDegree(45.f * DELTA_TIME, 0.f, 0.f);
}

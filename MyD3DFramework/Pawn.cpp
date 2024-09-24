#include "pch.h"
#include "Pawn.h"
#include "Renderer.h"

Pawn::Pawn()
{
	Transform& t = GetTransform();
	t.SetPosition(Vector3{ 0.f,0.f,5.f });

	SetMesh(Renderer::CubeMeshKey);
	SetMaterial(Renderer::BasicMaterialKey);
}

void Pawn::Init()
{
}

void Pawn::Update(float inDeltaTime)
{
	//if (KEY_HOLD(eKeyCode::J))
	//{
	//
	//}
}

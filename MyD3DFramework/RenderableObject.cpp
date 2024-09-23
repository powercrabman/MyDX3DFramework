#include "pch.h"
#include "RenderableObject.h"
#include "Renderer.h"

void RenderableObject::Init()
{
	m_mesh = Renderer::GetInst().GetMesh(Renderer::CubeMeshKey);
	m_material = Renderer::GetInst().GetMaterial(Renderer::BasicMaterialKey);
	m_transform.SetPosition(Vector3{ 0.f,0.f,5.f });
}

void RenderableObject::Update(float inDeltaTime)
{
	
}

void RenderableObject::Render()
{
	//???
}
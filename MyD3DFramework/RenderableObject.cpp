#include "pch.h"
#include "RenderableObject.h"
#include "Renderer.h"

RenderableObject::RenderableObject()
{
}

void RenderableObject::SetMesh(const std::wstring& inMeshKey) 
{
	m_mesh = Renderer::GetInst().GetMesh(inMeshKey);
}

void RenderableObject::SetMaterial(const std::wstring& inMaterialKey) 
{
	m_material = Renderer::GetInst().GetMaterial(inMaterialKey);
}

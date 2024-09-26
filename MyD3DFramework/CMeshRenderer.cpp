#include "pch.h"
#include "Renderer.h"
#include "CMeshRenderer.h"

void CMeshRenderer::SetMesh(const std::wstring& inMeshKey)
{
	m_mesh = Renderer::GetInst().GetMesh(inMeshKey);
}

void CMeshRenderer::SetMaterial(const std::wstring& inMaterialKey)
{
	m_material = Renderer::GetInst().GetMaterial(inMaterialKey);
}

void CMeshRenderer::RegisterToRenderer()
{
	ASSERT(m_mesh, "메시가 설정되지 않았습니다.");
	Renderer::GetInst().RegisterCMeshRenderer(this); 
}


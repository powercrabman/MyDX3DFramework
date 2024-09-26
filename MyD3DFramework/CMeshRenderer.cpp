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
	ASSERT(m_mesh, "�޽ð� �������� �ʾҽ��ϴ�.");
	Renderer::GetInst().RegisterCMeshRenderer(this); 
}


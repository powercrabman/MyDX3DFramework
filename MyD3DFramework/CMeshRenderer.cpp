#include "pch.h"
#include "Renderer.h"
#include "CMeshRenderer.h"

void CMeshRenderer::SetMesh(const std::string& inMeshKey)
{
	m_mesh = Renderer::GetInst().GetMesh(inMeshKey);
}

void CMeshRenderer::SetMaterial(const std::string& inMaterialKey)
{
	m_material = Renderer::GetInst().GetMaterial(inMaterialKey);
}

void CMeshRenderer::RegisterToRenderer()
{
	ASSERT(m_mesh, "�޽ð� �������� �ʾҽ��ϴ�.");
	Renderer::GetInst().RegisterCMeshRenderer(this); 
}

CMeshRenderer::~CMeshRenderer()
{
	Renderer::GetInst().UnRegisterCMeshRenderer(this);
}

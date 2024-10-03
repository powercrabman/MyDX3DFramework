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
	Renderer::GetInst().RegisterCMeshRenderer(this);
}

CMeshRenderer::~CMeshRenderer()
{
	Renderer::GetInst().UnRegisterCMeshRenderer(this);
}

CMeshRenderer::CMeshRenderer(const CTransform* inOwnerTrans)
	: m_ownerTrans(inOwnerTrans)
{
	ASSERT(inOwnerTrans, "Ʈ�������� nullptr �Դϴ�.");
}

void CMeshRenderer::Initialize()
{
	RegisterToRenderer();
}

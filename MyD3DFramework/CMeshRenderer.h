#pragma once
#include "CAttribute.h"

class Mesh;
class Material;
class CTransform;

class CMeshRenderer : public CAttribute
{
	friend class Renderer;
public:
	CMeshRenderer(const CTransform* inOwnerTrans);
	virtual ~CMeshRenderer();

	void RegisterToRenderer();
	
	void SetMesh(const std::string& inMeshKey);
	void SetMaterial(const std::string& inMaterialKey);

	const Mesh* GetMesh() const;
	const CTransform* GetTransform() const;
	const Material* GetMaterial() const;

private:
	void SetIndex(uint64 inIdx);
	uint64 GetIndex() const { return m_index; }

private:
	uint64 m_index = UINT64_MAX;

	Mesh* m_mesh = nullptr;
	Material* m_material = nullptr;
	const CTransform* m_ownerTrans = nullptr;
};

const Mesh* CMeshRenderer::GetMesh() const
{
	ASSERT(m_mesh, "메시가 설정되지 않음.");
	return m_mesh;
}

const CTransform* CMeshRenderer::GetTransform() const
{
	return m_ownerTrans;
}

const Material* CMeshRenderer::GetMaterial() const
{
	ASSERT(m_material, "머테리얼이 설정되지 않음");
	return m_material;
}

void CMeshRenderer::SetIndex(uint64 inIdx)
{
	m_index = inIdx;
}

CMeshRenderer::CMeshRenderer(const CTransform* inOwnerTrans)
	: m_ownerTrans(inOwnerTrans)
{
	ASSERT(inOwnerTrans, "트랜스폼이 nullptr 입니다.");
}


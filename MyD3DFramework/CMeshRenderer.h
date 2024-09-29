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
	
	void SetMesh(const std::wstring& inMeshKey);
	void SetMaterial(const std::wstring& inMaterialKey);

	inline const Mesh* GetMesh() const;
	inline const CTransform* GetTransform() const;
	inline const Material* GetMaterial() const;

private:
	inline void SetIndex(uint64 inIdx);
	inline uint64 GetIndex() const { return m_index; }

private:
	uint64 m_index = UINT64_MAX;

	Mesh* m_mesh = nullptr;
	Material* m_material = nullptr;
	const CTransform* m_ownerTrans = nullptr;
};

inline const Mesh* CMeshRenderer::GetMesh() const
{
	ASSERT(m_mesh, "메시가 설정되지 않음.");
	return m_mesh;
}

inline const CTransform* CMeshRenderer::GetTransform() const
{
	return m_ownerTrans;
}

inline const Material* CMeshRenderer::GetMaterial() const
{
	ASSERT(m_material, "머테리얼이 설정되지 않음");
	return m_material;
}

inline void CMeshRenderer::SetIndex(uint64 inIdx)
{
	m_index = inIdx;
}

inline CMeshRenderer::CMeshRenderer(const CTransform* inOwnerTrans)
	: m_ownerTrans(inOwnerTrans)
{
	ASSERT(inOwnerTrans, "트랜스폼이 nullptr 입니다.");
}


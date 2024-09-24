#pragma once
#include "IGameObject.h"

class RenderableObject : public IGameObject
{
public:
	RenderableObject();
	virtual ~RenderableObject() = default;

	void SetMesh(const std::wstring& inMeshKey);
	void SetMaterial(const std::wstring& inMaterialKey);

	NODISCARD Mesh* GetMesh() const { assert(m_mesh); return m_mesh; }
	NODISCARD Material* GetMaterial() const { assert(m_material); return m_material; }
	NODISCARD const Transform& GetTransform() const { return m_transform; }
	NODISCARD Transform& GetTransform() { return m_transform; }

private:
	Mesh* m_mesh = nullptr;
	Material* m_material = nullptr;
	Transform m_transform = {};
};


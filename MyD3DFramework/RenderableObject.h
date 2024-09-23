#pragma once
#include "GameObject.h"

class RenderableObject : public GameObject
{
public:
	RenderableObject() = default;
	virtual ~RenderableObject() = default;

	virtual void Init() override;
	virtual void Update(float inDeltaTime) override;
	virtual void Render() override;

	Mesh* GetMesh() const { assert(m_mesh); return m_mesh; }
	Material* GetMaterial() const { assert(m_material); return m_material; }
	const Transform& GetTransform() const { return m_transform; }

private:
	Mesh* m_mesh = nullptr;
	Material* m_material = nullptr;

	Transform m_transform = {};
};


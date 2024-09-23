#pragma once

class RenderableObject
{
public:
	RenderableObject() = default;
	virtual ~RenderableObject() = default;

private:
	Mesh m_mesh = {};
	Transform m_transform = {};
};


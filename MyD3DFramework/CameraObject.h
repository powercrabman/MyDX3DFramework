#pragma once
#include "GameObject.h"

class CTransform;
class CCamera;

class CameraObject : public GameObject
{
public:
	CameraObject();
	virtual ~CameraObject() = default;

	void Initialize() override;
	void Update(float inDeltaTime) override;

private:
	CTransform* m_trans = {};
	CCamera* m_camera = {};
};

#pragma once
#include "GameObject.h"
#include "ComponentSet.h"

class LightObject : public GameObject
{
public:
	void Initialize() override;
	void Update() override;

private:
	CTransform* m_trans = nullptr;
	CLight* m_light = nullptr;
};


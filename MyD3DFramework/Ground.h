#pragma once
#include "GameObject.h"
#include "ComponentSet.h"

class Ground : public GameObject
{
public:
	Ground();
	~Ground();

	void Initialize() override;
	void Update() override;

private:
	CTransform* m_trans = nullptr;
	CMeshRenderer* m_meshCmp = nullptr;
};

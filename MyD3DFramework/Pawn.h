#pragma once
#include "GameObject.h"
#include "ComponentSet.h"

class Pawn : public GameObject
{
public:
	Pawn();
	~Pawn();

	void Initialize() override;
	void Update() override;

private:
	CTransform* m_trans = nullptr;
	CKeyInput* m_keyInput = nullptr;
	CMeshRenderer* m_meshCmp = nullptr;
};



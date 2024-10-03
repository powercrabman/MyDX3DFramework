#pragma once
#include "GameObject.h"

class CTransform;
class CKeyInput;

class Pawn : public GameObject
{
public:
	Pawn();
	~Pawn();

	void Initialize() override;
	void Update() override;

private:


private:
	CTransform* m_trans = nullptr;
	CKeyInput* m_keyInput = nullptr;
};


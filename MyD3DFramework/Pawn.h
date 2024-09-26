#pragma once
#include "GameObject.h"

class Pawn : public GameObject
{
public:
	Pawn() = default;
	virtual ~Pawn() = default;

	void Initialize() override;
	void Update(float inDeltaTime) override;

private:
};

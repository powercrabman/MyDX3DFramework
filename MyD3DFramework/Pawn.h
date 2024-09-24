#pragma once
#include "RenderableObject.h"

class Pawn : public RenderableObject
{
public:
	Pawn();
	virtual ~Pawn() = default;

	void Init() override;
	void Update(float inDeltaTime) override;

private:
};


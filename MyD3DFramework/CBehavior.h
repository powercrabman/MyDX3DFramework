#pragma once
#include "Component.h"

class CBehavior : public Component
{
public:
	virtual ~CBehavior() = default;
	virtual void Update() = 0;

protected:
	CBehavior() = default;
};


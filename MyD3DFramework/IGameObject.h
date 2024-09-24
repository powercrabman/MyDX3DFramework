#pragma once

class IGameObject
{
public:
	IGameObject() = default;
	virtual ~IGameObject() = default;

	virtual void Init() abstract;
	virtual void Update(float inDeltaTime) abstract;
};



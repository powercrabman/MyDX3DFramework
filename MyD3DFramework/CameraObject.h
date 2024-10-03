#pragma once
#include "GameObject.h"

class CTransform;
class CCamera;
class CKeyInput;

class CameraObject : public GameObject
{
public:
	CameraObject();
	virtual ~CameraObject() = default;

	void Initialize() override;
	void Update() override;
	
private:
	void MoveLeft();
	void MoveRight();
	void MoveForward();
	void MoveBackward();
	void MoveUp();
	void MoveDown();

	void TurnLeft();
	void TurnRight();
	void TurnDown();
	void TurnUp();

private:
	CTransform* m_trans = {};
	CCamera* m_camera = {};
	CKeyInput* m_input = {};

	constexpr inline static float sTurnSpeed = 45.f;
	constexpr inline static float sMoveSpeed = 1.f;
};

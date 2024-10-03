#include "pch.h"
#include "CameraObject.h"
#include "CCamera.h"
#include "CTransform.h"
#include "CKeyInput.h"

CameraObject::CameraObject()
{
	m_trans = CreateComponent<CTransform>();
	m_camera = CreateComponent<CCamera>(45.f, 0.5f, 100.f, m_trans);
	m_input = CreateComponent<CKeyInput>();

	/* ÀÎÇ² ¼³Á¤ */
	m_input->BindAction(eKeyCode::Left, eKeyState::Hold, this, &CameraObject::MoveLeft);
	m_input->BindAction(eKeyCode::Right, eKeyState::Hold, this, &CameraObject::MoveRight);
	m_input->BindAction(eKeyCode::Up, eKeyState::Hold, this, &CameraObject::MoveForward);
	m_input->BindAction(eKeyCode::Down, eKeyState::Hold, this, &CameraObject::MoveBackward);
	m_input->BindAction(eKeyCode::Space, eKeyState::Hold, this, &CameraObject::MoveUp);
	m_input->BindAction(eKeyCode::L_Shift, eKeyState::Hold, this, &CameraObject::MoveDown);
	m_input->BindAction(eKeyCode::A, eKeyState::Hold, this, &CameraObject::TurnLeft);
	m_input->BindAction(eKeyCode::D, eKeyState::Hold, this, &CameraObject::TurnRight);
	m_input->BindAction(eKeyCode::W, eKeyState::Hold, this, &CameraObject::TurnUp);
	m_input->BindAction(eKeyCode::S	, eKeyState::Hold, this, &CameraObject::TurnDown);

}

void CameraObject::Initialize()
{
	m_trans->SetPosition(Vector3::Zero);
	m_trans->SetScale(Vector3::One);
	m_trans->SetRotate(Vector3::One);
}

void CameraObject::Update()
{
}

void CameraObject::MoveLeft()
{
	m_trans->AddPosition(m_trans->GetLeftVector() * sMoveSpeed * DELTA_TIME);
}

void CameraObject::MoveRight()
{
	m_trans->AddPosition(m_trans->GetRightVector() * sMoveSpeed * DELTA_TIME);
}

void CameraObject::MoveForward()
{
	m_trans->AddPosition(m_trans->GetForwardVector() * sMoveSpeed * DELTA_TIME);
}

void CameraObject::MoveBackward()
{
	m_trans->AddPosition(m_trans->GetBackwardVector() * sMoveSpeed * DELTA_TIME);
}

void CameraObject::MoveUp()
{
	m_trans->AddPosition(m_trans->GetUpVector() * sMoveSpeed * DELTA_TIME);
}

void CameraObject::MoveDown()
{
	m_trans->AddPosition(m_trans->GetDownVector() * sMoveSpeed * DELTA_TIME);
}

void CameraObject::TurnLeft()
{
	m_trans->AddRotateDegree(-sTurnSpeed * DELTA_TIME, 0, 0);
}

void CameraObject::TurnRight()
{
	m_trans->AddRotateDegree(sTurnSpeed * DELTA_TIME, 0, 0);
}

void CameraObject::TurnDown()
{
	m_trans->AddRotateDegree(0, -sTurnSpeed * DELTA_TIME, 0);
}

void CameraObject::TurnUp()
{
	m_trans->AddRotateDegree(0, sTurnSpeed * DELTA_TIME, 0);
}


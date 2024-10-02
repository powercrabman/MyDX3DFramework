#include "pch.h"
#include "CameraObject.h"
#include "CCamera.h"
#include "CTransform.h"

CameraObject::CameraObject()
{
	m_trans = CreateComponent<CTransform>();
	m_camera = CreateComponent<CCamera>(45.f, 0.5f, 100.f, m_trans);
}

void CameraObject::Initialize()
{
	m_trans->SetPosition(Vector3::Zero);
	m_trans->SetScale(Vector3::One);
	m_trans->SetRotate(Vector3::One);
}

void CameraObject::Update(float inDeltaTime)
{
}


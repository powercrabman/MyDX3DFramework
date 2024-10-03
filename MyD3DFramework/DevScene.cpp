#include "pch.h"
#include "DevScene.h"
#include "CameraObject.h"
#include "CCamera.h"

void DevScene::EnterScene()
{
	CameraObject* cmrObj = CreateGameObject<CameraObject>();

	/* ���� ī�޶� ���� */
	CCamera* cmr = cmrObj->GetComponentOrNull<CCamera>();
	if (cmr)
	{
		cmr->RegisterToMainCamera();
	}
}

void DevScene::UpdateScene()
{
}

void DevScene::ExitScene()
{
	ClearRepogitory();
}

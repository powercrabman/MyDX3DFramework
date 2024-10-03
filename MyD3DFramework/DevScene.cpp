#include "pch.h"
#include "DevScene.h"
#include "CameraObject.h"
#include "CCamera.h"
#include "Scene.h"
#include "Pawn.h"

void DevScene::EnterScene()
{
	CameraObject* cmrObj = CreateGameObject<CameraObject>();

	/* ���� ī�޶� ���� */
	CCamera* cmr = cmrObj->GetComponentOrNull<CCamera>();
	if (cmr)
	{
		cmr->RegisterToMainCamera();
	}

	CreateGameObject<Pawn>();
}

void DevScene::UpdateScene()
{
}

void DevScene::ExitScene()
{
	ClearRepogitory();
}

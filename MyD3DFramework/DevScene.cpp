#include "pch.h"
#include "DevScene.h"
#include "CameraObject.h"
#include "CCamera.h"
#include "Scene.h"
#include "Pawn.h"
#include "LightObject.h"
#include "Ground.h"
#include "CTransform.h"

void DevScene::EnterScene()
{
	CameraObject* cmrObj = CreateGameObject<CameraObject>();

	/* 메인 카메라 설정 */
	CCamera* cmr = cmrObj->GetComponentOrNull<CCamera>();
	if (cmr)
	{
		cmr->RegisterToMainCamera();
	}

	CreateGameObject<LightObject>();
	CreateGameObject<Ground>();
	CreateGameObject<Pawn>();
}

void DevScene::UpdateScene()
{
}

void DevScene::ExitScene()
{
	ClearRepogitory();
}

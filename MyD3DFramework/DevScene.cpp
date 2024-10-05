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
	
	{
		Pawn* pawn = CreateGameObject<Pawn>();
		pawn->GetComponentOrNull<CTransform>()->SetPosition(Vector3{ 0.f,0.f,3.f });
		cmrObj->GetComponentOrNull<CTransform>()->SetRotateByLookAtPoint(Vector3{ 0.f,0.f,3.f });
	}

	{
		Pawn* pawn = CreateGameObject<Pawn>();
		pawn->GetComponentOrNull<CTransform>()->SetPosition(Vector3{ 0.f,-2.f,5.f });
		cmrObj->GetComponentOrNull<CTransform>()->SetRotateByLookAtPoint(Vector3{ 0.f,0.f,3.f });
	}
}

void DevScene::UpdateScene()
{
}

void DevScene::ExitScene()
{
	ClearRepogitory();
}

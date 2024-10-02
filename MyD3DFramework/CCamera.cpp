#include "pch.h"
#include "CCamera.h"
#include "Renderer.h"
#include "Renderer.h"

void CCamera::RegisterToMainCamera()
{
	Renderer::GetInst().RegisterCamera(this);
}

CCamera::~CCamera()
{
	Renderer& r = Renderer::GetInst();
	if (r.GetCamera() == this)
	{
		r.ReleaseCamera();
	}
}

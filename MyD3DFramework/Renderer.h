#pragma once
#include <fstream>
#include "CMeshRenderer.h"
#include "CLight.h"
#include "FastCompVector.h"

class CCamera;
class Effect;
class RenderState;

class Renderer
{
public:
	DEFINE_SINGLE(Renderer);

	//초기화 관련
	bool InitD3D11Device();
	bool IsInitalized() const;

	//리소스 생성
	void InitializeDirectXTKResource();
	void InitializeRenderResoucre();

	//렌더링 코어 함수
	void Render();

	//추가 렌더링 함수
	void RenderIndices(uint32 indicesCount);

	//창 크기 조절
	void ResizeWindow(const WindowSize& winSize);
	void SetFullScreen(bool trigger);

	//더블 버퍼링
	void ClearBuffer(XMVECTORF32 inClearColor);
	void Present();

	//리소스 정리
	void CleanUp();

	//화면에 문자열 그리기
	void DrawString(const std::string& inStr, const Vector2& inScreenPos, eFont inFont, const XMVECTORF32& inColor, eFontAlign align = eFontAlign::Left, float scale = 1.f);
	void DrawString(const wchar_t* inStr, const Vector2& inScreenPos, eFont inFont, const XMVECTORF32& inColor, eFontAlign align = eFontAlign::Left, float scale = 1.f);

	//Setter Getter
	NODISCARD ID3D11Device5* GetDeivce() const { ASSERT(m_device, L"디바이스가 Nullptr입니다."); return m_device.Get(); }

	NODISCARD D3D_FEATURE_LEVEL GetFeatureLevel() const { return m_featureLevel; }
	NODISCARD std::string GetFeatureLevelToString() const;

	NODISCARD Effect* GetEffect(const std::string& inKey);
	NODISCARD Effect* GetCurrentEffect() const { assert(m_curEffect); return m_curEffect; }

	NODISCARD RenderState* GetRenderState(const std::string& inKey);
	NODISCARD RenderState* GetCurrentRenderState() const { assert(m_curRenderState); return m_curRenderState; }

	NODISCARD Mesh* GetMesh(const std::string& inKey);
	NODISCARD Material* GetMaterial(const std::string& inKey);

	void SetCurrentEffect(const std::string& inKey);
	void SetCurrentRenderState(const std::string& inKey);

	void BindMesh(Mesh* inMesh) { m_curMesh = inMesh; }
	Mesh* GetBindedMesh() const { return m_curMesh; }

	// 카메라 관련 코드
	void RegisterCamera(CCamera* inCamera);
	void ReleaseCamera() { m_camera = nullptr; }
	CCamera* GetCamera();

	//쉐이더 코드 가져오기
	void LoadAndCopileShaderFromFile(
		std::wstring_view inFilename,
		std::string_view inEntryPoint,
		std::string_view inTarget,
		ID3DBlob** outInppBlob);

	//리소스 관련
	Effect* CreateEffect(const std::string& inKey);
	Mesh* CreateMesh(const std::string& inKey);
	RenderState* CreateRenderState(const std::string& inKey);
	Material* CreateMaterial(const std::string& inKey);

	void RegisterCMeshRenderer(CMeshRenderer* inMesh);
	void UnRegisterCMeshRenderer(CMeshRenderer* inMesh);

	void RegisterCLight(CLight* inLight);
	void UnRegisterCLight(CLight* inLight);

public:
	//참조용 Static 변수들
	const static std::string BasicEffectKey;
	const static std::string CubeMeshKey;
	const static std::string SphereMeshKey;
	const static std::string BasicRenderStateKey;
	const static std::string BasicMaterialKey;
	const static std::string CbPerFrameKey;
	const static std::string CbPerObjectKey;

private:
	Renderer();;
	~Renderer() = default;

private:
	//DirectX Resource
	ComPtr<ID3D11Device5> m_device = nullptr;
	ComPtr<ID3D11DeviceContext4> m_deviceContext = nullptr;
	ComPtr<IDXGISwapChain4> m_swapChain = nullptr;

	ComPtr<ID3D11Texture2D> m_depthStencilBuffer = nullptr;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView = nullptr;
	ComPtr<ID3D11RenderTargetView> m_renderTargetView = nullptr;

	CD3D11_VIEWPORT m_viewport = {};
	D3D_FEATURE_LEVEL m_featureLevel = {};

	//내부 리소스
	std::unordered_map<std::string, std::unique_ptr<Effect>> m_effectRepo;
	std::unordered_map<std::string, std::unique_ptr<Mesh>> m_meshRepo;
	std::unordered_map<std::string, std::unique_ptr<RenderState>> m_renderStateRepo;
	std::unordered_map<std::string, std::unique_ptr<Material>> m_materialRepo;

	//현재 사용중인 Effect 캐싱
	Effect* m_curEffect = nullptr;
	RenderState* m_curRenderState = nullptr;

	//DirectXTK
	std::unique_ptr<SpriteBatch> m_spriteBatch = nullptr;

	//MSAA
	bool m_bUseMSAA = false;
	UINT m_MSAAquality = 0;

	//Constant Buffer Struct
	cbPerObject m_cbPerObject = {};
	cbPerFrame m_cbPerFrame = {};

	//3D 메쉬 집합
	CM::FastCompVector<CMeshRenderer*> m_cMeshRendererRepo;
	Mesh* m_curMesh = nullptr;

	//광원 
	CM::Array<CLight*, DIRECTIONAL_LIGHT_MAXIMUM_COUNT> m_dirLightRepo = {};
	CM::Array<CLight*, POINT_LIGHT_MAXIMUM_COUNT> m_pointLightRepo = {};
	CM::Array<CLight*, SPOT_LIGHT_MAXIMUM_COUNT> m_spotLightRepo = {};

	//카메라
	CCamera* m_camera = nullptr;
};

bool Renderer::IsInitalized() const
{
	return  (m_device != nullptr) &&
		(m_deviceContext != nullptr) &&
		(m_swapChain != nullptr) &&
		(m_renderTargetView != nullptr) &&
		(m_depthStencilView != nullptr);
}

//더블 버퍼링

void Renderer::ClearBuffer(XMVECTORF32 inClearColor)
{
	m_deviceContext->ClearRenderTargetView(
		m_renderTargetView.Get(),
		inClearColor);

	m_deviceContext->ClearDepthStencilView(
		m_depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.f,
		0);
}

void Renderer::Present()
{
	m_swapChain->Present(0, 0);
}


void Renderer::CleanUp()
{
	m_device.Reset();
	m_deviceContext.Reset();
	m_swapChain.Reset();

	m_depthStencilBuffer.Reset();
	m_depthStencilView.Reset();
	m_renderTargetView.Reset();

	m_spriteBatch.reset();
}

void Renderer::SetFullScreen(bool trigger)
{
	ASSERT(m_swapChain, L"스왑체인이 존재하지 않습니다.");
	HRESULT hr = m_swapChain->SetFullscreenState(trigger, nullptr);
	CHECK_FAILED(hr);
}

std::string Renderer::GetFeatureLevelToString() const
{
	switch (m_featureLevel)
	{
	case D3D_FEATURE_LEVEL_1_0_CORE: return "D3D_FEATURE_LEVEL_1_0_CORE";
	case D3D_FEATURE_LEVEL_9_1:		 return "D3D_FEATURE_LEVEL_9_1";
	case D3D_FEATURE_LEVEL_9_2:		 return "D3D_FEATURE_LEVEL_9_2";
	case D3D_FEATURE_LEVEL_9_3:		 return "D3D_FEATURE_LEVEL_9_3";
	case D3D_FEATURE_LEVEL_10_0:	 return "D3D_FEATURE_LEVEL_10_0";
	case D3D_FEATURE_LEVEL_10_1:	 return "D3D_FEATURE_LEVEL_10_1";
	case D3D_FEATURE_LEVEL_11_0:	 return "D3D_FEATURE_LEVEL_11_0";
	case D3D_FEATURE_LEVEL_11_1:	 return "D3D_FEATURE_LEVEL_11_1";
	case D3D_FEATURE_LEVEL_12_0:	 return "D3D_FEATURE_LEVEL_12_0";
	case D3D_FEATURE_LEVEL_12_1:	 return "D3D_FEATURE_LEVEL_12_1";
	case D3D_FEATURE_LEVEL_12_2:	 return "D3D_FEATURE_LEVEL_12_2";
	default: ASSERT(false, L"알 수 없는 FeatureLevel.");
	}
}

Effect* Renderer::GetEffect(const std::string& inKey)
{
	assert(m_effectRepo.contains(inKey));
	return m_effectRepo[inKey].get();
}

RenderState* Renderer::GetRenderState(const std::string& inKey)
{
	assert(m_renderStateRepo.contains(inKey));
	return m_renderStateRepo[inKey].get();
}

Effect* Renderer::CreateEffect(const std::string& inKey)
{
	assert(!m_effectRepo.contains(inKey));
	std::unique_ptr<Effect> inst = std::make_unique<Effect>();
	Effect* returnObj = inst.get();
	m_effectRepo[inKey] = std::move(inst);

	return returnObj;
}

Mesh* Renderer::CreateMesh(const std::string& inKey)
{
	assert(!m_meshRepo.contains(inKey));
	std::unique_ptr<Mesh> inst = std::make_unique<Mesh>();
	Mesh* returnObj = inst.get();
	m_meshRepo[inKey] = std::move(inst);

	return returnObj;
}

Material* Renderer::CreateMaterial(const std::string& inKey)
{
	assert(!m_materialRepo.contains(inKey));
	std::unique_ptr<Material> inst = std::make_unique<Material>();
	Material* returnObj = inst.get();
	m_materialRepo[inKey] = std::move(inst);

	return returnObj;
}

void Renderer::RegisterCMeshRenderer(CMeshRenderer* inMesh)
{
	m_cMeshRendererRepo.Insert(inMesh);
}

void Renderer::UnRegisterCMeshRenderer(CMeshRenderer* inMesh)
{
	m_cMeshRendererRepo.Remove(inMesh);
}

inline void Renderer::RegisterCLight(CLight* inLight)
{
	switch (inLight->GetLightType())
	{
	case eLightType::Directional_Light: m_dirLightRepo.PushBack(inLight); break;
	case eLightType::Point_Light: m_pointLightRepo.PushBack(inLight); break;
	case eLightType::Spot_Light: m_spotLightRepo.PushBack(inLight); break;
	default: assert(false);
	}
}

inline void Renderer::UnRegisterCLight(CLight* inLight)
{
	switch (inLight->GetLightType())
	{
	case eLightType::Directional_Light: m_dirLightRepo.Erase(inLight); break;
	case eLightType::Point_Light: m_pointLightRepo.Erase(inLight); break;
	case eLightType::Spot_Light: m_spotLightRepo.Erase(inLight); break;
	default: assert(false);
	}
}

Renderer::Renderer()
{
	m_cMeshRendererRepo.Reserve(1024);
}

Mesh* Renderer::GetMesh(const std::string& inKey)
{
	assert(m_meshRepo.contains(inKey));
	return m_meshRepo[inKey].get();
}

Material* Renderer::GetMaterial(const std::string& inKey)
{
	assert(m_materialRepo.contains(inKey));
	return m_materialRepo[inKey].get();
}

void Renderer::SetCurrentEffect(const std::string& inKey)
{
	m_curEffect = GetEffect(inKey);
}

void Renderer::SetCurrentRenderState(const std::string& inKey)
{
	m_curRenderState = GetRenderState(inKey);
}

inline void Renderer::RenderIndices(uint32 indicesCount)
{
	m_deviceContext->DrawIndexed(indicesCount, 0, 0);
}

// 카메라 관련 코드
inline void Renderer::RegisterCamera(CCamera* inCamera)
{
	assert(inCamera);
	m_camera = inCamera;
}

CCamera* Renderer::GetCamera()
{
	assert(m_camera);
	return m_camera;
}

//쉐이더 코드 가져오기

void Renderer::LoadAndCopileShaderFromFile(std::wstring_view inFilename, std::string_view inEntryPoint, std::string_view inTarget, ID3DBlob** outInppBlob)
{
	std::wifstream file{ inFilename.data() };
	VERTIFY(file.is_open(), L"쉐이더 파일 로드 실패");

	std::wstringstream buffer = {};
	buffer << file.rdbuf();

	std::wstring shaderCode = buffer.str();

	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif
	ShaderIncludeHelper includeHelper = {};
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3DCompile(
		shaderCode.c_str(),
		shaderCode.size(),
		nullptr,
		nullptr,
		&includeHelper,
		inEntryPoint.data(),
		inTarget.data(),
		compileFlags,
		0,
		outInppBlob,
		errorBlob.GetAddressOf());

	CHECK_FAILED_MESSAGE(hr, CM::StringToWstring((char*)errorBlob->GetBufferPointer()).c_str());
}

//쉐이더 코드 가져오기


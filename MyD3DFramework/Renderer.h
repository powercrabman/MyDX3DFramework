#pragma once

class CMeshRenderer;
class CLight;

class Renderer
{
public:
	DEFINE_SINGLE(Renderer);

	//초기화 관련
	bool InitD3D11Device();
	inline bool IsInitalized() const;

	//리소스 생성
	void CreateDirectXTKResource();
	void CreateRenderResoucre();

	//렌더링 코어 함수
	void Update(float inDeltaTime);
	void Render();

	//추가 업데이트 함수
	void UpdateCameraAction(float inDeltaTime);

	//추가 렌더링 함수
	inline void RenderIndices(uint32 indicesCount);

	//창 크기 조절
	void ResizeWindow(const WindowSize& winSize);
	inline void SetFullScreen(bool trigger);

	//더블 버퍼링
	inline void ClearBuffer(XMVECTORF32 inClearColor);
	inline void Present();

	//리소스 정리
	inline void CleanUp();

	//화면에 문자열 그리기
	void DrawString(const std::wstring& inStr, const Vector2& inScreenPos, eFont inFont, const XMVECTORF32& inColor, eFontAlign align = eFontAlign::Left, float scale = 1.f);
	void DrawString(const wchar_t* inStr, const Vector2& inScreenPos, eFont inFont, const XMVECTORF32& inColor, eFontAlign align = eFontAlign::Left, float scale = 1.f);

	//Setter Getter
	NODISCARD inline ID3D11Device5* GetDeivce() const { ASSERT(m_device, L"디바이스가 Nullptr입니다."); return m_device.Get(); }

	NODISCARD inline D3D_FEATURE_LEVEL GetFeatureLevel() const { return m_featureLevel; }
	NODISCARD inline std::wstring GetFeatureLevelToString() const;

	NODISCARD inline Effect* GetEffect(const std::wstring& inKey);
	NODISCARD inline Effect* GetCurrentEffect() const { assert(m_curEffect); return m_curEffect; }

	NODISCARD inline RenderState* GetRenderState(const std::wstring& inKey);
	NODISCARD inline RenderState* GetCurrentRenderState() const { assert(m_curRenderState); return m_curRenderState; }

	NODISCARD inline Mesh* GetMesh(const std::wstring& inKey);
	NODISCARD inline Material* GetMaterial(const std::wstring& inKey);

	inline void SetCurrentEffect(const std::wstring& inKey);
	inline void SetCurrentRenderState(const std::wstring& inKey);

	//쉐이더 코드 가져오기
	void LoadAndCopileShaderFromFile(
		const std::wstring& inFilename,
		const std::string& inEntryPoint,
		const std::string& inTarget,
		ID3DBlob** outInppBlob);

	//리소스 관련
	inline Effect* RegisterEffect(const std::wstring& inKey);
	inline Mesh* RegisterMesh(const std::wstring& inKey);
	inline RenderState* RegisterRenderState(const std::wstring& inKey);
	inline Material* RegisterMaterial(const std::wstring& inKey);

	inline void RegisterCMeshRenderer(CMeshRenderer* inMesh);
	inline void CleanGarbageInCMeshRendererList();

	template<typename ObjectType, typename ...Args>
	inline ObjectType* RegisterGameObject(Args... args)
	{
		static_assert(std::is_base_of<GameObject, ObjectType>::value);

		std::unique_ptr<> inst = std::make_unique<ObjectType>(args...);
		inst->InitalizeCore();
		inst->SetTypeInfo(CM::TypeTrait<ObjectType>::GetInfo());

		ObjectType* returnObj = inst.get();
		m_rObjRepo.push_back(std::move(inst));

		return returnObj;
	}

public:
	//참조용 Static 변수들
	const static std::wstring BasicEffectKey;
	const static std::wstring CubeMeshKey;
	const static std::wstring SphereMeshKey;
	const static std::wstring BasicRenderStateKey;
	const static std::wstring BasicMaterialKey;
	const static std::wstring CbPerFrameKey;
	const static std::wstring CbPerObjectKey;

private:
	Renderer() = default;
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
	std::unordered_map<std::wstring, std::unique_ptr<Effect>> m_effectRepo;
	std::unordered_map<std::wstring, std::unique_ptr<Mesh>> m_meshRepo;
	std::unordered_map<std::wstring, std::unique_ptr<RenderState>> m_renderStateRepo;
	std::unordered_map<std::wstring, std::unique_ptr<Material>> m_materialRepo;

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
	std::vector<CMeshRenderer*> m_cMashRendererRepo{1024};

	//빛
	//std::array<CLight*, USABLE_LIGHT_MAXIMUM_COUNT> m_cLightRepo;

	//카메라(임시)
	Vector3 m_cmrLook = Vector3{ 0.f,0.f,1.f };
	Vector3 m_cmrPosition = Vector3{ 0.f,3.f,0.f };
	Vector3 m_worldUpAxis = Vector3{ 0.f,1.f,0.f };
	float m_nearPlane = 0.5f;
	float m_farPlane = 100.f;

	Matrix m_viewMat = Matrix::Identity;
	Matrix m_projMat = Matrix::Identity;
	float m_fov = ::XMConvertToRadians(45.f);

	//광원(임시)
	//DirectionalLight m_dirLight = {};
	//PointLight m_pointLight = {};
	//SpotLight m_spotLight = {};
};

inline bool Renderer::IsInitalized() const
{
	return  (m_device != nullptr) &&
		(m_deviceContext != nullptr) &&
		(m_swapChain != nullptr) &&
		(m_renderTargetView != nullptr) &&
		(m_depthStencilView != nullptr);
}

//더블 버퍼링

inline void Renderer::ClearBuffer(XMVECTORF32 inClearColor)
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

inline void Renderer::Present()
{
	m_swapChain->Present(0, 0);
}


inline void Renderer::CleanUp()
{
	m_device.Reset();
	m_deviceContext.Reset();
	m_swapChain.Reset();

	m_depthStencilBuffer.Reset();
	m_depthStencilView.Reset();
	m_renderTargetView.Reset();

	m_spriteBatch.reset();
}

inline void Renderer::SetFullScreen(bool trigger)
{
	ASSERT(m_swapChain, L"스왑체인이 존재하지 않습니다.");
	HRESULT hr = m_swapChain->SetFullscreenState(trigger, nullptr);
	CHECK_FAILED(hr);
}

inline std::wstring Renderer::GetFeatureLevelToString() const
{
	switch (m_featureLevel)
	{
	case D3D_FEATURE_LEVEL_1_0_CORE: return L"D3D_FEATURE_LEVEL_1_0_CORE";
	case D3D_FEATURE_LEVEL_9_1:		 return L"D3D_FEATURE_LEVEL_9_1";
	case D3D_FEATURE_LEVEL_9_2:		 return L"D3D_FEATURE_LEVEL_9_2";
	case D3D_FEATURE_LEVEL_9_3:		 return L"D3D_FEATURE_LEVEL_9_3";
	case D3D_FEATURE_LEVEL_10_0:	 return L"D3D_FEATURE_LEVEL_10_0";
	case D3D_FEATURE_LEVEL_10_1:	 return L"D3D_FEATURE_LEVEL_10_1";
	case D3D_FEATURE_LEVEL_11_0:	 return L"D3D_FEATURE_LEVEL_11_0";
	case D3D_FEATURE_LEVEL_11_1:	 return L"D3D_FEATURE_LEVEL_11_1";
	case D3D_FEATURE_LEVEL_12_0:	 return L"D3D_FEATURE_LEVEL_12_0";
	case D3D_FEATURE_LEVEL_12_1:	 return L"D3D_FEATURE_LEVEL_12_1";
	case D3D_FEATURE_LEVEL_12_2:	 return L"D3D_FEATURE_LEVEL_12_2";
	default: ASSERT(false, L"알 수 없는 FeatureLevel.");
	}
}

inline Effect* Renderer::GetEffect(const std::wstring& inKey)
{
	assert(m_effectRepo.contains(inKey));
	return m_effectRepo[inKey].get();
}

inline RenderState* Renderer::GetRenderState(const std::wstring& inKey)
{
	assert(m_renderStateRepo.contains(inKey));
	return m_renderStateRepo[inKey].get();
}

inline Effect* Renderer::RegisterEffect(const std::wstring& inKey)
{
	assert(!m_effectRepo.contains(inKey));
	std::unique_ptr<Effect> inst = std::make_unique<Effect>();
	Effect* returnObj = inst.get();
	m_effectRepo[inKey] = std::move(inst);

	return returnObj;
}

inline Mesh* Renderer::RegisterMesh(const std::wstring& inKey)
{
	assert(!m_meshRepo.contains(inKey));
	std::unique_ptr<Mesh> inst = std::make_unique<Mesh>();
	Mesh* returnObj = inst.get();
	m_meshRepo[inKey] = std::move(inst);

	return returnObj;
}

inline Material* Renderer::RegisterMaterial(const std::wstring& inKey)
{
	assert(!m_materialRepo.contains(inKey));
	std::unique_ptr<Material> inst = std::make_unique<Material>();
	Material* returnObj = inst.get();
	m_materialRepo[inKey] = std::move(inst);

	return returnObj;
}

inline void Renderer::RegisterCMeshRenderer(CMeshRenderer* inMesh)
{
	if (inMesh != nullptr)
	{
		m_cMashRendererRepo.push_back(inMesh);
	}
}

inline void Renderer::CleanGarbageInCMeshRendererList()
{
	std::vector<CMeshRenderer*> newVector;
	size_t repoLen = m_cMashRendererRepo.size();
	newVector.reserve(repoLen);

	for (size_t i = 0; i < repoLen; ++i)
	{
		if (m_cMashRendererRepo[i] != nullptr)
		{
			newVector.push_back(m_cMashRendererRepo[i]);
		}
	}

	m_cMashRendererRepo = std::move(newVector);
}

inline Mesh* Renderer::GetMesh(const std::wstring& inKey)
{
	assert(m_meshRepo.contains(inKey));
	return m_meshRepo[inKey].get();
}

inline Material* Renderer::GetMaterial(const std::wstring& inKey)
{
	assert(m_materialRepo.contains(inKey));
	return m_materialRepo[inKey].get();
}

inline void Renderer::SetCurrentEffect(const std::wstring& inKey)
{
	m_curEffect = GetEffect(inKey);
}

inline void Renderer::SetCurrentRenderState(const std::wstring& inKey)
{
	m_curRenderState = GetRenderState(inKey);
}

inline void Renderer::RenderIndices(uint32 indicesCount)
{
	m_deviceContext->DrawIndexed(indicesCount, 0, 0);
}


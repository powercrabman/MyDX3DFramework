#pragma once

class Renderer
{
public:
	DEFINE_SINGLE(Renderer);

	//�ʱ�ȭ ����
	bool InitD3D11Device();
	inline bool IsInitalized() const;
	void CreateDeviceDependentResource();

	//������ �ھ� �Լ�
	void InputAssembler();
	void Render();

	//â ũ�� ����
	void ResizeWindow(const WindowSize& winSize);
	inline void SetFullScreen(bool trigger);

	//���� ���۸�
	inline void ClearBuffer(XMVECTORF32 inClearColor);
	inline void Present();

	//���ҽ� ����
	inline void CleanUp();

	//ȭ�鿡 ���ڿ� �׸���
	void DrawString(const std::wstring& inStr, const Vector2& inScreenPos, eFont inFont, const XMVECTORF32& inColor, eFontAlign align = eFontAlign::Left, float scale = 1.f);
	void DrawString(const wchar_t* inStr, const Vector2& inScreenPos, eFont inFont, const XMVECTORF32& inColor, eFontAlign align = eFontAlign::Left, float scale = 1.f);

	//���̴� �ڵ� ��������
	void LoadAndCopileShaderFromFile(
		const std::wstring& inFilename,
		const std::string& inEntryPoint, 
		const std::string& inTarget, 
		ID3DBlob** outInppBlob);

	//Getter
	inline ID3D11Device5* GetDeivce() const { ASSERT(m_device, L"����̽��� Nullptr�Դϴ�."); return m_device.Get(); }

	inline D3D_FEATURE_LEVEL GetFeatureLevel() const { return m_featureLevel; }
	inline std::wstring GetFeatureLevelToString() const;

private:
	Renderer() = default;
	~Renderer() = default;

private:
	ComPtr<ID3D11Device5> m_device = nullptr;
	ComPtr<ID3D11DeviceContext4> m_deviceContext = nullptr;
	ComPtr<IDXGISwapChain4> m_swapChain = nullptr;

	ComPtr<ID3D11Texture2D> m_depthStencilBuffer = nullptr;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView = nullptr;
	ComPtr<ID3D11RenderTargetView> m_renderTargetView = nullptr;

	ComPtr<ID3D11VertexShader> m_vertexShader = nullptr;
	ComPtr<ID3D11PixelShader> m_pixelShader = nullptr;
	ComPtr<ID3D11InputLayout> m_inputLayout = nullptr;
	ComPtr<ID3D11Buffer> m_constantBuffer = nullptr;

	CD3D11_VIEWPORT m_viewport = {};
	D3D_FEATURE_LEVEL m_featureLevel = {};

	//DirectXTK
	std::unique_ptr<SpriteBatch> m_spriteBatch = nullptr;

	//MSAA
	bool m_bUseMSAA = false;
	UINT m_MSAAquality = 0;
};

inline bool Renderer::IsInitalized() const
{
	return  (m_device != nullptr) &&
		(m_deviceContext != nullptr) &&
		(m_swapChain != nullptr) &&
		(m_renderTargetView != nullptr) &&
		(m_depthStencilView != nullptr);
}

//���� ���۸�

inline void Renderer::ClearBuffer(XMVECTORF32 inClearColor)
{
	m_deviceContext->ClearRenderTargetView(
		m_renderTargetView.Get(),
		inClearColor);

	m_deviceContext->ClearDepthStencilView(
		m_depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0,
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
	ASSERT(m_swapChain, L"����ü���� �������� �ʽ��ϴ�.");
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
	default: ASSERT(false, L"�� �� ���� FeatureLevel.");
	}
}


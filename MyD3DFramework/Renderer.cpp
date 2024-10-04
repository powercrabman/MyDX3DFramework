#include "pch.h"
#include "Renderer.h"
#include "WindowsApp.h"
#include "CCamera.h"
#include "Engine.h"
#include "CTransform.h"
#include "CMeshRenderer.h"
#include <fstream>


bool Renderer::InitD3D11Device()
{
	if (!::XMVerifyCPUSupport())
	{
		WinUtile::Alert(L"�ش� �÷������� DirectXMath�� �������� �ʽ��ϴ�.", L"Error");
		return false;
	}

	//����̽� �� ����̽� ���ؽ�Ʈ �ʱ�ȭ
	ComPtr<ID3D11Device> baseDevice = nullptr;
	ComPtr<ID3D11DeviceContext> baseDC = nullptr;
	HRESULT hr = NULL;

	hr = ::D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_SINGLETHREADED,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		baseDevice.GetAddressOf(),
		&m_featureLevel,
		baseDC.GetAddressOf()
	);
	CHECK_FAILED(hr);

	if (m_featureLevel < D3D_FEATURE_LEVEL_11_0)
	{
		WinUtile::Alert(L"DirectX11 �� �������� �ʴ� ��Ű��ó", L"Error");
		return false;
	}

	hr = baseDevice.As(&m_device);
	CHECK_FAILED(hr);

	hr = baseDC.As(&m_deviceContext);
	CHECK_FAILED(hr);

	//4xMSAA Ȯ��
	if (MSAA_OPTION)
	{
		uint32 qualityLevel = 0;
		hr = m_device->CheckMultisampleQualityLevels(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			4,
			&qualityLevel
		);
		ASSERT(qualityLevel > 0, "QualityleveL Error.");
		CHECK_FAILED(hr);
		m_bUseMSAA = true;
		m_MSAAquality = qualityLevel - 1;
	}

	//����ü�� ����
	WindowSize winSize = WindowSize::s_defaultWindowSize;

	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.BufferCount = 1;
	scDesc.Width = static_cast<UINT>(winSize.Width);
	scDesc.Height = static_cast<UINT>(winSize.Height);
	scDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.Scaling = DXGI_SCALING_STRETCH;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.SampleDesc.Count = m_bUseMSAA ? 4 : 1;
	scDesc.SampleDesc.Quality = m_MSAAquality;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	ComPtr<IDXGIDevice> dxgiDevice = nullptr;
	hr = m_device->QueryInterface(dxgiDevice.GetAddressOf());
	CHECK_FAILED(hr);

	ComPtr<IDXGIAdapter3> dxgiAdapter = nullptr;
	hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter3), reinterpret_cast<void**>(dxgiAdapter.GetAddressOf()));
	CHECK_FAILED(hr);

	ComPtr<IDXGIFactory5> factory = nullptr;
	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory5), reinterpret_cast<void**>(factory.GetAddressOf()));
	CHECK_FAILED(hr);

	ComPtr<IDXGISwapChain1> swapChain = nullptr;
	hr = factory->CreateSwapChainForHwnd(
		m_device.Get(),
		WindowsApp::GetInst().GetHwnd(),
		&scDesc,
		nullptr,
		nullptr,
		swapChain.GetAddressOf()
	);
	CHECK_FAILED(hr);

	hr = swapChain.As(&m_swapChain);
	CHECK_FAILED(hr);

	//���� Ÿ�� ����
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = m_swapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(backBuffer.GetAddressOf())
	);
	CHECK_FAILED(hr);

	hr = m_device->CreateRenderTargetView(
		backBuffer.Get(),
		nullptr,
		m_renderTargetView.GetAddressOf()
	);
	CHECK_FAILED(hr);

	//���� ���ٽ� ���� ���� �� �� ����
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = static_cast<UINT>(winSize.Width);
	textureDesc.Height = static_cast<UINT>(winSize.Height);
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = m_bUseMSAA ? 4 : 1;
	textureDesc.SampleDesc.Quality = m_MSAAquality;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.MiscFlags = 0;
	textureDesc.CPUAccessFlags = 0;

	hr = m_device->CreateTexture2D(
		&textureDesc,
		nullptr,
		m_depthStencilBuffer.GetAddressOf()
	);
	CHECK_FAILED(hr);

	hr = m_device->CreateDepthStencilView(
		m_depthStencilBuffer.Get(),
		nullptr,
		m_depthStencilView.GetAddressOf()
	);
	CHECK_FAILED(hr);

	m_deviceContext->OMSetRenderTargets(
		1,
		m_renderTargetView.GetAddressOf(),
		m_depthStencilView.Get()
	);

	//����Ʈ ����
	m_viewport.Width = static_cast<float>(winSize.Width);
	m_viewport.Height = static_cast<float>(winSize.Height);
	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.MinDepth = 0;
	m_viewport.MaxDepth = 1;

	m_deviceContext->RSSetViewports(1, &m_viewport);

	return true;
}

void Renderer::InitializeDirectXTKResource()
{
	m_spriteBatch = std::make_unique<SpriteBatch>(m_deviceContext.Get());
}

void Renderer::InitializeRenderResoucre()
{
	//����Ʈ ����
	Effect* effect = CreateEffect(sBasicEffectKey);

	effect->SetProperties(
		m_device.Get(),
		L"LightVertexShader.hlsl",
		"VS",
		"vs_5_0",
		L"LightPixelShader.hlsl",
		"PS",
		"ps_5_0",
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		}
	);

	CreateCubeMesh();
	CreateSphereMesh(20, 20, 0.5f);

	//RnederState ����
	RenderState* renderState = CreateRenderState(sBasicRenderStateKey);
	renderState->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderState->SetRasterizerState(m_device.Get());

	//������Ʈ ���� ����

	//cbPerObject
	effect->RegisterConstantBuffer(
		m_device.Get(),
		sizeof(cbPerObject),
		sCbPerObjectKey,
		0,
		CONSTANT_BUFFER_APPLY_VERTEX_SHADER | CONSTANT_BUFFER_APPLY_PIXEL_SHADER
	);

	//cbPerFrame
	effect->RegisterConstantBuffer(
		m_device.Get(),
		sizeof(cbPerFrame),
		sCbPerFrameKey,
		1,
		CONSTANT_BUFFER_APPLY_VERTEX_SHADER | CONSTANT_BUFFER_APPLY_PIXEL_SHADER
	);

	//���׸��� ����
	{
		Material* m = CreateMaterial(sBasicMaterialKey);
		m->Ambient = Color(0.2f, 0.2f, 0.2f);
		m->Diffuse = Color(0.7f, 0.7f, 0.7f);
		m->Specular = Color(1.0f, 1.0f, 1.0f);
		m->Specular.w = 32;
	}

	//���׸��� ����
	{
		Material* m = CreateMaterial(sGroundMaterialKey);
		m->Ambient = Color(0.6f, 0.6f, 0.6f);
		m->Diffuse = Color(0.7f, 0.7f, 1.0f);
		m->Specular = Color(1.0f, 1.0f, 1.0f);
		m->Specular.w = 32;
	}

	SetCurrentEffect(sBasicEffectKey);
	SetCurrentRenderState(sBasicRenderStateKey);

}

void Renderer::CreateCubeMesh()
{
	//�޽� ����
	Mesh* mesh = CreateMesh(sCubeMeshKey);

	//���ؽ� ���� ����
	std::vector<VertexNormal> vertices = {
		{Vector3{-1.f,-1.f,-1.f}, Vector3{-1.f,-1.f,-1.f}},
		{Vector3{-1.f,+1.f,-1.f}, Vector3{-1.f,+1.f,-1.f}},
		{Vector3{+1.f,+1.f,-1.f}, Vector3{+1.f,+1.f,-1.f}},
		{Vector3{+1.f,-1.f,-1.f}, Vector3{+1.f,-1.f,-1.f}},
		{Vector3{-1.f,-1.f,+1.f}, Vector3{-1.f,-1.f,+1.f}},
		{Vector3{-1.f,+1.f,+1.f}, Vector3{-1.f,+1.f,+1.f}},
		{Vector3{+1.f,+1.f,+1.f}, Vector3{+1.f,+1.f,+1.f}},
		{Vector3{+1.f,-1.f,+1.f}, Vector3{+1.f,-1.f,+1.f}}
	};

	//��� ���� ����ȭ
	for (auto& v : vertices)
	{
		v.Normal.Normalize();
	}

	mesh->SetVertexBuffer(m_device.Get(), vertices);

	//�ε��� ���� ����
	std::vector<UINT> indices = {
		0, 1, 2, 0, 2, 3,
		4, 6, 5, 4, 7, 6,
		1, 5, 6, 1, 6, 2,
		0, 3, 7, 0, 7, 4,
		0, 4, 5, 0, 5, 1,
		3, 2, 6, 3, 6, 7
	};

	mesh->SetIndexBuffer(m_device.Get(), indices);
}

void Renderer::CreateSphereMesh(int latitudeSegments, int longitudeSegments, float radius)
{
	Mesh* mesh = CreateMesh(sSphereMeshKey);

	std::vector<VertexNormal> vertices;
	std::vector<UINT> indices;

	// ���ؽ� ����
	for (int lat = 0; lat <= latitudeSegments; ++lat) {
		float theta = lat * DirectX::XM_PI / latitudeSegments;
		float sinTheta = sinf(theta);
		float cosTheta = cosf(theta);

		for (int lon = 0; lon <= longitudeSegments; ++lon) {
			float phi = lon * 2 * DirectX::XM_PI / longitudeSegments;
			float sinPhi = sinf(phi);
			float cosPhi = cosf(phi);

			Vector3 position = {
				radius * cosPhi * sinTheta,
				radius * cosTheta,
				radius * sinPhi * sinTheta
			};

			Vector3 normal = position;
			normal.Normalize();

			vertices.push_back({ position, normal });
		}
	}

	for (int lat = 0; lat < latitudeSegments; ++lat) {
		for (int lon = 0; lon < longitudeSegments; ++lon) {
			int first = (lat * (longitudeSegments + 1)) + lon;
			int second = first + longitudeSegments + 1;

			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(first + 1);

			indices.push_back(second);
			indices.push_back(second + 1);
			indices.push_back(first + 1);
		}
	}

	mesh->SetVertexBuffer(m_device.Get(), vertices);

	mesh->SetIndexBuffer(m_device.Get(), indices);
}
void Renderer::Render()
{
	/* ������ �ɼ� ���� */
	m_curEffect->Apply(m_deviceContext.Get());
	m_curEffect->BindConstantBuffer(m_deviceContext.Get(), sCbPerFrameKey);
	m_curEffect->BindConstantBuffer(m_deviceContext.Get(), sCbPerObjectKey);
	m_curRenderState->Apply(m_deviceContext.Get());
	m_curMesh = nullptr;

	/* ī�޶� ���� */
	{
		cbPerFrame cb = {};

		/* ���⼺ ���� */
		for (int i = 0; i < m_dirLightRepo.Size(); ++i)
		{
			cbStructDirLight desc = {};
			desc.Ambient = m_dirLightRepo[i]->GetAmbient();
			desc.Diffuse = m_dirLightRepo[i]->GetDiffuse();
			desc.Specular = m_dirLightRepo[i]->GetSpecular();
			desc.Direction = m_dirLightRepo[i]->GetDirection();

			cb.DirLight[i] = desc;
		}

		/* �� ���� */
		for (int i = 0; i < m_pointLightRepo.Size(); ++i)
		{
			cbStructPointLight desc = {};
			desc.Ambient = m_pointLightRepo[i]->GetAmbient();
			desc.Diffuse = m_pointLightRepo[i]->GetDiffuse();
			desc.Specular = m_pointLightRepo[i]->GetSpecular();
			desc.Position = m_pointLightRepo[i]->GetPosition();
			desc.Attenuation = m_pointLightRepo[i]->GetAttenuation();
			desc.Range = m_pointLightRepo[i]->GetRange();

			cb.PointLight[i] = desc;
		}

		/* ��������Ʈ ���� */
		for (int i = 0; i < m_spotLightRepo.Size(); ++i)
		{
			cbStructSpotLight desc = {};
			desc.Ambient = m_spotLightRepo[i]->GetAmbient();
			desc.Diffuse = m_spotLightRepo[i]->GetDiffuse();
			desc.Specular = m_spotLightRepo[i]->GetSpecular();
			desc.Position = m_spotLightRepo[i]->GetPosition();
			desc.Attenuation = m_spotLightRepo[i]->GetAttenuation();
			desc.Range = m_spotLightRepo[i]->GetRange();
			desc.Exponent = m_spotLightRepo[i]->GetExpoent();
			desc.SpotDirection = m_spotLightRepo[i]->GetDirection();

			cb.SpotLight[i] = desc;
		}

		/* ���� ������ ���� */
		cb.DirLightCount = m_dirLightRepo.Size();
		cb.PointLightCount = m_pointLightRepo.Size();
		cb.SpotLightCount = m_spotLightRepo.Size();

		/* ī�޶� ���� */
		cb.EyePosW = ToVector4(m_camera->GetPosition());
		cb.ViewProj = (m_camera->GetViewMatrix() * m_camera->GetPerspectiveMatrix(WindowsApp::GetInst().GetAspectRatio())).Transpose();

		m_curEffect->UpdateConstantBuffer(m_deviceContext.Get(), sCbPerFrameKey, cb);
	}

	/* ������Ʈ �������� */
	const auto& repo = m_cMeshRendererRepo.GetVector();
	for (int i = 0; i < repo.size(); ++i)
	{
		const CMeshRenderer* meshCmp = repo[i];
		if (meshCmp->IsEnable())
		{
			/* ������� ������Ʈ */
			cbPerObject cb = {};

			cb.Material = *meshCmp->GetMaterial();

			cb.World = meshCmp->GetTransform()->GetWorldMatrix().Transpose();
			cb.WorldInvTranspose = meshCmp->GetTransform()->GetWorldMatrixInverse();

			m_curEffect->UpdateConstantBuffer(m_deviceContext.Get(), sCbPerObjectKey, cb);

			/* ���ؽ� ���ۿ� �ε��� ���۸� ���ε��ϰ� ���*/
			Mesh* m = meshCmp->GetMesh();
			BindBuffers(m);
			RenderIndices(m);
		}
	}
}

//========================================
// 
//	1. Light ���� 
//  2. ������� ����
//	3. ������ �׽�Ʈ �ϸ� ��! 
// 
//========================================

void Renderer::ResizeWindow(const WindowSize& winSize)
{
	//����ü��, ����Ÿ��, ����-���ٽ� ����, ����Ʈ ����
	HRESULT hr = NULL;
	m_depthStencilBuffer.Reset();
	m_renderTargetView.Reset();
	m_depthStencilView.Reset();

	hr = m_swapChain->ResizeBuffers(
		1,
		static_cast<UINT>(winSize.Width),
		static_cast<UINT>(winSize.Height),
		DXGI_FORMAT_R8G8B8A8_UNORM,
		0
	);
	CHECK_FAILED(hr);

	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	CHECK_FAILED(hr);

	hr = m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf());
	CHECK_FAILED(hr);

	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Height = static_cast<UINT>(winSize.Height);
	depthStencilDesc.Width = static_cast<UINT>(winSize.Width);
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.MiscFlags = 0;
	depthStencilDesc.SampleDesc.Count = m_bUseMSAA ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = m_MSAAquality;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;

	hr = m_device->CreateTexture2D(&depthStencilDesc, nullptr, m_depthStencilBuffer.GetAddressOf());
	CHECK_FAILED(hr);

	hr = m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), nullptr, m_depthStencilView.GetAddressOf());
	CHECK_FAILED(hr);

	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Height = static_cast<float>(winSize.Height);
	m_viewport.Width = static_cast<float>(winSize.Width);
	m_viewport.MinDepth = 0;
	m_viewport.MaxDepth = 1;

	m_deviceContext->RSSetViewports(1, &m_viewport);
}

void Renderer::DrawString(const std::string& inStr, const Vector2& inScreenPos, eFont inFont, const XMVECTORF32& inColor, eFontAlign align, float scale)
{
	m_spriteBatch->Begin();

	//origin ���
	XMVECTOR origin = XMVectorZero();

	SpriteFont* font = ResourceManager::GetInst().GetFont(inFont);

	if (align == eFontAlign::Mid) origin = font->MeasureString(inStr.c_str()) * 0.5f;
	else if (align == eFontAlign::Right) origin = font->MeasureString(inStr.c_str());

	font->DrawString(m_spriteBatch.get(), inStr.c_str(), inScreenPos, inColor, 0.f, origin, scale);

	m_spriteBatch->End();
}

void Renderer::DrawString(const wchar_t* inStr, const Vector2& inScreenPos, eFont inFont, const XMVECTORF32& inColor, eFontAlign align, float scale)
{
	m_spriteBatch->Begin();

	//origin ���
	XMVECTOR origin = XMVectorZero();

	SpriteFont* font = ResourceManager::GetInst().GetFont(inFont);

	if (align == eFontAlign::Mid) origin = font->MeasureString(inStr) * 0.5f;
	else if (align == eFontAlign::Right) origin = font->MeasureString(inStr);

	font->DrawString(m_spriteBatch.get(), inStr, inScreenPos, inColor, 0.f, origin, scale);

	m_spriteBatch->End();
}


inline RenderState* Renderer::CreateRenderState(const std::string& inKey)
{
	assert(!m_renderStateRepo.contains(inKey));
	std::unique_ptr<RenderState> inst = std::make_unique<RenderState>();
	RenderState* returnObj = inst.get();
	m_renderStateRepo[inKey] = std::move(inst);

	return returnObj;
}


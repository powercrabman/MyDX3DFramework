#include "pch.h"
#include "Renderer.h"
#include "WindowsApp.h"
#include "Engine.h"
#include "CTransform.h"
#include "CMeshRenderer.h"
#include <fstream>

const std::string Renderer::BasicEffectKey = "BasicEffect";
const std::string Renderer::CubeMeshKey = "CubeMesh";
const std::string Renderer::SphereMeshKey = "SphereMesh";
const std::string Renderer::BasicRenderStateKey = "BasicRenderState";
const std::string Renderer::BasicMaterialKey = "BasicMateral";
const std::string Renderer::CbPerFrameKey = "cbPerFrame";
const std::string Renderer::CbPerObjectKey = "cbPerObject";

bool Renderer::InitD3D11Device()
{
	if (!::XMVerifyCPUSupport())
	{
		WinUtile::Alert(L"해당 플랫폼에서 DirectXMath를 지원하지 않습니다.", L"Error");
		return false;
	}

	//디바이스 및 디바이스 컨텍스트 초기화
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
		WinUtile::Alert(L"DirectX11 을 지원하지 않는 아키텍처", L"Error");
		return false;
	}

	hr = baseDevice.As(&m_device);
	CHECK_FAILED(hr);

	hr = baseDC.As(&m_deviceContext);
	CHECK_FAILED(hr);

	//4xMSAA 확인
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

	//스왑체인 생성
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

	//렌더 타겟 설정
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

	//깊이 스텐실 버퍼 생성 및 뷰 생성
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

	//뷰포트 설정
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
	//이펙트 생성
	Effect* effect = CreateEffect(BasicEffectKey);

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

	//메쉬 생성
	Mesh* mesh = CreateMesh(CubeMeshKey);

	//버텍스 버퍼 생성
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

	//노멀 벡터 정규화
	for (auto& v : vertices)
	{
		v.Normal.Normalize();
	}

	mesh->SetVertexBuffer(m_device.Get(), vertices);

	//인덱스 버퍼 생성
	std::vector<UINT> indices = {
		0, 1, 2, 0, 2, 3,
		4, 6, 5, 4, 7, 6,
		1, 5, 6, 1, 6, 2,
		0, 3, 7, 0, 7, 4,
		0, 4, 5, 0, 5, 1,
		3, 2, 6, 3, 6, 7
	};

	mesh->SetIndexBuffer(m_device.Get(), indices);

	//RnederState 설정
	RenderState* renderState = CreateRenderState(BasicRenderStateKey);
	renderState->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderState->SetRasterizerState(m_device.Get());

	//컨스턴트 버퍼 설정

	//cbPerObject
	effect->RegisterConstantBuffer(
		m_device.Get(),
		sizeof(cbPerObject),
		CbPerObjectKey,
		0,
		CONSTANT_BUFFER_APPLY_VERTEX_SHADER | CONSTANT_BUFFER_APPLY_PIXEL_SHADER
	);

	//cbPerFrame
	effect->RegisterConstantBuffer(
		m_device.Get(),
		sizeof(cbPerFrame),
		CbPerFrameKey,
		1,
		CONSTANT_BUFFER_APPLY_PIXEL_SHADER
	);

	//뷰 투영 행렬 초기화
	//const static Vector3 s_cmrLookAt{ 0.f, 0.f, 5.f };
	//m_viewMat = ::XMMatrixLookAtLH(m_cmrPosition, s_cmrLookAt, m_worldUpAxis);
	//m_cmrLook = s_cmrLookAt - m_cmrPosition;
	//m_cmrLook.Normalize();
	//
	//m_projMat = ::XMMatrixPerspectiveFovLH(
	//	m_fov,
	//	WindowsApp::GetInst().GetAspectRatio(),
	//	m_nearPlane,
	//	m_farPlane);
	//
	////광원 초기화
	//m_dirLight.Ambient = Color(0.2f, 0.2f, 0.2f, 1.f);
	//m_dirLight.Diffuse = Color(1.0f, 0.95f, 0.8f, 1.f);
	//m_dirLight.Specular = Color(1.0f, 1.0f, 1.0f, 1.f);
	//m_dirLight.Direction = Vector3(2.f, -1.f, 2.f);
	//m_dirLight.Direction.Normalize();

	//머테리얼 설정
	Material* material = CreateMaterial(BasicMaterialKey);
	material->Ambient = Color(0.4f, 0.4f, 0.4f, 1.f);
	material->Diffuse = Color(0.5f, 0.5f, 0.5f, 1.f);
	material->Specular = Color(0.8f, 0.8f, 0.8f, 16.f);

	SetCurrentEffect(BasicEffectKey);
	SetCurrentRenderState(BasicRenderStateKey);
}

void Renderer::Render()
{
	////Constant Buffer Per Frame
	//{
	//	cbPerFrame cb = {};
	//	//cb.DirLight = m_dirLight;
	//	//cb.PointLight = m_pointLight;
	//	//cb.SpotLight = m_spotLight;
	//
	//	cb.EyePosW = ToVector4(m_cmrPosition, true);
	//
	//	m_curEffect->UpdateConstantBuffer(m_deviceContext.Get(), CbPerFrameKey, cb);
	//}
	//
	////Constant Buffer Per Object
	//for (const auto* mesh : m_cMashRendererRepo)
	//{
	//	if (mesh == nullptr)
	//	{
	//		//mesh는 삭제시 즉시 삭제하지 않고, 나중에 한 꺼번에 삭제됨
	//		//완전히 삭제되기 전까지는 nullptr 상태로 존재함
	//		continue;
	//	}
	//
	//	const Mesh* m = mesh->GetMesh();
	//	const CTransform* t = mesh->GetTransform();
	//
	//	//상수 버퍼 설정
	//	Matrix wMat = t->GetWorldMatrix();
	//
	//	cbPerObject cb = {};
	//	cb.Material = *mesh->GetMaterial();
	//	cb.World = ::XMMatrixTranspose(t->GetWorldMatrix());
	//	cb.WorldInvTranspose = t->GetWorldMatrixInverse();
	//
	//	m_curEffect->UpdateConstantBuffer(m_deviceContext.Get(), CbPerObjectKey, cb);
	//
	//	m->BindBuffers(m_deviceContext.Get());
	//	RenderIndices(m->GetIndexBufferSize());
	//}

	////렌더상태 초기화 할까 말까??
	//m_curEffect->Apply(m_deviceContext.Get());
	//m_curRenderState->Apply(m_deviceContext.Get());
	//m_curEffect->BindConstantBuffer(m_deviceContext.Get(), CbPerObjectKey);
	//m_curEffect->BindConstantBuffer(m_deviceContext.Get(), CbPerFrameKey);
	//
	////cbPerFrame 업데이트
	//{
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//}
	//
	//for (const auto& obj : m_rObjRepo)
	//{
	//	//cbPerObjct 업데이트
	//	cbPerObject cb = {};
	//
	//	const Transform& t = obj->GetTransform();
	//	const Mesh* m = obj->GetMesh();
	//
	//	m->BindBuffers(m_deviceContext.Get());
	//
	//	cb.World = ::XMMatrixTranspose(t.GetWorldMatrix());
	//	cb.WorldInvTranspose = ::XMMatrixInverse(nullptr, t.GetWorldMatrix());
	//	cb.ViewProj = ::XMMatrixTranspose(m_viewMat * m_projMat);
	//
	//	cb.Material = *obj->GetMaterial();
	//
	//	m_curEffect->UpdateConstantBuffer(
	//		m_deviceContext.Get(),
	//		CbPerObjectKey,
	//		cb
	//	);
	//	m_curEffect->BindConstantBuffer(
	//		m_deviceContext.Get(),
	//		CbPerObjectKey
	//	);
	//
	//	RenderIndices(m->GetIndexBufferSize());
	//}


	//오브젝트 렌더링
	
	for (const CMeshRenderer* meshComp : m_cMeshRendererRepo.GetVector())
	{
		if (meshComp == nullptr)
		{
			break;
		}

		if (!meshComp->IsEnable())
		{
			continue;
		}

		/* 렌더링 수행 */
	}
}

//========================================
// 
//	1. Light 관리 
//  2. 상수버퍼 관리
//	3. 렌더링 테스트 하면 끝! 
// 
//========================================

void Renderer::ResizeWindow(const WindowSize& winSize)
{
	//스왑체인, 렌더타겟, 깊이-스텐실 버퍼, 뷰포트 수정
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

	//origin 계산
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

	//origin 계산
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


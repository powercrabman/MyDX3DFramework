#include "pch.h"
#include "Renderer.h"
#include "WindowsApp.h"
#include "Engine.h"
#include <fstream>

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

void Renderer::CreateDeviceDependentResource()
{
	m_spriteBatch = std::make_unique<SpriteBatch>(m_deviceContext.Get());
}


void Renderer::InputAssembler()
{
	//Vertex Shader 컴파일
	ComPtr<ID3DBlob> vsBlob = nullptr;
	LoadAndCopileShaderFromFile(
		L"LightVertexShader.hlsl",
		"VS",
		"vs_5_0",
		vsBlob.GetAddressOf()
	);

	HRESULT hr = m_device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		nullptr,
		m_vertexShader.GetAddressOf()
	);

	CHECK_FAILED(hr);

	D3D11_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr = m_device->CreateInputLayout(
		inputLayout,
		ARRAYSIZE(inputLayout),
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		m_inputLayout.GetAddressOf());

	m_deviceContext->IASetInputLayout(m_inputLayout.Get());

	//픽셀쉐이더 컴파일
	ComPtr<ID3DBlob> psBlob = nullptr;
	LoadAndCopileShaderFromFile(
		L"LightPixelShader.hlsl",
		"PS",
		"ps_5_0",
		psBlob.GetAddressOf()
	);

	m_device->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		nullptr,
		m_pixelShader.GetAddressOf()
	);

	//버텍스 버퍼 생성
	VertexNormal vertices[] = {
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

	D3D11_BUFFER_DESC vbd = {};
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(VertexNormal) * 8;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vinitData = {};
	vinitData.pSysMem = vertices;

	//버텍스 버퍼 설정
	hr = m_device->CreateBuffer(&vbd, &vinitData, m_vertexBuffer.GetAddressOf());
	CHECK_FAILED(hr);

	UINT strid = sizeof(VertexNormal);
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &strid, &offset);

	//인덱스 버퍼 생성
	UINT indices[] = {
		0, 1, 2, 0, 2, 3,
		4, 6, 5, 4, 7, 6,
		1, 5, 6, 1, 6, 2,
		0, 3, 7, 0, 7, 4,
		0, 4, 5, 0, 5, 1,
		3, 2, 6, 3, 6, 7
	};

	D3D11_BUFFER_DESC ibd = {};
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(UINT) * 36;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	//인덱스 버퍼 설정
	D3D11_SUBRESOURCE_DATA iinitData = {};
	iinitData.pSysMem = indices;

	ComPtr<ID3D11Buffer> iBuffer = nullptr;
	hr = m_device->CreateBuffer(&ibd, &iinitData, m_indexBuffer.GetAddressOf());
	CHECK_FAILED(hr);

	m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	//기하 토폴로지													
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Constant Buffer Per Object
	{
		D3D11_BUFFER_DESC desc = {};
		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(cbPerObject);
		desc.Usage = D3D11_USAGE_DEFAULT;
		hr = m_device->CreateBuffer(&desc, nullptr, m_cBufferPerObject.GetAddressOf());
		CHECK_FAILED(hr);
	}

	//Contant Buffer Per Frame
	{
		D3D11_BUFFER_DESC desc = {};
		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(cbPerFrame);
		desc.Usage = D3D11_USAGE_DEFAULT;
		hr = m_device->CreateBuffer(&desc, nullptr, m_cBufferPerFrame.GetAddressOf());
		CHECK_FAILED(hr);
	}

	//뷰 투영 행렬 초기화
	const static Vector3 s_cmrLookAt{ 0.f, 0.f, 5.f };
	m_viewMat = ::XMMatrixLookAtLH(m_cmrPosition, s_cmrLookAt, m_worldUpAxis);
	m_cmrLook = s_cmrLookAt - m_cmrPosition;
	m_cmrLook.Normalize();

	m_projMat = ::XMMatrixPerspectiveFovLH(
		m_fov,
		WindowsApp::GetInst().GetAspectRatio(),
		m_nearPlane,
		m_farPlane);

	//광원 초기화
	m_spotLight.Ambient  = Color(0.2f, 0.2f, 0.2f, 1.f);
	m_spotLight.Diffuse  = Color(1.0f, 0.95f, 0.8f, 1.f);
	m_spotLight.Specular = Color(1.0f, 1.0f, 1.0f, 1.f);
	m_spotLight.SpotDirection = Vector3(0.f, 0.f, 1.f);
	m_spotLight.Attenuation = Vector3(1.f,0.1f,0.f);
	m_spotLight.Range = 50.f;

	//머테리얼 설정
	m_material.Ambient  = Color(0.4f, 0.4f, 0.4f, 1.f);
	m_material.Diffuse  = Color(0.5f, 0.5f, 0.5f, 1.f);
	m_material.Specular = Color(0.8f, 0.8f, 0.8f, 16.f);
}

void Renderer::Update(float inDeltaTime)
{
	//Move Camera
	{
		static float s_cmrMoveSpeed = 3.f;
		static float s_cmrRotateSpeed = ::XMConvertToRadians(30.f);

		if (KEY_HOLD(eKeyCode::A))
		{
			Vector3 dir = -m_worldUpAxis.Cross(m_cmrLook);
			m_cmrPosition += dir * s_cmrMoveSpeed * inDeltaTime;
		}

		if (KEY_HOLD(eKeyCode::D))
		{
			Vector3 dir = m_worldUpAxis.Cross(m_cmrLook);
			m_cmrPosition += dir * s_cmrMoveSpeed * inDeltaTime;
		}

		if (KEY_HOLD(eKeyCode::S))
		{
			m_cmrPosition += -m_cmrLook * s_cmrMoveSpeed * inDeltaTime;
		}

		if (KEY_HOLD(eKeyCode::W))
		{
			m_cmrPosition += m_cmrLook * s_cmrMoveSpeed * inDeltaTime;
		}

		if (KEY_HOLD(eKeyCode::Space))
		{
			m_cmrPosition.y += s_cmrMoveSpeed * inDeltaTime;
		}

		if (KEY_HOLD(eKeyCode::L_Shift))
		{
			m_cmrPosition.y += -s_cmrMoveSpeed * inDeltaTime;
		}

		if (KEY_HOLD(eKeyCode::Left))
		{
			m_cmrLook = ::XMVector3Rotate(m_cmrLook, Quaternion::CreateFromYawPitchRoll(-s_cmrRotateSpeed * inDeltaTime,0.f,0.f));
		}

		if (KEY_HOLD(eKeyCode::Right))
		{
			m_cmrLook = ::XMVector3Rotate(m_cmrLook, Quaternion::CreateFromYawPitchRoll(s_cmrRotateSpeed * inDeltaTime, 0.f, 0.f));
		}

		if (KEY_HOLD(eKeyCode::Up))
		{
			m_cmrLook = ::XMVector3Rotate(m_cmrLook, Quaternion::CreateFromYawPitchRoll(0.f, -s_cmrRotateSpeed * inDeltaTime, 0.f));
		}

		if (KEY_HOLD(eKeyCode::Down))
		{
			m_cmrLook = ::XMVector3Rotate(m_cmrLook, Quaternion::CreateFromYawPitchRoll(0.f, s_cmrRotateSpeed * inDeltaTime, 0.f));
		}
	}

	static const float s_rotateSpeed = ::XMConvertToRadians(30.f);
	float s_rotateDelta = inDeltaTime * s_rotateSpeed;

	//Update cbPerObject
	{
		cbPerObject cb = {};

		// m_rotation *= Quaternion::CreateFromYawPitchRoll(s_rotateDelta, 0, 0);

		Matrix world = ::XMMatrixAffineTransformation(m_scale, Vector3::Zero, m_rotation, m_position);
		m_viewMat = ::XMMatrixLookToLH(m_cmrPosition, m_cmrLook, m_worldUpAxis);

		cb.World = ::XMMatrixTranspose(world);
		cb.WorldInvTranspose = ::XMMatrixInverse(nullptr, world);
		cb.ViewProj = ::XMMatrixTranspose(m_viewMat * m_projMat);
		cb.Material = m_material;

		m_deviceContext->UpdateSubresource(m_cBufferPerObject.Get(), 0, nullptr, &cb, 0, 0);
	}

	//Update cbPerFrame
	{
		cbPerFrame cb = {};
		m_spotLight.Position = m_cmrPosition;

		cb.gEyePosW = ToVector4(m_cmrPosition, true);
		cb.gDirLight = m_dirLight;
		cb.gPointLight = m_pointLight;
		cb.gSpotLight = m_spotLight;

		m_deviceContext->UpdateSubresource(m_cBufferPerFrame.Get(), 0, nullptr, &cb, 0, 0);
	}
}

void Renderer::Render()
{
	//렌더링 상태 재설정
	UINT strid = sizeof(VertexNormal);
	UINT offset = 0;

	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_deviceContext->IASetInputLayout(m_inputLayout.Get());
	m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &strid, &offset);

	// Renders a triangle
	m_deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	m_deviceContext->VSSetConstantBuffers(0, 1, m_cBufferPerObject.GetAddressOf());

	m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	m_deviceContext->PSSetConstantBuffers(0, 1, m_cBufferPerObject.GetAddressOf());
	m_deviceContext->PSSetConstantBuffers(1, 1, m_cBufferPerFrame.GetAddressOf());

	// 큐브를 그리기 위한 인덱스 개수 지정
	m_deviceContext->DrawIndexed(36, 0, 0);
}

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

void Renderer::DrawString(const std::wstring& inStr, const Vector2& inScreenPos, eFont inFont, const XMVECTORF32& inColor, eFontAlign align, float scale)
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

void Renderer::LoadAndCopileShaderFromFile(
	const std::wstring& inFilename,
	const std::string& inEntryPoint,
	const std::string& inTarget,
	ID3DBlob** outInBlob)
{
	std::ifstream file(inFilename);
	VERTIFY(file.is_open(), L"쉐이더 파일 로드 실패");

	std::stringstream buffer = {};
	buffer << file.rdbuf();

	std::string shaderCode = buffer.str();

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
		inEntryPoint.c_str(),
		inTarget.c_str(),
		compileFlags,
		0,
		outInBlob,
		errorBlob.GetAddressOf());

	CHECK_FAILED_MESSAGE(hr, MA::StringToWstring((char*)errorBlob->GetBufferPointer()).c_str());
}


#include "pch.h"
#include "Effect.h"
#include "Renderer.h"

void Effect::SetProperties(ID3D11Device* inDevice, std::wstring_view inVertexShaderFilePath, std::string_view inVertexShaderEntryPointName, std::string_view inVertexShaderModelVersion, std::wstring_view inPixelShaderFilePath, std::string_view inPixelShaderEntryPointName, std::string_view inPixelShaderModelVersion, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inInputElementDesc)
{
	m_inputLayout.Reset();
	m_vertexShader.Reset();
	m_pixelShader.Reset();

	//πˆ≈ÿΩ∫ Ω¶¿Ã¥ı
	ComPtr<ID3DBlob> blob = nullptr;
	Renderer::GetInst().LoadAndCopileShaderFromFile(
		inVertexShaderFilePath,
		inVertexShaderEntryPointName,
		inVertexShaderModelVersion,
		blob.GetAddressOf()
	);

	HRESULT hr = inDevice->CreateVertexShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		m_vertexShader.GetAddressOf()
	);
	CHECK_FAILED(hr);

	//¿Œ«≤ ∑π¿Ãæ∆øÙ º≥¡§
	hr = inDevice->CreateInputLayout(
		inInputElementDesc.data(),
		inInputElementDesc.size(),
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		m_inputLayout.GetAddressOf()
	);
	CHECK_FAILED(hr);

	//«»ºø Ω¶¿Ã¥ı
	Renderer::GetInst().LoadAndCopileShaderFromFile(
		inPixelShaderFilePath,
		inPixelShaderEntryPointName,
		inPixelShaderModelVersion,
		blob.GetAddressOf()
	);

	hr = inDevice->CreatePixelShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		m_pixelShader.GetAddressOf()
	);
	CHECK_FAILED(hr);
}

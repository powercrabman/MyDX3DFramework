#include "pch.h"
#include "Effect.h"
#include "Renderer.h"

void Effect::SetProperties(ID3D11Device* inDevice, const std::wstring& inVertexShaderFilePath, const std::string& inVertexShaderEntryPointName, const std::string& inVertexShaderModelVersion, const std::wstring& inPixelShaderFilePath, const std::string& inPixelShaderEntryPointName, const std::string& inPixelShaderModelVersion, const std::vector<D3D11_INPUT_ELEMENT_DESC> inInputElementDesc)
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

	inDevice->CreateVertexShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		m_vertexShader.GetAddressOf()
	);

	//¿Œ«≤ ∑π¿Ãæ∆øÙ º≥¡§
	inDevice->CreateInputLayout(
		inInputElementDesc.data(),
		inInputElementDesc.size(),
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		m_inputLayout.GetAddressOf()
	);

	//«»ºø Ω¶¿Ã¥ı
	Renderer::GetInst().LoadAndCopileShaderFromFile(
		inPixelShaderFilePath,
		inPixelShaderEntryPointName,
		inPixelShaderModelVersion,
		blob.GetAddressOf()
	);

	inDevice->CreatePixelShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		m_pixelShader.GetAddressOf()
	);
}

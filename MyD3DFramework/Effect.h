#pragma once

enum CONSTANT_BUFFER_APPLY_FLAGS
{
	CONSTANT_BUFFER_APPLY_VERTEX_SHADER = 0b0001,
	CONSTANT_BUFFER_APPLY_PIXEL_SHADER = 0b0010,
};

class Effect
{
public:
	Effect() = default;
	virtual ~Effect() = default;

	void SetProperties(
		ID3D11Device* inDevice, 
		std::wstring_view inVertexShaderFilePath, 
		std::string_view inVertexShaderEntryPointName, 
		std::string_view inVertexShaderModelVersion, 
		std::wstring_view inPixelShaderFilePath, 
		std::string_view inPixelShaderEntryPointName, 
		std::string_view inPixelShaderModelVersion, 
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& inInputElementDesc
	);

	void CreateConstantBuffer(
		ID3D11Device* inDevice,
		uint32 inContantBuffeStructSize,
		const std::string inConstantBufferName,
		uint32 inSlotToUse,
		uint32 inApplyFlags
	);

	inline void Apply(ID3D11DeviceContext* inDeviceContext);

	template <typename ConstantBufferStruct>
	inline void UpdateConstantBuffer(
		ID3D11DeviceContext* inDeviceContext,
		const std::string& inConstantBufferKey,
		const ConstantBufferStruct& inStructData
	);

	inline void BindConstantBuffer(
		ID3D11DeviceContext* inDeviceContext,
		const std::string& inConstantBufferKey
	);

	inline void PrintDebugLogAboutConstantBuffer(
		ID3D11Device* device,
		ID3D11DeviceContext* context,
		const std::string& key
	);

private:
	struct ConstantBufferInfo
	{
		ComPtr<ID3D11Buffer> ConstantBuffer = nullptr;
		uint32 Flags = 0;
		uint32 Slot = 0;
	};

private:
	ComPtr<ID3D11InputLayout> m_inputLayout = nullptr;

	ComPtr<ID3D11VertexShader> m_vertexShader = nullptr;
	ComPtr<ID3D11PixelShader> m_pixelShader = nullptr;

	std::unordered_map<std::string, ConstantBufferInfo> m_cbRepo;
}; 

inline void Effect::Apply(ID3D11DeviceContext* inDeviceContext)
{
	assert(m_inputLayout);
	assert(inDeviceContext);
	assert(m_vertexShader);
	assert(m_pixelShader);

	inDeviceContext->IASetInputLayout(m_inputLayout.Get());
	inDeviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	inDeviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);
}

inline void Effect::BindConstantBuffer(ID3D11DeviceContext* inDeviceContext, const std::string& inConstantBufferKey)
{
	assert(m_cbRepo.contains(inConstantBufferKey));
	const ConstantBufferInfo& cbInfo = m_cbRepo[inConstantBufferKey];

	if (cbInfo.Flags & CONSTANT_BUFFER_APPLY_VERTEX_SHADER)
	{
		inDeviceContext->VSSetConstantBuffers(
			cbInfo.Slot,
			1,
			cbInfo.ConstantBuffer.GetAddressOf()
		);
	}

	if (cbInfo.Flags & CONSTANT_BUFFER_APPLY_PIXEL_SHADER)
	{
		inDeviceContext->PSSetConstantBuffers(
			cbInfo.Slot,
			1,
			cbInfo.ConstantBuffer.GetAddressOf()
		);
	}
}

inline void Effect::PrintDebugLogAboutConstantBuffer(ID3D11Device* device, ID3D11DeviceContext* context, const std::string& key)
{
	ComPtr<ID3D11Buffer> constantBuffer = m_cbRepo[key].ConstantBuffer;

	D3D11_BUFFER_DESC desc = {};
	constantBuffer->GetDesc(&desc);

	uint32 bufferSize = desc.ByteWidth;

	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.MiscFlags = 0;

	ComPtr<ID3D11Buffer> stagingBuffer;
	HRESULT hr = device->CreateBuffer(&desc, nullptr, stagingBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		WinUtile::OutputLog(L"Failed to create staging buffer\n");
		return;
	}

	context->CopyResource(stagingBuffer.Get(), constantBuffer.Get());

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(stagingBuffer.Get(), 0, D3D11_MAP_READ, 0, &mappedResource);
	if (SUCCEEDED(hr))
	{
		std::byte* bufferData = static_cast<std::byte*>(mappedResource.pData);
		for (size_t i = 0; i < bufferSize; ++i)
		{
			auto& b = Buffer512W::GetInst();
			b.Input(L"Byte[%zu] = %d\n", i, bufferData[i]);
			WinUtile::OutputLog(b.c_str());
		}

		context->Unmap(stagingBuffer.Get(), 0);
	}
	else
	{
		WinUtile::OutputLog(L"Failed to map staging buffer\n");
	}
}

template<typename ConstantBufferStruct>
inline void Effect::UpdateConstantBuffer(ID3D11DeviceContext* inDeviceContext, const std::string& inConstantBufferKey, const ConstantBufferStruct& inStructData)
{
	assert(m_cbRepo.contains(inConstantBufferKey));
	const ConstantBufferInfo& cbInfo = m_cbRepo[inConstantBufferKey];

	inDeviceContext->UpdateSubresource(cbInfo.ConstantBuffer.Get(), 0, nullptr, &inStructData, 0, 0);
}

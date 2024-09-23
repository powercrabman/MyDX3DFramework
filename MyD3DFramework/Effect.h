#pragma once

enum class eContantBufferApplyFlags
{
	VertexShader = 0b0001,
	PixelShader = 0b0010
};

class Effect
{
public:
	Effect() = default;
	virtual ~Effect() = default;

	void SetProperties(
		ID3D11Device* inDevice,
		const std::wstring& inVertexShaderFilePath,
		const std::string& inVertexShaderEntryPointName,
		const std::string& inVertexShaderModelVersion,
		const std::wstring& inPixelShaderFilePath,
		const std::string& inPixelShaderEntryPointName,
		const std::string& inPixelShaderModelVersion,
		const std::vector<D3D11_INPUT_ELEMENT_DESC> inInputElementDesc
	);

	void RegisterConstantBuffer(
		ID3D11Device* inDevice,
		uint32 inContantBuffeStructSize,
		const std::wstring inConstantBufferName,
		uint32 inSlotToUse,
		uint32 inApplyFlags
	)
	{
		assert(!m_cbRepo.contains(inConstantBufferName));

		ConstantBufferInfo cbInfo = {};

		D3D11_BUFFER_DESC desc = {};
		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = inContantBuffeStructSize;
		desc.Usage = D3D11_USAGE_DEFAULT;
		HRESULT hr = inDevice->CreateBuffer(&desc, nullptr, cbInfo.ConstantBuffer.GetAddressOf());
		CHECK_FAILED(hr);

		cbInfo.Slot = inSlotToUse;
		cbInfo.Flags = inApplyFlags;

		m_cbRepo[inConstantBufferName] = cbInfo;
	}

	inline void Apply(ID3D11DeviceContext* inDeviceContext)
	{
		assert(m_inputLayout);
		assert(inDeviceContext);
		assert(m_vertexShader);
		assert(m_pixelShader);

		inDeviceContext->IASetInputLayout(m_inputLayout.Get());
		inDeviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		inDeviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	}

	template <typename ConstantBufferStruct>
	inline void UpdateConstantBuffer(
		ID3D11DeviceContext* inDeviceContext,
		const std::wstring& inConstantBufferName,
		const ConstantBufferStruct& inStructData
	)
	{
		assert(m_cbRepo.contains(inConstantBufferName));;
		ConstantBufferInfo& cb = m_cbRepo[inConstantBufferName];

		D3D11_MAPPED_SUBRESOURCE mapRes = { };
		HRESULT hr = inDeviceContext->Map(
			cb.ConstantBuffer.Get(),
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&mapRes
		);

		CHECK_FAILED(hr);

		memcpy(mapRes.pData, inStructData, sizeof(ConstantBufferStruct));
		inDeviceContext->Unmap(cb.ConstantBuffer.Get(), 0);
	}


	inline void BindConstantBuffers(ID3D11DeviceContext* inDeviceContext)
	{
		for (const auto& item : m_cbRepo)
		{
			const ConstantBufferInfo& cbInfo = item.second;
			
			if (cbInfo.Flags & static_cast<uint32>(eContantBufferApplyFlags::VertexShader))
			{
				inDeviceContext->VSSetConstantBuffers(
					cbInfo.Slot,
					1,
					cbInfo.ConstantBuffer.GetAddressOf()
					);
			}

			if (cbInfo.Flags & static_cast<uint32>(eContantBufferApplyFlags::PixelShader))
			{
				inDeviceContext->PSSetConstantBuffers(
					cbInfo.Slot,
					1,
					cbInfo.ConstantBuffer.GetAddressOf()
				);
			}
		}
	}

private:
	struct ConstantBufferInfo
	{
		ComPtr<ID3D11Buffer> ConstantBuffer;
		uint32 Flags;
		uint32 Slot;
	};

private:
	ComPtr<ID3D11InputLayout> m_inputLayout = nullptr;

	ComPtr<ID3D11VertexShader> m_vertexShader = nullptr;
	ComPtr<ID3D11PixelShader> m_pixelShader = nullptr;

	std::unordered_map<std::wstring, ConstantBufferInfo> m_cbRepo;
};
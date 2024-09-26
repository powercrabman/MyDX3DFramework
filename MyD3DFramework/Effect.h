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
		const std::wstring& inVertexShaderFilePath,
		const std::string& inVertexShaderEntryPointName,
		const std::string& inVertexShaderModelVersion,
		const std::wstring& inPixelShaderFilePath,
		const std::string& inPixelShaderEntryPointName,
		const std::string& inPixelShaderModelVersion,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& inInputElementDesc
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
		const std::wstring& inConstantBufferKey,
		const ConstantBufferStruct& inStructData
	)
	{
		assert(m_cbRepo.contains(inConstantBufferKey));
		const ConstantBufferInfo& cbInfo = m_cbRepo[inConstantBufferKey];

		inDeviceContext->UpdateSubresource(cbInfo.ConstantBuffer.Get(), 0, nullptr, &inStructData, 0, 0);
	}

	inline void BindConstantBuffer(
		ID3D11DeviceContext* inDeviceContext,
		const std::wstring& inConstantBufferKey
	)
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

	inline void PrintDebugLogAboutConstantBuffer(
		ID3D11Device* device,
		ID3D11DeviceContext* context,
		const std::wstring& key
	)
	{
		ComPtr<ID3D11Buffer> constantBuffer = m_cbRepo[key].ConstantBuffer;

		// 원본 컨스턴트 버퍼의 설명을 가져옴
		D3D11_BUFFER_DESC desc = {};
		constantBuffer->GetDesc(&desc);

		uint32 bufferSize = desc.ByteWidth;

		// 스테이징 버퍼 생성
		desc.Usage = D3D11_USAGE_STAGING;  // 스테이징 버퍼로 설정
		desc.BindFlags = 0;                // 스테이징 버퍼는 BindFlags가 필요 없음
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;  // CPU에서 읽기 가능
		desc.MiscFlags = 0;

		ComPtr<ID3D11Buffer> stagingBuffer;
		HRESULT hr = device->CreateBuffer(&desc, nullptr, stagingBuffer.GetAddressOf());
		if (FAILED(hr))
		{
			WinUtile::OutputLog(L"Failed to create staging buffer\n");
			return;
		}

		// 스테이징 버퍼로 컨스턴트 버퍼 데이터 복사
		context->CopyResource(stagingBuffer.Get(), constantBuffer.Get());

		// 스테이징 버퍼 맵핑 (읽기 전용)
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		hr = context->Map(stagingBuffer.Get(), 0, D3D11_MAP_READ, 0, &mappedResource);
		if (SUCCEEDED(hr))
		{
			// 데이터를 읽어와서 출력 (예시)
			std::byte* bufferData = static_cast<std::byte*>(mappedResource.pData);
			for (size_t i = 0; i < bufferSize; ++i)
			{
				auto& b = Buffer512W::GetInst();
				b.Input(L"Byte[%zu] = %d\n", i, bufferData[i]);
				WinUtile::OutputLog(b.c_str());
			}

			// 맵핑 해제
			context->Unmap(stagingBuffer.Get(), 0);
		}
		else
		{
			WinUtile::OutputLog(L"Failed to map staging buffer\n");
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
#pragma once

class RenderState
{
public:
	RenderState() = default;
	virtual ~RenderState() = default;

	void Apply(ID3D11DeviceContext* inDeviceContext)
	{
		inDeviceContext->RSSetState(m_rasterizerState.Get());
	}

	void SetRasterizerState(
		ID3D11Device* inDevice,
		D3D11_FILL_MODE fillMode = D3D11_FILL_SOLID,
		D3D11_CULL_MODE cullMode = D3D11_CULL_BACK,
		bool isfrontFaceCCW = false,
		bool useAntiAlians = false,
		bool useMultiSampling = false
		)
	{
		m_rasterizerState.Reset();

		D3D11_RASTERIZER_DESC desc = { };
		desc.FillMode = fillMode;
		desc.CullMode = cullMode;
		desc.FrontCounterClockwise = isfrontFaceCCW;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0.f;
		desc.SlopeScaledDepthBias = 0.f;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = useMultiSampling;
		desc.AntialiasedLineEnable = useAntiAlians;

		HRESULT hr = inDevice->CreateRasterizerState(
			&desc,
			m_rasterizerState.GetAddressOf()
		);

		CHECK_FAILED(hr);
	}

private:
	ComPtr<ID3D11RasterizerState> m_rasterizerState = nullptr;
};

#pragma once

class Mesh
{
public:
	Mesh() = default;
	virtual ~Mesh() = default;

	template<typename VertexType>
	inline void SetVertexBuffer(ID3D11Device* inDevice, const std::vector<VertexType>& inVertices);
	inline void SetIndexBuffer(ID3D11Device* inDevice, const std::vector<UINT>& inIndices);

	inline void BindBuffers(ID3D11DeviceContext* inDeviceContext);
private:
	ComPtr<ID3D11Buffer> m_indexBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_vertexBuffer = nullptr;
	uint32 m_strides = 0;	//TEMP
	uint32 m_offset = 0;	//TEMP
};

template<typename VertexType>
inline void Mesh::SetVertexBuffer(ID3D11Device* inDivice, const std::vector<VertexType>& inVertices)
{
	m_vertexBuffer.Reset();

	D3D11_BUFFER_DESC desc = {};
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = inVertices.size() * sizeof(VertexType);
	desc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA subData = {};
	subData.pSysMem = inVertices.data();

	CHECK_FAILED(inDivice->CreateBuffer(&desc, &subData, m_vertexBuffer.GetAddressOf()));
}

inline void Mesh::SetIndexBuffer(ID3D11Device* inDivice, const std::vector<UINT>& inIndices)
{
	m_indexBuffer.Reset();

	D3D11_BUFFER_DESC desc = {};
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = inIndices.size() * sizeof(UINT);
	desc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA subData = {};
	subData.pSysMem = inIndices.data();

	CHECK_FAILED(inDivice->CreateBuffer(&desc, &subData, m_vertexBuffer.GetAddressOf()));
}

inline void Mesh::BindBuffers(ID3D11DeviceContext* inDeviceContext)
{
	inDeviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	inDeviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &m_strides, &m_offset);
}

#pragma once

class Mesh
{
public:
	template<typename VertexType>
	inline void SetVertexBuffer(ID3D11Device* inDivice, const std::vector<VertexType>& inVertices);

	inline void SetIndexBuffer(ID3D11Device* inDivice, const std::vector<UINT>& inIndices);

private:
	ComPtr<ID3D11Buffer> m_indexBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_vertexBuffer = nullptr;
};

template<typename VertexType>
inline void Mesh::SetVertexBuffer(ID3D11Device* inDivice, const std::vector<VertexType>& inVertices)
{
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
	D3D11_BUFFER_DESC desc = {};
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = inIndices.size() * sizeof(UINT);
	desc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA subData = {};
	subData.pSysMem = inIndices.data();

	CHECK_FAILED(inDivice->CreateBuffer(&desc, &subData, m_vertexBuffer.GetAddressOf()));
}

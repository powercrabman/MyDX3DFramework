#pragma once

class ShaderIncludeHelper final : public ID3DInclude
{
    HRESULT __stdcall Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) override;

    HRESULT __stdcall Close(LPCVOID pData) override;
};


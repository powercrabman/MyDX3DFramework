#include "pch.h"
#include "ShaderIncludeHelper.h"
#include <fstream>

HRESULT __stdcall ShaderIncludeHelper::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes)
{
    std::ifstream file(pFileName, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        return E_FAIL;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* buffer = new char[size];
    if (!file.read(buffer, size))
    {
        delete[] buffer;
        return E_FAIL;
    }

    *ppData = buffer;
    *pBytes = static_cast<UINT>(size);

    return S_OK;
}

HRESULT __stdcall ShaderIncludeHelper::Close(LPCVOID pData)
{
    delete[] pData;
    return S_OK;
}

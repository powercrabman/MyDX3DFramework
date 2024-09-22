#pragma once

//DirectX
#include <d3d11_4.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <wrl/client.h> 
#include <d3dcompiler.h>
using namespace Microsoft::WRL;
using namespace DirectX;

#pragma comment(lib, "D3DCompiler.lib")

#include "BufferHelpers.h"
#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "DirectXHelpers.h"
#include "Effects.h"
#include "GamePad.h"
#include "GeometricPrimitive.h"
#include "GraphicsMemory.h"
#include "Keyboard.h"
#include "Model.h"
#include "Mouse.h"
#include "PostProcess.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"
using namespace DirectX::SimpleMath;

#include "ShaderIncludeHelper.h"

//Windows
#include <Windows.h>

//Standard
#include <cassert>
#include <string>
#include <sstream> 
#include <thread>
#include <filesystem>
#include <locale>

//STL
#include <array>

//MyInclude
#include "Types.h"
#include "Structs.h"
#include "Macro.h"
#include "WindowsUtile.h"
#include "InputManager.h"
#include "Enums.h"
#include "ResourceManager.h"
#include "BufferPool.h"
#include "Algorithm.h"
#include "SimpleMathAdv.h"

//About Rendering
#include "Vertex.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "ContantBuffers.h"

//Resource
#include "Font.h"
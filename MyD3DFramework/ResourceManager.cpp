#include "pch.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <filesystem>
namespace fs = std::filesystem;

void ResourceManager::Initialize()
{
	m_resourcePath = fs::current_path().parent_path();
	m_resourcePath += L"\\Resource\\";
}

void ResourceManager::LoadResource()
{
	LoadFont();
}

void ResourceManager::LoadFont()
{
	std::wstring fontResourcePath = m_resourcePath + L"Font\\";
	ID3D11Device5* device = Renderer::GetInst().GetDeivce();

	auto CreateSpriteFont = [&](const std::wstring& fileName)
		{
			return std::make_unique<SpriteFont>(device, (fontResourcePath + fileName).c_str());
		};

	m_fontStorage[(int)eFont::G_Market_Sans_8pt]	= CreateSpriteFont(L"GMarketSansTTFM8.spritefont");
	m_fontStorage[(int)eFont::G_Market_Sans_16pt]	= CreateSpriteFont(L"GMarketSansTTFM16.spritefont");
	m_fontStorage[(int)eFont::G_Market_Sans_32pt]	= CreateSpriteFont(L"GMarketSansTTFM32.spritefont");
	m_fontStorage[(int)eFont::G_Market_Sans_64pt]	= CreateSpriteFont(L"GMarketSansTTFM64.spritefont");
	m_fontStorage[(int)eFont::G_Market_Sans_128pt]	= CreateSpriteFont(L"GMarketSansTTFM128.spritefont");

	m_fontStorage[(int)eFont::Nanum_Gothic_8pt]		= CreateSpriteFont(L"NaNumGothic8.spritefont");
	m_fontStorage[(int)eFont::Nanum_Gothic_16pt]	= CreateSpriteFont(L"NaNumGothic16.spritefont");
	m_fontStorage[(int)eFont::Nanum_Gothic_32pt]	= CreateSpriteFont(L"NaNumGothic32.spritefont");
	m_fontStorage[(int)eFont::Nanum_Gothic_64pt]	= CreateSpriteFont(L"NaNumGothic64.spritefont");
	m_fontStorage[(int)eFont::Nanum_Gothic_128pt]	= CreateSpriteFont(L"NaNumGothic128.spritefont");

	m_fontStorage[(int)eFont::Consolas_8pt] = CreateSpriteFont(L"Consolas8.spritefont");
	m_fontStorage[(int)eFont::Consolas_16pt] = CreateSpriteFont(L"Consolas16.spritefont");
	m_fontStorage[(int)eFont::Consolas_32pt] = CreateSpriteFont(L"Consolas32.spritefont");
	m_fontStorage[(int)eFont::Consolas_64pt] = CreateSpriteFont(L"Consolas64.spritefont");
	m_fontStorage[(int)eFont::Consolas_128pt] = CreateSpriteFont(L"Consolas128.spritefont");

	for (size_t i = 0; i < m_fontStorage.size(); ++i)
	{
		VERTIFY(m_fontStorage[i].get(), L"폰스 리소스 로딩 실패");
	}
}


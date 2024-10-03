#pragma once

class ResourceManager
{
	DEFINE_SINGLE(ResourceManager);
private:
	ResourceManager() = default;
	~ResourceManager() = default;

public:
	void Initialize();
	void LoadResource();

	void LoadFont();
	SpriteFont* GetFont(eFont inFont);

	const std::wstring& GetResourcePath() const;

private:
	//로컬 경로
	std::wstring m_resourcePath = {};
	std::array<std::unique_ptr<SpriteFont>, (size_t)eFont::Count> m_fontStorage = {};
};

inline SpriteFont* ResourceManager::GetFont(eFont inFont)
{
	return m_fontStorage[(size_t)inFont].get();
}

const std::wstring& ResourceManager::GetResourcePath() const
{
	return m_resourcePath;
}

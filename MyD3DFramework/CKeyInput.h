#pragma once
#include "CBehavior.h"

class CKeyInput : public CBehavior
{
private:
	struct ActionKey
	{
		ActionKey() = default;
		ActionKey(const eKeyCode& inKeyCode, const eKeyState& inKeyState)
			: KeyCode(inKeyCode), KeyState(inKeyState) {}

		bool operator==(const ActionKey& other) const
		{
			return (KeyCode == other.KeyCode) && (KeyState == other.KeyState);
		}

		eKeyCode KeyCode;
		eKeyState KeyState;
	};

	struct ActionValue
	{
		std::function<void()> Action;
		size_t Index;
		ActionKey Key;
	};

	struct ActionKeyHash
	{
		std::size_t operator()(const ActionKey& key) const
		{
			return std::hash<int>()(static_cast<int>(key.KeyCode)) ^ std::hash<int>()(static_cast<int>(key.KeyState) << 1);
		}
	};

public:
	CKeyInput();
	~CKeyInput() = default;

	template <typename ObjType, typename Func>
	void BindAction(
		eKeyCode inKeyCode,
		eKeyState inKeyState,
		ObjType* inObj,
		Func inFunc);

	bool UnBindAction(eKeyCode inKeyCode, eKeyState inKeyState);

	void Update();

private:
	bool CheckKeyState(eKeyState state, eKeyCode code);
	void RegisterAction(ActionKey inKey, const std::function<void()>& inAction);

private:
	std::unordered_map<ActionKey, ActionValue, ActionKeyHash> m_actionRepo;
	std::vector<ActionValue*> m_actionVector;
	size_t m_validActionCount = 0;
	constexpr static size_t sReserveCapacity = 16;
};

inline CKeyInput::CKeyInput()
{
	m_actionRepo.reserve(sReserveCapacity);
	m_actionVector.reserve(sReserveCapacity);
}

inline bool CKeyInput::UnBindAction(eKeyCode inKeyCode, eKeyState inKeyState)
{
	ActionKey key{ inKeyCode, inKeyState };
	auto iter = m_actionRepo.find(key);

	if (iter == m_actionRepo.end())
	{
		return false;
	}

	size_t index = iter->second.Index;
	m_actionVector[index] = m_actionVector[m_validActionCount - 1];
	m_actionRepo[m_actionVector[index]->Key].Index = index;
	m_actionVector.pop_back();
	--m_validActionCount;

	m_actionRepo.erase(iter);

	return true;
}

inline bool CKeyInput::CheckKeyState(eKeyState state, eKeyCode code)
{
	switch (state)
	{
	case eKeyState::Press:   return InputManager::GetInst().GetKeyPress(code);
	case eKeyState::Hold:    return InputManager::GetInst().GetKeyHold(code);
	case eKeyState::Release: return InputManager::GetInst().GetKeyRelease(code);
	case eKeyState::Away:    return InputManager::GetInst().GetKeyAway(code);
	default:                 assert(false); return false;
	}
}

inline void CKeyInput::RegisterAction(ActionKey inKey, const std::function<void()>& inAction)
{
	auto iter = m_actionRepo.find(inKey);
	if (iter == m_actionRepo.end())
	{
		ActionValue value{ inAction, m_validActionCount, inKey };
		const auto& [it, result] = m_actionRepo.emplace(inKey, value);

		m_actionVector.push_back(&it->second);
		++m_validActionCount;
	}
	else
	{
		iter->second.Action = inAction;
		m_actionVector[iter->second.Index] = &iter->second;
	}
}

template<typename ObjType, typename Func>
inline void CKeyInput::BindAction(eKeyCode inKeyCode, eKeyState inKeyState, ObjType* inObj, Func inFunc)
{
	static_assert(std::is_base_of<GameObject, ObjType>::value);
	static_assert(std::is_member_function_pointer<Func>::value);
	RegisterAction(ActionKey{ inKeyCode, inKeyState }, [inObj, inFunc]() { (inObj->*inFunc)(); });
}

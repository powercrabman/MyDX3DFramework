#include "pch.h"
#include "CKeyInput.h"

void CKeyInput::Update()
{
    for (auto& action : m_actionVector)
    {
        const ActionKey& key = action->Key;
        if (CheckKeyState(key.KeyState, key.KeyCode))
        {
            action->Action();
        }
    }
}

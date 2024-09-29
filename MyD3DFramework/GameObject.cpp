#include "pch.h"
#include "GameObject.h"


void GameObject::InitalizeCore()
{
	Initialize();
}

void GameObject::UpdateCore(float inDeltaTime)
{
	for (size_t i = 0; i < m_validCompSizeInVector; ++i)
	{
		CBehavior* cmp = m_updateCompRepo[i];

		if (cmp->IsEnable())
		{
			UpdateCore(inDeltaTime);
		}
	}

	Update(inDeltaTime);
}


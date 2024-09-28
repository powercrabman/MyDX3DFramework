#include "pch.h"
#include "GameObject.h"
#include "CBehavior.h"

void GameObject::InitalizeCore()
{
	Initialize();

	CM::TypeTrait<int>::ID();
}

void GameObject::UpdateCore(float inDeltaTime)
{
//	for (CBehavior* ptr : m_updateCompRepo)
//	{
//		if (ptr && ptr->IsEnable())
//		{
//			ptr->Update();
//		}
//	}
//
//	Update(inDeltaTime);
}


#include "pch.h"
#include "GameObject.h"
#include "CBehavior.h"

uint64 GameObject::ObjectIDCounter = 0;



void GameObject::InitalizeCore()
{
	Initialize();
}

void GameObject::UpdateCore(float inDeltaTime)
{
	for (CBehavior* ptr : m_updateCompRepo)
	{
		if (ptr)
		{
			ptr->Update();
		}
	}

	Update(inDeltaTime);
}

